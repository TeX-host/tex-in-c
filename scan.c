#include <stdlib.h> // [func] labs
#include "texmac.h" // [macro] link, 
#include "texfunc.h"
#include "lexer.h" // [var] curcmd, 
#include "tex.h"
#include "tex_inc.h"
#include "global.h"
#include "macros.h"
#include "expand.h"
#include "texmac.h" // [macro] ischarnode

#include "texmath.h"
#include "fonts.h"
#include "printout.h" // [func] printcmdchr
#include "scan.h"


/** @defgroup S402x463 PART 26: BASIC SCANNING SUBROUTINES
 * [ #402~463 ]
 *
 * + #scanleftbrace
 * + #scanoptionalequals
 * + muerror
 * @{
 */

/// [#406] Get the next non-blank non-call token.
/// used in sections 405, 441, 455, 503, 526, 577, 785, 791, and 1045.
void skip_spaces(void) {
    do {
        get_x_token();
    } while (curcmd == SPACER);
} // [#406]

/// [#404] Get the next non-blank non-relax non-call token.
/// TEX allows `\relax` to appear before the left brace.
/// used in sections 403, 1078, 1084, 1151, 1160, 1211, 1226, and 1270.
void skip_spaces_or_relax(void) {
    do {
        get_x_token();
    } while (curcmd == SPACER || curcmd == relax);
} // [#404]

/// [#403] when a left brace is supposed to be the next non-blank token.
void scanleftbrace(void) {
    skip_spaces_or_relax();
    if (curcmd == LEFT_BRACE) return;
    printnl(S(292));
    print(S(566));
    help4(S(567), S(568), S(569), S(570));
    backerror();
    curtok = leftbracetoken + '{';
    curcmd = LEFT_BRACE;
    curchr = '{';
    align_state++;
} // [#403] scanleftbrace

/// [#405] looks for an optional ‘=’ sign preceded by optional spaces.
/// `\relax` is not ignored here.
void scanoptionalequals(void) {
    skip_spaces();
    if (curtok != (othertoken + '=')) backinput();
} // [#405] scanoptionalequals

