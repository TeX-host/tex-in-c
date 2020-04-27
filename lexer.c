#include "print.h" // [var] tally,
#include "global.h" // [var] mem, lo_mem_max, hi_mem_min, mem_end,
#include "texfunc.h" // [func] print_cs
// #include "texmac.h"  // [macro] temphead, holdhead,
#include "printout.h" // [func] printcmdchr,
#include "pure_func.h" // [func] aclose
#include "macros.h"    // [macro] help4, help3, help2
#include "inputln.h"   // [func] inputln
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


/** [ #332~365: PART 24: GETTING THE NEXT TOKEN ]
 *
 * + check_outer_validity
 * + getnext_worker
 * + getnext
 * + firm_up_the_line
 * + gettoken
 */

// P134#336
int check_outer_validity(int local_curcs) {
    Pointer p, q;

    if (scanner_status == NORMAL) return local_curcs;

    deletions_allowed = false; /*337:*/
    if (local_curcs != 0) {    /*:337*/
        if (STATE == TOKEN_LIST || NAME < 1 || NAME > 17) {
            p = get_avail();
            info(p) = CS_TOKEN_FLAG + local_curcs;
            backlist(p);
        }
        curcmd = SPACER;
        curchr = ' ';
    }
    curcs = local_curcs;
    if (scanner_status > SKIPPING) { /*338:*/
        runaway();
        if (curcs == 0) {
            printnl(S(292));
            print(S(512));
        } else {
            curcs = 0;
            printnl(S(292));
            print(S(513));
        }
        print(S(514)); /*339:*/
        p = get_avail();
        switch (scanner_status) {

            case DEFINING:
                print(S(313));
                info(p) = rightbracetoken + '}';
                break;

            case MATCHING:
                print(S(515));
                info(p) = partoken;
                longstate = outercall;
                break;

            case ALIGNING:
                print(S(315));
                info(p) = rightbracetoken + '}';
                q = p;
                p = get_avail();
                link(p) = q;
                info(p) = CS_TOKEN_FLAG + frozencr;
                align_state = -1000000L;
                break;

            case ABSORBING:
                print(S(316));
                info(p) = rightbracetoken + '}';
                break;
        }
        inslist(p); /*:339*/
        print(S(516));
        sprint_cs(warning_index);
        help4(S(517), S(518), S(519), S(520));
        error();
    } else {
        printnl(S(292));
        print(S(521));
        printcmdchr(iftest, curif);
        print(S(522));
        print_int(skipline);
        help3(S(523), S(524), S(525));
        if (curcs != 0)
            curcs = 0;
        else
            help_line[2] = S(526);
        curtok = CS_TOKEN_FLAG + frozenfi;
        inserror();
    }
    /*:338*/
    deletions_allowed = true;
    return curcs;
} // P134#336: check_outer_validity


#define CHECK_OUTER                            \
    do {                                       \
        curchr = cur_chr;                      \
        curcmd = cur_cmd;                      \
        cur_cs = check_outer_validity(cur_cs); \
        cur_cmd = curcmd;                      \
        cur_chr = curchr;                      \
    } while (0)

#define process_cmd \
    if (cur_cmd >= outercall) CHECK_OUTER;

#define Process_cs                \
    {                             \
        cur_cmd = eqtype(cur_cs); \
        cur_chr = equiv(cur_cs);  \
        process_cmd               \
    }

