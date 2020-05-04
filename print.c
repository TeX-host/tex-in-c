#include <stdlib.h> // [func] labs, 
#include "global_const.h"
#include "print.h"
#include "global.h" // [func] xchr, eqtb, write_file


/** @addtogroup S54x71_P24x29
 * @{
 */

// [p24#54]: On-line and off-line printing
// ? dig[23]    // digits in a number being output
                // 作为函数参数

FILE* log_file = NULL;  ///< transcript of TeX session.
enum Selector selector; ///< where to print a message.
Integer tally; ///< the number of characters recently printed.

/// the number of characters on the current terminal line.
/// #term_offset = [0, MAX_PRINT_LINE=79]
UChar term_offset;
static_assert(UMAXOF(UChar) >= MAX_PRINT_LINE,
              "term_offset = [0, MAX_PRINT_LINE=79]");

/// the number of characters on the current file line.
/// #file_offset = [0, MAX_PRINT_LINE=79]
UChar file_offset;
static_assert(UMAXOF(UChar) >= MAX_PRINT_LINE,
              "file_offset = [0, MAX_PRINT_LINE=79]");

/// circular buffer for pseudoprinting.
ASCIICode trick_buf[ERROR_LINE + 1];
Integer trick_count; ///< threshold for pseudoprinting, explained later.
Integer first_count; ///< another variable for pseudoprinting.


/*
    #54. On-line and off-line printing.

# Basic printing procedures[13]:
    [57], [58], [59],   60,   [62],
    [63], [64], [65], 

*/

/** [p25#57]: 输出换行 . prints an end-of-line.
 *
 * 全局变量:
 *  + [in]  #selector
 *  + [out] #write_file
 *
 * print.h 导出变量:
 *  + [out] #log_file
 *  + [out] #term_offset
 *  + [out] #file_offset
 */
void println(void) {
    switch (selector) {
        case TERM_AND_LOG:
            putc('\n', stdout);
            putc('\n', log_file);
            term_offset = 0;
            file_offset = 0;
            break;

        case LOG_ONLY:
            putc('\n', log_file);
            file_offset = 0;
            break;

        case TERM_ONLY:
            putc('\n', stdout);
            term_offset = 0;
            break;

        case NO_PRINT:
        case PSEUDO:
        case NEW_STRING:
            /* blank case */
            break;

        default: 
            putc('\n', write_file[selector]); 
            break;
    } // switch (selector)
} // #57: println

/** [p25#58]: 输出单个 ASCII 字符 . prints a single character.
 *
 * 根据 #selector 选择输出位置.
 *
 * 全局变量:
 *  + [in]  #selector
 *  + [in]  #xchr
 *  + [out] #write_file
 *
 * print.h 导出变量:
 *  + [out] #log_file
 *  + [out] #term_offset
 *  + [out] #file_offset
 *  + [out] #tally
 *  + [out] #trick_count
 *  + [out] #trick_buf
 */
void print_char(ASCIICode c) {
    if (newlinechar == c && selector < PSEUDO) {
        println();
        return;
    }

    switch (selector) {
        case TERM_AND_LOG:
            putc(xchr[c], stdout);
            putc(xchr[c], log_file);
            // fwrite(&xchr[s], 1, 1, stdout);
            // fwrite(&xchr[s], 1, 1, log_file);
            term_offset++;
            file_offset++;
            if (term_offset == MAX_PRINT_LINE) println();
            if (file_offset == MAX_PRINT_LINE) println();
            break;

        case LOG_ONLY:
            putc(xchr[c], log_file);
            // fwrite(&xchr[s], 1, 1, log_file);
            file_offset++;
            if (file_offset == MAX_PRINT_LINE) println();
            break;

        case TERM_ONLY:
            putc(xchr[c], stdout);
            // fwrite(&xchr[s], 1, 1, stdout);
            term_offset++;
            if (term_offset == MAX_PRINT_LINE) println();
            break;

        case NO_PRINT:
            /* blank case */
            break;

        case PSEUDO:
            if (tally < trick_count) {
                trick_buf[tally % ERROR_LINE] = c;
            }
            break;

        case NEW_STRING:
            append_char(c); // we drop characters if the string space is full
            break;

        default:
            putc(xchr[c], write_file[selector]);
            // fwrite(&xchr[s], 1, 1, write_file[selector]); 
            break;
    } // switch (selector)
    tally++;
} // #58: print_char

