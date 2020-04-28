#pragma once
#ifndef INC_TEX_HEADER
// 存放 tex.c 的头部。
// 导入库、其他模块，声明 tex.c 的内部静态变量
#define INC_TEX_HEADER

#include <stdio.h>  // FILE
#include <stdarg.h> // va_start, va_arg, va_end,
#include <stdlib.h> // labs, abs, exit, EXIT_SUCCESS
#include <string.h> // memcpy
#include <math.h>   // fabs

#include "global_const.h"
#include "tex_inc.h"
#include "texmath.h"
#include "tex.h" // [export]
#include "str.h"
#include "texmac.h"
#include "macros.h"
#include "funcs.h"
#include "global.h"
#include "printout.h"
#include "fonts.h"
#include "inputln.h" // [func] inputln
#include "dviout.h"
#include "pure_func.h"  // [func] 导入纯函数
#include "print.h"      // 打印函数
#include "lexer.h"      // lexer
#include "expand.h"     // expand
#include "texfunc.h"    // [export]


/*
 * [ #13: Global variables ]
 *
 * 真正的 global 放在 global.h 中。
 * 以下变量一般为 tex.h 中的全局变量。
 * 已更改为函数内局部变量的用 [00] 标出。
 *
 * 共 97 个
 *  [13],  20,  26,  30,  32, [39], [50],  54,  73,  76,
 *   79,  96, 104, 115, 116, 117, 118, 124, 165, 173,
 *  181, 213, 246, 253, 256, 271, 286, 297, 301, 304,
 *  305, 308, 309, 310, 333, 361, 382, 387, 388, 410,
 *  438, 447, 480, 489, 493, 512, 513, 520, 527, 532,
 *  539, 549, 550, 555, 592, 595, 605, 616, 646, 647,
 *  661, 684, 719, 724, 764, 770, 814, 821, 823, 825,
 *  828, 833, 839, 847, 872, 892, 900, 905, 907, 921,
 *  926, 943, 947, 950, 971, 980, 982, 989, 1032, 1074,
 *  1266, 1281, 1299, 1305, 1331, 1342, 1345
 *
 * Notes:
 *  locals: 13-bad,
 *  404 not found: 39-str*, 50-poolfile,
 */

/// [#20]
ASCIICode xord[256];   // specifies conversion of input characters
Char xchr[256]; // specifies conversion of output characters

/// [#26]: on some systems this may be a record variable
Char name_of_file[FILE_NAME_SIZE + 1];
// #26: this many characters are actually relevant in `name_of_file`
// (the rest are blank)
Static UInt16 namelength;

/// [#30]
ASCIICode buffer[BUF_SIZE + 1]; // lines of characters being read
UInt16 first;                  // the first unused position in `buffer`
UInt16 last;                   // end of the line just input to `buffer`
UInt16 max_buf_stack;          // largest index used in `buffer`

/// [#32]: 直接使用 stdin, stdout, stderr
#if !(defined(stdin) || defined(stdout) || defined(stderr))
Static FILE *termin = NULL, *termout = NULL;
#endif


/*
 * [ #72~98: REPORTING ERRORS ]
 *
 */

// [p30#73] current level of interaction
// interaction = [BATCH_MODE=0, ERROR_STOP_MODE=3]
// [REPORTING ERRORS]
UChar interaction;

/// [p31#76]
Boolean deletions_allowed; // is it safe for error to call get token?
Static Boolean set_box_allowed;   // is it safe to do a \setbox assignment?
// has the source input been clean so far?
// [SPOTLESS, FATAL_ERROR_STOP]
Static enum ErrorLevel history;
// the number of scrolled errors since the last paragraph ended
// errorcount = [-1, 100]
Static SChar errorcount;

/// [#79]
StrNumber help_line[6]; // helps for the `nexterror`
Static UChar help_ptr;         // the number of help lines present
Static Boolean use_err_help;   // should the `errhelp` list be shown?

