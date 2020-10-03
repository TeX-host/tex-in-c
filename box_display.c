#include <math.h> // fabs
#include "tex_constant.h" // [const] MEM_MIN, MAX_PRINT_LINE, FONT_MAX, MIN_QUARTER_WORD
// #include "texmath.h" // [macro] UNITY; [func] print_scaled
#include "print.h"   // [func] print_esc, print
#include "hash.h"    // [func] fontidtext
#include "mmode.h"   // [const] defaultcode
#include "io.h" // print_file_name
#include "extension.h" // [const] opennode, [macro] openname. [func] print_write_whatsit
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


/// [#174] prints highlights of list p.
void shortdisplay(Pointer p) {
    Integer n;

    while (p > MEM_MIN) {
        if (ischarnode(p)) {
            if (p <= mem_end) {
                if (font(p) != font_in_short_display) {
                    /// TODO: why ?
                    if (/* (font(p) < 0 ) || */ (font(p) > FONT_MAX)) {
                        print_char('*');
                    } else {
                        /// [#267] Print the font identifier for font(p)
                        print_esc(fontidtext(font(p)));
                    }
                    print_char(' ');
                    font_in_short_display = font(p);
                }
                print(character(p) - MIN_QUARTER_WORD);
            }
        } else {
            /// [#175] Print a short indication of the contents of node.
            switch (type(p)) {
                case HLIST_NODE:
                case VLIST_NODE:
                case INS_NODE:
                case WHATSIT_NODE:
                case MARK_NODE:
                case ADJUST_NODE:
                case UNSET_NODE:
                    print(S(328)); // "[]"
                    break;

                case RULE_NODE: 
                    print_char('|'); 
                    break;

                case GLUE_NODE:
                    if (glueptr(p) != zeroglue) print_char(' ');
                    break;

                case MATH_NODE: 
                    print_char('$'); 
                    break;

                case LIGATURE_NODE: 
                    shortdisplay(ligptr(p)); 
                    break;

                case DISC_NODE:
                    shortdisplay(prebreak(p));
                    shortdisplay(postbreak(p));
                    n = replacecount(p);
                    while (n > 0) {
                        if (link(p) != 0) p = link(p);
                        n--;
                    }
                    break;

                default:
                    /* do nothing */
                    break;
            } // switch (type(p))
        } // if (ischarnode(p)) - else
        p = link(p);
    } // while (p > MEM_MIN)
} /* shortdisplay */

/// [#176] prints char node data.
void printfontandchar(Pointer p) {
    if (p > mem_end) {
        print_esc(S(308)); // "CLOBBERED."
        return;
    }

    if ((font(p) > FONT_MAX)) {
        print_char('*');
    } else {
        /// [#267] Print the font identifier for font(p)
        print_esc(fontidtext(font(p)));
    }
    print_char(' ');
    print(character(p) - MIN_QUARTER_WORD);
} /* printfontandchar */

/// [#176] prints token list data in braces.
void printmark(Integer p) {
    print_char('{');
    if (p < hi_mem_min || p > mem_end){
        print_esc(S(308)); // "CLOBBERED."
    } else {
        showtokenlist(link(p), 0, MAX_PRINT_LINE - 10);
    }
    print_char('}');
} /* printmark */

/// [#176] prints dimension in rule node.
void printruledimen(Scaled d) {
    if (isrunning(d)) {
        print_char('*');
    } else {
        print_scaled(d);
    }
} /* printruledimen */

/// [#177] prints a glue component.
void printglue(Scaled d, Integer order, StrNumber s) {
    print_scaled(d);
    if (order > FILLL) {
        print(S(329)); // "foul"
        return;
    }
    if (order <= NORMAL) {
        if (s != 0) print(s);
        return;
    }

    print(S(330)); // "fil"
    while (order > FIL) {
        print_char('l');
        order--;
    }
} /* printglue */

/// [#178] prints a glue specification.
void printspec(long p, StrNumber s) {
    if (/* p < MEM_MIN || */ p >= lo_mem_max) {
        print_char('*');
        return;
    }

    print_scaled(width(p));
    if (s != 0) print(s);
    if (stretch(p) != 0) {
        print(S(331)); // " plus "
        printglue(stretch(p), stretchorder(p), s);
    }
    if (shrink(p) != 0) {
        print(S(332)); // " minus "
        printglue(shrink(p), shrinkorder(p), s);
    }
} /* printspec */

/** [#182] prints a node list symbolically.
 *
 */
