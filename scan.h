#pragma once
#ifndef INC_SCAN_H
/** digest certain kinds of patterns in the input.
 *
 *  + @ref S402x463 "PART 26: BASIC SCANNING SUBROUTINES"
 */
#define INC_SCAN_H
#include "lexer.h" // [macro] othertoken; [var] curcmd,


/** @addtogroup S402x463
 * @{
 */

/// [#410]
enum TokenLevel {
    INT_VAL,   ///< integer values
    DIMEN_VAL, ///< dimension values
    GLUE_VAL,  ///< glue specifications
    MU_VAL,    ///< math glue specifications
    IDENT_VAL, ///< font identifier
    TOK_VAL,   ///< token lists
}; // [#410] enum TokenLevel

/// [#416] `\inputlineno`
#define INPUT_LINE_NO_CODE  (GLUE_VAL + 1)
/// [#416] `\badness`
#define BADNESS_CODE        (GLUE_VAL + 2)
/// [#421] @f$ 2^{30}-1 @f$
#define MAX_DIMEN   1073741823L
/// [#445] zero, the smallest digit
#define ZERO_TOKEN  (othertoken + '0')
// [#448] label
// /// go here to pack cur val and f into cur val
// #define attachfraction  88
// //// go here when cur val is correct except perhaps for sign
// #define attachsign      89

/// [#448] 
#define SCAN_NORMAL_DIMEN() scandimen(false, false, false)
/** @}*/ // end group S402x463


extern Integer cur_val;
extern SmallNumber cur_val_level;
extern SmallNumber radix;
extern GlueOrd cur_order;


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