/// [#96]
Integer interrupt;       // should TeX pause for instructions?
Boolean OK_to_interrupt; // should interrupts be observed?

/// [ #115~132: DYNAMIC MEMORY ALLOCATION ]
/// [#115]
Static Pointer temp_ptr; // for occasional emergency use
/// [#116]
MemoryWord mem[MEM_MAX - MEM_MIN + 1]; // the big dynamic storage area
Pointer lo_mem_max; // the largest location of variable-size memory
Pointer hi_mem_min; // the smallest location of one-word memory
/// [#117]
Integer var_used, dyn_used; // how much memory is in use
/// [#118]
Pointer avail;   // head of the list of available one-word nodes
Pointer mem_end; // the last one-word node used in mem
/// [#124]
Static Pointer rover; // points to some node in the list of empties

/// [ #162~172: MEMORY LAYOUT ]
/// [p95#165]
#ifdef tt_DEBUG
// 以 byte(8) 分配，按位取用
Static UChar free_cells[(MEM_MAX - MEM_MIN + 8) / 8]; // free: free cells
Static UChar was_free[(MEM_MAX - MEM_MIN + 8) / 8];   // previously free cells
// previous `mem_end`, `lo_mem_max`, and `hi_mem_min`
Static Pointer was_mem_end, was_lo_max, was_hi_min;
Static Boolean panicking; // do we want to check memory constantly?
#endif // #165: tt_DEBUG

/// [ #173~198: DISPLAYING BOXES ]
Static Integer font_in_short_display; // an internal font number
/// [#181]
Static Integer depth_threshold; // maximum nesting depth in box displays
// maximum number of items shown at the same list level
Static Integer breadth_max;

/// [ #211~219: PART 16: THE SEMANTIC NEST ]
Static ListStateRecord nest[nestsize + 1]; // [0, nestsize=40]
Static UChar nest_ptr;                     // first unused location of nest
Static UChar max_nest_stack;               // maximum of nest_ptr when pushing
ListStateRecord cur_list;           // the "top" semantic state
UInt16 shown_mode; // most recent mode shown by \tracingcommands

/// [ #220~255: THE TABLE OF EQUIVALENTS ]
Static UChar diag_oldsetting; // [0, MAX_SELECTOR=21]
static_assert(UMAXOF(UChar) >= MAX_SELECTOR,
              "diag_oldsetting = [0, MAX_SELECTOR=21]");

/// #253
MemoryWord eqtb[eqtbsize - activebase + 1]; // equivalents table
// store the eq level information
Static QuarterWord xeqlevel[eqtbsize - intbase + 1];
/// [#256]
Static TwoHalves hash[UNDEFINED_CONTROL_SEQUENCE - hashbase]; // hash table
Static Pointer hash_used; // allocation pointer for hash
// ? no_new_control_sequence // are new identifiers legal?
Static Integer cs_count; // total number of known identifiers

/// [ #268~288: SAVING AND RESTORING EQUIVALENTS ]
/// [#271]
Static MemoryWord savestack[SAVE_SIZE + 1];
Static UInt16 saveptr; // first unused entry on save stack
Static UInt16 maxsavestack; // maximum usage of save stack
Static QuarterWord curlevel; // current nesting level for groups
Static GroupCode curgroup;   // current group type
Static UInt16 curboundary;   // where the current level begins
// [#286] if nonzero, 
// this magnification should be used henceforth
Static Integer magset;

/// [ #332~365：PART 24: GETTING THE NEXT TOKEN ]
// [#333] location of ‘\par’ in eqtb
Pointer parloc;
// [#333] token representing ‘\par’
HalfWord partoken;
// [#361] should the next \input be aborted early?
Boolean force_eof;

/// [ #366~401: PART 25: EXPANDING THE NEXT TOKEN ]
// [#382] token lists for marks
Pointer curmark[splitbotmarkcode - topmarkcode + 1];
// [#387] governs the acceptance of \par
char longstate;

