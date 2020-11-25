#include <stdlib.h> // [func] labs
#include "charset.h" // [type] ASCIICode
#include "tex_constant.h" // [const] BUF_SIZE, PARAM_SIZE
// [enum] TexCommandCode, CmdCode
#include "lexer.h" // [var] warning_index, partoken, force_eof [macro] inslist, noexpandflag
#include "printout.h" // [func] printcmdchr
#include "print.h"    // [var] selector
#include "fonts.h"    // [func] get_fontname, get_fontsize
#include "texmath.h"  // [func] print_scaled
#include "scan.h"     // [func] scan_eight_bit_int, skip_spaces
#include "error.h"    // [func] error, overflow, confusion, print_err,
#include "hash.h"     // [func] idlookup_p, sprint_cs, print_cs
#include "mem.h" // [macro] FREE_AVAIL, FAST_GET_AVAIL,
// [func] popligstack, get_node, flush_list, get_avail
#include "box.h" // NORMAL, subtype
#include "io.h"  // [func] startinput, openlogfile
#include "expand.h"


/** @addtogroup S464x486_P174x180
 * @{
 */
// [p179#480]
FILE* readfile[16];
char readopen[17];
/** @}*/ // end group S464x486_P174x180

/** @addtogroup S487x510_P181x187
 * @{
 */

/// [#489] top of the condition stack.
Pointer condptr;
/// [#489] upper bound on `fi_or_else` codes
char iflimit;
/// [#489] type of conditional being worked on
SmallNumber curif;
/// [#489] line where that conditional began
Integer ifline;

/// [#493] skipping began here
Integer skipline;
/** @} */ // end group S487x510_P181x187


/// [#387] governs the acceptance of `\par` .
char longstate;

static void conditional(void);
static void passtext(void);
static void convtoks(void);
static void insthetoks(void);


/** @addtogroup S366x401_P144x154
 *
 * + #expand
 * + #insertrelax
 * + #get_x_token
 * + #xtoken
 * + #macrocall
 *
 * + #startinput
 *
 * @{
 */
// [#382] token lists for marks
Pointer curmark[splitbotmarkcode - topmarkcode + 1];

/// [#383]
void expand_init() {
    topmark = null;
    firstmark = null;
    botmark = null;
    splitfirstmark = null;
    splitbotmark = null;
} /* expand_init */

/// [#396] If `longstate == outer_call`, 
/// a runaway argument has already been reported.
void report_argument(HalfWord unbalance, int n, Pointer* pstack) {
    HalfWord m;

    if (longstate == CALL) {
        runaway();
    #ifndef USE_REAL_STR
        print_err(S(533)); // "Paragraph ended before "
    #else
        print_err_str("Paragraph ended before ");
    #endif // USE_REAL_STR
        sprint_cs(warning_index);
        print(S(534)); // " was complete"
        /*
         * (535) "I suspect you've forgotten a `}' causing me to apply this"
         * (536) "control sequence to too much text. How can we recover?"
         * (537) "My plan is to forget the whole thing and hope for the best."
         */
        help3(S(535), S(536), S(537));
        backerror();
    } // if (longstate == CALL)

    pstack[n] = link(temphead);
    align_state -= unbalance;
    for (m = 0; m <= n; m++) {
        flush_list(pstack[m]);
    }
} // [#396] report_argument