void shownodelist(Integer p) {
    Integer n; // the number of items already printed at this level
    Real g;    // a glue ratio, as a floating point number

    if (cur_length() > depth_threshold) {
        // indicate that there’s been some truncation
        if (p > null) print(S(334)); // " []"
        return;
    }

    n = 0;
    while (p > MEM_MIN) {
        println();
        // display the nesting history
        printcurrentstring();
        if (p > mem_end) { // pointer out of range
            print(S(360)); // "Bad link, display aborted."
            return;
        }
        n++;
        if (n > breadth_max) { // time to stop
            print(S(361)); // "etc."
            return;
        }

        /// [#183] Display node p.
        if (ischarnode(p)) {
            printfontandchar(p);
        } else {
            switch (type(p)) {
                /// [#184] Display box p.
                case HLIST_NODE:
                case VLIST_NODE:
                case UNSET_NODE:
                    if (type(p) == HLIST_NODE) {
                        print_esc('h');
                    } else if (type(p) == VLIST_NODE) {
                        print_esc('v');
                    } else {
                        print_esc(S(362)); // "unset"
                    }
                    print(S(363)); // "box("
                    print_scaled(height(p));
                    print_char('+');
                    print_scaled(depth(p));
                    print(S(364)); // ")x"
                    print_scaled(width(p));
                    if (type(p) == UNSET_NODE) {
                        /// [#185] Display special fields of the unset node p.
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
                    } else {
                        /// [#186] Display the value of glue_set(p).
                        g = glueset(p);
                        if ((g != 0.0) && (gluesign(p) != NORMAL)) {
                            print(S(368)); // ", glue set "
                            if (gluesign(p) == shrinking) print(S(369)); // "− "
                            if (0 /*labs(mem[p + glueoffset - MEM_MIN].int_) < 1048576L */) {
                                print(S(370)); // "?.?"
                            } else {
                                if (fabs(g) > 20000.0) {
                                    if (g > 0.0) {
                                        print_char('>');
                                    } else {
                                        print(S(371)); // "< −"
                                    }
                                    printglue(UNITY * 20000, glueorder(p), 0);
                                } else {
                                    printglue((long)floor(UNITY * g + 0.5),
                                              glueorder(p), 0);
                                }
                            }
                        }
                        if (shiftamount(p) != 0) {
                            print(S(372)); // ", shifted "
                            print_scaled(shiftamount(p));
                        }
                    }
                    nodelistdisplay(listptr(p)); // recursive call
                    break;

                /// [#187] Display rule p.
                case RULE_NODE:
                    print_esc(S(373)); // "rule("
                    printruledimen(height(p));
                    print_char('+');
                    printruledimen(depth(p));
                    print(S(364)); // ")x"
                    printruledimen(width(p));
                    break;

                /// [#188] Display insertion p.
                case INS_NODE:
                    print_esc(S(374)); // "insert"
                    print_int(subtype(p) - MIN_QUARTER_WORD);
                    print(S(375)); // ", natural size "
                    print_scaled(height(p));
                    print(S(376)); // "; split("
                    printspec(splittopptr(p), 0);
                    print_char(',');
                    print_scaled(depth(p));
                    print(S(377)); // "); float cost "
                    print_int(floatcost(p));
                    nodelistdisplay(insptr(p)); // recursive call
                    break;

                /// [#1356] Display the whatsit node p.
                case WHATSIT_NODE:
                    switch (subtype(p)) {
                        case opennode:
                            print_write_whatsit(S(378), p); // "openout"
                            print_char('=');
                            print_file_name(openname(p), openarea(p),
                                            openext(p));
                            break;

                        case writenode:
                            print_write_whatsit(S(379), p); // "write"
                            printmark(writetokens(p));
                            break;

                        case closenode:
                            print_write_whatsit(S(380), p); // "closeout"
                            break;

                        case specialnode:
                            print_esc(S(381)); // "special"
                            printmark(writetokens(p));
                            break;

                        case languagenode:
                            print_esc(S(382)); // "setlanguage"
                            print_int(whatlang(p));
                            print(S(383)); // " (hyphenmin "
                            print_int(whatlhm(p));
                            print_char(',');
                            print_int(whatrhm(p));
                            print_char(')');
                            break;

                        default:
                            print(S(384)); // "whatsit?"
                            break;
                    } // switch (subtype(p))
                    break;

                /// [#189] Display glue p.
                case GLUE_NODE:
                    if (subtype(p) >= aleaders) {
                        /// [#190] Display leaders p.
                        print_esc(S(385)); // ""
                        if (subtype(p) == cleaders) {
                            print_char('c');
                        } else if (subtype(p) == xleaders){ 
                            print_char('x');
                        }
                        print(S(386)); // "leaders "
                        printspec(glueptr(p), 0);
                        nodelistdisplay(leaderptr(p)); // recursive call
                    } else {
                        print_esc(S(387)); // "glue"
                        if (subtype(p) != NORMAL) {
                            print_char('(');
                            if (subtype(p) < condmathglue) {
                                print_skip_param(subtype(p) - 1);
                            } else {
                                if (subtype(p) == condmathglue) {
                                    print_esc(S(388)); // "nonscript"
                                } else {
                                    print_esc(S(389)); // "mskip"
                                }
                            }
                            print_char(')');
                        }

                        if (subtype(p) != condmathglue) {
                            print_char(' ');
                            if (subtype(p) < condmathglue) {
                                printspec(glueptr(p), 0);
                            } else {
                                printspec(glueptr(p), S(390)); // "mu"
                            }
                        }
                    } // if (subtype(p) <=> aleaders)
                    break;

                /// [#191] Display kern p.
                case KERN_NODE:
                    if (subtype(p) != muglue) {
                        print_esc(S(391)); // "kern"
                        if (subtype(p) != NORMAL) print_char(' ');
                        print_scaled(width(p));
                        if (subtype(p) == acckern) {
                            print(S(392)); // " (for accent)"
                        }
                    } else {
                        print_esc(S(393)); // "mkern"
                        print_scaled(width(p));
                        print(S(390)); // "mu"
                    }
                    break;

                /// [#193] Display math node p.
                case MATH_NODE:
                    print_esc(S(394)); // "math"
                    if (subtype(p) == before) {
                        print(S(395)); // "on"
                    } else {
                        print(S(396)); // "off"
                    }
                    if (width(p) != 0) {
                        print(S(397)); // ", surrounded "
                        print_scaled(width(p));
                    }
                    break;

                /// [#193] Display ligature p.
                case LIGATURE_NODE:
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
                    print(S(398)); // " (ligature "
                    if (subtype(p) > 1) print_char('|');
                    font_in_short_display = font_ligchar(p);
                    shortdisplay(ligptr(p));
                    if ((subtype(p)) % 2) print_char('|');
                    print_char(')');
                    break;

                /// [#194] Display penalty p.
                case PENALTY_NODE:
                    print_esc(S(399)); // "penalty "
                    print_int(penalty(p));
                    break;

                /// [#195] Display discretionary p.
                case DISC_NODE:
                    print_esc(S(400)); // "discretionary"
                    if (replacecount(p) > 0) {
                        print(S(401)); // " replacing "
                        print_int(replacecount(p));
                    }
                    nodelistdisplay(prebreak(p));
                    append_char('|');
                    shownodelist(postbreak(p));
                    flush_char();
                    break;

                /// [#196] Display mark p.
                case MARK_NODE:
                    print_esc(S(402)); // "mark"
                    printmark(markptr(p));
                    break;

                /// [#197] Display adjustment p.
                case ADJUST_NODE:
                    print_esc(S(403)); // "vadjust"
                    nodelistdisplay(adjustptr(p));
                    break;

                /** [#690] Cases of show node list that arise in mlists only: */
                case stylenode: 
                    printstyle(subtype(p)); 
                    break;

                /// [#695] Display choice node p.
                case choicenode:
                    print_esc(S(404)); // "mathchoice"
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

                /// [#696] Display normal noad p.
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
                case rightnoad:
                    /// [#696] Display normal noad p.
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
                    } // switch (type(p))

                    if (subtype(p) != NORMAL) {
                        if (subtype(p) == limits) {
                            print_esc(S(420)); // "limits"
                        } else {
                            print_esc(S(421)); // "nolimits"
                        }
                    }
                    if (type(p) < leftnoad) {
                        printsubsidiarydata(nucleus(p), '.');
                    }
                    printsubsidiarydata(supscr(p), '^');
                    printsubsidiarydata(subscr(p), '_');
                    break;

                /// [#697] Display fraction noad p.
                case fractionnoad:
                    print_esc(S(422)); // "fraction, thickness "
                    if (thickness(p) == defaultcode) {
                        print(S(423)); // "= default"
                    } else {
                        print_scaled(thickness(p));
                    }
                    if (    (smallfam(leftdelimiter(p)) != 0) 
                        || (smallchar(leftdelimiter(p)) != MIN_QUARTER_WORD) 
                        ||  (largefam(leftdelimiter(p)) != 0) 
                        || (largechar(leftdelimiter(p)) != MIN_QUARTER_WORD)) {
                        print(S(424)); // ", left−delimiter "
                        printdelimiter(leftdelimiter(p));
                    }
                    if (    (smallfam(rightdelimiter(p)) != 0) 
                        || (smallchar(rightdelimiter(p)) != MIN_QUARTER_WORD) 
                        ||  (largefam(rightdelimiter(p)) != 0) 
                        || (largechar(rightdelimiter(p)) != MIN_QUARTER_WORD)) {
                        print(S(425)); // ", right−delimiter "
                        printdelimiter(rightdelimiter(p));
                    }
                    printsubsidiarydata(numerator(p), '\\');
                    printsubsidiarydata(denominator(p), '/');
                    break;

                default:
                    print(S(426)); // "Unknown node type!"
                    break;
            } // switch (type(p))
        } // if (ischarnode(p)) - else
        p = link(p);
    } // while (p > MEM_MIN)
} /* shownodelist */

/// [#198] 
void showbox(HalfWord p) {
    /// [#236]
    depth_threshold = showboxdepth;
    breadth_max = showboxbreadth;

    if (breadth_max <= 0) breadth_max = 5;

    #if 0
    if (pool_ptr + depth_threshold >= POOL_SIZE)
        depth_threshold = POOL_SIZE - pool_ptr - 1;
    #else
        depth_threshold = str_adjust_to_room(depth_threshold);
    #endif
    // now there’s enough room for prefix string

    shownodelist(p); // the show starts at p
    println();
} /* showbox */

/** @}*/ // end group S173x198_P62x68