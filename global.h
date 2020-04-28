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
extern ListStateRecord cur_list;
extern UInt16 shown_mode;
extern Pointer curmark[splitbotmarkcode - topmarkcode + 1];
extern Boolean OK_to_interrupt;
extern Integer interrupt;
extern Boolean deletions_allowed;
extern Pointer parloc;
extern HalfWord partoken;
extern Boolean force_eof;
extern char longstate;
extern SmallNumber curif;
extern Integer skipline;
extern StrNumber help_line[6];
extern UChar interaction;
extern Pointer curalign;

// [tex], expand
extern Integer curval;
extern char curvallevel;
extern SmallNumber radix;
extern GlueOrd curorder;
extern Pointer condptr;
extern char iflimit;
extern Integer ifline;
extern Boolean name_in_progress;
extern Pointer avail;
extern Integer dyn_used;

extern char readopen[17];
extern StrNumber job_name;

#endif // #ifndef GLOBAL_H