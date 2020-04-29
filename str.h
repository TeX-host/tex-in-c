#pragma once
#ifndef STR_H
// 字符串相关操作
#define STR_H
// Need:
// [type] Boolean@tex.h
#include <stdio.h> // FILE
#include <stdint.h> // int_fast32_t
#include "global_const.h"
#include "tex_types.h"

// str[6], tex[1]
#define MAX_STRINGS 300000
// inipool[2] only
#define STRING_VACANCIES 8000
// str[4], tex[4]
#define POOL_SIZE (1024 * 1024 * 4 - 1) // 4 MB

#define POOLPOINTER_IS_POINTER 1
#if POOLPOINTER_IS_POINTER
typedef ASCIICode* PoolPtr; // pool_pointer
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
// dviout, fonts, funcs.h, global.h, str, tex
typedef uint_fast32_t StrNumber; // [0, MAX_STRINGS=300000]
static_assert(UMAXOF(StrNumber) >= MAX_STRINGS,
              "StrNumber = [0, MAX_STRINGS=300000]");

#ifdef tt_INIT
// [str], tex
extern int get_strings_started(void); // #47
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
extern void str_room(long l);
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
extern int str_pool_init(void); // inipool.c

#endif // #ifndef STR_H