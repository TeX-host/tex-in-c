#pragma once
#ifndef INC_SCAN_H
/** digest certain kinds of patterns in the input.
 *
 *  + @ref S402x463_P155x173 "PART 26: BASIC SCANNING SUBROUTINES"
 */
#define INC_SCAN_H
#include "lexer.h" // [macro] othertoken; [var] curcmd,
#include "str.h"   // [type] widthoffset
#include "tex_types.h"

/** @addtogroup S402x463_P155x173
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
#define SCAN_NORMAL_DIMEN() scan_dimen(false, false, false)
/** @}*/ // end group S402x463_P155x173


extern Integer cur_val;
extern SmallNumber cur_val_level;
extern SmallNumber radix;
extern GlueOrd cur_order;


extern void skip_spaces(void);
extern void skip_spaces_or_relax(void);
extern void scan_left_brace(void);
extern void scan_optional_equals(void);
extern Boolean scankeyword(StrNumber s);
// [tex] 1
extern void
scan_something_internal(SmallNumber level,
                      Boolean negative);
extern void scan_eight_bit_int(void);
extern void scan_char_num(void);
extern void scan_four_bit_int(void);
extern void scan_fifteen_bit_int(void);
extern void scan_twenty_seven_bit_int(void);
extern void scan_int(void);
extern void scan_dimen(Boolean mu, Boolean inf, Boolean shortcut);
extern void scan_glue(SmallNumber level);
extern HalfWord scan_rule_spec(void);

extern void scan_init();

#endif // INC_SCAN_H