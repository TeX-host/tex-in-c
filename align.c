#include <math.h> // floor
#include "tex_constant.h" // [const] MEM_MIN, MIN_QUARTER_WORD, MAX_QUARTER_WORD
// [enum] CatCode, TexCommandCode
#include "mem.h"
#include "dviout.h"
#include "scan.h" // [func] skip_spaces
#include "print.h"
#include "pack.h"
#include "mmode.h"
#include "error.h"
#include "expand.h"
#include "main_ctrl.h"
#include "lexer.h" // [macro] ignoredepth
#include "align.h"


/** @addtogroup S768x812_P285x301
 * [ #768. Alignment. ]
 * @{
 */
/// [#770] current position in preamble list.
Pointer curalign;
/// [#770] start of currently spanned columns in preamble list.
Pointer curspan;
/// [#770] place to copy when extending a periodic preamble.
Pointer curloop;
/// [#770] most recently pushed-down alignment stack node.
Pointer alignptr;
/// [#770] adjustment list pointers.
Pointer curhead, curtail;


/// [#771]
void align_init() {
    alignptr = null;
    curalign = null;
    curspan = null;
    curloop = null;
    curhead = null;
    curtail = null;
} /* align_init */

long tex_round(double d) { return (long)(floor(d + 0.5)); }

/*772:*/
void pushalignment(void) {
    Pointer p;

    p = get_node(alignstacknodesize);
    link(p) = alignptr;
    info(p) = curalign;
    llink(p) = preamble;
    rlink(p) = curspan;
    mem[p - MEM_MIN + 2].int_ = curloop;
    mem[p - MEM_MIN + 3].int_ = align_state;
    info(p + 4) = curhead;
    link(p + 4) = curtail;
    alignptr = p;
    curhead = get_avail();
}


void popalignment(void) {
    Pointer p;

    FREE_AVAIL(curhead);
    p = alignptr;
    curtail = link(p + 4);
    curhead = info(p + 4);
    align_state = mem[p - MEM_MIN + 3].int_;
    curloop = mem[p - MEM_MIN + 2].int_;
    curspan = rlink(p);
    preamble = llink(p);
    curalign = info(p);
    alignptr = link(p);
    free_node(p, alignstacknodesize);
}
/*:772*/

/*774:*/
/*782:*/
void getpreambletoken(void) {
_Lrestart:
    gettoken();
    while (curchr == spancode && curcmd == TAB_MARK) {
        gettoken();
        if (curcmd > MAX_COMMAND) {
            expand();
            gettoken();
        }
    }
    // "(interwoven alignment preambles are not allowed)"
    if (curcmd == ENDV) fatalerror(S(509));
    if (curcmd != ASSIGN_GLUE || curchr != GLUE_BASE + TAB_SKIP_CODE) return;
    scan_optional_equals();
    scan_glue(GLUE_VAL);
    if (globaldefs > 0)
        geq_define(GLUE_BASE + TAB_SKIP_CODE, GLUE_REF, cur_val);
    else
        eq_define(GLUE_BASE + TAB_SKIP_CODE, GLUE_REF, cur_val);
    goto _Lrestart;
}
/*:782*/

