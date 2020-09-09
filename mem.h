#ifndef INC_MEM_H

#define INC_MEM_H
#include "tex_types.h"
#include "global.h"


/// [p44#115] the #null pointer.
/// null ≡ min_halfword.
#define null 0

/// [p45#118] the `link` field of a memory word.
#define link(x) (mem[(x)].hh.rh)
/// [p45#118] the `info` field of a memory word.
#define info(x) (mem[(x)].hh.UU.lh)

/// [p46#124] the `link` of an empty variable-size node.
#define empty_flag      MAX_HALF_WORD
/// [p46#124] tests for empty node.
#define is_empty(x)    (link(x) == empty_flag)
/// [p46#124] the size field in empty variable-size nodes.
#define node_size       info
/// [p46#124] left link in doubly-linked list of empty nodes.
#define llink(x)        info(x+1)
/// [p46#124] right link in doubly-linked list of empty nodes.
#define rlink(x)        link(x+1)

#ifdef tt_STAT
/// [p45#121] single-word node liberation.
/// a one-word node is recycled by calling FREE_AVAIL.
#define FREE_AVAIL(x)   (link(x) = avail, avail = (x), dyn_used -= CHAR_NODE_SIZE)

/// [#122] avoid get_avail() if possible, to save time.
#define FAST_GET_AVAIL(x)               \
    do {                                \
        if (((x) = avail) != null) {    \
            type(x) = charnodetype;     \
            avail = link(x);            \
            link(x) = null;             \
            dyn_used += CHAR_NODE_SIZE; \
        } else {                        \
            (x) = get_avail();          \
        }                               \
    } while (0)
// end #define FAST_GET_AVAIL(x)
#else
// not defined(tt_STAT)

#define FREE_AVAIL(x) (link(x) = avail, avail = (x))

#define FAST_GET_AVAIL(x)           \
    do {                            \
        if (((x) = avail)) {        \
            type(x) = charnodetype; \
            avail = link(x);        \
            link(x) = 0;            \
        } else {                    \
            (x) = get_avail();      \
        }                           \
    } while (0)
// end #define FAST_GET_AVAIL(x)
#endif // #121,122: tt_STAT


// tex only
extern Pointer temp_ptr;
extern Integer var_used;
extern Integer dyn_used;
extern Pointer avail; // tex/expand
extern Pointer rover;

// tex, lexer
extern Pointer lo_mem_max;
extern Pointer hi_mem_min;
extern Pointer mem_end;

// tex|texmac|tex_inc, scan, 
extern MemoryWord mem[MEM_MAX - MEM_MIN + 1];


extern Pointer get_lo_mem_max(void);
extern Pointer get_avail(void);
extern void flush_list(HalfWord p);
extern HalfWord get_node(Integer s);
extern void free_node(Pointer p, HalfWord s);
extern void sort_avail(void);

#endif /* INC_MEM_H */