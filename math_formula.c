#include <stdlib.h>
#include "tex.h"
#include "pack.h"
#include "mem.h"
#include "dviout.h"
#include "eqtb.h"
#include "texmath.h"
#include "print.h"
#include "error.h"
#include "fonts.h"
#include "mmode.h"


/** @addtogroup S719x767_P265x284
 * [ #719. Typesetting math formulas. ]
 * @{
 */
/*719:*/
Pointer curmlist;
SmallNumber curstyle, cursize;
Boolean mlistpenalties;
/*:719*/
/*724:*/
InternalFontNumber curf;
QuarterWord curc;
FourQuarters curi;
/*:724*/

/*720:*/
HalfWord cleanbox(HalfWord p, SmallNumber s) {
    Pointer q, x, r;
    SmallNumber savestyle;

    switch (mathtype(p)) {
        case mathchar:
            curmlist = newnoad();
            mem[nucleus(curmlist) - MEM_MIN] = mem[p - MEM_MIN];
            break;

        case subbox:
            q = info(p);
            goto _Lfound;
            break;

        case submlist: curmlist = info(p); break;

        default:
            q = newnullbox();
            goto _Lfound;
            break;
    }
    savestyle = curstyle;
    curstyle = s;
    mlistpenalties = false;
    mlisttohlist();
    q = link(temphead);
    curstyle = savestyle; /*703:*/
    if (curstyle < scriptstyle)
        cursize = TEXT_SIZE;
    else
        cursize = (curstyle - textstyle) / 2 * 16;
    curmu = x_over_n(mathquad(cursize), 18); /*:703*/
_Lfound:
    if (ischarnode(q) || q == 0) /*721:*/
        x = hpack(q, 0, additional);
    else if ((link(q) == 0) & (type(q) <= VLIST_NODE) & (shiftamount(q) == 0))
        x = q;
    else
        x = hpack(q, 0, additional);
    q = listptr(x);
    if (!ischarnode(q)) /*:721*/
        return x;
    r = link(q);
    if (r == 0) return x;
    if (link(r) != 0) return x;
    if (ischarnode(r)) return x;
    if (type(r) == KERN_NODE) {
        free_node(r, smallnodesize);
        link(q) = 0;
    }
    return x;
}
/*:720*/

/*722:*/
void fetch(HalfWord a) {
    curc = character(a);
    curf = fam_fnt(fam(a) + cursize);
    if (curf == NULL_FONT) { /*723:*/
        print_err(S(385)); // ""
        print_size(cursize);
        print_char(' ');
        print_int(fam(a));
        print(S(715));
        print(curc - MIN_QUARTER_WORD);
        print_char(')');
        /*
         * (716) "Somewhere in the math formula just ended you used the"
         * (717) "stated character from an undefined font family. For example"
         * (718) "plain TeX doesn't allow \\it or \\sl in subscripts. Proceed"
         * (719) "and I'll try to forget that I needed that character."
         */
        help4(S(716), S(717), S(718), S(719));
        error();
        curi = nullcharacter;
        mathtype(a) = EMPTY;
        return;
    } /*:723*/
    if (curc - MIN_QUARTER_WORD >= fontbc[curf] &&
        curc - MIN_QUARTER_WORD <= fontec[curf])
        curi = charinfo(curf, curc);
    else
        curi = nullcharacter;
    if (!charexists(curi)) {
        charwarning(curf, curc - MIN_QUARTER_WORD);
        mathtype(a) = EMPTY;
    }
}
/*:722*/


/*
 * [#726]: Declare math construction procedures 
 * 
 * xref[10]
 *  734, 735, 736, 737, 738, 
 *  743, 749, 752, 756, 762 
 */

// #734: makeover
void makeover(HalfWord q) {
    info(nucleus(q)) = overbar(cleanbox(nucleus(q), crampedstyle(curstyle)),
                               defaultrulethickness * 3,
                               defaultrulethickness);
    mathtype(nucleus(q)) = subbox;
} // #734: makeover

