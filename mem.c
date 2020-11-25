#include "lexer.h"  // [func] runaway, showtokenlist
#include "error.h"  // [func] overflow
#include "eqtb.h"   // [macro] ACTIVE_BASE, BOX_BASE
#include "texmath.h" // [macro] UNITY
#include "print.h"   // [func] printnl, print_int, print_char
#include "box.h"     // type
#include "align.h"
#include "linebreak.h"
#include "page.h"
#include "hyphen.h" // [var] hyphlist
#include "mem.h"


/** @addtogroup S115x132_P44x49
 *
 * #null=0 ≤ #MEM_MIN=0 
 * ≤ #MEM_BOT=0 < #lo_mem_max < #hi_mem_min < #MEM_TOP=3_000_000
 * ≤ #mem_end ≤ #MEM_MAX=3_000_000 .
 * @{
 */

Pointer temp_ptr;  ///< [#115] for occasional emergency use.
MemoryWord mem[MEM_MAX - MEM_MIN + 1]; ///< [#116] the big dynamic storage area.
Pointer lo_mem_max;  ///< [#116] the largest location of variable-size memory.
Pointer hi_mem_min;  ///< [#116] the smallest location of one-word memory.
Integer var_used, dyn_used; ///< [#117] how much memory is in use.
Pointer avail;    ///< [#118] head of the list of available one-word nodes.
Pointer mem_end;  ///< [#118] the last one-word node used in mem.
Pointer rover; ///< [#124] points to some node in the list of empties.


Pointer get_lo_mem_max(void) { return lo_mem_max; }


/// [p46#120]: single-word node allocation.
Pointer get_avail(void) {
    Pointer p; // the new node being got.

    p = avail; // get top location in the #avail stack.
    if (p != null) {
        avail = link(avail); // and pop it of
    } else if ((mem_end + CHAR_NODE_SIZE) <= MEM_MAX) {
        // or go into virgin territory
        p = mem_end + 1;
        mem_end += CHAR_NODE_SIZE;
    } else {
        p = hi_mem_min - 1;
        hi_mem_min -= CHAR_NODE_SIZE;
        if (hi_mem_min <= lo_mem_max) {
            //  if memory is exhausted, display possible runaway text
            runaway();
            // quit; all one-word nodes are busy
            //      "main memory size"
            overflow(S(317), MEM_MAX - MEM_MIN + 1);
        }
    }

    type(p) = charnodetype; // ??? set_as_char_node
    // provide an oft-desired initialization of the new node
    link(p) = null;
    #ifdef tt_STAT
        // maintain statistics
        dyn_used += CHAR_NODE_SIZE;
    #endif // #120: tt_STAT

    return p;
} // #120: get_avail

/// [p46#123]: makes list of single-word nodes available
void flush_list(HalfWord p) {
    Pointer q, r;

    if (p == 0) return;

    r = p;
    do {
        q = r;
        r = link(r);            
        #ifdef tt_STAT
            dyn_used -= CHAR_NODE_SIZE;
        #endif // #123: tt_STAT
    } while (r != null);
    // now q is the last node on the list
    link(q) = avail;
    avail = p;
} // #123: flush_list


