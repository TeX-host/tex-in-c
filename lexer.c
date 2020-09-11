#include <stdlib.h>   // labs
#include <math.h>     // abs
#include "print.h"      // [var] tally, term_input
#include "global.h"     // [var] mem, lo_mem_max, hi_mem_min, mem_end,
// pagetail
#include "texfunc.h"    // [func] begindiagnostic, print_mode, enddiagnostic
// delete_token_ref, set_help, openlogfile
#include "printout.h"   // [func] printcmdchr,
#include "pure_func.h"  // [func] aclose
#include "macros.h"     // [macro] help4, help3, help2
#include "io.h"         // [func] inputln, initterminal
#include "expand.h"     // [var] longstate
#include "error.h"
    // [func] error, fatalerror, overflow, print_err,
    // [macro] checkinterrupt,
#include "hash.h" // [func] idlookup_p, sprint_cs
#include "mem.h"  // [var] lo_mem_max, hi_mem_min, mem_end
    // [func] flush_list, get_avail
#include "box.h" // NORMAL, listoffset, listoffset
#include "texmath.h" // print_scaled, x_over_n
#include "lexer.h"


/** @addtogroup S211x219_P77x80
 * @{
 */
/// [ #211~219: PART 16: THE SEMANTIC NEST ]
ListStateRecord nest[NEST_SIZE + 1]; // [0, NEST_SIZE=40]
UChar nest_ptr;             // first unused location of nest
UChar max_nest_stack;       // maximum of nest_ptr when pushing
ListStateRecord cur_list;   // the "top" semantic state
UInt16 shown_mode;          // most recent mode shown by \tracingcommands
/** @}*/ // end group S211x219_P77x80

/** @addtogroup S297x299_P119x120
 *  @{
 */

/// [#297]: current command set by `get_next`
///  a *command code* from the long list of codes given above.
EightBits curcmd;
/// [#297]: operand of current command
///  a *character code* or other *modifier* of the command code.
HalfWord curchr;
/// [#297]: control sequence found here, zero if none found
///  the `eqtb` location of the current control sequence.
Pointer curcs;
/// [#297]: packed representative of `curcmd` and `curchr`.
HalfWord curtok;
/** @}*/ // end group S297x299_P119x120


/** @addtogroup S300x320_P121x130
 *  @{
 */

/// [#301]:
///
/// xref[2]: only tex.c
///     #error, #openlogfile
InStateRecord inputstack[STACK_SIZE + 1];
/// [#301]: first unused location of input stack.
///
/// xref[3, only tex.c]:
///     #openlogfile, #final_cleanup,
///     #S1337_Get_the_first_line_of_input_and_prepare_to_start
UChar inputptr;
/// [#301]: largest value of input ptr when pushing.
static UChar maxinstack;
/// [#301]: the "top" input state, according to convention.
/// xref[]: 35, 36, 87, 301, 302, 311, 321, 322, 534, 1131,
///     #error, #openlogfile
InStateRecord cur_input;

/// [#304] the number of lines in the buffer, less one
static UChar inopen;
/// the number of open text files
/// xref[2, only tex.c]: #startinput, #final_cleanup
char openparens;
Integer line; ///< current line number in the current source file.
/// xref[]: #startinput
FILE* inputfile[MAX_IN_OPEN];
static Integer linestack[MAX_IN_OPEN];

/// [#305] can a subfile end now?
char scanner_status;
/// [#305] identifier relevant to non-normal scanner status
Pointer warning_index;
/// [#305] reference count of token list being defined
Pointer defref;

/// [#308] token list pointers for parameters.
//// xref[only tex.c]: #macrocall
Pointer paramstack[PARAM_SIZE + 1];
/// [#308] first unused entry in param stack.
//// xref[only tex.c]: #macrocall
int paramptr;
/// [#308] largest value of param ptr, will be ≤ param size + 9 .
//// xref[only tex.c]: #macrocall
Integer maxparamstack;
/// [#309] group level with respect to current alignment.
Integer align_state;
/// [#310] shallowest level shown by show context.
/// xref[only tex.c]: #error
UChar baseptr;
/** @}*/ // end group S300x320_P121x130


