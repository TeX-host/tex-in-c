#pragma once
#ifndef TEX_H
// 待整理的常量。应合并到 tex_constant.h
#define TEX_H
#include <setjmp.h> // setjmp
#include "global_const.h"
#include "tex_types.h"
#include "tex_constant.h"


// [p229#619]
#define movepast        13
#define finrule         14
#define nextp           15



// [p267#725]
#define donewithnoad        80
#define donewithnode        81
#define checkdimensions     82
#define deleteq             83


// [p384#1030] jmp label
// #define bigswitch            60
// #define appendnormalspace    120


// #1178
#define abovecode       0
#define overcode        1
#define atopcode        2
#define delimitedcode   3

// #1222
// enum ShortHandDef {};
#define chardefcode     0
#define mathchardefcode  1
#define countdefcode    2
#define dimendefcode    3
#define skipdefcode     4
#define muskipdefcode   5
#define toksdefcode     6

// #1291
#define showcode        0
#define showboxcode     1
#define showthecode     2
#define showlists       3

#define badfmt          6666
#define breakpoint      888


// inputln, [tex]
extern jmp_buf _JMP_global__end_of_TEX;
extern jmp_buf _JMP_global__final_end;

#endif // #ifndef TEX_H