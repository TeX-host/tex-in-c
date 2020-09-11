#pragma once
#ifndef INC_MMODE_H
/// tex.c 里使用的宏
#define INC_MMODE_H

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


extern TwoHalves emptyfield;
extern FourQuarters nulldelimiter;

extern void mmode_init();
extern HalfWord newnoad(void);
extern HalfWord newstyle(SmallNumber s);
extern HalfWord newchoice(void);
extern void printfamandchar(HalfWord p);
extern void printdelimiter(HalfWord p);
extern void printsubsidiarydata(HalfWord p, ASCIICode c);
extern void showinfo(void);
extern void printstyle(Integer c);

#endif /* INC_MMODE_H */