#include <stdlib.h> // [func] labs
#include "tex.h"
#include "texmac.h" // [macro] link
#include "tex_inc.h" // [macro] STORE_NEW_TOKEN, FAST_STORE_NEW_TOKEN, FREE_AVAIL
#include "macros.h" // [macro] help1, help2, help3, help4,
#include "global.h"
#include "texfunc.h"
#include "lexer.h" // [var] warning_index,
#include "printout.h" // [func] printcmdchr
#include "print.h"    // [var] selector
#include "fonts.h"    // [func] get_fontname, get_fontsize
#include "texmath.h"  // [func] print_scaled
#include "scan.h"     // [func] scaneightbitint
#include "expand.h"

/** @addtogroup S487x510
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

/** @} */ // end group S487x510

/// [#387] governs the acceptance of `\par` .
char longstate;

static void conditional(void);
static void passtext(void);
static void convtoks(void);
static void insthetoks(void);


/** @defgroup S366x401 PART 25: EXPANDING THE NEXT TOKEN
 * [ p144~155#366~401 ]
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

/// [#396] If `longstate == outer_call`, 
/// a runaway argument has already been reported.
void report_argument(HalfWord unbalance, int n, Pointer* pstack) {
    HalfWord m;
    if (longstate == call) {
        runaway();
        printnl(S(292));
        print(S(533));
        sprint_cs(warning_index);
        print(S(534));
        help3(S(535), S(536), S(537));
        backerror();
    }
    pstack[n] = link(temphead);
    align_state -= unbalance;
    for (m = 0; m <= n; m++) {
        flushlist(pstack[m]);
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
        begindiagnostic();
        println();
        print_cs(warning_index);
        tokenshow(refcount);
        enddiagnostic(false);
    }
    /*:401*/
    if (info(r) != endmatchtoken) { /*391:*/
        scanner_status = MATCHING;
        unbalance = 0;
        longstate = eqtype(curcs);
        if (longstate >= outercall) longstate -= 2;
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
                if (s == 0) { /*398:*/
                    printnl(S(292));
                    print(S(538));
                    sprint_cs(warning_index);
                    print(S(539));
                    help4(S(540), S(541), S(542), S(543));
                    error();
                    goto _Lexit;
                }
                /*:398*/
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
                if (longstate != longcall) {
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
                            if (longstate != longcall) {
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
                    backinput();
                    printnl(S(292));
                    print(S(544));
                    sprint_cs(warning_index);
                    print(S(545));
                    help6(S(546), S(547), S(548), S(549), S(550), S(551));
                    align_state++;
                    longstate = call;
                    curtok = partoken;
                    inserror();
                }
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
                    begindiagnostic();
                    printnl(matchchr);
                    print_int(n);
                    print(S(552));
                    showtokenlist(pstack[n - 1], 0, 1000);
                    enddiagnostic(false);
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
            if (maxparamstack > paramsize) overflow(S(553), paramsize);
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
    curtok = CS_TOKEN_FLAG + frozenrelax;
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

    if (curcmd < call) { /*367:*/
        if (tracingcommands > 1) showcurcmdchr();
        switch (curcmd) {

            case topbotmark: /*386:*/
                if (curmark[curchr - topmarkcode] != 0)
                    begintokenlist(curmark[curchr - topmarkcode], MARK_TEXT);
                break;
                /*:386*/

            case expandafter: /*368:*/
                gettoken();
                t = curtok;
                gettoken();
                if (curcmd > maxcommand)
                    expand();
                else
                    backinput();
                curtok = t;
                backinput();
                break;
                /*:368*/

            case noexpand: /*369:*/
                savescannerstatus = scanner_status;
                scanner_status = NORMAL;
                gettoken();
                scanner_status = savescannerstatus;
                t = curtok;
                backinput();
                if (t >= CS_TOKEN_FLAG) {
                    p = get_avail();
                    info(p) = CS_TOKEN_FLAG + frozendontexpand;
                    link(p) = LOC;
                    START = p;
                    LOC = p;
                }
                break;
                /*:369*/

            case csname: /*372:*/
                r = get_avail();
                p = r;
                do {
                    get_x_token();
                    if (curcs == 0) {
                        STORE_NEW_TOKEN(p, curtok);
                    }
                } while (curcs == 0);
                if (curcmd != endcsname) { /*373:*/
                    printnl(S(292));
                    print(S(554));
                    print_esc(S(263));
                    print(S(555));
                    help2(S(556), S(557));
                    backerror();
                }
                /*:373*/
                /*374:*/
                j = first;
                p = link(r);
                while (p != 0) {
                    if (j >= max_buf_stack) {
                        max_buf_stack = j + 1;
                        if (max_buf_stack == BUF_SIZE)
                            overflow(S(511), BUF_SIZE);
                    }
                    buffer[j] = (info(p)) & (dwa_do_8 - 1);
                    j++;
                    p = link(p);
                }
                if (j > first + 1) {
                    curcs = idlookup_p(buffer + first, j - first, false);
                } else if (j == first)
                    curcs = nullcs;
                else
                    curcs = singlebase + buffer[first]; /*:374*/
                flushlist(r);
                if (eqtype(curcs) == undefinedcs) eqdefine(curcs, relax, 256);
                curtok = curcs + CS_TOKEN_FLAG;
                backinput();
                break;
                /*:372*/

            case convert: convtoks(); break;

            case the: insthetoks(); break;

            case iftest: conditional(); break;

            case fiorelse: /*510:*/
                if (curchr > iflimit) {
                    if (iflimit == ifcode)
                        insertrelax();
                    else {
                        printnl(S(292));
                        print(S(558));
                        printcmdchr(fiorelse, curchr);
                        help1(S(559));
                        error();
                    }
                } else {                     /*:510*/
                    while (curchr != ficode) /*496:*/
                        passtext();
                    p = condptr;
                    ifline = iflinefield(p);
                    curif = subtype(p);
                    iflimit = type(p);
                    condptr = link(p);
                    freenode(p, ifnodesize); /*:496*/
                }
                break;

            case input: /*378:*/
                if (curchr > 0)
                    force_eof = true;
                else if (name_in_progress)
                    insertrelax();
                else
                    startinput();
                break;
                /*370:*/

            default:
                printnl(S(292));
                print(S(560));
                help5(S(561), S(562), S(563), S(564), S(565));
                error(); /*:370*/
                break;
        }
    } /*:367*/
    else if (curcmd < endtemplate)
        macrocall(curchr);
    else {
        curtok = CS_TOKEN_FLAG + frozenendv;
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
        if (curcmd <= maxcommand) break;
        if (curcmd >= call) {
            if (curcmd < endtemplate) {
                macrocall(curchr);
            } else {
                curcs = frozenendv;
                curcmd = endv;
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
    while (curcmd > maxcommand) {
        expand();
        getnext();
    }
    curtok = pack_tok(curcs, curcmd, curchr);
} // [#381] 

/** @} */ // end group S366x401


/** @addtogroup S464x486 PART 27: BUILDING TOKEN LISTS
 * 
 * @{
 */

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
    enum Selector old_setting;
    char c;
    SmallNumber savescannerstatus;
    StrPoolPtr b;

    c = curchr;  /*471:*/
    switch (c) { /*:471*/
        case numbercode:
        case romannumeralcode:
            scanint(); 
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
/** @} */ // end group S464x486


/** @defgroup S487x510 PART 28: CONDITIONAL PROCESSING
 * [ p181~187#487~510 ]
 *
 * + #passtext
 * + #conditional
 * + #changeiflimit
 *
 * @{
 */

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
        if (curcmd == fiorelse) {
            if (l == 0) break;
            if (curchr == ficode) l--;
        } else if (curcmd == iftest) {
            l++;
        }
    }
    scanner_status = savescannerstatus;
} // [#494] passtext

/// [#497] changes the #iflimit code corresponding to a given value of #condptr.
static void changeiflimit(SmallNumber l, HalfWord p) {
    Pointer q;

    if (p == condptr)
        iflimit = l;
    else {
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
    (get_x_token(), ((curcmd == relax) && (curchr == noexpandflag))       \
                        ? (curcmd = ACTIVE_CHAR,                          \
                           cur_chr = curtok - CS_TOKEN_FLAG - activebase) \
                        : (cur_chr = curchr))


/// [#498] when the expand procedure encounters an if test command.
static void conditional(void) { /*495:*/
    Boolean b = false /* XXXX */;
    long r;
    long m, n;
    Pointer p, q, savecondptr;
    SmallNumber savescannerstatus, thisif;

    p = getnode(ifnodesize);
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
                m = relax;
                n = 256;
            } else {
                m = curcmd;
                n = cur_chr;
            }
            getxtokenoractivechar();
            if (curcmd > ACTIVE_CHAR || cur_chr > 255) {
                curcmd = relax;
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
                scanint();
            else {
                SCAN_NORMAL_DIMEN();
            }
            n = cur_val;
            skip_spaces();
            if ((curtok >= othertoken + '<') & (curtok <= othertoken + '>'))
                r = curtok - othertoken;
            else {
                printnl(S(292));
                print(S(659));
                printcmdchr(iftest, thisif);
                help1(S(660));
                backerror();
                r = '=';
            }
            if (thisif == IF_INT_CODE)
                scanint();
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
            scanint();
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
            scaneightbitint();
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
            else if (curcmd < call)
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
            scanfourbitint();
            b = (readopen[cur_val] == closed);
            break;

        case IF_TRUE_CODE: b = true; break;
        case IF_FALSE_CODE: b = false; break;
        case IF_CASE_CODE: /*509:*/
            scanint();
            n = cur_val;
            if (tracingcommands > 1) {
                begindiagnostic();
                print(S(661));
                print_int(n);
                print_char('}');
                enddiagnostic(false);
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
                freenode(p, ifnodesize);
            }
            changeiflimit(orcode, savecondptr);
            goto _Lexit;
            break;
            /*:509*/
    }
    if (tracingcommands > 1) { /*502:*/
        begindiagnostic();
        if (b)
            print(S(662));
        else
            print(S(663));
        enddiagnostic(false);
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
            printnl(S(292));
            print(S(558));
            print_esc(S(664));
            help1(S(559));
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
        freenode(p, ifnodesize);
    }
_Lcommonending:
    if (curchr == ficode) { /*496:*/
        p = condptr;
        ifline = iflinefield(p);
        curif = subtype(p);
        iflimit = type(p);
        condptr = link(p);
        freenode(p, ifnodesize);
    } /*:496*/
    else
        iflimit = ficode;
_Lexit:;

    /*:508*/
} // [#498] conditional

/** @} */ // end group S487x510