/// [p47#125] variable-size node allocation
HalfWord get_node(Integer s) {
    Pointer p; // the node currently under inspection
    Pointer q; // the node physically after node p
    Integer r; //  the newly allocated node, or a candidate for this honor
    Integer t; // temporary register


_Lrestart:
    //  start at some free node in the ring
    p = rover;
    /// [#127] Try to allocate within node p and its physical successors,
    /// and goto found if allocation was possible.
    do {
        // find the physical successor
        q = p + node_size(p);
        // merge node p with node q
        while (is_empty(q)) {
            t = rlink(q);
            if (q == rover) rover = t;
            llink(t) = llink(q);
            rlink(llink(q)) = t;
            q += node_size(q);
        }
        r = q - s;
        // [#128] Allocate from the top of node p and goto found
        if (r > (p + 1)) {
            // store the remaining size
            node_size(p) = r - p;
            // start searching here next time
            rover = p;
            goto _Lfound;
        }

        // [##129] Allocate entire node p and goto found
        if (r == p && rlink(p) != p) {
            rover = rlink(p);
            t = llink(p);
            llink(rover) = t;
            rlink(t) = rover;

            goto _Lfound;
        }
        // reset the size in case it grew
        node_size(p) = q - p;

        // move to the next node in the ring
        p = rlink(p);
    } while (p != rover);

    if (s == 1073741824L) {
        return MAX_HALF_WORD;
    }

    if (   (lo_mem_max + 2) < hi_mem_min 
        && (lo_mem_max + 2) <= (MEM_BOT + MAX_HALF_WORD)) {
        /// [#126]  Grow more variable-size memory and goto restart
        if ((hi_mem_min - lo_mem_max) >= 1998) {
            t = lo_mem_max + 1000;
        } else {
            // lo_mem_max + 2 ≤ t < hi_mem_min
            t = lo_mem_max + 1 + (hi_mem_min - lo_mem_max) / 2;
        }

        p = llink(rover);
        q = lo_mem_max;
        rlink(p) = q;
        llink(rover) = q;

        if (t > (MEM_BOT + MAX_HALF_WORD)) {
            t = MEM_BOT + MAX_HALF_WORD;
        }

        rlink(q) = rover;
        llink(q) = p;
        link(q) = empty_flag;
        node_size(q) = t - lo_mem_max;

        lo_mem_max = t;
        link(lo_mem_max) = null;
        info(lo_mem_max) = null;
        rover = q;

        goto _Lrestart;
    } // [#126]

    // sorry, nothing satisfactory is left
    //      "main memory size"
    overflow(S(317), MEM_MAX - MEM_MIN + 1);

_Lfound:
    // this node is now nonempty
    link(r) = null;
    #ifdef tt_STAT
        var_used += s;
    #endif // #125: tt_STAT

    return r;
} // #125: get_node

/// [p48#130]: variable-size node liberation
void free_node(Pointer p, HalfWord s) {
    Pointer q; // llink (rover )

    // set both links
    node_size(p) = s;
    link(p) = empty_flag;
    q = llink(rover);
    llink(p) = q;
    rlink(p) = rover;

    // insert p into the ring
    llink(rover) = p;
    rlink(q) = p;
    #ifdef tt_STAT
        var_used -= s;
    #endif // #130: tt_STAT
} // #130: free_node


#ifdef tt_INIT // #131,132
/// p49#131: sorts the available variable-size nodes by location
/// used at #1311
void sort_avail(void) {
    Pointer p, q, r,  // indices into mem
            oldrover; // initial rover setting

    p = get_node(1073741824L); // merge adjacent free areas
    p = rlink(rover);
    rlink(rover) = MAX_HALF_WORD;
    oldrover = rover;

    while (p != oldrover) {
        /// #132: Sort p into the list starting at rover
        /// and ADVANCE p to rlink(p)
        if (p < rover) {
            q = p;
            p = rlink(q);
            rlink(q) = rover;
            rover = q;
        } else {
            q = rover;
            while (rlink(q) < p) {
                q = rlink(q);
            }
            r = rlink(p);
            rlink(p) = rlink(q);
            rlink(q) = p;
            p = r;
        } // if (p < rover) - else
    } // while (p != oldrover)

    p = rover;
    while (rlink(p) != MAX_HALF_WORD) {
        llink(rlink(p)) = p;
        p = rlink(p);
    } // while (rlink(p) != MAX_HALF_WORD)
    rlink(p) = rover;
    llink(rover) = p;
} // sort_avail
#endif // #131: tt_INIT
/** @}*/ // end group S115x132_P44x49


/** @addtogroup S162x172_P58x61
 * @{
 */
// [ #162~172: MEMORY LAYOUT ]
// [p95#165]
#ifdef tt_DEBUG
/// [#165] free: free cells, 以 byte(8) 分配，按位取用.
UChar free_cells[(MEM_MAX - MEM_MIN + 8) / 8];
/// [#165] previously free cells, 以 byte(8) 分配，按位取用.
UChar was_free[(MEM_MAX - MEM_MIN + 8) / 8];
/// [#165] previous #mem_end, #lo_mem_max, and #hi_mem_min.
Pointer was_mem_end, was_lo_max, was_hi_min;
/// [#165] do we want to check memory constantly?
Boolean panicking;
#endif   // #165: tt_DEBUG

