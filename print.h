#pragma once
#ifndef INC_PRINT
/** print 输出相关函数 .
 *
 */
#define INC_PRINT
#include "tex_types.h" // Integer, UChar, ASCIICode
#include "tex.h"       // MAX_PRINT_LINE
#include "texmac.h"    // newlinechar
#include "str.h"       // StrNumber, str_adjust_to_room

typedef char* Str;

/** @addtogroup S54x71_P24x29
 * @{
 */

/** [p24#54]: #selector possible settings.
 * `enum Selector` = [16, 21]
 *
 * ## convenient relations
 *  + NO_PRINT + 1 = TERM_ONLY,
 *  + NO_PRINT + 2 = LOG_ONLY,
 *  + TERM_ONLY + 2 = LOG_ONLY + 1 = TERM_AND_LOG
 */
typedef enum _Selector {
    /* [0~15] write_file[16] */

    NO_PRINT = 16, ///< doesn't print at all.
    TERM_ONLY,     ///< prints only on the terminal.
    LOG_ONLY,      ///< prints only on the transcript file.
    TERM_AND_LOG,  ///< normal #selector setting.

    /// puts output into a cyclic buffer
    /// that is used by the show_context routine.
    PSEUDO = 20,
    /// appends the output to the current string in the string pool.
    NEW_STRING = 21,
    /// highest selector setting.
    /// NOTE: _not_use_, only used in assert
    MAX_SELECTOR = NEW_STRING,
} Selector; // [p24#54] enum Selector
/** @}*/ // end group S54x71_P24x29




extern FILE* log_file;
extern Selector selector;
extern Integer tally;
extern UChar term_offset;
extern UChar file_offset;
extern ASCIICode trick_buf[ERROR_LINE + 1];
extern Integer trick_count;
extern Integer first_count;

extern void println(void);
extern void print_char(ASCIICode s);
extern void print(StrNumber s);
extern void printnl(StrNumber s);
extern void print_esc(StrNumber s);
extern void print_the_digs(EightBits k, char dig[]);
extern void print_int(Integer n);
extern void print_two(Integer n);
extern void print_hex(Integer n);
extern void print_roman_int(Integer n);
extern void term_input(void);

// 直接输出 string 的函数
extern void print_str(Str s);
extern void printnl_str(Str s);
extern void print_esc_str(Str s);

#endif // INC_PRINT