/// [#774] When \\halign or \\valign has been scanned,
///     get everything off to a good start.
///
/// This mostly involves scanning the preamble and putting its
///     information into the preamble list.
void initalign(void) {
    Pointer savecsptr; ///< `warning_index` value for error messages.
    Pointer p;         ///< for short-term temporary use.

    // \halign or \valign, usually
    savecsptr = curcs; 
    // enter a new alignment level
    pushalignment();
    align_state = -1000000L;

    // [#776] Check for improper alignment in displayed math.
    // When \halign is used as a displayed formula, there should be no other
    // pieces of mlists present.
    if (mode == M_MODE && (tail != head || incompleatnoad != 0)) {
        print_err(S(597)); // "Improper "
        print_esc(S(724)); // "halign"
        print(S(725));     // " inside $$'s"
        /*
         * "Displays can use special alignments (like \\eqalignno)"
         * "only if nothing but the alignment itself is between $$'s."
         * "So I've deleted the formulas that preceded this alignment."
         */
        help3(S(726), S(727), S(728));
        error();
        flushmath();
    } // [#776]

    // enter a new semantic level
    pushnest();

    // [#775] Change current mode to:
    //  + `-vmode` for `\halign`
    //  + `-hmode` for `\valign`
    if (mode == M_MODE) {
        mode = -V_MODE;
        prevdepth = nest[nest_ptr - 2].auxfield.sc;
    } else if (mode > 0) {
        mode = -mode;
    } /* [#775] if (mode <=> M_MODE) */

    scanspec(aligngroup, false);

    // [#777] Scan the preamble and record it in the `preamble` list.
    preamble = 0;
    curalign = alignhead;
    curloop = 0;
    scanner_status = ALIGNING;
    // at this point, `cur_cmd = left_brace`
    warning_index = savecsptr;
    align_state = -1000000L;

    while (true) {
        // [#778] Append the current tabskip glue to the preamble list
        link(curalign) = new_param_glue(TAB_SKIP_CODE);
        curalign = link(curalign);

        // `\cr` ends the preamble
        if (curcmd == CAR_RET) goto _Ldone;

        /// [#779] Scan preamble text until cur cmd is tab mark or car ret,
        ///     looking for changes in the tabskip glue;
        ///     append an alignrecord to the preamble list.


        /// [#783] Scan the template ⟨ u_j ⟩, putting the resulting token list in hold head.
        p = holdhead;
        link(p) = 0;
        while (true) {
            getpreambletoken();
            if (curcmd == MAC_PARAM) goto _Ldone1;
            if (   curcmd <= CAR_RET 
                && curcmd >= TAB_MARK 
                && align_state == -1000000L) {
                if (p == holdhead && curloop == 0 && curcmd == TAB_MARK) {
                    curloop = curalign;
                    continue;
                } else {
                    // "Missing # inserted in alignment preamble"
                    print_err(S(729));
                    /*
                     * "There should be exactly one # between &'s when an"
                     * "\\halign or \\valign is being set up. In this case you had"
                     * "none so I've put one in; maybe that will work."
                     */
                    help3(S(730), S(731), S(732));
                    backerror();
                    goto _Ldone1;
                }
            } else {
                if (curcmd != SPACER || p != holdhead) {
                    link(p) = get_avail();
                    p = link(p);
                    info(p) = curtok;
                }
                continue;
            }
        } // while (true)
        _Ldone1:
        /// end [#783]

        // a new alignrecord
        link(curalign) = new_null_box();
        curalign = link(curalign);

        info(curalign) = endspan;
        width(curalign) = nullflag;
        upart(curalign) = link(holdhead); 
        
        /// [#784] Scan the template ⟨ v_j ⟩, putting the resulting token list in `hold_head`
        p = holdhead;
        link(p) = 0;
        while (true) {
        _Llabcontinue:
            getpreambletoken();
            if (   curcmd <= CAR_RET 
                && curcmd >= TAB_MARK 
                && align_state == -1000000L) {
                goto _Ldone2;
            }
            if (curcmd == MAC_PARAM) {
                print_err(S(733)); // "Only one # is allowed per tab"
                /*
                 * "There should be exactly one # between &'s when an"
                 * "\\halign or \\valign is being set up. In this case you had"
                 * "more than one so I'm ignoring all but the first."
                 */
                help3(S(730), S(731), S(734));
                error();
                goto _Llabcontinue;
            }
            link(p) = get_avail();
            p = link(p);
            info(p) = curtok;
        } // while (true)
        _Ldone2:

        // put `\endtemplate` at the end
        link(p) = get_avail();
        p = link(p);
        info(p) = endtemplatetoken; 
        /// end [#784]

        vpart(curalign) = link(holdhead);
        /// 3nd [#779]
    } // while (true)

_Ldone:
    scanner_status = NORMAL; // end [#777]

    new_save_level(aligngroup);
    if (everycr != 0) begintokenlist(everycr, EVERY_CR_TEXT);
    alignpeek(); //  look for `\noalign` or `\omit`
} /* [#774] initalign */

