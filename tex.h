#pragma once
#ifndef TEX_H
// 待整理的常量。应合并到 tex_constant.h
#define TEX_H
#include <setjmp.h> // setjmp
#include "tex_constant.h"


// inputln, [tex]
extern jmp_buf _JMP_global__end_of_TEX;
extern jmp_buf _JMP_global__final_end;

#endif // #ifndef TEX_H