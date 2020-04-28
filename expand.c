#include "tex.h"
#include "texmac.h" // [macro] link
#include "tex_inc.h" // [macro] STORE_NEW_TOKEN, FAST_STORE_NEW_TOKEN, FREE_AVAIL
#include "macros.h" // [macro] help1, help2, help3, help4,
#include "global.h"
#include "texfunc.h"
#include "lexer.h" // [var] warning_index,
#include "printout.h" // [func] printcmdchr
#include "expand.h"


/** @defgroup S366x401 PART 25: EXPANDING THE NEXT TOKEN
 * [ p144~155#366~401 ]
 *
 * + #macrocall
 * + #insertrelax
 * + #pass text
 * + #start input
 * + #conditional
 * + #get_x_token
 * + #conv toks
 * + #ins the toks
 * + #expand
 *
 * + #xtoken
 * @{
 */


/*396:*/
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
}
/*:396*/

/*366:*/
/*389:*/
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
}
/*:389*/

/*379:*/
void insertrelax(void) {
    curtok = CS_TOKEN_FLAG + curcs;
    backinput();
    curtok = CS_TOKEN_FLAG + frozenrelax;
    backinput();
    token_type = INSERTED;
} /*:379*/


/// [p143#366]
void expand(void) {
    HalfWord t;
    Pointer p, r, backupbackup;
    short j;
    long cvbackup;
    SmallNumber cvlbackup, radixbackup, cobackup, savescannerstatus;

    cvbackup = curval;
    cvlbackup = curvallevel;
    radixbackup = radix;
    cobackup = curorder;

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
    curval = cvbackup;
    curvallevel = cvlbackup;
    radix = radixbackup;
    curorder = cobackup;

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

/*381:*/
void xtoken(void) {
    while (curcmd > maxcommand) {
        expand();
        getnext();
    }
    curtok = pack_tok(curcs, curcmd, curchr);
}
/*:381*/

/* @} */ // end group S366x401