/// [#389] invokes a user-defined control sequence
void macrocall(Pointer refcount) {
    Pointer r, p = 0 /* XXXX */, s, t, u, v, rbraceptr = 0 /* XXXX */,
               savewarningindex;
    /* SmallNumber */ int n;
    HalfWord unbalance, m = 0 /* XXXX */;
    SmallNumber savescannerstatus;
    ASCIICode matchchr = 0 /* XXXX */;
    Pointer pstack[9];

    savescannerstatus = scanner_status;
    savewarningindex = warning_index;
    warning_index = curcs;
    r = link(refcount);
    n = 0;
    if (tracingmacros > 0) { /*401:*/
        begin_diagnostic();
        println();
        print_cs(warning_index);
        tokenshow(refcount);
        end_diagnostic(false);
    }
    /*:401*/
    if (info(r) != endmatchtoken) { /*391:*/
        scanner_status = MATCHING;
        unbalance = 0;
        longstate = eq_type(curcs);
        if (longstate >= OUTER_CALL) longstate -= 2;
        do {
            link(temphead) = 0;
            if ((info(r) > matchtoken + (dwa_do_8 - 1)) |
                (info(r) < matchtoken))
                /*392:*/
                s = 0;
            else {
                matchchr = info(r) - matchtoken;
                s = link(r);
                r = s;
                p = temphead;
                m = 0;
            }
        _Llabcontinue:
            gettoken();
            if (curtok == info(r)) { /*394:*/
                r = link(r);
                if ((info(r) >= matchtoken) & (info(r) <= endmatchtoken)) {
                    if (curtok < leftbracelimit) align_state--;
                    goto _Lfound;
                } else
                    goto _Llabcontinue;
            }
            /*:394*/
            /*397:*/
            if (s != r) {
                if (s == 0) {
                    // [#398] Report an improper use of the macro and abort.
                #ifndef USE_REAL_STR
                    print_err(S(538)); // "Use of "
                    sprint_cs(warning_index);
                    print(S(539)); // " doesn't match its definition"
                #else
                    print_err_str("Use of ");
                    sprint_cs(warning_index);
                    print_str(" doesn't match its definition");
                #endif // USE_REAL_STR
                    /*
                     * (540) "If you say e.g. `\\def\\a1{...}' then you must always"
                     * (541) "put `1' after `\\a' since control sequence names are"
                     * (542) "made up of letters only. The macro here has not been"
                     * (543) "followed by the required stuff so I'm ignoring it."
                     */
                    help4(S(540), S(541), S(542), S(543));
                    error();
                    goto _Lexit;
                } // [#398]

                t = s;
                do {
                    STORE_NEW_TOKEN(p, info(t));
                    m++;
                    u = link(t);
                    v = s;
                    while (true) {
                        if (u == r) {
                            if (curtok != info(v))
                                goto _Ldone;
                            else {
                                r = link(v);
                                goto _Llabcontinue;
                            }
                        }
                        if (info(u) != info(v)) goto _Ldone;
                        u = link(u);
                        v = link(v);
                    }
                _Ldone:
                    t = link(t);
                } while (t != r);
                r = s;
            }
            if (curtok == partoken) {
                if (longstate != LONG_CALL) {
                    report_argument(unbalance, n, pstack);
                    goto _Lexit;
                }
            }
            if (curtok < rightbracelimit) {
                if (curtok < leftbracelimit) { /*399:*/
                    unbalance = 1;
                    while (true) {
                        FAST_STORE_NEW_TOKEN(p, curtok);
                        gettoken();
                        if (curtok == partoken) {
                            if (longstate != LONG_CALL) {
                                report_argument(unbalance, n, pstack);
                                goto _Lexit;
                            }
                        }
                        if (curtok >= rightbracelimit) continue;
                        if (curtok < leftbracelimit)
                            unbalance++;
                        else {
                            unbalance--;
                            if (unbalance == 0) goto _Ldone1;
                        }
                    }
                _Ldone1:
                    rbraceptr = p;
                    STORE_NEW_TOKEN(p, curtok);
                } else { /*395:*/
                    // [#395] Report an extra right brace and goto continue.
                    // a white lie; the \par won’t always trigger a runaway.
                    backinput();
                #ifndef USE_REAL_STR
                    print_err(S(544)); // "Argument of "
                    sprint_cs(warning_index);
                    print(S(545)); // " has an extra }"
                #else
                    print_err_str("Argument of ");
                    sprint_cs(warning_index);
                    print_str(" has an extra }");
                #endif // USE_REAL_STR
                    /* 
                     * [546] "I've run across a `}' that doesn't seem to match anything."
                     * [547] "For example `\\def\\a#1{...}' and `\\a}' would produce"
                     * [548] "this error. If you simply proceed now the `\\par' that"
                     * [549] "I've just inserted will cause me to report a runaway"
                     * [550] "argument that might be the root of the problem. But if"
                     * [550] "your `}' was spurious just type `2' and it will go away."
                     */
                    help6(S(546), S(547), S(548), S(549), S(550), S(551));
                    align_state++;
                    longstate = CALL;
                    curtok = partoken;
                    inserror();
                } // [#395]
                /*:399*/
            } else /*393:*/
            {      /*:393*/
                if (curtok == spacetoken) {
                    if (info(r) <= endmatchtoken) {
                        if (info(r) >= matchtoken) goto _Llabcontinue;
                    }
                }
                STORE_NEW_TOKEN(p, curtok);
            }
            m++;
            if (info(r) > endmatchtoken) goto _Llabcontinue;
            if (info(r) < matchtoken) goto _Llabcontinue;
        _Lfound:
            if (s != 0) { /*400:*/
                if (((m == 1) & (info(p) < rightbracelimit)) && p != temphead) {
                    link(rbraceptr) = 0;
                    FREE_AVAIL(p);
                    p = link(temphead);
                    pstack[n] = link(p);
                    FREE_AVAIL(p);
                } else
                    pstack[n] = link(temphead);
                n++;
                if (tracingmacros > 0) {
                    begin_diagnostic();
                    printnl(matchchr);
                    print_int(n);
                    print(S(552));
                    showtokenlist(pstack[n - 1], 0, 1000);
                    end_diagnostic(false);
                }
            }
            /*:400*/
            /*:392*/
        } while (info(r) != endmatchtoken);
    }
    /*:391*/
    /*390:*/
    while (STATE == TOKEN_LIST && LOC == 0)
        endtokenlist();
    begintokenlist(refcount, MACRO);
    NAME = warning_index;
    LOC = link(r);
    if (n > 0) { /*:390*/
        if (paramptr + n > maxparamstack) {
            maxparamstack = paramptr + n;
            // "parameter stack size"
            if (maxparamstack > PARAM_SIZE) overflow(S(553), PARAM_SIZE);
        }
        for (m = 0; m < n; m++)
            paramstack[paramptr + m] = pstack[m];
        paramptr += n;
    }
_Lexit:
    scanner_status = savescannerstatus;
    warning_index = savewarningindex;

    /*:397*/
    /*:395*/
} // [#389] macrocall