// 辅助函数
UChar get_maxinstack() { return maxinstack; }


/** @addtogroup S211x219_P77x80
 * @{
 */

/// [#215]
void lexer_semantic_init() {
    nest_ptr = 0;
    max_nest_stack = 0;
    mode = V_MODE;
    head = contribhead;
    tail = contribhead;
    prevdepth = ignoredepth;
    modeline = 0;
    prevgraf = 0;
    shown_mode = 0;
}

// #211: prints the mode represented by m
void print_mode(Integer m) {
    if (m > 0) {
        switch (m / (MAX_COMMAND + 1)) {
            case 0: print(S(431)); break; // "vertical"
            case 1: print(S(432)); break; // "horizontal"
            case 2: print(S(433)); break; // "display math"
        }
    } else if (m == 0) {
        print(S(434)); // "no"
    } else { // m < 0
        switch ((-1 * m) / (MAX_COMMAND + 1)) {
            case 0: print(S(435)); break; // "internal vertical"
            case 1: print(S(436)); break; // "restricted horizontal"
            case 2: print(S(394)); break; // "math"
        }
    } // if (m <> 0)
    print(S(437)); // " mode"
} // #211: print_mode


/// [#216] enter a new semantic level, save the old.
void pushnest(void) {
    if (nest_ptr > max_nest_stack) {
        max_nest_stack = nest_ptr;
        // "semantic nest size"
        if (nest_ptr == NEST_SIZE) overflow(S(438), NEST_SIZE);
    }
    nest[nest_ptr] = cur_list;
    nest_ptr++;
    head = get_avail();
    tail = head;
    prevgraf = 0;
    modeline = line;
} /* pushnest */

/// [#217] leave a semantic level, re-enter the old.
void popnest(void) {
    FREE_AVAIL(head);
    nest_ptr--;
    cur_list = nest[nest_ptr];
} /* popnest */

/// [#218] displays what TEX is working on, at all levels.
void showactivities(void) {
    Int16 m;      // mode
    MemoryWord a; // auxiliary
    Pointer q, r; // for showing the current page
    Integer t;

    // put the top level into the array
    nest[nest_ptr] = cur_list;
    printnl(S(385)); // ""
    println();

    for (Pointer p = nest_ptr; p >= 0; p--) {
        m = nest[p].modefield;
        a = nest[p].auxfield;

        printnl(S(439)); // "### "
        print_mode(m);
        print(S(440)); // " entered at line "
        print_int(labs(nest[p].mlfield));

        if (m == H_MODE) {
            if (nest[p].pgfield != 8585216L) {
                print(S(441)); // " (language"
                print_int(nest[p].pgfield % 65536L);
                print(S(442)); // ":hyphenmin"
                print_int(nest[p].pgfield / 4194304L);
                print_char(',');
                print_int((nest[p].pgfield / 65536L) & 63);
                print_char(')');
            }
        } // if (m == H_MODE)

        if (nest[p].mlfield < 0) print(S(443)); // " (\output routine)"
        if (p == 0) {
            /// [#986] Show the status of the current page.
            if (pagehead != pagetail) {
                printnl(S(444)); // "### current page:"
                if (outputactive) {
                    print(S(445)); // " (held over for next output)"
                }
                showbox(link(pagehead));
                if (pagecontents > EMPTY) {
                    printnl(S(446)); // "total height "
                    printtotals();
                    printnl(S(447)); // " goal height "
                    print_scaled(pagegoal);
                    r = link(pageinshead);
                    while (r != pageinshead) {
                        println();
                        print_esc(S(374)); // "insert"
                        t = subtype(r) - MIN_QUARTER_WORD;
                        print_int(t);
                        print(S(448)); // " adds "
                        t = x_over_n(height(r), 1000) * count(t);
                        print_scaled(t);
                        if (type(r) == splitup) {
                            q = pagehead;
                            t = 0;
                            do {
                                q = link(q);
                                if ((type(q) == INS_NODE) &
                                    (subtype(q) == subtype(r)))
                                    t++;
                            } while (q != brokenins(r));
                            print(S(449)); // ", #"
                            print_int(t);
                            print(S(450)); // " might split"
                        } // if (type(r) == splitup)
                        r = link(r);
                    } // while (r != pageinshead)
                } // if (pagecontents > EMPTY)
            } // if (pagehead != pagetail)

            // "### recent contributions:"
            if (link(contribhead) != 0) printnl(S(451));
        } // if (p == 0)

        showbox(link(nest[p].headfield));
        /// [#219]  Show the auxiliary field, a.
        switch (abs(m) / (MAX_COMMAND + 1)) {
            case 0:
                printnl(S(452)); // "prevdepth "
                if (a.sc <= ignoredepth) {
                    print(S(453)); // "ignored"
                } else {
                    print_scaled(a.sc);
                }
                if (nest[p].pgfield != 0) {
                    print(S(454)); // ", prevgraf "
                    print_int(nest[p].pgfield);
                    print(S(455)); // " line"
                    if (nest[p].pgfield != 1) print_char('s');
                }
                break;

            case 1:
                printnl(S(456)); // "spacefactor "
                print_int(a.hh.UU.lh);
                if (m > 0) {
                    if (a.hh.rh > 0) {
                        print(S(457)); // ", current language "
                        print_int(a.hh.rh);
                    }
                }
                break;

            case 2:
                if (a.int_ != 0) {
                    print(S(458)); // "this will be denominator of:"
                    showbox(a.int_);
                }
                break;

            default:
                /* do nothing */
                break;
        } // switch (abs(m) / (MAX_COMMAND + 1))
    } // for (Pointer p = nest_ptr; p >= 0; p--)
} /* [#218] showactivities */
/** @}*/ // end group S211x219_P77x80


