#pragma once
#ifndef TEX_FUNC_H
// tex.c 导出的函数
#define TEX_FUNC_H
#include "str.h" // StrNumber


// [tex], fonts, macro.h, printout
extern void print(StrNumber s);

// [tex], fonts, printout, str
extern void print_int(Integer n);

// [tex], fonts, printout
extern void print_esc(StrNumber s);

// [tex], fonts, str
extern void print_char(ASCIICode s); 
extern void println(void);

// [tex], printout
extern void print_hex(Integer n);
extern void print_size(Integer s);
extern void print_skip_param(Integer n);
extern void printstyle(Integer c);

// [tex], fonts
extern void print_file_name(StrNumber n, StrNumber a, StrNumber e);
extern void packfilename(StrNumber n, StrNumber a, StrNumber e);
extern Integer get_defaultskewchar(void);
extern Integer get_defaulthyphenchar(void);


// [tex]
extern void set_help(SChar k, ...);

// [tex] lexer
extern void begindiagnostic(void);
extern void enddiagnostic(Boolean blankline);
extern void print_mode(Integer m);
extern Pointer get_avail(void);
extern void flushlist(HalfWord p);
extern void delete_token_ref(HalfWord p);
extern void openlogfile(void);

// [tex] expand
extern void startinput(void);
extern void freenode(Pointer p, HalfWord s);
extern void eqdefine(HalfWord p, QuarterWord t, HalfWord e);
extern HalfWord getnode(long s);
extern HalfWord strtoks(StrPoolPtr b);
extern HalfWord thetoks(void);

// [tex] scan
extern void delete_glue_ref(HalfWord p);
extern void preparemag(void);
extern Pointer newrule(void);
extern Pointer newspec(Pointer p);

// [tex], error
extern void debughelp(void);
extern void jumpout(void);
extern void giveerrhelp(void);

// [tex] eqtb
extern void printspec(long p, StrNumber s);
extern void shownodelist(long p);

#endif // TEX_FUNC_H