/// [#379] 插入 `\relax` .
/// Sometimes the expansion looks too far ahead,
/// so we want to insert a harmless `\relax` into the user’s input.
void insertrelax(void) {
    curtok = CS_TOKEN_FLAG + curcs;
    backinput();
    curtok = CS_TOKEN_FLAG + FROZEN_RELAX;
    backinput();
    token_type = INSERTED;
} // [#379] insertrelax

/// [p143#366]
void expand(void) {
    HalfWord t;
    Pointer p, r, backupbackup;
    short j;
    long cvbackup;
    SmallNumber cvlbackup, radixbackup, cobackup, savescannerstatus;

    cvbackup = cur_val;
    cvlbackup = cur_val_level;
    radixbackup = radix;
    cobackup = cur_order;

    backupbackup = link(backuphead);

    if (curcmd < CALL) { /*367:*/
        if (tracingcommands > 1) showcurcmdchr();
        switch (curcmd) {

            case TOP_BOT_MARK: /*386:*/
                if (curmark[curchr - topmarkcode] != 0)
                    begintokenlist(curmark[curchr - topmarkcode], MARK_TEXT);
                break;
                /*:386*/

            case EXPAND_AFTER: /*368:*/
                gettoken();
                t = curtok;
                gettoken();
                if (curcmd > MAX_COMMAND)
                    expand();
                else
                    backinput();
                curtok = t;
                backinput();
                break;
                /*:368*/

            case NO_EXPAND: /*369:*/
                savescannerstatus = scanner_status;
                scanner_status = NORMAL;
                gettoken();
                scanner_status = savescannerstatus;
                t = curtok;
                backinput();
                if (t >= CS_TOKEN_FLAG) {
                    p = get_avail();
                    info(p) = CS_TOKEN_FLAG + FROZEN_DONT_EXPAND;
                    link(p) = LOC;
                    START = p;
                    LOC = p;
                }
                break;
                /*:369*/

            case CS_NAME: /*372:*/
                r = get_avail();
                p = r;
                do {
                    get_x_token();
                    if (curcs == 0) {
                        STORE_NEW_TOKEN(p, curtok);
                    }
                } while (curcs == 0);

                if (curcmd != END_CS_NAME) {
                    // [#373] Complain about missing \endcsname
                #ifndef USE_REAL_STR
                    print_err(S(554)); // "Missing "
                    print_esc(S(263)); // "endcsname"
                    print(S(555)); // " inserted"
                #else
                    print_err_str("Missing ");
                    print_esc_str("endcsname");
                    print_str(" inserted");
                #endif // USE_REAL_STR
                    // "The control sequence marked <to be read again> should"
                    // "not appear between \\csname and \\endcsname."
                    help2(S(556), S(557));
                    backerror();
                } // [#373]
                
                /*374:*/
                j = first;
                p = link(r);
                while (p != 0) {
                    if (j >= max_buf_stack) {
                        max_buf_stack = j + 1;
                        if (max_buf_stack == BUF_SIZE) {
                            // "buffer size"
                            overflow(S(511), BUF_SIZE);
                        }
                    }
                    buffer[j] = (info(p)) & (dwa_do_8 - 1);
                    j++;
                    p = link(p);
                }
                if (j > first + 1) {
                    curcs = idlookup_p(buffer + first, j - first, false);
                } else if (j == first)
                    curcs = NULL_CS;
                else
                    curcs = ACTIVE_BASE + buffer[first]; /*:374*/
                flush_list(r);
                if (eq_type(curcs) == UNDEFINED_CS) eq_define(curcs, RELAX, 256);
                curtok = curcs + CS_TOKEN_FLAG;
                backinput();
                break;
                /*:372*/

            case CONVERT: convtoks(); break;
            case THE: insthetoks(); break;
            case IF_TEST: conditional(); break;

            case FI_OR_ELSE:
                // [#510]  Terminate the current conditional and skip to \fi
                if (curchr > iflimit) {
                    if (iflimit == ifcode) {
                        insertrelax(); // condition not yet evaluated
                    } else {
                    #ifndef USE_REAL_STR
                        print_err(S(558)); // "Extra "
                    #else
                        print_err_str("Extra ");
                    #endif // USE_REAL_STR
                        printcmdchr(FI_OR_ELSE, curchr);
                        // "I'm ignoring this; it doesn't match any \\if."
                        help1(S(559));
                        error();
                    }
                } else {
                    while (curchr != ficode) {
                        passtext(); // skip to `\fi`
                    }
                    // [#496] Pop the condition stack
                    p = condptr;
                    ifline = iflinefield(p);
                    curif = subtype(p);
                    iflimit = type(p);
                    condptr = link(p);
                    free_node(p, ifnodesize);
                } // [#510, 496] if (curchr <> iflimit)
                break;

            case INPUT: /*378:*/
                if (curchr > 0)
                    force_eof = true;
                else if (name_in_progress)
                    insertrelax();
                else
                    startinput();
                break;
                /*370:*/

            default:
            #ifndef USE_REAL_STR
                print_err(S(560)); // "Undefined control sequence"
            #else
                print_err_str("Undefined control sequence");
            #endif // USE_REAL_STR
                /*
                 * (561) "The control sequence at the end of the top line"
                 * (562) "of your error message was never \\def'ed. If you have"
                 * (563) "misspelled it (e.g. `\\hobx') type `I' and the correct"
                 * (564) "spelling (e.g. `I\\hbox'). Otherwise just continue"
                 * (565) "and I'll forget about whatever was undefined."
                 */
                help5(S(561), S(562), S(563), S(564), S(565));
                error(); /*:370*/
                break;
        }
    } /*:367*/
    else if (curcmd < END_TEMPLATE)
        macrocall(curchr);
    else {
        curtok = CS_TOKEN_FLAG + FROZEN_ENDV;
        backinput();
    }
    cur_val = cvbackup;
    cur_val_level = cvlbackup;
    radix = radixbackup;
    cur_order = cobackup;

    link(backuphead) = backupbackup;

    /*:378*/
} // [#366] expand

