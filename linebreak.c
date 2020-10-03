#include <stdlib.h> // labs
#include "tex_constant.h" // [const] MEM_MIN, MIN_QUARTER_WORD, MAX_HALF_WORD
#include "mem.h"
#include "print.h"
#include "eqtb.h"
#include "pack.h"
#include "error.h"
#include "texmath.h" // [macro] INF_BAD; [func] badness
#include "hyphen.h"
#include "extension.h" // [macor] advpast
#include "linebreak.h"


Static void postlinebreak(long finalwidowpenalty);
/** @addtogroup S813x861_P302x318
 * [ #813. Breaking paragraphs into lines. ]
 * @{
 */

/*814:*/
Pointer justbox;
/*:814*/
/*821:*/
Pointer passive, printednode;
HalfWord passnumber;
/*:821*/
/*823:*/
Scaled activewidth[6];
Scaled curactivewidth[6];
Scaled background[6];
Scaled breakwidth[6];
/*:823*/
/*825:*/
Boolean noshrinkerroryet, secondpass, finalpass;
/*:825*/
/*828:*/
Pointer curp;
Integer threshold;
/*:828*/
/*833:*/
Integer minimaldemerits[tightfit - veryloosefit + 1];
Integer minimumdemerits;
Pointer bestplace[tightfit - veryloosefit + 1];
HalfWord bestplline[tightfit - veryloosefit + 1];
/*:833*/
/*839:*/
Scaled discwidth, firstwidth, secondwidth, firstindent, secondindent;
/*:839*/
/*847:*/
HalfWord easyline, lastspecialline;
/*:847*/

