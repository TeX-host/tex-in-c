#include <math.h> // fabs
#include "tex_types.h"
#include "tex_constant.h"
#include "mem.h" // [var] mem
#include "texfunc.h" // [func] print_char, printsubsidiarydata,
    // printdelimiter, printfamandchar
#include "texmath.h"  // [func] print_scaled
#include "print.h"   // [func] print_esc, print
#include "hash.h"    // [func] fontidtext
#include "box.h"


/** @addtogroup S173x198_P62x68
 * @{
 */

/// [#173] an internal font number.
Integer font_in_short_display;
/// [#181] maximum nesting depth in box displays.
Integer depth_threshold;
/// [#181] maximum number of items shown at the same list level.
Integer breadth_max;


/*174:*/
void shortdisplay(Pointer p) {
    long n;

    while (p > MEM_MIN) {
        if (ischarnode(p)) {
            if (p <= mem_end) {
                if (font(p) != font_in_short_display) {
                    if (/* (font(p) < 0 ) | */ (font(p) > FONT_MAX))
                        print_char('*');
                    else /*267:*/
                         /*:267*/
                        print_esc(fontidtext(font(p)));
                    print_char(' ');
                    font_in_short_display = font(p);
                }
                print(character(p) - MIN_QUARTER_WORD);
            }
        } else {               /*175:*/
            switch (type(p)) { /*:175*/

                case HLIST_NODE:
                case VLIST_NODE:
                case INS_NODE:
                case WHATSIT_NODE:
                case MARK_NODE:
                case ADJUST_NODE:
                case UNSET_NODE: print(S(328)); break;

                case RULE_NODE: print_char('|'); break;

                case GLUE_NODE:
                    if (glueptr(p) != zeroglue) print_char(' ');
                    break;

                case MATH_NODE: print_char('$'); break;

                case LIGATURE_NODE: shortdisplay(ligptr(p)); break;

                case DISC_NODE:
                    shortdisplay(prebreak(p));
                    shortdisplay(postbreak(p));
                    n = replacecount(p);
                    while (n > 0) {
                        if (link(p) != 0) p = link(p);
                        n--;
                    }
                    break;
            }
        }
        p = link(p);
    }
}
/*:174*/

/*176:*/
void printfontandchar(Pointer p) {
    if (p > mem_end) {
        print_esc(S(308));
        return;
    }
    if ((font(p) > FONT_MAX))
        print_char('*');
    else /*267:*/
         /*:267*/
        print_esc(fontidtext(font(p)));
    print_char(' ');
    print(character(p) - MIN_QUARTER_WORD);
}


void printmark(long p) {
    print_char('{');
    if (p < hi_mem_min || p > mem_end)
        print_esc(S(308));
    else
        showtokenlist(link(p), 0, MAX_PRINT_LINE - 10);
    print_char('}');
}


void printruledimen(long d) {
    if (isrunning(d)) {
        print_char('*');
    } else
        print_scaled(d);
}
/*:176*/

/*177:*/
void printglue(long d, long order, StrNumber s) {
    print_scaled(d);
    if ((unsigned long)order > FILLL) {
        print(S(329));
        return;
    }
    if (order <= NORMAL) {
        if (s != 0) print(s);
        return;
    }
    print(S(330));
    while (order > FIL) {
        print_char('l');
        order--;
    }
}
/*:177*/

/*178:*/
void printspec(long p, StrNumber s) {
    if (p >= lo_mem_max) {
        print_char('*');
        return;
    }
    print_scaled(width(p));
    if (s != 0) print(s);
    if (stretch(p) != 0) {
        print(S(331));
        printglue(stretch(p), stretchorder(p), s);
    }
    if (shrink(p) != 0) {
        print(S(332));
        printglue(shrink(p), shrinkorder(p), s);
    }
}
/*:178*/

