#include "texmath.h"

/*
 * [ p38~41#99~109 ]: PART 7: ARITHMETIC WITH SCALED DIMENSIONS
 *
 * export:
 *  + void print_scaled(Scaled s)
 *  + Scaled xn_over_d(Scaled x, long n, long d)
 *  +
 *
 * static:
 *  - Static Integer half(Scaled x)
 *  - Static Scaled round_decimals(int k, char digs[])
 *  - Static Scaled mult_and_add(long n, Scaled x, Scaled y, Scaled max_ans)
 *  - Static Scaled x_over_n(Scaled x, Scaled n)
 *  - Static HalfWord badness(Scaled t, Scaled s)
 * 
 */