/// [ #402~463: PART 26: BASIC SCANNING SUBROUTINES ]
// [#410] curval
Integer curval;
// [#410] the “level” of this value
char curvallevel;
/*438:*/
SmallNumber radix;
/*:438*/
/*447:*/
GlueOrd curorder;
/*:447*/

/// [ #464~486: PART 27: BUILDING TOKEN LISTS ]
/*480:*/
Static FILE* readfile[16];
char readopen[17];
/*:480*/

/*512:*/
Static StrNumber curname, curarea, curext;
/*:512*/
/*513:*/
Static StrNumber extdelimiter;
/*:513*/
/*520:*/
Static Char TEXformatdefault[FORMAT_DEFAULT_LENGTH]; /*:520*/
/*527:*/
Boolean name_in_progress;
StrNumber job_name;
Static Boolean log_opened; /*:527*/
/*532:*/
Static StrNumber output_file_name, logname; 
/*:532*/


/*555:*/
Static FourQuarters nullcharacter;
/*:555*/


// [#592]
Static Scaled maxh, maxv;
Static Integer totalpages, maxpush, deadcycles;
Static Boolean doingleaders;
Static Integer lq, lr;
Static Scaled ruleht, ruledp, rulewd;

// #616
Static Scaled dvih = 0, dviv = 0, // a DVI reader program thinks we are here
    curh, curv,                   // TeX thinks we are here
    curmu;
#define synchh()                                                               \
    do {                                                                       \
        dvih = synch_h(curh, dvih);                                            \
    } while (0)
#define synchv()                                                               \
    do {                                                                       \
        dviv = synch_v(curv, dviv);                                            \
    } while (0)
Static InternalFontNumber dvif = NULL_FONT; // the current font
Static Integer curs; // current depth of output box nesting, initially −1


/*646:*/
Static Scaled totalstretch[FILLL - NORMAL + 1], totalshrink[FILLL - NORMAL + 1];
Static Integer lastbadness;
/*:646*/
/*647:*/
Static Pointer adjusttail;
/*:647*/
/*661:*/
Static Integer packbeginline;
/*:661*/


/*684:*/
Static TwoHalves emptyfield;
Static FourQuarters nulldelimiter;
/*:684*/


/*719:*/
Static Pointer curmlist;
Static SmallNumber curstyle, cursize;
Static Boolean mlistpenalties;
/*:719*/
/*724:*/
Static InternalFontNumber curf;
Static QuarterWord curc;
Static FourQuarters curi;
/*:724*/



/*770:*/
Pointer curalign;
Static Pointer curspan, curloop, alignptr, curhead, curtail;
/*:770*/


/*814:*/
Static Pointer justbox;
/*:814*/
/*821:*/
Static Pointer passive, printednode;
Static HalfWord passnumber;
/*:821*/
/*823:*/
Static Scaled activewidth[6];
Static Scaled curactivewidth[6];
Static Scaled background[6];
Static Scaled breakwidth[6];
/*:823*/
/*825:*/
Static Boolean noshrinkerroryet, secondpass, finalpass;
/*:825*/
/*828:*/
Static Pointer curp;
Static Integer threshold;
/*:828*/
/*833:*/
Static Integer minimaldemerits[tightfit - veryloosefit + 1];
Static Integer minimumdemerits;
Static Pointer bestplace[tightfit - veryloosefit + 1];
Static HalfWord bestplline[tightfit - veryloosefit + 1];
/*:833*/
/*839:*/
Static Scaled discwidth, firstwidth, secondwidth, firstindent, secondindent;
/*:839*/
/*847:*/
Static HalfWord easyline, lastspecialline;
/*:847*/


/*872:*/
Static Pointer bestbet, ha, hb, initlist, curq, ligstack;
Static Integer fewestdemerits;
Static HalfWord bestline;
Static Integer actuallooseness, linediff;
/*:872*/


