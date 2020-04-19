#pragma once
#ifndef FUNCS_H
// 一些辅助函数。
#define FUNCS_H
#include <stdio.h> // FILE

// [funcs], tex
extern int PASCAL_MAIN(int argc, char** argv);
extern Boolean initinc(int _not_use_);
extern Boolean open_fmt(FILE** fmt);
extern void
fix_date_and_time(long* tex_time_p, long* day_p, long* month_p, long* year_p);
extern Boolean need_to_load_format;

// [funcs], fonts, tex
extern Boolean a_open_in(FILE** f);

// [funcs], dviout, tex, texmac.h
extern Boolean a_open_out(FILE** f);

#endif // #ifndef FUNCS_H