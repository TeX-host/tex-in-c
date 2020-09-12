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

extern HalfWord scantoks(Boolean macrodef, Boolean xpand);

// debug
extern void printword(MemoryWord w);

// indep
extern void readtoks(long n, HalfWord r);

#endif // TEX_FUNC_H