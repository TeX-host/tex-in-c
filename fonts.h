#pragma once
#ifndef FONTS_H
#define FONTS_H
#include <stdio.h> // FILE
#include "global_const.h"
// [fonts], dviout, tex

// #548: [FONT_BASE, FONT_MAX] = [0, 75]
typedef UChar InternalFontNumber;
static_assert(FONT_BASE == 0, "FONT_BASE == 0");
static_assert(UMAXOF(InternalFontNumber) >= FONT_MAX,
              "InternalFontNumber = [FONT_BASE, FONT_MAX] = [0, 75]");
extern InternalFontNumber fontptr;
extern Boolean fontused[FONT_MAX + 1];

// [fonts], dviout, printout, tex
extern Scaled get_fontsize(InternalFontNumber x);
extern Scaled get_fontdsize(InternalFontNumber x);
extern StrNumber get_fontname(InternalFontNumber x);

// [fonts], tex
typedef Pointer FontIndex; // #548: [0, FONT_MEM_SIZE] = [0, 200000]
static_assert(UMAXOF(FontIndex) >= FONT_MEM_SIZE,
              "FontIndex = [0, FONT_MEM_SIZE] = [0, 200000]");
extern FourQuarters charinfo(InternalFontNumber f, EightBits p);
extern Scaled charwidth(InternalFontNumber x, FourQuarters y);
extern Scaled charitalic(InternalFontNumber x, FourQuarters y);
extern Scaled charheight(InternalFontNumber x, Integer y);
extern Scaled chardepth(InternalFontNumber x, Integer y);
extern Scaled charkern(InternalFontNumber x, FourQuarters y);
extern FourQuarters get_fontcheck(InternalFontNumber x);
extern void fonts_init(void);
extern InternalFontNumber
readfontinfo(Pointer u, StrNumber nom, StrNumber aire, Scaled s);
extern int fonts_undump(FILE* fmtfile, FILE* _not_use_);
extern void fonts_dump(FILE* fmtfile);
extern long get_hyphenchar(InternalFontNumber x); // texmac.h
extern void set_hyphenchar(InternalFontNumber x, Integer c);
extern long get_skewchar(InternalFontNumber x);
extern void set_skewchar(InternalFontNumber x, Integer c);

extern FontIndex fmemptr;
extern MemoryWord fontinfo[FONT_MEM_SIZE + 1];
extern FontIndex fontparams[FONT_MAX + 1];
extern EightBits fontbc[FONT_MAX + 1];
extern EightBits fontec[FONT_MAX + 1];
extern Pointer fontglue[FONT_MAX + 1];
extern FontIndex bcharlabel[FONT_MAX + 1];
extern Integer fontbchar[FONT_MAX + 1];
extern Integer fontfalsebchar[FONT_MAX + 1];
extern Integer ligkernbase[FONT_MAX + 1]; // texmac
extern Integer extenbase[FONT_MAX + 1];
extern Integer parambase[FONT_MAX + 1];

// [tex], fonts
extern StrNumber fontidtext(InternalFontNumber x);

#endif // #ifndef FONTS_H