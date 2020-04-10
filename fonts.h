#pragma once
#ifndef FONTS_H
#define FONTS_H
#include <stdio.h>

// [fonts], dviout, tex
typedef unsigned short internalfontnumber;
extern internalfontnumber fontptr;
extern boolean fontused[fontmax + 1];

// [fonts], dviout, printout, tex
extern scaled get_fontsize(internalfontnumber x);
extern scaled get_fontdsize(internalfontnumber x);
extern strnumber get_fontname(internalfontnumber x);

// [fonts], tex
typedef int fontindex;
extern fourquarters charinfo(internalfontnumber f, eightbits p);
extern scaled charwidth(internalfontnumber x, fourquarters y);
extern scaled charitalic(internalfontnumber x, fourquarters y);
extern scaled charheight(internalfontnumber x, int y);
extern scaled chardepth(internalfontnumber x, int y);
extern scaled charkern(internalfontnumber x, fourquarters y);
extern fourquarters get_fontcheck(internalfontnumber x);
extern void fonts_init(void);
extern internalfontnumber
readfontinfo(halfword u, strnumber nom, strnumber aire, long s);
extern int fonts_undump(FILE* fmtfile, FILE* _not_use_);
extern void fonts_dump(FILE* fmtfile);
extern long get_hyphenchar(internalfontnumber x); // texmac.h
extern void set_hyphenchar(internalfontnumber x, long c);
extern long get_skewchar(internalfontnumber x);
extern void set_skewchar(internalfontnumber x, long c);

extern fontindex fmemptr;
extern memoryword fontinfo[fontmemsize + 1];
extern fontindex fontparams[fontmax + 1];
extern eightbits fontbc[fontmax + 1];
extern eightbits fontec[fontmax + 1];
extern pointer fontglue[fontmax + 1];
extern fontindex bcharlabel[fontmax + 1];
extern long fontbchar[fontmax + 1];
extern long fontfalsebchar[fontmax + 1];
extern long ligkernbase[fontmax + 1]; // texmac
extern long extenbase[fontmax + 1];
extern long parambase[fontmax + 1];

#endif // #ifndef FONTS_H