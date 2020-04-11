#pragma once
#ifndef FONTS_H
#define FONTS_H
#include <stdio.h> // FILE
#include "str.h"   // [type] StrNumber

#define FONT_BASE       0
#define FONT_MAX        75
#define FONT_MEM_SIZE   200000

// [fonts], dviout, tex

// #548: [FONT_BASE, FONT_MAX] = [0, 75]
typedef unsigned short InternalFontNumber;
extern InternalFontNumber fontptr;
extern Boolean fontused[FONT_MAX + 1];

// [fonts], dviout, printout, tex
extern Scaled get_fontsize(InternalFontNumber x);
extern Scaled get_fontdsize(InternalFontNumber x);
extern StrNumber get_fontname(InternalFontNumber x);

// [fonts], tex
typedef Pointer FontIndex; // #548: [0, FONT_MEM_SIZE] = [0, 200000]
extern FourQuarters charinfo(InternalFontNumber f, EightBits p);
extern Scaled charwidth(InternalFontNumber x, FourQuarters y);
extern Scaled charitalic(InternalFontNumber x, FourQuarters y);
extern Scaled charheight(InternalFontNumber x, int y);
extern Scaled chardepth(InternalFontNumber x, int y);
extern Scaled charkern(InternalFontNumber x, FourQuarters y);
extern FourQuarters get_fontcheck(InternalFontNumber x);
extern void fonts_init(void);
extern InternalFontNumber
readfontinfo(HalfWord u, StrNumber nom, StrNumber aire, long s);
extern int fonts_undump(FILE* fmtfile, FILE* _not_use_);
extern void fonts_dump(FILE* fmtfile);
extern long get_hyphenchar(InternalFontNumber x); // texmac.h
extern void set_hyphenchar(InternalFontNumber x, long c);
extern long get_skewchar(InternalFontNumber x);
extern void set_skewchar(InternalFontNumber x, long c);

extern FontIndex fmemptr;
extern MemoryWord fontinfo[FONT_MEM_SIZE + 1];
extern FontIndex fontparams[FONT_MAX + 1];
extern EightBits fontbc[FONT_MAX + 1];
extern EightBits fontec[FONT_MAX + 1];
extern Pointer fontglue[FONT_MAX + 1];
extern FontIndex bcharlabel[FONT_MAX + 1];
extern long fontbchar[FONT_MAX + 1];
extern long fontfalsebchar[FONT_MAX + 1];
extern long ligkernbase[FONT_MAX + 1]; // texmac
extern long extenbase[FONT_MAX + 1];
extern long parambase[FONT_MAX + 1];

// [tex], fonts
extern StrNumber fontidtext(InternalFontNumber x);

#endif // #ifndef FONTS_H