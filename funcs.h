#pragma once
#ifndef FUNCS_H
#define FUNCS_H

// [funcs], tex
extern int PASCAL_MAIN(int argc, char** argv);
extern int initinc(int dummy);
extern int open_fmt(FILE** fmt, FILE* _not_use_);
extern void
fixdateandtime(long* tex_time_p, long* day_p, long* month_p, long* year_p);
extern int need_to_load_format;

// [funcs], fonts, tex
extern int aopenin(FILE** f);

// [funcs], dviout, tex, texmac.h
extern int aopenout(FILE** f);

// [tex], inputln, pool_str, str
extern void overflow(strnumber s, long n);

#endif // #ifndef FUNCS_H