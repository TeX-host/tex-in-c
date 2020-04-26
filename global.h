#pragma once
#ifndef GLOBAL_H
// 全局变量
#define GLOBAL_H
#include <setjmp.h> // setjmp
#include "str.h"    // StrNumber

// funcs, inputln, [tex]
extern ASCIICode buffer[];
extern UInt16 last;
extern UInt16 first;

// inputln, [tex]
extern UInt16 max_buf_stack;
extern jmp_buf _JMP_global__final_end;
extern StrNumber format_ident;
extern InStateRecord cur_input;
extern ASCIICode xord[256];

// funcs, [tex]
extern Char name_of_file[FILE_NAME_SIZE + 1];

// [tex], print
Char xchr[256];
MemoryWord eqtb[eqtbsize - activebase + 1];
FILE* write_file[16];

#endif // #ifndef GLOBAL_H