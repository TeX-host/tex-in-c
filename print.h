#pragma once
#ifdef USE_SPLIT_MOD
#ifndef INC_PRINT
/** 
 *
 */
#define INC_PRINT
#include "tex_types.h" // Integer, UChar, ASCIICode
#include "str.h"       // StrNumber

/// [p24#54]: On-line and off-line printing.
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

/// [p24#54]: On-line and off-line printing
FILE* log_file = NULL;  // transcript of TeX session
enum Selector selector; // where to print a message
// ? dig[23] // digits in a number being output
Integer tally; // the number of characters recently printed
// the number of characters on the current terminal line
// term_offset = [0, MAX_PRINT_LINE=79]
UChar term_offset;
static_assert(UMAXOF(UChar) >= MAX_PRINT_LINE,
              "term_offset = [0, MAX_PRINT_LINE=79]");
// the number of characters on the current file line
// file_offset = [0, MAX_PRINT_LINE=79]
UChar file_offset;
static_assert(UMAXOF(UChar) >= MAX_PRINT_LINE,
              "file_offset = [0, MAX_PRINT_LINE=79]");
// circular buffer for pseudoprinting
ASCIICode trick_buf[ERROR_LINE + 1];
Integer trick_count; // threshold for pseudoprinting, explained later
Integer first_count; // another variable for pseudoprinting


void println(void);
void print_char(ASCIICode s);
void print(StrNumber s);
void printnl(StrNumber s);
void print_esc(StrNumber s);
void print_the_digs(EightBits k, char dig[]);
void print_int(Integer n);
void print_two(Integer n);
void print_hex(Integer n);
void print_roman_int(Integer n);

#endif // INC_PRINT
#endif // USE_SPLIT_MOD