/*815:*/
void linebreak(long finalwidowpenalty) {
    /*862:*/
    Boolean autobreaking;
    Pointer prevp, q, r, s, prevs;
    InternalFontNumber f;
    /*:862*/
    /*893:*/
    /* SmallNumber */ int j; /* INT */
    UChar c;                 /*:893*/

    packbeginline = modeline; /*816:*/
    link(temphead) = link(head);
    if (ischarnode(tail)) {
        tailappend(newpenalty(INF_PENALTY));
    } else if (type(tail) != GLUE_NODE) {
        tailappend(newpenalty(INF_PENALTY));
    } else {
        type(tail) = PENALTY_NODE;
        delete_glue_ref(glueptr(tail));
        flush_node_list(leaderptr(tail));
        penalty(tail) = INF_PENALTY;
    }
    link(tail) = newparamglue(PAR_FILL_SKIP_CODE);
    initcurlang = prevgraf % 65536L;
    initlhyf = prevgraf / 4194304L;
    initrhyf = (prevgraf / 65536L) & 63;
    popnest(); /*:816*/
    /*827:*/
    noshrinkerroryet = true;
    checkshrinkage(leftskip);
    checkshrinkage(rightskip);
    q = leftskip;
    r = rightskip;
    background[0] = width(q) + width(r);
    background[1] = 0;
    background[2] = 0;
    background[3] = 0;
    background[4] = 0;
    background[stretchorder(q) + 1] = stretch(q);
    background[stretchorder(r) + 1] += stretch(r);
    background[5] = shrink(q) + shrink(r); /*:827*/
    /*834:*/
    minimumdemerits = awfulbad;
    minimaldemerits[tightfit - veryloosefit] = awfulbad;
    minimaldemerits[decentfit - veryloosefit] = awfulbad;
    minimaldemerits[loosefit - veryloosefit] = awfulbad;
    minimaldemerits[0] = awfulbad; /*:834*/
    /*848:*/
    if (parshapeptr == 0) {
        if (hangindent == 0) {
            lastspecialline = 0;
            secondwidth = hsize;
            secondindent = 0;
        } else /*849:*/
        {      /*:849*/
            lastspecialline = labs(hangafter);
            if (hangafter < 0) {
                firstwidth = hsize - labs(hangindent);
                if (hangindent >= 0)
                    firstindent = hangindent;
                else
                    firstindent = 0;
                secondwidth = hsize;
                secondindent = 0;
            } else {
                firstwidth = hsize;
                firstindent = 0;
                secondwidth = hsize - labs(hangindent);
                if (hangindent >= 0)
                    secondindent = hangindent;
                else
                    secondindent = 0;
            }
        }
    } else {
        lastspecialline = info(parshapeptr) - 1;
        secondwidth = mem[parshapeptr + (lastspecialline + 1) * 2 - MEM_MIN].sc;
        secondindent = mem[parshapeptr + lastspecialline * 2 - MEM_MIN + 1].sc;
    }
    if (looseness == 0)
        easyline = lastspecialline;
    else {
        easyline = MAX_HALF_WORD;
        /*:848*/
    }
    /*863:*/
    threshold = pretolerance;
    if (threshold >= 0) {           
        #ifdef tt_STAT
            if (tracingparagraphs > 0) {
                begindiagnostic();
                printnl(S(777));
            }
        #endif // #863.1: tt_STAT
        secondpass = false;
        finalpass = false;
    } else {
        threshold = tolerance;
        secondpass = true;
        finalpass = (emergencystretch <= 0);
        #ifdef tt_STAT
            if (tracingparagraphs > 0) begindiagnostic();
        #endif // #863.2: tt_STAT
    }
    while (true) {
        if (threshold > INF_BAD) threshold = INF_BAD;
        if (secondpass) { /*891:*/
#ifdef tt_INIT
            if (trie_not_ready) inittrie();
#endif // #891: tt_INIT
            curlang = initcurlang;
            lhyf = initlhyf;
            rhyf = initrhyf;
        }
        /*:891*/
        /*864:*/
        q = get_node(activenodesize);
        type(q) = unhyphenated;
        fitness(q) = decentfit;
        link(q) = lastactive;
        breaknode(q) = 0;
        linenumber(q) = prevgraf + 1;
        totaldemerits(q) = 0;
        link(active) = q;
        storebackground(1);
        storebackground(2);
        storebackground(3);
        storebackground(4);
        storebackground(5);
        storebackground(6);
        passive = 0;
        printednode = temphead;
        passnumber = 0;
        font_in_short_display = NULL_FONT; /*:864*/
        curp = link(temphead);
        autobreaking = true;
        prevp = curp;
        while ((curp != 0) & (link(active) != lastactive)) { /*866:*/
            if (ischarnode(curp)) {                          /*867:*/
                prevp = curp;
                do {
                    f = font(curp);
                    actwidth += charwidth(f, charinfo(f, character(curp)));
                    curp = link(curp);
                } while (ischarnode(curp));
            }
            /*:867*/
            switch (type(curp)) {

                case HLIST_NODE:
                case VLIST_NODE:
                case RULE_NODE:
                    actwidth += width(curp);
                    break;

                case WHATSIT_NODE:  /*1362:*/
                    advpast(curp); /*:1362*/
                    break;

                case GLUE_NODE: /*868:*/
                    if (autobreaking) {
                        if (ischarnode(prevp))
                            trybreak(0, unhyphenated);
                        else if (precedesbreak(prevp)) {
                            trybreak(0, unhyphenated);
                        } else if ((type(prevp) == KERN_NODE) &
                                   (subtype(prevp) != explicit))
                            trybreak(0, unhyphenated);
                    }
                    checkshrinkage(glueptr(curp));
                    q = glueptr(curp);
                    actwidth += width(q);
                    activewidth[stretchorder(q) + 1] += stretch(q);
                    activewidth[5] += shrink(q);      /*:868*/
                    if (secondpass && autobreaking) { /*894:*/
                        prevs = curp;
                        s = link(prevs);
                        if (s != 0) { /*896:*/
                            while (true) {
                                if (ischarnode(s)) {
                                    c = character(s) - MIN_QUARTER_WORD;
                                    hf = font(s);
                                } else if (type(s) == LIGATURE_NODE) {
                                    if (ligptr(s) == 0) goto _Llabcontinue;
                                    q = ligptr(s);
                                    c = character(q) - MIN_QUARTER_WORD;
                                    hf = font(q);
                                } else if ((type(s) == KERN_NODE) &
                                           (subtype(s) == NORMAL))
                                    goto _Llabcontinue;
                                else if (type(s) == WHATSIT_NODE) {
                                    advpast(s); /*:1363*/
                                    goto _Llabcontinue;
                                } else
                                    goto _Ldone1;
                                if (lc_code(c) != 0) {
                                    if (lc_code(c) == c || uchyph > 0)
                                        goto _Ldone2;
                                    else
                                        goto _Ldone1;
                                }
                            _Llabcontinue:
                                prevs = s;
                                s = link(prevs);
                            }
                        _Ldone2:
                            hyfchar = get_hyphenchar(hf);
                            if (hyfchar < 0) goto _Ldone1;
                            if (hyfchar > 255) goto _Ldone1;
                            ha = prevs;           /*:896*/
                            if (lhyf + rhyf > 63) /*897:*/
                                goto _Ldone1;
                            hn = 0;
                            while (true) {
                                if (ischarnode(s)) {
                                    if (font(s) != hf) goto _Ldone3;
                                    hyfbchar = character(s);
                                    c = hyfbchar - MIN_QUARTER_WORD;
                                    if (lc_code(c) == 0) goto _Ldone3;
                                    if (hn == 63) goto _Ldone3;
                                    hb = s;
                                    hn++;
                                    hu[hn] = c;
                                    hc[hn] = lc_code(c);
                                    hyfbchar = NON_CHAR;
                                } else if (type(s) == LIGATURE_NODE) {
                                    if (font_ligchar(s) != hf) {
                                        goto _Ldone3;
                                    }
                                    j = hn;
                                    q = ligptr(s);
                                    if (q > 0) hyfbchar = character(q);
                                    while (q > 0) {
                                        c = character(q) - MIN_QUARTER_WORD;
                                        if (lc_code(c) == 0) goto _Ldone3;
                                        if (j == 63) goto _Ldone3;
                                        j++;
                                        hu[j] = c;
                                        hc[j] = lc_code(c);
                                        q = link(q);
                                    }
                                    hb = s;
                                    hn = j;
                                    if ((subtype(s)) & 1)
                                        hyfbchar = fontbchar[hf];
                                    else
                                        hyfbchar = NON_CHAR;
                                } else if ((type(s) == KERN_NODE) &
                                           (subtype(s) == NORMAL)) {
                                    hb = s;
                                    hyfbchar = fontbchar[hf];
                                } else
                                    goto _Ldone3;
                                s = link(s);
                            }
                        _Ldone3: /*:897*/
                            /*899:*/
                            if (hn < lhyf + rhyf) goto _Ldone1;
                            while (true) {
                                if (!ischarnode(s)) {
                                    switch (type(s)) {

                                        case LIGATURE_NODE:
                                            /* blank case */
                                            break;

                                        case KERN_NODE:
                                            if (subtype(s) != NORMAL)
                                                goto _Ldone4;
                                            break;

                                        case WHATSIT_NODE:
                                        case GLUE_NODE:
                                        case PENALTY_NODE:
                                        case INS_NODE:
                                        case ADJUST_NODE:
                                        case MARK_NODE:
                                            goto _Ldone4;
                                            break;

                                        default:
                                            goto _Ldone1;
                                            break;
                                    }
                                }
                                s = link(s);
                            }
                        _Ldone4: /*:899*/
                            hyphenate();
                        }
                    _Ldone1:;
                    }
                    /*:894*/
                    break;

                case KERN_NODE:
                    if (subtype(curp) == explicit) {
                        kernbreak();
                    } else
                        actwidth += width(curp);
                    break;

                case LIGATURE_NODE:
                    f = font_ligchar(curp);
                    actwidth +=
                        charwidth(f, charinfo(f, character_ligchar(curp)));
                    break;

                case DISC_NODE: /*869:*/
                    s = prebreak(curp);
                    discwidth = 0;
                    if (s == 0)
                        trybreak(exhyphenpenalty, hyphenated);
                    else {
                        do { /*870:*/
                            if (ischarnode(s)) {
                                f = font(s);
                                discwidth +=
                                    charwidth(f, charinfo(f, character(s)));
                            } else {
                                switch (type(s)) { /*:870*/

                                    case LIGATURE_NODE:
                                        f = font_ligchar(s);
                                        discwidth += charwidth(
                                            f,
                                            charinfo(f, character_ligchar(s)));
                                        break;

                                    case HLIST_NODE:
                                    case VLIST_NODE:
                                    case RULE_NODE:
                                    case KERN_NODE:
                                        discwidth += width(s);
                                        break;

                                    default:
                                        confusion(S(778)); // "disc3"
                                        break;
                                }
                            }
                            s = link(s);
                        } while (s != 0);
                        actwidth += discwidth;
                        trybreak(hyphenpenalty, hyphenated);
                        actwidth -= discwidth;
                    }
                    r = replacecount(curp);
                    s = link(curp);
                    while (r > 0) { /*871:*/
                        if (ischarnode(s)) {
                            f = font(s);
                            actwidth += charwidth(f, charinfo(f, character(s)));
                        } else {
                            switch (type(s)) { /*:871*/

                                case LIGATURE_NODE:
                                    f = font_ligchar(s);
                                    actwidth += charwidth(
                                        f, charinfo(f, character_ligchar(s)));
                                    break;

                                case HLIST_NODE:
                                case VLIST_NODE:
                                case RULE_NODE:
                                case KERN_NODE:
                                    actwidth += width(s);
                                    break;

                                default:
                                    confusion(S(779)); // "disc4"
                                    break;
                            }
                        }
                        r--;
                        s = link(s);
                    }
                    prevp = curp;
                    curp = s;
                    goto _Ldone5;
                    break;
                    /*:869*/

                case MATH_NODE:
                    autobreaking = (subtype(curp) == after);
                    kernbreak();
                    break;

                case PENALTY_NODE:
                    trybreak(penalty(curp), unhyphenated);
                    break;

                case MARK_NODE:
                case INS_NODE:
                case ADJUST_NODE:
                    /* blank case */
                    break;

                default:
                    confusion(S(780)); // "paragraph"
                    break;
            }
            prevp = curp;
            curp = link(curp);
        _Ldone5:;
        }
        /*:866*/
        if (curp == 0) { /*873:*/
            trybreak(EJECT_PENALTY, hyphenated);
            if (link(active) != lastactive) { /*874:*/
                r = link(active);
                fewestdemerits = awfulbad;
                do {
                    if (type(r) != deltanode) {
                        if (totaldemerits(r) < fewestdemerits) {
                            fewestdemerits = totaldemerits(r);
                            bestbet = r;
                        }
                    }
                    r = link(r);
                } while (r != lastactive);
                bestline = linenumber(bestbet); /*:874*/
                if (looseness == 0) goto _Ldone;
                /*875:*/
                r = link(active);
                actuallooseness = 0;
                do {
                    if (type(r) != deltanode) {
                        linediff = linenumber(r) - bestline;
                        if ((linediff < actuallooseness &&
                             looseness <= linediff) ||
                            (linediff > actuallooseness &&
                             looseness >= linediff)) {
                            bestbet = r;
                            actuallooseness = linediff;
                            fewestdemerits = totaldemerits(r);
                        } else if ((linediff == actuallooseness) &
                                   (totaldemerits(r) < fewestdemerits)) {
                            bestbet = r;
                            fewestdemerits = totaldemerits(r);
                        }
                    }
                    r = link(r);
                } while (r != lastactive);
                bestline = linenumber(bestbet); /*:875*/
                if (actuallooseness == looseness || finalpass) goto _Ldone;
            }
        }
        /*:873*/
        /*865:*/
        q = link(active);
        while (q != lastactive) {
            curp = link(q);
            if (type(q) == deltanode)
                free_node(q, deltanodesize);
            else
                free_node(q, activenodesize);
            q = curp;
        }
        q = passive;
        while (q != 0) { /*:865*/
            curp = link(q);
            free_node(q, passivenodesize);
            q = curp;
        }
        if (secondpass) {
            #ifdef tt_STAT
                if (tracingparagraphs > 0) printnl(S(781));
            #endif // #863.3: tt_STAT
            background[1] += emergencystretch;
            finalpass = true;
            continue;
        }
        #ifdef tt_STAT
            if (tracingparagraphs > 0) printnl(S(782));
        #endif // #863.4: tt_STAT

        threshold = tolerance;
        secondpass = true;
        finalpass = (emergencystretch <= 0);
    }
_Ldone:
    #ifdef tt_STAT
        if (tracingparagraphs > 0) {
            enddiagnostic(true);
            normalize_selector();
        }
    #endif // #863.5: tt_STAT
    /*:863*/
    /*876:*/
    /*:876*/
    postlinebreak(finalwidowpenalty); /*865:*/
    q = link(active);
    while (q != lastactive) {
        curp = link(q);
        if (type(q) == deltanode)
            free_node(q, deltanodesize);
        else
            free_node(q, activenodesize);
        q = curp;
    }
    q = passive;
    while (q != 0) { /*:865*/
        curp = link(q);
        free_node(q, passivenodesize);
        q = curp;
    }
    packbeginline = 0;

    /*1363:*/
    /*898:*/
    /*:898*/
}
/*:815*/

