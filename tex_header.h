#pragma once
#ifndef INC_TEX_HEADER
// 存放 tex.c 的头部。
// 导入库、其他模块，声明 tex.c 的内部静态变量
#define INC_TEX_HEADER

#include <stdlib.h> // exit, EXIT_SUCCESS

#include "global_const.h"
#include "tex.h"
#include "tex_types.h"
#include "macros.h"

#include "charset.h"
#include "error.h"
    // [func] normalize_selector, succumb, fatalerror,
    // overflow, confusion, print_err,
    // [macro] checkinterrupt,
#include "mem.h"
#include "lexer.h"      // lexer

#include "eqtb.h"
#include "hash.h"       // [func] sprint_cs
#include "expand.h"     // [macro] STORE_NEW_TOKEN
#include "scan.h"       // scan mod;

#include "io.h" // [func] inputln
#include "fonts.h"
#include "dviout.h"

#include "pack.h"
#include "mmode.h"
#include "align.h"
#include "hyphen.h"
#include "page.h"

#include "main_ctrl.h"
#include "dump.h"
#include "extension.h"

#include "str.h" // [func] get_strings_started
#include "box.h" // [macro] subtype, widthoffset; [func] delete_token_ref


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

jmp_buf _JMP_global__end_of_TEX;
jmp_buf _JMP_global__final_end;

#endif // INC_TEX_HEADER