// #341: getnext_worker
void getnext_worker(Boolean no_new_control_sequence) {
    UInt16 k;            // an index into buffer; [0, BUF_SIZE=5000]
    UChar cat;           // cat_code(cur chr), usually
    ASCIICode c, cc = 0; // constituents of a possible expanded code
    UChar d;             // number of excess characters in an expanded code

    // getnext_worker 内部变量
    int cur_cs, cur_chr, cur_cmd;

// go here to: get the next input token
_getnext_worker__restart:
    cur_cs = 0;
    if (STATE != TOKEN_LIST) {
        // [#343] Input from external file,
        // goto _restart if no input found

    // go here to: eat the next character from a file
    _getnext_worker__switch:
        if (LOC > LIMIT) {
            STATE = NEW_LINE;
            // [#360] Move to next line of file,
            // or goto restart if there is no next line
            //  or return if a \read line has finished
            if (NAME > 17) {
                // #362: Read next line of file into buffer,
                //   or goto restart if the file has ended
                line++;
                first = START;
                if (!force_eof) {
                    if (inputln(curfile, true)) { // 这里读取了下一个字符
                        firm_up_the_line();
                    } else {
                        force_eof = true;
                    }
                }
                if (force_eof) {
                    print_char(')');
                    openparens--;
                    fflush(stdout);
                    force_eof = false;
                    endfilereading();
                    cur_cs = check_outer_validity(cur_cs);
                    goto _getnext_worker__restart;
                }
                if (end_line_char_inactive) {
                    LIMIT--;
                } else {
                    buffer[LIMIT] = end_line_char;
                }
                first = LIMIT + 1;
                LOC = START;
            } else { // NAME <= 17
                /*:360*/
                // \read line has ended
                if (!terminal_input) {
                    cur_cmd = 0;
                    cur_chr = 0;
                    goto _getnext_worker__exit;
                }
                // text was inserted during error recovery
                if (inputptr > 0) {
                    endfilereading();
                    goto _getnext_worker__restart; // resume previous level
                }
                if (selector < LOG_ONLY) openlogfile();
                if (interaction > NON_STOP_MODE) {
                    if (end_line_char_inactive) {
                        LIMIT++;
                    }
                    // previous line was empty
                    if (LIMIT == START) {
                        // "(Please type a command or say `\end´)"
                        printnl(S(527));
                    }
                    // input on-line into `buffer`
                    println();
                    first = START;
                    print('*');
                    term_input();

                    LIMIT = last;
                    if (end_line_char_inactive) {
                        LIMIT--;
                    } else {
                        buffer[LIMIT] = end_line_char;
                    }
                    first = LIMIT + 1;
                    LOC = START;
                } else { // nonstop mode
                         // which is intended for overnight batch processing
                         // never waits for on-line input
                    // "*** (job aborted, no legal \end found)"
                    fatalerror(S(528));
                } // if (interaction <> NON_STOP_MODE)
            }     // if (NAME <> 17)
            checkinterrupt();
            goto _getnext_worker__switch;
        } // if (LOC > LIMIT)
        // else: if (LOC <= LIMIT)
        cur_chr = buffer[LOC];
        LOC++;

    // go here to: digest it again
    _getnext_worker__reswitch:
        cur_cmd = catcode(cur_chr);

        // #344 Change state if necessary,
        // and goto switch if the current character should be ignored,
        // or goto reswitch if the current character changes to another
        switch (STATE + cur_cmd) {
            // any state plus(IGNORE)
            case MID_LINE + IGNORE:
            case SKIP_BLANKS + IGNORE:
            case NEW_LINE + IGNORE:
            case SKIP_BLANKS + SPACER:
            case NEW_LINE + SPACER:
                // [#345]: Cases where character is ignored
                goto _getnext_worker__switch;
                break;

            // any state plus(escape)
            case MID_LINE + ESCAPE:
            case SKIP_BLANKS + ESCAPE:
            case NEW_LINE + ESCAPE:
                // [#354] Scan a control sequence
                //  and set state ← `SKIP_BLANKS` or `MID_LINE`
                if (LOC > LIMIT) {
                    cur_cs = nullcs; // state is irrelevant in this case
                } else {             // LOC <= LIMIT
                // go here to: start looking for a control sequence
                _getnext_worker__startcs_:
                    k = LOC;
                    cur_chr = buffer[k];
                    cat = catcode(cur_chr);
                    k++;

                    if (cat == LETTER) {
                        STATE = SKIP_BLANKS;
                    } else if (cat == SPACER) {
                        STATE = SKIP_BLANKS;
                    } else {
                        STATE = MID_LINE;
                    } // if (cat == ...)

                    if (cat == LETTER && k <= LIMIT) {
                        // [#356]
                        do {
                            cur_chr = buffer[k];
                            cat = catcode(cur_chr);
                            k++;
                        } while (cat == LETTER && k <= LIMIT);

                        // [#355] If an expanded code is present,
                        // reduce it and goto start cs

                        if (buffer[k] == cur_chr && cat == SUP_MARK &&
                            k < LIMIT
                            // yes, one is indeed present
                            && (c = buffer[k + 1]) < 128) {
                            d = 2;

                            if (ishex(c) && (k + 2) <= LIMIT) {
                                cc = buffer[k + 2];
                                if (ishex(cc)) d++;
                            }
                            if (d > 2) {
                                buffer[k - 1] = cur_chr = hex_to_i(c, cc);
                            } else if (c < 64) {
                                buffer[k - 1] = c + 64;
                            } else {
                                buffer[k - 1] = c - 64;
                            }
                            LIMIT -= d;
                            first -= d;
                            while (k <= LIMIT) {
                                buffer[k] = buffer[k + d];
                                k++;
                            }
                            goto _getnext_worker__startcs_;
                        } // #355: if-set

                        if (cat != LETTER) {
                            k--; // now k points to first nonletter
                        }
                        if (k > LOC + 1) {
                            // multiletter control sequence has been scanned
                            cur_cs = idlookup_p(buffer + LOC, k - LOC,
                                                no_new_control_sequence);
                            LOC = k;
                            goto _getnext_worker__found;
                        }
                    } else {
                        // [#355] If an expanded code is present,
                        // reduce it and goto start cs
                        c = buffer[k + 1];
                        if (buffer[k] == cur_chr && cat == SUP_MARK &&
                            k < LIMIT &&
                            c < 128) { // yes, one is indeed present
                            d = 2;

                            cc = buffer[k + 2];
                            if (ishex(c) && (k + 2) <= LIMIT && ishex(cc)) {
                                d++;
                            }
                            if (d > 2) {
                                buffer[k - 1] = cur_chr = hex_to_i(c, cc);
                            } else if (c < 64) {
                                buffer[k - 1] = c + 64;
                            } else {
                                buffer[k - 1] = c - 64;
                            }
                            LIMIT -= d;
                            first -= d;
                            while (k <= LIMIT) {
                                buffer[k] = buffer[k + d];
                                k++;
                            }
                            goto _getnext_worker__startcs_;
                        } // #355: if-set
                    }     // if (cat == LETTER && k <= LIMIT)

                    cur_cs = singlebase + buffer[LOC];
                    LOC++;
                } // if (LOC <> LIMIT)

            // go here: when a control sequence has been found
            _getnext_worker__found:
                Process_cs;
                break; // [#354]

            // any state plus(active_char)
            case MID_LINE + ACTIVE_CHAR:
            case SKIP_BLANKS + ACTIVE_CHAR:
            case NEW_LINE + ACTIVE_CHAR:
                // [#353]: Process an active-character control sequence
                // and set state ← mid line
                STATE = MID_LINE;
                cur_cs = cur_chr + activebase;
                Process_cs;
                break; // [#353]

            // any state plus(SUP_MARK)
            case MID_LINE + SUP_MARK:
            case SKIP_BLANKS + SUP_MARK:
            case NEW_LINE + SUP_MARK:
                // [#352]
                c = buffer[LOC + 1];
                if (cur_chr == buffer[LOC] && LOC < LIMIT && c < 128) {
                    LOC += 2;
                    cc = buffer[LOC];
                    if (ishex(c) && LOC <= LIMIT && ishex(cc)) {
                        LOC++;
                        cur_chr = hex_to_i(c, cc);
                        goto _getnext_worker__reswitch;
                    } // if - set
                    if (c < 64) {
                        cur_chr = c + 64;
                    } else {
                        cur_chr = c - 64;
                    } // if (c <> 64)
                    goto _getnext_worker__reswitch;
                } // if - set
                STATE = MID_LINE;
                break; // [#352]

            // any state plus(INVALID_CHAR)
            case MID_LINE + INVALID_CHAR:
            case SKIP_BLANKS + INVALID_CHAR:
            case NEW_LINE + INVALID_CHAR:
                // [#346] Decry the invalid character and goto restart
                printnl(S(292));
                print(S(529)); // "Text line contains an invalid character"
                // "A funny symbol that I can´t read has just been input."
                // "Continue, and I´ll forget that it ever happened."
                help2(S(530), S(531));
                deletions_allowed = false;
                error();
                deletions_allowed = true;
                goto _getnext_worker__restart;
                break; // [#346]

            // [#347] Handle situations involving spaces, braces, changes of
            // state
            case MID_LINE + SPACER:
                // [#349] Enter skip blanks state, emit a space
                STATE = SKIP_BLANKS;
                cur_chr = ' ';
                break;

            case MID_LINE + CAR_RET:
                // [#348] Finish line, emit a space
                LOC = LIMIT + 1;
                cur_cmd = SPACER;
                cur_chr = ' ';
                break;

            // any state plus(COMMENT)
            case SKIP_BLANKS + CAR_RET:
            case MID_LINE + COMMENT:
            case SKIP_BLANKS + COMMENT:
            case NEW_LINE + COMMENT:
                // [#350] Finish line, goto switch
                LOC = LIMIT + 1;
                goto _getnext_worker__switch;
                break;

            case NEW_LINE + CAR_RET:
                // [#351] Finish line, emit a \par
                LOC = LIMIT + 1;
                cur_cs = parloc;
                Process_cs;
                break;

            case MID_LINE + LEFT_BRACE: align_state++; break;

            case SKIP_BLANKS + LEFT_BRACE:
            case NEW_LINE + LEFT_BRACE:
                STATE = MID_LINE;
                align_state++;
                break;

            case MID_LINE + RIGHT_BRACE: align_state--; break;

            case SKIP_BLANKS + RIGHT_BRACE:
            case NEW_LINE + RIGHT_BRACE:
                STATE = MID_LINE;
                align_state--;
                break;

            // add_delims_to(SKIP_BLANKS)
            // add_delims_to(NEW_LINE)
            case SKIP_BLANKS + MATH_SHIFT:
            case SKIP_BLANKS + TAB_MARK:
            case SKIP_BLANKS + MAC_PARAM:
            case SKIP_BLANKS + SUB_MARK:
            case SKIP_BLANKS + LETTER:
            case SKIP_BLANKS + OTHER_CHAR:
            case NEW_LINE + MATH_SHIFT:
            case NEW_LINE + TAB_MARK:
            case NEW_LINE + MAC_PARAM:
            case NEW_LINE + SUB_MARK:
            case NEW_LINE + LETTER:
            case NEW_LINE + OTHER_CHAR: STATE = MID_LINE; break;

            default: break;
        }    // #344: switch (STATE + cur_cmd)
    } else { // STATE == TOKEN_LIST
        // [#357] Input from token list,
        // goto _restart
        //  if end of list
        //  or if a parameter needs to be expanded
        HalfWord t;
        if (LOC == 0) { // we are done with this token list
            endtokenlist();
            goto _getnext_worker__restart; // resume previous level
        }
        // assert(LOC != 0)
        t = info(LOC);
        LOC = link(LOC);          // move to next
        if (t >= CS_TOKEN_FLAG) { // a control sequence token
            cur_cs = t - CS_TOKEN_FLAG;
            cur_cmd = eqtype(cur_cs);
            cur_chr = equiv(cur_cs);
            if (cur_cmd >= outercall) {
                if (cur_cmd == dontexpand) {
                    // [#358] Get the next token, suppressing expansion
                    cur_cs = info(LOC) - CS_TOKEN_FLAG;
                    LOC = 0;
                    cur_cmd = eqtype(cur_cs);
                    cur_chr = equiv(cur_cs);
                    if (cur_cmd > maxcommand) {
                        cur_cmd = relax;
                        cur_chr = noexpandflag;
                    }
                } else {
                    CHECK_OUTER;
                } // if (cur_cmd <> dontexpand)
            }     // if (cur_cmd >= outercall)
        } else {  // t < CS_TOKEN_FLAG
            cur_cmd = t / dwa_do_8;
            cur_chr = t % dwa_do_8;
            switch (cur_cmd) {
                case LEFT_BRACE: align_state++; break;
                case RIGHT_BRACE: align_state--; break;
                case outparam:
                    begintokenlist(paramstack[param_start + cur_chr - 1],
                                   PARAMETER);
                    goto _getnext_worker__restart;
                    break;
            } // switch (cur_cmd)
        }     // if (t <> CS_TOKEN_FLAG)
    }         // #343: if (STATE <> TOKEN_LIST)

    // [#342] If an alignment entry has just ended, take appropriate action
    if (cur_cmd <= CAR_RET && cur_cmd >= TAB_MARK && align_state == 0) {
        // [#789] Insert the <v_j> template
        //  and goto _restart
        if (scanner_status == ALIGNING || curalign == null) {
            // "(interwoven alignment preambles are not allowed)"
            fatalerror(S(509));
        }

        cur_cmd = extrainfo(curalign);
        extrainfo(curalign) = cur_chr;
        if (cur_cmd == omit) {
            begintokenlist(omittemplate, V_TEMPLATE);
        } else {
            begintokenlist(vpart(curalign), V_TEMPLATE);
        }
        align_state = 1000000L;
        goto _getnext_worker__restart;
    } // #342: if (cur_cmd <= CAR_RET)

// [#341]: go here: when the next input token has been got
// xref[1]: #360
_getnext_worker__exit:;
    curcmd = cur_cmd;
    curchr = cur_chr;
    curcs = cur_cs;
} // #341: getnext_worker

