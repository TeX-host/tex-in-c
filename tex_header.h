#pragma once
#ifndef INC_TEX_HEADER
// 存放 tex.c 的头部。
// 导入库、其他模块，声明 tex.c 的内部静态变量
#define INC_TEX_HEADER

#include <stdio.h>  // FILE
#include <stdlib.h> // labs, abs, exit, EXIT_SUCCESS
#include <string.h> // memcpy
#include <setjmp.h> // jmp_buf
#include <math.h>   // fabs

#include "global_const.h"
#include "charset.h"
#include "texmath.h"
#include "tex.h" // [export]
#include "str.h"
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
#include "hyphen.h"
#include "page.h"
#include "main_ctrl.h"
#include "dump.h"
#include "extension.h"


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


/** @addtogroup S464x486_P174x180
 * [ #464~486: PART 27: BUILDING TOKEN LISTS ]
 * @{
 */
/// [#480]
FILE* readfile[16];
char readopen[17];
/** @}*/ // end group S464x486_P174x180

jmp_buf _JMP_global__end_of_TEX;
jmp_buf _JMP_global__final_end;

#endif // INC_TEX_HEADER