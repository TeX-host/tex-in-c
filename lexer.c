#include "print.h" // [var] tally,
#include "global.h" // [var] mem, lo_mem_max, hi_mem_min, mem_end,
#include "texfunc.h" // [func] print_cs
// #include "texmac.h"  // [macro] temphead, holdhead,
#include "printout.h" // [func] printcmdchr,
#include "pure_func.h" // [func] aclose
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

/// [#292]
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

/// [#295] display a token list,  given a pointer to its reference count.
void tokenshow(HalfWord p) {
    if (p != 0) showtokenlist(link(p), 0, 10000000L);
} // #295 : tokenshow

/// [p118#296] displays cur cmd and cur chr in symbolic form, 
///     including the expansion of a macro or mark.
void printmeaning(int cur_chr, int cur_cmd) {
    printcmdchr(cur_cmd, cur_chr);
    if (cur_cmd >= call) {
        print_char(':');
        println();
        tokenshow(cur_chr);
        return;
    }
    if (cur_cmd != topbotmark) return;
    print_char(':');
    println();
    tokenshow(curmark[cur_chr - topmarkcode]);
} // [#296] printmeaning

/// [p120#299] displays the current command.
void showcurcmdchr(void) {
    begindiagnostic();
    printnl('{');
    if (mode != shown_mode) {
        print_mode(mode);
        print(S(488));
        shown_mode = mode;
    }
    printcmdchr(curcmd, curchr);
    print_char('}');
    enddiagnostic(false);
} /// [#299] showcurcmdchr

/// [#306] uses scanner status to print a warning message
///  when a subfile has ended, and at certain other crucial times.
void runaway(void) {
    Pointer p = 0 /* XXXX */;

    if (scanner_status <= SKIPPING) return;
    printnl(S(312));
    switch (scanner_status) {

        case DEFINING:
            print(S(313));
            p = defref;
            break;

        case MATCHING:
            print(S(314));
            p = temphead;
            break;

        case ALIGNING:
            print(S(315));
            p = holdhead;
            break;

        case ABSORBING:
            print(S(316));
            p = defref;
            break;
    }
    print_char('?');
    println();
    showtokenlist(link(p), 0, ERROR_LINE - 10);
} // [#306] runaway


/// [#311] prints where the scanner is.
void showcontext(void) { /*:315*/
    enum Selector old_setting;
    long nn;
    Boolean bottomline; /*315:*/
    short i;
    short j;
    char l;
    long m;
    char n;
    long p, q;

    baseptr = inputptr;
    inputstack[baseptr] = cur_input;
    nn = -1;
    bottomline = false;
    while (true) {
        cur_input = inputstack[baseptr];
        if (STATE != TOKEN_LIST) {
            if (NAME > 17 || baseptr == 0) bottomline = true;
        }
        if (baseptr == inputptr || bottomline || nn < errorcontextlines) {
            /*312:*/
            if (baseptr == inputptr || STATE != TOKEN_LIST ||
                token_type != BACKED_UP || LOC != 0) {
                tally = 0;
                old_setting = selector;
                if (STATE != TOKEN_LIST) { /*313:*/
                    if (NAME <= 17) {      /*:313*/
                        if (terminal_input) {
                            if (baseptr == 0)
                                printnl(S(489));
                            else
                                printnl(S(490));
                        } else {
                            printnl(S(491));
                            if (NAME == 17)
                                print_char('*');
                            else
                                print_int(NAME - 1);
                            print_char('>');
                        }
                    } else {
                        printnl(S(492));
                        print_int(line);
                    }
                    print_char(' '); /*318:*/
                    beginpseudoprint();
                    if (buffer[LIMIT] == end_line_char)
                        j = LIMIT;
                    else
                        j = LIMIT + 1;
                    if (j > 0) {
                        for (i = START; i < j; i++) { /*:318*/
                            if (i == LOC) {
                                settrick_count();
                            }
                            print(buffer[i]);
                        }
                    }
                } else {                  /*314:*/
                    switch (token_type) { /*:314*/

                        case PARAMETER: printnl(S(493)); break;

                        case U_TEMPLATE:
                        case V_TEMPLATE: printnl(S(494)); break;

                        case BACKED_UP:
                            if (LOC == 0)
                                printnl(S(495));
                            else
                                printnl(S(496));
                            break;

                        case INSERTED: printnl(S(497)); break;

                        case MACRO:
                            println();
                            print_cs(NAME);
                            break;

                        case OUTPUT_TEXT: printnl(S(498)); break;

                        case EVERY_PAR_TEXT: printnl(S(499)); break;

                        case EVERY_MATH_TEXT: printnl(S(500)); break;

                        case EVERY_DISPLAY_TEXT: printnl(S(501)); break;

                        case EVERY_HBOX_TEXT: printnl(S(502)); break;

                        case EVERY_VBOX_TEXT: printnl(S(503)); break;

                        case EVERY_JOB_TEXT: printnl(S(504)); break;

                        case EVERY_CR_TEXT: printnl(S(505)); break;

                        case MARK_TEXT: printnl(S(506)); break;

                        case WRITE_TEXT: printnl(S(507)); break;

                        default: printnl('?'); break;
                    }
                    /*319:*/
                    beginpseudoprint();
                    if (token_type < MACRO)
                        showtokenlist(START, LOC, 100000L);
                    else /*:319*/
                        showtokenlist(link(START), LOC, 100000L);
                }
                selector = old_setting; /*317:*/
                if (trick_count == 1000000L) {
                    settrick_count();
                }
                if (tally < trick_count)
                    m = tally - first_count;
                else
                    m = trick_count - first_count;
                if (l + first_count <= HALF_ERROR_LINE) {
                    p = 0;
                    n = l + first_count;
                } else {
                    print(S(284));
                    p = l + first_count - HALF_ERROR_LINE + 3;
                    n = HALF_ERROR_LINE;
                }
                for (q = p; q < first_count; q++) {
                    print_char(trick_buf[q % ERROR_LINE]);
                }
                println();
                for (q = 1; q <= n; q++)
                    print_char(' ');
                if (m + n <= ERROR_LINE)
                    p = first_count + m;
                else
                    p = first_count + ERROR_LINE - n - 3;
                for (q = first_count; q < p; q++) {
                    print_char(trick_buf[q % ERROR_LINE]);
                }
                if (m + n > ERROR_LINE) /*:317*/
                    print(S(284));
                nn++;
            }
        } /*:312*/
        else if (nn == errorcontextlines) {
            printnl(S(284));
            nn++;
        }
        if (bottomline) goto _Ldone;
        baseptr--;
    }
_Ldone:
    cur_input = inputstack[inputptr];
} // [#311] showcontext


