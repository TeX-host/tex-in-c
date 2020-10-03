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

#ifdef BIG_CHARNODE
#define CHAR_NODE_SIZE 2
#define font_ligchar(x) type(ligchar(x))
#define character_ligchar(x) subtype(ligchar(x))
#else
#define CHAR_NODE_SIZE 1
#define font_ligchar(x) font(ligchar(x))
#define character_ligchar(x) character(ligchar(x))
#endif // #ifdef BIG_CHARNODE

// 标记字符串序号
#define S(x) (x)

// fonts, str, tex
#define pget(x) fread(&x, 8, 1, fmtfile)
#define pput(x) fwrite(&x, 8, 1, fmtfile)

/// [p43#112]
#define qi(x) (x)
#define qo(x) (x)

#endif // #ifndef MACROS_H