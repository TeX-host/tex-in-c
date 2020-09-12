#pragma once
#ifndef INC_LINEBREAK_H
///
#define INC_LINEBREAK_H

/** @addtogroup S813x861_P302x318
 * @{
 */

// [p303#817]
#define tightfit        3
#define loosefit        1
#define veryloosefit    0
#define decentfit       2
// [p304#819]
#define activenodesize  3
#define unhyphenated    0
#define hyphenated      1
#define lastactive      active
// [p304#821]
#define passivenodesize  2
// [p304#822]
#define deltanodesize   7
#define deltanode       2
// [p308#829]
#define deactivate      60


/// [p304#819]
#define fitness  subtype /* |veryloosefit..tightfit| on final line for this break}*/
#define breaknode  rlink /* pointer to the corresponding passive node}*/
#define linenumber  llink /* line that begins at this breakpoint}*/
#define totaldemerits(x)  mem[x+2].int_ /* the quantity that \TeX\ minimizes}*/
/// [p304#821]
#define curbreak  rlink /* in passive node, points to position of this breakpoint}*/
#define prevbreak  llink /* points to passive node that should precede this one}*/
#define serial  info /* serial number for symbolic identification}*/


/// [p306#825]
#define checkshrinkage(x) \
    ((shrinkorder(x) != NORMAL) && (shrink(x) != 0) ? (x) = finiteshrink(x) : 0)

/// [p308#829]
#define copytocuractive(x) (curactivewidth[(x)-1] = activewidth[(x)-1])

/// [p309#832]
#define updatewidth(x) (curactivewidth[(x)-1] += mem[r + (x)].sc)
/// [#833]
#define awfulbad 1073741823L
/// [p310#837]
#define setbreakwidthtobackground(x) (breakwidth[(x)-1] = background[(x)-1])


/// [p312#843]
#define converttobreakwidth(x) \
    (mem[prevr + (x)].sc += -curactivewidth[(x)-1] + breakwidth[(x)-1])
#define storebreakwidth(x) (activewidth[(x)-1] = breakwidth[(x)-1])
#define newdeltatobreakwidth(x) \
    (mem[q + (x)].sc = breakwidth[(x)-1] - curactivewidth[(x)-1])
/// [p312#944]
#define newdeltafrombreakwidth(x) \
    (mem[q + (x)].sc = curactivewidth[(x)-1] - breakwidth[(x)-1])

/// [p318#860]
#define combinetwodeltas(x) \
    (mem[prevr + (x)].sc = mem[prevr + (x)].sc + mem[r + (x)].sc)
#define downdatewidth(x) (curactivewidth[(x)-1] -= mem[prevr + (x)].sc)
/// [p318#861]
#define updateactive(x) (activewidth[(x)-1] += mem[r + (x)].sc)

/** @}*/ // end group S813x861_P302x318

/** @addtogroup S862x890_P319x329
 * @{
 */

/// [p320#864]
#define storebackground(x) (activewidth[(x)-1] = background[(x)-1])

/// [p321#866]
#define kernbreak()                                                       \
    {                                                                     \
        if (!ischarnode(link(curp)) && (autobreaking)) {                  \
            if (type(link(curp)) == GLUE_NODE) trybreak(0, unhyphenated); \
        }                                                                 \
        actwidth += width(curp);                                          \
    }

/// [p325#877]
#define nextbreak  prevbreak /* new name for |prevbreak| after links are reversed}*/

/** @}*/ // end group S862x890_P319x329

extern Pointer justbox;
extern Pointer passive, printednode;
extern HalfWord passnumber;
extern Scaled activewidth[6];
extern Scaled curactivewidth[6];
extern Scaled background[6];
extern Scaled breakwidth[6];
extern Boolean noshrinkerroryet, secondpass, finalpass;
extern Pointer curp;
extern Integer threshold;
extern Integer minimaldemerits[tightfit - veryloosefit + 1];
extern Integer minimumdemerits;
extern Pointer bestplace[tightfit - veryloosefit + 1];
extern Scaled discwidth, firstwidth, secondwidth, firstindent, secondindent;
extern HalfWord easyline, lastspecialline;

extern void linebreak(long finalwidowpenalty);
extern HalfWord finiteshrink(HalfWord p);
extern void trybreak(long pi, SmallNumber breaktype);


extern Pointer bestbet, ha, hb, initlist, curq, ligstack;
extern Integer fewestdemerits;
extern HalfWord bestline;
extern Integer actuallooseness, linediff;

extern void postlinebreak(long finalwidowpenalty);
extern void hyphenate(void);


#endif /* INC_LINEBREAK_H */