// [#380]: get an expanded token
//  sets cur_cmd, cur_chr, cur_tok, and expands macros
//
// xref[26]
//  364, 366, 372, [380], 381,
//  402, 404, 406, 407, 443,
//  444, 445, 452, 465, 479,
//  506, 526, 780, 935, 961,
//  1029, 1030, 1138, 1197, 1237,
//  1375
void get_x_token(void) {
    while (true) {
        getnext();
        if (curcmd <= MAX_COMMAND) break;
        if (curcmd >= CALL) {
            if (curcmd < END_TEMPLATE) {
                macrocall(curchr);
            } else {
                curcs = FROZEN_ENDV;
                curcmd = ENDV;
                break; // cur_chr = null_list
            }          // if (curcmd <> endtemplate)
        } else {
            // maxcommand < curcmd < call
            expand();
        } // if (curcmd <> call)
    }     // while (true)

    curtok = pack_tok(curcs, curcmd, curchr);
} // #380: get_x_token

/// [#381] #get_x_token without the initial #getnext
void xtoken(void) {
    while (curcmd > MAX_COMMAND) {
        expand();
        getnext();
    }
    curtok = pack_tok(curcs, curcmd, curchr);
} // [#381]

/** @} */ // end group S366x401_P144x154


/** @addtogroup S464x486_P174x180
 * @{
 */

Static Pointer tex_global_p;

Static void strtoks_helper(ASCIICode t) {
    long tt = t;
    Pointer p = tex_global_p;
    if (tt == ' ')
        tt = spacetoken;
    else
        tt += othertoken;
    FAST_STORE_NEW_TOKEN(p, tt);
    tex_global_p = p;
}

Static HalfWord strtoks(StrPoolPtr b) {
    Pointer p;
    str_room(1);
    p = temphead;
    link(p) = 0;
    tex_global_p = p;
    str_map_from_mark(b, strtoks_helper);
    p = tex_global_p;
    return p;
}
/*:464*/

