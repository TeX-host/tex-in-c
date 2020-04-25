#pragma once
#ifndef INC_TEX_MATH
/** [ p38~41#99~109 ]: PART 7: ARITHMETIC WITH SCALED DIMENSIONS.
 *
 * export:
 *  + Boolean #arith_error
 *  + Scaled #tex_remainder
 *
 *  - Integer half(Integer x)
 *  - Scaled round_decimals(SmallNumber k, char digs[])
 *  - Scaled mult_and_add(Integer n, Scaled x, Scaled y, Scaled max_ans)
 *  - Scaled x_over_n(Scaled x, Integer n)
 *  - Scaled xn_over_d(Scaled x, Integer n, Integer d)
 *  - HalfWord badness(Scaled t, Scaled s)
 *
 *  - void print_scaled(Scaled s)
 *
 */
#define INC_TEX_MATH
#include "tex_types.h"

// #101
#define UNITY   65536L  ///< 2^16, rep 1.00000
#define TWO     131072L ///< 2^17, rep 2.00000

/// [105] 455, 716, 1240. 
/// Note:  `07777777777L == 1073741823L`
#define nx_plus_y(n, x, y)      mult_and_add((n), (x), (y),  07777777777L)
/// [105] 1240.
/// Note: `017777777777L == 2147483647L`
#define mult_integers(n, x)     mult_and_add((n), (x),  0,  017777777777L)

extern Boolean arith_error;
extern Scaled tex_remainder;

extern Integer half(Integer x);
extern Scaled round_decimals(SmallNumber k, char digs[]);
extern void print_scaled(Scaled s);
// only used in [macro] nx_plus_y, mult_integers
extern Scaled mult_and_add(Integer n, Scaled x, Scaled y, Scaled max_ans);
extern Scaled x_over_n(Scaled x, Integer n);
extern Scaled xn_over_d(Scaled x, Integer n, Integer d);
extern HalfWord badness(Scaled t, Scaled s);

#endif // INC_TEX_MATH