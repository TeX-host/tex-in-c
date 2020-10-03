#pragma once
#ifndef DIV_OUT_H
/// DVI 格式输出
#define DIV_OUT_H
#include "global.h" // [const] UMAXOF, static_assert
#include "charset.h" // [type] ASCIICode
#include "fonts.h"  // [type] InternalFontNumber

/// [#11]: size of the output buffer; must be a multiple of 8
#define DVI_BUF_SIZE 800

/** @addtogroup S592x643_P220x238
 * @{
 */
/// [p229#619] label _not_use_
// #define movepast        13
// #define finrule         14
// #define nextp           15

#define synchh()                    \
    do {                            \
        dvih = synch_h(curh, dvih); \
    } while (0)

#define synchv()                    \
    do {                            \
        dviv = synch_v(curv, dviv); \
    } while (0)
/** @}*/ // end group S592x643_P220x238


extern Scaled maxh, maxv;
extern Integer totalpages, maxpush, deadcycles;
extern Boolean doingleaders;
extern Scaled dvih, dviv, curh, curv;
extern Scaled curmu;
extern Integer curs;

// [dviout], tex
extern void dviout(int x);
extern void dvi_four(Integer x);
extern void dvi_pop(Integer l);
extern void dvi_font_def(InternalFontNumber f);
extern void prune_movements(Integer l);
extern Scaled synch_h(Scaled cur_h, Scaled dvi_h);
extern Scaled synch_v(Scaled cur_v, Scaled dvi_v);
extern void dviout_helper(ASCIICode c);

// help func
extern void dviout_ID_BYTE(void);
extern void dviout_SET_RULE(void);
extern void dviout_PUT_RULE(void);
extern void dviout_EOP(void);
extern void dviout_PUSH(void);
extern void dviout_POP(void);
extern void dviout_PRE(void);
extern void dviout_POST(void);
extern void dviout_XXX1(void);
extern void dviout_XXX4(void);

extern long get_dvi_mark(void);
extern Boolean dvi_openout(void);
extern void _dvibop(long counts[]);
extern void _dvi_lastbop(void);
extern void dvi_set_font(int f);
extern void dvi_set_char(long c);
extern void dviout_init(void);
extern long dviflush(void);

extern void hlistout(void);
extern void vlistout(void);
extern void shipout(Pointer p);

#endif // #ifndef DIV_OUT_H