// #735: makeunder
void makeunder(HalfWord q) {
    Pointer p, x, y;
    Scaled delta;

    x = cleanbox(nucleus(q), curstyle);
    p = newkern(defaultrulethickness * 3);
    link(x) = p;
    link(p) = fractionrule(defaultrulethickness);
    y = vpack(x, 0, additional);
    delta = height(y) + depth(y) + defaultrulethickness;
    height(y) = height(x);
    depth(y) = delta - height(y);
    info(nucleus(q)) = y;
    mathtype(nucleus(q)) = subbox;
} // #735: makeunder

// #736
void makevcenter(HalfWord q) {
    Pointer v;
    Scaled delta;

    v = info(nucleus(q));
    if (type(v) != VLIST_NODE) confusion(S(415)); // "vcenter"
    delta = height(v) + depth(v);
    height(v) = axisheight(cursize) + half(delta);
    depth(v) = delta - height(v);
} // #736: makevcenter

// #737
void makeradical(HalfWord q) {
    Pointer x, y;
    Scaled delta, clr;

    x = cleanbox(nucleus(q), crampedstyle(curstyle));
    if (curstyle < textstyle)
        clr = defaultrulethickness + labs(mathxheight(cursize)) / 4;
    else {
        clr = defaultrulethickness;
        clr += labs(clr) / 4;
    }
    y = vardelimiter(leftdelimiter(q),
                     cursize,
                     height(x) + depth(x) + clr + defaultrulethickness);
    delta = depth(y) - height(x) - depth(x) - clr;
    if (delta > 0) clr += half(delta);
    shiftamount(y) = -(height(x) + clr);
    link(y) = overbar(x, clr, height(y));
    info(nucleus(q)) = hpack(y, 0, additional);
    mathtype(nucleus(q)) = subbox;
} // #737: makeradical

// #738: makemathaccent
void makemathaccent(HalfWord q) {
    Pointer p, x, y;
    long a;
    QuarterWord c;
    InternalFontNumber f;
    FourQuarters i;
    Scaled s, h, delta, w;

    fetch(accentchr(q));
    if (!charexists(curi)) return;
    i = curi;
    c = curc;
    f = curf; /*741:*/
    s = 0;
    if (mathtype(nucleus(q)) == mathchar) {
        fetch(nucleus(q));
        if (chartag(curi) == LIG_TAG) {
            a = ligkernstart(curf, curi);
            curi = fontinfo[a].qqqq;
            if (skipbyte(curi) > stopflag) {
                a = ligkernrestart(curf, curi);
                curi = fontinfo[a].qqqq;
            }
            while (true) {
                if (nextchar(curi) - MIN_QUARTER_WORD == get_skewchar(curf)) {
                    if (opbyte(curi) >= kernflag) {
                        if (skipbyte(curi) <= stopflag)
                            s = charkern(curf, curi);
                    }
                    goto _Ldone1;
                }
                if (skipbyte(curi) >= stopflag) goto _Ldone1;
                a += skipbyte(curi) - MIN_QUARTER_WORD + 1;
                curi = fontinfo[a].qqqq;
            }
        }
    }
_Ldone1: /*:741*/
    x = cleanbox(nucleus(q), crampedstyle(curstyle));
    w = width(x);
    h = height(x); /*740:*/
    while (true) {
        if (chartag(i) != LIST_TAG) {
            goto _Ldone;
        }
        y = rembyte(i);
        i = charinfo(f, y);
        if (!charexists(i)) goto _Ldone;
        if (charwidth(f, i) > w) goto _Ldone;
        c = y;
    }
_Ldone: /*:740*/
    if (h < xheight(f))
        delta = h;
    else
        delta = xheight(f);
    if ((mathtype(supscr(q)) != EMPTY) | (mathtype(subscr(q)) != EMPTY)) {
        if (mathtype(nucleus(q)) == mathchar) { /*742:*/
            flush_node_list(x);
            x = newnoad();
            mem[nucleus(x) - MEM_MIN] = mem[nucleus(q) - MEM_MIN];
            mem[supscr(x) - MEM_MIN] = mem[supscr(q) - MEM_MIN];
            mem[subscr(x) - MEM_MIN] = mem[subscr(q) - MEM_MIN];
            mem[supscr(q) - MEM_MIN].hh = emptyfield;
            mem[subscr(q) - MEM_MIN].hh = emptyfield;
            mathtype(nucleus(q)) = submlist;
            info(nucleus(q)) = x;
            x = cleanbox(nucleus(q), curstyle);
            delta += height(x) - h;
            h = height(x);
        }
        /*:742*/
    }
    y = charbox(f, c);
    shiftamount(y) = s + half(w - width(y));
    width(y) = 0;
    p = newkern(-delta);
    link(p) = x;
    link(y) = p;
    y = vpack(y, 0, additional);
    width(y) = width(x);
    if (height(y) < h) { /*739:*/
        p = newkern(h - height(y));
        link(p) = listptr(y);
        listptr(y) = p;
        height(y) = h;
    } /*:739*/
    info(nucleus(q)) = y;
    mathtype(nucleus(q)) = subbox;
} // #738: makemathaccent

