#pragma once
#ifndef FONTS_H
/// 字体相关函数
#define FONTS_H
#include <stdio.h> // FILE
#include "global.h"       // [macro] qi, qo; [const] UMAXOF, static_assert
#include "tex_constant.h" // [const] FONT_MAX, FONT_MEM_SIZE, FONT_BASE, MIN_QUARTER_WORD, MAX_HALF_WORD
#include "str.h"
#include "dump.h" // [macro] pget
// [fonts], dviout, tex


/** @addtogroup S539x582_P196x213
 * @{
 */

/** [p198#544]: tag field in a char_info_word
 * that explain how to interpret the remainder field.
 */ 
enum CharTag {
    NO_TAG,   ///< vanilla character
    LIG_TAG,  ///< character has a ligature/kerning program
    LIST_TAG, ///< character has a successor in a charlist
    EXT_TAG   ///< character is extensible
};

/// [p200#547]
enum TFMParamVal {
    TFM_PARAM_MISSING, ///< TeX sets the missing parameters to zero

    SLANT_CODE,
    SPACE_CODE,
    SPACE_STRETCH_CODE,
    SPACE_SHRINK_CODE,
    X_HEIGHT_CODE,
    QUAD_CODE,
    EXTRA_SPACE_CODE
};

/// [p199#545]
#define opbyte(x)       (x.b2)
#define rembyte(x)      (x.b3)

/// [p199#545] value indicating `\.{STOP}' in a lig/kern program.
#define stopflag        qi(128)
/// [p199#545] op code for a kern step.
#define kernflag        qi(128)
#define skipbyte(x)     x.b0
#define nextchar(x)     x.b1

/// [p203#554]
#define charexists(x)  ((x).b0>MIN_QUARTER_WORD)
#define chartag(x)  ((qo((x).b2)) % 4)

/// [p203#554]
#define heightdepth(x)      qo(x.b1)

/// [p204#557] beginning of lig/kern program
#define ligkernstart(x, y) (ligkernbase[(x)] + rembyte(y))
/// [p204#557]
#define ligkernrestart(x, y) \
    (ligkernbase[(x)] + opbyte(y) * 256 + rembyte(y) - kernbaseoffset + 32768L)
// #define lig_kern_start(x)  (lig_kern_base[x]+rem_byte)

/// [#549] a |halfword| code that can't match a real character}
#define NON_CHAR        256
/// [p201#549] a spurious bchar label
#define nonaddress      0
/// [p204#557]
#define kernbaseoffset  32768

/// [p204#558]
#define param(x, y)    (fontinfo[(x) + parambase[y]].sc)
#define slant(x)        param(SLANT_CODE,x) /* slant to the right, per unit distance upward}*/
#define space(x)        param(SPACE_CODE,x) /* NORMAL space between words}*/
#define spacestretch(x) param(SPACE_STRETCH_CODE,x) /* stretch between words}*/
#define spaceshrink(x)  param(SPACE_SHRINK_CODE,x) /* shrink between words}*/
#define xheight(x)      param(X_HEIGHT_CODE,x) /* one ex}*/
#define quad(x)         param(QUAD_CODE,x) /* one em}*/
#define extraspace(x)   param(EXTRA_SPACE_CODE,x) /* additional space at end of sentence}*/

/// [p205#560]
#define badtfm      11

/// [p199#564]
#define exttop(x)   x.b0 /* |top| piece in a recipe */
#define extmid(x)   x.b1 /* |mid| piece in a recipe */
#define extbot(x)   x.b2 /* |bot| piece in a recipe */
#define extrep(x)   x.b3 /* |rep| piece in a recipe */

/// [#548]: [FONT_BASE, FONT_MAX] = [0, 75]
typedef UChar InternalFontNumber;
static_assert(FONT_BASE == 0, "FONT_BASE == 0");
static_assert(UMAXOF(InternalFontNumber) >= FONT_MAX,
              "InternalFontNumber = [FONT_BASE, FONT_MAX] = [0, 75]");
/** @}*/ // end group S539x582_P196x213


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
extern void fonts_init_once(void);

extern int fonts_undump(FILE* fmt_file, FILE* _not_use_);
extern void fonts_dump(FILE* fmt_file);
extern Integer get_hyphenchar(InternalFontNumber x);
extern void set_hyphenchar(InternalFontNumber x, Integer c);
extern Integer get_skewchar(InternalFontNumber x);
extern void set_skewchar(InternalFontNumber x, Integer c);

extern InternalFontNumber
readfontinfo(Pointer u, StrNumber nom, StrNumber aire, Scaled s);
extern void scanfontident(void);
extern void findfontdimen(Boolean writing);
extern void charwarning(InternalFontNumber f, EightBits c);
extern HalfWord newcharacter(InternalFontNumber f, EightBits c);

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

extern FourQuarters nullcharacter;

extern void font_init();

#endif // #ifndef FONTS_H