/*182:*/
void shownodelist(long p) {
    long n;
    double g;

    if (cur_length() > depth_threshold) {
        if (p > 0) print(S(334));
        goto _Lexit;
    }
    n = 0;
    while (p > MEM_MIN) {
        println();
        printcurrentstring();
        if (p > mem_end) {
            print(S(360));
            goto _Lexit;
        }
        n++;
        if (n > breadth_max) {
            print(S(361));
            goto _Lexit;
        } /*183:*/
        if (ischarnode(p))
            printfontandchar(p);
        else {
            switch (type(p)) { /*:183*/
                case HLIST_NODE:
                case VLIST_NODE:
                case UNSET_NODE: /*184:*/
                    if (type(p) == HLIST_NODE)
                        print_esc('h');
                    else if (type(p) == VLIST_NODE)
                        print_esc('v');
                    else
                        print_esc(S(362));
                    print(S(363));
                    print_scaled(height(p));
                    print_char('+');
                    print_scaled(depth(p));
                    print(S(364));
                    print_scaled(width(p));
                    if (type(p) == UNSET_NODE) { /*185:*/
                        if (spancount(p) != MIN_QUARTER_WORD) {
                            print(S(303));
                            print_int(spancount(p) - MIN_QUARTER_WORD + 1);
                            print(S(365));
                        }
                        if (gluestretch(p) != 0) {
                            print(S(366));
                            printglue(gluestretch(p), glueorder(p), 0);
                        }
                        if (glueshrink(p) != 0) {
                            print(S(367));
                            printglue(glueshrink(p), gluesign(p), 0);
                        }
                    }      /*:185*/
                    else { /*186:*/
                        g = glueset(p);
                        if ((g != 0.0) & (gluesign(p) != NORMAL)) { /*:186*/
                            print(S(368));
                            if (gluesign(p) == shrinking) print(S(369));
                            if (0 /*labs(mem[p + glueoffset - MEM_MIN].int_) < 1048576L */)
                                print(S(370));
                            else {
                                if (fabs(g) > 20000.0) {
                                    if (g > 0.0)
                                        print_char('>');
                                    else
                                        print(S(371));
                                    printglue(UNITY * 20000, glueorder(p), 0);
                                } else
                                    printglue((long)floor(UNITY * g + 0.5),
                                              glueorder(p),
                                              0);
                            }
                        }
                        if (shiftamount(p) != 0) {
                            print(S(372));
                            print_scaled(shiftamount(p));
                        }
                    }
                    nodelistdisplay(listptr(p));
                    break;
                    /*:184*/

                case RULE_NODE: /*187:*/
                    print_esc(S(373));
                    printruledimen(height(p));
                    print_char('+');
                    printruledimen(depth(p));
                    print(S(364));
                    printruledimen(width(p));
                    break;
                    /*:187*/

                case INS_NODE: /*188:*/
                    print_esc(S(374));
                    print_int(subtype(p) - MIN_QUARTER_WORD);
                    print(S(375));
                    print_scaled(height(p));
                    print(S(376));
                    printspec(splittopptr(p), 0);
                    print_char(',');
                    print_scaled(depth(p));
                    print(S(377));
                    print_int(floatcost(p));
                    nodelistdisplay(insptr(p));
                    break;
                    /*:188*/

                case WHATSIT_NODE:        /*1356:*/
                    switch (subtype(p)) { /*:1356*/
                        case opennode:
                            print_write_whatsit(S(378), p);
                            print_char('=');
                            print_file_name(
                                openname(p), openarea(p), openext(p));
                            break;

                        case writenode:
                            print_write_whatsit(S(379), p);
                            printmark(writetokens(p));
                            break;

                        case closenode: print_write_whatsit(S(380), p); break;

                        case specialnode:
                            print_esc(S(381));
                            printmark(writetokens(p));
                            break;

                        case languagenode:
                            print_esc(S(382));
                            print_int(whatlang(p));
                            print(S(383));
                            print_int(whatlhm(p));
                            print_char(',');
                            print_int(whatrhm(p));
                            print_char(')');
                            break;

                        default: print(S(384)); break;
                    }
                    break;

                case GLUE_NODE:                   /*189:*/
                    if (subtype(p) >= aleaders) { /*190:*/
                        print_esc(S(385));
                        if (subtype(p) == cleaders)
                            print_char('c');
                        else if (subtype(p) == xleaders)
                            print_char('x');
                        print(S(386));
                        printspec(glueptr(p), 0);
                        nodelistdisplay(leaderptr(p));
                    }      /*:190*/
                    else { /*:189*/
                        print_esc(S(387));
                        if (subtype(p) != NORMAL) {
                            print_char('(');
                            if (subtype(p) < condmathglue)
                                print_skip_param(subtype(p) - 1);
                            else {
                                if (subtype(p) == condmathglue)
                                    print_esc(S(388));
                                else
                                    print_esc(S(389));
                            }
                            print_char(')');
                        }
                        if (subtype(p) != condmathglue) {
                            print_char(' ');
                            if (subtype(p) < condmathglue)
                                printspec(glueptr(p), 0);
                            else
                                printspec(glueptr(p), S(390));
                        }
                    }
                    break;

                case KERN_NODE: /*191:*/
                    if (subtype(p) != muglue) {
                        print_esc(S(391));
                        if (subtype(p) != NORMAL) print_char(' ');
                        print_scaled(width(p));
                        if (subtype(p) == acckern) print(S(392));
                    } else { /*:191*/
                        print_esc(S(393));
                        print_scaled(width(p));
                        print(S(390));
                    }
                    break;

                case MATH_NODE: /*192:*/
                    print_esc(S(394));
                    if (subtype(p) == before)
                        print(S(395));
                    else
                        print(S(396));
                    if (width(p) != 0) {
                        print(S(397));
                        print_scaled(width(p));
                    }
                    break;
                    /*:192*/

                case LIGATURE_NODE: /*193:*/
#ifdef BIG_CHARNODE
                {
                    Pointer pp = get_avail();
                    type(pp) = charnodetype;
                    font(pp) = font_ligchar(p);
                    character(pp) = character_ligchar(p);
                    printfontandchar(pp);
                    FREE_AVAIL(pp);
                }
#else
                    printfontandchar(ligchar(p));
#endif
                    print(S(398));
                    if (subtype(p) > 1) print_char('|');
                    font_in_short_display = font_ligchar(p);
                    shortdisplay(ligptr(p));
                    if ((subtype(p)) & 1) print_char('|');
                    print_char(')');
                    break;
                    /*:193*/

                case PENALTY_NODE: /*194:*/
                    print_esc(S(399));
                    print_int(penalty(p));
                    break;
                    /*:194*/

                case DISC_NODE: /*195:*/
                    print_esc(S(400));
                    if (replacecount(p) > 0) {
                        print(S(401));
                        print_int(replacecount(p));
                    }
                    nodelistdisplay(prebreak(p));
                    append_char('|');
                    shownodelist(postbreak(p));
                    flush_char();
                    break;
                    /*:195*/

                case MARK_NODE: /*196:*/
                    print_esc(S(402));
                    printmark(markptr(p));
                    break;
                    /*:196*/

                case ADJUST_NODE: /*197:*/
                    print_esc(S(403));
                    nodelistdisplay(adjustptr(p));
                    break;
                    /*:197*/
                    /*690:*/

                case stylenode: printstyle(subtype(p)); break;

                case choicenode: /*695:*/
                    print_esc(S(404));
                    append_char('D');
                    shownodelist(displaymlist(p));
                    flush_char();
                    append_char('T');
                    shownodelist(textmlist(p));
                    flush_char();
                    append_char('S');
                    shownodelist(scriptmlist(p));
                    flush_char();
                    append_char('s');
                    shownodelist(scriptscriptmlist(p));
                    flush_char();
                    break;
                    /*:695*/

                case ordnoad:
                case opnoad:
                case binnoad:
                case relnoad:
                case opennoad:
                case closenoad:
                case punctnoad:
                case innernoad:
                case radicalnoad:
                case overnoad:
                case undernoad:
                case vcenternoad:
                case accentnoad:
                case leftnoad:
                case rightnoad: /*696:*/
                    switch (type(p)) {

                        case ordnoad: print_esc(S(405)); break;

                        case opnoad: print_esc(S(406)); break;

                        case binnoad: print_esc(S(407)); break;

                        case relnoad: print_esc(S(408)); break;

                        case opennoad: print_esc(S(409)); break;

                        case closenoad: print_esc(S(410)); break;

                        case punctnoad: print_esc(S(411)); break;

                        case innernoad: print_esc(S(412)); break;

                        case overnoad: print_esc(S(413)); break;

                        case undernoad: print_esc(S(414)); break;

                        case vcenternoad: print_esc(S(415)); break;

                        case radicalnoad:
                            print_esc(S(416));
                            printdelimiter(leftdelimiter(p));
                            break;

                        case accentnoad:
                            print_esc(S(417));
                            printfamandchar(accentchr(p));
                            break;

                        case leftnoad:
                            print_esc(S(418));
                            printdelimiter(nucleus(p));
                            break;

                        case rightnoad:
                            print_esc(S(419));
                            printdelimiter(nucleus(p));
                            break;
                    }
                    if (subtype(p) != NORMAL) {
                        if (subtype(p) == limits)
                            print_esc(S(420));
                        else
                            print_esc(S(421));
                    }
                    if (type(p) < leftnoad)
                        printsubsidiarydata(nucleus(p), '.');
                    printsubsidiarydata(supscr(p), '^');
                    printsubsidiarydata(subscr(p), '_');
                    break;
                    /*:696*/

                case fractionnoad: /*697:*/
                    print_esc(S(422));
                    if (thickness(p) == defaultcode)
                        print(S(423));
                    else
                        print_scaled(thickness(p));
                    if ((smallfam(leftdelimiter(p)) != 0) |
                        (smallchar(leftdelimiter(p)) != MIN_QUARTER_WORD) |
                        (largefam(leftdelimiter(p)) != 0) |
                        (largechar(leftdelimiter(p)) != MIN_QUARTER_WORD)) {
                        print(S(424));
                        printdelimiter(leftdelimiter(p));
                    }
                    if ((smallfam(rightdelimiter(p)) != 0) |
                        (smallchar(rightdelimiter(p)) != MIN_QUARTER_WORD) |
                        (largefam(rightdelimiter(p)) != 0) |
                        (largechar(rightdelimiter(p)) != MIN_QUARTER_WORD)) {
                        print(S(425));
                        printdelimiter(rightdelimiter(p));
                    }
                    printsubsidiarydata(numerator(p), '\\');
                    printsubsidiarydata(denominator(p), '/');
                    break;
                    /*:697*/
                    /*:690*/

                default: print(S(426)); break;
            }
        }
        p = link(p);
    }
_Lexit:;
}
/*:182*/

/*198:*/
void showbox(HalfWord p) { /*236:*/
    depth_threshold = showboxdepth;
    breadth_max = showboxbreadth; /*:236*/
    if (breadth_max <= 0) breadth_max = 5;
#if 0
  if (pool_ptr + depth_threshold >= POOL_SIZE)
    depth_threshold = POOL_SIZE - pool_ptr - 1;
#else
    depth_threshold = str_adjust_to_room(depth_threshold);
#endif
    shownodelist(p);
    println();
}
/*:198*/
/** @}*/ // end group S173x198_P62x68