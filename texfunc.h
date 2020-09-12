#pragma once
#ifndef TEX_FUNC_H
// tex.c 导出的函数
#define TEX_FUNC_H


// [tex], printout
extern void print_size(Integer s);

// [tex] expand
extern HalfWord strtoks(StrPoolPtr b);
extern HalfWord thetoks(void);

// [tex], error
extern void debughelp(void);
extern void jumpout(void);
extern void giveerrhelp(void);

// tex, box
extern void print_write_whatsit(StrNumber s, Pointer p);

// dviout
extern void outwhat(HalfWord p);

extern void normalparagraph(void);
extern void doassignments(void);
extern void resumeafterdisplay(void);
extern void buildpage(void);

extern HalfWord scantoks(Boolean macrodef, Boolean xpand);

#endif // TEX_FUNC_H