/*786:*/
/*787:*/
void initspan(HalfWord p) {
    pushnest();
    if (mode == -H_MODE)
        spacefactor = 1000;
    else {
        prevdepth = ignoredepth;
        normalparagraph();
    }
    curspan = p;
}
/*:787*/

void initrow(void) {
    pushnest();
    mode = -H_MODE - V_MODE - mode;
    if (mode == -H_MODE)
        spacefactor = 0;
    else
        prevdepth = 0;
    tailappend(new_glue(glueptr(preamble)));
    subtype(tail) = TAB_SKIP_CODE + 1;
    curalign = link(preamble);
    curtail = curhead;
    initspan(curalign);
}
/*:786*/

/*788:*/
void initcol(void) {
    extrainfo(curalign) = curcmd;
    if (curcmd == OMIT)
        align_state = 0;
    else {
        backinput();
        begintokenlist(upart(curalign), U_TEMPLATE);
    }
}
/*:788*/

/// [#791] 
Boolean fincol(void) {
    Pointer p;  ///< the alignrecord after the current one.
    Pointer q, r; ///< temporary pointers for list manipulation
    Pointer s;  ///< a new span node
    Pointer u;  ///< a new unset box
    Scaled w;   ///< natural width
    GlueOrd o;  ///< order of infinity
    HalfWord n; ///< span counter
    Boolean Result;

    if (curalign == 0) confusion(S(735)); // "endv"
    q = link(curalign);
    if (q == 0) confusion(S(735)); // "endv"
    // "(interwoven alignment preambles are not allowed)"
    if (align_state < 500000L) fatalerror(S(509));
    p = link(q);

    /// [#792]  If the preamble list has been traversed,
    /// check that the row has ended.
    if ((p == 0) & (extrainfo(curalign) < crcode)) {
        if (curloop != 0) {
            /// [#793] Lengthen the preamble periodically
            link(q) = new_null_box();
            p = link(q);
            info(p) = endspan;
            width(p) = nullflag;
            curloop = link(curloop); 

            /// [#794] Copy the templates from node cur loop into node p
            q = holdhead;
            r = upart(curloop);
            while (r != 0) {
                link(q) = get_avail();
                q = link(q);
                info(q) = info(r);
                r = link(r);
            }
            link(q) = 0;
            upart(p) = link(holdhead);

            q = holdhead;
            r = vpart(curloop);
            while (r != 0) {
                link(q) = get_avail();
                q = link(q);
                info(q) = info(r);
                r = link(r);
            }
            link(q) = 0;
            vpart(p) = link(holdhead);
            /// end [#794]

            curloop = link(curloop);
            link(p) = new_glue(glueptr(curloop));
            /// end [#793]
        } else {
            print_err(S(736)); // "Extra alignment tab has been changed to "
            print_esc(S(737)); // "cr"
            // "You have given more \\span or & marks than there were"
            // "in the preamble to the \\halign or \\valign now in progress."
            // "So I'll assume that you meant to type \\cr instead."
            help3(S(738), S(739), S(740));
            extrainfo(curalign) = crcode;
            error();
        } // end [#792] if (curloop <=> 0)
    } // if ((p == 0) & (extrainfo(curalign) < crcode))

    if (extrainfo(curalign) != spancode) {
        unsave();
        new_save_level(aligngroup);

        /// [#796] Package an unset box for the current column 
        /// and record its width
        if (mode == -H_MODE) {
            adjusttail = curtail;
            u = hpack(link(head), 0, additional);
            w = width(u);
            curtail = adjusttail;
            adjusttail = 0;
        } else {
            u = vpackage(link(head), 0, additional, 0);
            w = height(u);
        }
        n = MIN_QUARTER_WORD; // this represents a span count of 1

        if (curspan != curalign) {
            /// [#798] Update width entry for spanned columns.
            q = curspan;
            do {
                n++;
                q = link(link(q));
            } while (q != curalign);

            if (n > MAX_QUARTER_WORD) confusion(S(741)); // "256 spans"
            q = curspan;
            while (link(info(q)) < n) {
                q = info(q);
            }
            if (link(info(q)) > n) {
                s = get_node(spannodesize);
                info(s) = info(q);
                link(s) = n;
                info(q) = s;
                width(s) = w;
            } else if (width(info(q)) < w) {
                width(info(q)) = w;
            }
        } else if (w > width(curalign)) {
            width(curalign) = w;
        }
    
        type(u) = UNSET_NODE;
        spancount(u) = n; 
        
        /// [#659] Determine the stretch order.
        if (totalstretch[FILLL - NORMAL] != 0) {
            o = FILLL;
        } else if (totalstretch[FILL - NORMAL] != 0) {
            o = FILL;
        } else if (totalstretch[FIL - NORMAL] != 0) {
            o = FIL;
        } else {
            o = NORMAL;
        }
        glueorder(u) = o;
        gluestretch(u) = totalstretch[o - NORMAL];

        /// [#655] Determine the shrink order.
        if (totalshrink[FILLL - NORMAL] != 0) {
            o = FILLL;
        } else if (totalshrink[FILL - NORMAL] != 0) {
            o = FILL;
        } else if (totalshrink[FIL - NORMAL] != 0) {
            o = FIL;
        } else {
            o = NORMAL;
        }
        gluesign(u) = o;
        glueshrink(u) = totalshrink[o - NORMAL];

        popnest();
        link(tail) = u;
        tail = u;
        /// end [#796]

        /// [#795] Copy the tabskip glue between columns.
        tailappend(new_glue(glueptr(link(curalign))));
        subtype(tail) = TAB_SKIP_CODE + 1;

        if (extrainfo(curalign) >= crcode) {
            Result = true;
            goto _Lexit;
        }
        initspan(p);
    } // if (extrainfo(curalign) != spancode)

    align_state = 1000000L;
    skip_spaces();
    curalign = p;
    initcol();
    Result = false;

_Lexit:
    return Result;
} /* [#791] fincol */