// #743
void makefraction(HalfWord q) {
    Pointer p, v, x, y, z;
    Scaled delta, delta1, delta2, shiftup, shiftdown, clr;

    if (thickness(q) == defaultcode) /*744:*/
        thickness(q) = defaultrulethickness;
    x = cleanbox(numerator(q), numstyle(curstyle));
    z = cleanbox(denominator(q), denomstyle(curstyle));
    if (width(x) < width(z))
        x = rebox(x, width(z));
    else
        z = rebox(z, width(x));
    if (curstyle < textstyle) {
        shiftup = num1(cursize);
        shiftdown = denom1(cursize);
    } else { /*:744*/
        shiftdown = denom2(cursize);
        if (thickness(q) != 0)
            shiftup = num2(cursize);
        else
            shiftup = num3(cursize);
    }
    if (thickness(q) == 0) { /*745:*/
        if (curstyle < textstyle)
            clr = defaultrulethickness * 7;
        else
            clr = defaultrulethickness * 3;
        delta = half(clr - shiftup + depth(x) + height(z) - shiftdown);
        if (delta > 0) {
            shiftup += delta;
            shiftdown += delta;
        }
    } else {
        if (curstyle < textstyle)
            clr = thickness(q) * 3;
        else
            clr = thickness(q);
        delta = half(thickness(q));
        delta1 = clr - shiftup + depth(x) + axisheight(cursize) + delta;
        delta2 = clr - axisheight(cursize) + delta + height(z) - shiftdown;
        if (delta1 > 0) shiftup += delta1;
        if (delta2 > 0) shiftdown += delta2;
    }
    /*:745*/
    /*747:*/
    v = newnullbox();
    type(v) = VLIST_NODE;
    height(v) = shiftup + height(x);
    depth(v) = depth(z) + shiftdown;
    width(v) = width(x);
    if (thickness(q) == 0) {
        p = newkern(shiftup - depth(x) - height(z) + shiftdown);
        link(p) = z;
    } else {
        y = fractionrule(thickness(q));
        p = newkern(axisheight(cursize) - delta - height(z) + shiftdown);
        link(y) = p;
        link(p) = z;
        p = newkern(shiftup - depth(x) - axisheight(cursize) - delta);
        link(p) = y;
    }
    link(x) = p;
    listptr(v) = x; /*:747*/
    /*748:*/
    if (curstyle < textstyle)
        delta = delim1(cursize);
    else
        delta = delim2(cursize);
    x = vardelimiter(leftdelimiter(q), cursize, delta);
    link(x) = v;
    z = vardelimiter(rightdelimiter(q), cursize, delta);
    link(v) = z;
    newhlist(q) = hpack(x, 0, additional); /*:748*/
} // #743: makefraction