/*826:*/
HalfWord finiteshrink(HalfWord p) {
    Pointer q;

    if (noshrinkerroryet) {
        noshrinkerroryet = false;
        print_err(S(748)); // "Infinite glue shrinkage found in a paragraph"
        /*
         * (749) "The paragraph just ended includes some glue that has"
         * (750) "infinite shrinkability e.g. `\\hskip 0pt minus 1fil'."
         * (751) "Such glue doesn't belong there---it allows a paragraph"
         * (752) "of any length to fit on one line. But it's safe to proceed"
         * (753) "since the offensive shrinkability has been made finite."
         */
        help5(S(749), S(750), S(751), S(752), S(753));
        error();
    }
    q = newspec(p);
    shrinkorder(q) = NORMAL;
    delete_glue_ref(p);
    return q;
} /*:826*/


/// p308#829
void trybreak(long pi, SmallNumber breaktype) { /*831:*/
    Pointer r, prevr;
    HalfWord oldl;
    Boolean nobreakyet;
    /*830:*/
    Pointer prevprevr = 0 /* XXXXX */, s, q, v, savelink;
    long t;
    InternalFontNumber f;
    HalfWord l;
    Boolean noderstaysactive;
    Scaled linewidth = 0 /* XXXX */, shortfall; /*:830*/
    char fitclass;
    HalfWord b;
    long d;
    Boolean artificialdemerits;

    if (labs(pi) >= INF_PENALTY) {
        if (pi > 0) goto _Lexit;
        pi = EJECT_PENALTY;
        /*:831*/
    }

    nobreakyet = true;
    prevr = active;
    oldl = 0;
    copytocuractive(1);
    copytocuractive(2);
    copytocuractive(3);
    copytocuractive(4);
    copytocuractive(5);
    copytocuractive(6);

    while (true) {
    _Llabcontinue:
        r = link(prevr);            /*832:*/
        if (type(r) == deltanode) { /*:832*/
            updatewidth(1);
            updatewidth(2);
            updatewidth(3);
            updatewidth(4);
            updatewidth(5);
            updatewidth(6);
            prevprevr = prevr;
            prevr = r;
            goto _Llabcontinue;
        }
        /*835:*/
        l = linenumber(r);
        if (l > oldl) { /*:835*/
            if (minimumdemerits < awfulbad &&
                (oldl != easyline || r == lastactive) ) { /*836:*/
                if (nobreakyet) {                        /*837:*/
                    nobreakyet = false;
                    setbreakwidthtobackground(1);
                    setbreakwidthtobackground(2);
                    setbreakwidthtobackground(3);
                    setbreakwidthtobackground(4);
                    setbreakwidthtobackground(5);
                    setbreakwidthtobackground(6);
                    s = curp;
                    if (breaktype > unhyphenated) {
                        if (curp != 0) { /*840:*/
                            t = replacecount(curp);
                            v = curp;
                            s = postbreak(curp);
                            while (t > 0) {
                                t--;
                                v = link(v); /*841:*/
                                if (ischarnode(v)) {
                                    f = font(v);
                                    breakwidth[0] -=
                                        charwidth(f, charinfo(f, character(v)));
                                    continue;
                                }
                                switch (type(v)) { /*:841*/

                                    case LIGATURE_NODE:
                                        f = font_ligchar(v);
                                        breakwidth[0] -= charwidth(
                                            f,
                                            charinfo(f, character_ligchar(v)));
                                        break;

                                    case HLIST_NODE:
                                    case VLIST_NODE:
                                    case RULE_NODE:
                                    case KERN_NODE:
                                        breakwidth[0] -= width(v);
                                        break;

                                    default:
                                        confusion(S(754)); // "disc1"
                                        break;
                                }
                            }
                            while (s != 0) { /*842:*/
                                if (ischarnode(s)) {
                                    f = font(s);
                                    breakwidth[0] +=
                                        charwidth(f, charinfo(f, character(s)));
                                } else {
                                    switch (type(s)) { /*:842*/

                                        case LIGATURE_NODE:
                                            f = font_ligchar(s);
                                            breakwidth[0] += charwidth(
                                                f,
                                                charinfo(f,
                                                         character_ligchar(s)));
                                            break;

                                        case HLIST_NODE:
                                        case VLIST_NODE:
                                        case RULE_NODE:
                                        case KERN_NODE:
                                            breakwidth[0] += width(s);
                                            break;

                                        default:
                                            confusion(S(755)); // "disc2"
                                            break;
                                    }
                                }
                                s = link(s);
                            }
                            breakwidth[0] += discwidth;
                            if (postbreak(curp) == 0) s = link(v);
                        }
                        /*:840*/
                    }
                    while (s != 0) {
                        if (ischarnode(s)) goto _Ldone;
                        switch (type(s)) {

                            case GLUE_NODE: /*838:*/
                                v = glueptr(s);
                                breakwidth[0] -= width(v);
                                breakwidth[stretchorder(v) + 1] -= stretch(v);
                                breakwidth[5] -= shrink(v);
                                break;
                                /*:838*/

                            case PENALTY_NODE:
                                /* blank case */
                                break;

                            case MATH_NODE:
                                breakwidth[0] -= width(s);
                                break;

                            case KERN_NODE:
                                if (subtype(s) != explicit) goto _Ldone;
                                breakwidth[0] -= width(s);
                                break;

                            default:
                                goto _Ldone;
                                break;
                        }
                        s = link(s);
                    }
                _Ldone:;
                }
                /*:837*/
                /*843:*/
                if (type(prevr) == deltanode) {
                    converttobreakwidth(1);
                    converttobreakwidth(2);
                    converttobreakwidth(3);
                    converttobreakwidth(4);
                    converttobreakwidth(5);
                    converttobreakwidth(6);
                } else if (prevr == active) {
                    storebreakwidth(1);
                    storebreakwidth(2);
                    storebreakwidth(3);
                    storebreakwidth(4);
                    storebreakwidth(5);
                    storebreakwidth(6);
                } else {
                    q = get_node(deltanodesize);
                    link(q) = r;
                    type(q) = deltanode;
                    subtype(q) = 0;
                    newdeltatobreakwidth(1);
                    newdeltatobreakwidth(2);
                    newdeltatobreakwidth(3);
                    newdeltatobreakwidth(4);
                    newdeltatobreakwidth(5);
                    newdeltatobreakwidth(6);
                    link(prevr) = q;
                    prevprevr = prevr;
                    prevr = q;
                }
                if (labs(adjdemerits) >= awfulbad - minimumdemerits)
                    minimumdemerits = awfulbad - 1;
                else
                    minimumdemerits += labs(adjdemerits);
                for (fitclass = veryloosefit; fitclass <= tightfit;
                     fitclass++) {
                    if (minimaldemerits[fitclass - veryloosefit] <=
                        minimumdemerits) {
                        /// p313#845: Insert a new active node from best
                        /// place[fit class] to cur p
                        q = get_node(passivenodesize);
                        link(q) = passive;
                        passive = q;
                        curbreak(q) = curp;
                        #ifdef tt_STAT
                            passnumber++;
                            serial(q) = passnumber;
                        #endif // #845.1: tt_STAT
                        prevbreak(q) = bestplace[fitclass - veryloosefit];
                        q = get_node(activenodesize);
                        breaknode(q) = passive;
                        linenumber(q) = bestplline[fitclass - veryloosefit] + 1;
                        fitness(q) = fitclass;
                        type(q) = breaktype;
                        totaldemerits(q) =
                            minimaldemerits[fitclass - veryloosefit];
                        link(q) = r;
                        link(prevr) = q;
                        prevr = q; 
                        #ifdef tt_STAT
                            if (tracingparagraphs > 0) { /*846:*/
                                printnl(S(756));
                                print_int(serial(passive));
                                print(S(757));
                                print_int(linenumber(q) - 1);
                                print_char('.');
                                print_int(fitclass);
                                if (breaktype == hyphenated) print_char('-');
                                print(S(758));
                                print_int(totaldemerits(q));
                                print(S(759));
                                if (prevbreak(passive) == 0)
                                    print_char('0');
                                else
                                    print_int(serial(prevbreak(passive)));
                            }
                            /*:846*/
                        #endif // #845: tt_STAT
                    }
                    /*:845*/
                    minimaldemerits[fitclass - veryloosefit] = awfulbad;
                }
                minimumdemerits = awfulbad; /*844:*/
                if (r != lastactive) {      /*:844*/
                    q = get_node(deltanodesize);
                    link(q) = r;
                    type(q) = deltanode;
                    subtype(q) = 0;
                    newdeltafrombreakwidth(1);
                    newdeltafrombreakwidth(2);
                    newdeltafrombreakwidth(3);
                    newdeltafrombreakwidth(4);
                    newdeltafrombreakwidth(5);
                    newdeltafrombreakwidth(6);
                    link(prevr) = q;
                    prevprevr = prevr;
                    prevr = q;
                }
            } // if (minimumdemerits < awfulbad && (oldl != easyline || r == lastactive)
            /*:836*/
            if (r == lastactive) goto _Lexit; /*850:*/
            if (l > easyline) {
                linewidth = secondwidth;
                oldl = MAX_HALF_WORD - 1;
            } else { /*:850*/
                oldl = l;
                if (l > lastspecialline)
                    linewidth = secondwidth;
                else if (parshapeptr == 0)
                    linewidth = firstwidth;
                else
                    linewidth = mem[parshapeptr + l * 2 - MEM_MIN].sc;
            } // if (l > easyline) - else
        } // if (l > oldl)

        /*851:*/
        artificialdemerits = false;
        shortfall = linewidth - curactivewidth[0];
        if (shortfall > 0) { /*852:*/
            if (curactivewidth[2] != 0 || curactivewidth[3] != 0 ||
                curactivewidth[4] != 0) {
                b = 0;
                fitclass = decentfit;
            } else { /*:852*/
                if (shortfall > 7230584L) {
                    if (curactivewidth[1] < 1663497L) {
                        b = INF_BAD;
                        fitclass = veryloosefit;
                        goto _Ldone1;
                    }
                }
                b = badness(shortfall, curactivewidth[1]);
                if (b > 12) {
                    if (b > 99)
                        fitclass = veryloosefit;
                    else
                        fitclass = loosefit;
                } else
                    fitclass = decentfit;
            _Ldone1:;
            }
        } else /*853:*/
        {      /*:853*/
            if (-shortfall > curactivewidth[5])
                b = INF_BAD + 1;
            else
                b = badness(-shortfall, curactivewidth[5]);
            if (b > 12)
                fitclass = tightfit;
            else
                fitclass = decentfit;
        }
        if (b > INF_BAD || pi == EJECT_PENALTY) { /*854:*/
            if (((finalpass && minimumdemerits == awfulbad) &
                 (link(r) == lastactive)) &&
                prevr == active)
                artificialdemerits = true;
            else if (b > threshold)
                goto _Ldeactivate_;
            noderstaysactive = false;
        } /*:854*/
        else {
            prevr = r;
            if (b > threshold) goto _Llabcontinue;
            noderstaysactive = true;
        } /*855:*/
        if (artificialdemerits)
            d = 0;
        else { /*859:*/
            d = linepenalty + b;
            if (labs(d) >= 10000)
                d = 100000000L;
            else
                d *= d;
            if (pi != 0) {
                if (pi > 0)
                    d += pi * pi;
                else if (pi > EJECT_PENALTY)
                    d -= pi * pi;
            }
            if ((breaktype == hyphenated) & (type(r) == hyphenated)) {
                if (curp != 0)
                    d += doublehyphendemerits;
                else
                    d += finalhyphendemerits;
            }
            if (labs(fitclass - fitness(r)) > 1) d += adjdemerits;
        }

        #ifdef tt_STAT
            if (tracingparagraphs > 0) { /*:856*/
                /*856:*/
                if (printednode != curp) { /*857:*/
                    printnl(S(385));
                    if (curp == 0)
                        shortdisplay(link(printednode));
                    else {
                        savelink = link(curp);
                        link(curp) = 0;
                        printnl(S(385));
                        shortdisplay(link(printednode));
                        link(curp) = savelink;
                    }
                    printednode = curp;
                }
                /*:857*/
                printnl('@');
                if (curp == 0)
                    print_esc(S(760));
                else if (type(curp) != GLUE_NODE) {
                    if (type(curp) == PENALTY_NODE)
                        print_esc(S(761));
                    else if (type(curp) == DISC_NODE)
                        print_esc(S(400));
                    else if (type(curp) == KERN_NODE)
                        print_esc(S(391));
                    else
                        print_esc(S(394));
                }
                print(S(762));
                if (breaknode(r) == 0)
                    print_char('0');
                else
                    print_int(serial(breaknode(r)));
                print(S(763));
                if (b > INF_BAD)
                    print_char('*');
                else
                    print_int(b);
                print(S(764));
                print_int(pi);
                print(S(765));
                if (artificialdemerits)
                    print_char('*');
                else
                    print_int(d);
            }
        #endif // #855: tt_STAT

        d += totaldemerits(r);
        if (d <= minimaldemerits[fitclass - veryloosefit]) { /*:855*/
            minimaldemerits[fitclass - veryloosefit] = d;
            bestplace[fitclass - veryloosefit] = breaknode(r);
            bestplline[fitclass - veryloosefit] = l;
            if (d < minimumdemerits) minimumdemerits = d;
        }
        if (noderstaysactive) goto _Llabcontinue;
    _Ldeactivate_: /*860:*/
        link(prevr) = link(r);
        free_node(r, activenodesize);
        if (prevr == active) { /*861:*/
            r = link(active);
            if (type(r) != deltanode) continue;
            updateactive(1);
            updateactive(2);
            updateactive(3);
            updateactive(4);
            updateactive(5);
            updateactive(6);
            copytocuractive(1);
            copytocuractive(2);
            copytocuractive(3);
            copytocuractive(4);
            copytocuractive(5);
            copytocuractive(6);
            link(active) = link(r);
            free_node(r, deltanodesize);
            continue;
        } /*:861*/
        /*:851*/
        if (type(prevr) != deltanode) /*:860*/
            continue;
        r = link(prevr);
        if (r == lastactive) {
            downdatewidth(1);
            downdatewidth(2);
            downdatewidth(3);
            downdatewidth(4);
            downdatewidth(5);
            downdatewidth(6);
            link(prevprevr) = lastactive;
            free_node(prevr, deltanodesize);
            prevr = prevprevr;
            continue;
        }
        if (type(r) != deltanode) continue;
        updatewidth(1);
        updatewidth(2);
        updatewidth(3);
        updatewidth(4);
        updatewidth(5);
        updatewidth(6);
        combinetwodeltas(1);
        combinetwodeltas(2);
        combinetwodeltas(3);
        combinetwodeltas(4);
        combinetwodeltas(5);
        combinetwodeltas(6);
        link(prevr) = link(r);
        free_node(r, deltanodesize);
    } // while (true)

    _Lexit:
    /// p317#858: Update the value of printed node for symbolic displays
    #ifdef tt_STAT
        if (curp != printednode) return;
        if (curp == 0) return;
        if (type(curp) != DISC_NODE) return;
        t = replacecount(curp);
        while (t > 0) {
            t--;
            printednode = link(printednode);
        }
    #endif // #829: tt_STAT
       /*:843*/
       /*:859*/
    return;
} // #829: trybreak

