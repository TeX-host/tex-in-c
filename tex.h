#pragma once
#ifndef TEX_H
/// tex.c, tex_header 中需要导出的符号。
#define TEX_H
#include <setjmp.h> // [type] jmp_buf; [func] setjmp

/*
 * [ #13: Global variables ]
 *
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

extern jmp_buf _JMP_global__end_of_TEX;
extern jmp_buf _JMP_global__final_end;

#endif // #ifndef TEX_H