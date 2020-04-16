#include <stdio.h>  // FILE, EOF, stderr
#include "tex.h"    // [type] Boolean, [macro] true, false, bufsize
#include "global.h"
    // [var] buffer, last, first, max_buf_stack
    //  _JL_final_end, format_ident, cur_input, xord
#include "texfunc.h" // [func] overflow
#include "inputln.h" // [export] inputln


Boolean inputln(FILE* f, Boolean bypasseoln) {
    short lastnonblank;
    if (bypasseoln) {
        int c = getc(f);
        if (c != '\n') ungetc(c, f);
    }
    last = first;
    {
        int c = getc(f);
        if (c == EOF) return false;
        ungetc(c, f);
    }
    {
        int inp_c;
        lastnonblank = first;
        while ((inp_c = getc(f)) != EOF && inp_c != '\n') {
            if (last >= max_buf_stack) {
                max_buf_stack = last + 1;
                if (max_buf_stack == bufsize) { /*35:*/
                    if (format_ident == 0) {
                        fprintf(stderr, "Buffer size exceeded!\n");
                        longjmp(_JL_final_end, 1);
                    } else { /*:35*/
                        cur_input.locfield = first;
                        cur_input.limitfield = last - 1;
                        overflow(S(511), bufsize);
                    }
                }
            }
            buffer[last] = xord[inp_c];
            last++;
            if (buffer[last - 1] != 32) lastnonblank = last;
        }
        ungetc(inp_c, f);
        last = lastnonblank;
        return true;
    }
}
