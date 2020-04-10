#include "global_const.h" // tt_STAT
#pragma once
// 存放仅在 tex.c 中使用的宏
#ifndef _INC_TEX_ONLY
#define _INC_TEX_ONLY

#ifdef tt_STAT
/// #121
#define FREE_AVAIL(x)                                                          \
    (mem[(x)].hh.rh = avail, avail = (x), dynused -= charnodesize)
// end #define FREE_AVAIL(x)

/// #122
#define FAST_GET_AVAIL(x)                                                      \
    do {                                                                       \
        if (((x) = avail)) {                                                   \
            type(x) = charnodetype;                                            \
            avail = link(x);                                                   \
            link(x) = 0;                                                       \
            dynused += charnodesize;                                           \
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