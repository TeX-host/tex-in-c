#pragma once
#ifndef INC_CHARSET_H
///
#define INC_CHARSET_H
#include "tex_types.h" // [type] UChar

/** @addtogroup S17x24_P10x12
 * @{
 */
/// [#18]: `ASCIICode::UChar(8)` = [0, 255]
typedef UChar ASCIICode;
/// [#19]: `TextChar::UChar(8)` = [0, 255]
typedef UChar TextChar;

/// [#19] ordinal number of the smallest element of #TextChar.
#define FIRST_TEXT_CHAR 0
/// [#19] ordinal number of the largest element of #TextChar.
#define LAST_TEXT_CHAR  255

/// [#22] ASCII code that might disappear.
#define NULL_CODE       0
/// [#22] ASCII code used at end of line.
#define CARRIAGE_RETURN '\r'
/// [#22] ASCII code that many systems prohibit in text files.
#define INVALID_CODE    127
/** @}*/ // end group S17x24_P10x12


extern ASCIICode xord[256];
extern TextChar xchr[256];
extern void charset_init();

#endif /* INC_CHARSET_H */