// #749
Integer makeop(HalfWord q) {
    Scaled delta, shiftup, shiftdown;
    Pointer p, v, x, y, z;
    QuarterWord c;
    FourQuarters i;

    if (subtype(q) == NORMAL && curstyle < textstyle) subtype(q) = limits;
    if (mathtype(nucleus(q)) == mathchar) {
        fetch(nucleus(q));
        if ((curstyle < textstyle) & (chartag(curi) == LIST_TAG)) {
            c = rembyte(curi);
            i = charinfo(curf, c);
            if (charexists(i)) {
                curc = c;
                curi = i;
                character(nucleus(q)) = c;
            }
        }
        delta = charitalic(curf, curi);
        x = cleanbox(nucleus(q), curstyle);
        if ((mathtype(subscr(q)) != EMPTY) & (subtype(q) != limits))
            width(x) -= delta;
        shiftamount(x) = half(height(x) - depth(x)) - axisheight(cursize);
        mathtype(nucleus(q)) = subbox;
        info(nucleus(q)) = x;
    } else
        delta = 0;
    if (subtype(q) != limits) /*750:*/
        return delta;
    /*:750*/
    x = cleanbox(supscr(q), supstyle(curstyle));
    y = cleanbox(nucleus(q), curstyle);
    z = cleanbox(subscr(q), substyle(curstyle));
    v = newnullbox();
    type(v) = VLIST_NODE;
    width(v) = width(y);
    if (width(x) > width(v)) width(v) = width(x);
    if (width(z) > width(v)) width(v) = width(z);
    x = rebox(x, width(v));
    y = rebox(y, width(v));
    z = rebox(z, width(v));
    shiftamount(x) = half(delta);
    shiftamount(z) = -shiftamount(x);
    height(v) = height(y);
    depth(v) = depth(y); /*751:*/
    if (mathtype(supscr(q)) == EMPTY) {
        free_node(x, boxnodesize);
        listptr(v) = y;
    } else {
        shiftup = bigopspacing3 - depth(x);
        if (shiftup < bigopspacing1) shiftup = bigopspacing1;
        p = newkern(shiftup);
        link(p) = y;
        link(x) = p;
        p = newkern(bigopspacing5);
        link(p) = x;
        listptr(v) = p;
        height(v) += bigopspacing5 + height(x) + depth(x) + shiftup;
    }
    if (mathtype(subscr(q)) == EMPTY)
        free_node(z, boxnodesize);
    else { /*:751*/
        shiftdown = bigopspacing4 - height(z);
        if (shiftdown < bigopspacing2) shiftdown = bigopspacing2;
        p = newkern(shiftdown);
        link(y) = p;
        link(p) = z;
        p = newkern(bigopspacing5);
        link(z) = p;
        depth(v) += bigopspacing5 + height(z) + depth(z) + shiftdown;
    }
    newhlist(q) = v;
    return delta;
} // #749: makeop

