#pragma once
#ifndef INC_MMODE_H
/// tex.c 里使用的宏
#define INC_MMODE_H
#include "charset.h" // [type] ASCIICode
#include "fonts.h"
#include "scan.h" // MAX_DIMEN

/** @addtogroup S680x698_P249x257
 * @{
 */

/** [p251#682] type of noad classified XXX.
 * 
 * Each portion of a formula is classified as 
 * Ord, Op, Bin, Rel, Ope, Clo, Pun, or Inn,
 * for purposes of spacing and line breaking.
 */
enum NoadType {
    /// [#682] [UNSET_NODE=13 + 3 = 16]
    /// type of a noad classified Ord.
    ordnoad = (UNSET_NODE + 3),

    // [#682]
    opnoad,    ///< noad classified Op
    binnoad,   ///< noad classified Bin
    relnoad,   ///< noad classified Rel
    opennoad,  ///< noad classified Ope
    closenoad, ///< noad classified Clo
    punctnoad, ///< noad classified Pun
    innernoad, ///< noad classified Inn.

    // [#683]
    radicalnoad,  ///< noad for square roots
    fractionnoad, ///< noad for generalized fractions.

    // [#687]
    undernoad,   ///< noad for underlining
    overnoad,    ///< noad for overlining
    accentnoad,  ///< noad for accented subformulas
    vcenternoad, ///< noad for `\vcenter`
    leftnoad,    ///< noad for `\left`
    rightnoad,   ///< noad for `\right`
}; // [#682] enum NoadType

/// [#682] `subtype` of `op_noad` whose scripts are to be above, below
#define limits   1
/// [#682] `subtype` of `op_noad` whose scripts are to be normal
#define nolimits 2

/// [#681] number of words in a normal noad
#define noadsize            (4 * CHAR_NODE_SIZE)
/// [#683] number of mem words in a radical noad
#define radicalnoadsize     (5 * CHAR_NODE_SIZE)
/// [#683] number of mem words in a fraction noad
#define fractionnoadsize    (6 * CHAR_NODE_SIZE)
/// [#687] number of mem words in an accent noad
#define accentnoadsize      (5 * CHAR_NODE_SIZE)

/// [#688] type of a style node
#define stylenode (UNSET_NODE + 1)
/// [#688] number of words in a style node
#define stylenodesize 3

/// [#688]
enum StyleNodeSubtype {
    displaystyle = 0,      ///< subtype for `\displaystyle`
    textstyle = 2,         ///< subtype for `\textstyle`
    scriptstyle = 4,       ///< subtype for `\scriptstyle`
    scriptscriptstyle = 6, ///< subtype for `\scriptscriptstyle`
}; // [#688] StyleNodeSubtype

/// [#688] add this to an uncramped style if you want to cramp it.
#define cramped 1
/// [#689] type of a choice node.
#define choicenode (UNSET_NODE + 2)



#if 1
/// [p250#681]
#define nucleus(x)  ((x)+CHAR_NODE_SIZE) /* the |nucleus| field of a noad}*/
#define supscr(x)  (nucleus(x)+CHAR_NODE_SIZE) /* the |supscr| field of a noad}*/
#define subscr(x)  (supscr(x)+CHAR_NODE_SIZE) /* the |subscr| field of a noad}*/
/// [p252#683]
#define leftdelimiter(x)  (subscr(x)+CHAR_NODE_SIZE) /* first delimiter field of a noad}*/
#define rightdelimiter(x)  (leftdelimiter(x)+CHAR_NODE_SIZE) /* second delimiter field of a fraction noad}*/
/// [p253#687]
#define accentchr(x)  (subscr(x)+CHAR_NODE_SIZE) /* the |accentchr| field of an accent noad}*/
#else
#define nucleus(x)  ((x)+1) /* the |nucleus| field of a noad}*/
#define supscr(x)  ((x)+2) /* the |supscr| field of a noad}*/
#define subscr(x)  ((x)+7) /* the |subscr| field of a noad}*/
#define leftdelimiter(x)  ((x)+4) /* first delimiter field of a noad}*/
#define rightdelimiter(x)  ((x)+5) /* second delimiter field of a fraction noad}*/
#define accentchr(x)  ((x)+4) /* the |accentchr| field of an accent noad}*/
#endif

/// [p250#681] a |halfword| in |mem|.
#define mathtype  link
#define fam(x)  type(x)
/// [p252#683]
#define smallfam(x)     mem[x].qqqq.b0 /* |fam| for ``small'' delimiter}*/
#define smallchar(x)    mem[x].qqqq.b1 /* |character| for ``small'' delimiter}*/
#define largefam(x)     mem[x].qqqq.b2 /* |fam| for ``large'' delimiter}*/
#define largechar(x)    mem[x].qqqq.b3 /* |character| for ``large'' delimiter}*/
#define thickness(x)   (mem[nucleus(x)-MEM_MIN].sc) /* |thickness| field in a fraction noad}*/
#define numerator       supscr /* |numerator| field in a fraction noad}*/
#define denominator     subscr /* |denominator| field in a fraction noad}*/
/// [p253#687]
#define delimiter       nucleus /* |delimiter| field in left and right noads}*/
/// [p253#687]
#define scriptsallowed(x)  ((type(x)>=ordnoad)&&(type(x)<leftnoad))
/// [p245#668] special case of unconstrained depth
#define vpack(x, y, z)  vpackage((x), (y), (z), MAX_DIMEN)
/// [p254#689]
#define displaymlist(x)     info(x+1) /* mlist to be used in display style}*/
#define textmlist(x)        link(x+1) /* mlist to be used in text style}*/
#define scriptmlist(x)      info(x+2) /* mlist to be used in script style}*/
#define scriptscriptmlist(x)    link(x+2) /* mlist to be used in scriptscript style}*/
/** @}*/ // end group S680x698_P249x257
/** @addtogroup S699x718_P258x264
 * @{
 */