/** @addtogroup S289x296_P115x118
 *
 * + #showtokenlist
 * + #tokenshow
 * + #printmeaning
 *
 *  @{
 */

/// [p117#292]
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
            set_trick_count();
        }

        // [#293] Display token p, and return if there are problems
        if (p < hi_mem_min || p > mem_end) {
        #ifndef USE_REAL_STR
            print_esc(S(308)); // "CLOBBERED."
        #else
            print_esc_str("CLOBBERED.");
        #endif // USE_REAL_STR
            return;
        }
        if (info(p) >= CS_TOKEN_FLAG) {
            print_cs(info(p) - CS_TOKEN_FLAG);
        } else { // info(p) < CS_TOKEN_FLA
            m = info(p) / dwa_do_8;
            c = info(p) % dwa_do_8;
            if (info(p) < 0) {
            #ifndef USE_REAL_STR
                print_esc(S(309)); // "BAD."
            #else
                print_esc_str("BAD.");
            #endif // USE_REAL_STR
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

                    case OUT_PARAM:
                        print(matchchr);
                        if (c > 9) {
                            print_char('!');
                            return;
                        }
                        print_char(c + '0');
                        break;

                    case MATCH:
                        matchchr = c;
                        print(c);
                        n++;
                        print_char(n);
                        if (n > '9') return;
                        break;

                    case END_MATCH:
                        print(S(310)); // "−>"
                        break;

                    default:
                    #ifndef USE_REAL_STR
                        print_esc(S(309)); // "BAD."
                    #else
                        print_esc_str("BAD.");
                    #endif // USE_REAL_STR
                        break;
                } // switch (m)
            } // if (info(p) <> 0)
        } // if (info(p) <> CS_TOKEN_FLAG)

        p = link(p);
    } // while (p != 0 && tally < l)

    if (p != 0) {
    #ifndef USE_REAL_STR
        print_esc(S(311)); // "ETC."
    #else
        print_esc_str("ETC.");
    #endif // USE_REAL_STR
    }
} // #292: showtokenlist

/// [#295] display a token list,  given a pointer to its reference count.
void tokenshow(HalfWord p) {
    if (p != 0) showtokenlist(link(p), 0, 10000000L);
} // #295 : tokenshow