// #752
void makeord(HalfWord q) {
    long a;
    Pointer p, r;

_Lrestart:
    if (mathtype(subscr(q)) == EMPTY) {
        if (mathtype(supscr(q)) == EMPTY) {
            if (mathtype(nucleus(q)) == mathchar) {
                p = link(q);
                if (p != 0) {
                    if ((type(p) >= ordnoad) & (type(p) <= punctnoad)) {
                        if (mathtype(nucleus(p)) == mathchar) {
                            if (fam(nucleus(p)) == fam(nucleus(q))) {
                                mathtype(nucleus(q)) = mathtextchar;
                                fetch(nucleus(q));
                                if (chartag(curi) == LIG_TAG) {
                                    a = ligkernstart(curf, curi);
                                    curc = character(nucleus(p));
                                    curi = fontinfo[a].qqqq;
                                    if (skipbyte(curi) > stopflag) {
                                        a = ligkernrestart(curf, curi);
                                        curi = fontinfo[a].qqqq;
                                    }
                                    while (true) { /*753:*/
                                        if (nextchar(curi) == curc) {
                                            if (skipbyte(curi) <= stopflag) {
                                                if (opbyte(curi) >= kernflag) {
                                                    p = newkern(
                                                        charkern(curf, curi));
                                                    link(p) = link(q);
                                                    link(q) = p;
                                                    goto _Lexit;
                                                } else { /*:753*/
                                                    checkinterrupt();
                                                    switch (opbyte(curi)) {

                                                        case MIN_QUARTER_WORD +
                                                            1:
                                                        case MIN_QUARTER_WORD +
                                                            5:
                                                            character(
                                                                nucleus(q)) =
                                                                rembyte(curi);
                                                            break;

                                                        case MIN_QUARTER_WORD +
                                                            2:
                                                        case MIN_QUARTER_WORD +
                                                            6:
                                                            character(
                                                                nucleus(p)) =
                                                                rembyte(curi);
                                                            break;

                                                        case MIN_QUARTER_WORD +
                                                            3:
                                                        case MIN_QUARTER_WORD +
                                                            7:
                                                        case MIN_QUARTER_WORD +
                                                            11:
                                                            r = newnoad();
                                                            character(
                                                                nucleus(r)) =
                                                                rembyte(curi);
                                                            fam(nucleus(r)) =
                                                                fam(nucleus(q));
                                                            link(q) = r;
                                                            link(r) = p;
                                                            if (opbyte(curi) <
                                                                MIN_QUARTER_WORD +
                                                                    11)
                                                                mathtype(
                                                                    nucleus(
                                                                        r)) =
                                                                    mathchar;
                                                            else
                                                                mathtype(
                                                                    nucleus(
                                                                        r)) =
                                                                    mathtextchar;
                                                            break;

                                                        default:
                                                            link(q) = link(p);
                                                            character(
                                                                nucleus(q)) =
                                                                rembyte(curi);
                                                            mem[subscr(q) -
                                                                MEM_MIN] =
                                                                mem[subscr(p) -
                                                                    MEM_MIN];
                                                            mem[supscr(q) -
                                                                MEM_MIN] =
                                                                mem[supscr(p) -
                                                                    MEM_MIN];
                                                            free_node(p,
                                                                     noadsize);
                                                            break;
                                                    }
                                                    if (opbyte(curi) >
                                                        MIN_QUARTER_WORD + 3)
                                                        goto _Lexit;
                                                    mathtype(nucleus(q)) =
                                                        mathchar;
                                                    goto _Lrestart;
                                                }
                                            }
                                        }
                                        if (skipbyte(curi) >= stopflag)
                                            goto _Lexit;
                                        a += skipbyte(curi) - MIN_QUARTER_WORD +
                                             1;
                                        curi = fontinfo[a].qqqq;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
_Lexit:;
} // #752: makeord

// #756
void makescripts(HalfWord q, long delta) {
    Pointer p, x, y, z;
    Scaled shiftup, shiftdown, clr;
    SmallNumber t;

    p = newhlist(q);
    if (ischarnode(p)) {
        shiftup = 0;
        shiftdown = 0;
    } else {
        z = hpack(p, 0, additional);
        if (curstyle < scriptstyle)
            t = SCRIPT_SIZE;
        else
            t = SCRIPT_SCRIPT_SIZE;
        shiftup = height(z) - supdrop(t);
        shiftdown = depth(z) + subdrop(t);
        free_node(z, boxnodesize);
    }
    if (mathtype(supscr(q)) == EMPTY) { /*757:*/
        x = cleanbox(subscr(q), substyle(curstyle));
        width(x) += scriptspace;
        if (shiftdown < sub1(cursize)) shiftdown = sub1(cursize);
        clr = height(x) - labs(mathxheight(cursize) * 4) / 5;
        if (shiftdown < clr) shiftdown = clr;
        shiftamount(x) = shiftdown;
    } else { /*758:*/
        x = cleanbox(supscr(q), supstyle(curstyle));
        width(x) += scriptspace;
        if (curstyle & 1)
            clr = sup3(cursize);
        else if (curstyle < textstyle)
            clr = sup1(cursize);
        else
            clr = sup2(cursize);
        if (shiftup < clr) shiftup = clr;
        clr = depth(x) + labs(mathxheight(cursize)) / 4;
        if (shiftup < clr) /*:758*/
            shiftup = clr;
        if (mathtype(subscr(q)) == EMPTY)
            shiftamount(x) = -shiftup;
        else { /*759:*/
            y = cleanbox(subscr(q), substyle(curstyle));
            width(y) += scriptspace;
            if (shiftdown < sub2(cursize)) shiftdown = sub2(cursize);
            clr = defaultrulethickness * 4 - shiftup + depth(x) + height(y) -
                  shiftdown;
            if (clr > 0) {
                shiftdown += clr;
                clr = labs(mathxheight(cursize) * 4) / 5 - shiftup + depth(x);
                if (clr > 0) {
                    shiftup += clr;
                    shiftdown -= clr;
                }
            }
            shiftamount(x) = delta;
            p = newkern(shiftup - depth(x) - height(y) + shiftdown);
            link(x) = p;
            link(p) = y;
            x = vpack(x, 0, additional);
            shiftamount(x) = shiftdown;
        }
    }
    /*:757*/
    if (newhlist(q) == 0) {
        newhlist(q) = x;
        return;
    }
    p = newhlist(q);
    while (link(p) != 0)
        p = link(p);
    link(p) = x;

    /*:759*/
} // #756: makescripts

// #762
SmallNumber makeleftright(HalfWord q,
                                 SmallNumber style,
                                 long maxd,
                                 long maxh) {
    Scaled delta, delta1, delta2;

    if (style < scriptstyle)
        cursize = TEXT_SIZE;
    else
        cursize = (style - textstyle) / 2 * 16;
    delta2 = maxd + axisheight(cursize);
    delta1 = maxh + maxd - delta2;
    if (delta2 > delta1) delta1 = delta2;
    delta = delta1 / 500 * delimiterfactor;
    delta2 = delta1 + delta1 - delimitershortfall;
    if (delta < delta2) delta = delta2;
    newhlist(q) = vardelimiter(delimiter(q), cursize, delta);
    return (type(q) - leftnoad + opennoad);
} // #762: makeleftright


void mlisttohlist(void) {
    Pointer mlist, q, r, p = 0 /* XXXX */, x = 0 /* XXXX */, y, z;
    Boolean penalties;
    SmallNumber style, savestyle, rtype, t;
    long pen;
    SmallNumber s;
    Scaled maxh, maxd, delta;

    mlist = curmlist;
    penalties = mlistpenalties;
    style = curstyle;
    q = mlist;
    r = 0;
    rtype = opnoad;
    maxh = 0;
    maxd = 0; /*703:*/
    if (curstyle < scriptstyle)
        cursize = TEXT_SIZE;
    else
        cursize = (curstyle - textstyle) / 2 * 16;
    curmu = x_over_n(mathquad(cursize), 18); /*:703*/
    while (q != 0) {                         /*727:*/
    _LN_mlisttohlist__reswitch:
        delta = 0;
        switch (type(q)) {

            case binnoad:
                switch (rtype) {

                    case binnoad:
                    case opnoad:
                    case relnoad:
                    case opennoad:
                    case punctnoad:
                    case leftnoad:
                        type(q) = ordnoad;
                        goto _LN_mlisttohlist__reswitch;
                        break;
                }
                break;

            case relnoad:
            case closenoad:
            case punctnoad:
            case rightnoad:                              /*729:*/
                if (rtype == binnoad) type(r) = ordnoad; /*:729*/
                if (type(q) == rightnoad) goto _Ldonewithnoad_;
                break;
                /*733:*/

            case leftnoad: goto _Ldonewithnoad_; break;

            case fractionnoad:
                makefraction(q);
                goto _Lcheckdimensions_;
                break;

            case opnoad:
                delta = makeop(q);
                if (subtype(q) == limits) goto _Lcheckdimensions_;
                break;

            case ordnoad: makeord(q); break;

            case opennoad:
            case innernoad:
                /* blank case */
                break;

            case radicalnoad: makeradical(q); break;

            case overnoad: makeover(q); break;

            case undernoad: makeunder(q); break;

            case accentnoad: makemathaccent(q); break;

            case vcenternoad: /*:733*/
                makevcenter(q);
                break;
                /*730:*/

            case stylenode:
                curstyle = subtype(q); /*703:*/
                if (curstyle < scriptstyle)
                    cursize = TEXT_SIZE;
                else
                    cursize = (curstyle - textstyle) / 2 * 16;
                curmu = x_over_n(mathquad(cursize), 18); /*:703*/
                goto _Ldonewithnode_;
                break;

            case choicenode: /*731:*/
                switch (curstyle / 2) {

                    case 0:
                        p = displaymlist(q);
                        displaymlist(q) = 0;
                        break;

                    case 1:
                        p = textmlist(q);
                        textmlist(q) = 0;
                        break;

                    case 2:
                        p = scriptmlist(q);
                        scriptmlist(q) = 0;
                        break;

                    case 3:
                        p = scriptscriptmlist(q);
                        scriptscriptmlist(q) = 0;
                        break;
                }
                flush_node_list(displaymlist(q));
                flush_node_list(textmlist(q));
                flush_node_list(scriptmlist(q));
                flush_node_list(scriptscriptmlist(q));
                type(q) = stylenode;
                subtype(q) = curstyle;
                width(q) = 0;
                depth(q) = 0;
                if (p != 0) {
                    z = link(q);
                    link(q) = p;
                    while (link(p) != 0)
                        p = link(p);
                    link(p) = z;
                }
                goto _Ldonewithnode_;
                break;
                /*:731*/

            case INS_NODE:
            case MARK_NODE:
            case ADJUST_NODE:
            case WHATSIT_NODE:
            case PENALTY_NODE:
            case DISC_NODE: goto _Ldonewithnode_; break;

            case RULE_NODE:
                if (height(q) > maxh) maxh = height(q);
                if (depth(q) > maxd) maxd = depth(q);
                goto _Ldonewithnode_;
                break;

            case GLUE_NODE: /*732:*/
                if (subtype(q) == muglue) {
                    x = glueptr(q);
                    y = mathglue(x, curmu);
                    delete_glue_ref(x);
                    glueptr(q) = y;
                    subtype(q) = NORMAL;
                } else if ((cursize != TEXT_SIZE) &
                           (subtype(q) == condmathglue)) {
                    p = link(q);
                    if (p != 0) {
                        if ((type(p) == GLUE_NODE) | (type(p) == KERN_NODE)) {
                            link(q) = link(p);
                            link(p) = 0;
                            flush_node_list(p);
                        }
                    }
                }
                goto _Ldonewithnode_;
                break;

            case KERN_NODE: /*:730*/
                mathkern(q, curmu);
                goto _Ldonewithnode_;
                break;

            default:
                confusion(S(720)); // "mlist1"
                break;
        }
        /*754:*/
        switch (mathtype(nucleus(q))) {

            case mathchar:
            case mathtextchar: /*:755*/
                /*755:*/
                fetch(nucleus(q));
                if (charexists(curi)) {
                    delta = charitalic(curf, curi);
                    p = newcharacter(curf, curc - MIN_QUARTER_WORD);
                    if ((mathtype(nucleus(q)) == mathtextchar) &
                        (space(curf) != 0))
                        delta = 0;
                    if (mathtype(subscr(q)) == EMPTY && delta != 0) {
                        link(p) = newkern(delta);
                        delta = 0;
                    }
                } else
                    p = 0;
                break;

            case EMPTY: p = 0; break;

            case subbox: p = info(nucleus(q)); break;

            case submlist:
                curmlist = info(nucleus(q));
                savestyle = curstyle;
                mlistpenalties = false;
                mlisttohlist();
                curstyle = savestyle; /*703:*/
                if (curstyle < scriptstyle)
                    cursize = TEXT_SIZE;
                else
                    cursize = (curstyle - textstyle) / 2 * 16;
                curmu = x_over_n(mathquad(cursize), 18); /*:703*/
                p = hpack(link(temphead), 0, additional);
                break;

            default:
                confusion(S(721)); // "mlist2"
                break;
        }
        newhlist(q) = p;
        if ((mathtype(subscr(q)) == EMPTY) & (mathtype(supscr(q)) == EMPTY))
            /*:754*/
            goto _Lcheckdimensions_;
        /*:728*/
        makescripts(q, delta);
    _Lcheckdimensions_:
        z = hpack(newhlist(q), 0, additional);
        if (height(z) > maxh) maxh = height(z);
        if (depth(z) > maxd) maxd = depth(z);
        free_node(z, boxnodesize);
    _Ldonewithnoad_:
        r = q;
        rtype = type(r);
    _Ldonewithnode_:
        q = link(q);
    }
    /*728:*/
    /*:727*/
    /*729:*/
    if (rtype == binnoad)  /*760:*/
        type(r) = ordnoad; /*:729*/
    p = temphead;
    link(p) = 0;
    q = mlist;
    rtype = 0;
    curstyle = style; /*703:*/
    if (curstyle < scriptstyle)
        cursize = TEXT_SIZE;
    else
        cursize = (curstyle - textstyle) / 2 * 16;
    curmu = x_over_n(mathquad(cursize), 18); /*:703*/
    while (q != 0) {                         /*761:*/
        t = ordnoad;
        s = noadsize;
        pen = INF_PENALTY;
        switch (type(q)) { /*:761*/

            case opnoad:
            case opennoad:
            case closenoad:
            case punctnoad:
            case innernoad: t = type(q); break;

            case binnoad:
                t = binnoad;
                pen = binoppenalty;
                break;

            case relnoad:
                t = relnoad;
                pen = relpenalty;
                break;

            case ordnoad:
            case vcenternoad:
            case overnoad:
            case undernoad:
                /* blank case */
                break;

            case radicalnoad: s = radicalnoadsize; break;

            case accentnoad: s = accentnoadsize; break;

            case fractionnoad:
                t = innernoad;
                s = fractionnoadsize;
                break;

            case leftnoad:
            case rightnoad: t = makeleftright(q, style, maxd, maxh); break;

            case stylenode: /*763:*/
                curstyle = subtype(q);
                s = stylenodesize; /*703:*/
                if (curstyle < scriptstyle)
                    cursize = TEXT_SIZE;
                else
                    cursize = (curstyle - textstyle) / 2 * 16;
                curmu = x_over_n(mathquad(cursize), 18); /*:703*/
                goto _Ldeleteq_;
                break;
                /*:763*/

            case WHATSIT_NODE:
            case PENALTY_NODE:
            case RULE_NODE:
            case DISC_NODE:
            case ADJUST_NODE:
            case INS_NODE:
            case MARK_NODE:
            case GLUE_NODE:
            case KERN_NODE:
                link(p) = q;
                p = q;
                q = link(q);
                link(p) = 0;
                goto _Ldone;
                break;

            default:
                confusion(S(722)); // "mlist3"
                break;
        }
        /*766:*/
        if (rtype > 0) { /*:766*/
            const char math_spacing[] = "0234000122*4000133**3**344*0400400*"
                                        "000000234000111*1111112341011";
            switch (math_spacing[rtype * 8 + t - ordnoad * 9]) {
                case '0': x = 0; break;

                case '1':
                    if (curstyle < scriptstyle)
                        x = THIN_MU_SKIP_CODE;
                    else
                        x = 0;
                    break;

                case '2': x = THIN_MU_SKIP_CODE; break;

                case '3':
                    if (curstyle < scriptstyle)
                        x = MED_MU_SKIP_CODE;
                    else
                        x = 0;
                    break;

                case '4':
                    if (curstyle < scriptstyle)
                        x = THICK_MU_SKIP_CODE;
                    else
                        x = 0;
                    break;

                default:
                    confusion(S(723)); // "mlist4"
                    break;
            }
            if (x != 0) {
                y = mathglue(glue_par(x), curmu);
                z = newglue(y);
                gluerefcount(y) = 0;
                link(p) = z;
                p = z;
                subtype(z) = x + 1;
            }
        }
        /*767:*/
        if (newhlist(q) != 0) {
            link(p) = newhlist(q);
            do {
                p = link(p);
            } while (link(p) != 0);
        }
        if (penalties) {
            if (link(q) != 0) {
                if (pen < INF_PENALTY) { /*:767*/
                    rtype = type(link(q));
                    if (rtype != PENALTY_NODE) {
                        if (rtype != relnoad) {
                            z = newpenalty(pen);
                            link(p) = z;
                            p = z;
                        }
                    }
                }
            }
        }
        rtype = t;
    _Ldeleteq_:
        r = q;
        q = link(q);
        free_node(r, s);
    _Ldone:;
    }
    /*:760*/

    /*:732*/
}
/*:726*/
/** @}*/ // end group S719x767_P265x284