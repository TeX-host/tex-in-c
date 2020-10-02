#pragma once
#ifndef GLOBAL_H
// 全局变量
#define GLOBAL_H
#include <stdio.h>
#include <setjmp.h> // setjmp
#include "tex_types.h"

// inputln, [tex]
extern jmp_buf _JMP_global__end_of_TEX;
extern jmp_buf _JMP_global__final_end;

// [tex] 仅在 lexer 中使用
extern Pointer parloc;
extern HalfWord partoken;
extern Boolean force_eof;

extern char readopen[17];

// eqtb
extern Boolean use_independence_date;

// main-ctrl
extern FILE* readfile[16];

#endif // #ifndef GLOBAL_H