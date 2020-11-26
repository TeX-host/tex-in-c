#include <string.h> // memcmp, strlen
#include "error.h"   // [func] overflow,
#include "hash.h"    // [func] idlookup_p
#include "print.h"   // [func] print, print_char
#include "io.h"      // [macro] TERM_ERR
#include "str_pool_init.h" // [var] pool_strs
#include "dump.h"          // [macro]
#include "str.h"    // [export]

#define str_end(x) (str_start[(x) + 1])


/** @addtogroup S38x53_P19x23
 * @{
 */

/// [#39] the characters.
static ASCIICode    str_pool[POOL_SIZE + 1];
/// [#39] the starting pointers.
static PoolPtr      str_start[MAX_STRINGS + 1];
/// [#39] first unused position in #str_pool.
static PoolPtr      pool_ptr;
/// [#39] number of the current string being created.
static StrNumber    str_ptr;
/// [#39] the starting value of #pool_ptr.
static PoolPtr      init_pool_ptr;
/// [#39] the starting value of #str_ptr.
static StrNumber    init_str_ptr;


/// [#40]: the number of characters in string number x.
int str_length(StrNumber x) { return str_end(x) - str_start[(x)]; }

/// [#41]: The length of the current string.
int cur_length() { return pool_ptr - str_start[str_ptr]; }

/// [#42]: put ASCII code s at the end of #str_pool.
void append_char(ASCIICode s) {
    str_room(1);
    POOL_ELEM(pool_ptr, 0) = s;
    pool_ptr++;
}

/// [#42]: forget the last character in the pool.
void flush_char(void) { pool_ptr--; }

/// [#42]: make sure that the pool hasn’t overflowed.
void str_room(StrNumber l) { //            "pool size"
    if ((pool_ptr + l) > POOL_TOP) overflow(S(1276), POOL_TOP - init_pool_ptr);
}

/** [#43]: current string enters the pool.
 * Once a sequence of characters has been appended to str pool,
 *  it officially becomes a string 
 *  when the function make string is called.
 * 
 * This function returns 
 *  the identification number of the new string as its value.
 */
StrNumber makestring(void) { //         "number of strings"
    if (str_ptr == MAX_STRINGS) overflow(S(1277), MAX_STRINGS - init_str_ptr);

    str_ptr++;
    str_start[str_ptr] = pool_ptr;
    return (str_ptr - 1);
} // #43: makestring

/// [#44]: destroy the most recently made string.
void flush_string(void) {
    str_ptr--;
    pool_ptr = str_start[str_ptr];
} // #44: flush_string

/// 比较两个 str 的差别。返回 0 为相同，非 0 为有差异。
int str_cmp(StrNumber s, StrNumber t) {
    PoolPtr j = str_start[s];
    PoolPtr k = str_start[t];
    int l = 0, dif;

    while (l < str_length(s) && l < str_length(t)) {
        dif = POOL_ELEM(j, l) - POOL_ELEM(k, l);
        if (dif) {
            return dif;
        }
        l++;
    }
    return str_length(s) - str_length(t);
}

/// [p21#46]: test equality of strings。
Boolean str_eq_str(StrNumber s, StrNumber t) {
    if (str_length(s) != str_length(t)) {
        return false;
    }
    return !str_cmp(s, t);
} // #46: str_eq_str

#ifdef tt_INIT
/// [#47]: initializes the string pool, 
///  but returns false if something goes wrong.
/// called by: main()
Boolean get_strings_started(void) {
    #if POOLPOINTER_IS_POINTER
        pool_ptr = str_pool;
    #else
        pool_ptr = 0;
    #endif

    str_ptr = 0;
    str_start[0] = pool_ptr;
    return str_pool_init();
} // [#47] get_strings_started
#endif // #47: tt_INIT

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
/** @}*/ // end group S38x53_P19x23.


/*
# Basic printing procedures:
    57, 58, 59, [60], 62, 
    63, 64, 65, 262, 263, 
    518, 699, 1355.
    slow_print, 

    大多数放在 tex.c 中
*/
/// #60: prints string s
void slow_print(StrNumber s) {
    if (s >= str_ptr || s < 256) {
        print(s);
    } else {
        // 256 <= s <= str_ptr
        for (PoolPtr j = str_start[s]; j < str_end(s); j++) {
            print(POOL_ELEM(j, 0));
        }
    }
} // #60: slow_print

/// [#70] prints a yet-unmade string.
void printcurrentstring(void) {
    PoolPtr j;

    j = str_start[str_ptr];
    while (j < pool_ptr) {
        print_char(POOL_ELEM(j, 0));
        j++;
    }
}


int str_getc(StrNumber s, int k) { return POOL_ELEM(str_start[s], k); }

void str_set_init_ptrs(void) {
    init_str_ptr = str_ptr;
    init_pool_ptr = pool_ptr;
}

StrPoolPtr str_mark(void) {
    StrPoolPtr res;
    res.val = pool_ptr;
    return res;
}

void str_map_from_mark(StrPoolPtr b, void (*f)(ASCIICode)) {
    PoolPtr k = b.val;
    while (k < pool_ptr) {
        f(POOL_ELEM(k, 0));
        k++;
    }
    pool_ptr = b.val;
}

