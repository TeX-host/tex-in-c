#include <stdio.h> // FILE, EOF, stderr
#include "charset.h"
#include "tex.h"   // [type] Boolean, [macro] true, false, BUF_SIZE
    // [var] buffer, last, first, max_buf_stack
    //  _JMP_global__final_end, format_ident, cur_input, xord
#include "lexer.h"   // [var] cur_input
#include "error.h"   // [func] overflow,
#include "dump.h"    // [var] format_ident
#include "io.h"     // [macro] TERM_ERR


/** @addtogroup S25x37_P13x18
 * @{
 */


/// [#26]: on some systems this may be a record variable.
Char name_of_file[FILE_NAME_SIZE + 1];
/** [#26]: this many characters are actually relevant in #name_of_file
 *  (the rest are blank).
 *
 *  [0, FILE_NAME_SIZE=240]
 */
UInt16 namelength;

/// [#30]: lines of characters being read
ASCIICode buffer[BUF_SIZE + 1];
/// [#30]: the first unused position in #buffer. [0, BUF_SIZE=5000]
UInt16 first;
/// [#30]: end of the line just input to #buffer. [0, BUF_SIZE=5000]
UInt16 last;
/// [#30]: largest index used in #buffer. [0, BUF_SIZE=5000]
UInt16 max_buf_stack;


/*28:*/
void aclose(FILE** f) {
    if (*f != NULL) fclose(*f);
    *f = NULL;
}

void w_close(FILE** f) {
    if (*f != NULL) fclose(*f);
    *f = NULL;
}
/*:28*/

/// [#31] inputs the next line or returns false.
/// 30, [31], 37, 58, 71, 362, 485, 486, 538
Boolean inputln(FILE* f, Boolean bypass_eoln) {
    UInt16 lastnonblank;
    int c, inp_c;

    if (bypass_eoln) {
        c = getc(f);
        if (c != '\n') ungetc(c, f);
    }
    last = first;

    c = getc(f);
    if (c == EOF) return false;
    ungetc(c, f);

    lastnonblank = first;
    // TODO: check `inp_c != '\n'`
    while ((inp_c = getc(f)) != EOF && inp_c != '\n') {
        if (last >= max_buf_stack) {
            max_buf_stack = last + 1;
            if (max_buf_stack == BUF_SIZE) {
                // #35: Report overflow of the input buffer, and abort
                if (format_ident == 0) {
                    fprintf(TERM_ERR, "Buffer size exceeded!\n");
                    longjmp(_JMP_global__final_end, 1);
                } else {
                    cur_input.locfield = first;
                    cur_input.limitfield = last - 1;
                    overflow(S(511), BUF_SIZE); // "buffer size"
                }
            }
        } // if (last >= max_buf_stack)

        buffer[last] = xord[inp_c];
        last++;
        if (buffer[last - 1] != ' ') lastnonblank = last;
    } // while ((inp_c = getc(f)) != EOF && inp_c != '\n')

    ungetc(inp_c, f); // ???
    last = lastnonblank;
    return true;
} // #31: inputln

/// [#37] gets the terminal input started.
Boolean initterminal(void) {
    if (initinc(1)) { // initinc@func.c
        LOC = first;
        return true;
    }
    while (true) {
        fprintf(TERM_OUT, "**");
        update_terminal();
        if (!inputln(TERM_IN, true)) {
            putc('\n', TERM_OUT);
            fprintf(TERM_OUT, "! End of file on the terminal... why?");
            return false;
        }
        LOC = first;
        while ((LOC < last) && (buffer[LOC] == ' '))
            LOC++;
        if (LOC < last) {
            return true;
        }
        fprintf(TERM_OUT, "Please type the name of your input file.\n");
    } // while (true) {
} // [#37] initterminal

/** @}*/ // end group S25x37_P13x18