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
#include "texmath.h"
#include "eqtb.h"
#include "page.h"
#include "lexer.h" // [macro] inslist, [var] partoken
#include "error.h"
#include "global.h"
#include "hyphen.h"
#include "macros.h"
#include "expand.h"
#include "linebreak.h"
#include "printout.h"
#include "align.h"
#include "dviout.h"
#include "mmode.h"
#include "pack.h"
#include "print.h"
#include "hash.h"
#include "scan.h" // [func] skip_spaces, MAX_DIMEN
#include "extension.h"


/** @addtogroup S1029x1054_P383x394
 * @{
 */


/** @}*/ // end group S1029x1054_P383x394

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