/*465:*/
HalfWord thetoks(void) {
    Selector old_setting;
    Pointer p, r;

    get_x_token();
    scan_something_internal(TOK_VAL, false);
    if (cur_val_level >= IDENT_VAL) { /*466:*/
        p = temphead;
        link(p) = 0;
        if (cur_val_level == IDENT_VAL) {
            STORE_NEW_TOKEN(p, CS_TOKEN_FLAG + cur_val);
            return p;
        }
        if (cur_val == 0) return p;
        r = link(cur_val);
        while (r != 0) {
            FAST_STORE_NEW_TOKEN(p, info(r));
            r = link(r);
        }
        return p;
    } else {
        StrPoolPtr b = str_mark();
        old_setting = selector;
        selector = NEW_STRING;
        switch (cur_val_level) {

            case INT_VAL: print_int(cur_val); break;

            case DIMEN_VAL:
                print_scaled(cur_val);
                print(S(459));
                break;

            case GLUE_VAL:
                printspec(cur_val, S(459));
                delete_glue_ref(cur_val);
                break;

            case MU_VAL:
                printspec(cur_val, S(390));
                delete_glue_ref(cur_val);
                break;
        }
        selector = old_setting;
        /*	fprintf(stderr,"(%d %d)",bb-b,pool_ptr-b); */
        return (strtoks(b));
    }
    /*:466*/
}
/*:465*/

/// [#467] Here’s part of the expand subroutine 
//// that we are now ready to complete.
static void insthetoks(void) {
    link(garbage) = thetoks();
    inslist(link(temphead));
} // [#467] insthetoks

/// [#470] The procedure conv toks uses str toks to 
/// insert the token list for convert functions into the scanner;
/// `\outer` control sequences are allowed to follow `\string` and `\meaning`.
static void convtoks(void) {
    Selector old_setting;
    char c;
    SmallNumber savescannerstatus;
    StrPoolPtr b;

    c = curchr;  /*471:*/
    switch (c) { /*:471*/
        case numbercode:
        case romannumeralcode:
            scan_int(); 
            break;

        case stringcode:
        case meaningcode:
            savescannerstatus = scanner_status;
            scanner_status = NORMAL;
            gettoken();
            scanner_status = savescannerstatus;
            break;

        case fontnamecode: 
            scanfontident(); 
            break;

        case jobnamecode:
            if (job_name == 0) openlogfile();
            break;
    }
    old_setting = selector;
    selector = NEW_STRING;
    b = str_mark();
    /*472:*/
    switch (c) { /*:472*/
        case numbercode: print_int(cur_val); break;
        case romannumeralcode: print_roman_int(cur_val); break;

        case stringcode:
            if (curcs != 0)
                sprint_cs(curcs);
            else
                print_char(curchr);
            break;

        case meaningcode: printmeaning(curchr, curcmd); break;

        case fontnamecode:
            print(get_fontname(cur_val));
            if (get_fontsize(cur_val) != get_fontdsize(cur_val)) {
                print(S(642));
                print_scaled(get_fontsize(cur_val));
                print(S(459));
            }
            break;

        case jobnamecode: print(job_name); break;
    }
    selector = old_setting;
    link(garbage) = strtoks(b);
    inslist(link(temphead));
} // [#470] convtoks