// [#341] : sets cur cmd, cur chr, cur cs to next token
//
// xref[22]
//   76, 297, 332, 336, 340,
//  [341], 357, 360, 364, 365,
//  366, 369, 380, 381, 387,
//  389, 478, 494, 507, 644,
//  1038, 1126
void getnext(void) { getnext_worker(true); }

void firm_up_the_line(void) {
    short k;
    // #363:
    // 340, 362, [363], 538

    LIMIT = last;
    if (pausing <= 0) return;
    if (interaction <= NON_STOP_MODE) return;

    println();
    if (START < LIMIT) {
        for (k = START; k < LIMIT; k++) {
            print(buffer[k]);
        }
    }

    first = LIMIT;
    print(S(532)); // "=>"
    term_input();  // wait for user response

    if (last <= first) return;
    for (k = first; k < last; k++) {
        buffer[k + START - first] = buffer[k];
    }
    LIMIT = START + last - first;
} // #363: firm_up_the_line

int pack_tok(int cs, int cmd, int chr) {
    if (cs == 0) {
        return dwa_do_8 * cmd + chr;
    } else {
        return CS_TOKEN_FLAG + cs;
    }
}

/*365:*/
void gettoken(void) {
    getnext_worker(false);
    curtok = pack_tok(curcs, curcmd, curchr);
}
/*:365*/
