#pragma once
#ifndef STR_H
#define STR_H
#include <stdio.h>

#define maxstrings 300000
#define stringvacancies 8000
#define poolsize 3200000

#ifdef tt_INIT
extern int getstringsstarted(void); // #47
#endif // tt_INIT

typedef struct {
    long val;
} str_poolpointer;
typedef unsigned char strASCIIcode;
typedef int strnumber;

extern void f_pool(strnumber s);
extern void slowprint(strnumber s);
extern str_poolpointer str_mark(void);
extern int str_cmp(strnumber s, strnumber t);
extern int str_getc(strnumber s, int k);
extern void str_map_from_mark(str_poolpointer b, void (*f)(strASCIIcode));
extern long str_adjust_to_room(long l);
extern void str_room(long l);
extern void str_set_init_ptrs(void);
extern void str_print_stats(FILE* logfile);
extern void str_dump(FILE* fmtfile);
extern int str_undump(FILE* fmtfile, FILE* _not_use_);
extern void str_cur_map(void (*f)(strASCIIcode));
extern void str_map(strnumber k, void (*f)(strASCIIcode));
extern int get_cur_length(void);
extern int flength(strnumber s);
extern void flushstring(void);
extern void flushchar(void);
extern strnumber idlookup_s(strnumber s, int nonew);
extern void printcurrentstring(void);
extern strnumber makestring(void);
extern boolean streqstr(strnumber s, strnumber t);
extern void appendchar(strASCIIcode s);
extern void str_appendchar(strASCIIcode s);
extern void str_print(strnumber s);
extern int str_valid(strnumber s);
extern int str_bcmp(unsigned char* buffp, long l, strnumber s);
extern int str_scmp(strnumber s, short* buffp);
extern strnumber str_insert(unsigned char* buffp, long l);
extern strnumber str_ins(short* buffp, long l);

#endif // #ifndef STR_H