/*473:*/
HalfWord scantoks(Boolean macrodef, Boolean xpand) {
    HalfWord t, s, unbalance, hashbrace;
    Pointer p;

    if (macrodef) {
        scanner_status = DEFINING;
    } else {
        scanner_status = ABSORBING;
    }

    warning_index = curcs;
    defref = get_avail();
    tokenrefcount(defref) = 0;
    p = defref;
    hashbrace = 0;
    t = ZERO_TOKEN;

    if (macrodef) { /*474:*/
        while (true) {
            gettoken();
            if (curtok < rightbracelimit) goto _Ldone1;
            if (curcmd == MAC_PARAM) { /*476:*/
                s = matchtoken + curchr;
                gettoken();
                if (curcmd == LEFT_BRACE) {
                    hashbrace = curtok;
                    STORE_NEW_TOKEN(p, curtok);
                    STORE_NEW_TOKEN(p, endmatchtoken);
                    goto _Ldone;
                }
                if (t == ZERO_TOKEN + 9) {
                    print_err(S(643)); // "You already have nine parameters"
                    // "I'm going to ignore the # sign you just used."
                    help1(S(644));
                    error();
                } else {
                    t++;
                    if (curtok != t) {
                        // "Parameters must be numbered consecutively"
                        print_err(S(645));
                        // "I've inserted the digit you should have used after the #."
                        // "Type `1' to delete what you did use."
                        help2(S(646), S(647));
                        backerror();
                    }
                    curtok = s;
                }
            }
            /*:476*/
            STORE_NEW_TOKEN(p, curtok);
        }

    _Ldone1:
        STORE_NEW_TOKEN(p, endmatchtoken);
        if (curcmd == RIGHT_BRACE) { /*475:*/
            print_err(S(566)); // "Missing { inserted"
            align_state++;
            // "Where was the left brace? You said something like `\\def\\a}'"
            // "which I'm going to interpret as `\\def\\a{}'."
            help2(S(648), S(649));
            error();
            goto _Lfound;
        } /*:475*/

    _Ldone:;
    } else {
        scan_left_brace();
    } /*:474*/

    /*477:*/
    unbalance = 1;
    while (true) {   /*:477*/
        if (xpand) { /*478:*/
            while (true) {
                getnext();
                if (curcmd <= MAX_COMMAND) goto _Ldone2;
                if (curcmd != THE) {
                    expand();
                } else {
                    Pointer q = thetoks();
                    if (link(temphead) != 0) {
                        link(p) = link(temphead);
                        p = q;
                    }
                }
            }

        _Ldone2:
            xtoken();
        } else {
            gettoken();
        }

        /*:478*/
        if (curtok < rightbracelimit) {
            if (curcmd < RIGHT_BRACE) {
                unbalance++;
            } else {
                unbalance--;
                if (unbalance == 0) goto _Lfound;
            }
        } else if (curcmd == MAC_PARAM) {
            if (macrodef) { /*479:*/
                s = curtok;
                if (xpand) {
                    get_x_token();
                } else {
                    gettoken();
                }
                if (curcmd != MAC_PARAM) {
                    if (curtok <= ZERO_TOKEN || curtok > t) {
                        // "Illegal parameter number in definition of "
                        print_err(S(650));
                        sprint_cs(warning_index);
                        /*
                         * "You meant to type ## instead of # right?"
                         * "Or maybe a } was forgotten somewhere earlier and things"
                         * "are all screwed up? I'm going to assume that you meant ##."
                         */
                        help3(S(651), S(652), S(653));
                        backerror();
                        curtok = s;
                    } else {
                        curtok = outparamtoken - '0' + curchr;
                    }
                }
            } /*:479*/
        }
        STORE_NEW_TOKEN(p, curtok);
    }

_Lfound:
    scanner_status = NORMAL;
    if (hashbrace != 0) {
        STORE_NEW_TOKEN(p, hashbrace);
    }

    return p;
} /*:473*/

/// [#481]
void build_token_init() {
    for (size_t k = 0; k <= 16; k++) {
        readopen[k] = closed;
    }
} /* build_token_init */

/// [#482] constructs a token list like that for any macro definition,
///     and makes cur val point to it.
/// Parameter r points to the control sequence 
///     that will receive this token list.
void readtoks(long n, HalfWord r) {
    Pointer p; // tail of the token list
    long s;    // saved value of align_state
    /* SmallNumber */ int m; // stream number

    scanner_status = DEFINING;
    warning_index = r;
    defref = get_avail();
    tokenrefcount(defref) = 0;
    p = defref; // the reference count
    STORE_NEW_TOKEN(p, endmatchtoken);

    // OLD: `if ((unsigned long)n > 15)`
    if (n < 0 || n > 15) {
        m = 16;
    } else {
        m = n;
    }

    s = align_state;
    align_state = 1000000L; // disable tab marks, etc.

    do { /// [#483] Input and store tokens from the next line of the file
        beginfilereading();
        NAME = m + 1;
        if (readopen[m] == closed) {
            /// [#484] Input for `\\read` from the terminal.
            if (interaction > NON_STOP_MODE) {
                if (n < 0) {
                    print(S(385)); // ""
                    term_input();
                } else {
                    println();
                    sprint_cs(r);
                    print('=');
                    term_input();
                    n = -1;
                }
            } else {
                // "*** (cannot \read from terminal in nonstop modes)"
                fatalerror(S(654));
            } // if (interaction <=> NON_STOP_MODE)
        } else if (readopen[m] == justopen) {
            /// [#485] Input the first line of read_file[m].
            if (inputln(readfile[m], false)) {
                readopen[m] = NORMAL;
            } else {
                aclose(&readfile[m]);
                readopen[m] = closed;
            }
        } else {
            /// [#486] Input the next line of read_file[m].
            if (!inputln(readfile[m], true)) {
                aclose(&readfile[m]);
                readopen[m] = closed;
                if (align_state != 1000000L) {
                    runaway();
                    print_err(S(655)); // "File ended within "
                    print_esc(S(656)); // "read"
                    help1(S(657));     // "This \\read has unbalanced braces."
                    align_state = 1000000L;
                    error();
                }
            }
        } // if (readopen[m] <=>)

        LIMIT = last;
        if (end_line_char_inactive) {
            LIMIT--;
        } else {
            buffer[LIMIT] = end_line_char;
        }
        first = LIMIT + 1;
        LOC = START;
        STATE = NEW_LINE;
    
        while (true) {
            gettoken();
            // cur_cmd = cur_chr = 0 will occur at the end of the line
            if (curtok == 0) goto _Ldone;
            if (align_state < 1000000L) { // unmatched ‘}’ aborts the line
                do {
                    gettoken();
                } while (curtok != 0);
                align_state = 1000000L;
                goto _Ldone;
            }
            STORE_NEW_TOKEN(p, curtok);
        } // while (true)

    _Ldone: /*:483*/    
        endfilereading();
    } while (align_state != 1000000L);

    cur_val = defref;
    scanner_status = NORMAL;
    align_state = s;
} /* [#482] readtoks */
/** @} */ // end group S464x486_P174x180


