#ifndef INC_MAIN_CTRL_H

#define INC_MAIN_CTRL_H
#include <stdlib.h>
#include <math.h>
#include "tex_types.h"
#include "tex_constant.h"
#include "fonts.h" // [macro] NON_CHAR
#include "io.h"
#include "str.h"
#include "mem.h"
#include "texmath.h" // [macro] UNITY; [var] arith_error; 
    // [func] nx_plus_y, mult_integers, half, print_scaled, x_over_n, xn_over_d
#include "eqtb.h"
#include "page.h"
#include "lexer.h" // [macro] inslist, ignoredepth, [var] partoken
#include "error.h"
#include "hyphen.h"
#include "macros.h"
#include "expand.h" // [var] readfile, readopen
#include "linebreak.h"
#include "printout.h"
#include "align.h"
#include "dviout.h"
#include "mmode.h" // [const] defaultcode
#include "pack.h"
#include "print.h"
#include "hash.h"
#include "scan.h" // [func] skip_spaces, MAX_DIMEN
#include "extension.h"


/** @addtogroup S1055x1135_P395x416
 * @{
 */
/// [p400#1071] context code for `\setbox0`.
#define boxflag         1073741824L
#define shipoutflag    (boxflag + 512)
#define leaderflag     (boxflag + 513)

// enum ChrCode {};
#define boxcode         0
#define copycode        1
#define lastboxcode     2
#define vsplitcode      3
#define vtopcode        4
/** @}*/ // end group S1055x1135_P395x416

/// [p420#1151]
#define faminrange ((curfam >= 0) && (curfam < 16))

/// [p437#1214]
/// 1218, 1241
#define define(x, y, z) \
    ((a >= 4) ? geqdefine((x), (y), (z)) : eqdefine((x), (y), (z)))
#define worddefine(x, y) \
    ((a >= 4) ? geqworddefine((x), (y)) : eqworddefine((x), (y)))


extern FontIndex maink;
extern Pointer mainp;
extern void main_ctrl_init();
extern void main_control(void);
extern void youcant(void);
extern void reportillegalcase(void);

// mode-indep-process
extern void mode_indep_init();
extern void mode_indep_init_once();

// build box and list
// build box, align, page builder
extern void normalparagraph(void);
// build box, build math, ext
extern SmallNumber normmin(long h);

/* build math */
extern void resumeafterdisplay(void);

/* indep porcess */
// align, build-box
extern void doassignments(void);


#endif /* INC_MAIN_CTRL_H */