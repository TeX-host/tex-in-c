#pragma once
#ifndef INC_PAGE_H
//
#define INC_PAGE_H
#include "lexer.h" // [var] nest; [macro] ignoredepth
#include "tex_types.h"

/** @addtogroup S967x979_P360x365
 * @{
 */
/// [p361#970] new name for the six distance variables
#define activeheight        activewidth
/// [p361#970] the natural height
#define curheight           (activeheight[0])
/// [p361#970] initialize the height to zero.
#define setheightzero(x)    (activeheight[(x)-1] = 0)
// [p361#970]
#define updateheights       90

// [p363#974]
#define deplorable          100000L
/** @}*/ // end group S967x979_P360x365

/** @addtogroup S980x1028_P366x382
 * @{
 */
// [p366#980]
#define insertsonly     1
#define boxthere        2
// [p367#981]
#define pageinsnodesize  4
#define inserting        0
#define splitup          1
// [p371#994]
#define contribute      80

/// [p367#981] an insertion for this class will break here if anywhere
#define brokenptr(x)    link(x+1)
#define brokenins(x)    info(x+1) /* this insertion might break at |brokenptr|}*/
#define lastinsptr(x)   link(x+2) /* the most recent insertion for this |subtype|}*/
#define bestinsptr(x)   info(x+2) /* the optimum most recent insertion}*/

/// [p369#987]
#define setpagesofarzero(x) (pagesofar[(x)] = 0)

/// [p368#982] desired height of information on page being built
#define pagegoal    (pagesofar[0])
#define pagetotal   (pagesofar[1])  /*height of the current page*/
#define pageshrink  (pagesofar[6]) /*shrinkability of the current page*/
#define pagedepth   (pagesofar[7])  /*depth of the current page*/

/// [p371#995]
#define contribtail (nest[0].tailfield) /*tail of the contribution list*/
/** @}*/ // end group S980x1028_P366x382


extern Pointer pagetail;
extern char pagecontents;
extern Scaled pagesofar[8];
extern Pointer lastglue;
extern Scaled lastkern;
extern Integer lastpenalty;
extern Integer insertpenalties;
extern Boolean outputactive;

extern void page_builder_init();
extern void page_builder_init_once();
// lexer
extern void printtotals(void);
extern void boxerror(EightBits n);
extern void buildpage(void);


extern Scaled bestheightplusdepth;
extern HalfWord prunepagetop(HalfWord p);
extern HalfWord vertbreak(HalfWord p, long h, long d);
extern HalfWord vsplit(EightBits n, long h);

#endif /* INC_PAGE_H */