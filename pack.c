#include <stdlib.h> // [func] labs
#include "tex_constant.h" // [const] MIN_QUARTER_WORD
#include "texmath.h" // [func] print_scaled, badness
#include "scan.h"
#include "eqtb.h"
#include "mem.h"
#include "fonts.h"
#include "print.h"
#include "error.h"
#include "fonts.h"
#include "page.h"
#include "lexer.h" // [macro] ignoredepth
#include "pack.h"


/** @addtogroup S644x679_P239x248
 * [ p239~248#644~679 Packaging. ]
 * the real work of typesetting
 * @{
 */
/*646:*/
Scaled totalstretch[FILLL - NORMAL + 1], totalshrink[FILLL - NORMAL + 1];
Integer lastbadness;
/*:646*/
/*647:*/
Pointer adjusttail;
/*:647*/
/*661:*/
Integer packbeginline;
/*:661*/


/// [#648,#662]
void pack_init() {
    /// [#648]
    adjusttail = null;
    lastbadness = 0;

    /// [#662]
    packbeginline = 0;
} /* [#648,#662] pack_init */

// ##645
void scanspec(GroupCode c, Boolean threecodes) {
    long s = 0 /* XXXX */;
    char speccode;

    if (threecodes) s = saved(0);
    if (scankeyword(S(697)))
        speccode = exactly;
    else if (scankeyword(S(698)))
        speccode = additional;
    else {
        speccode = additional;
        cur_val = 0;
        goto _Lfound;
    }
    SCAN_NORMAL_DIMEN();
_Lfound:
    if (threecodes) {
        saved(0) = s;
        save_ptr++;
    }
    saved(0) = speccode;
    saved(1) = cur_val;
    save_ptr += 2;
    new_save_level(c);
    scan_left_brace();
} // #645: scanspec