/// p60#167
#ifdef tt_DEBUG
void check_mem(Boolean printlocs) {
    Pointer p, q; // current locations of interest in mem
    Boolean clobbered; // is something amiss?
    HalfWord FORLIM;

    for (p = MEM_MIN; p <= lo_mem_max; p++) {
        // free_cells[p - MEM_MIN] = 0;
        P_clrbits_B(free_cells, p - MEM_MIN, 0, 3);
    }
    for (p = hi_mem_min; p <= mem_end; p++) {
        // free_cells[p - MEM_MIN] = 0;
        P_clrbits_B(free_cells, p - MEM_MIN, 0, 3);
    }

    /// [#168] Check single-word avail list
    p = avail;
    q = null;
    clobbered = false;
    while (p != 0) {
        if (p > mem_end || p < hi_mem_min) {
            clobbered = true;
        } else {
            if (P_getbits_UB(free_cells, p - MEM_MIN, 0, 3)) clobbered = true;
        }
        if (clobbered) {
            // " AVAIL list clobbered at "
            printnl(S(318));
            print_int(q);

            break;
        }
        P_putbits_UB(free_cells, p - MEM_MIN, 1, 0, 3);
        q = p;
        p = link(q);
    } // while (p != 0)

    /// [#169] Check variable-size avail list
    p = rover;
    q = 0;
    clobbered = false;
    do {
        if (p >= lo_mem_max || p < MEM_MIN) {
            clobbered = true;
        } else if ((rlink(p) >= lo_mem_max) | (rlink(p) < MEM_MIN)) {
            clobbered = true;
        } else if ( (!is_empty(p)) 
                    || (node_size(p) < 2) 
                    || ((p + node_size(p)) > lo_mem_max) 
                    || (llink(rlink(p)) != p)) {
            clobbered = true;
        }
        if (clobbered) {
            printnl(S(319));
            print_int(q);
            goto _L_checkmem_done;
        }
        // mark all locations free
        FORLIM = p + node_size(p);
        for (q = p; q < FORLIM; q++) {
            if (P_getbits_UB(free_cells, q - MEM_MIN, 0, 3)) {
                printnl(S(320));
                print_int(q);
                goto _L_checkmem_done;
            }
            P_putbits_UB(free_cells, q - MEM_MIN, 1, 0, 3);
        }
        q = p;
        p = rlink(p);
    } while (p != rover);
_L_checkmem_done:

    /// [#170] Check flags of unavailable nodes
    p = MEM_MIN;
    while (p <= lo_mem_max) {
        if (is_empty(p)) {
            printnl(S(321));
            print_int(p);
        }
        while ((p <= lo_mem_max) & (!P_getbits_UB(free_cells, p - MEM_MIN, 0, 3)))
            p++;
        while ((p <= lo_mem_max) &   P_getbits_UB(free_cells, p - MEM_MIN, 0, 3))
            p++;
    }

    if (printlocs) {
        /// [#171] Print newly busy locations
        printnl(S(322)); // "New busy locs:"
        FORLIM = lo_mem_max;
        for (p = MEM_MIN; p <= lo_mem_max; p++) {
            if (   (!P_getbits_UB(free_cells, p - MEM_MIN, 0, 3)) 
                && ((p > was_lo_max) 
                    || P_getbits_UB(was_free, p - MEM_MIN, 0, 3))
                ) {
                print_char(' ');
                print_int(p);
            }
        }
        for (p = hi_mem_min; p <= mem_end; p++) {
            if (   (!P_getbits_UB(free_cells, p - MEM_MIN, 0, 3)) 
                && (p < was_hi_min 
                    || p > was_mem_end 
                    || P_getbits_UB(was_free, p - MEM_MIN, 0, 3))) {
                print_char(' ');
                print_int(p);
            }
        }
    } // if (printlocs)

    for (p = MEM_MIN; p <= lo_mem_max; p++) {
        P_clrbits_B(was_free, p - MEM_MIN, 0, 3);
        P_putbits_UB(was_free,
            p - MEM_MIN, P_getbits_UB(free_cells, p - MEM_MIN, 0, 3), 0, 3);
    }
    for (p = hi_mem_min; p <= mem_end; p++) {
        P_clrbits_B(was_free, p - MEM_MIN, 0, 3);
        P_putbits_UB(was_free, 
            p - MEM_MIN, P_getbits_UB(free_cells, p - MEM_MIN, 0, 3), 0, 3);
    }

    was_mem_end = mem_end;
    was_lo_max = lo_mem_max;
    was_hi_min = hi_mem_min;
} // #164: check_mem

