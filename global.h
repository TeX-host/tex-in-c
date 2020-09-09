#pragma once
#ifndef GLOBAL_H
// 全局变量
#define GLOBAL_H
#include <setjmp.h> // setjmp
#include "str.h"    // StrNumber
#include "scan.h"   // [macro] EQTB_SIZE

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
extern TextChar xchr[256];
extern FILE* write_file[16];

// [tex] 仅在 lexer 中使用
extern ListStateRecord cur_list;
extern UInt16 shown_mode;
extern Pointer curmark[splitbotmarkcode - topmarkcode + 1];
extern Boolean OK_to_interrupt;
extern Integer interrupt;
extern Boolean deletions_allowed;
extern Pointer parloc;
extern HalfWord partoken;
extern Boolean force_eof;
extern SmallNumber curif;
extern Integer skipline;
extern StrNumber help_line[6];
extern UChar interaction;
extern Pointer curalign;

// [tex], expand
extern Pointer condptr;
extern char iflimit;
extern Integer ifline;
extern Boolean name_in_progress;

extern char readopen[17];
extern StrNumber job_name;

// [tex] scan
extern ListStateRecord nest[NEST_SIZE + 1];
extern UChar nest_ptr;
extern Integer deadcycles;
extern char pagecontents;
extern Scaled pagesofar[8];
extern Integer lastpenalty, insertpenalties;
extern Boolean outputactive;
extern Integer lastbadness;
extern Scaled lastkern;
extern Pointer lastglue;


// [tex], error
extern enum ErrorLevel history;
extern SChar errorcount;
extern UChar help_ptr;
extern Boolean use_err_help;
extern Boolean log_opened;

// [tex], eqtb
extern Integer depth_threshold;
extern Integer breadth_max;

// [tex], mem
extern MemoryWord savestack[SAVE_SIZE + 1];
extern Pointer hyphlist[HYPH_SIZE + 1];
extern UInt16 saveptr;

#endif // #ifndef GLOBAL_H