/// [p118#296] displays cur cmd and cur chr in symbolic form, 
///     including the expansion of a macro or mark.
void printmeaning(int cur_chr, int cur_cmd) {
    printcmdchr(cur_cmd, cur_chr);
    if (cur_cmd >= CALL) {
        print_char(':');
        println();
        tokenshow(cur_chr);
        return;
    }
    if (cur_cmd != TOP_BOT_MARK) return;
    print_char(':');
    println();
    tokenshow(curmark[cur_chr - topmarkcode]);
} // [#296] printmeaning
/** @}*/ // end group S289x296_P115x118


/** @addtogroup S297x299_P119x120
 * + #printcmdchr 放在 printout.c 中
 * + #showcurcmdchr
 * @{
 */

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
} // [#299] showcurcmdchr
/** @}*/ // end group S297x299_P119x120


/** @addtogroup S300x320_P121x130
 * + #runaway
 * + #showcontext
 *  @{
 */

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
    Selector old_setting;
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
                    begin_pseudo_print(&l);
                    if (buffer[LIMIT] == end_line_char)
                        j = LIMIT;
                    else
                        j = LIMIT + 1;
                    if (j > 0) {
                        for (i = START; i < j; i++) { /*:318*/
                            if (i == LOC) {
                                set_trick_count();
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
                            if (LOC == 0) {
                                printnl(S(495));
                            } else {
                                printnl(S(496));
                            }
                            break;

                        case INSERTED: printnl(S(497)); break;
                        case MACRO: println(); print_cs(NAME); break;
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
                    begin_pseudo_print(&l);
                    if (token_type < MACRO)
                        showtokenlist(START, LOC, 100000L);
                    else /*:319*/
                        showtokenlist(link(START), LOC, 100000L);
                }
                selector = old_setting; /*317:*/
                if (trick_count == 1000000L) {
                    set_trick_count();
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
/** @}*/ // end group S300x320_P121x130


/** @addtogroup S321x331_P131x133
 *
 * + #begintokenlist
 * + #endtokenlist
 * + #backinput
 * + #backerror
 * + #inserror
 * + #beginfilereading
 * + #endfilereading
 * + #clearforerrorprompt
 *
 * @{
 */

// #323:  starts a new level of token-list input,
// given a token list p and its type t.
void begintokenlist(HalfWord p, QuarterWord t) {
    // [#321] push_input: enter a new input level, save the old
    if (inputptr > maxinstack) {
        maxinstack = inputptr;
        // "input stack size"
        if (inputptr == STACK_SIZE) overflow(S(508), STACK_SIZE);
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
            printcmdchr(ASSIGN_TOKS, t - OUTPUT_TEXT + OUTPUT_ROUTINE_LOC);
            break;
    }
    print(S(310)); // "−>"
    tokenshow(p);
    enddiagnostic(false);
} // #323: begintokenlist

/// [#324] leave a token-list input level
void endtokenlist(void) {
    if (token_type >= BACKED_UP) {
        if (token_type <= INSERTED) {
            flush_list(START);
        } else {
            delete_token_ref(START);
            if (token_type == MACRO) {
                while (paramptr > param_start) {
                    paramptr--;
                    flush_list(paramstack[paramptr]);
                }
            }
        }
    } else if (token_type == U_TEMPLATE) {
        if (align_state > 500000L) {
            align_state = 0;
        } else {
            // "(interwoven alignment preambles are not allowed)"
            fatalerror(S(509));
        }
    }
    popinput();
    checkinterrupt();
} // [#324] endtokenlist

/// [#325] undoes one token of input
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
        // "input stack size"
        if (inputptr == STACK_SIZE) overflow(S(508), STACK_SIZE);
    }
    inputstack[inputptr] = cur_input;
    inputptr++;
    STATE = TOKEN_LIST;
    START = p;
    token_type = BACKED_UP;
    LOC = p;
} // [#325] backinput

/// [#327] back up one token and call #error
void backerror(void) {
    OK_to_interrupt = false;
    backinput();
    OK_to_interrupt = true;
    error();
} // [#327] backerror

/// [#327] back up one inserted token and call #error
void inserror(void) {
    OK_to_interrupt = false;
    backinput();
    token_type = INSERTED;
    OK_to_interrupt = true;
    error();
} // [#327] inserror

/// [#328] starts a new level of input for lines of characters to be read from
/// a file, or as an insertion from the terminal.
void beginfilereading(void) {
    // "text input levels"
    if (inopen == MAX_IN_OPEN) overflow(S(510), MAX_IN_OPEN);
    // "buffer size"
    if (first == BUF_SIZE) overflow(S(511), BUF_SIZE);
    inopen++;
    if (inputptr > maxinstack) {
        maxinstack = inputptr;
        // "input stack size"
        if (inputptr == STACK_SIZE) overflow(S(508), STACK_SIZE);
    }
    inputstack[inputptr] = cur_input;
    inputptr++;
    IINDEX = inopen;
    linestack[IINDEX - 1] = line;
    START = first;
    STATE = MID_LINE;
    NAME = 0;
} // [#328] beginfilereading

/// [#329] variables must be downdated when such a level of input is finished
void endfilereading(void) {
    first = START;
    line = linestack[IINDEX - 1];
    if (NAME > 17) aclose(&curfile);
    popinput();
    inopen--;
} // [#329] endfilereading

/// [#330] removes completed error-inserted lines from memory
void clearforerrorprompt(void) {
    while (STATE != TOKEN_LIST 
            && terminal_input 
            && inputptr > 0 
            && LOC > LIMIT) {
        endfilereading();
    } // while
    println();
    clear_terminal();
} // [#330] clearforerrorprompt

/// [#331] 初始化变量
Boolean init_lexer(void) {
    const Boolean HAS_ERROR = true, NO_ERROR = false;

    /// [#331]: Initialize the input routines
    inputptr = 0;
    maxinstack = 0;
    inopen = 0;
    openparens = 0;
    max_buf_stack = 0;
    paramptr = 0;
    maxparamstack = 0;
    first = BUF_SIZE;

    do {
        buffer[first] = 0;
        first--;
    } while (first != 0);

    scanner_status = NORMAL;
    warning_index = 0;
    first = 1;
    STATE = NEW_LINE;
    START = 1;
    IINDEX = 0;
    line = 0;
    NAME = 0;
    force_eof = false;
    align_state = 1000000L;
    if (!initterminal()) return HAS_ERROR;
    LIMIT = last;
    first = last + 1; // `init_terminal` has set `LOC` and `last`

    return NO_ERROR;
} // [#331, ] init_lexer
/** @}*/ // end group S321x331_P131x133


/** @addtogroup S332x365_P134x143
 *
 * + #check_outer_validity
 * + #getnext_worker
 * + #getnext
 * + #firm_up_the_line
 * + #gettoken
 *
 * @{
 */

/// [P134#336]
static int check_outer_validity(int local_curcs) {
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
            print_err(S(512)); // "File ended"
        } else {
            curcs = 0;
            print_err(S(513)); // "Forbidden control sequence found"
        }
        print(S(514)); // " while scanning "
        /*339:*/
        p = get_avail();
        switch (scanner_status) {

            case DEFINING:
                print(S(313)); // "definition"
                info(p) = rightbracetoken + '}';
                break;

            case MATCHING:
                print(S(515)); // "use"
                info(p) = partoken;
                longstate = OUTER_CALL;
                break;

            case ALIGNING:
                print(S(315)); // "preamble"
                info(p) = rightbracetoken + '}';
                q = p;
                p = get_avail();
                link(p) = q;
                info(p) = CS_TOKEN_FLAG + FROZEN_CR;
                align_state = -1000000L;
                break;

            case ABSORBING:
                print(S(316)); // "text"
                info(p) = rightbracetoken + '}';
                break;
        }
        inslist(p); /*:339*/
        print(S(516)); // " of "
        sprint_cs(warning_index);
        /*
         * (517) "I suspect you have forgotten a `}' causing me"
         * (518) "to read past where you wanted me to stop."
         * (519) "I'll try to recover; but if the error is serious"
         * (520)s "you'd better type `E' or `X' now and fix your file."
         */
        help4(S(517), S(518), S(519), S(520));
        error();
    } else {
        print_err(S(521)); // "Incomplete "
        printcmdchr(IF_TEST, curif);
        print(S(522)); // "; all text was ignored after line "
        print_int(skipline);
        /*
         * (523) "A forbidden control sequence occurred in skipped text."
         * (524) "This kind of error happens when you say `\\if...' and forget"
         * (525) "the matching `\\fi'. I've inserted a `\\fi'; this might work."
         */
        help3(S(523), S(524), S(525));
        if (curcs != 0) {
            curcs = 0;
        } else {
            // "The file ended while I was skipping conditional text."
            help_line[2] = S(526);
        }
        curtok = CS_TOKEN_FLAG + FROZEN_FI;
        inserror();
    }
    /*:338*/
    deletions_allowed = true;
    return curcs;
} // [P134#336]: check_outer_validity