/// p61#172
void search_mem(Pointer p) {
    Integer q;

    for (q = MEM_MIN; q <= lo_mem_max; q++) {
        if (link(q) == p) {
            printnl(S(323)); // "LINK("
            print_int(q);
            print_char(')');
        }
        if (info(q) == p) {
            printnl(S(324)); // "INFO("
            print_int(q);
            print_char(')');
        }
    }
    for (q = hi_mem_min; q <= mem_end; q++) {
        if (link(q) == p) {
            printnl(S(323)); // "LINK("
            print_int(q);
            print_char(')');
        }
        if (info(q) == p) {
            printnl(S(324)); // "INFO("
            print_int(q);
            print_char(')');
        }
    }

    /// [#255] Search eqtb for equivalents equal to p
    for (q = ACTIVE_BASE; q <= (BOX_BASE + 255); q++) {
        if (equiv(q) == p) {
            printnl(S(325)); // "EQUIV("
            print_int(q);
            print_char(')');
        }
    }
    /// [#285] Search save stack for equivalents that point to p
    if (save_ptr > 0) {
        for (q = 0; q < save_ptr; q++) {
            if (equiv_field(save_stack[q]) == p) {
                printnl(S(326)); // "SAVE("
                print_int(q);
                print_char(')');
            }
        }
    }
    /// [#933] Search hyph list for pointers to p
    for (q = 0; q <= HYPH_SIZE; q++) {
        if (hyphlist[q] == p) {
            printnl(S(327)); // "HYPH("
            print_int(q);
            print_char(')');
        }
    }
} // #172: search_mem
#endif // #167,172: tt_DEBUG

/// [#164]
void mem_init_once() {
    size_t k;

    for (k = MEM_BOT + 1; k <= lomemstatmax; k++) {
        mem[k - MEM_MIN].sc = 0; // all glue dimensions are zeroed
    }

    k = MEM_BOT;
    while (k <= lomemstatmax) {
        // set first words of glue specifications
        gluerefcount(k) = null + 1;
        stretchorder(k) = NORMAL;
        shrinkorder(k) = NORMAL;
        k += gluespecsize;
    }

    stretch(filglue) = UNITY;
    stretchorder(filglue) = FIL;
    stretch(fillglue) = UNITY;
    stretchorder(fillglue) = FILL;
    stretch(ssglue) = UNITY;
    stretchorder(ssglue) = FIL;
    shrink(ssglue) = UNITY;
    shrinkorder(ssglue) = FIL;
    stretch(filnegglue) = -UNITY;
    stretchorder(filnegglue) = FIL;

    // now initialize the dynamic memory
    rover = lomemstatmax + 1;
    link(rover) = empty_flag;

    node_size(rover) = 1000; // which is a 1000-word available node
    llink(rover) = rover;
    rlink(rover) = rover;
    lo_mem_max = rover + 1000;
    link(lo_mem_max) = null;
    info(lo_mem_max) = null;

    for (k = himemstatmin; k <= MEM_TOP; k++) {
        // clear list heads
        mem[k - MEM_MIN].sc = 0;
        type(k) = charnodetype;
    }

    // Initialize the special list heads and constant nodes
    {
        /// #790
        info(omittemplate) = endtemplatetoken; // link (omit_template) = null
        /// #797
        link(endspan) = MAX_QUARTER_WORD + 1;
        info(endspan) = null;
        /// #820
        type(lastactive) = hyphenated;
        linenumber(lastactive) = MAX_HALF_WORD;
        // the subtype is never examined by the algorithm
        subtype(lastactive) = 0;

        page_builder_init_once();
    }

    /// p59#164
    avail = null;
    mem_end = MEM_TOP;
    hi_mem_min = himemstatmin; // initialize the one-word memory
    var_used = lomemstatmax - MEM_BOT + 1;
    dyn_used = himemstatusage; // initialize statistics
} /* mem_init_once */

/// [p59#166]
void mem_var_init() {
#ifdef tt_DEBUG
    /// indicate that everything was previously free.
    was_mem_end = MEM_MIN;
    was_lo_max = MEM_MIN;
    was_hi_min = MEM_MAX;
    panicking = false;
#endif // [#166]: tt_DEBUG
} /* mem_var_init */
/** @}*/ // end group S162x172_P58x61