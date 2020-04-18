#include <stdio.h>  // FILE, EOF, stderr
#include "tex.h"    // [type] Boolean, [macro] true, false, BUF_SIZE
#include "global.h"
    // [var] buffer, last, first, max_buf_stack
    //  _JMP_global__final_end, format_ident, cur_input, xord
#include "texfunc.h" // [func] overflow
#include "inputln.h" // [export] inputln

// [#31] inputs the next line or returns false
// 30, [31], 37, 58, 71, 362, 485, 486, 538
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
                    fprintf(stderr, "Buffer size exceeded!\n");
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