/*892:*/
Static short hc[66];
Static /* SmallNumber */ int hn;
Static InternalFontNumber hf;
Static short hu[64];
Static Integer hyfchar;
Static ASCIICode curlang, initcurlang;
Static Integer lhyf, rhyf, initlhyf, initrhyf;
Static HalfWord hyfbchar;
/*:892*/


/*900:*/
Static char hyf[65];
Static Boolean initlig, initlft;
/*:900*/
/*905:*/
Static SmallNumber hyphenpassed;
/*:905*/
/*907:*/
Static HalfWord curl, curr;
Static Boolean ligaturepresent, lfthit, rthit;
/*:907*/


/*921:*/
Static TwoHalves trie[TRIE_SIZE + 1];
Static SmallNumber hyfdistance[trieopsize];
Static SmallNumber hyfnum[trieopsize];
Static QuarterWord hyfnext[trieopsize];
Static short opstart[256];
/*:921*/
/*926:*/
Static StrNumber hyphword[HYPH_SIZE + 1];
Static Pointer hyphlist[HYPH_SIZE + 1];
Static HyphPointer hyphcount;
/*:926*/

/// [ #942~966: PART 43: INITIALIZING THE HYPHENATION TABLES ]
/// #943, 947, 950
#ifdef tt_INIT
/// #943
// trie op codes for quadruples
Static short trieophash[trieopsize + trieopsize + 1];
// largest opcode used so far for this language
Static QuarterWord trieused[256];
// language part of a hashed quadruple
Static ASCIICode trieoplang[trieopsize];
// opcode corresponding to a hashed quadruple
Static QuarterWord trieopval[trieopsize];
// number of stored ops so far
Static UInt16 trieopptr;

/// #947
// characters to match
Static PackedASCIICode triec[TRIE_SIZE + 1];
// operations to perform
Static QuarterWord trieo[TRIE_SIZE + 1];
// left subtrie links
Static TriePointer triel[TRIE_SIZE + 1];
// right subtrie links
Static TriePointer trier[TRIE_SIZE + 1];
// the number of nodes in the trie
Static TriePointer trieptr;
// used to identify equivalent subtries
Static TriePointer triehash[TRIE_SIZE + 1];

/// #950
// does a family START here?
Static UChar trietaken[(TRIE_SIZE + 7) / 8];
// the first possible slot for each character
Static TriePointer triemin[256];
// largest location used in trie
Static TriePointer triemax;
// is the trie still in linked form?
// xref: 891, [950], 951, 960, 966, 1324, 1325
Static Boolean trie_not_ready;
#endif // #943,947,950: tt_INIT


/*971:*/
Static Scaled bestheightplusdepth, pagemaxdepth, bestsize, lastkern;
/*:971*/


/*980:*/
Static Pointer pagetail, bestpagebreak, lastglue, mainp;
Static char pagecontents;
Static Integer leastpagecost;
/*:980*/
/*982:*/
Static Scaled pagesofar[8];
Static Integer lastpenalty, insertpenalties;
/*:982*/
/*989:*/
Static Boolean outputactive;
/*:989*/


/*1032:*/
Static InternalFontNumber mainf;
Static FourQuarters maini, mainj;
Static FontIndex maink;
Static Integer mains;
Static HalfWord bchar, falsebchar;
Static Boolean cancelboundary, insdisc;
/*:1032*/


/*1074:*/
Static Pointer curbox;
/*:1074*/


/*1266:*/
Static HalfWord aftertoken;
/*:1266*/
/*1281:*/
Static Boolean longhelpseen;
/*:1281*/
// #1299: a string that is printed right after the banner
// format ident: 35, 61, 536, [1299], 1300, 1301, 1326, 1327, 1328, 1337
StrNumber format_ident;

/*1305:*/
Static FILE* fmtfile = NULL;
/*:1305*/


/*1342:*/
FILE* write_file[16]; // [0~15]
Static Boolean writeopen[18];
/*:1342*/
/*1345:*/
Static Pointer writeloc; 
/*:1345*/


jmp_buf _JMP_global__final_end;

#endif // INC_TEX_HEADER