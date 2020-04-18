#pragma once
#ifndef TEX_FUNC_H
#define TEX_FUNC_H
#include "str.h" // StrNumber

// [tex], inputln, pool_str, str
extern void overflow(StrNumber s, Integer n);

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
extern void printnl(StrNumber s);
extern void print_file_name(StrNumber n, StrNumber a, StrNumber e);
extern void packfilename(StrNumber n, StrNumber a, StrNumber e);
extern void sprint_cs(Pointer p);
extern void error(void);
extern Integer get_defaultskewchar(void);
extern Integer get_defaulthyphenchar(void);
extern Pointer get_lo_mem_max(void);


// [tex], str
extern HalfWord idlookup_p(unsigned char* buffp, long l, int no_new);

// [tex]
extern void set_help(UChar k, ...);

#endif // TEX_FUNC_H