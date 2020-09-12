#pragma once
#ifndef INC_TEX_HEADER
// 存放 tex.c 的头部。
// 导入库、其他模块，声明 tex.c 的内部静态变量
#define INC_TEX_HEADER

#include <stdio.h>  // FILE
#include <stdlib.h> // labs, abs, exit, EXIT_SUCCESS
#include <string.h> // memcpy
#include <math.h>   // fabs

#include "global_const.h"
#include "texmath.h"
#include "tex.h" // [export]
#include "str.h"
#include "texmac.h"
#include "macros.h"
#include "global.h"
#include "printout.h"
#include "fonts.h"
#include "dviout.h"
#include "print.h"      // 打印函数 term_input
// 模块导入
#include "lexer.h"      // lexer
#include "expand.h"     // [macro] STORE_NEW_TOKEN
#include "scan.h"       // scan mod;
#include "error.h"
    // [func] normalize_selector, succumb, fatalerror,
    // overflow, confusion, print_err,
    // [macro] checkinterrupt,
#include "hash.h"       // [func] sprint_cs
#include "eqtb.h"
#include "mem.h"
#include "box.h"
#include "io.h" // [func] inputln
#include "pack.h"
#include "mmode.h"
#include "align.h"
#include "linebreak.h"
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

Static jmp_buf _JMP_global__end_of_TEX;


/** @addtogroup S17x24_P10x12
 * @{
 */

/// [#20] specifies conversion of input characters.
ASCIICode xord[256];
/// [#20] specifies conversion of output characters.
TextChar  xchr[256];
/** @}*/ // end group S17x24_P10x12


/** @addtogroup S332x365_P134x143
 * @{
 */

/// [ #332~365：PART 24: GETTING THE NEXT TOKEN ]
// [#333] location of ‘\par’ in eqtb
Pointer parloc;
// [#333] token representing ‘\par’
HalfWord partoken;
// [#361] should the next \input be aborted early?
Boolean force_eof;
/** @}*/ // end group S332x365_P134x143


/** @addtogroup S464x486_P174x180
 * @{
 */

/// [ #464~486: PART 27: BUILDING TOKEN LISTS ]
/*480:*/
Static FILE* readfile[16];
char readopen[17];
/*:480*/
/** @}*/ // end group S464x486_P174x180


/*892:*/
short hc[66];
/* SmallNumber */ int hn;
InternalFontNumber hf;
short hu[64];
Integer hyfchar;
ASCIICode curlang, initcurlang;
Integer lhyf, rhyf, initlhyf, initrhyf;
HalfWord hyfbchar;
/*:892*/


/*900:*/
char hyf[65];
Boolean initlig, initlft;
/*:900*/
/*905:*/
SmallNumber hyphenpassed;
/*:905*/
/*907:*/
Static HalfWord curl, curr;
Static Boolean ligaturepresent, lfthit, rthit;
/*:907*/


/*921:*/
TwoHalves trie[TRIE_SIZE + 1];
SmallNumber hyfdistance[TRIE_OP_SIZE];
SmallNumber hyfnum[TRIE_OP_SIZE];
QuarterWord hyfnext[TRIE_OP_SIZE];
short opstart[256];
/*:921*/
/*926:*/
StrNumber hyphword[HYPH_SIZE + 1];
Pointer hyphlist[HYPH_SIZE + 1];
Static HyphPointer hyphcount;
/*:926*/

/// [ #942~966: PART 43: INITIALIZING THE HYPHENATION TABLES ]
/// #943, 947, 950
#ifdef tt_INIT
/// #943
// trie op codes for quadruples
Static short trieophash[TRIE_OP_SIZE + TRIE_OP_SIZE + 1];
// largest opcode used so far for this language
Static QuarterWord trieused[256];
// language part of a hashed quadruple
Static ASCIICode trieoplang[TRIE_OP_SIZE];
// opcode corresponding to a hashed quadruple
Static QuarterWord trieopval[TRIE_OP_SIZE];
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
Boolean trie_not_ready;
#endif // #943,947,950: tt_INIT


/*971:*/
Scaled bestheightplusdepth, pagemaxdepth, bestsize, lastkern;
/*:971*/


/*980:*/
Pointer pagetail, bestpagebreak, lastglue, mainp;
char pagecontents;
Static Integer leastpagecost;
/*:980*/
/*982:*/
Scaled pagesofar[8];
Integer lastpenalty, insertpenalties;
/*:982*/
/*989:*/
Boolean outputactive;
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


/// [#1342] Hold 16 possible `\write` streams.
/// Selector [0~15]
FILE* write_file[16];
/// [#1342] Set to true if that file is open.
/// + j=0..16: j-th file is open if and only if write open[j] = true.
/// + write_open[16]: stream number greater than 15
/// + write_open[17]: negative stream number
Static Boolean write_open[18];

/*1345:*/
Static Pointer writeloc; 
/*:1345*/


jmp_buf _JMP_global__final_end;

#endif // INC_TEX_HEADER