/// [#407] look for a given string.
/// checks to see whether the next tokens of input match this string.
///
/// If a match is found, 
///     the characters are effectively removed from the input 
///     and `true` is returned.
/// Otherwise false is returned, 
///     and the input is left essentially unchanged 
///     (except for the fact that some macros may have been expanded, etc.).
Boolean scankeyword(StrNumber s) {
    Boolean Result;
#if 1
    Pointer p = get_avail();
    Pointer my_backup_head = p;
#else
    Pointer my_backup_head = backuphead;
    Pointer p = backuphead;
#endif

#if 0
    PoolPtr k;
#else
    int k;
    int k_e;
#endif

    link(p) = 0;

#if 0
    k = str_start[s];
    while (k < str_end(s) ) {
        get_x_token();
        if ((curcs == 0) & ((curchr == str_pool[k]) |
                (curchr == str_pool[k] - 'a' + 'A'))) {
#else
    k = 0;
    k_e = str_length(s);
    while (k < k_e) {
        int str_c = str_getc(s, k);
        get_x_token();
        if ((curcs == 0) &
            ((curchr == str_c) | (curchr == str_c - 'a' + 'A'))) {
#endif
            STORE_NEW_TOKEN(p, curtok);
            k++;
            continue;
        } else {
            if (curcmd == SPACER && p == my_backup_head) continue;
            backinput();
            if (p != my_backup_head) {
                backlist(link(my_backup_head));
            }
            Result = false;
            goto _Lexit;
        }
    } // while (k < k_e)
    flushlist(link(my_backup_head));
    Result = true;

_Lexit :
    FREE_AVAIL(my_backup_head);
    return Result;
} // [#407] scankeyword

/// [#408] sounds an alarm when mu and non-mu units are being switched.
static void muerror(void) {
    printnl(S(292));
    print(S(571)); // "Incompatible␣glue␣units"
    help1(S(572)); // "I´m␣going␣to␣assume␣that␣1mu=1pt␣when␣they´re␣mixed."
    error();
} // [#408] muerror

/// [#413] fetch an internal parameter
void scansomethinginternal(SmallNumber level, Boolean negative) {
    HalfWord m;
    /* char */ int p; /* INT */

    m = curchr;
    switch (curcmd) {
        case defcode: /*414:*/
            scancharnum();
            if (m == mathcodebase) {
                curval = mathcode(curval);
                curvallevel = intval;
            } else if (m < mathcodebase) {
                curval = equiv(m + curval);
                curvallevel = intval;
            } else {
                curval = eqtb[m + curval - activebase].int_;
                curvallevel = intval;
            }
            break;
            /*:414*/

        case toksregister:
        case assigntoks:
        case deffamily:
        case setfont:
        case deffont: /*415:*/
            if (level != tokval) {
                printnl(S(292));
                print(S(593));
                help3(S(594), S(595), S(596));
                backerror();
                curval = 0;
                curvallevel = dimenval;
            } else if (curcmd <= assigntoks) {
                if (curcmd < assigntoks) {
                    scaneightbitint();
                    m = toksbase + curval;
                }
                curval = equiv(m);
                curvallevel = tokval;
            } else {
                backinput();
                scanfontident();
                curval += fontidbase;
                curvallevel = identval;
            }
            break;

        case assignint:
            curval = eqtb[m - activebase].int_;
            curvallevel = intval;
            break;

        case assigndimen:
            curval = eqtb[m - activebase].sc;
            curvallevel = dimenval;
            break;

        case assignglue:
            curval = equiv(m);
            curvallevel = glueval;
            break;

        case assignmuglue:
            curval = equiv(m);
            curvallevel = muval;
            break;

        case setaux: /*418:*/
            if (labs(mode) != m) {
                printnl(S(292));
                print(S(597));
                printcmdchr(setaux, m);
                help4(S(598), S(599), S(600), S(601));
                error();
                if (level != tokval) {
                    curval = 0;
                    curvallevel = dimenval;
                } else {
                    curval = 0;
                    curvallevel = intval;
                }
            } else if (m == V_MODE) {
                curval = prevdepth;
                curvallevel = dimenval;
            } else {
                curval = spacefactor;
                curvallevel = intval;
            }
            break;

        case setprevgraf: /*422:*/
            if (mode == 0) {
                curval = 0;
                curvallevel = intval;
            } else { /*:422*/
                nest[nest_ptr] = cur_list;
                p = nest_ptr;
                while (abs(nest[p].modefield) != V_MODE)
                    p--;
                curval = nest[p].pgfield;
                curvallevel = intval;
            }
            break;

        case setpageint: /*419:*/
            if (m == 0)
                curval = deadcycles;
            else
                curval = insertpenalties;
            curvallevel = intval;
            break;
            /*:419*/

        case setpagedimen: /*421:*/
            if (pagecontents == empty && !outputactive) {
                if (m == 0)
                    curval = maxdimen;
                else
                    curval = 0;
            } else
                curval = pagesofar[m];
            curvallevel = dimenval;
            break;
            /*:421*/

        case setshape: /*423:*/
            if (parshapeptr == 0)
                curval = 0;
            else
                curval = info(parshapeptr);
            curvallevel = intval;
            break;
            /*:423*/

        case setboxdimen: /*420:*/
            scaneightbitint();
            if (box(curval) == 0)
                curval = 0;
            else
                curval = mem[box(curval) + m - MEM_MIN].sc;
            curvallevel = dimenval;
            break;
            /*:420*/

        case chargiven:
        case mathgiven:
            curval = curchr;
            curvallevel = intval;
            break;

        case assignfontdimen: /*425:*/
            findfontdimen(false);
            fontinfo[fmemptr].sc = 0;
            curval = fontinfo[curval].sc;
            curvallevel = dimenval;
            break;
            /*:425*/

        case assignfontint: /*426:*/
            scanfontident();
            if (m == 0) {
                curval = get_hyphenchar(curval);
                curvallevel = intval;
            } else {
                curval = get_skewchar(curval);
                curvallevel = intval;
            }
            break;
            /*:426*/

        case register_: /*427:*/
            scaneightbitint();
            switch (m) {
                case intval: curval = count(curval); break;
                case dimenval: curval = dimen(curval); break;
                case glueval: curval = skip(curval); break;
                case muval: curval = muskip(curval); break;
            }
            curvallevel = m;
            break;
            /*:427*/

        case lastitem: /*424:*/
            if (curchr > glueval) {
                if (curchr == inputlinenocode)
                    curval = line;
                else
                    curval = lastbadness;
                curvallevel = intval;
            } else { /*:424*/
                if (curchr == glueval)
                    curval = zeroglue;
                else
                    curval = 0;
                curvallevel = curchr;
                if (!ischarnode(tail) && mode != 0) {
                    switch (curchr) {

                        case intval:
                            if (type(tail) == PENALTY_NODE)
                                curval = penalty(tail);
                            break;

                        case dimenval:
                            if (type(tail) == KERN_NODE) curval = width(tail);
                            break;

                        case glueval:
                            if (type(tail) == GLUE_NODE) {
                                curval = glueptr(tail);
                                if (subtype(tail) == muglue)
                                    curvallevel = muval;
                            }
                            break;
                    }
                } else if (mode == V_MODE && tail == head) {
                    switch (curchr) {

                        case intval: curval = lastpenalty; break;

                        case dimenval: curval = lastkern; break;

                        case glueval:
                            if (lastglue != MAX_HALF_WORD) curval = lastglue;
                            break;
                    }
                }
            }
            break;
            /*428:*/

        default:
            printnl(S(292));
            print(S(602));
            printcmdchr(curcmd, curchr);
            print(S(603));
            print_esc(S(604));
            help1(S(601));
            error();
            if (level != tokval) { /*:428*/
                curval = 0;
                curvallevel = dimenval;
            } else {
                curval = 0;
                curvallevel = intval;
            }
            break;
    }
    while (curvallevel > level) { /*429:*/
        if (curvallevel == glueval)
            curval = width(curval);
        else if (curvallevel == muval)
            muerror();
        curvallevel--;
    }
    /*:429*/
    /*430:*/
    if (!negative) {
        if (curvallevel >= glueval && curvallevel <= muval) {
            addglueref(curval); /*:430*/
        }
        return;
    }
    if (curvallevel < glueval) {
        curval = -curval;
        return;
    }
    curval = newspec(curval); /*431:*/
    width(curval) = -width(curval);
    stretch(curval) = -stretch(curval);
    shrink(curval) = -shrink(curval); /*:431*/

    /*:415*/
    /*:418*/
} // [#413] scansomethinginternal

/// [#433]
void scaneightbitint(void) {
    scanint();
    if ((unsigned long)curval <= 255) return;
    printnl(S(292));
    print(S(573));
    help2(S(574), S(575));
    int_error(curval);
    curval = 0;
} // [#433] scaneightbitint

/// [#434]
void scancharnum(void) {
    scanint();
    if ((unsigned long)curval <= 255) return;
    printnl(S(292));
    print(S(576));
    help2(S(577), S(575));
    int_error(curval);
    curval = 0;
} // [#434] scancharnum

/// [#435]
void scanfourbitint(void) {
    scanint();
    if ((unsigned long)curval <= 15) return;
    printnl(S(292));
    print(S(578));
    help2(S(579), S(575));
    int_error(curval);
    curval = 0;
} // [#435] scanfourbitint

/// [#436]
void scanfifteenbitint(void) {
    scanint();
    if ((unsigned long)curval <= 32767) return;
    printnl(S(292));
    print(S(580));
    help2(S(581), S(575));
    int_error(curval);
    curval = 0;
} // [#436] scanfifteenbitint

/// [#437]
void scantwentysevenbitint(void) {
    scanint();
    if ((unsigned long)curval <= 134217727L) return;
    printnl(S(292));
    print(S(582));
    help2(S(583), S(575));
    int_error(curval);
    curval = 0;
} // [#437] scantwentysevenbitint

/// [#440] sets #curval to an integer.
void scanint(void) {
    Boolean negative;
    long m;
    SmallNumber d;
    Boolean vacuous, OKsofar;

    radix = 0;
    OKsofar = true; /*441:*/
    negative = false;
    do {
        skip_spaces();
        if (curtok == othertoken + '-') { /*:441*/
            negative = !negative;
            curtok = othertoken + '+';
        }
    } while (curtok == othertoken + '+');
    if (curtok == alphatoken) { /*442:*/
        gettoken();
        if (curtok < CS_TOKEN_FLAG) {
            curval = curchr;
            if (curcmd <= RIGHT_BRACE) {
                if (curcmd == RIGHT_BRACE)
                    align_state++;
                else
                    align_state--;
            }
        } else if (curtok < CS_TOKEN_FLAG + singlebase)
            curval = curtok - CS_TOKEN_FLAG - activebase;
        else
            curval = curtok - CS_TOKEN_FLAG - singlebase;
        if (curval > 255) {
            printnl(S(292));
            print(S(605));
            help2(S(606), S(607));
            curval = '0';
            backerror();
        } else { /*443:*/
            get_x_token();
            if (curcmd != SPACER) backinput();
        }
    } /*:442*/
    else if (curcmd >= mininternal && curcmd <= maxinternal)
        scansomethinginternal(intval, false);
    else {
        radix = 10;
        m = 214748364L;
        if (curtok == octaltoken) {
            radix = 8;
            m = 268435456L;
            get_x_token();
        } else if (curtok == hextoken) {
            radix = 16;
            m = 134217728L;
            get_x_token();
        }
        vacuous = true;
        curval = 0; /*445:*/
        while (true) {
            if (curtok < zerotoken + radix && curtok >= zerotoken &&
                curtok <= zerotoken + 9)
                d = curtok - zerotoken;
            else if (radix == 16) {
                if (curtok <= Atoken + 5 && curtok >= Atoken)
                    d = curtok - Atoken + 10;
                else if (curtok <= otherAtoken + 5 && curtok >= otherAtoken)
                    d = curtok - otherAtoken + 10;
                else
                    goto _Ldone;
            } else
                goto _Ldone;
            vacuous = false;
            if (curval >= m && (curval > m || d > 7 || radix != 10)) {
                if (OKsofar) {
                    printnl(S(292));
                    print(S(608));
                    help2(S(609), S(610));
                    error();
                    curval = infinity;
                    OKsofar = false;
                }
            } else
                curval = curval * radix + d;
            get_x_token();
        }
    _Ldone:            /*:445*/
        if (vacuous) { /*446:*/
            printnl(S(292));
            print(S(593));
            help3(S(594), S(595), S(596));
            backerror();
        } /*:446*/
        else if (curcmd != SPACER)
            backinput();
    }
    if (negative) curval = -curval;
    /*:443*/

} // [#440] scanint

/// [#448] sets #curval to a dimension.
void scandimen(Boolean mu, Boolean inf, Boolean shortcut) {
    Boolean negative;
    long f;
    /*450:*/
    long num, denom;
    /* SmallNumber */ int k, kk; /* INT */
    Pointer p, q;
    Scaled v;
    long savecurval; /*:450*/
    char digs[23];

    f = 0;
    arith_error = false;
    curorder = NORMAL;
    negative = false;
    if (!shortcut) { /*441:*/
        negative = false;
        do {
            skip_spaces();
            if (curtok == othertoken + '-') { /*:441*/
                negative = !negative;
                curtok = othertoken + '+';
            }
        } while (curtok == othertoken + '+');
        if (curcmd >= mininternal && curcmd <= maxinternal) { /*449:*/
            if (mu) {
                scansomethinginternal(muval, false); /*451:*/
                if (curvallevel >= glueval) {        /*:451*/
                    v = width(curval);
                    delete_glue_ref(curval);
                    curval = v;
                }
                if (curvallevel == muval) goto _Lattachsign_;
                if (curvallevel != intval) muerror();
            } else {
                scansomethinginternal(dimenval, false);
                if (curvallevel == dimenval) goto _Lattachsign_;
            } /*:449*/
        } else {
            backinput();
            if (curtok == continentalpointtoken) curtok = pointtoken;
            if (curtok != pointtoken)
                scanint();
            else {
                radix = 10;
                curval = 0;
            }
            if (curtok == continentalpointtoken) curtok = pointtoken;
            if (radix == 10 && curtok == pointtoken) { /*452:*/
                k = 0;
                p = 0;
                gettoken();
                while (true) {
                    get_x_token();
                    if (curtok > zerotoken + 9 || curtok < zerotoken)
                        goto _Ldone1;
                    if (k >= 17) continue;
                    q = get_avail();
                    link(q) = p;
                    info(q) = curtok - zerotoken;
                    p = q;
                    k++;
                }
            _Ldone1:
                for (kk = k - 1; kk >= 0; kk--) {
                    digs[kk] = info(p);
                    q = p;
                    p = link(p);
                    FREE_AVAIL(q);
                }
                f = round_decimals(k, digs);
                if (curcmd != SPACER) backinput();
            }
            /*:452*/
        }
    }
    if (curval < 0) {
        negative = !negative;
        curval = -curval;
    }                              /*453:*/
    if (inf) {                     /*454:*/
        if (scankeyword(S(330))) { /*:454*/
            curorder = FIL;
            while (scankeyword('l')) {
                if (curorder != FILLL) {
                    curorder++;
                    continue;
                }
                printnl(S(292));
                print(S(611));
                print(S(612));
                help1(S(613));
                error();
            }
            goto _Lattachfraction_;
        }
    }
    /*455:*/
    savecurval = curval;
    skip_spaces();
    if (curcmd >= mininternal && curcmd <= maxinternal) {
        if (mu) {
            scansomethinginternal(muval, false); /*451:*/
            if (curvallevel >= glueval) {        /*:451*/
                v = width(curval);
                delete_glue_ref(curval);
                curval = v;
            }
            if (curvallevel != muval) muerror();
        } else
            scansomethinginternal(dimenval, false);
        v = curval;
        goto _Lfound;
    }
    backinput();
    if (mu) goto _Lnotfound;
    if (scankeyword(S(614))) /*443:*/
        v = quad(curfont);   /*558:*/
                             /*:558*/
    else if (scankeyword(S(615)))
        v = xheight(curfont); /*559:*/
                              /*:559*/
    else
        goto _Lnotfound;
    get_x_token();
    if (curcmd != SPACER) /*:443*/
        backinput();
_Lfound:
    curval = nx_plus_y(savecurval, v, xn_over_d(v, f, 65536L));
    goto _Lattachsign_;
_Lnotfound:   /*:455*/
    if (mu) { /*456:*/
        if (scankeyword(S(390)))
            goto _Lattachfraction_;
        else { /*:456*/
            printnl(S(292));
            print(S(611));
            print(S(616));
            help4(S(617), S(618), S(619), S(620));
            error();
            goto _Lattachfraction_;
        }
    }
    if (scankeyword(S(621))) { /*457:*/
        preparemag();
        if (mag != 1000) {
            curval = xn_over_d(curval, 1000, mag);
            f = (f * 1000 + tex_remainder * 65536L) / mag;
            curval += f / 65536L;
            f %= 65536L;
        }
    }
    /*:457*/
    if (scankeyword(S(459))) /*458:*/
        goto _Lattachfraction_;
    if (scankeyword(S(622))) {
        num = 7227;
        denom = 100;
    } else if (scankeyword(S(623))) {
        num = 12;
        denom = 1;
    } else if (scankeyword(S(624))) {
        num = 7227;
        denom = 254;
    } else if (scankeyword(S(625))) {
        num = 7227;
        denom = 2540;
    } else if (scankeyword(S(626))) {
        num = 7227;
        denom = 7200;
    } else if (scankeyword(S(627))) {
        num = 1238;
        denom = 1157;
    } else if (scankeyword(S(628))) {
        num = 14856;
        denom = 1157;
    } else if (scankeyword(S(629)))
        goto _Ldone;
    else {
        printnl(S(292));
        print(S(611));
        print(S(630));
        help6(S(631), S(632), S(633), S(618), S(619), S(620));
        error();
        goto _Ldone2;
    }
    curval = xn_over_d(curval, num, denom);
    f = (num * f + tex_remainder * 65536L) / denom;
    curval += f / 65536L;
    f %= 65536L;
_Ldone2: /*:458*/
_Lattachfraction_:
    if (curval >= 16384)
        arith_error = true;
    else
        curval = curval * UNITY + f;
_Ldone: /*:453*/
    /*443:*/
    get_x_token();
    if (curcmd != SPACER) /*:443*/
        backinput();
_Lattachsign_:
    if (arith_error || labs(curval) >= 1073741824L) { /*460:*/
        printnl(S(292));
        print(S(634));
        help2(S(635), S(636));
        error();
        curval = maxdimen;
        arith_error = false;
    }
    /*:460*/
    if (negative) curval = -curval;

    /*459:*/
    /*:459*/
} // [#448] scandimen

/// [#461] sets #curval to a glue spec pointer
void scanglue(SmallNumber level) {
    Boolean negative, mu;
    Pointer q;

    mu = (level == muval); /*441:*/
    negative = false;
    do {
        skip_spaces();
        if (curtok == othertoken + '-') { /*:441*/
            negative = !negative;
            curtok = othertoken + '+';
        }
    } while (curtok == othertoken + '+');
    if (curcmd >= mininternal && curcmd <= maxinternal) { /*462:*/
        scansomethinginternal(level, negative);
        if (curvallevel >= glueval) {
            if (curvallevel != level) muerror();
            goto _Lexit;
        }
        if (curvallevel == intval)
            scandimen(mu, false, true);
        else if (level == muval)
            muerror();
    } else {
        backinput();
        scandimen(mu, false, false);
        if (negative) curval = -curval;
    }
    q = newspec(zeroglue);
    width(q) = curval;
    if (scankeyword(S(637))) {
        scandimen(mu, true, false);
        stretch(q) = curval;
        stretchorder(q) = curorder;
    }
    if (scankeyword(S(638))) {
        scandimen(mu, true, false);
        shrink(q) = curval;
        shrinkorder(q) = curorder;
    }
    curval = q; /*:462*/
_Lexit:;
} // [#461] scanglue

/// [#463]
HalfWord scanrulespec(void) {
    Pointer q;

    q = newrule();
    if (curcmd == vrule)
        width(q) = defaultrule;
    else {
        height(q) = defaultrule;
        depth(q) = 0;
    }
_LN_scanrulespec__reswitch:
    if (scankeyword(S(639))) {
        scannormaldimen();
        width(q) = curval;
        goto _LN_scanrulespec__reswitch;
    }
    if (scankeyword(S(640))) {
        scannormaldimen();
        height(q) = curval;
        goto _LN_scanrulespec__reswitch;
    }
    if (!scankeyword(S(641))) return q;
    scannormaldimen();
    depth(q) = curval;
    goto _LN_scanrulespec__reswitch;
} // [#463] scanrulespec

/** @}*/ // end group S402x463