/** @addtogroup S487x510_P181x187
 *
 * + #passtext
 * + #conditional
 * + #changeiflimit
 *
 * @{
 */
/// [#490]
void cond_process_init() {
    condptr = null;
    iflimit = NORMAL;
    curif = 0;
    ifline = 0;
} /* cond_process_init */

/// [#494] ignores text until coming to an 
/// `\or`, `\else`, or `\fi` at level zero of `\if...\fi` nesting.
static void passtext(void) {
    long l;
    SmallNumber savescannerstatus;

    savescannerstatus = scanner_status;
    scanner_status = SKIPPING;
    l = 0;
    skipline = line;
    while (true) {
        getnext();
        if (curcmd == FI_OR_ELSE) {
            if (l == 0) break;
            if (curchr == ficode) l--;
        } else if (curcmd == IF_TEST) {
            l++;
        }
    }
    scanner_status = savescannerstatus;
} // [#494] passtext

/// [#497] changes the #iflimit code corresponding to a given value of #condptr.
static void changeiflimit(SmallNumber l, HalfWord p) {
    Pointer q;

    if (p == condptr) {
        iflimit = l;
    } else {
        q = condptr;
        while (true) {
            if (q == 0) confusion(S(658));
            if (link(q) == p) {
                type(q) = l;
                return;
            }
            q = link(q);
        }
    }
} // [#497] changeiflimit

#define getxtokenoractivechar()                                           \
    (get_x_token(), ((curcmd == RELAX) && (curchr == noexpandflag))       \
                        ? (curcmd = ACTIVE_CHAR,                          \
                           cur_chr = curtok - CS_TOKEN_FLAG - ACTIVE_BASE) \
                        : (cur_chr = curchr))


