#pragma once
#ifndef DIV_OUT_H
#define DIV_OUT_H
#include "fonts.h"  // [type] InternalFontNumber
#include "str.h"    // [type] StrASCIICode

// #11: size of the output buffer; must be a multiple of 8
#define DVI_BUF_SIZE 800

// [dviout] only
extern void dvifontdef(InternalFontNumber f);

// [dviout], tex
extern int dvi_openout(void);
extern void dviout_init(void);
extern void dvibop(long* counts);
extern void dviout(int x);
extern long get_dvi_mark(void);
extern void move_h(long w); // texmac(synchh)
extern void move_v(long w); // texmac(synchv)
extern void dvifour(unsigned long x);
extern void dviout_helper(StrASCIICode c);
extern void prunemovements(long l);
extern void dvipop(long l);
extern void dvi_putrule(long ruleht, long rulewd);
extern void dvi_pre(long num_, long den_, long mag_);
extern void dvi_set_font(int f);
extern void dvi_set_char(long c);
extern void dvi_setrule(long ruleht, long rulewd);
extern void dvi_push(void);
extern void dvi_pop(void);
extern void dvi_eop(void);
extern long dviflush(void);
extern void dvipost(
    long num_,
    long den_,
    long mag_,
    long max_v,
    long max_h,
    int max_push,
    int totalpages,
    int fontptr
);

#endif // #ifndef DIV_OUT_H