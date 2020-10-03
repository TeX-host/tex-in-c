#pragma once
#ifndef MACROS_H
/// 全局常用的宏
#define MACROS_H
#include <assert.h> // static_assert


/// 取无符号类型的最大值
#define UMAXOF(t) ((t) ~(t)0)

#define charnodetype 0xfff
#undef BIG_CHARNODE
#define BIG_CHARNODE
#define BIG_NOAD

// 标记字符串序号
#define S(x) (x)

/// [p43#112]
#define qi(x) (x)
#define qo(x) (x)

#endif // #ifndef MACROS_H