/** @}*/ // end group S813x861_P302x318

/** @addtogroup S862x890_P319x329
 * @{
 */

/*872:*/
Pointer bestbet, ha, hb, initlist, curq, ligstack;
Integer fewestdemerits;
HalfWord bestline;
Integer actuallooseness, linediff;
/*:872*/

/*877:*/
void postlinebreak(long finalwidowpenalty) { /*878:*/
    Pointer q, r, s;
    Boolean discbreak, postdiscbreak;
    Scaled curwidth, curindent;
    QuarterWord t;
    long pen;
    HalfWord curline;

    q = breaknode(bestbet);
    curp = 0;
    do {
        r = q;
        q = prevbreak(q);
        nextbreak(r) = curp;
        curp = r; /*:878*/
    } while (q != 0);
    curline = prevgraf + 1;
    do { /*880:*/
        q = curbreak(curp);
        discbreak = false;
        postdiscbreak = false;
        if (q != 0) {
            if (type(q) == GLUE_NODE) {
                delete_glue_ref(glueptr(q));
                glueptr(q) = rightskip;
                subtype(q) = RIGHT_SKIP_CODE + 1;
                addglueref(rightskip);
                goto _Ldone;
            }
            if (type(q) == DISC_NODE) { /*882:*/
                t = replacecount(q);    /*883:*/
                if (t == 0)
                    r = link(q);
                else { /*:883*/
                    r = q;
                    while (t > 1) {
                        r = link(r);
                        t--;
                    }
                    s = link(r);
                    r = link(s);
                    link(s) = 0;
                    flush_node_list(link(q));
                    replacecount(q) = 0;
                }
                if (postbreak(q) != 0) { /*884:*/
                    s = postbreak(q);
                    while (link(s) != 0)
                        s = link(s);
                    link(s) = r;
                    r = postbreak(q);
                    postbreak(q) = 0;
                    postdiscbreak = true;
                }
                /*:884*/
                if (prebreak(q) != 0) { /*885:*/
                    s = prebreak(q);
                    link(q) = s;
                    while (link(s) != 0)
                        s = link(s);
                    prebreak(q) = 0;
                    q = s;
                } /*:885*/
                link(q) = r;
                discbreak = true;
            } /*:882*/
            else if ((type(q) == MATH_NODE) | (type(q) == KERN_NODE))
                width(q) = 0;
        } else {
            q = temphead;
            while (link(q) != 0)
                q = link(q);
        }
        /*886:*/
        r = newparamglue(RIGHT_SKIP_CODE);
        link(r) = link(q);
        link(q) = r;
        q = r; /*:886*/
    _Ldone:    /*:881*/
        /*887:*/
        r = link(q);
        link(q) = 0;
        q = link(temphead);
        link(temphead) = r;
        if (leftskip != zeroglue) { /*:887*/
            r = newparamglue(LEFT_SKIP_CODE);
            link(r) = q;
            q = r;
        }
        /*889:*/
        if (curline > lastspecialline) {
            curwidth = secondwidth;
            curindent = secondindent;
        } else if (parshapeptr == 0) {
            curwidth = firstwidth;
            curindent = firstindent;
        } else {
            curwidth = mem[parshapeptr + curline * 2 - MEM_MIN].sc;
            curindent = mem[parshapeptr + curline * 2 - MEM_MIN - 1].sc;
        }
        adjusttail = adjusthead;
        justbox = hpack(q, curwidth, exactly);
        shiftamount(justbox) = curindent; /*:889*/
        /*888:*/
        appendtovlist(justbox);
        if (adjusthead != adjusttail) {
            link(tail) = link(adjusthead);
            tail = adjusttail;
        }
        adjusttail = 0; /*:888*/
        /*890:*/
        if (curline + 1 != bestline) { /*:890*/
            pen = interlinepenalty;
            if (curline == prevgraf + 1) pen += clubpenalty;
            if (curline + 2 == bestline) pen += finalwidowpenalty;
            if (discbreak) pen += brokenpenalty;
            if (pen != 0) {
                r = newpenalty(pen);
                link(tail) = r;
                tail = r;
            }
        }
        /*:880*/
        curline++;
        curp = nextbreak(curp);
        if (curp != 0) {
            if (!postdiscbreak) { /*879:*/
                r = temphead;
                while (true) {
                    q = link(r);
                    if (q == curbreak(curp)) goto _Ldone1;
                    if (ischarnode(q)) goto _Ldone1;
                    if (nondiscardable(q)) {
                        goto _Ldone1;
                    }
                    if (type(q) == KERN_NODE) {
                        if (subtype(q) != explicit) goto _Ldone1;
                    }
                    r = q;
                }
            _Ldone1:
                if (r != temphead) {
                    link(r) = 0;
                    flush_node_list(link(temphead));
                    link(temphead) = q;
                }
            }
            /*:879*/
        }
    } while (curp != 0);
    /*881:*/
    // "line breaking"
    if ((curline != bestline) | (link(temphead) != 0)) confusion(S(766));
    prevgraf = bestline - 1;
}
/*:877*/

/** @}*/ // end group S862x890_P319x329