// #649
HalfWord hpack(HalfWord p, long w, SmallNumber m) {
    Pointer r, q, g;
    Scaled h, d, x, s;
    GlueOrd o;
    EightBits hd;

    lastbadness = 0;
    r = get_node(boxnodesize);
    type(r) = HLIST_NODE;
    subtype(r) = MIN_QUARTER_WORD;
    shiftamount(r) = 0;
    q = r + listoffset;
    link(q) = p;
    h = 0; /*650:*/
    d = 0;
    x = 0;
    totalstretch[0] = 0;
    totalshrink[0] = 0;
    totalstretch[FIL - NORMAL] = 0;
    totalshrink[FIL - NORMAL] = 0;
    totalstretch[FILL - NORMAL] = 0;
    totalshrink[FILL - NORMAL] = 0;
    totalstretch[FILLL - NORMAL] = 0;
    totalshrink[FILLL - NORMAL] = 0; /*:650*/
    while (p != 0) {                 /*651:*/
    _LN_hpack__reswitch:
        while (ischarnode(p)) { /*654:*/
            InternalFontNumber f = font(p);
            FourQuarters i = charinfo(f, character(p));
            hd = heightdepth(i);
            x += charwidth(f, i);
            s = charheight(f, hd);
            if (s > h) h = s;
            s = chardepth(f, hd);
            if (s > d) d = s;
            p = link(p);
        }
        /*:654*/
        if (p == 0) break;
        switch (type(p)) {

            case HLIST_NODE:
            case VLIST_NODE:
            case RULE_NODE:
            case UNSET_NODE: /*653:*/
                x += width(p);
                if (type(p) >= RULE_NODE)
                    s = 0;
                else
                    s = shiftamount(p);
                if (height(p) - s > h) h = height(p) - s;
                if (depth(p) + s > d) d = depth(p) + s;
                break;
                /*:653*/

            case INS_NODE:
            case MARK_NODE:
            case ADJUST_NODE:
                if (adjusttail != 0) { /*655:*/
                    while (link(q) != p)
                        q = link(q);
                    if (type(p) == ADJUST_NODE) {
                        link(adjusttail) = adjustptr(p);
                        while (link(adjusttail) != 0)
                            adjusttail = link(adjusttail);
                        p = link(p);
                        free_node(link(q), smallnodesize);
                    } else {
                        link(adjusttail) = p;
                        adjusttail = p;
                        p = link(p);
                    }
                    link(q) = p;
                    p = q;
                }
                /*:655*/
                break;

            case WHATSIT_NODE: /*1360:*/
                break;
                /*:1360*/

            case GLUE_NODE: /*656:*/
                g = glueptr(p);
                x += width(g);
                o = stretchorder(g);
                totalstretch[o - NORMAL] += stretch(g);
                o = shrinkorder(g);
                totalshrink[o - NORMAL] += shrink(g);
                if (subtype(p) >= aleaders) {
                    g = leaderptr(p);
                    if (height(g) > h) h = height(g);
                    if (depth(g) > d) d = depth(g);
                }
                break;
                /*:656*/

            case KERN_NODE:
            case MATH_NODE: x += width(p); break;

            case LIGATURE_NODE: /*652:*/
                type(ligtrick) = charnodetype;
                font(ligtrick) = font_ligchar(p);
                character(ligtrick) = character_ligchar(p);
                link(ligtrick) = link(p);
                p = ligtrick;
                goto _LN_hpack__reswitch;
                break;
                /*:652*/
        }
        p = link(p);
    }
    /*:651*/
    if (adjusttail != 0) link(adjusttail) = 0;
    height(r) = h;
    depth(r) = d; /*657:*/
    if (m == additional) w += x;
    width(r) = w;
    x = w - x;
    if (x == 0) {
        gluesign(r) = NORMAL;
        glueorder(r) = NORMAL;
        glueset(r) = 0.0;
        goto _Lexit;
    } else if (x > 0) {
        if (totalstretch[FILLL - NORMAL] != 0)
            o = FILLL;
        else if (totalstretch[FILL - NORMAL] != 0)
            o = FILL;
        else if (totalstretch[FIL - NORMAL] != 0)
            o = FIL;
        else {
            o = NORMAL;
            /*:659*/
        }
        glueorder(r) = o;
        gluesign(r) = stretching;
        if (totalstretch[o - NORMAL] != 0)
            glueset(r) = (double)x / totalstretch[o - NORMAL];
        else {
            gluesign(r) = NORMAL;
            glueset(r) = 0.0;
        }
        if (o == NORMAL) {
            if (listptr(r) != 0) { /*660:*/
                lastbadness = badness(x, totalstretch[0]);
                if (lastbadness > hbadness) {
                    println();
                    if (lastbadness > 100)
                        printnl(S(699));
                    else
                        printnl(S(700));
                    print(S(701));
                    print_int(lastbadness);
                    goto _Lcommonending;
                }
            }
            /*:660*/
        }
        goto _Lexit;
    } else {
        if (totalshrink[FILLL - NORMAL] != 0)
            o = FILLL;
        else if (totalshrink[FILL - NORMAL] != 0)
            o = FILL;
        else if (totalshrink[FIL - NORMAL] != 0)
            o = FIL;
        else
            o = NORMAL; /*:665*/
        glueorder(r) = o;
        gluesign(r) = shrinking;
        if (totalshrink[o - NORMAL] != 0)
            glueset(r) = (double)(-x) / totalshrink[o - NORMAL];
        else {
            gluesign(r) = NORMAL;
            glueset(r) = 0.0;
        }
        if ((totalshrink[o - NORMAL] < -x && o == NORMAL) & (listptr(r) != 0)) {
            lastbadness = 1000000L;
            glueset(r) = 1.0;                                    /*666:*/
            if (-x - totalshrink[0] > hfuzz || hbadness < 100) { /*:666*/
                if (overfullrule > 0 && -x - totalshrink[0] > hfuzz) {
                    while (link(q) != 0)
                        q = link(q);
                    link(q) = newrule();
                    width(link(q)) = overfullrule;
                }
                println();
                printnl(S(702));
                print_scaled(-x - totalshrink[0]);
                print(S(703));
                goto _Lcommonending;
            }
        } else if (o == NORMAL) {
            if (listptr(r) != 0) { /*667:*/
                lastbadness = badness(-x, totalshrink[0]);
                if (lastbadness > hbadness) {
                    println();
                    printnl(S(704));
                    print_int(lastbadness);
                    goto _Lcommonending;
                }
            }
            /*:667*/
        }
        goto _Lexit;
    }
_Lcommonending: /*663:*/
    if (outputactive)
        print(S(705));
    else {
        if (packbeginline != 0) {
            if (packbeginline > 0)
                print(S(706));
            else
                print(S(707));
            print_int(labs(packbeginline));
            print(S(708));
        } else
            print(S(709));
        print_int(line);
    }
    println();
    font_in_short_display = NULL_FONT;
    shortdisplay(listptr(r));
    println();
    begin_diagnostic();
    showbox(r); /*:663*/
    end_diagnostic(true);
_Lexit:
    return r;

    /*:657*/
} // #649: hpack

