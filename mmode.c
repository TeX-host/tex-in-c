#include "tex.h"
#include "mem.h"
#include "pack.h"
#include "print.h"
#include "texmath.h"
#include "eqtb.h"
#include "macros.h"
#include "mmode.h"


/** @addtogroup S680x698_P249x257
 * @{
 */
/*684:*/
TwoHalves emptyfield;
FourQuarters nulldelimiter;
/*:684*/


/// [#685]
void mmode_init() {
    emptyfield.rh = EMPTY;
    emptyfield.UU.lh = 0;
    nulldelimiter.b0 = 0;
    nulldelimiter.b1 = MIN_QUARTER_WORD;
    nulldelimiter.b2 = 0;
    nulldelimiter.b3 = MIN_QUARTER_WORD;
}

// #686
HalfWord newnoad(void) {
    Pointer p;
    int i = 0;
    p = get_node(noadsize);
    type(p) = ordnoad;
    subtype(p) = NORMAL;
#ifdef BIG_CHARNODE
    for (i = 0; i < CHAR_NODE_SIZE; i++) {
#endif
        mem[nucleus(p) + i - MEM_MIN].hh = emptyfield;
        mem[subscr(p) + i - MEM_MIN].hh = emptyfield;
        mem[supscr(p) + i - MEM_MIN].hh = emptyfield;
#ifdef BIG_CHARNODE
    }
#endif
    return p;
}
/*:686*/

/*688:*/
HalfWord newstyle(SmallNumber s) {
    Pointer p;

    p = get_node(stylenodesize);
    type(p) = stylenode;
    subtype(p) = s;
    width(p) = 0;
    depth(p) = 0;
    return p;
} /*:688*/


/*689:*/
HalfWord newchoice(void) {
    Pointer p;

    p = get_node(stylenodesize);
    type(p) = choicenode;
    subtype(p) = 0;
    displaymlist(p) = 0;
    textmlist(p) = 0;
    scriptmlist(p) = 0;
    scriptscriptmlist(p) = 0;
    return p;
}
/*:689*/


/// [#691] prints family and character.
void printfamandchar(HalfWord p) {
    print_esc(S(333));
    print_int(fam(p));
    print_char(' ');
    print(character(p) - MIN_QUARTER_WORD);
} // [#691] printfamandchar

/// [#691] prints a delimiter as 24-bit hex value.
void printdelimiter(HalfWord p) {
    long a;

    a = smallfam(p) * 256 + smallchar(p) - MIN_QUARTER_WORD;
    a = a * 4096 + largefam(p) * 256 + largechar(p) - MIN_QUARTER_WORD;
    if (a < 0)
        print_int(a);
    else
        print_hex(a);
} // [#691] printdelimiter

/// [#692] display a noad field.
void printsubsidiarydata(HalfWord p, ASCIICode c) {
    if (cur_length() >= depth_threshold) {
        if (mathtype(p) != EMPTY) print(S(334));
        return;
    }
    append_char(c);
    temp_ptr = p;
    switch (mathtype(p)) {
        case mathchar:
            println();
            printcurrentstring();
            printfamandchar(p);
            break;

        case subbox: 
            showinfo(); 
            break;

        case submlist:
            if (info(p) == 0) {
                println();
                printcurrentstring();
                print(S(335));
            } else
                showinfo();
            break;
    }
    flush_char();
} // [#692] printsubsidiarydata

/*693:*/
void showinfo(void) { shownodelist(info(temp_ptr)); }
/*:693*/

/// [#694]
void printstyle(Integer c) {
    switch (c / 2) {
        case 0: print_esc(S(336)); break;
        case 1: print_esc(S(337)); break;
        case 2: print_esc(S(338)); break;
        case 3: print_esc(S(339)); break;

        default: print(S(340)); break;
    }
} // [#694] printstyle

/** @}*/ // end group S680x698_P249x257


