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

extern void print_err(StrNumber s);
extern void print_err_str(Str s);
extern void error(void);
extern void int_error(Integer n);
extern void normalize_selector(void);
extern void succumb(void);
extern void fatalerror(StrNumber s);
extern void overflow(StrNumber s, Integer n);
extern void confusion(StrNumber s);
extern void pause_for_instructions(void);

#endif // INC_ERROR_H