/// [#498] when the expand procedure encounters an if test command.
static void conditional(void) { /*495:*/
    Boolean b = false /* XXXX */;
    long r;
    long m, n;
    Pointer p, q, savecondptr;
    SmallNumber savescannerstatus, thisif;

    p = get_node(ifnodesize);
    link(p) = condptr;
    type(p) = iflimit;
    subtype(p) = curif;
    iflinefield(p) = ifline;
    condptr = p;
    curif = curchr;
    iflimit = ifcode;
    ifline = line; /*:495*/
    savecondptr = condptr;
    thisif = curchr;  /*501:*/
    switch (thisif) { /*:501*/
        case IF_CHAR_CODE:
        case IF_CAT_CODE: /*506:*/
        {
            int cur_chr = curchr;
            getxtokenoractivechar();
            if (curcmd > ACTIVE_CHAR || cur_chr > 255) {
                m = RELAX;
                n = 256;
            } else {
                m = curcmd;
                n = cur_chr;
            }
            getxtokenoractivechar();
            if (curcmd > ACTIVE_CHAR || cur_chr > 255) {
                curcmd = RELAX;
                cur_chr = 256;
            }
            if (thisif == IF_CHAR_CODE)
                b = (n == cur_chr);
            else
                b = (m == curcmd);
        } break;
            /*:506*/

        case IF_INT_CODE:
        case IF_DIM_CODE: /*503:*/
            if (thisif == IF_INT_CODE)
                scan_int();
            else {
                SCAN_NORMAL_DIMEN();
            }
            n = cur_val;
            skip_spaces();
            if ((curtok >= othertoken + '<') & (curtok <= othertoken + '>'))
                r = curtok - othertoken;
            else {
            #ifndef USE_REAL_STR
                print_err(S(659)); // "Missing = inserted for "
            #else
                print_err_str("Missing = inserted for ");
            #endif // USE_REAL_STR
                printcmdchr(IF_TEST, thisif);
                // "I was expecting to see `<' `=' or `>'. Didn't."
                help1(S(660));
                backerror();
                r = '=';
            }
            if (thisif == IF_INT_CODE)
                scan_int();
            else {
                SCAN_NORMAL_DIMEN();
            }
            switch (r) {
                case '<': b = (n < cur_val); break;
                case '=': b = (n == cur_val); break;
                case '>': b = (n > cur_val); break;
            }
            break;
            /*:503*/

        case IF_ODD_CODE: /*504:*/
            scan_int();
            b = cur_val & 1;
            break;
            /*:504*/

        case IF_VMODE_CODE: b = (labs(mode) == V_MODE); break;
        case IF_HMODE_CODE: b = (labs(mode) == H_MODE); break;
        case IF_MMODE_CODE: b = (labs(mode) == M_MODE); break;
        case IF_INNER_CODE: b = (mode < 0); break;

        case IF_VOID_CODE:
        case IF_HBOX_CODE:
        case IF_VBOX_CODE: /*505:*/
            scan_eight_bit_int();
            p = box(cur_val);
            if (thisif == IF_VOID_CODE)
                b = (p == 0);
            else if (p == 0)
                b = false;
            else if (thisif == IF_HBOX_CODE)
                b = (type(p) == HLIST_NODE);
            else
                b = (type(p) == VLIST_NODE);
            break;
            /*:505*/

        case IF_X_CODE: /*507:*/
            savescannerstatus = scanner_status;
            scanner_status = NORMAL;
            getnext();
            n = curcs;
            p = curcmd;
            q = curchr;
            getnext();
            if (curcmd != p)
                b = false;
            else if (curcmd < CALL)
                b = (curchr == q);
            else {
                /* 508:*/
                p = link(curchr);
                q = link(equiv(n));
                if (p == q)
                    b = true;
                else {
                    while (p != 0 && q != 0) {
                        if (info(p) != info(q))
                            p = 0;
                        else {
                            p = link(p);
                            q = link(q);
                        }
                    }
                    b = (p == 0 && q == 0);
                }
            }
            scanner_status = savescannerstatus;
            break;
            /*:507*/

        case IF_EOF_CODE:
            scan_four_bit_int();
            b = (readopen[cur_val] == closed);
            break;

        case IF_TRUE_CODE: b = true; break;
        case IF_FALSE_CODE: b = false; break;
        case IF_CASE_CODE: /*509:*/
            scan_int();
            n = cur_val;
            if (tracingcommands > 1) {
                begin_diagnostic();
                print(S(661)); // "{case "
                print_int(n);
                print_char('}');
                end_diagnostic(false);
            }
            while (n != 0) {
                passtext();
                if (condptr == savecondptr) {
                    if (curchr != orcode) goto _Lcommonending;
                    n--;
                    continue;
                }
                if (curchr != ficode) /*496:*/
                    continue;
                /*:496*/
                p = condptr;
                ifline = iflinefield(p);
                curif = subtype(p);
                iflimit = type(p);
                condptr = link(p);
                free_node(p, ifnodesize);
            }
            changeiflimit(orcode, savecondptr);
            goto _Lexit;
            break;
            /*:509*/
    }
    if (tracingcommands > 1) { /*502:*/
        begin_diagnostic();
        if (b)
            print(S(662)); // "{true}"
        else
            print(S(663)); // "{false}"
        end_diagnostic(false);
    }
    /*:502*/
    if (b) {
        changeiflimit(elsecode, savecondptr);
        goto _Lexit;
    }              /*500:*/
    while (true) { /*:500*/
        passtext();
        if (condptr == savecondptr) {
            if (curchr != orcode) goto _Lcommonending;
        #ifndef USE_REAL_STR
            print_err(S(558)); // "Extra "
            print_esc(S(664)); // "or"
        #else
            print_err_str("Extra ");
            print_esc_str("or");
        #endif // USE_REAL_STR
            help1(S(559)); // "I'm ignoring this; it doesn't match any \\if."
            error();
            continue;
        }
        if (curchr != ficode) /*496:*/
            continue;
        /*:496*/
        p = condptr;
        ifline = iflinefield(p);
        curif = subtype(p);
        iflimit = type(p);
        condptr = link(p);
        free_node(p, ifnodesize);
    }
_Lcommonending:
    if (curchr == ficode) { /*496:*/
        p = condptr;
        ifline = iflinefield(p);
        curif = subtype(p);
        iflimit = type(p);
        condptr = link(p);
        free_node(p, ifnodesize);
    } /*:496*/
    else
        iflimit = ficode;
_Lexit:;

    /*:508*/
} // [#498] conditional

/** @} */ // end group S487x510
