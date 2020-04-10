#pragma once
#ifndef TEX_FUNC_H
#define TEX_FUNC_H
#include "str.h" // StrNumber

// [tex], inputln, pool_str, str
extern void overflow(StrNumber s, long n);

// [tex], fonts, macro.h, printout
extern void print(StrNumber s);

// [tex], fonts, printout, str
extern void printint(long n);

// [tex], fonts, printout
extern void printesc(StrNumber s); 
extern void printscaled(long s);

// [tex], fonts, str
extern void printchar(ASCIICode s); 
extern void println(void);

// [tex], printout
extern void printhex(long n);
extern void printsize(long s);
extern void printskipparam(long n);
extern void printstyle(long c);

// [tex], fonts
extern void printnl(StrNumber s);
extern void printfilename(StrNumber n, StrNumber a, StrNumber e);
extern void packfilename(StrNumber n, StrNumber a, StrNumber e);
extern void sprintcs(HalfWord p);
extern void error(void);
extern long xnoverd(long x, long n, long d);
extern int get_defaultskewchar(void);
extern int get_defaulthyphenchar(void);
extern Pointer get_lomemmax(void);


// [tex], str
extern HalfWord idlookup_p(unsigned char* buffp, long l, int no_new);

// [tex]
extern void set_help(int k, ...);

#endif // TEX_FUNC_H