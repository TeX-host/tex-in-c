#pragma once
#ifndef INC_ERROR_H
/// 
#define INC_ERROR_H
#include "global.h"    // [type] Integer
#include "str.h"       // [type] StrNumber

/** @addtogroup S72x98_P30x37
 * @{
 */

/** [p30#73]: four settings, 
 *  representing increasing amounts of user interaction.
 * 
 * [REPORTING ERRORS]
 */
typedef enum _UserInteractionMode {
    BATCH_MODE,      ///< omits all stops and omits terminal output.
    NON_STOP_MODE,   ///< omits all stops.
    SCROLL_MODE,     ///< omits error stops.
    ERROR_STOP_MODE, ///< stops at every opportunity to interact.
} UserInteractionMode; // [#73] enum UserInteractionMode

/** [p31#76]: four possible values for
 *  the worst level of error that has been detected.
 * 
 * [REPORTING ERRORS]
 */
typedef enum _ErrorLevel {
    SPOTLESS = 0,         ///< nothing has been amiss yet.
    WARNING_ISSUED,       ///< begin_diagnostic has been called.
    ERROR_MESSAGE_ISSUED, ///< error has been called.
    FATAL_ERROR_STOP,     ///< termination was premature.
} ErrorLevel; // [#76] enum ErrorLevel

/// [#79]
// tex only
#define help1(x1) set_help(1, x1)
#define help2(x1, x2) set_help(2, x1, x2)
#define help3(x1, x2, x3) set_help(3, x1, x2, x3)
// fonts, tex
#define help4(x1, x2, x3, x4) set_help(4, x1, x2, x3, x4)
#define help5(x1, x2, x3, x4, x5) set_help(5, x1, x2, x3, x4, x5)
// tex only
#define help6(x1, x2, x3, x4, x5, x6) set_help(6, x1, x2, x3, x4, x5, x6)


/// [p36#96]
#define checkinterrupt() ((interrupt != 0) ? (pause_for_instructions(), 0) : 0)
/** @}*/ // end group S72x98_P30x37


extern UChar interaction;
extern Boolean deletions_allowed;
extern Boolean set_box_allowed;
extern ErrorLevel history;
extern SChar errorcount;
extern StrNumber help_line[6];
extern UChar help_ptr;
extern Boolean use_err_help;
extern Integer interrupt;
extern Boolean OK_to_interrupt;

extern void set_help(SChar k, ...);
extern void error_init();
extern void error_selector_init();
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