/** [ #321~331: PART 23: MAINTAINING THE INPUT STACKS ].
 *
 * + begintokenlist
 * + endtokenlist
 * + backinput
 * + backerror
 * + inserror
 * + beginfilereading
 * + endfilereading
 * + clearforerrorprompt
 */

// #323:  starts a new level of token-list input,
// given a token list p and its type t.
void begintokenlist(HalfWord p, QuarterWord t) {
    // [#321] push_input: enter a new input level, save the old
    if (inputptr > maxinstack) {
        maxinstack = inputptr;
        // "input stack size"
        if (inputptr == stacksize) overflow(S(508), stacksize);
        // 此处已经报错跳出函数
    }
    // stack the record
    inputstack[inputptr] = cur_input;
    inputptr++;

    STATE = TOKEN_LIST;
    START = p;
    token_type = t;

    if (t < MACRO) {
        LOC = p;
        return;
    }
    // @assert(t >= MACRO)
    // the token list starts with a reference count
    addtokenref(p);
    if (t == MACRO) {
        param_start = paramptr;
        return;
    }
    // assert(t > MACRO)
    LOC = link(p);
    if (tracingmacros <= 1) return;

    begindiagnostic();
    printnl(S(385)); // ""
    switch (t) {
        case MARK_TEXT: print_esc(S(402)); break;  // "mark"
        case WRITE_TEXT: print_esc(S(379)); break; // "write"
        default:
            printcmdchr(assigntoks, t - OUTPUT_TEXT + outputroutineloc);
            break;
    }
    print(S(310)); // "−>"
    tokenshow(p);
    enddiagnostic(false);
} // #323: begintokenlist

/*324:*/
void endtokenlist(void) {
    if (token_type >= BACKED_UP) {
        if (token_type <= INSERTED) {
            flushlist(START);
        } else {
            delete_token_ref(START);
            if (token_type == MACRO) {
                while (paramptr > param_start) {
                    paramptr--;
                    flushlist(paramstack[paramptr]);
                }
            }
        }
    } else if (token_type == U_TEMPLATE) {
        if (align_state > 500000L)
            align_state = 0;
        else
            fatalerror(S(509));
    }
    popinput();
    checkinterrupt();
}
/*:324*/

/*325:*/
void backinput(void) {
    Pointer p;

    while (STATE == TOKEN_LIST && LOC == 0)
        endtokenlist();
    p = get_avail();
    info(p) = curtok;
    if (curtok < rightbracelimit) {
        if (curtok < leftbracelimit)
            align_state--;
        else
            align_state++;
    }
    if (inputptr > maxinstack) {
        maxinstack = inputptr;
        if (inputptr == stacksize) overflow(S(508), stacksize);
    }
    inputstack[inputptr] = cur_input;
    inputptr++;
    STATE = TOKEN_LIST;
    START = p;
    token_type = BACKED_UP;
    LOC = p;
}
/*:325*/

/*327:*/
void backerror(void) {
    OK_to_interrupt = false;
    backinput();
    OK_to_interrupt = true;
    error();
}


void inserror(void) {
    OK_to_interrupt = false;
    backinput();
    token_type = INSERTED;
    OK_to_interrupt = true;
    error();
}
/*:327*/

/*328:*/
void beginfilereading(void) {
    if (inopen == MAX_IN_OPEN) overflow(S(510), MAX_IN_OPEN);
    if (first == BUF_SIZE) overflow(S(511), BUF_SIZE);
    inopen++;
    if (inputptr > maxinstack) {
        maxinstack = inputptr;
        if (inputptr == stacksize) overflow(S(508), stacksize);
    }
    inputstack[inputptr] = cur_input;
    inputptr++;
    IINDEX = inopen;
    linestack[IINDEX - 1] = line;
    START = first;
    STATE = MID_LINE;
    NAME = 0;
} /*:328*/


/*329:*/
void endfilereading(void) {
    first = START;
    line = linestack[IINDEX - 1];
    if (NAME > 17) aclose(&curfile);
    popinput();
    inopen--;
}
/*:329*/

/*330:*/
void clearforerrorprompt(void) {
    while (STATE != TOKEN_LIST && terminal_input && inputptr > 0 &&
           LOC > LIMIT) {
        endfilereading();
    }
    println();
}
/*:330*/