#define CHECK_OUTER                            \
    do {                                       \
        curchr = cur_chr;                      \
        curcmd = cur_cmd;                      \
        cur_cs = check_outer_validity(cur_cs); \
        cur_cmd = curcmd;                      \
        cur_chr = curchr;                      \
    } while (0)

#define process_cmd \
    if (cur_cmd >= OUTER_CALL) CHECK_OUTER;

#define Process_cs                \
    {                             \
        cur_cmd = eq_type(cur_cs); \
        cur_chr = equiv(cur_cs);  \
        process_cmd               \
    }

/// [#341]: getnext_worker
static void getnext_worker(Boolean no_new_control_sequence) {
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
                    update_terminal(); // show user that file has been read
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

                // selector in [NO_PRINT, TERM_ONLY]
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
        cur_cmd = cat_code(cur_chr);

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
                    cur_cs = NULL_CS; // state is irrelevant in this case
                } else {             // LOC <= LIMIT
                // go here to: start looking for a control sequence
                _getnext_worker__startcs_:
                    k = LOC;
                    cur_chr = buffer[k];
                    cat = cat_code(cur_chr);
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
                            cat = cat_code(cur_chr);
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

                    cur_cs = ACTIVE_BASE + buffer[LOC];
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
                cur_cs = cur_chr + ACTIVE_BASE;
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
                print_err(S(529)); // "Text line contains an invalid character"
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
            cur_cmd = eq_type(cur_cs);
            cur_chr = equiv(cur_cs);
            if (cur_cmd >= OUTER_CALL) {
                if (cur_cmd == DONT_EXPAND) {
                    // [#358] Get the next token, suppressing expansion
                    cur_cs = info(LOC) - CS_TOKEN_FLAG;
                    LOC = 0;
                    cur_cmd = eq_type(cur_cs);
                    cur_chr = equiv(cur_cs);
                    if (cur_cmd > MAX_COMMAND) {
                        cur_cmd = RELAX;
                        cur_chr = noexpandflag;
                    }
                } else {
                    CHECK_OUTER;
                } // if (cur_cmd <> DONT_EXPAND)
            }     // if (cur_cmd >= outercall)
        } else {  // t < CS_TOKEN_FLAG
            cur_cmd = t / dwa_do_8;
            cur_chr = t % dwa_do_8;
            switch (cur_cmd) {
                case LEFT_BRACE: align_state++; break;
                case RIGHT_BRACE: align_state--; break;
                case OUT_PARAM:
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
        if (cur_cmd == OMIT) {
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

// [#341] : sets #curcmd, #curchr, #curcs to next token.
//
// xref[22]
//   76, 297, 332, 336, 340,
//  [341], 357, 360, 364, 365,
//  366, 369, 380, 381, 387,
//  389, 478, 494, 507, 644,
//  1038, 1126
void getnext(void) { getnext_worker(true); }

/// [#363] If the user has set the pausing parameter to some positive value, and
/// if nonstop mode has not beenselected.
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

/// [#365] 
void gettoken(void) {
    getnext_worker(false);
    curtok = pack_tok(curcs, curcmd, curchr);
} // [#365] gettoken
/** @}*/ // end group S332x365_P134x143

/// [#365] 辅助函数
int pack_tok(int cs, int cmd, int chr) {
    if (cs == 0) {
        return (cmd * dwa_do_8) + chr;
    } else {
        return CS_TOKEN_FLAG + cs;
    }
}
