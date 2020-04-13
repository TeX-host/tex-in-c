#pragma once
#ifndef DIV_OUT_H
#define DIV_OUT_H
#include "fonts.h"  // [type] InternalFontNumber
#include "str.h"    // [type] StrASCIICode

// #11: size of the output buffer; must be a multiple of 8
#define DVI_BUF_SIZE 800

// [dviout], tex
extern void dviout(int x);
extern void dvi_four(Integer x);
extern void dvi_pop(Integer l);
extern void dvi_font_def(InternalFontNumber f);
extern void prune_movements(Integer l);
extern Scaled synch_h(Scaled cur_h, Scaled dvi_h);
extern Scaled synch_v(Scaled cur_v, Scaled dvi_v);
extern void dviout_helper(StrASCIICode c);

// help func
extern void dviout_ID_BYTE(void);
extern void dviout_SET_RULE(void);
extern void dviout_PUT_RULE(void);
extern void dviout_EOP(void);
extern void dviout_PUSH(void);
extern void dviout_POP(void);
extern void dviout_PRE(void);
extern void dviout_POST(void);
extern void dviout_XXX1(void);
extern void dviout_XXX4(void);

extern long get_dvi_mark(void);
extern Boolean dvi_openout(void);
extern void _dvibop(long counts[]);
extern void _dvi_lastbop(void);
extern void dvi_set_font(int f);
extern void dvi_set_char(long c);
extern void dviout_init(void);
extern long dviflush(void);

#endif // #ifndef DIV_OUT_H