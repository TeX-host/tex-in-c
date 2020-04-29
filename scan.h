#pragma once
#ifndef INC_SCAN_H
/** digest certain kinds of patterns in the input.
 *
 *  + @ref S402x463 "PART 26: BASIC SCANNING SUBROUTINES"
 */
#define INC_SCAN_H

#define intval          0
#define dimenval        1
#define glueval         2
#define muval           3
#define identval        4
#define tokval          5

/// [ #402~463: PART 26: BASIC SCANNING SUBROUTINES ]
// [#410] curval
Integer curval;
// [#410] the “level” of this value
char curvallevel;
/*438:*/
SmallNumber radix;
/*:438*/
/*447:*/
GlueOrd curorder;
/*:447*/

extern void skip_spaces(void);
extern void skip_spaces_or_relax(void);
extern void scanleftbrace(void);
extern void scanoptionalequals(void);
extern Boolean scankeyword(StrNumber s);
extern void muerror(void);
extern void scaneightbitint(void);
extern void scancharnum(void);
extern void scanfourbitint(void);
extern void scanfifteenbitint(void);
extern void scantwentysevenbitint(void);


#endif // INC_SCAN_H