/** @addtogroup S699x718_P258x264
 * [ #699. Subroutines for math mode.  ]
 * @{
 */

/*704:*/
HalfWord fractionrule(long t) {
    Pointer p;

    p = newrule();
    height(p) = t;
    depth(p) = 0;
    return p;
}
/*:704*/

/*705:*/
HalfWord overbar(HalfWord b, long k, long t) {
    Pointer p, q;

    p = newkern(k);
    link(p) = b;
    q = fractionrule(t);
    link(q) = p;
    p = newkern(t);
    link(p) = q;
    return (vpack(p, 0, additional));
}
/*:705*/

/*706:*/
/*709:*/
HalfWord charbox(InternalFontNumber f, QuarterWord c) {
    FourQuarters q;
    EightBits hd;
    Pointer b, p;

    q = charinfo(f, c);
    hd = heightdepth(q);
    b = newnullbox();
    width(b) = charwidth(f, q) + charitalic(f, q);
    height(b) = charheight(f, hd);
    depth(b) = chardepth(f, hd);
    p = get_avail();
    character(p) = c;
    font(p) = f;
    listptr(b) = p;
    return b;
}
/*:709*/

/*711:*/
void stackintobox(HalfWord b, InternalFontNumber f, QuarterWord c) {
    Pointer p;

    p = charbox(f, c);
    link(p) = listptr(b);
    listptr(b) = p;
    height(b) = height(p);
}
/*:711*/

/*712:*/
Integer heightplusdepth(InternalFontNumber f, QuarterWord c) {
    FourQuarters q;
    EightBits hd;

    q = charinfo(f, c);
    hd = heightdepth(q);
    return (charheight(f, hd) + chardepth(f, hd));
} /*:712*/


HalfWord vardelimiter(HalfWord d, SmallNumber s, long v) {
    Pointer b;
    InternalFontNumber f, g;
    QuarterWord c = 0 /* XXXX */, x, y;
    long m, n;
    Scaled u, w;
    FourQuarters q;
    EightBits hd;
    FourQuarters r;
    SmallNumber z;
    Boolean largeattempt;

    f = NULL_FONT;
    w = 0;
    largeattempt = false;
    z = smallfam(d);
    x = smallchar(d);
    while (true) {                             /*707:*/
        if (z != 0 || x != MIN_QUARTER_WORD) { /*:707*/
            z += s + 16;
            do {
                z -= 16;
                g = fam_fnt(z);
                if (g != NULL_FONT) { /*708:*/
                    y = x;
                    if (y - MIN_QUARTER_WORD >= fontbc[g] &&
                        y - MIN_QUARTER_WORD <= fontec[g]) {
                    _Llabcontinue:
                        q = charinfo(g, y);
                        if (charexists(q)) {
                            if (chartag(q) == EXT_TAG) {
                                f = g;
                                c = y;
                                goto _Lfound;
                            }
                            hd = heightdepth(q);
                            u = charheight(g, hd) + chardepth(g, hd);
                            if (u > w) {
                                f = g;
                                c = y;
                                w = u;
                                if (u >= v) goto _Lfound;
                            }
                            if (chartag(q) == LIST_TAG) {
                                y = rembyte(q);
                                goto _Llabcontinue;
                            }
                        }
                    }
                }
                /*:708*/
            } while (z >= 16);
        }
        if (largeattempt) goto _Lfound;
        largeattempt = true;
        z = largefam(d);
        x = largechar(d);
    }
_Lfound:
    if (f != NULL_FONT) {             /*710:*/
        if (chartag(q) == EXT_TAG) { /*713:*/
            b = newnullbox();
            type(b) = VLIST_NODE;
            r = exteninfo(f,
                          q); /* fontinfo[extenbase[f ] + rembyte(q)].qqqq; */
                              /*714:*/
            c = extrep(r);
            u = heightplusdepth(f, c);
            w = 0;
            q = charinfo(f, c);
            width(b) = charwidth(f, q) + charitalic(f, q);
            c = extbot(r);
            if (c != MIN_QUARTER_WORD) w += heightplusdepth(f, c);
            c = extmid(r);
            if (c != MIN_QUARTER_WORD) w += heightplusdepth(f, c);
            c = exttop(r);
            if (c != MIN_QUARTER_WORD) w += heightplusdepth(f, c);
            n = 0;
            if (u > 0) {
                while (w < v) { /*:714*/
                    w += u;
                    n++;
                    if (extmid(r) != MIN_QUARTER_WORD) w += u;
                }
            }
            c = extbot(r);
            if (c != MIN_QUARTER_WORD) stackintobox(b, f, c);
            c = extrep(r);
            for (m = 1; m <= n; m++)
                stackintobox(b, f, c);
            c = extmid(r);
            if (c != MIN_QUARTER_WORD) {
                stackintobox(b, f, c);
                c = extrep(r);
                for (m = 1; m <= n; m++)
                    stackintobox(b, f, c);
            }
            c = exttop(r);
            if (c != MIN_QUARTER_WORD) stackintobox(b, f, c);
            depth(b) = w - height(b);
        } else
            b = charbox(f, c); /*:710*/
                               /*:713*/
    } else {
        b = newnullbox();
        width(b) = nulldelimiterspace;
    }
    shiftamount(b) = half(height(b) - depth(b)) - axisheight(s);
    return b;
}
/*:706*/

