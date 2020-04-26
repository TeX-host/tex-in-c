#pragma once
#ifndef INC_PRINT
/** print 输出相关函数 .
 *
 */
#define INC_PRINT
#include "tex_types.h" // Integer, UChar, ASCIICode
#include "tex.h"       // MAX_PRINT_LINE
#include "texmac.h"    // newlinechar
#include "str.h"       // StrNumber

/// [p24#54]:  
/// `enum Selector` = [16, 21],
///     use by #selector
enum Selector {
    NO_PRINT = 16, ///< doesn't print at all
    TERM_ONLY,     ///< prints only on the terminal
    LOG_ONLY,      ///< prints only on the transcript file
    TERM_AND_LOG,  ///< normal #selector setting.

    /// puts output into a cyclic buffer
    /// that is used by the show context routine.
    PSEUDO = 20,
    /// appends the output to the current string in the string pool
    NEW_STRING = 21,
    /// highest selector setting.
    /// NOTE: _not_use_, only used in assert
    MAX_SELECTOR = NEW_STRING,
}; // [p24#54] enum Selector

extern FILE* log_file;
extern enum Selector selector;
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

#endif // INC_PRINT