#pragma once
#ifndef STR_H
#define STR_H
#include <stdio.h>

// str[6], tex[1]
#define maxstrings 300000
// inipool[2] only
#define stringvacancies 8000
// str[4], tex[4]
#define poolsize 3200000


#ifdef tt_INIT
// [str], tex
extern int getstringsstarted(void); // #47
#endif // tt_INIT

// str[3], tex[3]
typedef struct {
    long val;
} str_poolpointer;
// dviout, str, tex
typedef unsigned char strASCIIcode;
// dviout, fonts, funcs.h, global.h, str, tex
typedef int strnumber;


// [str], dviout, tex
extern void str_map(strnumber k, void (*f)(strASCIIcode));
extern int flength(strnumber s);

// [str][0] only
// NOTE: _not_use_
// extern void f_pool(strnumber s); 

// [str][0], printout[1], tex[16]
extern void slowprint(strnumber s);

// [str], inipool, tex
extern long str_adjust_to_room(long l);
extern strnumber makestring(void);

// [str], tex
extern str_poolpointer str_mark(void); // [str][0], tex[2]
extern int str_cmp(strnumber s, strnumber t);
extern int str_getc(strnumber s, int k);
extern void str_map_from_mark(str_poolpointer b, void (*f)(strASCIIcode));
extern void str_room(long l);
extern void str_set_init_ptrs(void);
extern void str_print_stats(FILE* f_log_file);
extern void str_dump(FILE* fmtfile);
extern int str_undump(FILE* fmtfile, FILE* _not_use_);
extern void str_cur_map(void (*f)(strASCIIcode));
extern int get_cur_length(void);
extern void flushstring(void);
extern void flushchar(void); // texmax.h
extern strnumber idlookup_s(strnumber s, int nonew);
extern void printcurrentstring(void);
extern boolean streqstr(strnumber s, strnumber t);
extern void appendchar(strASCIIcode s);
extern void str_appendchar(strASCIIcode s);
extern void str_print(strnumber s);
extern int str_valid(strnumber s);
extern int str_bcmp(unsigned char* buffp, long l, strnumber s);
extern int str_scmp(strnumber s, short* buffp);
extern strnumber str_insert(unsigned char* buffp, long l);
extern strnumber str_ins(short* buffp, long l);

// [inipool], str
extern int str_pool_init(void); // inipool.c

#endif // #ifndef STR_H