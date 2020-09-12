#include <stdlib.h> // labs
#include "tex.h"
#include "global.h"
#include "mem.h"
#include "macros.h"
#include "texfunc.h"
#include "print.h"
#include "eqtb.h"
#include "pack.h"
#include "error.h"
#include "texmath.h"
#include "linebreak.h"



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


void hyphenate(void) /*:929*/
{                           /*923:*/
                            /*901:*/
    /* char */ int i, j, l; /* INT */
    Pointer q, r, s;
    HalfWord bchar;
    /*:901*/
    /*912:*/
    Pointer majortail, minortail, hyfnode;
    ASCIICode c = 0 /* XXXX */;
    /* char */ int cloc; /* INT */
    long rcount;
    /*:912*/
    /*922:*/
    TriePointer z;
    long v;
    /*:922*/
    /*929:*/
    HyphPointer h;
    StrNumber k;
    char FORLIM;

    for (j = 0; j <= hn; j++) /*930:*/
        hyf[j] = 0;
    h = hc[1];
    hn++;
    hc[hn] = curlang;
    for (j = 2; j <= hn; j++)
        h = (h + h + hc[j]) % HYPH_SIZE;
    while (true) { /*931:*/
        k = hyphword[h];
        if (k == 0) goto _Lnotfound;
        if (str_length(k) < hn) {
            goto _Lnotfound;
        }
        if (str_length(k) == hn) {
            {
                int ress = str_scmp(k, hc + 1);
                if (ress < 0) goto _Lnotfound;
                if (ress > 0) goto _Ldone;
            }
            s = hyphlist[h];
            while (s != 0) { /*:932*/
                hyf[info(s)] = 1;
                s = link(s);
            }
            hn--;
            goto _Lfound;
        }
    _Ldone: /*:931*/
        if (h > 0)
            h--;
        else
            h = HYPH_SIZE;
    }
_Lnotfound:
    hn--; /*:930*/
    if (triechar(curlang + 1) != curlang) goto _Lexit;
    hc[0] = 0;
    hc[hn + 1] = 0;
    hc[hn + 2] = 256;
    FORLIM = hn - rhyf + 1;
    for (j = 0; j <= FORLIM; j++) {
        z = trielink(curlang + 1) + hc[j];
        l = j;
        while (hc[l] == triechar(z) - MIN_QUARTER_WORD) {
            if (trieop(z) != MIN_QUARTER_WORD) { /*924:*/
                v = trieop(z);
                do {
                    v += opstart[curlang];
                    i = l - hyfdistance[v - 1];
                    if (hyfnum[v - 1] > hyf[i]) hyf[i] = hyfnum[v - 1];
                    v = hyfnext[v - 1];
                } while (v != MIN_QUARTER_WORD);
            }
            /*:924*/
            l++;
            z = trielink(z) + hc[l];
        }
    }
_Lfound:
    for (j = 0; j < lhyf; j++)
        hyf[j] = 0;
    for (j = 0; j < rhyf; j++) /*902:*/
        hyf[hn - j] = 0;       /*:923*/
    for (j = lhyf; j <= hn - rhyf; j++) {
        if (hyf[j] & 1) goto _Lfound1;
    }
    goto _Lexit;
_Lfound1: /*:902*/
    /*903:*/
    q = link(hb);
    link(hb) = 0;
    r = link(ha);
    link(ha) = 0;
    bchar = hyfbchar;
    if (ischarnode(ha)) {
        if (font(ha) != hf) goto _Lfound2;
        initlist = ha;
        initlig = false;
        hu[0] = character(ha) - MIN_QUARTER_WORD;
    } else if (type(ha) == LIGATURE_NODE) {
        if (font_ligchar(ha) != hf) {
            goto _Lfound2;
        }
        initlist = ligptr(ha);
        initlig = true;
        initlft = (subtype(ha) > 1);
        hu[0] = character_ligchar(ha) - MIN_QUARTER_WORD;
        if (initlist == 0) {
            if (initlft) {
                hu[0] = 256;
                initlig = false;
            }
        }
        free_node(ha, smallnodesize);
    } else {
        if (!ischarnode(r)) {
            if (type(r) == LIGATURE_NODE) {
                if (subtype(r) > 1) goto _Lfound2;
            }
        }
        j = 1;
        s = ha;
        initlist = 0;
        goto _Lcommonending;
    }
    s = curp;
    while (link(s) != ha)
        s = link(s);
    j = 0;
    goto _Lcommonending;
_Lfound2:
    s = ha;
    j = 0;
    hu[0] = 256;
    initlig = false;
    initlist = 0;
_Lcommonending:
    flush_node_list(r); /*913:*/
    do {
        l = j;
        j = reconstitute(j, hn, bchar, hyfchar) + 1;
        if (hyphenpassed == 0) {
            link(s) = link(holdhead);
            while (link(s) > 0)
                s = link(s);
            if (hyf[j - 1] & 1) {
                l = j;
                hyphenpassed = j - 1;
                link(holdhead) = 0;
            }
        }
        if (hyphenpassed > 0) { /*914:*/
            do {
                r = get_node(smallnodesize);
                link(r) = link(holdhead);
                type(r) = DISC_NODE;
                majortail = r;
                rcount = 0;
                while (link(majortail) > 0) {
                    advancemajortail();
                }
                i = hyphenpassed;
                hyf[i] = 0; /*915:*/
                minortail = 0;
                prebreak(r) = 0;
                hyfnode = newcharacter(hf, hyfchar);
                if (hyfnode != 0) {
                    i++;
                    c = hu[i];
                    hu[i] = hyfchar;
                    FREE_AVAIL(hyfnode);
                }
                while (l <= i) {
                    l = reconstitute(l, i, fontbchar[hf], NON_CHAR) + 1;
                    if (link(holdhead) <= 0) continue;
                    if (minortail == 0)
                        prebreak(r) = link(holdhead);
                    else
                        link(minortail) = link(holdhead);
                    minortail = link(holdhead);
                    while (link(minortail) > 0)
                        minortail = link(minortail);
                }
                if (hyfnode != 0) { /*:915*/
                    hu[i] = c;
                    l = i;
                    i--;
                }
                /*916:*/
                minortail = 0;
                postbreak(r) = 0;
                cloc = 0;
                if (bcharlabel[hf] != nonaddress) {
                    l--;
                    c = hu[l];
                    cloc = l;
                    hu[l] = 256;
                }
                while (l < j) { /*:916*/
                    do {
                        l = reconstitute(l, hn, bchar, NON_CHAR) + 1;
                        if (cloc > 0) {
                            hu[cloc] = c;
                            cloc = 0;
                        }
                        if (link(holdhead) > 0) {
                            if (minortail == 0)
                                postbreak(r) = link(holdhead);
                            else
                                link(minortail) = link(holdhead);
                            minortail = link(holdhead);
                            while (link(minortail) > 0)
                                minortail = link(minortail);
                        }
                    } while (l < j);
                    while (l > j) { /*917:*/
                        j = reconstitute(j, hn, bchar, NON_CHAR) + 1;
                        link(majortail) = link(holdhead);
                        while (link(majortail) > 0) {
                            advancemajortail();
                        }
                    }
                    /*:917*/
                }
                /*918:*/
                if (rcount > 127) {
                    link(s) = link(r);
                    link(r) = 0;
                    flush_node_list(r);
                } else {
                    link(s) = r;
                    replacecount(r) = rcount;
                }
                s = majortail; /*:918*/
                hyphenpassed = j - 1;
                link(holdhead) = 0; /*:914*/
            } while (hyf[j - 1] & 1);
        }
    } while (j <= hn);
    link(s) = q; /*:913*/
    /*:903*/
    flush_list(initlist);
_Lexit:;
}
/*:895*/
/** @}*/ // end group S862x890_P319x329