#include <stdio.h>  // fprintf, stderr
#include <string.h> // strlen
#include "tex.h" // true, false
#include "str.h" // [export] str_pool_init
    // [macro] STRING_VACANCIES,
    // [func] str_adjust_to_room, makestring
#include "pool_str.c" // [var] pool_strs

/// $48
static void app_lc_hex(int l) {
    if (l < 10) {
        append_char(l + '0');
    } else {
        append_char(l - 10 + 'a');
    }
}

int str_pool_init(void) {
    int l;
    /* Make strings corresponding to single chars */
    /// #48
    for (int k = 0; k < 256; k++) {
        if ((k < ' ') || (k > '~')) {
            // Character k cannot be printed
            // 不可见字符，需要转义
            append_char('^');
            append_char('^');
            if (k < 64) {
                append_char(k + 64);
            } else if (k < 128) {
                append_char(k - 64);
            } else {
                app_lc_hex(k / 16);
                app_lc_hex(k % 16);
            }
        } else {
            // 可见字符
            append_char(k);
        }
        makestring();
    } // for (k = 0; k < 256; k++)

    /* Copy strings from 'pool_strs' to the pool */
    for (int k = 0; k < (int)(sizeof(pool_strs) / sizeof(char*)); k++) {
        const char* const sp = pool_strs[k];
        l = strlen(sp);
        if (l + STRING_VACANCIES != str_adjust_to_room(l + STRING_VACANCIES)) {
            fprintf(stderr, "! You have to increase POOLSIZE.\n");
            return false;
        }
        for (int i = 0; i < l; i++) {
            append_char(sp[i]);
        }
        makestring();
    }
    return true;
}