void str_print_stats(FILE* f_log_file) {
    fprintf(f_log_file, " %ld string", str_ptr - init_str_ptr);
    if (str_ptr != init_str_ptr + 1) {
        fprintf(f_log_file, "s");
    }
    fprintf(f_log_file, " out of %ld\n", (long)(MAX_STRINGS - init_str_ptr));
    fprintf(f_log_file,
            " %ld string characters out of %ld\n",
            (long)(pool_ptr - init_pool_ptr),
            (long)(POOL_TOP - init_pool_ptr));
}

int str_undump(FILE* fmt_file, FILE* _not_use_) {
    long x;
    /*:1308*/
    x = undump_int();
    if (x < 0) goto _Lbadfmt_;
    if (x > POOL_SIZE) {
        fprintf(TERM_OUT, "---! Must increase the string pool size\n");
        goto _Lbadfmt_;
    }
    pool_ptr = (PoolPtr)x;
    x = undump_int();
    if (x < 0) goto _Lbadfmt_;
    if (x > MAX_STRINGS) {
        fprintf(TERM_OUT, "---! Must increase the max strings\n");
        goto _Lbadfmt_;
    }
    str_ptr = x;
    fread(str_start, 1, sizeof(str_start[0]) * (str_ptr + 1), fmt_file);
    fread(str_pool, 1, sizeof(str_pool[0]) * (long)pool_ptr, fmt_file);
    init_str_ptr = str_ptr;
#if POOLPOINTER_IS_POINTER
    for (x = 0; x <= str_ptr; x++) {
        str_start[x] = str_pool + (long)str_start[x];
    }
    pool_ptr = str_pool + (long)pool_ptr;
#endif
    init_pool_ptr = pool_ptr; /*:1310*/
    return 1;
_Lbadfmt_:
    return 0;
}

void str_dump(FILE* fmt_file) {
    long poolused;
#if POOLPOINTER_IS_POINTER
    /* Convert for dumping */
    long x;
    for (x = 0; x <= str_ptr; x++) {
        str_start[x] = (PoolPtr)(str_start[x] - str_pool);
    }
    poolused = pool_ptr - str_pool;
#else
    poolused = pool_ptr;
#endif

    dump_int(poolused);
    dump_int(str_ptr);
    fwrite(str_start, 1, sizeof(str_start[0]) * (str_ptr + 1), fmt_file);
    fwrite(str_pool, 1, sizeof(str_pool[0]) * poolused, fmt_file);
#if POOLPOINTER_IS_POINTER
    /* Pool is still in use, need to convert back */
    for (x = 0; x <= str_ptr; x++) {
        str_start[x] = (PoolPtr)(str_pool + (long)str_start[x]);
    }
#endif
    println();
    print_int(str_ptr);
    print(S(1275));
    print_int(poolused);
    /*:1309*/
}

Integer str_adjust_to_room(Integer len) {
    if ((pool_ptr + len) > POOL_TOP) {
        return POOL_TOP - pool_ptr;
    } else {
        return len;
    }
}

void str_cur_map(void (*f)(ASCIICode)) {
    for (PoolPtr s = str_start[str_ptr]; s < pool_ptr; s++) {
        f(POOL_ELEM(s, 0));
    }
    pool_ptr = str_start[str_ptr];
}

// 辅助函数对字符串的每一个字符调用 f
void str_map(StrNumber str, void (*f)(ASCIICode)) {
    for (PoolPtr p = str_start[str]; p < str_end(str); p++) {
        f(POOL_ELEM(p, 0));
    }
}

// void print_char_helper(ASCIICode c) { print_char(c); } // _not_use_

// 辅助函数。自带 `s >= str_ptr` 检查
// `print` 依赖的的内部实现
void str_print(StrNumber s) {
    if (s < 0 || s >= str_ptr) s = S(261); // "???"
    str_map(s, print_char);
}

static void f_pool_helper(ASCIICode c) { fputc(c, TERM_ERR); }

void f_pool(StrNumber s) {
    if (s >= str_ptr || s < 0) s = 274;
    str_map(s, f_pool_helper);
    fputc('\n', TERM_ERR);
    fflush(TERM_ERR);
}

StrNumber idlookup_s(StrNumber s, Boolean no_new_cs) {
    PoolPtr k = str_start[s];
    int l = str_length(s);
    return idlookup_p(&POOL_ELEM(k, 0), l, no_new_cs);
}




int str_valid(StrNumber s) { return ((s >= 0) && (s < str_ptr)); }



int str_scmp(StrNumber s, short* buffp) {
    PoolPtr u = str_start[s];
    int hn = str_length(s);
    int j = 0;
    do {
        if (POOL_ELEM(u, j) < buffp[j]) return -1;
        if (POOL_ELEM(u, j) > buffp[j]) return 1;
        j++;
    } while (j < hn); /*932:*/
    return 0;
}

Boolean str_bcmp(ASCIICode buffp[], long l, StrNumber s) {
    return str_length(s) == l 
        && !memcmp(&POOL_ELEM(str_start[s], 0), buffp, l);
}

StrNumber str_ins(short* buffp, long l) {
    int k;
    str_room(l);
    for (k = 0; k < l; k++) {
        append_char(buffp[k]);
    }
    return makestring();
}

StrNumber str_insert(ASCIICode buffp[], Integer l) {
    int d, k;
    StrNumber result;

    str_room(l);
    d = cur_length();
    while (pool_ptr > str_start[str_ptr]) {
        pool_ptr--;
        POOL_ELEM(pool_ptr, l) = POOL_ELEM(pool_ptr, 0);
    }
    for (k = 0; k < l; k++) {
        append_char(buffp[k]);
    }
    result = makestring();
    pool_ptr += d;
    return result;
}
