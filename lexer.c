#include "print.h" // [var] tally,
#include "global.h" // [var] mem, lo_mem_max, hi_mem_min, mem_end,
#include "texfunc.h" // [func] print_cs
#include "lexer.h"


// [ #29~299: PART 21: INTRODUCTION TO THE SYNTACTIC ROUTINES  ]

// [#297]: current command set by `get_next`
//  a *command code* from the long list of codes given above;
EightBits curcmd;
// [#297]: operand of current command
//  a *character code* or other *modifier* of the command code;
HalfWord curchr;
// [#297]: control sequence found here, zero if none found
//  the `eqtb` location of the current control sequence
Pointer curcs;
// [#297]: packed representative of `curcmd` and `curchr`
HalfWord curtok;


// [ #300~320: PART 22: INPUT STACKS AND STATES ]

// [#301]:
InStateRecord inputstack[stacksize + 1];
// [#301]: first unused location of input stack
UChar inputptr;
// [#301]: largest value of input ptr when pushing
UChar maxinstack;
// [#301]: the "top" input state, according to convention (1)
// cur input: 35, 36, 87, 301, 302, 311, 321, 322, 534, 1131
InStateRecord cur_input;
// [#304] the number of lines in the buffer, less one
char inopen;
// the number of open text files
char openparens;
Integer line; // current line number in the current source file
FILE* inputfile[MAX_IN_OPEN];
Integer linestack[MAX_IN_OPEN];
// [#305] can a subfile end now?
char scanner_status;
// identifier relevant to non-normal scanner status
Pointer warning_index;
// reference count of token list being defined
Pointer defref;
// [#308] token list pointers for parameters
Pointer paramstack[paramsize + 1];
// first unused entry in param stack
int paramptr;
// largest value of param ptr, will be ≤ param size + 9
Integer maxparamstack;
// [#309] group level with respect to current alignment
Integer align_state;
// [#310] shallowest level shown by show context
UChar baseptr;

void init_lexer_var(void) {

} // init_lexer_var


/*
 * [ #289~296: PART 20: TOKEN LISTS ]
 */

// #292
void showtokenlist(Integer p, Integer q, Integer l) {
    Integer m, c;       // pieces of a token
    ASCIICode matchchr; // character used in a 'match'
    ASCIICode n;        // the highest parameter number, as an ASCII digit

    matchchr = '#';
    n = '0';
    tally = 0;
    while (p != 0 && tally < l) {
        if (p == q) {
            // #320 Do magic computation
            settrick_count();
        }

        // [#293] Display token p, and return if there are problems
        if (p < hi_mem_min || p > mem_end) {
            print_esc(S(308)); // "CLOBBERED."
            return;
        }
        if (info(p) >= CS_TOKEN_FLAG) {
            print_cs(info(p) - CS_TOKEN_FLAG);
        } else { // info(p) < CS_TOKEN_FLA
            m = info(p) / dwa_do_8;
            c = info(p) % dwa_do_8;
            if (info(p) < 0) {
                print_esc(S(309)); // "BAD."
            } else {
                // [#294] Display the token (m,c)
                switch (m) {
                    case LEFT_BRACE:
                    case RIGHT_BRACE:
                    case MATH_SHIFT:
                    case TAB_MARK:
                    case SUP_MARK:
                    case SUB_MARK:
                    case SPACER:
                    case LETTER:
                    case OTHER_CHAR: print(c); break;

                    case MAC_PARAM:
                        print(c);
                        print(c);
                        break;

                    case outparam:
                        print(matchchr);
                        if (c > 9) {
                            print_char('!');
                            return;
                        }
                        print_char(c + '0');
                        break;

                    case match:
                        matchchr = c;
                        print(c);
                        n++;
                        print_char(n);
                        if (n > '9') return;
                        break;

                    case endmatch:
                        print(S(310)); // "−>"
                        break;

                    default:
                        print_esc(S(309)); // "BAD."
                        break;
                } // switch (m)
            } // if (info(p) <> 0)
        } // if (info(p) <> CS_TOKEN_FLAG)

        p = link(p);
    } // while (p != 0 && tally < l)

    if (p != 0) print_esc(S(311)); // "ETC."
} // #292: showtokenlist

// #295
void tokenshow(HalfWord p) {
    if (p != 0) showtokenlist(link(p), 0, 10000000L);
} // #295 : tokenshow


