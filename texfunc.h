#pragma once
#include <stdio.h>

// no impl
// extern void bclose(FILE**);

// [tex], fonts, macro.h, printout
extern void print(strnumber s);

// [tex], fonts, printout, str
extern void printint(long n);

// [tex], fonts, printout
extern void printesc(strnumber s); 
extern void printscaled(long s);

// [tex], fonts, str
extern void printchar(ASCIIcode s); 
extern void println(void);

// [tex], printout
extern void printhex(long n);
extern void printsize(long s);
extern void printskipparam(long n);
extern void printstyle(long c);

// [tex], fonts
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

// [tex], str
extern halfword idlookup_p(unsigned char* buffp, long l, int no_new);

// [tex]
extern void set_help(int k, ...); 
