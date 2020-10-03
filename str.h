#pragma once
#ifndef STR_H
// 字符串相关操作
#define STR_H
// Need:
// [type] Boolean@tex_types
#include <stdio.h> // FILE
#include <stdint.h> // int_fast32_t
#include "global_const.h"
#include "global_macros.h" // [const] UMAXOF, static_assert
#include "charset.h" // [type] ASCIICode
#include "tex_types.h"


/** @addtogroup S1x16_P3x9
 * @{
 */

/** [#11] maximum number of strings;
 *  must not exceed #MAX_HALF_WORD.
 *
 *  xref: str[6], tex[1].
 *  [3000 => 300_000]
 */
#define MAX_STRINGS 300000
/** [#11] the minimum number of characters
 *  that should be available for the user’s control sequences and font names,
 * after TEX’s own error messages are stored.
 *
 *  xref: inipool[2] only
 */
#define STRING_VACANCIES 8000
/** [#11] maximum number of characters in strings.
 *
 *  including all error messages and help texts,
 *  and the names of all fonts and control sequences;
 *  must exceed #STRING_VACANCIES by the total length of TEX’s own strings,
 *  which is currently about 23000.
 *
 *  xref: str[4], tex[4].
 *  [32000 => 1024*1024*4 (4 MB)]
 */
#define POOL_SIZE (1024 * 1024 * 4 - 1)
/** @}*/ // end group S1x16_P3x9


/** @addtogroup S38x53_P19x23
 * @{
 */

#define POOLPOINTER_IS_POINTER 1
#if POOLPOINTER_IS_POINTER
/// [#38]  for variables that point into #str_pool.
/// [PoolPointer => PoolPtr],
/// [0, POOL_SIZE=4MB-1B]
typedef ASCIICode* PoolPtr;
#define POOL_TOP (str_pool + POOL_SIZE)
#define POOL_ELEM(x, y) ((x)[(y)])
#else
typedef Pointer PoolPtr; // PoolPtr = [0, POOL_SIZE=3200000]
static_assert(UMAXOF(PoolPtr) >= POOL_SIZE,
              "PoolPtr = [0, POOL_SIZE=3200000]");
#define POOL_TOP POOL_SIZE
#define POOL_ELEM(x, y) (str_pool[(x) + (y)])
#endif

// [str], tex
typedef struct {
    PoolPtr val; // 暂存 pool_ptr::PoolPtr
} StrPoolPtr;
// dviout, fonts, global.h, str, tex
typedef uint_fast32_t StrNumber; // [0, MAX_STRINGS=300000]
static_assert(UMAXOF(StrNumber) >= MAX_STRINGS,
              "StrNumber = [0, MAX_STRINGS=300000]");
/// real string.
typedef char* Str;
/** @}*/ // end group S38x53_P19x23


#ifdef tt_INIT
// [str], tex
extern Boolean get_strings_started(void); // #47
#endif // tt_INIT

// [str], dviout, tex
extern void str_map(StrNumber k, void (*f)(ASCIICode));
extern int str_length(StrNumber s);

// [str][0] only
// NOTE: _not_use_
// extern void f_pool(StrNumber s); 

// [str][0], printout[1], tex[16]
extern void slow_print(StrNumber s);

// [str], inipool, tex
extern long str_adjust_to_room(long l);
extern StrNumber makestring(void);

// [str], tex
extern StrPoolPtr str_mark(void); // [str][0], tex[2]
extern int str_cmp(StrNumber s, StrNumber t);
extern int str_getc(StrNumber s, int k);
extern void str_map_from_mark(StrPoolPtr b, void (*f)(ASCIICode));
extern void str_room(StrNumber l);
extern void str_set_init_ptrs(void);
extern void str_print_stats(FILE* f_log_file);
extern void str_dump(FILE* fmtfile);
extern int str_undump(FILE* fmtfile, FILE* _not_use_);
extern void str_cur_map(void (*f)(ASCIICode));
extern int cur_length(void);
extern void flush_string(void);
extern void flush_char(void); // texmax.h
extern StrNumber idlookup_s(StrNumber s, Boolean no_new_cs);
extern void printcurrentstring(void);
extern Boolean str_eq_str(StrNumber s, StrNumber t);
extern void append_char(ASCIICode s);
extern void str_print(StrNumber s);
extern int str_valid(StrNumber s);
extern Boolean str_bcmp(ASCIICode buffp[], long l, StrNumber s);
extern int str_scmp(StrNumber s, short* buffp);
extern StrNumber str_insert(ASCIICode buffp[], Integer l);
extern StrNumber str_ins(short* buffp, long l);

// [inipool], str
extern Boolean str_pool_init(void); // inipool.c

#endif // #ifndef STR_H