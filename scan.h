#pragma once
#ifndef INC_SCAN_H
/** digest certain kinds of patterns in the input.
 *
 *  + @ref S402x463 "PART 26: BASIC SCANNING SUBROUTINES"
 */
#define INC_SCAN_H


/** @addtogroup S402x463
 * @{
 */

/// [#410]
// enum TokenLevel {};
#define intval      0 // integer values
#define dimenval    1 // dimension values
#define glueval     2 // glue specifications
#define muval       3 // math glue specifications
#define identval    4 // font identifier
#define tokval      5 // token lists
// [#416]
#define scaledbase  (dimenbase + dimenpars)
#define eqtbsize    (scaledbase + 255)
// [#438]
#define octaltoken  (othertoken + 39)
#define hextoken    (othertoken + 34)
#define alphatoken  (othertoken + 96)
#define pointtoken  (othertoken + 46)
#define continentalpointtoken   (othertoken + 44)
// [#445]
#define infinity    2147483647L //  the largest positive value that TEX knows
#define zerotoken   (othertoken  + 48)
#define Atoken      (lettertoken + 65)
#define otherAtoken (othertoken  + 65)
// [#448]
#define attachfraction  88
#define attachsign      89
// [#463]
#define defaultrule     26214 // 0.4 pt


/// [#410] curval
Integer curval;
/// [#410] the "level" of this value.
/// [intval=0, tokval=5]
SmallNumber curvallevel;
/// [#438] #scanint sets this to 8, 10, 16, or zero
SmallNumber radix;
/// [#447] order of infinity found by #scandimen
GlueOrd curorder;
/** @}*/ // end group S402x463


extern void skip_spaces(void);
extern void skip_spaces_or_relax(void);
extern void scanleftbrace(void);
extern void scanoptionalequals(void);
extern Boolean scankeyword(StrNumber s);
// extern void muerror(void);
// [tex] 1
extern void
scansomethinginternal(SmallNumber level,
                      Boolean negative);
extern void scaneightbitint(void);
extern void scancharnum(void);
extern void scanfourbitint(void);
extern void scanfifteenbitint(void);
extern void scantwentysevenbitint(void);
extern void scanint(void);
extern void scandimen(Boolean mu, Boolean inf, Boolean shortcut);
extern void scanglue(SmallNumber level);
extern HalfWord scanrulespec(void);


#endif // INC_SCAN_H