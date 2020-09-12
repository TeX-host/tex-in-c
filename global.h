#pragma once
#ifndef GLOBAL_H
// 全局变量
#define GLOBAL_H
#include <setjmp.h> // setjmp
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
extern ASCIICode xord[256];

// [tex], 仅在 print 中使用
extern TextChar xchr[256];
extern FILE* write_file[16];

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
extern char pagecontents;
extern Scaled pagesofar[8];
extern Integer lastpenalty, insertpenalties;
extern Boolean outputactive;
extern Scaled lastkern;
extern Pointer lastglue;


// [tex], error
extern Boolean log_opened;

// [tex], eqtb
extern Integer depth_threshold;
extern Integer breadth_max;

// [tex], mem
extern Pointer hyphlist[HYPH_SIZE + 1];

// [tex] lexer
extern Pointer pagetail;

// eqtb
extern Boolean use_independence_date;

// linebreak
extern TwoHalves trie[TRIE_SIZE + 1];
extern char hyf[65];
extern Boolean initlig, initlft;
extern SmallNumber hyphenpassed;
extern short hc[66];
extern int hn;
extern InternalFontNumber hf;
extern short hu[64];
extern Integer hyfchar;
extern ASCIICode curlang, initcurlang;
extern StrNumber hyphword[HYPH_SIZE + 1];
extern Integer lhyf, rhyf, initlhyf, initrhyf;
extern SmallNumber hyfdistance[TRIE_OP_SIZE];
extern SmallNumber hyfnum[TRIE_OP_SIZE];
extern QuarterWord hyfnext[TRIE_OP_SIZE];
extern short opstart[256];
extern HalfWord hyfbchar;

#endif // #ifndef GLOBAL_H