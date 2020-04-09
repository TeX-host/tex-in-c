#pragma once
#include <stdio.h>
extern void bclose(FILE * *);
extern void printchar(ASCIIcode s);
extern void println(void);
extern void print(strnumber s);
extern halfword idlookup_p(unsigned char * buffp, long l, int no_new);
extern void set_help(int k,...);
extern void printesc(strnumber s);
extern void printhex(long n);
extern void printint(long n);
extern void printlengthparam(long n);
extern void printscaled(long s);
extern void printsize(long s);
extern void printskipparam(long n);
extern void printstyle(long c);
extern void printnl(strnumber s);
extern void printfilename(strnumber n, strnumber a, strnumber e);
extern void packfilename(strnumber n, strnumber a, strnumber e);
extern void sprintcs(halfword p);
extern void error(void);
extern long xnoverd(long x, long n, long d);
extern int get_defaultskewchar(void);
extern int get_defaulthyphenchar(void);
extern pointer get_lomemmax(void);
extern strnumber fontidtext(internalfontnumber x);
