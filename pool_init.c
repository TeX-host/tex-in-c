#include <stdio.h>  // fprintf, stderr
#include <string.h> // strlen
#include "tex.h" // true, false
#include "str.h" // [export] str_pool_init
    // [macro] STRING_VACANCIES,
    // [func] str_adjust_to_room, makestring
#include "pool_init.h" // [var] pool_strs


/// [#48] int to hex
static void app_lc_hex(UChar l) {
    if (0 <= l && l <= 9) {
        // [0, 9] => ['0', '9']
        append_char(l + '0');
    } else if (10 <= l && l <= 16) {
        // [10, 16] => ['a', 'f']
        append_char(l - 10 + 'a');
    }
}

/// [p21#47] Make the first 256 strings,
/// and copy all string in pool_strs[] to string pool.
Boolean str_pool_init(void) {
    /// length of pool_strs[].
    const size_t POOL_STRS_LEN = sizeof(pool_strs) / sizeof(char*);
    size_t str_len; // length of string in pool_strs[].

    /* Make strings corresponding to single chars */
    /// [#48] Make the first 256 strings.
    /// ASCII 输入表见 TeXBook: Appendix C: Character Codes.
    for (int k = 0; k < 256; k++) {
        if ((k < ' ') || (k > '~')) {
            // Character k cannot be printed
            // 不可见字符，需要转义
            append_char('^');
            append_char('^');

            if (k < 64) {
                // [0, 31] => [64, 95] => ['@', '_']
                append_char(k + 64);
            } else if (k < 128) {
                // [127] => [63] => ['?']
                append_char(k - 64);
            } else { 
                // [128, 255] => ['80', 'ff']
                app_lc_hex(k / 16);
                app_lc_hex(k % 16);
            }
        } else {
            // [32, 126] => [' ', '~'] 可见字符
            append_char(k);
        }
        makestring();
    } /* for (int k = 0; k < 256; k++) */

    /** [#52] Copy strings from 'pool_strs[]' to the pool
     * 
     */
    for (size_t k = 0; k < POOL_STRS_LEN; k++) {
        const char* sp = pool_strs[k];

        str_len = strlen(sp);
        // make sure there are enough spaces.
        if ((str_len + STRING_VACANCIES) !=
            str_adjust_to_room(str_len + STRING_VACANCIES)) {
            fprintf(TERM_ERR, "! You have to increase POOLSIZE.\n");
            return false;
        }

        // add string
        for (size_t i = 0; i < str_len; i++) {
            append_char(sp[i]);
        }
        makestring();
    } /* for (size_t k = 0; k < POOL_STRS_LEN; k++) */

    return true;
} /* str_pool_init */
