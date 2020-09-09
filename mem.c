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
    Pointer p;

    p = avail;
    if (p != 0) 
        avail = link(avail);
     else if (mem_end + CHAR_NODE_SIZE <= MEM_MAX) {
        p = mem_end + 1;
        mem_end += CHAR_NODE_SIZE;
    } else {
        p = hi_mem_min - 1;
        hi_mem_min -= CHAR_NODE_SIZE;
        if (hi_mem_min <= lo_mem_max) {
            runaway();
            // "main memory size"
            overflow(S(317), MEM_MAX - MEM_MIN + 1);
        }
    }
    type(p) = charnodetype; // ??? set_as_char_node
    link(p) = 0;
    #ifdef tt_STAT
        dyn_used += CHAR_NODE_SIZE;
    #endif // #120: tt_STAT
    return p;
} // #120: get_avail

/// [p46#123]: makes list of single-word nodes available
void flushlist(HalfWord p) {
    Pointer q, r;

    if (p == 0) return;
    r = p;
    do {
        q = r;
        r = link(r);            
        #ifdef tt_STAT
            dyn_used -= CHAR_NODE_SIZE;
        #endif // #123: tt_STAT
    } while (r != 0);
    link(q) = avail;
    avail = p;
} // #123: flushlist


/// [p47#125]
HalfWord getnode(long s) {
    HalfWord Result;
    Pointer p, q;
    long r, t;

_Lrestart:
    p = rover;
    do { /*127:*/
        q = p + nodesize(p);
        while (isempty(q)) {
            t = rlink(q);
            if (q == rover) rover = t;
            llink(t) = llink(q);
            rlink(llink(q)) = t;
            q += nodesize(q);
        }
        r = q - s;
        if (r > p + 1) { /*128:*/
            nodesize(p) = r - p;
            rover = p;
            goto _Lfound;
        }
        /*:128*/
        if (r == p) {
            if (rlink(p) != p) { /*129:*/
                rover = rlink(p);
                t = llink(p);
                llink(rover) = t;
                rlink(t) = rover;
                goto _Lfound;
            }
            /*:129*/
        }
        nodesize(p) = q - p; /*:127*/
        p = rlink(p);
    } while (p != rover);
    if (s == 1073741824L) {
        Result = MAX_HALF_WORD;
        goto _Lexit;
    }
    if (lo_mem_max + 2 < hi_mem_min) {
        if (lo_mem_max + 2 <= MEM_BOT + MAX_HALF_WORD) { /*126:*/
            if (hi_mem_min - lo_mem_max >= 1998)
                t = lo_mem_max + 1000;
            else
                t = lo_mem_max + (hi_mem_min - lo_mem_max) / 2 + 1;
            p = llink(rover);
            q = lo_mem_max;
            rlink(p) = q;
            llink(rover) = q;
            if (t > MEM_BOT + MAX_HALF_WORD) t = MEM_BOT + MAX_HALF_WORD;
            rlink(q) = rover;
            llink(q) = p;
            link(q) = emptyflag;
            nodesize(q) = t - lo_mem_max;
            lo_mem_max = t;
            link(lo_mem_max) = 0;
            info(lo_mem_max) = 0;
            rover = q;
            goto _Lrestart;
        }
        /*:126*/
    }
    // "main memory size"
    overflow(S(317), MEM_MAX - MEM_MIN + 1);
_Lfound:
    link(r) = 0;
    #ifdef tt_STAT
        var_used += s;
    #endif // #125: tt_STAT
    Result = r;
_Lexit:
    return Result;
} // #125: getnode

/// [p48#130]: variable-size node liberation
void freenode(Pointer p, HalfWord s) {
    Pointer q;

    nodesize(p) = s;
    link(p) = emptyflag;
    q = llink(rover);
    llink(p) = q;
    rlink(p) = rover;
    llink(rover) = p;
    rlink(q) = p;
    #ifdef tt_STAT
        var_used -= s;
    #endif // #130: tt_STAT
} // #130: freenode


#ifdef tt_INIT // #131,132
/// p49#131: sorts the available variable-size nodes by location
void sort_avail(void)
{   // used at #1311
    Pointer p, q, r,  // indices into mem
            oldrover; // initial rover setting

    p = getnode(1073741824L); // merge adjacent free areas
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
            while (rlink(q) < p)
                q = rlink(q);
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