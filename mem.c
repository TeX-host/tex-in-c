#include "lexer.h"  // [func] runaway, showtokenlist
#include "texmac.h" // [macro] type
#include "error.h"  // [func] overflow
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