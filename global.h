#pragma once
#ifndef GLOBAL_H
// 全局变量
#define GLOBAL_H
#include <setjmp.h> // setjmp
#include "charset.h" // [type] ASCIICode
#include "str.h"    // StrNumber
#include "scan.h"   // [macro] EQTB_SIZE
#include "fonts.h"  // InternalFontNumber

// funcs, inputln, [tex]
extern ASCIICode buffer[];
extern UInt16 last;
extern UInt16 first;

// inputln, [tex]
extern UInt16 max_buf_stack;
extern jmp_buf _JMP_global__final_end;
extern StrNumber format_ident;

// [tex] 仅在 lexer 中使用
extern ListStateRecord cur_list;
extern Boolean OK_to_interrupt;
extern Integer interrupt;
extern Pointer parloc;
extern HalfWord partoken;
extern Boolean force_eof;
extern SmallNumber curif;
extern Integer skipline;

// [tex], expand
extern Pointer condptr;
extern char iflimit;
extern Integer ifline;
extern Boolean name_in_progress;

extern char readopen[17];
extern StrNumber job_name;

// [tex] scan
extern ListStateRecord nest[NEST_SIZE + 1];
extern Integer deadcycles;

// [tex], error
extern Boolean log_opened;

// [tex], eqtb
extern Integer depth_threshold;
extern Integer breadth_max;

// [tex], mem
extern Pointer hyphlist[HYPH_SIZE + 1];

// eqtb
extern Boolean use_independence_date;

// main-ctrl
extern FILE* readfile[16];


#endif // #ifndef GLOBAL_H