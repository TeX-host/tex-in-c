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
extern void jumpout(void);
extern void giveerrhelp(void);

// tex, box
extern void print_write_whatsit(StrNumber s, Pointer p);

extern void normalparagraph(void);
extern void doassignments(void);
extern void resumeafterdisplay(void);
extern void buildpage(void);

extern HalfWord scantoks(Boolean macrodef, Boolean xpand);

// debug
extern void printword(MemoryWord w);

// extension
extern SmallNumber normmin(long h);
extern void reportillegalcase(void);

#endif // TEX_FUNC_H