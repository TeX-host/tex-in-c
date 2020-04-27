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
extern ASCIICode xord[256];

// funcs, [tex]
extern Char name_of_file[FILE_NAME_SIZE + 1];

// [tex], 仅在 print 中使用
extern Char xchr[256];
extern MemoryWord eqtb[eqtbsize - activebase + 1];
extern FILE* write_file[16];

// [tex] 仅在 lexer 中使用
/// [#116]
extern MemoryWord mem[MEM_MAX - MEM_MIN + 1];
extern Pointer lo_mem_max;
extern Pointer hi_mem_min;
extern Pointer mem_end;

#endif // #ifndef GLOBAL_H