/// p258#699: Subroutines for math mode
// enum FontSizeCode {};
// size code for the largest size in a family
#define TEXT_SIZE           0
// size code for the medium size in a family
#define SCRIPT_SIZE         16
// size code for the smallest size in a family
#define SCRIPT_SCRIPT_SIZE  32

// [p258#700]
#define totalmathsyparams  22
// [p258#701]
#define totalmathexparams  13

/// [p258#700]
#define mathsy(x, y) (fontinfo[(x) + parambase[fam_fnt(2 + (y))]].sc)
#define mathxheight(x) mathsy(5, x) /* height of `\.x'}*/
#define mathquad(x) mathsy(6, x)    /* \.{18mu}}*/
#define num1(x)  mathsy(8,x) /* numerator shift-up in display styles}*/
#define num2(x)  mathsy(9,x) /* numerator shift-up in non-display, non-\.{\\atop}}*/
#define num3(x)  mathsy(10,x) /* numerator shift-up in non-display \.{\\atop}}*/
#define denom1(x)  mathsy(11,x) /* denominator shift-down in display styles}*/
#define denom2(x)  mathsy(12,x) /* denominator shift-down in non-display styles}*/
#define sup1(x)  mathsy(13,x) /* superscript shift-up in uncramped display style}*/
#define sup2(x)  mathsy(14,x) /* superscript shift-up in uncramped non-display}*/
#define sup3(x)  mathsy(15,x) /* superscript shift-up in cramped styles}*/
#define sub1(x)  mathsy(16,x) /* subscript shift-down if superscript is absent}*/
#define sub2(x)  mathsy(17,x) /* subscript shift-down if superscript is present}*/
#define supdrop(x)  mathsy(18,x) /* superscript baseline below top of large box}*/
#define subdrop(x)  mathsy(19,x) /* subscript baseline below bottom of large box}*/
#define delim1(x)  mathsy(20,x) /* size of \.{\\atopwithdelims} delimiters*/
/* in display styles}*/
#define delim2(x)  mathsy(21,x) /* size of \.{\\atopwithdelims} delimiters in non-displays}*/
#define axisheight(x)  mathsy(22,x) /* height of fraction lines above the baseline}*/

/// [p258#701]
#define mathex(x) fontinfo[x + parambase[fam_fnt(3 + cursize)]].sc
#define defaultrulethickness  mathex(8) /* thickness of \.{\\over} bars}*/
#define bigopspacing1  mathex(9) /* minimum clearance above a displayed op}*/
#define bigopspacing2  mathex(10) /* minimum clearance below a displayed op}*/
#define bigopspacing3  mathex(11) /* minimum baselineskip above displayed op}*/
#define bigopspacing4  mathex(12) /* minimum baselineskip below displayed op}*/
#define bigopspacing5  mathex(13) /* padding above and below displayed limits}*/

/// [p259#702]
#define crampedstyle(x)  2*(x / 2)+cramped /* cramp the style}*/
#define substyle(x)  2*(x / 4)+scriptstyle+cramped /* smaller and cramped}*/
#define supstyle(x)  2*(x / 4)+scriptstyle+(x % 2) /* smaller}*/
#define numstyle(x)  x+2-2*(x / 6) /* smaller unless already script-script}*/
#define denomstyle(x)  2*(x / 2)+cramped+2-2*(x / 6) /* smaller, cramped}*/
/** @}*/ // end group S699x718_P258x264
/** @addtogroup S719x767_P265x284
 * @{
 */
/// [p267#725]the translation of an mlist
#define newhlist(x)     mem[nucleus(x)].int_
/** @}*/ // end group S719x767_P265x284

extern TwoHalves emptyfield;
extern FourQuarters nulldelimiter;

extern Pointer curmlist;
extern SmallNumber curstyle, cursize;
extern Boolean mlistpenalties;
extern InternalFontNumber curf;
extern QuarterWord curc;
extern FourQuarters curi;

extern void mmode_init();
extern void print_size(Integer s);
extern HalfWord newnoad(void);
extern HalfWord newstyle(SmallNumber s);
extern HalfWord newchoice(void);
extern void printfamandchar(HalfWord p);
extern void printdelimiter(HalfWord p);
extern void printsubsidiarydata(HalfWord p, ASCIICode c);
extern void showinfo(void);
extern void printstyle(Integer c);

extern HalfWord fractionrule(long t);
extern HalfWord overbar(HalfWord b, long k, long t);
extern HalfWord charbox(InternalFontNumber f, QuarterWord c);
extern void stackintobox(HalfWord b, InternalFontNumber f, QuarterWord c);
extern Integer heightplusdepth(InternalFontNumber f, QuarterWord c);
extern HalfWord vardelimiter(HalfWord d, SmallNumber s, long v);
extern HalfWord rebox(HalfWord b, long w);
extern HalfWord mathglue(HalfWord g, long m);
extern void mathkern(HalfWord p, long m);
extern void flushmath(void);

// math formula
extern void mlisttohlist(void);

#endif /* INC_MMODE_H */