/*799:*/
void finrow(void) {
    Pointer p;

    if (mode == -H_MODE) {
        p = hpack(link(head), 0, additional);
        popnest();
        appendtovlist(p);
        if (curhead != curtail) {
            link(tail) = link(curhead);
            tail = curtail;
        }
    } else {
        p = vpack(link(head), 0, additional);
        popnest();
        link(tail) = p;
        tail = p;
        spacefactor = 1000;
    }
    type(p) = UNSET_NODE;
    gluestretch(p) = 0;
    if (everycr != 0) begintokenlist(everycr, EVERY_CR_TEXT);
    alignpeek();
}
/*:799*/

/*800:*/

void finalign(void) {
    Pointer p, q, r, s, u, v;
    Scaled t, w, o, rulesave;
    HalfWord n;
    MemoryWord auxsave;

    if (cur_group != aligngroup) confusion(S(742)); // "align1"
    unsave();
    if (cur_group != aligngroup) confusion(S(743)); // "align0"
    unsave();
    if (nest[nest_ptr - 1].modefield == M_MODE)
        o = displayindent;
    else
        o = 0;
    /*801:*/
    q = link(preamble);
    do { /*804:*/
        flush_list(upart(q));
        flush_list(vpart(q));
        p = link(link(q));
        if (width(q) == nullflag) { /*802:*/
            width(q) = 0;
            r = link(q);
            s = glueptr(r);
            if (s != zeroglue) {
                addglueref(zeroglue);
                delete_glue_ref(s);
                glueptr(r) = zeroglue;
            }
        }
        /*:802*/
        if (info(q) != endspan) { /*803:*/
            t = width(q) + width(glueptr(link(q)));
            r = info(q);
            s = endspan;
            info(s) = p;
            n = MIN_QUARTER_WORD + 1;
            do {
                width(r) -= t;
                u = info(r);
                while (link(r) > n) {
                    s = info(s);
                    n = link(info(s)) + 1;
                }
                if (link(r) < n) {
                    info(r) = info(s);
                    info(s) = r;
                    (link(r))--;
                    s = r;
                } else {
                    if (width(r) > width(info(s))) width(info(s)) = width(r);
                    free_node(r, spannodesize);
                }
                r = u;
            } while (r != endspan);
        }
        /*:803*/
        type(q) = UNSET_NODE;
        spancount(q) = MIN_QUARTER_WORD;
        height(q) = 0;
        depth(q) = 0;
        glueorder(q) = NORMAL;
        gluesign(q) = NORMAL;
        gluestretch(q) = 0;
        glueshrink(q) = 0;
        q = p; /*:801*/
    } while (q != 0);
    save_ptr -= 2;
    packbeginline = -modeline;
    if (mode == -V_MODE) {
        rulesave = overfullrule;
        overfullrule = 0;
        p = hpack(preamble, saved(1), saved(0));
        overfullrule = rulesave;
    } else {
        q = link(preamble);
        do {
            height(q) = width(q);
            width(q) = 0;
            q = link(link(q));
        } while (q != 0);
        p = vpack(preamble, saved(1), saved(0));
        q = link(preamble);
        do {
            width(q) = height(q);
            height(q) = 0;
            q = link(link(q));
        } while (q != 0);
    }
    packbeginline = 0; /*:804*/
    /*805:*/
    q = link(head);
    s = head;
    while (q != 0) { /*:805*/
        if (!is_char_node(q)) {
            if (type(q) == UNSET_NODE) { /*807:*/
                if (mode == -V_MODE) {
                    type(q) = HLIST_NODE;
                    width(q) = width(p);
                } else {
                    type(q) = VLIST_NODE;
                    height(q) = height(p);
                }
                glueorder(q) = glueorder(p);
                gluesign(q) = gluesign(p);
                glueset(q) = glueset(p);
                shiftamount(q) = o;
                r = link(listptr(q));
                s = link(listptr(p));
                do { /*808:*/
                    n = spancount(r);
                    t = width(s);
                    w = t;
                    u = holdhead;
                    while (n > MIN_QUARTER_WORD) {
                        n--;
                        /*809:*/
                        s = link(s);
                        v = glueptr(s);
                        link(u) = new_glue(v);
                        u = link(u);
                        subtype(u) = TAB_SKIP_CODE + 1;
                        t += width(v);
                        if (gluesign(p) == stretching) {
                            if (stretchorder(v) == glueorder(p))
                                t += tex_round(((double)glueset(p)) *
                                               stretch(v));
                        } else if (gluesign(p) == shrinking) {
                            if (shrinkorder(v) == glueorder(p))
                                t -=
                                    tex_round(((double)glueset(p)) * shrink(v));
                        }
                        s = link(s);
                        link(u) = new_null_box();
                        u = link(u);
                        t += width(s);
                        if (mode == -V_MODE)
                            width(u) = width(s);
                        else { /*:809*/
                            type(u) = VLIST_NODE;
                            height(u) = width(s);
                        }
                    }
                    if (mode == -V_MODE) { /*810:*/
                        height(r) = height(q);
                        depth(r) = depth(q);
                        if (t == width(r)) {
                            gluesign(r) = NORMAL;
                            glueorder(r) = NORMAL;
                            glueset(r) = 0.0;
                        } else if (t > width(r)) {
                            gluesign(r) = stretching;
                            if (gluestretch(r) == 0)
                                glueset(r) = 0.0;
                            else
                                glueset(r) =
                                    (double)(t - width(r)) / gluestretch(r);
                        } else {
                            glueorder(r) = gluesign(r);
                            gluesign(r) = shrinking;
                            if (glueshrink(r) == 0)
                                glueset(r) = 0.0;
                            else if ((glueorder(r) == NORMAL) &
                                     (width(r) - t > glueshrink(r)))
                                glueset(r) = 1.0;
                            else
                                glueset(r) =
                                    (double)(width(r) - t) / glueshrink(r);
                        }
                        width(r) = w;
                        type(r) = HLIST_NODE;
                    } else /*811:*/
                    {      /*:811*/
                        width(r) = width(q);
                        if (t == height(r)) {
                            gluesign(r) = NORMAL;
                            glueorder(r) = NORMAL;
                            glueset(r) = 0.0;
                        } else if (t > height(r)) {
                            gluesign(r) = stretching;
                            if (gluestretch(r) == 0)
                                glueset(r) = 0.0;
                            else
                                glueset(r) =
                                    (double)(t - height(r)) / gluestretch(r);
                        } else {
                            glueorder(r) = gluesign(r);
                            gluesign(r) = shrinking;
                            if (glueshrink(r) == 0)
                                glueset(r) = 0.0;
                            else if ((glueorder(r) == NORMAL) &
                                     (height(r) - t > glueshrink(r)))
                                glueset(r) = 1.0;
                            else
                                glueset(r) =
                                    (double)(height(r) - t) / glueshrink(r);
                        }
                        height(r) = w;
                        type(r) = VLIST_NODE;
                    }
                    /*:810*/
                    shiftamount(r) = 0;
                    if (u != holdhead) { /*:808*/
                        link(u) = link(r);
                        link(r) = link(holdhead);
                        r = u;
                    }
                    r = link(link(r));
                    s = link(link(s));
                } while (r != 0);
            } /*:807*/
            else if (type(q) == RULE_NODE) {
                if (isrunning(width(q))) {
                    width(q) = width(p);
                }
                if (isrunning(height(q))) {
                    height(q) = height(p);
                }
                if (isrunning(depth(q))) {
                    depth(q) = depth(p);
                }
                if (o != 0) {
                    r = link(q);
                    link(q) = 0;
                    q = hpack(q, 0, additional);
                    shiftamount(q) = o;
                    link(q) = r;
                    link(s) = q;
                }
            }
        }
        s = q;
        q = link(q);
    }
    flush_node_list(p);
    popalignment(); /*812:*/
    auxsave = aux;
    p = link(head);
    q = tail;
    popnest();
    if (mode == M_MODE) { /*1206:*/
        doassignments();
        if (curcmd != MATH_SHIFT) { /*1207:*/
            print_err(S(744)); // "Missing $$ inserted"
            // "Displays can use special alignments (like \\eqalignno)"
            // "only if nothing but the alignment itself is between $$'s."
            help2(S(726), S(727));
            backerror();
        } else { /*1197:*/
            get_x_token();
            if (curcmd != MATH_SHIFT) {
                print_err(S(745)); // "Display math should end with $$"
                // "The `$' that I just saw supposedly matches a previous `$$'."
                // "So I shall assume that you typed `$$' both times."
                help2(S(746), S(747));
                backerror();
            }
        }

        /*:1207*/
        popnest();
        tailappend(new_penalty(predisplaypenalty));
        tailappend(new_param_glue(ABOVE_DISPLAY_SKIP_CODE));
        link(tail) = p;
        if (p != 0) tail = q;
        tailappend(new_penalty(postdisplaypenalty));
        tailappend(new_param_glue(BELOW_DISPLAY_SKIP_CODE));
        prevdepth = auxsave.sc;
        resumeafterdisplay();
        return;
    }

    /*:1206*/
    aux = auxsave;
    link(tail) = p;
    if (p != 0) tail = q;
    if (mode == V_MODE) buildpage();

    /*:1197*/
    /*:812*/
} /*785:*/


void alignpeek(void) {
_Lrestart:
    align_state = 1000000L;
    skip_spaces();
    if (curcmd == NO_ALIGN) {
        scan_left_brace();
        new_save_level(noaligngroup);
        if (mode == -V_MODE) normalparagraph();
        return;
    }
    if (curcmd == RIGHT_BRACE) {
        finalign();
        return;
    }
    if (curcmd == CAR_RET && curchr == crcrcode) goto _Lrestart;
    initrow();
    initcol();
}
/*:785*/
/*:800*/

/** @}*/ // end group S768x812_P285x301