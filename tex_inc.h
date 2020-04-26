#pragma once
#ifndef _INC_TEX_ONLY
/// 存放仅在 tex.c 中使用的宏
#define _INC_TEX_ONLY
#include "global_const.h" // tt_STAT


#ifdef tt_STAT
/// [p45#121] single-word node liberation.
/// a one-word node is recycled by calling FREE_AVAIL.
#define FREE_AVAIL(x) \
    (mem[(x)].hh.rh = avail, avail = (x), dyn_used -= CHAR_NODE_SIZE)
// end #define FREE_AVAIL(x)

/// [#122] avoid get_avail() if possible, to save time.
#define FAST_GET_AVAIL(x)               \
    do {                                \
        if (((x) = avail)) {            \
            type(x) = charnodetype;     \
            avail = link(x);            \
            link(x) = 0;                \
            dyn_used += CHAR_NODE_SIZE; \
        } else {                        \
            (x) = get_avail();          \
        }                               \
    } while (0)
// end #define FAST_GET_AVAIL(x)
#else
// not defined(tt_STAT)

#define FREE_AVAIL(x) (mem[(x)].hh.rh = avail, avail = (x))

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


/// [p145#371]
#define STORE_NEW_TOKEN(p, x) \
    do {                      \
        int q = get_avail();  \
        link(p) = q;          \
        info(q) = (x);        \
        p = q;                \
    } while (0)
// end #define STORE_NEW_TOKEN(p, x)

/// [p145#371]
#define FAST_STORE_NEW_TOKEN(p, x) \
    do {                           \
        int q;                     \
        FAST_GET_AVAIL(q);         \
        link(p) = q;               \
        info(q) = (x);             \
        p = q;                     \
    } while (0)
// end #define FAST_STORE_NEW_TOKEN(p, x)

#endif // _INC_TEX_ONLY