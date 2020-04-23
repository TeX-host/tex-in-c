#include "global_const.h" // tt_STAT
#pragma once
// 存放仅在 tex.c 中使用的宏
#ifndef _INC_TEX_ONLY
#define _INC_TEX_ONLY

// p24#54: On-line and off-line printing
// enum Selector = [16, 21]
//     use by `selector`
enum Selector {
    NO_PRINT = 16, // doesn’t print at all
    TERM_ONLY,     // prints only on the terminal
    LOG_ONLY,      // prints only on the transcript file
    TERM_AND_LOG,  // normal `selector` setting
    // puts output into a cyclic buffer that is used by the show context routine
    PSEUDO,
    // appends the output to the current string in the string pool
    NEW_STRING,    
};
// highest selector setting
// NOTE: _not_use_, only used in assert
#define MAX_SELECTOR NEW_STRING 


#ifdef tt_STAT
/// #121
#define FREE_AVAIL(x)                                                          \
    (mem[(x)].hh.rh = avail, avail = (x), dyn_used -= CHAR_NODE_SIZE)
// end #define FREE_AVAIL(x)

/// #122
#define FAST_GET_AVAIL(x)                                                      \
    do {                                                                       \
        if (((x) = avail)) {                                                   \
            type(x) = charnodetype;                                            \
            avail = link(x);                                                   \
            link(x) = 0;                                                       \
            dyn_used += CHAR_NODE_SIZE;                                           \
        } else {                                                               \
            (x) = get_avail();                                                 \
        }                                                                      \
    } while (0)
// end #define FAST_GET_AVAIL(x)
#else
// not defined(tt_STAT)

#define FREE_AVAIL(x) (mem[(x)].hh.rh = avail, avail = (x))

#define FAST_GET_AVAIL(x)                                                      \
    do {                                                                       \
        if (((x) = avail)) {                                                   \
            type(x) = charnodetype;                                            \
            avail = link(x);                                                   \
            link(x) = 0;                                                       \
        } else {                                                               \
            (x) = get_avail();                                                 \
        }                                                                      \
    } while (0)
// end #define FAST_GET_AVAIL(x)
#endif // #121,122: tt_STAT


/// p145#371
#define STORE_NEW_TOKEN(p, x)                                                  \
    do {                                                                       \
        int q = get_avail();                                                   \
        link(p) = q;                                                           \
        info(q) = (x);                                                         \
        p = q;                                                                 \
    } while (0)
// end #define STORE_NEW_TOKEN(p, x)
#define FAST_STORE_NEW_TOKEN(p, x)                                             \
    do {                                                                       \
        int q;                                                                 \
        FAST_GET_AVAIL(q);                                                     \
        link(p) = q;                                                           \
        info(q) = (x);                                                         \
        p = q;                                                                 \
    } while (0)
// end #define FAST_STORE_NEW_TOKEN(p, x)

#endif // _INC_TEX_ONLY