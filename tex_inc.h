#pragma once
#ifndef _INC_TEX_ONLY
/// 存放仅在 tex.c 中使用的宏
#define _INC_TEX_ONLY
#include "global_const.h" // tt_STAT


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