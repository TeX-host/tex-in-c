#pragma once
#ifndef DIV_OUT_H
#define DIV_OUT_H

extern int dvi_openout(void);
extern void dviout_init(void);
extern void dvibop(long * counts);
extern void dviout (int x);
extern long get_dvi_mark(void);
extern void move_h(long w);
extern void move_v(long w);
extern void dvifour(unsigned long x);
extern void dviout_helper(strASCIIcode c);
extern void dvifontdef(internalfontnumber f);
extern void prunemovements(long l);
extern void dvipop(long l);

#if 0
extern void dvipost();
#else
extern void dvipost(
    long num_, long den_, long mag_, 
    long max_v, long max_h, int max_push, 
    int totalpages, int fontptr
);
#endif // #if 0

extern void dvi_putrule(long ruleht, long rulewd);
extern void dvi_pre(long num_, long den_, long mag_);
extern void dvi_set_font(int f);
extern void dvi_set_char(long c);
extern void dvi_setrule(long ruleht, long rulewd);
extern void dvi_push(void);
extern void dvi_pop(void);
extern void dvi_eop(void);
extern long dviflush(void);

#endif // #ifndef DIV_OUT_H