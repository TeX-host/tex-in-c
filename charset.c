#include "charset.h"

/** @addtogroup S17x24_P10x12
 * @{
 */

/// [#20] specifies conversion of input characters.
ASCIICode   xord[256];
/// [#20] specifies conversion of output characters.
TextChar    xchr[256];

// [#21, #23, #24]
void charset_init() {
    // Local variables for initialization
    Integer i;

    /// [#21]
    for (i = 0; i <= 255; i++) {
        xchr[i] = (TextChar)i;
    }

#ifdef tt_USE_TEX82
    /// [#23]
    // [0, 31]
    for (i = 0; i <= 037; i++) {
        xchr[i] = ' ';
    }
    // [127, 255]
    for (i = 0177; i <= 0377; i++) {
        xchr[i] = ' ';
    }
#endif /* tt_USE_TEX82 */

    /// [#24]
    for (i = FIRST_TEXT_CHAR; i <= LAST_TEXT_CHAR; i++) {
        xord[i] = INVALID_CODE;
    }
    for (i = 128; i <= 255; i++) {
        xord[xchr[i]] = i;
    }
    for (i = 0; i <= 126; i++) {
        xord[xchr[i]] = i;
    }
} /* charset_init */

/** @}*/ // end group S17x24_P10x12