// #668
HalfWord vpackage(HalfWord p, long h, SmallNumber m, long l) {
    Pointer r, g;
    Scaled w, d, x, s;
    GlueOrd o;

    lastbadness = 0;
    r = get_node(boxnodesize);
    type(r) = VLIST_NODE;
    subtype(r) = MIN_QUARTER_WORD;
    shiftamount(r) = 0;
    listptr(r) = p;
    w = 0; /*650:*/
    d = 0;
    x = 0;
    totalstretch[0] = 0;
    totalshrink[0] = 0;
    totalstretch[FIL - NORMAL] = 0;
    totalshrink[FIL - NORMAL] = 0;
    totalstretch[FILL - NORMAL] = 0;
    totalshrink[FILL - NORMAL] = 0;
    totalstretch[FILLL - NORMAL] = 0;
    totalshrink[FILLL - NORMAL] = 0; /*:650*/
    while (p != 0) {                 /*669:*/
        if (ischarnode(p)) {
            confusion(S(710)); // "mathclose"
        } else {
            switch (type(p)) {

                case HLIST_NODE:
                case VLIST_NODE:
                case RULE_NODE:
                case UNSET_NODE: /*670:*/
                    x += d + height(p);
                    d = depth(p);
                    if (type(p) >= RULE_NODE)
                        s = 0;
                    else
                        s = shiftamount(p);
                    if (width(p) + s > w) w = width(p) + s;
                    break;
                    /*:670*/

                case WHATSIT_NODE: /*1359:*/ break;

                /*:1359*/
                case GLUE_NODE: /*:671*/
                    /*671:*/
                    x += d;
                    d = 0;
                    g = glueptr(p);
                    x += width(g);
                    o = stretchorder(g);
                    totalstretch[o - NORMAL] += stretch(g);
                    o = shrinkorder(g);
                    totalshrink[o - NORMAL] += shrink(g);
                    if (subtype(p) >= aleaders) {
                        g = leaderptr(p);
                        if (width(g) > w) w = width(g);
                    }
                    break;

                case KERN_NODE:
                    x += d + width(p);
                    d = 0;
                    break;
            }
        }
        p = link(p);
    }
    /*:669*/
    width(r) = w;
    if (d > l) { /*672:*/
        x += d - l;
        depth(r) = l;
    } else
        depth(r) = d;
    if (m == additional) h += x;
    height(r) = h;
    x = h - x;
    if (x == 0) {
        gluesign(r) = NORMAL;
        glueorder(r) = NORMAL;
        glueset(r) = 0.0;
        goto _Lexit;
    } else if (x > 0) {
        if (totalstretch[FILLL - NORMAL] != 0)
            o = FILLL;
        else if (totalstretch[FILL - NORMAL] != 0)
            o = FILL;
        else if (totalstretch[FIL - NORMAL] != 0)
            o = FIL;
        else {
            o = NORMAL;
            /*:659*/
        }
        glueorder(r) = o;
        gluesign(r) = stretching;
        if (totalstretch[o - NORMAL] != 0)
            glueset(r) = (double)x / totalstretch[o - NORMAL];
        else {
            gluesign(r) = NORMAL;
            glueset(r) = 0.0;
        }
        if (o == NORMAL) {
            if (listptr(r) != 0) { /*674:*/
                lastbadness = badness(x, totalstretch[0]);
                if (lastbadness > vbadness) {
                    println();
                    if (lastbadness > 100)
                        printnl(S(699));
                    else
                        printnl(S(700));
                    print(S(711));
                    print_int(lastbadness);
                    goto _Lcommonending;
                }
            }
            /*:674*/
        }
        goto _Lexit;
    } else {
        if (totalshrink[FILLL - NORMAL] != 0)
            o = FILLL;
        else if (totalshrink[FILL - NORMAL] != 0)
            o = FILL;
        else if (totalshrink[FIL - NORMAL] != 0)
            o = FIL;
        else
            o = NORMAL; /*:665*/
        glueorder(r) = o;
        gluesign(r) = shrinking;
        if (totalshrink[o - NORMAL] != 0)
            glueset(r) = (double)(-x) / totalshrink[o - NORMAL];
        else {
            gluesign(r) = NORMAL;
            glueset(r) = 0.0;
        }
        if ((totalshrink[o - NORMAL] < -x && o == NORMAL) & (listptr(r) != 0)) {
            lastbadness = 1000000L;
            glueset(r) = 1.0;                                    /*677:*/
            if (-x - totalshrink[0] > vfuzz || vbadness < 100) { /*:677*/
                println();
                printnl(S(712));
                print_scaled(-x - totalshrink[0]);
                print(S(713));
                goto _Lcommonending;
            }
        } else if (o == NORMAL) {
            if (listptr(r) != 0) { /*678:*/
                lastbadness = badness(-x, totalshrink[0]);
                if (lastbadness > vbadness) {
                    println();
                    printnl(S(714));
                    print_int(lastbadness);
                    goto _Lcommonending;
                }
            }
            /*:678*/
        }
        goto _Lexit;
    }
_Lcommonending: /*675:*/
    if (outputactive)
        print(S(705));
    else {
        if (packbeginline != 0) {
            print(S(707));
            print_int(labs(packbeginline));
            print(S(708));
        } else
            print(S(709));
        print_int(line);
        println();
    }
    begin_diagnostic();
    showbox(r); /*:675*/
    end_diagnostic(true);
_Lexit:
    return r;

    /*:672*/
} // #668: vpackage

// #679
void appendtovlist(HalfWord b) {
    Scaled d;
    Pointer p;

    if (prevdepth > ignoredepth) {
        d = width(baselineskip) - prevdepth - height(b);
        if (d < lineskiplimit)
            p = newparamglue(LINE_SKIP_CODE);
        else {
            p = newskipparam(BASELINE_SKIP_CODE);
            width(temp_ptr) = d;
        }
        link(tail) = p;
        tail = p;
    }
    link(tail) = b;
    tail = b;
    prevdepth = depth(b);
} // #679: appendtovlist

/** @}*/ // end group S644x679_P239x248