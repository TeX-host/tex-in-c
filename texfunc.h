#pragma once
#ifndef TEX_FUNC_H
// tex.c 导出的函数
#define TEX_FUNC_H
#include "str.h" // StrNumber


// [tex], fonts, printout, str
extern void print_int(Integer n);

// [tex], fonts, printout
extern void print_esc(StrNumber s);

// [tex], fonts, str
extern void println(void);

// [tex], printout
extern void print_hex(Integer n);
extern void print_size(Integer s);

// [tex], fonts
extern void print_file_name(StrNumber n, StrNumber a, StrNumber e);



// [tex]
extern void set_help(SChar k, ...);


// [tex] expand
extern HalfWord strtoks(StrPoolPtr b);
extern HalfWord thetoks(void);

// [tex] scan
extern Pointer newspec(Pointer p);

// [tex], error
extern void debughelp(void);
extern void jumpout(void);
extern void giveerrhelp(void);

// tex, box
extern void print_write_whatsit(StrNumber s, Pointer p);

// lexer
extern void printtotals(void);

// dviout
extern void outwhat(HalfWord p);

extern void normalparagraph(void);
extern void doassignments(void);
extern void resumeafterdisplay(void);
extern void buildpage(void);


extern SmallNumber reconstitute(int j, SmallNumber n,
                                HalfWord bchar, HalfWord hchar);

extern void inittrie(void);
#endif // TEX_FUNC_H