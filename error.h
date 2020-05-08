#pragma once
#ifndef INC_ERROR_H
/// 
#define INC_ERROR_H

/** @addtogroup S72x98_P30x37
 * @{
 */

/// [p36#96]
#define checkinterrupt() ((interrupt != 0) ? (pause_for_instructions(), 0) : 0)
/** @}*/ // end group S72x98_P30x37


void error(void);
void int_error(long n);
void normalize_selector(void);
void succumb(void);
void fatalerror(StrNumber s);
void overflow(StrNumber s, Integer n);
void confusion(StrNumber s);
void pause_for_instructions(void);

#endif // INC_ERROR_H