/*715:*/
HalfWord rebox(HalfWord b, long w) {
    Pointer p;
    InternalFontNumber f;
    Scaled v;

    if ((width(b) != w) & (listptr(b) != 0)) {
        if (type(b) == VLIST_NODE) b = hpack(b, 0, additional);
        p = listptr(b);
        if (ischarnode(p) & (link(p) == 0)) {
            f = font(p);
            v = charwidth(f, charinfo(f, character(p)));
            if (v != width(b)) link(p) = newkern(width(b) - v);
        }
        free_node(b, boxnodesize);
        b = newglue(ssglue);
        link(b) = p;
        while (link(p) != 0)
            p = link(p);
        link(p) = newglue(ssglue);
        return (hpack(b, w, exactly));
    } else {
        width(b) = w;
        return b;
    }
}
/*:715*/

// #716: 
HalfWord mathglue(HalfWord g, long m) {
    Pointer p;
    long n;
    Scaled f;
    #define MU_MULT(yyy) nx_plus_y(n, yyy, xn_over_d(yyy, f, 65536L))

    n = x_over_n(m, 65536L);
    f = tex_remainder;
    if (f < 0) {
        n--;
        f += 65536L;
    }
    p = get_node(gluespecsize);
    width(p) = MU_MULT(width(g));

    stretchorder(p) = stretchorder(g);
    if (stretchorder(p) == NORMAL) {
        stretch(p) = MU_MULT(stretch(g));
    } else {
        stretch(p) = stretch(g);
    }

    shrinkorder(p) = shrinkorder(g);
    if (shrinkorder(p) == NORMAL) {
        shrink(p) = MU_MULT(shrink(g));
    } else {
        shrink(p) = shrink(g);
    }

    return p;
} // #716: mathglue

/*717:*/
void mathkern(HalfWord p, long m) {
    long n;
    Scaled f;

    if (subtype(p) != muglue) return;
    n = x_over_n(m, 65536L);
    f = tex_remainder;
    if (f < 0) {
        n--;
        f += 65536L;
    }
    width(p) = nx_plus_y(n, width(p), xn_over_d(width(p), f, 65536L));
    subtype(p) = explicit;
}
/*:717*/

/*718:*/
void flushmath(void) {
    flush_node_list(link(head));
    flush_node_list(incompleatnoad);
    link(head) = 0;
    tail = head;
    incompleatnoad = 0;
}
/*:718*/

/** @}*/ // end group S699x718_P258x264