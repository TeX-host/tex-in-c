#pragma once
#ifndef TEX_FUNC_H
// tex.c 导出的函数
#define TEX_FUNC_H
#include "tex_types.h"
#include "str.h"

// [tex] expand
extern HalfWord strtoks(StrPoolPtr b);
extern HalfWord thetoks(void);

extern HalfWord scantoks(Boolean macrodef, Boolean xpand);

// indep
extern void readtoks(long n, HalfWord r);

#endif // TEX_FUNC_H