/** [p26#59]: prints string `s` .
 *
 * 全局变量:
 *  + [in]  #selector
 *  + [in]  #eqtb       由 #newlinechar 间接引用
 */
void print(StrNumber s) {
    long nl; // new-line character to restore

    if (0 <= s && s <= 255) {
        if (selector > PSEUDO) {
            print_char(s); // internal strings are not expanded
            return;
        }
        if (newlinechar == s && selector < PSEUDO) { // #244
            println();
            return;
        }

        // temporarily disable new-line character
        nl = newlinechar;
        newlinechar = -1;
        str_print(s);
        newlinechar = nl;
    } else {
        // s < 0 || s > 255
        // 错误处理在 str_print 函数内部
        str_print(s);
    } // if (0 <= s && s <= 255) - else
} // #59: print

// #60: `slow_print` 位于 str.c 中

/** [#62]: prints string s at beginning of line.
 *
 * 全局变量:
 *  + [in]  #selector
 *
 * print.h 导出变量:
 *  + [out] #term_offset
 *  + [out] #file_offset
 */
void printnl(StrNumber s) {
    if (    (term_offset > 0 && (selector % 2)) 
        ||  (file_offset > 0 && selector >= LOG_ONLY) ) {
        println();
    }
    print(s);
} // #62: printnl

/** [#63]: prints escape character, then `s` .
 * 
 */
void print_esc(StrNumber s) {
    Integer c = ESCAPE_CHAR; // the escape character code

    if (0 <= c && c <= 255) {
        print(c);
    }
    slow_print(s);
} // #63: print_esc

/** [#64]: prints `dig[k − 1]...dig[0]` .
 * 
 * `dig[k] = [0, 15]` (hex value: `0~F`)
 */
void print_the_digs(EightBits k, char dig[]) {
    while (k > 0) {
        k--;
        if (dig[k] < 10) { // 0~9
            print_char('0' + dig[k]);
        } else { // A~F
            print_char('A' - 10 + dig[k]);
        }
    } // while (k > 0)
} // #64: print_the_digs

/** [#65]: prints an integer in decimal form.
 * 
 */
void print_int(Integer n) {
    // index to current digit; we assume that n < 10^23
    // k = [0, 23]
    int k = 0;
    // used to negate n in possibly dangerous cases
    Integer m;
    char dig[23];

    if (n < 0) {
        print_char('-');
        if (n > -100000000L) {
            n = -n;
        } else {
            m = -1 - n;
            n = m / 10;
            m = (m % 10) + 1;
            k = 1;
            if (m < 10) {
                dig[0] = m;
            } else {
                dig[0] = 0;
                n++;
            }
        } // if (n > -100000000L) - else
    } // if (n < 0)

    do {
        dig[k] = n % 10;
        n /= 10;
        k++;
    } while (n != 0);
    print_the_digs(k, dig);
} // #65: print_int

/** [#69]: 打印两位数字 `(0 <= n <= 99)`.
 *
 */
void print_two(Integer n) {
    n = labs(n) % 100;
    print_char('0' + n / 10);
    print_char('0' + n % 10);
} // #66 print_two

/** [#67]: 打印十六进制的非负整数 `(n >= 0)`.
 * 
 */
void print_hex(Integer n) {
    UChar k = 0; // [0, 22], 0<= n <= 16^22
    char digs[23];

    print_char('"');
    do {
        digs[k] = n % 16;
        n /= 16;
        k++;
    } while (n != 0);
    print_the_digs(k, digs);
} // #67: print_hex

/** [#69]: 打印罗马数字 .
 * 
 * Notice: 1990 => "mcmxc", not "mxm"
 */
void print_roman_int(Integer n) {
    int j, k;
    NonNegativeInteger u, v;
    const char romstr[] = "m2d5c2l5x2v5i";

    j = 0;
    v = 1000;
    while (true) {
        while (n >= v) {
            print_char(romstr[j]);
            n -= v;
        }
        // nonpositive input produces no output
        if (n <= 0) break;

        k = j + 2;
        u = v / (romstr[k - 1] - '0');
        if (romstr[k - 1] == '2') {
            k += 2;
            u /= romstr[k - 1] - '0';
        }
        if (n + u >= v) {
            print_char(romstr[k]);
            n += u;
        } else {
            j += 2;
            v /= romstr[j - 1] - '0';
        }
    } // while (true)
} // #69: print_roman_int

/** @}*/ // end group S54x71_P24x29