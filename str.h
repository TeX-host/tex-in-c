#pragma once
#ifndef STR_H
#define STR_H
// Need:
// [type] Boolean@tex.h
#include <stdio.h> // FILE

// str[6], tex[1]
#define MAX_STRINGS 300000
// inipool[2] only
#define STRING_VACANCIES 8000
// str[4], tex[4]
#define POOL_SIZE 3200000


#ifdef tt_INIT
// [str], tex
extern int get_strings_started(void); // #47
#endif // tt_INIT

// str, tex
typedef struct {
    long val;
} StrPoolPtr;
// dviout, str, tex
typedef unsigned char StrASCIICode;
// dviout, fonts, funcs.h, global.h, str, tex
typedef int StrNumber;


// [str], dviout, tex
extern void str_map(StrNumber k, void (*f)(StrASCIICode));
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
extern void str_map_from_mark(StrPoolPtr b, void (*f)(StrASCIICode));
extern void str_room(long l);
extern void str_set_init_ptrs(void);
extern void str_print_stats(FILE* f_log_file);
extern void str_dump(FILE* fmtfile);
extern int str_undump(FILE* fmtfile, FILE* _not_use_);
extern void str_cur_map(void (*f)(StrASCIICode));
extern int cur_length(void);
extern void flush_string(void);
extern void flush_char(void); // texmax.h
extern StrNumber idlookup_s(StrNumber s, int nonew);
extern void printcurrentstring(void);
extern Boolean str_eq_str(StrNumber s, StrNumber t);
extern void append_char(StrASCIICode s);
extern void str_print(StrNumber s);
extern int str_valid(StrNumber s);
extern int str_bcmp(unsigned char* buffp, long l, StrNumber s);
extern int str_scmp(StrNumber s, short* buffp);
extern StrNumber str_insert(unsigned char* buffp, long l);
extern StrNumber str_ins(short* buffp, long l);

// [inipool], str
extern int str_pool_init(void); // inipool.c

#endif // #ifndef STR_H