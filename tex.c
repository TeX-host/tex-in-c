#include "tex_header.h"

/*
 * 前置声明
 * 
 */

// hlistout
Static void vlistout(void);

// printsubsidiarydata, 
Static void showinfo(void);

// showactivities
Static void printtotals(void);

// finalign, makeaccent
Static void doassignments(void);
// finalign, aftermath
Static void resumeafterdisplay(void);
// finalign
Static void buildpage(void);
// cleanbox
Static void mlisttohlist(void);

// initalign, finrow
Static void alignpeek(void);
// initspan, alignpeek, fireup, 
Static void normalparagraph(void);


/*
 * functions
 * 
 */

long tex_round(double d) { return (long)(floor(d + 0.5)); }

// [tex], fonts
Integer get_defaulthyphenchar(void) { return defaulthyphenchar; }
Integer get_defaultskewchar(void) { return defaultskewchar; }
Pointer get_lo_mem_max(void) { return lo_mem_max; }

StrNumber fontidtext(InternalFontNumber x) { return text(fontidbase + x); }

static void set_fontidtext(InternalFontNumber x, StrNumber t) {
    text(fontidbase + x) = t;
}

void set_help(SChar k, ...) {
    va_list ap;
    va_start(ap, k);
    help_ptr = k;
    k--;
    while (k >= 0) {
        help_line[k] = va_arg(ap, uint_fast32_t);
        k--;
    }
    va_end(ap);
}



/*
# Basic printing procedures[13]:
    [57], [58], [59],   60,   [62],
    [63], [64], [65], [262], [263],
    [518], [699], [1355].
    println,  printchar,      print,               printnl,
    printesc, print_the_digs, print_int, print_cs, sprint_cs,
    print_file_name, print_size, print_write_whatsit
*/

// #70: print current string #str.c

/** @addtogroup S54x71_P24x29
 * @{
 */

/// #71: gets a line from the terminal
void term_input(void) {
    UInt16 k; // [0, BUF_SIZE=5000]

    // now the user sees the prompt for sure
    fflush(stdout); // update terminal
    if (!inputln(stdin, true)) {
        fatalerror(S(302)); // "End of file on the terminal!"
    }

    term_offset = 0; // the user’s line ended with hreturni
    selector--;      // prepare to echo the input
    if (last != first) {
        for (k = first; k < last; k++)
            print(buffer[k]);
    }
    println();
    selector++; // restore previous status
} // #71: term_input
/** @}*/ // end group S54x71_P24x29


// #262: prints a purported control sequence
// [Basic printing procedures]
void print_cs(long p) {
    if (p < hashbase) {
        // single character
        if (p >= singlebase) {
            if (p == nullcs) {
                print_esc(S(262)); // "csname"
                print_esc(S(263)); // "endcsname"
                print_char(' ');
            } else {
                print_esc(p - singlebase);
                if (catcode(p - singlebase) == LETTER) 
                    print_char(' ');
            } // if (p == nullcs) - else
        } else {
            if (p < activebase) {
                print_esc(S(264)); // "IMPOSSIBLE."
            } else {
                print(p - activebase);
            } // if (p < activebase) - else
        } // if (p >= singlebase) - else
    } else if (p >= UNDEFINED_CONTROL_SEQUENCE) {
        print_esc(S(264)); // "IMPOSSIBLE."
    } else if (!str_valid(text(p))) {
        print_esc(S(265)); // "NONEXISTENT."
    } else {
        print_esc(text(p));
        print_char(' ');
    } // if - elseif - ... - else
} // #262: print_cs

/// #263: prints a control sequence
void sprint_cs(Pointer p) {
    if (p >= hashbase) {
        print_esc(text(p));
        return;
    }
    if (p < singlebase) {
        print(p - activebase);
        return;
    }

    if (p < nullcs) {
        print_esc(p - singlebase);
    } else {
        print_esc(S(262)); // "csname"
        print_esc(S(263)); // "endcsname"
    }
} // #263: sprint_cs

/// #518
void print_file_name(StrNumber n, StrNumber a, StrNumber e) {
    slow_print(a);
    slow_print(n);
    slow_print(e);
} // #518: print_file_name

/// #699
void print_size(Integer s) {
    if (s == TEXT_SIZE) {
        print_esc(S(266)); // "textfont"
    } else {
        if (s == SCRIPT_SIZE) {
            print_esc(S(267)); // "scriptfont"
        } else {
            print_esc(S(268)); // "scriptscriptfont"
        }
    }
} // #699: print_size

/// #1355
Static void print_write_whatsit(StrNumber s, Pointer p) {
    print_esc(s);
    if (writestream(p) < 16) {
        print_int(writestream(p));
    } else if (writestream(p) == 16) {
        print_char('*');
    } else { // writestream(p) > 16
        print_char('-');
    } // if (writestream(p) <> 16)
} // #1355: print_write_whatsit


// #1284
void giveerrhelp(void) { tokenshow(errhelp); }


/** @addtogroup S72x98_P30x37
 * @{
 */

/*
 * Error handling procedures
 * 
 * xref[4]: 81, 93, 94, 95, 1304
 */

/// [#81]: goto end of TEX.
/// jump out: [81], 82, 84, 93
void jumpout(void) { longjmp(_JMP_global__end_of_TEX, 1); }

/** @}*/ // end group S72x98_P30x37



/** @addtogroup S110x114_P42x43
 * @{
 */

/// p43#114
#ifdef tt_DEBUG
Static void printword(MemoryWord w) {
    print_int(w.int_);
    print_char(' ');
    print_scaled(w.sc);
    print_char(' ');
    print_scaled( (long)floor(UNITY * w.gr + 0.5) );
    println();
    print_int(w.hh.UU.lh);
    print_char('=');
    print_int(w.hh.UU.U2.b0);
    print_char(':');
    print_int(w.hh.UU.U2.b1);
    print_char(';');
    print_int(w.hh.rh);
    print_char(' ');
    print_int(w.qqqq.b0);
    print_char(':');
    print_int(w.qqqq.b1);
    print_char(':');
    print_int(w.qqqq.b2);
    print_char(':');
    print_int(w.qqqq.b3);
}
#endif // #114: tt_DEBUG
/** @}*/ // end group S110x114_P42x43


/** @addtogroup S115x132_P44x49
 * @{
 */

/// [p46#120]: single-word node allocation.
Pointer get_avail(void) {
    Pointer p;

    p = avail;
    if (p != 0)
        avail = link(avail);
    else if (mem_end + CHAR_NODE_SIZE <= MEM_MAX) {
        p = mem_end + 1;
        mem_end += CHAR_NODE_SIZE;
    } else {
        p = hi_mem_min - 1;
        hi_mem_min -= CHAR_NODE_SIZE;
        if (hi_mem_min <= lo_mem_max) {
            runaway();
            overflow(S(317), MEM_MAX - MEM_MIN + 1);
        }
    }
    type(p) = charnodetype; // ??? set_as_char_node
    link(p) = 0;
    #ifdef tt_STAT
        dyn_used += CHAR_NODE_SIZE;
    #endif // #120: tt_STAT
    return p;
} // #120: get_avail

/// p46#123: makes list of single-word nodes available
void flushlist(HalfWord p) {
    Pointer q, r;

    if (p == 0) return;
    r = p;
    do {
        q = r;
        r = link(r);            
        #ifdef tt_STAT
            dyn_used -= CHAR_NODE_SIZE;
        #endif // #123: tt_STAT
    } while (r != 0);
    link(q) = avail;
    avail = p;
} // #123: flushlist


/// #125
HalfWord getnode(long s) {
    HalfWord Result;
    Pointer p, q;
    long r, t;

_Lrestart:
    p = rover;
    do { /*127:*/
        q = p + nodesize(p);
        while (isempty(q)) {
            t = rlink(q);
            if (q == rover) rover = t;
            llink(t) = llink(q);
            rlink(llink(q)) = t;
            q += nodesize(q);
        }
        r = q - s;
        if (r > p + 1) { /*128:*/
            nodesize(p) = r - p;
            rover = p;
            goto _Lfound;
        }
        /*:128*/
        if (r == p) {
            if (rlink(p) != p) { /*129:*/
                rover = rlink(p);
                t = llink(p);
                llink(rover) = t;
                rlink(t) = rover;
                goto _Lfound;
            }
            /*:129*/
        }
        nodesize(p) = q - p; /*:127*/
        p = rlink(p);
    } while (p != rover);
    if (s == 1073741824L) {
        Result = MAX_HALF_WORD;
        goto _Lexit;
    }
    if (lo_mem_max + 2 < hi_mem_min) {
        if (lo_mem_max + 2 <= MEM_BOT + MAX_HALF_WORD) { /*126:*/
            if (hi_mem_min - lo_mem_max >= 1998)
                t = lo_mem_max + 1000;
            else
                t = lo_mem_max + (hi_mem_min - lo_mem_max) / 2 + 1;
            p = llink(rover);
            q = lo_mem_max;
            rlink(p) = q;
            llink(rover) = q;
            if (t > MEM_BOT + MAX_HALF_WORD) t = MEM_BOT + MAX_HALF_WORD;
            rlink(q) = rover;
            llink(q) = p;
            link(q) = emptyflag;
            nodesize(q) = t - lo_mem_max;
            lo_mem_max = t;
            link(lo_mem_max) = 0;
            info(lo_mem_max) = 0;
            rover = q;
            goto _Lrestart;
        }
        /*:126*/
    }
    overflow(S(317), MEM_MAX - MEM_MIN + 1);
_Lfound:
    link(r) = 0;
    #ifdef tt_STAT
        var_used += s;
    #endif // #125: tt_STAT
    Result = r;
_Lexit:
    return Result;
} // #125: getnode

/// p48#130: variable-size node liberation
void freenode(Pointer p, HalfWord s) {
    Pointer q;

    nodesize(p) = s;
    link(p) = emptyflag;
    q = llink(rover);
    llink(p) = q;
    rlink(p) = rover;
    llink(rover) = p;
    rlink(q) = p;
    #ifdef tt_STAT
        var_used -= s;
    #endif // #130: tt_STAT
} // #130: freenode


#ifdef tt_INIT // #131,132
/// p49#131: sorts the available variable-size nodes by location
Static void sort_avail(void)
{   // used at #1311
    Pointer p, q, r,  // indices into mem
            oldrover; // initial rover setting

    p = getnode(1073741824L); // merge adjacent free areas
    p = rlink(rover);
    rlink(rover) = MAX_HALF_WORD;
    oldrover = rover;

    while (p != oldrover) {
        /// #132: Sort p into the list starting at rover
        /// and ADVANCE p to rlink(p)
        if (p < rover) {
            q = p;
            p = rlink(q);
            rlink(q) = rover;
            rover = q;
        } else {
            q = rover;
            while (rlink(q) < p)
                q = rlink(q);
            r = rlink(p);
            rlink(p) = rlink(q);
            rlink(q) = p;
            p = r;
        } // if (p < rover) - else
    } // while (p != oldrover)

    p = rover;
    while (rlink(p) != MAX_HALF_WORD) {
        llink(rlink(p)) = p;
        p = rlink(p);
    } // while (rlink(p) != MAX_HALF_WORD)
    rlink(p) = rover;
    llink(rover) = p;
} // sort_avail
#endif // #131: tt_INIT
/** @}*/ // end group S115x132_P44x49


/** @addtogroup S133x161_P50x57
 * @{
 */

/*136:*/
Static Pointer newnullbox(void) {
    Pointer p;

    p = getnode(boxnodesize);
    type(p) = HLIST_NODE;
    subtype(p) = MIN_QUARTER_WORD;
    width(p) = 0;
    depth(p) = 0;
    height(p) = 0;
    shiftamount(p) = 0;
    listptr(p) = 0;
    gluesign(p) = NORMAL;
    glueorder(p) = NORMAL;
    glueset(p) = 0.0;
    return p;
}
/*:136*/

/*139:*/
Pointer newrule(void) {
    Pointer p;

    p = getnode(rulenodesize);
    type(p) = RULE_NODE;
    subtype(p) = 0;
    width(p) = nullflag;
    depth(p) = nullflag;
    height(p) = nullflag;
    return p;
}
/*:139*/

/*144:*/
Static Pointer newligature(QuarterWord f, QuarterWord c, Pointer q) {
    Pointer p;

    p = getnode(smallnodesize);
    type(p) = LIGATURE_NODE;
    font_ligchar(p) = f;
    character_ligchar(p) = c;
    ligptr(p) = q;
    subtype(p) = 0;
    return p;
}


Static Pointer newligitem(QuarterWord c) {
    Pointer p;

    p = getnode(smallnodesize);
    character(p) = c;
    ligptr(p) = 0;
    return p;
}
/*:144*/

/*145:*/
Static Pointer newdisc(void) {
    Pointer p;

    p = getnode(smallnodesize);
    type(p) = DISC_NODE;
    replacecount(p) = 0;
    prebreak(p) = 0;
    postbreak(p) = 0;
    return p;
}
/*:145*/

/*147:*/
Static Pointer newmath(long w, SmallNumber s) {
    Pointer p;

    p = getnode(smallnodesize);
    type(p) = MATH_NODE;
    subtype(p) = s;
    width(p) = w;
    return p;
}
/*:147*/

/*151:*/
Pointer newspec(Pointer p) {
    Pointer q;

    q = getnode(gluespecsize);
    mem[q - MEM_MIN] = mem[p - MEM_MIN];
    gluerefcount(q) = 0;
    width(q) = width(p);
    stretch(q) = stretch(p);
    shrink(q) = shrink(p);
    return q;
}
/*:151*/

/*152:*/
Static Pointer newparamglue(SmallNumber n) {
    Pointer p, q;

    p = getnode(smallnodesize);
    type(p) = GLUE_NODE;
    subtype(p) = n + 1;
    leaderptr(p) = 0;
    q = gluepar(n); /*224:*/
    /*:224*/
    glueptr(p) = q;
    (gluerefcount(q))++;
    return p;
}
/*:152*/

/*153:*/
Static Pointer newglue(Pointer q) {
    Pointer p;

    p = getnode(smallnodesize);
    type(p) = GLUE_NODE;
    subtype(p) = NORMAL;
    leaderptr(p) = 0;
    glueptr(p) = q;
    (gluerefcount(q))++;
    return p;
}
/*:153*/

/*154:*/
Static Pointer newskipparam(SmallNumber n) {
    Pointer p;

    temp_ptr = newspec(gluepar(n)); /*224:*/
    /*:224*/
    p = newglue(temp_ptr);
    gluerefcount(temp_ptr) = 0;
    subtype(p) = n + 1;
    return p;
}
/*:154*/

/*156:*/
Static Pointer newkern(long w) {
    Pointer p;

    p = getnode(smallnodesize);
    type(p) = KERN_NODE;
    subtype(p) = NORMAL;
    width(p) = w;
    return p;
}
/*:156*/

/*158:*/
Static Pointer newpenalty(long m) {
    Pointer p;

    p = getnode(smallnodesize);
    type(p) = PENALTY_NODE;
    subtype(p) = 0;
    penalty(p) = m;
    return p;
}
/*:158*/
/** @}*/ // end group S133x161_P50x57


/** @addtogroup S162x172_P58x61
 * @{
 */

/// p60#167
#ifdef tt_DEBUG
Static void checkmem(Boolean printlocs) {
    Pointer p, q;
    Boolean clobbered;
    HalfWord FORLIM;

    for (p = MEM_MIN; p <= lo_mem_max; p++)
        P_clrbits_B(free_cells, p - MEM_MIN, 0, 3);
    for (p = hi_mem_min; p <= mem_end; p++) /*168:*/
        P_clrbits_B(free_cells, p - MEM_MIN, 0, 3);
    p = avail;
    q = 0;
    clobbered = false;
    while (p != 0) {
        if (p > mem_end || p < hi_mem_min)
            clobbered = true;
        else {
            if (P_getbits_UB(free_cells, p - MEM_MIN, 0, 3)) clobbered = true;
        }
        if (clobbered) {
            printnl(S(318));
            print_int(q);
            goto _Ldone1;
        }
        P_putbits_UB(free_cells, p - MEM_MIN, 1, 0, 3);
        q = p;
        p = link(q);
    }

_Ldone1: /*:168*/
    /*169:*/
    p = rover;
    q = 0;
    clobbered = false;
    do {
        if (p >= lo_mem_max || p < MEM_MIN)
            clobbered = true;
        else if ((rlink(p) >= lo_mem_max) | (rlink(p) < MEM_MIN))
            clobbered = true;
        else if ((!isempty(p)) | (nodesize(p) < 2) |
                 (p + nodesize(p) > lo_mem_max) | (llink(rlink(p)) != p)) {
            clobbered = true;
        }
        if (clobbered) {
            printnl(S(319));
            print_int(q);
            goto _Ldone2;
        }
        FORLIM = p + nodesize(p);
        for (q = p; q < FORLIM; q++) {
            if (P_getbits_UB(free_cells, q - MEM_MIN, 0, 3)) {
                printnl(S(320));
                print_int(q);
                goto _Ldone2;
            }
            P_putbits_UB(free_cells, q - MEM_MIN, 1, 0, 3);
        }
        q = p;
        p = rlink(p);
    } while (p != rover);

_Ldone2: /*:169*/
    /*170:*/
    p = MEM_MIN;
    while (p <= lo_mem_max) { /*:170*/
        if (isempty(p)) {
            printnl(S(321));
            print_int(p);
        }
        while ((p <= lo_mem_max) & (!P_getbits_UB(free_cells, p - MEM_MIN, 0, 3)))
            p++;
        while ((p <= lo_mem_max) &   P_getbits_UB(free_cells, p - MEM_MIN, 0, 3))
            p++;
    }
    if (printlocs) { /*171:*/
        printnl(S(322));
        FORLIM = lo_mem_max;
        for (p = MEM_MIN; p <= lo_mem_max; p++) {
            if ((!P_getbits_UB(free_cells, p - MEM_MIN, 0, 3)) &
                ((p > was_lo_max) | P_getbits_UB(was_free, p - MEM_MIN, 0, 3))) {
                print_char(' ');
                print_int(p);
            }
        }
        for (p = hi_mem_min; p <= mem_end; p++) {
            if ((!P_getbits_UB(free_cells, p - MEM_MIN, 0, 3)) &
                ((p < was_hi_min || p > was_mem_end) |
                 P_getbits_UB(was_free, p - MEM_MIN, 0, 3))) {
                print_char(' ');
                print_int(p);
            }
        }
    }
    /*:171*/
    for (p = MEM_MIN; p <= lo_mem_max; p++) {
        P_clrbits_B(was_free, p - MEM_MIN, 0, 3);
        P_putbits_UB(was_free,
            p - MEM_MIN, P_getbits_UB(free_cells, p - MEM_MIN, 0, 3), 0, 3);
    }
    for (p = hi_mem_min; p <= mem_end; p++) {
        P_clrbits_B(was_free, p - MEM_MIN, 0, 3);
        P_putbits_UB(was_free, 
            p - MEM_MIN, P_getbits_UB(free_cells, p - MEM_MIN, 0, 3), 0, 3);
    }
    was_mem_end = mem_end;
    was_lo_max = lo_mem_max;
    was_hi_min = hi_mem_min;
} // #164: checkmem

/// p61#172
Static void searchmem(Pointer p) {
    long q;

    for (q = MEM_MIN; q <= lo_mem_max; q++) {
        if (link(q) == p) {
            printnl(S(323));
            print_int(q);
            print_char(')');
        }
        if (info(q) == p) {
            printnl(S(324));
            print_int(q);
            print_char(')');
        }
    }
    for (q = hi_mem_min; q <= mem_end; q++) {
        if (link(q) == p) {
            printnl(S(323));
            print_int(q);
            print_char(')');
        }
        if (info(q) == p) {
            printnl(S(324));
            print_int(q);
            print_char(')');
        }
    }                                               /*255:*/
    for (q = activebase; q <= boxbase + 255; q++) { /*:255*/
        if (equiv(q) == p) {
            printnl(S(325));
            print_int(q);
            print_char(')');
        }
    }
    /*285:*/
    if (saveptr > 0) {                  /*933:*/
        for (q = 0; q < saveptr; q++) { /*:285*/
            if (equivfield(savestack[q]) == p) {
                printnl(S(326));
                print_int(q);
                print_char(')');
            }
        }
    }
    for (q = 0; q <= HYPH_SIZE; q++) { /*:933*/
        if (hyphlist[q] == p) {
            printnl(S(327));
            print_int(q);
            print_char(')');
        }
    }
} // #172: searchmem
#endif // #167,172: tt_DEBUG
/** @}*/ // end group S162x172_P58x61


/** @addtogroup S173x198_P62x68
 * @{
 */

/*174:*/
Static void shortdisplay(Pointer p) {
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
Static void printfontandchar(Pointer p) {
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


Static void printmark(long p) {
    print_char('{');
    if (p < hi_mem_min || p > mem_end)
        print_esc(S(308));
    else
        showtokenlist(link(p), 0, MAX_PRINT_LINE - 10);
    print_char('}');
}


Static void printruledimen(long d) {
    if (isrunning(d)) {
        print_char('*');
    } else
        print_scaled(d);
}
/*:176*/

/*177:*/
Static void printglue(long d, long order, StrNumber s) {
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
Static void printspec(long p, StrNumber s) {
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
/** @}*/ // end group S173x198_P62x68


/** @addtogroup S680x698_P249x257
 * @{
 */

/// [#691] prints family and character.
Static void printfamandchar(HalfWord p) {
    print_esc(S(333));
    print_int(fam(p));
    print_char(' ');
    print(character(p) - MIN_QUARTER_WORD);
} // [#691] printfamandchar

/// [#691] prints a delimiter as 24-bit hex value.
Static void printdelimiter(HalfWord p) {
    long a;

    a = smallfam(p) * 256 + smallchar(p) - MIN_QUARTER_WORD;
    a = a * 4096 + largefam(p) * 256 + largechar(p) - MIN_QUARTER_WORD;
    if (a < 0)
        print_int(a);
    else
        print_hex(a);
} // [#691] printdelimiter

/// [#692] display a noad field.
Static void printsubsidiarydata(HalfWord p, ASCIICode c) {
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

        case subbox: showinfo(); break;

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


/** [#225]: 打印 `glue` 参数的名称
 * 
 * \param[in] n
 * \return void 无返回值
 * 
 * 函数会打印参数对应的字符串。
 * 如果没有对应的字符串，则打印 `"[unknown glue parameter!]"`
 */
void print_skip_param(Integer n) {
    switch (n) {
        case LINE_SKIP_CODE: print_esc(S(341)); break; // "lineskip"
        case BASELINE_SKIP_CODE: print_esc(S(342)); break; // "baselineskip"
        case PAR_SKIP_CODE: print_esc(S(343)); break;
        case ABOVE_DISPLAY_SKIP_CODE: print_esc(S(344)); break;
        case BELOW_DISPLAY_SKIP_CODE: print_esc(S(345)); break;
        case ABOVE_DISPLAY_SHORT_SKIP_CODE: print_esc(S(346)); break;
        case BELOW_DISPLAY_SHORT_SKIP_CODE: print_esc(S(347)); break;
        case LEFT_SKIP_CODE: print_esc(S(348)); break;
        case RIGHT_SKIP_CODE: print_esc(S(349)); break;
        case TOP_SKIP_CODE: print_esc(S(350)); break;
        case SPLIT_TOP_SKIP_CODE: print_esc(S(351)); break;
        case TAB_SKIP_CODE: print_esc(S(352)); break;
        case SPACE_SKIP_CODE: print_esc(S(353)); break;
        case XSPACE_SKIP_CODE: print_esc(S(354)); break;
        case PAR_FILL_SKIP_CODE: print_esc(S(355)); break;
        case THIN_MU_SKIP_CODE: print_esc(S(356)); break;
        case MED_MU_SKIP_CODE: print_esc(S(357)); break;
        case THICK_MU_SKIP_CODE: print_esc(S(358)); break;

        default: print(S(359)); break; // "[unknown glue parameter!]"
    } // switch (n)
} // #225: print_skip_param


/** @addtogroup S173x198_P62x68
 * @{
 */

/*182:*/
Static void shownodelist(long p) {
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
Static void showbox(HalfWord p)
{  /*236:*/
  depth_threshold = showboxdepth;
  breadth_max = showboxbreadth;   /*:236*/
  if (breadth_max <= 0)
    breadth_max = 5;
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


/** @addtogroup S199x202_P69x70
 * @{
 */

// [#200] p points to the reference count of a token list that
//  is losing one reference
void delete_token_ref(HalfWord p) {
    if (tokenrefcount(p) == 0) {
        flushlist(p);
    } else {
        tokenrefcount(p)--;
    }
} // [#200] delete_token_ref

// [#201] p points to a glue specification
void delete_glue_ref(HalfWord p) {
    if (gluerefcount(p) == 0) {
        freenode(p, gluespecsize);
    } else {
        gluerefcount(p)--;
    }
} // [#201] delete_glue_ref

// [#202] erase list of nodes starting at p
Static void flush_node_list(HalfWord p) {
    Pointer q;

    while (p != 0) {
        q = link(p);
        if (ischarnode(p)) {
            FREE_AVAIL(p);
        } else {
            switch (type(p)) {
                case HLIST_NODE:
                case VLIST_NODE:
                case UNSET_NODE:
                    flush_node_list(listptr(p));
                    freenode(p, boxnodesize);
                    goto _Ldone;
                    break;

                case RULE_NODE:
                    freenode(p, rulenodesize);
                    goto _Ldone;
                    break;

                case INS_NODE:
                    flush_node_list(insptr(p));
                    delete_glue_ref(splittopptr(p));
                    freenode(p, insnodesize);
                    goto _Ldone;
                    break;

                case WHATSIT_NODE:
                    // #1358: Wipe out the whatsit node p and goto done
                    switch (subtype(p)) {
                        case opennode: 
                            freenode(p, opennodesize); 
                            break;

                        case writenode:
                        case specialnode:
                            delete_token_ref(writetokens(p));
                            freenode(p, writenodesize);
                            goto _Ldone;
                            break;

                        case closenode:
                        case languagenode: 
                            freenode(p, smallnodesize); 
                            break;

                        default:
                            confusion(S(427)); // "ext3"
                            break;
                    } // switch (subtype(p))
                    goto _Ldone;
                    break;

                case GLUE_NODE:
                    delete_glue_ref(glueptr(p));
                    if (leaderptr(p) != 0) flush_node_list(leaderptr(p));
                    break;

                case KERN_NODE:
                case MATH_NODE:
                case PENALTY_NODE:
                    /* blank case */
                    break;

                case LIGATURE_NODE: flush_node_list(ligptr(p)); break;
                case MARK_NODE: delete_token_ref(markptr(p)); break;

                case DISC_NODE:
                    flush_node_list(prebreak(p));
                    flush_node_list(postbreak(p));
                    break;

                // #698: Cases of flush node list that arise in mlists only
                case ADJUST_NODE:
                    flush_node_list(adjustptr(p));
                    break;

                case stylenode:
                    freenode(p, stylenodesize);
                    goto _Ldone;
                    break;

                case choicenode:
                    flush_node_list(displaymlist(p));
                    flush_node_list(textmlist(p));
                    flush_node_list(scriptmlist(p));
                    flush_node_list(scriptscriptmlist(p));
                    freenode(p, stylenodesize);
                    goto _Ldone;
                    break;

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
                    if (mathtype(nucleus(p)) >= subbox)
                        flush_node_list(info(nucleus(p)));
                    if (mathtype(supscr(p)) >= subbox)
                        flush_node_list(info(supscr(p)));
                    if (mathtype(subscr(p)) >= subbox)
                        flush_node_list(info(subscr(p)));
                    
                    if (type(p) == radicalnoad) {
                        freenode(p, radicalnoadsize);
                    } else if (type(p) == accentnoad) {
                        freenode(p, accentnoadsize);
                    } else {
                        freenode(p, noadsize);
                    }
                    goto _Ldone;
                    break;

                case leftnoad:
                case rightnoad:
                    freenode(p, noadsize);
                    goto _Ldone;
                    break;

                case fractionnoad:
                    flush_node_list(info(numerator(p)));
                    flush_node_list(info(denominator(p)));
                    freenode(p, fractionnoadsize);
                    goto _Ldone;
                    break;

                default: 
                    confusion(S(428)); // "flushing"
                    break;
            }
            freenode(p, smallnodesize);
        _Ldone:;
        }
        p = q;
    } // while (p != 0)
} // [#202] flush_node_list
/** @}*/ // end group S199x202_P69x70


/** @addtogroup S203x206_P71x72
 * @{
 */

/*204:*/
Static HalfWord copynodelist(HalfWord p)
{
  Pointer h, q, r=0 /* XXXX */;
  char words;

  h = get_avail();
  q = h;
  while (p != 0) {  /*205:*/
    words = 1;
    if (ischarnode(p)) {
      r = get_avail();
#ifdef BIG_CHARNODE
	words = CHAR_NODE_SIZE;
#endif
    } else {  /*206:*/
      switch (type(p)) {   /*:206*/

      case HLIST_NODE:
      case VLIST_NODE:
      case UNSET_NODE:
	r = getnode(boxnodesize);
	mem[r - MEM_MIN + 6] = mem[p - MEM_MIN + 6];
	mem[r - MEM_MIN + 5] = mem[p - MEM_MIN + 5];
	listptr(r) = copynodelist(listptr(p));
	words = 5;
	break;

      case RULE_NODE:
	r = getnode(rulenodesize);
	words = rulenodesize;
	break;

      case INS_NODE:
	r = getnode(insnodesize);
	mem[r - MEM_MIN + 4] = mem[p - MEM_MIN + 4];
	addglueref(splittopptr(p));
	insptr(r) = copynodelist(insptr(p));
	words = insnodesize - 1;
	break;

      case WHATSIT_NODE:   /*1357:*/
	switch (subtype(p)) {   /*:1357*/

	case opennode:
	  r = getnode(opennodesize);
	  words = opennodesize;
	  break;

	case writenode:
	case specialnode:
	  r = getnode(writenodesize);
	  addtokenref(writetokens(p));
	  words = writenodesize;
	  break;

	case closenode:
	case languagenode:
	  r = getnode(smallnodesize);
	  words = smallnodesize;
	  break;

	default:
	  confusion(S(429));
	  break;
	}
	break;

      case GLUE_NODE:
	r = getnode(smallnodesize);
	addglueref(glueptr(p));
	glueptr(r) = glueptr(p);
	leaderptr(r) = copynodelist(leaderptr(p));
	break;

      case KERN_NODE:
      case MATH_NODE:
      case PENALTY_NODE:
	r = getnode(smallnodesize);
	words = smallnodesize;
	break;

      case LIGATURE_NODE:
	r = getnode(smallnodesize);
	mem[ligchar(r) - MEM_MIN] = mem[ligchar(p) - MEM_MIN];
	ligptr(r) = copynodelist(ligptr(p));
	break;

      case DISC_NODE:
	r = getnode(smallnodesize);
	prebreak(r) = copynodelist(prebreak(p));
	postbreak(r) = copynodelist(postbreak(p));
	break;

      case MARK_NODE:
	r = getnode(smallnodesize);
	addtokenref(markptr(p));
	words = smallnodesize;
	break;

      case ADJUST_NODE:
	r = getnode(smallnodesize);
	adjustptr(r) = copynodelist(adjustptr(p));
	break;

      default:
	confusion(S(430));
	break;
      }
    }
    while (words > 0) {   /*:205*/
      words--;
      mem[r + words - MEM_MIN] = mem[p + words - MEM_MIN];
    }
    link(q) = r;
    q = r;
    p = link(p);
  }
  link(q) = 0;
  q = link(h);
  FREE_AVAIL(h);
  return q;
}
/*:204*/
/** @}*/ // end group S203x206_P71x72


/** @addtogroup S211x219_P77x80
 * @{
 */

// #211: prints the mode represented by m
void print_mode(Integer m) {
    if (m > 0) {
        switch (m / (MAX_COMMAND + 1)) {
            case 0: print(S(431)); break; // "vertical"
            case 1: print(S(432)); break; // "horizontal"
            case 2: print(S(433)); break; // "display math"
        }
    } else if (m == 0) {
        print(S(434)); // "no"
    } else { // m < 0
        switch ((-1 * m) / (MAX_COMMAND + 1)) {
            case 0: print(S(435)); break; // "internal vertical"
            case 1: print(S(436)); break; // "restricted horizontal"
            case 2: print(S(394)); break; // "math"
        }
    } // if (m <> 0)
    print(S(437)); // " mode"
} // #211: print_mode


/*216:*/
Static void pushnest(void)
{
  if (nest_ptr > max_nest_stack) {
    max_nest_stack = nest_ptr;
    if (nest_ptr == NEST_SIZE)
      overflow(S(438), NEST_SIZE);
  }
  nest[nest_ptr] = cur_list;
  nest_ptr++;
  head = get_avail();
  tail = head;
  prevgraf = 0;
  modeline = line;
}
/*:216*/

/*217:*/
Static void popnest(void)
{
  FREE_AVAIL(head);
  nest_ptr--;
  cur_list = nest[nest_ptr];
}
/*:217*/

/*218:*/

Static void showactivities(void)
{
  Pointer q, r;
  long t;
  short TEMP;

  nest[nest_ptr] = cur_list;
  printnl(S(385));
  println();
  for (TEMP = nest_ptr; TEMP >= 0; TEMP--) {
    int p = TEMP;
    short m = nest[p].modefield;
    MemoryWord a = nest[p].auxfield;
    printnl(S(439));
    print_mode(m);
    print(S(440));
    print_int(labs(nest[p].mlfield));
    if (m == H_MODE) {
      if (nest[p].pgfield != 8585216L) {
	print(S(441));
	print_int(nest[p].pgfield % 65536L);
	print(S(442));
	print_int(nest[p].pgfield / 4194304L);
	print_char(',');
	print_int((nest[p].pgfield / 65536L) & 63);
	print_char(')');
      }
    }
    if (nest[p].mlfield < 0)
      print(S(443));
    if (p == 0) {  /*986:*/
      if (pagehead != pagetail) {   /*:986*/
	printnl(S(444));
	if (outputactive)
	  print(S(445));
	showbox(link(pagehead));
	if (pagecontents > EMPTY) {
	  printnl(S(446));
	  printtotals();
	  printnl(S(447));
	  print_scaled(pagegoal);
	  r = link(pageinshead);
	  while (r != pageinshead) {
	    println();
	    print_esc(S(374));
	    t = subtype(r) - MIN_QUARTER_WORD;
	    print_int(t);
	    print(S(448));
	    t = x_over_n(height(r), 1000) * count(t);
	    print_scaled(t);
	    if (type(r) == splitup) {
	      q = pagehead;
	      t = 0;
	      do {
		q = link(q);
		if ((type(q) == INS_NODE) & (subtype(q) == subtype(r)))
		  t++;
	      } while (q != brokenins(r));
	      print(S(449));
	      print_int(t);
	      print(S(450));
	    }
	    r = link(r);
	  }
	}
      }
      if (link(contribhead) != 0)
	printnl(S(451));
    }
    showbox(link(nest[p].headfield));   /*219:*/
    switch (abs(m) / (MAX_COMMAND + 1)) {   /*:219*/

    case 0:
      printnl(S(452));
      if (a.sc <= ignoredepth)
	print(S(453));
      else
	print_scaled(a.sc);
      if (nest[p].pgfield != 0) {
	print(S(454));
	print_int(nest[p].pgfield);
	print(S(455));
	if (nest[p].pgfield != 1)
	  print_char('s');
      }
      break;

    case 1:
      printnl(S(456));
      print_int(a.hh.UU.lh);
      if (m > 0) {
	if (a.hh.rh > 0) {
	  print(S(457));
	  print_int(a.hh.rh);
	}
      }
      break;

    case 2:
      if (a.int_ != 0) {
	print(S(458));
	showbox(a.int_);
      }
      break;
    }
  }
}
/*:218*/
/** @}*/ // end group S211x219_P77x80


/** @addtogroup S220x255_P81x101
 * @{
 */

// [#245] prepare to do some tracing
void begindiagnostic(void) {
    diag_oldsetting = selector;
    if (tracingonline > 0 || selector != TERM_AND_LOG) return;

    selector--;
    if (history == SPOTLESS) history = WARNING_ISSUED;
}

// [#245] restore proper conditions after tracing
void enddiagnostic(Boolean blankline) {
    printnl(S(385)); // ""
    if (blankline) println();
    selector = diag_oldsetting;
}

#ifdef tt_STAT
/// #252:
Static void showeqtb(HalfWord n) {
    if (n < activebase) {
        print_char('?');
        return;
    }
    if (n < gluebase) {
        /*223:*/
        sprint_cs(n);
        print_char('=');
        printcmdchr(eqtype(n), equiv(n));
        if (eqtype(n) >= CALL) {
            print_char(':');
            showtokenlist(link(equiv(n)), 0, 32);
        }
        return;
    }                    /*:223*/
    if (n < localbase) { /*229:*/
        if (n < skipbase) {
            print_skip_param(n - gluebase);
            print_char('=');
            if (n < gluebase + THIN_MU_SKIP_CODE)
                printspec(equiv(n), S(459));
            else
                printspec(equiv(n), S(390));
            return;
        }
        if (n < muskipbase) {
            print_esc(S(460));
            print_int(n - skipbase);
            print_char('=');
            printspec(equiv(n), S(459));
            return;
        }
        print_esc(S(461));
        print_int(n - muskipbase);
        print_char('=');
        printspec(equiv(n), S(390));
        return;
    }
    if (n < intbase) { /*233:*/
        if (n == parshapeloc) {
            print_esc(S(462));
            print_char('=');
            if (parshapeptr == 0)
                print_char('0');
            else
                print_int(info(parshapeptr));
            return;
        }
        if (n < toksbase) {
            printcmdchr(ASSIGN_TOKS, n);
            print_char('=');
            if (equiv(n) != 0) showtokenlist(link(equiv(n)), 0, 32);
            return;
        }
        if (n < boxbase) {
            print_esc(S(463));
            print_int(n - toksbase);
            print_char('=');
            if (equiv(n) != 0) showtokenlist(link(equiv(n)), 0, 32);
            return;
        }
        if (n < curfontloc) {
            print_esc(S(464));
            print_int(n - boxbase);
            print_char('=');
            if (equiv(n) == 0) {
                print(S(465));
                return;
            }
            depth_threshold = 0;
            breadth_max = 1;
            shownodelist(equiv(n));
            return;
        }
        if (n < catcodebase) { /*234:*/
            if (n == curfontloc)
                print(S(466));
            else if (n < mathfontbase + 16) {
                print_esc(S(266));
                print_int(n - mathfontbase);
            } else if (n < mathfontbase + 32) {
                print_esc(S(267));
                print_int(n - mathfontbase - 16);
            } else {
                print_esc(S(268));
                print_int(n - mathfontbase - 32);
            }
            print_char('=');
            print_esc(fontidtext(equiv(n)));
            return;
        }
        /*:234*/
        if (n < mathcodebase) {
            if (n < lccodebase) {
                print_esc(S(467));
                print_int(n - catcodebase);
            } else if (n < uccodebase) {
                print_esc(S(468));
                print_int(n - lccodebase);
            } else if (n < sfcodebase) {
                print_esc(S(469));
                print_int(n - uccodebase);
            } else {
                print_esc(S(470));
                print_int(n - sfcodebase);
            }
            print_char('=');
            print_int(equiv(n));
            return;
        }
        print_esc(S(471));
        print_int(n - mathcodebase);
        print_char('=');
        print_int(equiv(n));
        return;
    }
    if (n < dimenbase) { /*242:*/
        if (n < countbase)
            printparam(n - intbase);
        else if (n < delcodebase) {
            print_esc(S(472));
            print_int(n - countbase);
        } else {
            print_esc(S(473));
            print_int(n - delcodebase);
        }
        print_char('=');
        print_int(eqtb[n - activebase].int_);
        return;
    }                   /*:242*/
    if (n > EQTB_SIZE) { /*251:*/
        print_char('?');
        return;
    }
    /*:251*/
    if (n < SCALED_BASE)
        printlengthparam(n - dimenbase);
    else {
        print_esc(S(474));
        print_int(n - SCALED_BASE);
    }
    print_char('=');
    print_scaled(eqtb[n - activebase].sc);
    print(S(459));

    /*:229*/
    /*235:*/
    /*:235*/
    /*:233*/
} // #252: showeqtb
#endif // #252: tt_STAT
/** @}*/ // end group S220x255_P81x101


// Static HalfWord idlookup(Integer j, Integer l, Boolean no_new_cs) { 
//     return idlookup_p(buffer + j, l, no_new_cs); 
// }


/** @addtogroup S256x267_P102x108
 * @{
 */

// [#259]  
//  that matches a given string of length `l > 1`
//  appearing in `buffer[j, (j + l − 1)]
// 
// xref[3]: 264, 356, 374
HalfWord idlookup_p(ASCIICode buf_ptr[], Integer len, Boolean no_new_cs) {
    // start index; 已经在调用时整合到 buf_ptr 和 len 中
    Integer j = 0; 

    Integer h; // hash code
    Pointer p; // index in hash array
    Pointer k; // index in buffer array

    // [#261] Compute the hash code h
    h = buf_ptr[j];
    for (k = j + 1; k <= j + len - 1; k++) {
        h = h + h + buf_ptr[k];
        while (h >= HASH_PRIME) {
            h -= HASH_PRIME;
        }
    } // [#261]

    // we start searching here; 
    // note that `0 ≤ h < HASH_PRIME`
    p = h + hashbase;
    while (true) {
        if (text(p) > 0 && str_bcmp(buf_ptr, len, text(p))) break;
        if (next(p) == 0) {
            if (no_new_cs) {
                p = UNDEFINED_CONTROL_SEQUENCE;
            } else {
                // [#260] Insert a new control sequence after p, 
                //  then make p point to it
                if (text(p) > 0) {
                    do { // search for an empty location in hash
                        if (hashisfull) {
                            overflow(S(475), HASH_SIZE); // "hash size
                        }
                        hash_used--;
                    } while (text(hash_used) != 0);
                    next(p) = hash_used;
                    p = hash_used;
                } // if (text(p) > 0)
                text(p) = str_insert(buf_ptr, len);

                #ifdef tt_STAT
                    cs_count++;
                #endif // #260: tt_STAT
            } // if (no_new_cs <>)
            break;
        } // if (next(p) == 0)
    
        p = next(p);
    } // while (true)

    return p;
} // [#259] idlookup_p

/// p105#264
#ifdef tt_INIT
Static void primitive(StrNumber s, QuarterWord c, HalfWord o) {
    if (s < 256) {
        cur_val = s + singlebase;
    } else {
        // TODO:
        // k ← str START[s]; l ← str START[s + 1] − k;
        // for j ← 0 to l − 1 do buffer[j] ← so(str pool[k + j]);
        cur_val = idlookup_s(s, false);
        flush_string();
        text(cur_val) = s;
    }
    eqlevel(cur_val) = levelone;
    eqtype(cur_val) = c;
    equiv(cur_val) = o;
} // #264: primitive
#endif // #264: tt_INIT
/** @}*/ // end group S256x267_P102x108


/** @addtogroup S268x288_P109x114
 * @{
 */

/*274:*/
Static void newsavelevel(GroupCode c) {
    if (saveptr > maxsavestack) {
        maxsavestack = saveptr;
        if (maxsavestack > SAVE_SIZE - 6) overflow(S(476), SAVE_SIZE);
    }
    savetype(saveptr) = levelboundary;
    savelevel(saveptr) = curgroup;
    saveindex(saveptr) = curboundary;
    if (curlevel == MAX_QUARTER_WORD)
        overflow(S(477), MAX_QUARTER_WORD - MIN_QUARTER_WORD);
    curboundary = saveptr;
    curlevel++;
    saveptr++;
    curgroup = c;
}
/*:274*/

/*275:*/
Static void eqdestroy(MemoryWord w) {
    Pointer q;

    switch (eqtypefield(w)) {
        case CALL:
        case LONG_CALL:
        case OUTER_CALL:
        case LONG_OUTER_CALL: delete_token_ref(equivfield(w)); break;

        case GLUE_REF: delete_glue_ref(equivfield(w)); break;

        case SHAPE_REF:
            q = equivfield(w);
            if (q != 0) freenode(q, info(q) + info(q) + 1);
            break;

        case BOX_REF: flush_node_list(equivfield(w)); break;
    }
}
/*:275*/

/*276:*/
Static void eqsave(HalfWord p, QuarterWord l) {
    if (saveptr > maxsavestack) {
        maxsavestack = saveptr;
        if (maxsavestack > SAVE_SIZE - 6) overflow(S(476), SAVE_SIZE);
    }
    if (l == levelzero) {
        savetype(saveptr) = restorezero;
    } else {
        savestack[saveptr] = eqtb[p - activebase];
        saveptr++;
        savetype(saveptr) = restoreoldvalue;
    }
    savelevel(saveptr) = l;
    saveindex(saveptr) = p;
    saveptr++;
}
/*:276*/

/*277:*/
void eqdefine(HalfWord p, QuarterWord t, HalfWord e) {
    if (eqlevel(p) == curlevel)
        eqdestroy(eqtb[p - activebase]);
    else if (curlevel > levelone)
        eqsave(p, eqlevel(p));
    eqlevel(p) = curlevel;
    eqtype(p) = t;
    equiv(p) = e;
}
/*:277*/

/*278:*/
Static void eqworddefine(HalfWord p, long w) {
    if (xeqlevel[p - intbase] != curlevel) {
        eqsave(p, xeqlevel[p - intbase]);
        xeqlevel[p - intbase] = curlevel;
    }
    eqtb[p - activebase].int_ = w;
}
/*:278*/

/*279:*/
Static void geqdefine(HalfWord p, QuarterWord t, HalfWord e) {
    eqdestroy(eqtb[p - activebase]);
    eqlevel(p) = levelone;
    eqtype(p) = t;
    equiv(p) = e;
}


Static void geqworddefine(HalfWord p, long w) {
    eqtb[p - activebase].int_ = w;
    xeqlevel[p - intbase] = levelone;
}
/*:279*/

/*280:*/
Static void saveforafter(HalfWord t) {
    if (curlevel <= levelone) return;
    if (saveptr > maxsavestack) {
        maxsavestack = saveptr;
        if (maxsavestack > SAVE_SIZE - 6) overflow(S(476), SAVE_SIZE);
    }
    savetype(saveptr) = inserttoken;
    savelevel(saveptr) = levelzero;
    saveindex(saveptr) = t;
    saveptr++;
}
/*:280*/

/*281:*/
#ifdef tt_STAT
/// #284
Static void restoretrace(HalfWord p, StrNumber s) {
    begindiagnostic();
    print_char('{');
    print(s);
    print_char(' ');
    showeqtb(p);
    print_char('}');
    enddiagnostic(false);
}
#endif // #284: tt_STAT


Static void unsave(void) {
    Pointer p;
    QuarterWord l = 0 /* XXXX */;

    if (curlevel <= levelone) {
        confusion(S(478));
        return;
    }
    curlevel--; /*282:*/
    while (true) {
        saveptr--;
        if (savetype(saveptr) == levelboundary) break;
        p = saveindex(saveptr);
        if (savetype(saveptr) == inserttoken) { /*326:*/
            HalfWord t = curtok;
            curtok = p;
            backinput();
            curtok = t;
            continue;
        }                                           /*:326*/
        if (savetype(saveptr) == restoreoldvalue) { /*283:*/
            l = savelevel(saveptr);
            saveptr--;
        } else {
            savestack[saveptr] = eqtb[UNDEFINED_CONTROL_SEQUENCE - activebase];
        }
        if (p < intbase) {
            if (eqlevel(p) == levelone) {
                eqdestroy(savestack[saveptr]);
                #ifdef tt_STAT
                    if (tracingrestores > 0) restoretrace(p, S(479));
                #endif // #283.1: tt_STAT
            } else {
                eqdestroy(eqtb[p - activebase]);
                eqtb[p - activebase] = savestack[saveptr];
                #ifdef tt_STAT
                    if (tracingrestores > 0) restoretrace(p, S(480));
                #endif // #283.2: tt_STAT
            }
            continue;
        }
        if (xeqlevel[p - intbase] != levelone) {
            eqtb[p - activebase] = savestack[saveptr];
            xeqlevel[p - intbase] = l; 
            #ifdef tt_STAT
                if (tracingrestores > 0) restoretrace(p, S(480));
            #endif // #283.3: tt_STAT
                /*:283*/
        } else {                            
            #ifdef tt_STAT
                if (tracingrestores > 0) restoretrace(p, S(479));
            #endif // #283.4: tt_STAT
        }
    } // while (true)

    curgroup = savelevel(saveptr);
    curboundary = saveindex(saveptr); /*:282*/
}
/*:281*/

/*288:*/
void preparemag(void) {
    if (magset > 0 && mag != magset) {
        print_err(S(481));
        print_int(mag);
        print(S(482));
        printnl(S(483));
        help2(S(484), S(485));
        int_error(magset);
        geqworddefine(intbase + magcode, magset);
    }
    if (mag <= 0 || mag > 32768L) {
        print_err(S(486));
        help1(S(487));
        int_error(mag);
        geqworddefine(intbase + magcode, 1000);
    }
    magset = mag;
}
/*:288*/
/** @}*/ // end group S268x288_P109x114


/** @addtogroup S539x582_P196x213
 * @{
 */

/// [ #539~582: PART 30: FONT METRIC DATA ]

/*577:*/
void scanfontident(void) { /*406:*/
    InternalFontNumber f;
    HalfWord m;

    skip_spaces();
    if (curcmd == DEF_FONT)
        f = curfont;
    else if (curcmd == SET_FONT)
        f = curchr;
    else if (curcmd == DEF_FAMILY) {
        m = curchr;
        scan_four_bit_int();
        f = equiv(m + cur_val);
    } else {
        print_err(S(584));
        help2(S(585), S(586));
        backerror();
        f = NULL_FONT;
    }
    cur_val = f;
}
/*:577*/

/*578:*/
void findfontdimen(Boolean writing) {
    InternalFontNumber f;
    long n;

    scan_int();
    n = cur_val;
    scanfontident();
    f = cur_val;
    if (n <= 0) {
        cur_val = fmemptr;
    } else {
        if (writing && n <= SPACE_SHRINK_CODE && n >= SPACE_CODE &&
            fontglue[f] != 0) {
            delete_glue_ref(fontglue[f]);
            fontglue[f] = 0;
        }
        if (n > fontparams[f]) {
            if (f < fontptr) {
                cur_val = fmemptr;
            } else { /*:580*/
                do {
                    if (fmemptr == FONT_MEM_SIZE) overflow(S(587), FONT_MEM_SIZE);
                    fontinfo[fmemptr].sc = 0;
                    fmemptr++;
                    fontparams[f]++;
                } while (n != fontparams[f]);
                cur_val = fmemptr - 1;
            }
        } else {
            cur_val = n + parambase[f];
        }
    } /*579:*/
    if (cur_val != fmemptr) return; /*:579*/
    print_err(S(588));
    print_esc(fontidtext(f));
    print(S(589));
    print_int(fontparams[f]);
    print(S(590));
    help2(S(591), S(592));
    error();
}
/*:578*/
/** @}*/ // end group S539x582_P196x213


/*464:*/

Pointer tex_global_p;

Static void strtoks_helper(ASCIICode t) {
    long tt = t;
    Pointer p = tex_global_p;
    if (tt == ' ')
        tt = spacetoken;
    else
        tt += othertoken;
    FAST_STORE_NEW_TOKEN(p, tt);
    tex_global_p = p;
}

HalfWord strtoks(StrPoolPtr b) {
    Pointer p;
    str_room(1);
    p = temphead;
    link(p) = 0;
    tex_global_p = p;
    str_map_from_mark(b, strtoks_helper);
    p = tex_global_p;
    return p;
}
/*:464*/

/*465:*/
HalfWord thetoks(void) {
    enum Selector old_setting;
    Pointer p, r;

    get_x_token();
    scan_something_internal(TOK_VAL, false);
    if (cur_val_level >= IDENT_VAL) { /*466:*/
        p = temphead;
        link(p) = 0;
        if (cur_val_level == IDENT_VAL) {
            STORE_NEW_TOKEN(p, CS_TOKEN_FLAG + cur_val);
            return p;
        }
        if (cur_val == 0) return p;
        r = link(cur_val);
        while (r != 0) {
            FAST_STORE_NEW_TOKEN(p, info(r));
            r = link(r);
        }
        return p;
    } else {
        StrPoolPtr b = str_mark();
        old_setting = selector;
        selector = NEW_STRING;
        switch (cur_val_level) {

            case INT_VAL: print_int(cur_val); break;

            case DIMEN_VAL:
                print_scaled(cur_val);
                print(S(459));
                break;

            case GLUE_VAL:
                printspec(cur_val, S(459));
                delete_glue_ref(cur_val);
                break;

            case MU_VAL:
                printspec(cur_val, S(390));
                delete_glue_ref(cur_val);
                break;
        }
        selector = old_setting;
        /*	fprintf(stderr,"(%d %d)",bb-b,pool_ptr-b); */
        return (strtoks(b));
    }
    /*:466*/
}
/*:465*/


/*473:*/
Static HalfWord scantoks(Boolean macrodef, Boolean xpand) {
    HalfWord t, s, unbalance, hashbrace;
    Pointer p;

    if (macrodef)
        scanner_status = DEFINING;
    else
        scanner_status = ABSORBING;
    warning_index = curcs;
    defref = get_avail();
    tokenrefcount(defref) = 0;
    p = defref;
    hashbrace = 0;
    t = ZERO_TOKEN;
    if (macrodef) { /*474:*/
        while (true) {
            gettoken();
            if (curtok < rightbracelimit) goto _Ldone1;
            if (curcmd == MAC_PARAM) { /*476:*/
                s = matchtoken + curchr;
                gettoken();
                if (curcmd == LEFT_BRACE) {
                    hashbrace = curtok;
                    STORE_NEW_TOKEN(p, curtok);
                    STORE_NEW_TOKEN(p, endmatchtoken);
                    goto _Ldone;
                }
                if (t == ZERO_TOKEN + 9) {
                    print_err(S(643));
                    help1(S(644));
                    error();
                } else {
                    t++;
                    if (curtok != t) {
                        print_err(S(645));
                        help2(S(646), S(647));
                        backerror();
                    }
                    curtok = s;
                }
            }
            /*:476*/
            STORE_NEW_TOKEN(p, curtok);
        }
    _Ldone1:
        STORE_NEW_TOKEN(p, endmatchtoken);
        if (curcmd == RIGHT_BRACE) { /*475:*/
            print_err(S(566));
            align_state++;
            help2(S(648), S(649));
            error();
            goto _Lfound;
        }
        /*:475*/
    _Ldone:;
    } else
        scan_left_brace();
    /*:474*/
    /*477:*/
    unbalance = 1;
    while (true) {   /*:477*/
        if (xpand) { /*478:*/
            while (true) {
                getnext();
                if (curcmd <= MAX_COMMAND) goto _Ldone2;
                if (curcmd != THE)
                    expand();
                else {
                    Pointer q = thetoks();
                    if (link(temphead) != 0) {
                        link(p) = link(temphead);
                        p = q;
                    }
                }
            }
        _Ldone2:
            xtoken();
        } else
            gettoken();
        /*:478*/
        if (curtok < rightbracelimit) {
            if (curcmd < RIGHT_BRACE)
                unbalance++;
            else {
                unbalance--;
                if (unbalance == 0) goto _Lfound;
            }
        } else if (curcmd == MAC_PARAM) {
            if (macrodef) { /*479:*/
                s = curtok;
                if (xpand)
                    get_x_token();
                else
                    gettoken();
                if (curcmd != MAC_PARAM) {
                    if (curtok <= ZERO_TOKEN || curtok > t) {
                        print_err(S(650));
                        sprint_cs(warning_index);
                        help3(S(651), S(652), S(653));
                        backerror();
                        curtok = s;
                    } else
                        curtok = outparamtoken - '0' + curchr;
                }
            }
            /*:479*/
        }
        STORE_NEW_TOKEN(p, curtok);
    }
_Lfound:
    scanner_status = NORMAL;
    if (hashbrace != 0) {
        STORE_NEW_TOKEN(p, hashbrace);
    }
    return p;
} /*:473*/


/*482:*/
Static void readtoks(long n, HalfWord r) {
    Pointer p;
    long s;
    /* SmallNumber */ int m; /* INT */

    scanner_status = DEFINING;
    warning_index = r;
    defref = get_avail();
    tokenrefcount(defref) = 0;
    p = defref;
    STORE_NEW_TOKEN(p, endmatchtoken);
    if ((unsigned long)n > 15)
        m = 16;
    else
        m = n;
    s = align_state;
    align_state = 1000000L;
    do { /*483:*/
        beginfilereading();
        NAME = m + 1;
        if (readopen[m] == closed) { /*484:*/
            if (interaction > NON_STOP_MODE) {
                if (n < 0) {
                    print(S(385));
                    term_input();
                } else {
                    println();
                    sprint_cs(r);
                    print('=');
                    term_input();
                    n = -1;
                }
            } else /*:484*/
                fatalerror(S(654));
        } else if (readopen[m] == justopen) {
            if (inputln(readfile[m], false))
                readopen[m] = NORMAL;
            else { /*:485*/
                aclose(&readfile[m]);
                readopen[m] = closed;
            }
        } else {
            if (!inputln(readfile[m], true)) {
                aclose(&readfile[m]);
                readopen[m] = closed;
                if (align_state != 1000000L) {
                    runaway();
                    print_err(S(655));
                    print_esc(S(656));
                    help1(S(657));
                    align_state = 1000000L;
                    error();
                }
            }
        }
        LIMIT = last;
        if (end_line_char_inactive) {
            LIMIT--;
        } else {
            buffer[LIMIT] = end_line_char;
        }
        first = LIMIT + 1;
        LOC = START;
        STATE = NEW_LINE;
        while (true) {
            gettoken();
            if (curtok == 0) goto _Ldone;
            if (align_state < 1000000L) {
                do {
                    gettoken();
                } while (curtok != 0);
                align_state = 1000000L;
                goto _Ldone;
            }
            STORE_NEW_TOKEN(p, curtok);
        }
_Ldone: /*:483*/    
        endfilereading();
    } while (align_state != 1000000L);
    cur_val = defref;
    scanner_status = NORMAL;
    align_state = s;

    /*485:*/
}
/*:482*/


/** @addtogroup S511x538_P188x195
 * @{
 */

/// [ #511. File names. ] 

/*515:*/
Static void beginname(void) { extdelimiter = 0; }
/*:515*/

/*516:*/
Static Boolean morename(ASCIICode c) {
    if (c == ' ') {
        return false;
    } else {
        str_room(1);
        if (c == '.' && extdelimiter == 0) {
            extdelimiter = makestring();
        }
        append_char(c);
        return true;
    }
}
/*:516*/

/*517:*/
Static void endname(void) {
    curarea = S(385);
    if (extdelimiter == 0) {
        curext = S(385);
        curname = makestring();
    } else {
        curname = extdelimiter;
        curext = makestring();
    }
}

long filename_k;
Static void appendtoname(ASCIICode x) {
    filename_k++;
    if (filename_k <= FILE_NAME_SIZE) {
        name_of_file[filename_k - 1] = xchr[x];
    }
}
/*:517*/

/*519:*/
void packfilename(StrNumber n, StrNumber a, StrNumber e) {
    long k;

    k = 0;
    filename_k = 0;
    str_map(a, appendtoname);
    str_map(n, appendtoname);
    str_map(e, appendtoname);
    k = filename_k;
    if (k <= FILE_NAME_SIZE)
        namelength = k;
    else
        namelength = FILE_NAME_SIZE;
    for (k = namelength; k < FILE_NAME_SIZE; k++)
        name_of_file[k] = ' ';
} /*:519*/

/*525:*/
Static StrNumber makenamestring(void) {
    int k;
    for (k = 0; k < namelength; k++) {
        append_char(xord[name_of_file[k]]);
    }
    return (makestring());
}

Static StrNumber amakenamestring(void) { return (makenamestring()); }
Static StrNumber bmakenamestring(void) { return (makenamestring()); }
Static StrNumber wmakenamestring(void) { return (makenamestring()); }
/*:525*/

/*526:*/
Static void scanfilename(void) {
    name_in_progress = true;
    beginname();
    skip_spaces();
    while (true) {
        if (curcmd > OTHER_CHAR || curchr > 255) {
            backinput();
            break;
        }
        if (!morename(curchr)) break;
        get_x_token();
    }
    endname();
    name_in_progress = false;
}
/*:526*/

/*529:*/
Static void packjobname(StrNumber s) {
    curarea = S(385);
    curext = s;
    curname = job_name;
    packfilename(curname, curarea, curext);
}
/*:529*/

/*530:*/
Static void promptfilename(StrNumber s, StrNumber e) {
    short k;

    if (s == S(665)) {
        print_err(S(666));
    } else {
        print_err(S(667));
    }
    print_file_name(curname, curarea, curext);
    print(S(668));
    if (e == S(669)) showcontext();
    printnl(S(670));
    print(s);
    if (interaction < SCROLL_MODE) fatalerror(S(671));
    print(S(488));
    term_input(); /*531:*/
    beginname();
    k = first;
    while (buffer[k] == ' ' && k < last)
        k++;
    while (true) {
        if (k == last) break;
        if (!morename(buffer[k])) break;
        k++;
    }
    endname(); /*:531*/
    if (curext == S(385)) curext = e;
    packfilename(curname, curarea, curext);
}
/*:530*/

/*534:*/
void openlogfile(void) {
    enum Selector old_setting;
    short k;
    short l;
    Char months[36];
    short FORLIM;

    old_setting = selector;
    if (job_name == 0) job_name = S(672);
    packjobname(S(673));
    while (!a_open_out(&log_file)) { /*535:*/
        selector = TERM_ONLY;
        promptfilename(S(674), S(673));
    }
    /*:535*/
    logname = amakenamestring();
    selector = LOG_ONLY;
    log_opened = true;
    /*536:*/
    fprintf(log_file, "%s", TEX_BANNER);
    slow_print(format_ident);
    print(S(675));
    print_int(day);
    print_char(' ');
    memcpy(months, "JANFEBMARAPRMAYJUNJULAUGSEPOCTNOVDEC", 36);
    FORLIM = month * 3;
    for (k = month * 3 - 3; k < FORLIM; k++)
        fwrite(&months[k], 1, 1, log_file);
    print_char(' ');
    print_int(year);
    print_char(' ');
    print_two(tex_time / 60);
    print_char(':');
    print_two(tex_time % 60); /*:536*/
    inputstack[inputptr] = cur_input;
    printnl(S(676));
    l = inputstack[0].limitfield;
    if (buffer[l] == end_line_char) l--;
    for (k = 1; k <= l; k++)
        print(buffer[k]);
    println();
    selector = old_setting + 2;
}
/*:534*/

/*537:*/
void startinput(void) {
    scanfilename();
    if (curext == S(385)) curext = S(669);
    packfilename(curname, curarea, curext);
    while (true) {
        beginfilereading();
        if (a_open_in(&curfile)) break;
        if (curarea == S(385)) {
            packfilename(curname, S(677), curext);
            if (a_open_in(&curfile)) break;
        }
        endfilereading();
        promptfilename(S(665), S(669));
    }

    NAME = amakenamestring();
    if (job_name == 0) {
        job_name = curname;
        openlogfile();
    }
    if (term_offset + str_length(NAME) > MAX_PRINT_LINE - 2) {
        println();
    } else if (term_offset > 0 || file_offset > 0)
        print_char(' ');
    print_char('(');
    openparens++;
    slow_print(NAME);
    fflush(stdout);
    STATE = NEW_LINE;

#if 0
  if (NAME == str_ptr - 1) {   /*538:*/
    flush_string();
    NAME = curname;
  }
#else
    NAME = curname;
#endif

    line = 1;
    inputln(curfile, false);
    firm_up_the_line();
    if (end_line_char_inactive) {
        LIMIT--;
    } else {
        buffer[LIMIT] = end_line_char;
    }
    first = LIMIT + 1;
    LOC = START; /*:538*/
}
/*:537*/
/** @}*/ // end group S511x538_P188x195


/*581:*/
Static void charwarning(InternalFontNumber f, EightBits c) {
    if (tracinglostchars <= 0) return;
    begindiagnostic();
    printnl(S(678));
    print(c);
    print(S(679));
    slow_print(get_fontname(f));
    print_char('!');
    enddiagnostic(false);
}
/*:581*/

/*582:*/
Static HalfWord newcharacter(InternalFontNumber f, EightBits c) {
    HalfWord Result;
    Pointer p;

    if (fontbc[f] <= c) {
        if (fontec[f] >= c) {
            if (charexists(charinfo(f, c))) {
                p = get_avail();
                font(p) = f;
                character(p) = c;
                Result = p;
                goto _Lexit;
            }
        }
    }
    charwarning(f, c);
    Result = 0;
_Lexit:
    return Result;
}
/*:582*/

/*618:*/

/*:618*/

/*619:*/




/// [ #1340. Extensions. ]

// #1368
Static void specialout(HalfWord p) {
    enum Selector old_setting;

    synchh();
    synchv();
    old_setting = selector;
    selector = NEW_STRING;
    showtokenlist(link(writetokens(p)), 0, POOL_SIZE /* - pool_ptr */);
    selector = old_setting;
    str_room(1);
    
    int p_len = cur_length(); /* XXXX - Assumes byte=ASCIICode */

    if (p_len < 256) {
        dviout_XXX1();
        dviout(p_len);
    } else {
        dviout_XXX4();
        dvi_four(p_len);
    }
    str_cur_map(dviout_helper);
} // #1368: specialout

/*1370:*/
Static void writeout(HalfWord p) { /*1371:*/
    enum Selector old_setting;
    long oldmode;
    /* SmallNumber */ int j; /* INT */
    Pointer q, r;

    q = get_avail();
    info(q) = rightbracetoken + '}';
    r = get_avail();
    link(q) = r;
    info(r) = endwritetoken;
    inslist(q);
    begintokenlist(writetokens(p), WRITE_TEXT);
    q = get_avail();
    info(q) = leftbracetoken + '{';
    inslist(q);
    oldmode = mode;
    mode = 0;
    curcs = writeloc;
    q = scantoks(false, true);
    gettoken();
    if (curtok != endwritetoken) { /*1372:*/
        print_err(S(680));
        help2(S(681), S(682));
        error();
        do {
            gettoken();
        } while (curtok != endwritetoken);
    }
    /*:1372*/
    mode = oldmode; /*:1371*/
    endtokenlist();
    old_setting = selector;
    j = writestream(p);
    if (writeopen[j])
        selector = j;
    else {
        if (j == 17 && selector == TERM_AND_LOG) selector = LOG_ONLY;
        printnl(S(385));
    }
    tokenshow(defref);
    println();
    flushlist(defref);
    selector = old_setting;
}
/*:1370*/

/*1373:*/
Static void outwhat(HalfWord p) {
    /* SmallNumber */ int j; /* INT */

    switch (subtype(p)) {
        case opennode:
        case writenode:
        case closenode:          /*1374:*/
            if (!doingleaders) { /*:1374*/
                j = writestream(p);
                if (subtype(p) == writenode)
                    writeout(p);
                else {
                    if (writeopen[j]) aclose(&write_file[j]);
                    if (subtype(p) == closenode)
                        writeopen[j] = false;
                    else if (j < 16) {
                        curname = openname(p);
                        curarea = openarea(p);
                        curext = openext(p);
                        if (curext == S(385)) curext = S(669);
                        packfilename(curname, curarea, curext);
                        while (!a_open_out(&write_file[j]))
                            promptfilename(S(683), S(669));
                        writeopen[j] = true;
                    }
                }
            }
            break;

        case specialnode: specialout(p); break;

        case languagenode:
            /* blank case */
            break;

        default: confusion(S(684)); break;
    }
}
/*:1373*/

// [ p229#619 ]: output an hlist node box
Static void hlistout(void) {
    Scaled baseline, leftedge, saveh, savev, leaderwd, lx, edge;
    Pointer thisbox, p, leaderbox;
    GlueOrd gorder;
    char gsign;
    long saveloc;
    Boolean outerdoingleaders;
    double gluetemp;

    thisbox = temp_ptr;
    gorder = glueorder(thisbox);
    gsign = gluesign(thisbox);
    p = listptr(thisbox);
    curs++;

    if (curs > 0) dviout_PUSH();
    if (curs > maxpush) maxpush = curs;
    saveloc = get_dvi_mark();
    baseline = curv;
    leftedge = curh;

    while (p != 0) {
        // #620 
        // Output node p for hlist out and move to the next node, 
        // maintaining the condition cur v = base line
    _LN_hlistout__reswitch:
        if (ischarnode(p)) {
            synchh();
            synchv();
            do {
                QuarterWord f = font(p);
                QuarterWord c = character(p);
                if (f != dvif) { /*621:*/
                    dvi_set_font(f);
                    dvif = f;
                }
                /*:621*/
                dvi_set_char(c);
                curh += charwidth(f, charinfo(f, c));
                p = link(p);
            } while (ischarnode(p));
            dvih = curh;
            continue; // ???
        }

        // #622:
        // Output the non-char node p for hlist out and move to the next node
        switch (type(p)) {
            case HLIST_NODE:
            case VLIST_NODE:
                // #623: Output a box in an hlist
                if (listptr(p) == 0)
                    curh += width(p);
                else { /*:623*/
                    saveh = dvih;
                    savev = dviv;
                    curv = baseline + shiftamount(p);
                    temp_ptr = p;
                    edge = curh;
                    if (type(p) == VLIST_NODE)
                        vlistout();
                    else
                        hlistout();
                    dvih = saveh;
                    dviv = savev;
                    curh = edge + width(p);
                    curv = baseline;
                }
                break;

            case RULE_NODE:
                ruleht = height(p);
                ruledp = depth(p);
                rulewd = width(p);
                goto _Lfinrule_;
                break;

            case WHATSIT_NODE:
                // #1367: Output the whatsit node p in an hlist
                outwhat(p);
                break;

            case GLUE_NODE: {
                // #625: Move right or output leaders
                Pointer g = glueptr(p);
                rulewd = width(g);
                if (gsign != NORMAL) {
                    if (gsign == stretching) {
                        if (stretchorder(g) == gorder) {
                            vetglue(glueset(thisbox) * stretch(g));
                            rulewd += (long)floor(gluetemp + 0.5);
                        }
                    } else if (shrinkorder(g) == gorder) {
                        vetglue(glueset(thisbox) * shrink(g));
                        rulewd -= (long)floor(gluetemp + 0.5);
                    }
                }
            }
                if (subtype(p) >= aleaders) { /*626:*/
                    leaderbox = leaderptr(p);
                    if (type(leaderbox) == RULE_NODE) {
                        ruleht = height(leaderbox);
                        ruledp = depth(leaderbox);
                        goto _Lfinrule_;
                    }
                    leaderwd = width(leaderbox);
                    if (leaderwd > 0 && rulewd > 0) {
                        rulewd += 10;
                        edge = curh + rulewd;
                        lx = 0; /*627:*/
                        if (subtype(p) == aleaders) {
                            saveh = curh;
                            curh = leftedge +
                                   leaderwd * ((curh - leftedge) / leaderwd);
                            if (curh < saveh) curh += leaderwd;
                        } else { /*:627*/
                            lq = rulewd / leaderwd;
                            lr = rulewd % leaderwd;
                            if (subtype(p) == cleaders)
                                curh += lr / 2;
                            else {
                                lx = (lr * 2 + lq + 1) / (lq * 2 + 2);
                                curh += (lr - (lq - 1) * lx) / 2;
                            }
                        }
                        while (curh + leaderwd <= edge) { /*628:*/
                            curv = baseline + shiftamount(leaderbox);
                            synchv();
                            savev = dviv;
                            synchh();
                            saveh = dvih;
                            temp_ptr = leaderbox;
                            outerdoingleaders = doingleaders;
                            doingleaders = true;
                            if (type(leaderbox) == VLIST_NODE)
                                vlistout();
                            else
                                hlistout();
                            doingleaders = outerdoingleaders;
                            dviv = savev;
                            dvih = saveh;
                            curv = baseline;
                            curh = saveh + leaderwd + lx;
                        }
                        /*:628*/
                        curh = edge - 10;
                        goto _Lnextp_;
                    }
                } /*:626*/
                goto _Lmovepast_;
                break;

            case KERN_NODE:
            case MATH_NODE: curh += width(p); break;

            case LIGATURE_NODE:
                // #652: Make node p look like a char node and goto reswitch
                type(ligtrick) = charnodetype;
                font(ligtrick) = font_ligchar(p);
                character(ligtrick) = character_ligchar(p);
                link(ligtrick) = link(p);
                p = ligtrick;
                goto _LN_hlistout__reswitch;
                break;

        } // switch (type(p))
        goto _Lnextp_;

    _Lfinrule_:
        // #624: Output a rule in an hlist
        if (isrunning(ruleht)) ruleht = height(thisbox);
        if (isrunning(ruledp)) ruledp = depth(thisbox);
        ruleht += ruledp; // this is the rule thickness

        // we don’t output empty rules
        if (ruleht > 0 && rulewd > 0) {
            synchh();
            curv = baseline + ruledp;
            synchv();
            dviout_SET_RULE();
            dvi_four(ruleht);
            dvi_four(rulewd);
            curv = baseline;
            dvih += rulewd;
        }

    _Lmovepast_:
        curh += rulewd;

    _Lnextp_:
        p = link(p);
    } // while (p != 0)

    prune_movements(saveloc);
    if (curs > 0) dvi_pop(saveloc);
    curs--;
} // p229#619: hlistout


// [ p233#629 ]: output a vlist node box
Static void vlistout(void) {
    Scaled leftedge, topedge, saveh, savev, leaderht, lx, edge;
    Pointer thisbox, p, leaderbox, g;
    GlueOrd gorder;
    char gsign;
    long saveloc;
    Boolean outerdoingleaders;
    double gluetemp;

    thisbox = temp_ptr;
    gorder = glueorder(thisbox);
    gsign = gluesign(thisbox);
    p = listptr(thisbox);
    curs++;

    if (curs > 0) dviout_PUSH();
    if (curs > maxpush) maxpush = curs;
    saveloc = get_dvi_mark();
    leftedge = curh;
    curv -= height(thisbox);
    topedge = curv;

    while (p != 0) {
        // 630: 
        // Output node p for vlist out and move to the next node, 
        // maintaining the condition cur h = left edge
        if (ischarnode(p)) {
            confusion(S(685)); // "vlistout"
        } else {
            // #631: Output the non-char node p for vlist out
            switch (type(p)) {
                case HLIST_NODE:
                case VLIST_NODE:
                    // #632: Output a box in a vlist
                    if (listptr(p) == 0) {
                        curv += height(p) + depth(p);
                    } else { /*:632*/
                        curv += height(p);
                        synchv();
                        saveh = dvih;
                        savev = dviv;
                        curh = leftedge + shiftamount(p);
                        temp_ptr = p;
                        if (type(p) == VLIST_NODE) {
                            vlistout();
                        } else {
                            hlistout();
                        }
                        dvih = saveh;
                        dviv = savev;
                        curv = savev + depth(p);
                        curh = leftedge;
                    }
                    break;

                case RULE_NODE:
                    ruleht = height(p);
                    ruledp = depth(p);
                    rulewd = width(p);
                    goto _Lfinrule_;
                    break;

                case WHATSIT_NODE:
                    // #1366: Output the whatsit node p in a vlist
                    outwhat(p);
                    break;

                case GLUE_NODE:
                    // #634: Move down or output leaders
                    g = glueptr(p);
                    ruleht = width(g); // ??? - curg
                    if (gsign != NORMAL) {
                        if (gsign == stretching) {
                            if (stretchorder(g) == gorder) {
                                vetglue(glueset(thisbox) * stretch(g));
                                ruleht += (long)floor(gluetemp + 0.5);
                            }
                        } else if (shrinkorder(g) == gorder) {
                            vetglue(glueset(thisbox) * shrink(g));
                            ruleht -= (long)floor(gluetemp + 0.5);
                        }
                    } // if (gsign != NORMAL)
                    if (subtype(p) >= aleaders) { /*635:*/
                        leaderbox = leaderptr(p);
                        if (type(leaderbox) == RULE_NODE) {
                            rulewd = width(leaderbox);
                            ruledp = 0;
                            goto _Lfinrule_;
                        }
                        leaderht = height(leaderbox) + depth(leaderbox);
                        if (leaderht > 0 && ruleht > 0) {
                            ruleht += 10;
                            edge = curv + ruleht;
                            lx = 0; /*636:*/
                            if (subtype(p) == aleaders) {
                                savev = curv;
                                curv = topedge +
                                       leaderht * ((curv - topedge) / leaderht);
                                if (curv < savev) curv += leaderht;
                            } else { /*:636*/
                                lq = ruleht / leaderht;
                                lr = ruleht % leaderht;
                                if (subtype(p) == cleaders)
                                    curv += lr / 2;
                                else {
                                    lx = (lr * 2 + lq + 1) / (lq * 2 + 2);
                                    curv += (lr - (lq - 1) * lx) / 2;
                                }
                            }
                            while (curv + leaderht <= edge) { /*637:*/
                                curh = leftedge + shiftamount(leaderbox);
                                synchh();
                                saveh = dvih;
                                curv += height(leaderbox);
                                synchv();
                                savev = dviv;
                                temp_ptr = leaderbox;
                                outerdoingleaders = doingleaders;
                                doingleaders = true;
                                if (type(leaderbox) == VLIST_NODE)
                                    vlistout();
                                else
                                    hlistout();
                                doingleaders = outerdoingleaders;
                                dviv = savev;
                                dvih = saveh;
                                curh = leftedge;
                                curv =
                                    savev - height(leaderbox) + leaderht + lx;
                            }
                            /*:637*/
                            curv = edge - 10;
                            goto _Lnextp_;
                        }
                    } // if (subtype(p) >= aleaders)
                    /*:635*/
                    goto _Lmovepast_;
                    break;
                    /*:634*/

                case KERN_NODE: curv += width(p); break;
            } // switch (type(p))
            goto _Lnextp_;
        
        _Lfinrule_:
            // #633: Output a rule in a vlist, goto next p
            if (isrunning(rulewd)) {
                rulewd = width(thisbox);
            }
            ruleht += ruledp;
            curv += ruleht;
            if (ruleht > 0 && rulewd > 0) {
                synchh();
                synchv();
                dviout_PUT_RULE();
                dvi_four(ruleht);
                dvi_four(rulewd);
            }
            goto _Lnextp_;

        _Lmovepast_:
            curv += ruleht;
        } // if (ischarnode(p)) - else
    _Lnextp_:
        p = link(p);
    } // while (p != 0)

    prune_movements(saveloc);
    if (curs > 0) dvi_pop(saveloc);
    curs--;
} // #629: vlistout


// [ p236#638 ]: output the box `p`
Static void shipout(Pointer p) {
    int j, k; // [0, 9]: indices to first ten count registers
    enum Selector old_setting; // saved selector setting

    if (tracingoutput > 0) {
        printnl(S(385));
        println();
        print(S(686));
    }
    if (term_offset > (MAX_PRINT_LINE - 9)) {
        println();
    } else if (term_offset > 0 || file_offset > 0) {
        print_char(' ');
    }
    print_char('[');
    j = 9;
    while (count(j) == 0 && j > 0)
        j--;
    for (k = 0; k <= j; k++) {
        print_int(count(k));
        if (k < j) print_char('.');
    }
    fflush(stdout); // update_terminal

    if (tracingoutput > 0) {
        print_char(']');
        begindiagnostic();
        showbox(p);
        enddiagnostic(true);
    }

    /// [ #640 ]: Ship box p out
    // [#641]: Update the values of max h and max v; 
    // but if the page is too large, goto done
    if (   (height(p) > MAX_DIMEN) 
        || (depth(p) > MAX_DIMEN) 
        || ((height(p) + depth(p) + voffset) > MAX_DIMEN) 
        || ((width(p) + hoffset) > MAX_DIMEN) ) {
        print_err(S(687));
        help2(S(688), S(689));
        error();
        if (tracingoutput <= 0) {
            begindiagnostic();
            printnl(S(690));
            showbox(p);
            enddiagnostic(true);
        }
        goto _L_shipout_done;
    }
    if ((height(p) + depth(p) + voffset) > maxv)
        maxv = height(p) + depth(p) + voffset;
    if ((width(p) + hoffset) > maxh)
        maxh = width(p) + hoffset;

    // [ #617 ]: Initialize variables as ship out begins
    dvih = 0;
    dviv = 0;
    curh = hoffset;
    dvif = NULL_FONT;
    // ensure dvi open
    if (output_file_name == 0) {
        if (job_name == 0) openlogfile();
        packjobname(S(691));
        while (!dvi_openout())
            promptfilename(S(692), S(691));
        output_file_name = bmakenamestring();
    }
    if (totalpages == 0) {
        // output the preamble
        dviout_PRE();
        dviout_ID_BYTE();
        // conversion ratio for sp
        dvi_four(25400000L);
        dvi_four(473628672L);
        // magnification factor is frozen
        preparemag();
        dvi_four(mag);
        
        old_setting = selector;
        selector = NEW_STRING;
        print(S(693)); // " TeX output "
        print_int(year);
        print_char('.');
        print_two(month);
        print_char('.');
        print_two(day);
        print_char(':');
        print_two(tex_time / 60);
        print_two(tex_time % 60);
        selector = old_setting;
        dviout(cur_length());
        // flush the current string
        str_cur_map(dviout_helper);
    }
    {
        long cp[10];
        for (k = 0; k <= 9; k++) {
            cp[k] = count(k);
        }
        _dvibop(cp); // BOP 相关变量处理
    }
    curv = height(p) + voffset;
    temp_ptr = p;
    if (type(p) == VLIST_NODE) {
        vlistout();
    } else {
        hlistout();
    }
    dviout_EOP();
    totalpages++;
    curs = -1;

// [p236#638]
_L_shipout_done:
    if (tracingoutput <= 0) print_char(']');
    deadcycles = 0;
    fflush(stdout); // progress report

    /// [p236#639]
    /// Flush the box from memory, showing statistics if requested
    #ifdef tt_STAT
        if (tracingstats > 1) {
            printnl(S(694)); // "Memory usage before: "
            print_int(var_used);
            print_char('&');
            print_int(dyn_used);
            print_char(';');
        }
    #endif // #639.1: tt_STAT

    flush_node_list(p);

    #ifdef tt_STAT
        if (tracingstats > 1) {
            print(S(695)); // " after: "
            print_int(var_used);
            print_char('&');
            print_int(dyn_used);
            print(S(696)); // "; still untouched: "
            print_int(hi_mem_min - lo_mem_max - 1);
            println();
        }
    #endif // #639.2: tt_STAT
} // [ p236#638 ]:shipout


/*
 *  [ p239~248#644~679 Packaging. ]
 *  the real work of typesetting
 * 
 */

// ##645
Static void scanspec(GroupCode c, Boolean threecodes) {
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
        saveptr++;
    }
    saved(0) = speccode;
    saved(1) = cur_val;
    saveptr += 2;
    newsavelevel(c);
    scan_left_brace();
} // #645: scanspec

// #649
Static HalfWord hpack(HalfWord p, long w, SmallNumber m) {
    Pointer r, q, g;
    Scaled h, d, x, s;
    GlueOrd o;
    EightBits hd;

    lastbadness = 0;
    r = getnode(boxnodesize);
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
                        freenode(link(q), smallnodesize);
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
    begindiagnostic();
    showbox(r); /*:663*/
    enddiagnostic(true);
_Lexit:
    return r;

    /*:657*/
} // #649: hpack

// #668
Static HalfWord vpackage(HalfWord p, long h, SmallNumber m, long l) {
    Pointer r, g;
    Scaled w, d, x, s;
    GlueOrd o;

    lastbadness = 0;
    r = getnode(boxnodesize);
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
        if (ischarnode(p))
            confusion(S(710));
        else {
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
    begindiagnostic();
    showbox(r); /*:675*/
    enddiagnostic(true);
_Lexit:
    return r;

    /*:672*/
} // #668: vpackage

// #679
Static void appendtovlist(HalfWord b) {
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


// #686
Static HalfWord newnoad(void) {
    Pointer p;
    int i = 0;
    p = getnode(noadsize);
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
Static HalfWord newstyle(SmallNumber s) {
    Pointer p;

    p = getnode(stylenodesize);
    type(p) = stylenode;
    subtype(p) = s;
    width(p) = 0;
    depth(p) = 0;
    return p;
} /*:688*/


/*689:*/
Static HalfWord newchoice(void) {
    Pointer p;

    p = getnode(stylenodesize);
    type(p) = choicenode;
    subtype(p) = 0;
    displaymlist(p) = 0;
    textmlist(p) = 0;
    scriptmlist(p) = 0;
    scriptscriptmlist(p) = 0;
    return p;
}
/*:689*/

/*693:*/
Static void showinfo(void) { shownodelist(info(temp_ptr)); }
/*:693*/


/// [ #699. Subroutines for math mode.  ]

/*704:*/
Static HalfWord fractionrule(long t) {
    Pointer p;

    p = newrule();
    height(p) = t;
    depth(p) = 0;
    return p;
}
/*:704*/

/*705:*/
Static HalfWord overbar(HalfWord b, long k, long t) {
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
Static HalfWord charbox(InternalFontNumber f, QuarterWord c) {
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
Static void stackintobox(HalfWord b, InternalFontNumber f, QuarterWord c) {
    Pointer p;

    p = charbox(f, c);
    link(p) = listptr(b);
    listptr(b) = p;
    height(b) = height(p);
}
/*:711*/

/*712:*/
Static Integer heightplusdepth(InternalFontNumber f, QuarterWord c) {
    FourQuarters q;
    EightBits hd;

    q = charinfo(f, c);
    hd = heightdepth(q);
    return (charheight(f, hd) + chardepth(f, hd));
} /*:712*/


Static HalfWord vardelimiter(HalfWord d, SmallNumber s, long v) {
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
                g = famfnt(z);
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
Static HalfWord rebox(HalfWord b, long w) {
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
        freenode(b, boxnodesize);
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
Static HalfWord mathglue(HalfWord g, long m) {
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
    p = getnode(gluespecsize);
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
Static void mathkern(HalfWord p, long m) {
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
Static void flushmath(void) {
    flush_node_list(link(head));
    flush_node_list(incompleatnoad);
    link(head) = 0;
    tail = head;
    incompleatnoad = 0;
}
/*:718*/

/// [ #719. Typesetting math formulas. ]

/*720:*/

Static HalfWord cleanbox(HalfWord p, SmallNumber s) {
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
        freenode(r, smallnodesize);
        link(q) = 0;
    }
    return x;
}
/*:720*/

/*722:*/
Static void fetch(HalfWord a) {
    curc = character(a);
    curf = famfnt(fam(a) + cursize);
    if (curf == NULL_FONT) { /*723:*/
        print_err(S(385));
        print_size(cursize);
        print_char(' ');
        print_int(fam(a));
        print(S(715));
        print(curc - MIN_QUARTER_WORD);
        print_char(')');
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
Static void makeover(HalfWord q) {
    info(nucleus(q)) = overbar(cleanbox(nucleus(q), crampedstyle(curstyle)),
                               defaultrulethickness * 3,
                               defaultrulethickness);
    mathtype(nucleus(q)) = subbox;
} // #734: makeover

// #735: makeunder
Static void makeunder(HalfWord q) {
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
Static void makevcenter(HalfWord q) {
    Pointer v;
    Scaled delta;

    v = info(nucleus(q));
    if (type(v) != VLIST_NODE) confusion(S(415));
    delta = height(v) + depth(v);
    height(v) = axisheight(cursize) + half(delta);
    depth(v) = delta - height(v);
} // #736: makevcenter

// #737
Static void makeradical(HalfWord q) {
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
Static void makemathaccent(HalfWord q) {
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
Static void makefraction(HalfWord q) {
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
Static Integer makeop(HalfWord q) {
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
        freenode(x, boxnodesize);
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
        freenode(z, boxnodesize);
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
Static void makeord(HalfWord q) {
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
                                                            freenode(p,
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
Static void makescripts(HalfWord q, long delta) {
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
        freenode(z, boxnodesize);
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
Static SmallNumber makeleftright(HalfWord q,
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


Static void mlisttohlist(void) {
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

            default: confusion(S(720)); break;
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

            default: confusion(S(721)); break;
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
        freenode(z, boxnodesize);
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

            default: confusion(S(722)); break;
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

                default: confusion(S(723)); break;
            }
            if (x != 0) {
                y = mathglue(gluepar(x), curmu);
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
        freenode(r, s);
    _Ldone:;
    }
    /*:760*/

    /*:732*/
}
/*:726*/

/// [ #768. Alignment. ]
/*772:*/
Static void pushalignment(void) {
    Pointer p;

    p = getnode(alignstacknodesize);
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


Static void popalignment(void) {
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
    freenode(p, alignstacknodesize);
}
/*:772*/

/*774:*/
/*782:*/
Static void getpreambletoken(void) {
_Lrestart:
    gettoken();
    while (curchr == spancode && curcmd == TAB_MARK) {
        gettoken();
        if (curcmd > MAX_COMMAND) {
            expand();
            gettoken();
        }
    }
    if (curcmd == ENDV) fatalerror(S(509));
    if (curcmd != ASSIGN_GLUE || curchr != gluebase + TAB_SKIP_CODE) return;
    scan_optional_equals();
    scan_glue(GLUE_VAL);
    if (globaldefs > 0)
        geqdefine(gluebase + TAB_SKIP_CODE, GLUE_REF, cur_val);
    else
        eqdefine(gluebase + TAB_SKIP_CODE, GLUE_REF, cur_val);
    goto _Lrestart;
}
/*:782*/

Static void initalign(void) {
    Pointer savecsptr, p;

    savecsptr = curcs;
    pushalignment();
    align_state = -1000000L;                                        /*776:*/
    if (mode == M_MODE && (tail != head || incompleatnoad != 0)) { /*:776*/
        print_err(S(597));
        print_esc(S(724));
        print(S(725));
        help3(S(726), S(727), S(728));
        error();
        flushmath();
    }
    pushnest(); /*775:*/
    if (mode == M_MODE) {
        mode = -V_MODE;
        prevdepth = nest[nest_ptr - 2].auxfield.sc;
    } else if (mode > 0)
        mode = -mode; /*:775*/
    scanspec(aligngroup, false);
    /*777:*/
    preamble = 0;
    curalign = alignhead;
    curloop = 0;
    scanner_status = ALIGNING;
    warning_index = savecsptr;
    align_state = -1000000L;
    while (true) { /*778:*/
        link(curalign) = newparamglue(TAB_SKIP_CODE);
        curalign = link(curalign); /*:778*/
        if (curcmd == CAR_RET) goto _Ldone;
        /*779:*/
        /*783:*/
        p = holdhead;
        link(p) = 0;
        while (true) {
            getpreambletoken();
            if (curcmd == MAC_PARAM) goto _Ldone1;
            if (curcmd <= CAR_RET && curcmd >= TAB_MARK &&
                align_state == -1000000L) {
                if (p == holdhead && curloop == 0 && curcmd == TAB_MARK) {
                    curloop = curalign;
                    continue;
                } else {
                    print_err(S(729));
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
        }
    _Ldone1: /*:783*/
        link(curalign) = newnullbox();
        curalign = link(curalign);
        info(curalign) = endspan;
        width(curalign) = nullflag;
        upart(curalign) = link(holdhead); /*784:*/
        p = holdhead;
        link(p) = 0;
        while (true) {
        _Llabcontinue:
            getpreambletoken();
            if (curcmd <= CAR_RET && curcmd >= TAB_MARK &&
                align_state == -1000000L)
                goto _Ldone2;
            if (curcmd == MAC_PARAM) {
                print_err(S(733));
                help3(S(730), S(731), S(734));
                error();
                goto _Llabcontinue;
            }
            link(p) = get_avail();
            p = link(p);
            info(p) = curtok;
        }
    _Ldone2:
        link(p) = get_avail();
        p = link(p);
        info(p) = endtemplatetoken;       /*:784*/
        vpart(curalign) = link(holdhead); /*:779*/
    }
_Ldone:
    scanner_status = NORMAL; /*:777*/
    newsavelevel(aligngroup);
    if (everycr != 0) begintokenlist(everycr, EVERY_CR_TEXT);
    alignpeek();
}
/*:774*/

/*786:*/
/*787:*/
Static void initspan(HalfWord p) {
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

Static void initrow(void) {
    pushnest();
    mode = -H_MODE - V_MODE - mode;
    if (mode == -H_MODE)
        spacefactor = 0;
    else
        prevdepth = 0;
    tailappend(newglue(glueptr(preamble)));
    subtype(tail) = TAB_SKIP_CODE + 1;
    curalign = link(preamble);
    curtail = curhead;
    initspan(curalign);
}
/*:786*/

/*788:*/
Static void initcol(void) {
    extrainfo(curalign) = curcmd;
    if (curcmd == OMIT)
        align_state = 0;
    else {
        backinput();
        begintokenlist(upart(curalign), U_TEMPLATE);
    }
}
/*:788*/

/*791:*/
Static Boolean fincol(void) {
    Boolean Result;
    Pointer p, q, r, s, u;
    Scaled w;
    GlueOrd o;
    HalfWord n;

    if (curalign == 0) confusion(S(735));
    q = link(curalign);
    if (q == 0) confusion(S(735));
    if (align_state < 500000L) fatalerror(S(509));
    p = link(q);
    /*792:*/
    if ((p == 0) & (extrainfo(curalign) < crcode)) {
        if (curloop != 0) { /*793:*/
            link(q) = newnullbox();
            p = link(q);
            info(p) = endspan;
            width(p) = nullflag;
            curloop = link(curloop); /*794:*/
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
            vpart(p) = link(holdhead); /*:794*/
            curloop = link(curloop);
            link(p) = newglue(glueptr(curloop));
        } else { /*:792*/
            print_err(S(736));
            print_esc(S(737));
            help3(S(738), S(739), S(740));
            extrainfo(curalign) = crcode;
            error();
        }
        /*:793*/
    }
    if (extrainfo(curalign) != spancode) {
        unsave();
        newsavelevel(aligngroup); /*796:*/
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
        n = MIN_QUARTER_WORD;
        if (curspan != curalign) { /*798:*/
            q = curspan;
            do {
                n++;
                q = link(link(q));
            } while (q != curalign);
            if (n > MAX_QUARTER_WORD) confusion(S(741));
            q = curspan;
            while (link(info(q)) < n)
                q = info(q);
            if (link(info(q)) > n) {
                s = getnode(spannodesize);
                info(s) = info(q);
                link(s) = n;
                info(q) = s;
                width(s) = w;
            } else if (width(info(q)) < w)
                width(info(q)) = w;
        } else if (w > width(curalign)) /*:798*/
            width(curalign) = w;
        type(u) = UNSET_NODE;
        spancount(u) = n; /*659:*/
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
        glueorder(u) = o;
        gluestretch(u) = totalstretch[o - NORMAL]; /*665:*/
        if (totalshrink[FILLL - NORMAL] != 0)
            o = FILLL;
        else if (totalshrink[FILL - NORMAL] != 0)
            o = FILL;
        else if (totalshrink[FIL - NORMAL] != 0)
            o = FIL;
        else
            o = NORMAL; /*:665*/
        gluesign(u) = o;
        glueshrink(u) = totalshrink[o - NORMAL];
        popnest();
        link(tail) = u;
        tail = u; /*:796*/
        /*795:*/
        tailappend(newglue(glueptr(link(curalign))));
        subtype(tail) = TAB_SKIP_CODE + 1; /*:795*/
        if (extrainfo(curalign) >= crcode) {
            Result = true;
            goto _Lexit;
        }
        initspan(p);
    }
    align_state = 1000000L;
    skip_spaces();
    curalign = p;
    initcol();
    Result = false;
_Lexit:
    return Result;
}
/*:791*/

/*799:*/
Static void finrow(void) {
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

Static void finalign(void) {
    Pointer p, q, r, s, u, v;
    Scaled t, w, o, rulesave;
    HalfWord n;
    MemoryWord auxsave;

    if (curgroup != aligngroup) confusion(S(742));
    unsave();
    if (curgroup != aligngroup) confusion(S(743));
    unsave();
    if (nest[nest_ptr - 1].modefield == M_MODE)
        o = displayindent;
    else
        o = 0;
    /*801:*/
    q = link(preamble);
    do { /*804:*/
        flushlist(upart(q));
        flushlist(vpart(q));
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
                    freenode(r, spannodesize);
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
    saveptr -= 2;
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
        if (!ischarnode(q)) {
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
                        link(u) = newglue(v);
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
                        link(u) = newnullbox();
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
            print_err(S(744));
            help2(S(726), S(727));
            backerror();
        } else { /*1197:*/
            get_x_token();
            if (curcmd != MATH_SHIFT) {
                print_err(S(745));
                help2(S(746), S(747));
                backerror();
            }
        }
        /*:1207*/
        popnest();
        tailappend(newpenalty(predisplaypenalty));
        tailappend(newparamglue(ABOVE_DISPLAY_SKIP_CODE));
        link(tail) = p;
        if (p != 0) tail = q;
        tailappend(newpenalty(postdisplaypenalty));
        tailappend(newparamglue(BELOW_DISPLAY_SKIP_CODE));
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


Static void alignpeek(void) {
_Lrestart:
    align_state = 1000000L;
    skip_spaces();
    if (curcmd == NO_ALIGN) {
        scan_left_brace();
        newsavelevel(noaligngroup);
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


/// [ #813. Breaking paragraphs into lines. ]

/*815:*/
/*826:*/
Static HalfWord finiteshrink(HalfWord p) {
    Pointer q;

    if (noshrinkerroryet) {
        noshrinkerroryet = false;
        print_err(S(748));
        help5(S(749), S(750), S(751), S(752), S(753));
        error();
    }
    q = newspec(p);
    shrinkorder(q) = NORMAL;
    delete_glue_ref(p);
    return q;
} /*:826*/


/// p308#829
Static void trybreak(long pi, SmallNumber breaktype) { /*831:*/
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
                                        confusion(S(754));
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
                                            confusion(S(755));
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
                    q = getnode(deltanodesize);
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
                        q = getnode(passivenodesize);
                        link(q) = passive;
                        passive = q;
                        curbreak(q) = curp;
                        #ifdef tt_STAT
                            passnumber++;
                            serial(q) = passnumber;
                        #endif // #845.1: tt_STAT
                        prevbreak(q) = bestplace[fitclass - veryloosefit];
                        q = getnode(activenodesize);
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
                    q = getnode(deltanodesize);
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
        freenode(r, activenodesize);
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
            freenode(r, deltanodesize);
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
            freenode(prevr, deltanodesize);
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
        freenode(r, deltanodesize);
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


/*877:*/
Static void postlinebreak(long finalwidowpenalty) { /*878:*/
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
    if ((curline != bestline) | (link(temphead) != 0)) confusion(S(766));
    prevgraf = bestline - 1;
}
/*:877*/

/*895:*/
/*906:*/
Static SmallNumber reconstitute(/* SmallNumber */ int j,
                                SmallNumber n,
                                HalfWord bchar,
                                HalfWord hchar) {
    Pointer p, t;
    FourQuarters q;
    HalfWord currh, testchar;
    Scaled w;
    FontIndex k;

    hyphenpassed = 0;
    t = holdhead;
    w = 0;
    link(holdhead) = 0; /*908:*/
    curl = hu[j];
    curq = t;
    if (j == 0) {
        ligaturepresent = initlig;
        p = initlist;
        if (ligaturepresent) lfthit = initlft;
        while (p > 0) {
            appendcharnodetot(character(p));
            p = link(p);
        }
    } else if (curl < NON_CHAR) {
        appendcharnodetot(curl);
    }
    ligstack = 0; /*:908*/
    setcurr();
_Llabcontinue: /*909:*/
    if (curl == NON_CHAR) {
        k = bcharlabel[hf];
        if (k == nonaddress) goto _Ldone;
        q = fontinfo[k].qqqq;
    } else {
        q = charinfo(hf, curl);
        if (chartag(q) != LIG_TAG) {
            goto _Ldone;
        }
        k = ligkernstart(hf, q);
        q = fontinfo[k].qqqq;
        if (skipbyte(q) > stopflag) {
            k = ligkernrestart(hf, q);
            q = fontinfo[k].qqqq;
        }
    }
    if (currh < NON_CHAR)
        testchar = currh;
    else
        testchar = curr;
    while (true) {
        if (nextchar(q) == testchar) {
            if (skipbyte(q) <= stopflag) {
                if (currh < NON_CHAR) {
                    hyphenpassed = j;
                    hchar = NON_CHAR;
                    currh = NON_CHAR;
                    goto _Llabcontinue;
                } else {
                    if (hchar < NON_CHAR) {
                        if (hyf[j] & 1) {
                            hyphenpassed = j;
                            hchar = NON_CHAR;
                        }
                    }
                    if (opbyte(q) < kernflag) { /*911:*/
                        if (curl == NON_CHAR) lfthit = true;
                        if (j == n) {
                            if (ligstack == 0) rthit = true;
                        }
                        checkinterrupt();
                        switch (opbyte(q)) {

                            case MIN_QUARTER_WORD + 1:
                            case MIN_QUARTER_WORD + 5:
                                curl = rembyte(q);
                                ligaturepresent = true;
                                break;

                            case MIN_QUARTER_WORD + 2:
                            case MIN_QUARTER_WORD + 6:
                                curr = rembyte(q);
                                if (ligstack > 0)
                                    character(ligstack) = curr;
                                else {
                                    ligstack = newligitem(curr);
                                    if (j == n)
                                        bchar = NON_CHAR;
                                    else {
                                        p = get_avail();
                                        ligptr(ligstack) = p;
                                        character(p) = hu[j + 1];
                                        font(p) = hf;
                                    }
                                }
                                break;

                            case MIN_QUARTER_WORD + 3:
                                curr = rembyte(q);
                                p = ligstack;
                                ligstack = newligitem(curr);
                                link(ligstack) = p;
                                break;

                            case MIN_QUARTER_WORD + 7:
                            case MIN_QUARTER_WORD + 11:
                                wraplig(false);
                                curq = t;
                                curl = rembyte(q);
                                ligaturepresent = true;
                                break;

                            default:
                                curl = rembyte(q);
                                ligaturepresent = true;
                                if (ligstack > 0) {
                                    popligstack();
                                } else if (j == n)
                                    goto _Ldone;
                                else {
                                    appendcharnodetot(curr);
                                    j++;
                                    setcurr();
                                }
                                break;
                        }
                        if (opbyte(q) > MIN_QUARTER_WORD + 4) {
                            if (opbyte(q) != MIN_QUARTER_WORD + 7) goto _Ldone;
                        }
                        goto _Llabcontinue;
                    }
                    /*:911*/
                    w = charkern(hf, q);
                    goto _Ldone;
                }
            }
        }
        if (skipbyte(q) >= stopflag) {
            if (currh == NON_CHAR)
                goto _Ldone;
            else {
                currh = NON_CHAR;
                goto _Llabcontinue;
            }
        }
        k += skipbyte(q) - MIN_QUARTER_WORD + 1;
        q = fontinfo[k].qqqq;
    }
_Ldone: /*:909*/
    /*910:*/
    wraplig(rthit);
    if (w != 0) {
        link(t) = newkern(w);
        t = link(t);
        w = 0;
    }
    if (ligstack <= 0) /*:910*/
        return j;
    curq = t;
    curl = character(ligstack);
    ligaturepresent = true;
    popligstack();
    goto _Llabcontinue;
}
/*:906*/

Static void hyphenate(void) /*:929*/
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
        freenode(ha, smallnodesize);
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
                r = getnode(smallnodesize);
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
    flushlist(initlist);
_Lexit:;
}
/*:895*/


/*
 * [p350#942]: Declare procedures for preprocessing hyphenation patterns
 * 
 * xref[8]
 *  944, 948, 949, 953, 957, 
 *  959, 960, 966.
 */

#ifdef tt_INIT

/// p351#944
Static QuarterWord newtrieop(SmallNumber d, SmallNumber n, QuarterWord v) {
    QuarterWord Result;
    short h;
    QuarterWord u;
    short l;

    h = abs(n + d * 313 + v * 361 + curlang * 1009) %
            (TRIE_OP_SIZE + TRIE_OP_SIZE) -
        TRIE_OP_SIZE;
    while (true) {
        l = trieophash[h + TRIE_OP_SIZE];
        if (l == 0) {
            if (trieopptr == TRIE_OP_SIZE) overflow(S(767), TRIE_OP_SIZE);
            u = trieused[curlang];
            if (u == MAX_QUARTER_WORD)
                overflow(S(768), MAX_QUARTER_WORD - MIN_QUARTER_WORD);
            trieopptr++;
            u++;
            trieused[curlang] = u;
            hyfdistance[trieopptr - 1] = d;
            hyfnum[trieopptr - 1] = n;
            hyfnext[trieopptr - 1] = v;
            trieoplang[trieopptr - 1] = curlang;
            trieophash[h + TRIE_OP_SIZE] = trieopptr;
            trieopval[trieopptr - 1] = u;
            Result = u;
            break;
        }
        if (hyfdistance[l - 1] == d && hyfnum[l - 1] == n &&
            hyfnext[l - 1] == v && trieoplang[l - 1] == curlang) {
            Result = trieopval[l - 1];
            break;
        }
        if (h > -TRIE_OP_SIZE)
            h--;
        else
            h = TRIE_OP_SIZE;
    } // while (true)

    return Result;
} // #944: newtrieop

/// #948
Static TriePointer trienode(TriePointer p) {
    TriePointer Result, h, q;

    h = abs(triec[p] + trieo[p] * 1009 + triel[p] * 2718 + trier[p] * 3142) %
       TRIE_SIZE;
    while (true) {
        q = triehash[h];
        if (q == 0) {
            triehash[h] = p;
            Result = p;
            break;
        }
        if (triec[q] == triec[p] && trieo[q] == trieo[p] &&
            triel[q] == triel[p] && trier[q] == trier[p]) {
            Result = q;
            break;
        }
        if (h > 0)
            h--;
        else
            h =TRIE_SIZE;
    } // while (true)

    return Result;
} // #948: trienode

/// #949
Static TriePointer compresstrie(TriePointer p) {
    if (p == 0) {
        return 0;
    } else {
        triel[p] = compresstrie(triel[p]);
        trier[p] = compresstrie(trier[p]);
        return (trienode(p));
    }
} // #949: compresstrie

/// #953
Static void firstfit(TriePointer p) {
    TriePointer h, z, q;
    ASCIICode c;
    TriePointer l, r;
    short ll;

    c = triec[p];
    z = triemin[c];
    while (true) {
        h = z - c;               /*954:*/
        if (triemax < h + 256) { /*:954*/
            if (TRIE_SIZE <= h + 256) overflow(S(769), TRIE_SIZE);
            do {
                triemax++;
                P_clrbits_B(trietaken, triemax - 1, 0, 3);
                trielink(triemax) = triemax + 1;
                trieback(triemax) = triemax - 1;
            } while (triemax != h + 256);
        }
        if (P_getbits_UB(trietaken, h - 1, 0, 3)) /*955:*/
            goto _Lnotfound;
        q = trier[p];
        while (q > 0) {
            if (trielink(h + triec[q]) == 0) goto _Lnotfound;
            q = trier[q];
        }
        goto _Lfound; /*:955*/

    _Lnotfound:
        z = trielink(z);
    } // while (true)

_Lfound: /*956:*/
    P_putbits_UB(trietaken, h - 1, 1, 0, 3);
    triehash[p] = h;
    q = p;
    do {
        z = h + triec[q];
        l = trieback(z);
        r = trielink(z);
        trieback(r) = l;
        trielink(l) = r;
        trielink(z) = 0;
        if (l < 256) {
            if (z < 256) {
                ll = z;
            } else {
                ll = 256;
            }
            do {
                triemin[l] = r;
                l++;
            } while (l != ll);
        }
        q = trier[q]; /*:956*/
    } while (q != 0);
} // #953: firstfit

/// #957
Static void triepack(TriePointer p) {
    TriePointer q;

    do {
        q = triel[p];
        if (q > 0 && triehash[q] == 0) {
            firstfit(q);
            triepack(q);
        }
        p = trier[p];
    } while (p != 0);
} // #957: triepack

/// #959
Static void triefix(TriePointer p) {
    TriePointer q;
    ASCIICode c;
    TriePointer z;

    z = triehash[p];
    do {
        q = triel[p];
        c = triec[p];
        trielink(z + c) = triehash[q];
        triechar(z + c) = c;
        trieop(z + c) = trieo[p];
        if (q > 0) triefix(q);
        p = trier[p];
    } while (p != 0);
} // #959: triefix

/// #960
Static void newpatterns(void) {
    unsigned char k, l; /* INT */
    Boolean digitsensed;
    QuarterWord v;
    TriePointer p, q;
    Boolean firstchild;
    ASCIICode c;

    if (trie_not_ready) {
        setcurlang();
        scan_left_brace();
        /*961:*/
        k = 0;
        hyf[0] = 0;
        digitsensed = false;
        while (true) {
            get_x_token();
            switch (curcmd) {
                case LETTER:
                case OTHER_CHAR: /*962:*/
                    if (digitsensed | (curchr < '0') | (curchr > '9')) {
                        int cur_chr;
                        if (curchr == '.') {
                            cur_chr = 0;
                        } else {
                            cur_chr = lccode(curchr);
                            if (cur_chr == 0) {
                                print_err(S(770));
                                help1(S(771));
                                error();
                            }
                        }
                        if (k < 63) {
                            k++;
                            hc[k] = cur_chr;
                            hyf[k] = 0;
                            digitsensed = false;
                        }
                    } else if (k < 63) {
                        hyf[k] = curchr - '0';
                        digitsensed = true;
                    }
                    break;

                case SPACER:
                case RIGHT_BRACE:
                    if (k > 0) { /*963:*/
                        if (hc[1] == 0) hyf[0] = 0;
                        if (hc[k] == 0) hyf[k] = 0;
                        l = k;
                        v = MIN_QUARTER_WORD;
                        while (true) {
                            if (hyf[l] != 0) v = newtrieop(k - l, hyf[l], v);
                            if (l <= 0) goto _Ldone1;
                            l--;
                        }
                    _Ldone1: /*:965*/
                        q = 0;
                        hc[0] = curlang;
                        while (l <= k) {
                            c = hc[l];
                            l++;
                            p = triel[q];
                            firstchild = true;
                            while (p > 0 && c > triec[p]) {
                                q = p;
                                p = trier[q];
                                firstchild = false;
                            }
                            if (p == 0 || c < triec[p]) { /*964:*/
                                if (trieptr == TRIE_SIZE)
                                    overflow(S(769), TRIE_SIZE);
                                trieptr++;
                                trier[trieptr] = p;
                                p = trieptr;
                                triel[p] = 0;
                                if (firstchild)
                                    triel[q] = p;
                                else
                                    trier[q] = p;
                                triec[p] = c;
                                trieo[p] = MIN_QUARTER_WORD;
                            }
                            /*:964*/
                            q = p;
                        }
                        if (trieo[q] != MIN_QUARTER_WORD) {
                            print_err(S(772));
                            help1(S(771));
                            error();
                        }
                        trieo[q] = v;
                    }
                    /*965:*/
                    /*:963*/
                    if (curcmd == RIGHT_BRACE) goto _Ldone;
                    k = 0;
                    hyf[0] = 0;
                    digitsensed = false;
                    break;

                default:
                    print_err(S(773));
                    print_esc(S(774));
                    help1(S(771));
                    error();
                    break;
            } // switch (curcmd)
        } // while (true)
    _Ldone: /*:961*/
        return;
    } // if (trie_not_ready)

    print_err(S(775));
    print_esc(S(774));
    help1(S(776));
    error();
    link(garbage) = scantoks(false, false);
    flushlist(defref);
    /*:962*/
} // #960: newpatterns

/// #966
Static void inittrie(void) { /*952:*/
    TriePointer p;
    long j, k, t;
    TriePointer r, s;
    TwoHalves h;

    /*945:*/
    opstart[0] = -MIN_QUARTER_WORD;
    for (j = 1; j <= 255; j++)
        opstart[j] = opstart[j - 1] + trieused[j - 1] - MIN_QUARTER_WORD;
    for (j = 1; j <= trieopptr; j++)
        trieophash[j + TRIE_OP_SIZE] =
            opstart[trieoplang[j - 1]] + trieopval[j - 1];
    for (j = 1; j <= trieopptr; j++) {
        while (trieophash[j + TRIE_OP_SIZE] > j) { /*:945*/
            k = trieophash[j + TRIE_OP_SIZE];
            t = hyfdistance[k - 1];
            hyfdistance[k - 1] = hyfdistance[j - 1];
            hyfdistance[j - 1] = t;
            t = hyfnum[k - 1];
            hyfnum[k - 1] = hyfnum[j - 1];
            hyfnum[j - 1] = t;
            t = hyfnext[k - 1];
            hyfnext[k - 1] = hyfnext[j - 1];
            hyfnext[j - 1] = t;
            trieophash[j + TRIE_OP_SIZE] = trieophash[k + TRIE_OP_SIZE];
            trieophash[k + TRIE_OP_SIZE] = k;
        }
    }
    for (p = 0; p <= TRIE_SIZE; p++)
        triehash[p] = 0;
    trieroot = compresstrie(trieroot);
    for (p = 0; p <= trieptr; p++)
        triehash[p] = 0;
    for (p = 0; p <= 255; p++)
        triemin[p] = p + 1;
    trielink(0) = 1;
    triemax = 0; /*:952*/
    if (trieroot != 0) {
        firstfit(trieroot);
        triepack(trieroot);
    }
    /*958:*/
    h.rh = 0;
    h.UU.U2.b0 = MIN_QUARTER_WORD;
    h.UU.U2.b1 = MIN_QUARTER_WORD;
    if (trieroot == 0) {
        for (r = 0; r <= 256; r++)
            trie[r] = h;
        triemax = 256;
    } else {
        triefix(trieroot);
        r = 0;
        do {
            s = trielink(r);
            trie[r] = h;
            r = s;
        } while (r <= triemax);
    }
    triechar(0) = '?'; /*:958*/
    trie_not_ready = false;
} // #966: inittrie

#endif // #942: tt_INIT


Static void linebreak(long finalwidowpenalty) {
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
        q = getnode(activenodesize);
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
                                if (lccode(c) != 0) {
                                    if (lccode(c) == c || uchyph > 0)
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
                                    if (lccode(c) == 0) goto _Ldone3;
                                    if (hn == 63) goto _Ldone3;
                                    hb = s;
                                    hn++;
                                    hu[hn] = c;
                                    hc[hn] = lccode(c);
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
                                        if (lccode(c) == 0) goto _Ldone3;
                                        if (j == 63) goto _Ldone3;
                                        j++;
                                        hu[j] = c;
                                        hc[j] = lccode(c);
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
                                        confusion(S(778));
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
                                    confusion(S(779));
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
                    confusion(S(780));
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
                freenode(q, deltanodesize);
            else
                freenode(q, activenodesize);
            q = curp;
        }
        q = passive;
        while (q != 0) { /*:865*/
            curp = link(q);
            freenode(q, passivenodesize);
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
            freenode(q, deltanodesize);
        else
            freenode(q, activenodesize);
        q = curp;
    }
    q = passive;
    while (q != 0) { /*:865*/
        curp = link(q);
        freenode(q, passivenodesize);
        q = curp;
    }
    packbeginline = 0;

    /*1363:*/
    /*898:*/
    /*:898*/
}
/*:815*/

/*934:*/
Static void newhyphexceptions(void)
{
  unsigned char n; /* INT */
  unsigned char j; /* INT */
  HyphPointer h;
  StrNumber k, s, t;
  Pointer p, q;

  scan_left_brace();
  setcurlang();   /*935:*/
  n = 0;
  p = 0;
  while (true) {   /*:935*/
    int cur_chr;
    get_x_token();
    cur_chr = curchr;
_LN_newhyphexceptions__reswitch:
    switch (curcmd) {

    case LETTER:
    case OTHER_CHAR:
    case CHAR_GIVEN:   /*937:*/
      if (cur_chr == '-') {   /*938:*/
	if (n < 63) {
	  q = get_avail();
	  link(q) = p;
	  info(q) = n;
	  p = q;
	}
      }  /*:938*/
      else {   /*:937*/
	if (lccode(cur_chr) == 0) {
	  print_err(S(783));
	  help2(S(784),
                S(785));
	  error();
	} else if (n < 63) {
	  n++;
	  hc[n] = lccode(cur_chr);
	}
      }
      break;

    case CHAR_NUM:
      scan_char_num();
      cur_chr = cur_val;
      curcmd = CHAR_GIVEN;
      goto _LN_newhyphexceptions__reswitch;
      break;

    case SPACER:
    case RIGHT_BRACE:   /*936:*/
      if (n > 1) {   /*939:*/
	n++;
	hc[n] = curlang;
	h = 0;
	for (j = 1; j <= n; j++) {
	  h = (h + h + hc[j]) % HYPH_SIZE;
	}
	s = str_ins(hc+1,n);
	if (hyphcount == HYPH_SIZE)
	  overflow(S(786), HYPH_SIZE);
	hyphcount++;
	while (hyphword[h] != 0) {  /*941:*/
	  k = hyphword[h];
	  if (str_length(k) < str_length(s)) {
	    goto _Lfound;
	  }
	  if (str_length(k) > str_length(s)) {
	    goto _Lnotfound;
	  }
	{ int ress = str_cmp(k,s);
	if(ress < 0) goto _Lfound;
	if(ress > 0) goto _Lnotfound;
	}
_Lfound:
	  q = hyphlist[h];
	  hyphlist[h] = p;
	  p = q;
	  t = hyphword[h];
	  hyphword[h] = s;
	  s = t;
_Lnotfound:   /*:941*/
	  if (h > 0)
	    h--;
	  else
	    h = HYPH_SIZE;
	}
	hyphword[h] = s;
	hyphlist[h] = p;   /*:940*/
      }
      /*:939*/
      if (curcmd == RIGHT_BRACE)
	        goto _Lexit;
      n = 0;
      p = 0;
      break;

    default:
      print_err(S(597));
      print_esc(S(787));
      print(S(788));
      help2(S(789),
            S(790));
      error();   /*:936*/
      break;
    }
  }
_Lexit: ;
}
/*:934*/

/*968:*/
Static HalfWord prunepagetop(HalfWord p)
{
  Pointer prevp, q;

  prevp = temphead;
  link(temphead) = p;
  while (p != 0) {
    switch (type(p)) {

    case HLIST_NODE:
    case VLIST_NODE:
    case RULE_NODE:   /*969:*/
      q = newskipparam(SPLIT_TOP_SKIP_CODE);
      link(prevp) = q;
      link(q) = p;
      if (width(temp_ptr) > height(p))
	width(temp_ptr) -= height(p);
      else
	width(temp_ptr) = 0;
      p = 0;
      break;
      /*:969*/

    case WHATSIT_NODE:
    case MARK_NODE:
    case INS_NODE:
      prevp = p;
      p = link(prevp);
      break;

    case GLUE_NODE:
    case KERN_NODE:
    case PENALTY_NODE:
      q = p;
      p = link(q);
      link(q) = 0;
      link(prevp) = p;
      flush_node_list(q);
      break;

    default:
      confusion(S(791));
      break;
    }
  }
  return (link(temphead));
}
/*:968*/

/*970:*/
Static HalfWord vertbreak(HalfWord p, long h, long d)
{
  Pointer prevp, q, r, bestplace=p /* XXXX */ ;
  long pi=0 /* XXXX */, b, leastcost;
  Scaled prevdp;
  SmallNumber t;

  prevp = p;
  leastcost = awfulbad;
  setheightzero(1);
  setheightzero(2);
  setheightzero(3);
  setheightzero(4);
  setheightzero(5);
  setheightzero(6);
  prevdp = 0;
  while (true) {  /*972:*/
    if (p == 0)   /*974:*/
      pi = EJECT_PENALTY;
    else {  /*973:*/
      switch (type(p)) {   /*:973*/

      case HLIST_NODE:
      case VLIST_NODE:
      case RULE_NODE:
	curheight += prevdp + height(p);
	prevdp = depth(p);
	goto _Lnotfound;
	break;

      case WHATSIT_NODE:   /*1365:*/
	goto _Lnotfound;   /*:1365*/
	break;

      case GLUE_NODE:
	if (!precedesbreak(prevp))
	  goto _Lupdateheights_;
	pi = 0;
	break;

      case KERN_NODE:
	if (link(p) == 0)
	  t = PENALTY_NODE;
	else
	  t = type(link(p));
	if (t != GLUE_NODE)
	  goto _Lupdateheights_;
	pi = 0;
	break;

      case PENALTY_NODE:
	pi = penalty(p);
	break;

      case MARK_NODE:
      case INS_NODE:
	goto _Lnotfound;
	break;

      default:
	confusion(S(792));
	break;
      }
    }
    if (pi < INF_PENALTY)   /*:974*/
    {  /*975:*/
      if (curheight < h) {
	if (activeheight[2] != 0 || activeheight[3] != 0 ||
	    activeheight[4] != 0)
	  b = 0;
	else
	  b = badness(h - curheight, activeheight[1]);
      } else if (curheight - h > activeheight[5])
	b = awfulbad;
      else
	b = badness(curheight - h, activeheight[5]);   /*:975*/
      if (b < awfulbad) {
	if (pi <= EJECT_PENALTY)
	  b = pi;
	else if (b < INF_BAD)
	  b += pi;
	else
	  b = deplorable;
      }
      if (b <= leastcost) {
	bestplace = p;
	leastcost = b;
	bestheightplusdepth = curheight + prevdp;
      }
      if (b == awfulbad || pi <= EJECT_PENALTY)
	goto _Ldone;
    }
    if ((type(p) < GLUE_NODE) | (type(p) > KERN_NODE))
      goto _Lnotfound;
_Lupdateheights_:   /*976:*/
    if (type(p) == KERN_NODE)
      q = p;
    else {
      q = glueptr(p);
      activeheight[stretchorder(q) + 1] += stretch(q);
      activeheight[5] += shrink(q);
      if ((shrinkorder(q) != NORMAL) & (shrink(q) != 0)) {
	print_err(S(793));
	help4(S(794),
              S(795),
              S(796),
              S(753));
	error();
	r = newspec(q);
	shrinkorder(r) = NORMAL;
	delete_glue_ref(q);
	glueptr(p) = r;
	q = r;
      }
    }
    curheight += prevdp + width(q);
    prevdp = 0;   /*:976*/
_Lnotfound:
    if (prevdp > d) {   /*:972*/
      curheight += prevdp - d;
      prevdp = d;
    }
    prevp = p;
    p = link(prevp);
  }
_Ldone:
  return bestplace;
}
/*:970*/

/*977:*/
Static HalfWord vsplit(EightBits n, long h) {
    HalfWord Result;
    Pointer v, p, q;

    v = box(n);
    if (splitfirstmark != 0) {
        delete_token_ref(splitfirstmark);
        splitfirstmark = 0;
        delete_token_ref(splitbotmark);
        splitbotmark = 0;
    }
    /*978:*/
    if (v == 0) {
        Result = 0;
        goto _Lexit;
    }
    if (type(v) != VLIST_NODE) { /*:978*/
        print_err(S(385));
        print_esc(S(797));
        print(S(798));
        print_esc(S(799));
        help2(S(800), S(801));
        error();
        Result = 0;
        goto _Lexit;
    }
    q = vertbreak(listptr(v), h, splitmaxdepth); /*979:*/
    p = listptr(v);
    if (p == q)
        listptr(v) = 0;
    else {
        while (true) {
            if (type(p) == MARK_NODE) {
                if (splitfirstmark == 0) {
                    splitfirstmark = markptr(p);
                    splitbotmark = splitfirstmark;
                    tokenrefcount(splitfirstmark) += 2;
                } else {
                    delete_token_ref(splitbotmark);
                    splitbotmark = markptr(p);
                    addtokenref(splitbotmark);
                }
            }
            if (link(p) == q) {
                link(p) = 0;
                goto _Ldone;
            }
            p = link(p);
        }
    }
_Ldone: /*:979*/
    q = prunepagetop(q);
    p = listptr(v);
    freenode(v, boxnodesize);
    if (q == 0)
        box(n) = 0;
    else {
        box(n) = vpack(q, 0, additional);
    }
    Result = vpackage(p, h, exactly, splitmaxdepth);
_Lexit:
    return Result;
}
/*:977*/

/*985:*/
Static void printtotals(void)
{
  print_scaled(pagetotal);
  if (pagesofar[2] != 0) {
    print(S(331));
    print_scaled(pagesofar[2]);
    print(S(385));
  }
  if (pagesofar[3] != 0) {
    print(S(331));
    print_scaled(pagesofar[3]);
    print(S(330));
  }
  if (pagesofar[4] != 0) {
    print(S(331));
    print_scaled(pagesofar[4]);
    print(S(802));
  }
  if (pagesofar[5] != 0) {
    print(S(331));
    print_scaled(pagesofar[5]);
    print(S(803));
  }
  if (pageshrink != 0) {
    print(S(332));
    print_scaled(pageshrink);
  }
}
/*:985*/

/*987:*/
Static void freezepagespecs(SmallNumber s) {
    pagecontents = s;
    pagegoal = vsize;
    pagemaxdepth = maxdepth;
    pagedepth = 0;
    setpagesofarzero(1);
    setpagesofarzero(2);
    setpagesofarzero(3);
    setpagesofarzero(4);
    setpagesofarzero(5);
    setpagesofarzero(6);
    leastpagecost = awfulbad;
    #ifdef tt_STAT
        if (tracingpages <= 0) return;
    #endif // #987: tt_STAT
    begindiagnostic();
    printnl(S(804));
    print_scaled(pagegoal);
    print(S(805));
    print_scaled(pagemaxdepth);
    enddiagnostic(false);
} /*:987*/


/*992:*/
Static void boxerror(EightBits n)
{
  error();
  begindiagnostic();
  printnl(S(690));
  showbox(box(n));
  enddiagnostic(true);
  flush_node_list(box(n));
  box(n) = 0;
}
/*:992*/

/*993:*/
Static void ensurevbox(EightBits n)
{
  Pointer p;

  p = box(n);
  if (p == 0)
    return;
  if (type(p) != HLIST_NODE)
    return;
  print_err(S(806));
  help3(S(807),
        S(808),
        S(809));
  boxerror(n);
}
/*:993*/

/*994:*/
/*1012:*/
Static void fireup(HalfWord c)
{  /*1013:*/
  Pointer p, q, r, s, prevp, savesplittopskip;
  UChar n;
  Boolean wait;
  long savevbadness;
  Scaled savevfuzz;

  if (type(bestpagebreak) == PENALTY_NODE) {
    geqworddefine(intbase + outputpenaltycode, penalty(bestpagebreak));
    penalty(bestpagebreak) = INF_PENALTY;
  } else   /*:1013*/
    geqworddefine(intbase + outputpenaltycode, INF_PENALTY);
  if (botmark != 0) {   /*1014:*/
    if (topmark != 0)
      delete_token_ref(topmark);
    topmark = botmark;
    addtokenref(topmark);
    delete_token_ref(firstmark);
    firstmark = 0;
  }
  if (c == bestpagebreak)   /*1015:*/
    bestpagebreak = 0;
  if (box(255) != 0) {   /*:1015*/
    print_err(S(385));
    print_esc(S(464));
    print(S(810));
    help2(S(811),
          S(809));
    boxerror(255);
  }
  insertpenalties = 0;
  savesplittopskip = splittopskip;
  if (holdinginserts <= 0) {   /*1018:*/
    r = link(pageinshead);
    while (r != pageinshead) {
      if (bestinsptr(r) != 0) {
	n = subtype(r) - MIN_QUARTER_WORD;
	ensurevbox(n);
	if (box(n) == 0)
	  box(n) = newnullbox();
	p = box(n) + listoffset;
	while (link(p) != 0)
	  p = link(p);
	lastinsptr(r) = p;
      }
      r = link(r);
    }
  }
  /*:1018*/
  q = holdhead;
  link(q) = 0;
  prevp = pagehead;
  p = link(prevp);
  while (p != bestpagebreak) {
    if (type(p) == INS_NODE) {
      if (holdinginserts <= 0) {   /*1020:*/
	r = link(pageinshead);
	while (subtype(r) != subtype(p))
	  r = link(r);
	if (bestinsptr(r) == 0)   /*1022:*/
	  wait = true;
	else {
	  wait = false;
	  s = lastinsptr(r);
	  link(s) = insptr(p);
	  if (bestinsptr(r) == p) {   /*1021:*/
	    if (type(r) == splitup) {
	      if ((brokenins(r) == p) & (brokenptr(r) != 0)) {
		while (link(s) != brokenptr(r))
		  s = link(s);
		link(s) = 0;
		splittopskip = splittopptr(p);
		insptr(p) = prunepagetop(brokenptr(r));
		if (insptr(p) != 0) {
		  temp_ptr = vpack(insptr(p), 0, additional);
		  height(p) = height(temp_ptr) + depth(temp_ptr);
		  freenode(temp_ptr, boxnodesize);
		  wait = true;
		}
	      }
	    }
	    bestinsptr(r) = 0;
	    n = subtype(r) - MIN_QUARTER_WORD;
	    temp_ptr = listptr(box(n));
	    freenode(box(n), boxnodesize);
	    box(n) = vpack(temp_ptr, 0, additional);
	  }  /*:1021*/
	  else {
	    while (link(s) != 0)
	      s = link(s);
	    lastinsptr(r) = s;
	  }
	}
	link(prevp) = link(p);
	link(p) = 0;
	if (wait) {
	  link(q) = p;
	  q = p;
	  insertpenalties++;
	} else {
	  delete_glue_ref(splittopptr(p));
	  freenode(p, insnodesize);
	}
	p = prevp;   /*:1022*/
      }
      /*:1020*/
    } else if (type(p) == MARK_NODE) {
      if (firstmark == 0) {
	firstmark = markptr(p);
	addtokenref(firstmark);
      }
      if (botmark != 0)
	delete_token_ref(botmark);
      botmark = markptr(p);
      addtokenref(botmark);
    }
    prevp = p;
    p = link(prevp);
  }
  splittopskip = savesplittopskip;   /*1017:*/
  if (p != 0) {
    if (link(contribhead) == 0) {
      if (nest_ptr == 0)
	tail = pagetail;
      else
	contribtail = pagetail;
    }
    link(pagetail) = link(contribhead);
    link(contribhead) = p;
    link(prevp) = 0;
  }
  savevbadness = vbadness;
  vbadness = INF_BAD;
  savevfuzz = vfuzz;
  vfuzz = MAX_DIMEN;
  box(255) = vpackage(link(pagehead), bestsize, exactly, pagemaxdepth);
  vbadness = savevbadness;
  vfuzz = savevfuzz;
  if (lastglue != MAX_HALF_WORD)   /*991:*/
    delete_glue_ref(lastglue);
  pagecontents = EMPTY;
  pagetail = pagehead;
  link(pagehead) = 0;
  lastglue = MAX_HALF_WORD;
  lastpenalty = 0;
  lastkern = 0;
  pagedepth = 0;
  pagemaxdepth = 0;   /*:991*/
  if (q != holdhead) {   /*:1017*/
    link(pagehead) = link(holdhead);
    pagetail = q;
  }
  /*1019:*/
  r = link(pageinshead);
  while (r != pageinshead) {
    q = link(r);
    freenode(r, pageinsnodesize);
    r = q;
  }
  link(pageinshead) = pageinshead;   /*:1019*/
  /*:1014*/
  if (topmark != 0 && firstmark == 0) {
    firstmark = topmark;
    addtokenref(topmark);
  }
  if (outputroutine != 0) {   /*1023:*/
    if (deadcycles < maxdeadcycles) {   /*1024:*/
      outputactive = true;
      deadcycles++;
      pushnest();
      mode = -V_MODE;
      prevdepth = ignoredepth;
      modeline = -line;
      begintokenlist(outputroutine, OUTPUT_TEXT);
      newsavelevel(outputgroup);
      normalparagraph();
      scan_left_brace();
      goto _Lexit;
    }
    /*:1024*/
    print_err(S(812));
    print_int(deadcycles);
    print(S(813));
    help3(S(814),
          S(815),
          S(816));
    error();
  }
  if (link(pagehead) != 0) {
    if (link(contribhead) == 0) {
      if (nest_ptr == 0)
	tail = pagetail;
      else
	contribtail = pagetail;
    } else
      link(pagetail) = link(contribhead);
    link(contribhead) = link(pagehead);
    link(pagehead) = 0;
    pagetail = pagehead;
  }
  shipout(box(255));
  box(255) = 0;   /*:1023*/
_Lexit: ;
}
/*:1012*/

Static void buildpage(void) {
    Pointer p, q, r;
    long b, c, pi = 0 /* XXXX */;
    UChar n;
    Scaled delta, h, w;

    if (link(contribhead) == 0 || outputactive) goto _Lexit;
    do {
    _Llabcontinue:
        p = link(contribhead); /*996:*/
        if (lastglue != MAX_HALF_WORD) delete_glue_ref(lastglue);
        lastpenalty = 0;
        lastkern = 0;
        if (type(p) == GLUE_NODE) { /*997:*/
            lastglue = glueptr(p);
            addglueref(lastglue);
        } else { /*:996*/
            lastglue = MAX_HALF_WORD;
            if (type(p) == PENALTY_NODE)
                lastpenalty = penalty(p);
            else if (type(p) == KERN_NODE)
                lastkern = width(p);
        }
        /*1000:*/
        switch (type(p)) { /*:1000*/

            case HLIST_NODE:
            case VLIST_NODE:
            case RULE_NODE:
                if (pagecontents < boxthere) { /*1001:*/
                    if (pagecontents == EMPTY)
                        freezepagespecs(boxthere);
                    else
                        pagecontents = boxthere;
                    q = newskipparam(TOP_SKIP_CODE);
                    if (width(temp_ptr) > height(p))
                        width(temp_ptr) -= height(p);
                    else
                        width(temp_ptr) = 0;
                    link(q) = p;
                    link(contribhead) = q;
                    goto _Llabcontinue;
                } else /*1002:*/
                {      /*:1002*/
                    pagetotal += pagedepth + height(p);
                    pagedepth = depth(p);
                    goto _Lcontribute_;
                }
                /*:1001*/
                break;

            case WHATSIT_NODE:       /*1364:*/
                goto _Lcontribute_; /*:1364*/
                break;

            case GLUE_NODE:
                if (pagecontents < boxthere) goto _Ldone1;
                if (!precedesbreak(pagetail)) goto _Lupdateheights_;
                pi = 0;
                break;

            case KERN_NODE:
                if (pagecontents < boxthere) goto _Ldone1;
                if (link(p) == 0) goto _Lexit;
                if (type(link(p)) != GLUE_NODE) goto _Lupdateheights_;
                pi = 0;
                break;

            case PENALTY_NODE:
                if (pagecontents < boxthere) goto _Ldone1;
                pi = penalty(p);
                break;

            case MARK_NODE:
                goto _Lcontribute_;
                break;

            case INS_NODE: /*1008:*/
                if (pagecontents == EMPTY) freezepagespecs(insertsonly);
                n = subtype(p);
                r = pageinshead;
                while (n >= subtype(link(r)))
                    r = link(r);
                n -= MIN_QUARTER_WORD;
                if (subtype(r) != n) { /*1009:*/
                    q = getnode(pageinsnodesize);
                    link(q) = link(r);
                    link(r) = q;
                    r = q;
                    subtype(r) = n;
                    type(r) = inserting;
                    ensurevbox(n);
                    if (box(n) == 0)
                        height(r) = 0;
                    else
                        height(r) = height(box(n)) + depth(box(n));
                    bestinsptr(r) = 0;
                    q = skip(n);
                    if (count(n) == 1000)
                        h = height(r);
                    else
                        h = x_over_n(height(r), 1000) * count(n);
                    pagegoal += -h - width(q);
                    pagesofar[stretchorder(q) + 2] += stretch(q);
                    pageshrink += shrink(q);
                    if ((shrinkorder(q) != NORMAL) & (shrink(q) != 0)) {
                        print_err(S(817));
                        print_esc(S(460));
                        print_int(n);
                        help3(S(818), S(819), S(753));
                        error();
                    }
                }
                /*:1009*/
                if (type(r) == splitup)
                    insertpenalties += floatcost(p);
                else {
                    lastinsptr(r) = p;
                    delta = pagegoal - pagetotal - pagedepth + pageshrink;
                    if (count(n) == 1000)
                        h = height(p);
                    else
                        h = x_over_n(height(p), 1000) * count(n);
                    if ((h <= 0 || h <= delta) &
                        (height(p) + height(r) <= dimen(n))) {
                        pagegoal -= h;
                        height(r) += height(p);
                    } else /*1010:*/
                    {      /*:1010*/
                        if (count(n) <= 0)
                            w = MAX_DIMEN;
                        else {
                            w = pagegoal - pagetotal - pagedepth;
                            if (count(n) != 1000)
                                w = x_over_n(w, count(n)) * 1000;
                        }
                        if (w > dimen(n) - height(r)) w = dimen(n) - height(r);
                        q = vertbreak(insptr(p), w, depth(p));
                        height(r) += bestheightplusdepth;

                        #ifdef tt_STAT
                            if (tracingpages > 0) {
                                /*1011:*/
                                begindiagnostic();
                                printnl(S(820));
                                print_int(n);
                                print(S(821));
                                print_scaled(w);
                                print_char(',');
                                print_scaled(bestheightplusdepth);
                                print(S(764));
                                if (q == 0)
                                    print_int(EJECT_PENALTY);
                                else if (type(q) == PENALTY_NODE)
                                    print_int(penalty(q));
                                else
                                    print_char('0');
                                enddiagnostic(false);
                            } /*:1011*/
                        #endif // #1010: tt_STAT

                        if (count(n) != 1000)
                            bestheightplusdepth =
                                x_over_n(bestheightplusdepth, 1000) * count(n);
                        pagegoal -= bestheightplusdepth;
                        type(r) = splitup;
                        brokenptr(r) = q;
                        brokenins(r) = p;
                        if (q == 0)
                            insertpenalties += EJECT_PENALTY;
                        else if (type(q) == PENALTY_NODE)
                            insertpenalties += penalty(q);
                    }
                }
                goto _Lcontribute_;
                break;
                /*:1008*/

            default:
                confusion(S(822));
                break;
        }
        /*1005:*/
        if (pi < INF_PENALTY) /*:1005*/
        {                    /*1007:*/
            if (pagetotal < pagegoal) {
                if (pagesofar[3] != 0 || pagesofar[4] != 0 || pagesofar[5] != 0)
                    b = 0;
                else
                    b = badness(pagegoal - pagetotal, pagesofar[2]);
            } else if (pagetotal - pagegoal > pageshrink)
                b = awfulbad;
            else
                b = badness(pagetotal - pagegoal, pageshrink); /*:1007*/
            if (b < awfulbad) {
                if (pi <= EJECT_PENALTY)
                    c = pi;
                else if (b < INF_BAD)
                    c = b + pi + insertpenalties;
                else
                    c = deplorable;
            } else
                c = b;
            if (insertpenalties >= 10000)
                c = awfulbad;

            #ifdef tt_STAT
                if (tracingpages > 0) { /*1006:*/
                    begindiagnostic();
                    printnl('%');
                    print(S(758));
                    printtotals();
                    print(S(823));
                    print_scaled(pagegoal);
                    print(S(763));
                    if (b == awfulbad)
                        print_char('*');
                    else
                        print_int(b);
                    print(S(764));
                    print_int(pi);
                    print(S(824));
                    if (c == awfulbad)
                        print_char('*');
                    else
                        print_int(c);
                    if (c <= leastpagecost) print_char('#');
                    enddiagnostic(false);
                }
            #endif // #1005: tt_STAT
            /*:1006*/

            if (c <= leastpagecost) {
                bestpagebreak = p;
                bestsize = pagegoal;
                leastpagecost = c;
                r = link(pageinshead);
                while (r != pageinshead) {
                    bestinsptr(r) = lastinsptr(r);
                    r = link(r);
                }
            }
            if (c == awfulbad || pi <= EJECT_PENALTY) {
                fireup(p);
                if (outputactive) goto _Lexit;
                goto _Ldone;
            }
        }
        if ((type(p) < GLUE_NODE) | (type(p) > KERN_NODE)) goto _Lcontribute_;
    _Lupdateheights_: /*1004:*/
        if (type(p) == KERN_NODE)
            q = p;
        else {
            q = glueptr(p);
            pagesofar[stretchorder(q) + 2] += stretch(q);
            pageshrink += shrink(q);
            if ((shrinkorder(q) != NORMAL) & (shrink(q) != 0)) {
                print_err(S(825));
                help4(S(826), S(795), S(796), S(753));
                error();
                r = newspec(q);
                shrinkorder(r) = NORMAL;
                delete_glue_ref(q);
                glueptr(p) = r;
                q = r;
            }
        }
        pagetotal += pagedepth + width(q);
        pagedepth = 0;                  /*:1004*/
    _Lcontribute_:                      /*1003:*/
        if (pagedepth > pagemaxdepth) { /*:1003*/
            pagetotal += pagedepth - pagemaxdepth;
            pagedepth = pagemaxdepth;
        }
        /*998:*/
        link(pagetail) = p;
        pagetail = p;
        link(contribhead) = link(p);
        link(p) = 0;
        goto _Ldone; /*:998*/
    _Ldone1:         /*999:*/
        link(contribhead) = link(p);
        link(p) = 0; /*:999*/
        flush_node_list(p);
    _Ldone:;                          /*:997*/
    } while (link(contribhead) != 0); /*995:*/
    if (nest_ptr == 0)
        tail = contribhead;
    else
        contribtail = contribhead; /*:995*/
_Lexit:;
}
/*:994*/

/*1030:*/


/*
 * [#1043]: Declare action procedures for use by `main_control
 * `
 * xref[57]:
 *  1043, 1047, 1049, 1050, 1051,
 *  1054, 1060, 1061, 1064, 1069,
 *  1070, 1075, 1079, 1084, 1086,
 *  1091, 1093, 1095, 1096, 1099,
 *  1101, 1103, 1105, 1110, 1113,
 *  1117, 1119, 1123, 1127, 1129,
 *  1131, 1135, 1136, 1138, 1142,
 *  1151, 1155, 1159, 1160, 1163,
 *  1165, 1172, 1174, 1176, 1181,
 *  1191, 1194, 1200, 1211, 1270,
 *  1275, 1279, 1288, 1293, 1302,
 *  1348, 1376.
 */

// #1043
Static void appspace(void) {
    Pointer q;

    if (spacefactor >= 2000 && xspaceskip != zeroglue) {
        q = newparamglue(XSPACE_SKIP_CODE);
    } else {
        if (spaceskip != zeroglue) {
            mainp = spaceskip;
        } else { /*1042:*/
            mainp = fontglue[curfont];
            if (mainp == 0) {
                FontIndex mmaink;
                mainp = newspec(zeroglue);
                mmaink = parambase[curfont] + SPACE_CODE;
            #if 1
                maink = mmaink;
            #endif
                width(mainp) = fontinfo[maink].sc;
                stretch(mainp) = fontinfo[mmaink + 1].sc;
                shrink(mainp) = fontinfo[mmaink + 2].sc;
                fontglue[curfont] = mainp;
            }
        }
        mainp = newspec(mainp); /*1044:*/
        if (spacefactor >= 2000) width(mainp) += extraspace(curfont);
        stretch(mainp) = xn_over_d(stretch(mainp), spacefactor, 1000);
        shrink(mainp) = xn_over_d(shrink(mainp), 1000, spacefactor);
        /*:1044*/
        q = newglue(mainp);
        gluerefcount(mainp) = 0;
    }
    link(tail) = q;
    tail = q;

    /*:1042*/
} // #1043: appspace

// #1047
Static void insertdollarsign(void) {
    backinput();
    curtok = mathshifttoken + '$';
    print_err(S(827));
    help2(S(828), S(829));
    inserror();
} // #1047: insertdollarsign

// #1049
Static void youcant(void) {
    print_err(S(602));
    printcmdchr(curcmd, curchr);
    print(S(830));
    print_mode(mode);
} // #1049: youcant

// #1050
Static void reportillegalcase(void) {
    youcant();
    help4(S(831), S(832), S(833), S(834));
    error();
} // #1050: reportillegalcase

// #1051
Static Boolean privileged(void) {
    if (mode > 0) {
        return true;
    } else {
        reportillegalcase();
        return false;
    }
} // #1051: privileged

// #1054
Static Boolean itsallover(void) {
    if (privileged()) {
        if (pagehead == pagetail && head == tail && deadcycles == 0) {
            return true;
        }
        backinput();
        tailappend(newnullbox());
        width(tail) = hsize;
        tailappend(newglue(fillglue));
        tailappend(newpenalty(-1073741824L));
        buildpage();
    }

    return false;
}// #1054: itsallover

// #1060
Static void appendglue(void) {
    SmallNumber s;

    s = curchr;
    switch (s) {
        case FIL_CODE:      cur_val = filglue; break;
        case FILL_CODE:     cur_val = fillglue; break;
        case SS_CODE:       cur_val = ssglue; break;
        case FIL_NEG_CODE:  cur_val = filnegglue; break;
        case SKIP_CODE:     scan_glue(GLUE_VAL); break;
        case MSKIP_CODE:    scan_glue(MU_VAL); break;
    }
    tailappend(newglue(cur_val));
    if (s < SKIP_CODE) return;

    (gluerefcount(cur_val))--;
    if (s > SKIP_CODE) subtype(tail) = muglue;
} // #1060: appendglue

// #1061
Static void appendkern(void) {
    QuarterWord s;

    s = curchr;
    scan_dimen(s == muglue, false, false);
    tailappend(newkern(cur_val));
    subtype(tail) = s;
} // #1061: appendkern

// #1064:
Static void offsave(void) {
    Pointer p;

    if (curgroup == bottomlevel) { /*1066:*/
        print_err(S(558));
        printcmdchr(curcmd, curchr);
        help1(S(835));
        error();
        return;
    } /*:1066*/
    backinput();
    p = get_avail();
    link(temphead) = p;
    print_err(S(554));      /*1065:*/
    switch (curgroup) { /*:1065*/
        case semisimplegroup:
            info(p) = CS_TOKEN_FLAG + frozenendgroup;
            print_esc(S(836));
            break;

        case mathshiftgroup:
            info(p) = mathshifttoken + '$';
            print_char('$');
            break;

        case mathleftgroup:
            info(p) = CS_TOKEN_FLAG + frozenright;
            link(p) = get_avail();
            p = link(p);
            info(p) = othertoken + '.';
            print_esc(S(837));
            break;

        default:
            info(p) = rightbracetoken + '}';
            print_char('}');
            break;
    }
    print(S(555));
    inslist(link(temphead));
    help5(S(838), S(839), S(840), S(841), S(842));
    error();
} // #1064: offsave

// #1069:
Static void extrarightbrace(void) {
    print_err(S(843));
    switch (curgroup) {
        case semisimplegroup: print_esc(S(836)); break;
        case mathshiftgroup: print_char('$'); break;
        case mathleftgroup: print_esc(S(419)); break;
    }
    help5(S(844), S(845), S(846), S(847), S(848));
    error();
    align_state++;
} // #1069: extrarightbrace


/*1070:*/
Static void normalparagraph(void)
{
  if (looseness != 0)
    eqworddefine(intbase + loosenesscode, 0);
  if (hangindent != 0)
    eqworddefine(dimenbase + hangindentcode, 0);
  if (hangafter != 1)
    eqworddefine(intbase + hangaftercode, 1);
  if (parshapeptr != 0)
    eqdefine(parshapeloc, SHAPE_REF, 0);
}  /*:1070*/


/*1075:*/
Static void boxend(long boxcontext)
{
  Pointer p;

  if (boxcontext < boxflag) {   /*1076:*/
    if (curbox == 0)
      return;
    shiftamount(curbox) = boxcontext;
    if (labs(mode) == V_MODE) {
      appendtovlist(curbox);
      if (adjusttail != 0) {
	if (adjusthead != adjusttail) {
	  link(tail) = link(adjusthead);
	  tail = adjusttail;
	}
	adjusttail = 0;
      }
      if (mode > 0)
	buildpage();
      return;
    }
    if (labs(mode) == H_MODE)
      spacefactor = 1000;
    else {
      p = newnoad();
      mathtype(nucleus(p)) = subbox;
      info(nucleus(p)) = curbox;
      curbox = p;
    }
    link(tail) = curbox;
    tail = curbox;
    return;
  }  /*:1076*/
  if (boxcontext < shipoutflag) {   /*1077:*/
    if (boxcontext < boxflag + 256)
      eqdefine(boxbase - boxflag + boxcontext, BOX_REF, curbox);
    else   /*:1077*/
      geqdefine(boxbase - boxflag + boxcontext - 256, BOX_REF, curbox);
    return;
  }
  if (curbox == 0)
    return;
  if (boxcontext <= shipoutflag)   /*1078:*/
  {
    shipout(curbox);
    return;
  }
  /*:1078*/
  skip_spaces_or_relax();
  if ( (curcmd == HSKIP && labs(mode) != V_MODE) ||
      (curcmd == VSKIP && labs(mode) == V_MODE) ||
      (curcmd == MSKIP && labs(mode) == M_MODE) ) {
    appendglue();
    subtype(tail) = boxcontext - leaderflag + aleaders;
    leaderptr(tail) = curbox;
    return;
  }
  print_err(S(849));
  help3(S(850),
        S(851),
        S(852));
  backerror();
  flush_node_list(curbox);
}
/*:1075*/

/*1079:*/
Static void beginbox(long boxcontext) {
    Pointer p, q;
    QuarterWord m;
    HalfWord k;
    EightBits n;

    switch (curchr) {

    case boxcode:
        scan_eight_bit_int();
        curbox = box(cur_val);
        box(cur_val) = 0;
        break;

    case copycode:
        scan_eight_bit_int();
        curbox = copynodelist(box(cur_val));
        break;

    case lastboxcode: /*1080:*/
        curbox = 0;
        if (labs(mode) == M_MODE) {
            youcant();
            help1(S(853));
            error();
        } else if (mode == V_MODE && head == tail) {
            youcant();
            help2(S(854), S(855));
            error();
        } else {
            if (!ischarnode(tail)) {
                if ((type(tail) == HLIST_NODE) |
                    (type(tail) == VLIST_NODE)) { /*1081:*/
                    q = head;
                    do {
                        p = q;
                        if (!ischarnode(q)) {
                            if (type(q) == DISC_NODE) {
                                QuarterWord FORLIM = replacecount(q);
                                for (m = 1; m <= FORLIM; m++)
                                    p = link(p);
                                if (p == tail) goto _Ldone;
                            }
                        }
                        q = link(p);
                    } while (q != tail);
                    curbox = tail;
                    shiftamount(curbox) = 0;
                    tail = p;
                    link(p) = 0;
_Ldone:;
                }
                /*:1081*/
            }
        }
        break;
        /*:1080*/

    case vsplitcode: /*1082:*/
        scan_eight_bit_int();
        n = cur_val;
        if (!scankeyword(S(697))) {
            print_err(S(856));
            help2(S(857), S(858));
            error();
        }
        SCAN_NORMAL_DIMEN();
        curbox = vsplit(n, cur_val);
        break;
        /*:1082*/
        /*1083:*/

    default:
        k = curchr - vtopcode;
        saved(0) = boxcontext;
        if (k == H_MODE) {
            if (boxcontext < boxflag && labs(mode) == V_MODE)
                scanspec(adjustedhboxgroup, true);
            else
                scanspec(hboxgroup, true);
        } else {
            if (k == V_MODE)
                scanspec(vboxgroup, true);
            else {
                scanspec(vtopgroup, true);
                k = V_MODE;
            }
            normalparagraph();
        }
        pushnest();
        mode = -k;
        if (k == V_MODE) {
            prevdepth = ignoredepth;
            if (everyvbox != 0) begintokenlist(everyvbox, EVERY_VBOX_TEXT);
        } else {
            spacefactor = 1000;
            if (everyhbox != 0) begintokenlist(everyhbox, EVERY_HBOX_TEXT);
        }
        goto _Lexit; 

        /*:1083*/
        break;
    }
    boxend(boxcontext);
_Lexit:;
}
/*:1079*/

/*1084:*/
Static void scanbox(long boxcontext)
{
  skip_spaces_or_relax();
  if (curcmd == MAKE_BOX) {
    beginbox(boxcontext);
    return;
  }
  if (boxcontext >= leaderflag && (curcmd == HRULE || curcmd == VRULE)) {
    curbox = scan_rule_spec();
    boxend(boxcontext);
    return;
  }
  print_err(S(859));
  help3(S(860),
        S(861),
        S(862));
  backerror();
}
/*:1084*/

/*1086:*/
Static void package(SmallNumber c)
{
  Scaled h, d;
  Pointer p;

  d = boxmaxdepth;
  unsave();
  saveptr -= 3;
  if (mode == -H_MODE)
    curbox = hpack(link(head), saved(2), saved(1));
  else {
    curbox = vpackage(link(head), saved(2), saved(1), d);
    if (c == vtopcode) {   /*1087:*/
      h = 0;
      p = listptr(curbox);
      if (p != 0) {
	if (type(p) <= RULE_NODE)
	  h = height(p);
      }
      depth(curbox) += height(curbox) - h;
      height(curbox) = h;
    }
    /*:1087*/
  }
  popnest();
  boxend(saved(0));
}
/*:1086*/

/*1091:*/
Static SmallNumber normmin(long h) {
    if (h <= 0)
        return 1;
    else if (h >= 63)
        return 63;
    else
        return h;
}


Static void newgraf(Boolean indented)
{
  prevgraf = 0;
  if (mode == V_MODE || head != tail) {
    tailappend(newparamglue(PAR_SKIP_CODE));
  }
  pushnest();
  mode = H_MODE;
  spacefactor = 1000;
  setcurlang();
  clang = curlang;
  prevgraf = (normmin(lefthyphenmin) * 64 + normmin(righthyphenmin)) * 65536L +
	     curlang;
  if (indented) {
    tail = newnullbox();
    link(head) = tail;
    width(tail) = parindent;
  }
  if (everypar != 0)
    begintokenlist(everypar, EVERY_PAR_TEXT);
  if (nest_ptr == 1)
    buildpage();
}
/*:1091*/

/*1093:*/
Static void indentinhmode(void)
{
  Pointer p, q;

  if (curchr <= 0)
    return;
  p = newnullbox();
  width(p) = parindent;
  if (labs(mode) == H_MODE)
    spacefactor = 1000;
  else {
    q = newnoad();
    mathtype(nucleus(q)) = subbox;
    info(nucleus(q)) = p;
    p = q;
  }
  tailappend(p);
}
/*:1093*/

/*1095:*/
Static void headforvmode(void)
{
  if (mode < 0) {
    if (curcmd != HRULE) {
      offsave();
      return;
    }
    print_err(S(602));
    print_esc(S(863));
    print(S(864));
    help2(S(865),
          S(866));
    error();
    return;
  }
  backinput();
  curtok = partoken;
  backinput();
  token_type = INSERTED;
}
/*:1095*/

/*1096:*/
Static void endgraf(void)
{
  if (mode != H_MODE)
    return;
  if (head == tail)
    popnest();
  else
    linebreak(widowpenalty);
  normalparagraph();
  errorcount = 0;
}
/*:1096*/

/*1099:*/
Static void begininsertoradjust(void)
{
  if (curcmd == VADJUST)
    cur_val = 255;
  else {
    scan_eight_bit_int();
    if (cur_val == 255) {
      print_err(S(867));
      print_esc(S(374));
      print_int(255);
      help1(S(868));
      error();
      cur_val = 0;
    }
  }
  saved(0) = cur_val;
  saveptr++;
  newsavelevel(insertgroup);
  scan_left_brace();
  normalparagraph();
  pushnest();
  mode = -V_MODE;
  prevdepth = ignoredepth;
}
/*:1099*/


/*1101:*/
Static void makemark(void)
{
  Pointer p;

  p = scantoks(false, true);
  p = getnode(smallnodesize);
  type(p) = MARK_NODE;
  subtype(p) = 0;
  markptr(p) = defref;
  link(tail) = p;
  tail = p;
}
/*:1101*/

/*1103:*/
Static void appendpenalty(void)
{
  scan_int();
  tailappend(newpenalty(cur_val));
  if (mode == V_MODE)
    buildpage();
}
/*:1103*/

/*1105:*/
Static void deletelast(void) {
    Pointer p, q;

    if (mode == V_MODE && tail == head) { /*1106:*/
    if (curchr != GLUE_NODE || lastglue != MAX_HALF_WORD) {
        youcant();
        help2(S(854), S(869));
        if (curchr == KERN_NODE)
            help_line[0] = S(870);
        else if (curchr != GLUE_NODE)
            help_line[0] = S(871);
        error();
    }
    } /*:1106*/
    else {
        if (!ischarnode(tail)) {
        if (type(tail) == curchr) {
            q = head;
            do {
                p = q;
                if (!ischarnode(q)) {
                if (type(q) == DISC_NODE) {
                    QuarterWord FORLIM = replacecount(q);
                    QuarterWord m;
                    for (m = 1; m <= FORLIM; m++)
                        p = link(p);
                    if (p == tail) return;
                }
                }
                q = link(p);
            } while (q != tail);
            link(p) = 0;
            flush_node_list(tail);
            tail = p;
        }
        }
    }
}
/*:1105*/

/*1110:*/
Static void unpackage(void) {
    Pointer p;
    char c;

    c = curchr;
    scan_eight_bit_int();
    p = box(cur_val);
    if (p == 0) return;
    if ( (labs(mode) == M_MODE) |
        ((labs(mode) == V_MODE) & (type(p) != VLIST_NODE)) |
        ((labs(mode) == H_MODE) & (type(p) != HLIST_NODE))) {
        print_err(S(872));
        help3(S(873), S(874), S(875));
        error();
        return;
    }

    if (c == copycode)
        link(tail) = copynodelist(listptr(p));
    else {
        link(tail) = listptr(p);
        box(cur_val) = 0;
        freenode(p, boxnodesize);
    }
    while (link(tail) != 0)
        tail = link(tail);
}
/*:1110*/

/*1113:*/
Static void appenditaliccorrection(void) {
    Pointer p;
    InternalFontNumber f;
    int c;

    if (tail != head) {
        if (ischarnode(tail)) {
            p = tail;
            f = font(p);
            c = character(p);
        } else if (type(tail) == LIGATURE_NODE) {
            p = ligchar(tail);
            f = font_ligchar(tail);
            c = character_ligchar(tail);
        } else {
            return;
        }
        /*   
            f = font(p);
            tailappend(newkern(charitalic(f, charinfo(f, character(p)))));
        */
        tailappend(newkern(charitalic(f, charinfo(f, c))));
        subtype(tail) = explicit;
    }
}
/*:1113*/

/*1117:*/
Static void appenddiscretionary(void)
{
  long c;

  tailappend(newdisc());
  if (curchr == 1) {
    c = get_hyphenchar(curfont);
    if (c >= 0) {
      if (c < 256)
	prebreak(tail) = newcharacter(curfont, c);
    }
    return;
  }
  saveptr++;
  saved(-1) = 0;
  newsavelevel(discgroup);
  scan_left_brace();
  pushnest();
  mode = -H_MODE;
  spacefactor = 1000;
}
/*:1117*/

/*1119:*/
Static void builddiscretionary(void) {
    Pointer p, q;
    long n;

    unsave(); /*1121:*/
    q = head;
    p = link(q);
    n = 0;

    while (p != 0) {
        if (!ischarnode(p)) {
            if (type(p) > RULE_NODE) {
                if (type(p) != KERN_NODE) {
                    if (type(p) != LIGATURE_NODE) {
                        print_err(S(876));
                        help1(S(877));
                        error();
                        begindiagnostic();
                        printnl(S(878));
                        showbox(p);
                        enddiagnostic(true);
                        flush_node_list(p);
                        link(q) = 0;
                        break;
                    }
                }
            }
        }
        q = p;
        p = link(q);
        n++;
    }
/*:1121*/

    p = link(head);
    popnest();
    switch (saved(-1)) {

        case 0:
            prebreak(tail) = p;
            break;

        case 1:
            postbreak(tail) = p;
            break;

        case 2: /*1120:*/
            if (n > 0 && labs(mode) == M_MODE) {
                print_err(S(879));
                print_esc(S(400));
                help2(S(880), S(881));
                flush_node_list(p);
                n = 0;
                error();
            } else
                link(tail) = p;
            if (n <= MAX_QUARTER_WORD)
                replacecount(tail) = n;
            else {
                print_err(S(882));
                help2(S(883), S(884));
                error();
            }
            if (n > 0) tail = q;
            saveptr--;
            goto _Lexit;
            break;
            /*:1120*/
    }
    (saved(-1))++;
    newsavelevel(discgroup);
    scan_left_brace();
    pushnest();
    mode = -H_MODE;
    spacefactor = 1000;
_Lexit:;
}
/*:1119*/

/*1123:*/
Static void makeaccent(void)
{
  double s, t;
  Pointer p, q, r;
  InternalFontNumber f;
  Scaled a, h, x, w, delta;
  FourQuarters i;

  scan_char_num();
  f = curfont;
  p = newcharacter(f, cur_val);
  if (p == 0)
    return;
  x = xheight(f);
  s = slant(f) / 65536.0;
  a = charwidth(f, charinfo(f, character(p)));
  doassignments();   /*1124:*/
  q = 0;
  f = curfont;
  if (curcmd == LETTER || curcmd == OTHER_CHAR || curcmd == CHAR_GIVEN)
    q = newcharacter(f, curchr);
  else if (curcmd == CHAR_NUM) {
    scan_char_num();
    q = newcharacter(f, cur_val);
  } else
    backinput();
  if (q != 0) {   /*1125:*/
    t = slant(f) / 65536.0;
    i = charinfo(f, character(q));
    w = charwidth(f, i);
    h = charheight(f, heightdepth(i));
    if (h != x) {
      p = hpack(p, 0, additional);
      shiftamount(p) = x - h;
    }
    delta = (long)floor((w - a) / 2.0 + h * t - x * s + 0.5);
    r = newkern(delta);
    subtype(r) = acckern;
    link(tail) = r;
    link(r) = p;
    tail = newkern(-a - delta);
    subtype(tail) = acckern;
    link(p) = tail;
    p = q;
  }
  /*:1125*/
  link(tail) = p;
  tail = p;
  spacefactor = 1000;

  /*:1124*/
}
/*:1123*/

/*1127:*/
Static void alignerror(void)
{
  if (labs(align_state) > 2) {   /*1128:*/
    print_err(S(885));
    printcmdchr(curcmd, curchr);
    if (curtok == tabtoken + '&') {
      help6(S(886),
            S(887),
            S(888),
            S(889),
            S(890),
            S(891));
    } else {
      help5(S(886),
            S(892),
            S(889),
            S(890),
            S(891));
    }
    error();
    return;
  }
  /*:1128*/
  backinput();
  if (align_state < 0) {
    print_err(S(566));
    align_state++;
    curtok = leftbracetoken + '{';
  } else {
    print_err(S(893));
    align_state--;
    curtok = rightbracetoken + '}';
  }
  help3(S(894),
        S(895),
        S(896));
  inserror();
}
/*:1127*/

/*1129:*/
Static void noalignerror(void)
{
  print_err(S(885));
  print_esc(S(897));
  help2(S(898),
        S(899));
  error();
}


Static void omiterror(void)
{
  print_err(S(885));
  print_esc(S(900));
  help2(S(901),
        S(899));
  error();
}
/*:1129*/

/*1131:*/
Static void doendv(void)
{
  if (curgroup != aligngroup) {
    offsave();
    return;
  }
  endgraf();
  if (fincol())
    finrow();
}
/*:1131*/

/*1135:*/
Static void cserror(void)
{
  print_err(S(558));
  print_esc(S(263));
  help1(S(902));
  error();
}  /*:1135*/


/*1136:*/
Static void pushmath(GroupCode c)
{
  pushnest();
  mode = -M_MODE;
  incompleatnoad = 0;
  newsavelevel(c);
}
/*:1136*/

/*1138:*/
Static void initmath(void)
{
  Scaled w, l, s, v, d;
  Pointer p, q;
  InternalFontNumber f;
  long n;

  gettoken();
  if (curcmd == MATH_SHIFT && mode > 0) {   /*1145:*/
    if (head == tail) {
      popnest();
      w = -MAX_DIMEN;
    } else {
      linebreak(displaywidowpenalty);   /*1146:*/
      v = shiftamount(justbox) + quad(curfont) * 2;
      w = -MAX_DIMEN;
      p = listptr(justbox);
      while (p != 0) {  /*1147:*/
_LN_initmath__reswitch:
	if (ischarnode(p)) {
	  f = font(p);
	  d = charwidth(f, charinfo(f, character(p)));
	  goto _Lfound;
	}
	switch (type(p)) {   /*:1147*/

	case HLIST_NODE:
	case VLIST_NODE:
	case RULE_NODE:
	  d = width(p);
	  goto _Lfound;
	  break;

	case LIGATURE_NODE:   /*652:*/
      type(ligtrick) = charnodetype ;
      font(ligtrick) = font_ligchar(p);
      character(ligtrick) = character_ligchar(p);
	  link(ligtrick) = link(p);
	  p = ligtrick;
	  goto _LN_initmath__reswitch;
	  break;
	  /*:652*/

	case KERN_NODE:
	case MATH_NODE:
	  d = width(p);
	  break;

	case GLUE_NODE:   /*1148:*/
	  q = glueptr(p);
	  d = width(q);
	  if (gluesign(justbox) == stretching) {
	    if ((glueorder(justbox) == stretchorder(q)) &
		(stretch(q) != 0))
	      v = MAX_DIMEN;
	  } else if (gluesign(justbox) == shrinking) {
	    if ((glueorder(justbox) == shrinkorder(q)) &
		(shrink(q) != 0))
	      v = MAX_DIMEN;
	  }
	  if (subtype(p) >= aleaders)
	    goto _Lfound;
	  break;
	  /*:1148*/

	case WHATSIT_NODE:   /*1361:*/
	  d = 0;   /*:1361*/
	  break;

	default:
	  d = 0;
	  break;
	}
	if (v < MAX_DIMEN)
	  v += d;
	goto _Lnotfound;
_Lfound:
	if (v >= MAX_DIMEN) {
	  w = MAX_DIMEN;
	  goto _Ldone;
	}
	v += d;
	w = v;
_Lnotfound:
	p = link(p);
      }
_Ldone: ;   /*:1146*/
    }  /*1149:*/
    if (parshapeptr == 0) {
      if (hangindent != 0 &&
	  ( (hangafter >= 0 && prevgraf + 2 > hangafter) ||
	   prevgraf + 1 < -hangafter) ) {
	l = hsize - labs(hangindent);
	if (hangindent > 0)
	  s = hangindent;
	else
	  s = 0;
      } else {
	l = hsize;
	s = 0;
      }
    } else {   /*:1149*/
      n = info(parshapeptr);
      if (prevgraf + 2 >= n)
	p = parshapeptr + n * 2;
      else
	p = parshapeptr + (prevgraf + 2) * 2;
      s = mem[p - MEM_MIN - 1].sc;
      l = mem[p - MEM_MIN].sc;
    }
    pushmath(mathshiftgroup);
    mode = M_MODE;
    eqworddefine(intbase + curfamcode, -1);
    eqworddefine(dimenbase + predisplaysizecode, w);
    eqworddefine(dimenbase + displaywidthcode, l);
    eqworddefine(dimenbase + displayindentcode, s);
    if (everydisplay != 0)
      begintokenlist(everydisplay, EVERY_DISPLAY_TEXT);
    if (nest_ptr == 1)
      buildpage();
    return;
  }
  /*:1145*/
  backinput();   /*1139:*/
  pushmath(mathshiftgroup);
  eqworddefine(intbase + curfamcode, -1);
  if (everymath != 0)   /*:1139*/
    begintokenlist(everymath, EVERY_MATH_TEXT);
}
/*:1138*/

/*1142:*/
Static void starteqno(void) {
    saved(0) = curchr;
    saveptr++; /*1139:*/
    pushmath(mathshiftgroup);
    eqworddefine(intbase + curfamcode, -1);
    if (everymath != 0) /*:1139*/
        begintokenlist(everymath, EVERY_MATH_TEXT);
}
/*:1142*/

/*1151:*/
Static void scanmath(HalfWord p) {
    long c;

_Lrestart:
    skip_spaces_or_relax();
_LN_scanmath__reswitch:
    switch (curcmd) {

        case LETTER:
        case OTHER_CHAR:
        case CHAR_GIVEN:
            c = mathcode(curchr);
            if (c == 32768L) { /*1152:*/
                curcs = curchr + activebase;
                curcmd = eqtype(curcs);
                curchr = equiv(curcs);
                xtoken();
                backinput(); /*:1152*/
                goto _Lrestart;
            }
            break;

        case CHAR_NUM:
            scan_char_num();
            curchr = cur_val;
            curcmd = CHAR_GIVEN;
            goto _LN_scanmath__reswitch;
            break;

        case MATH_CHAR_NUM:
            scan_fifteen_bit_int();
            c = cur_val;
            break;

        case MATH_GIVEN:
            c = curchr;
            break;

        case DELIM_NUM: /*1153:*/
            scan_twenty_seven_bit_int();
            c = cur_val / 4096;
            break;

        default:
            backinput();
            scan_left_brace();
            saved(0) = p;
            saveptr++;
            pushmath(mathgroup);
            goto _Lexit;
            /*:1153*/
            break;
    }
    mathtype(p) = mathchar;
    character(p) = c & 255;
    if (c >= varcode && faminrange) {
        fam(p) = curfam;
    } else {
        fam(p) = (c / 256) & 15;
    }
_Lexit:;
}
/*:1151*/

/*1155:*/
Static void setmathchar(long c)
{
  Pointer p;

  if (c >= 32768L) {   /*1152:*/
    curcs = curchr + activebase;
    curcmd = eqtype(curcs);
    curchr = equiv(curcs);
    xtoken();
    backinput();
    return;
  }  /*:1152*/
  p = newnoad();
  mathtype(nucleus(p)) = mathchar;
  character(nucleus(p)) = c & 255;
  fam(nucleus(p)) = (c / 256) & 15;
  if (c >= varcode) {
    if (faminrange) {
      fam(nucleus(p)) = curfam;
    }
    type(p) = ordnoad;
  } else
    type(p) = ordnoad + c / 4096;
  link(tail) = p;
  tail = p;
}
/*:1155*/

/*1159:*/
Static void mathlimitswitch(void) {
    if (head != tail) {
    if (type(tail) == opnoad) {
        subtype(tail) = curchr;
        return;
    }
    }
    print_err(S(903));
    help1(S(904));
    error();
}
/*:1159*/

/*1160:*/
Static void scandelimiter(HalfWord p, Boolean r)
{
  if (r)
    scan_twenty_seven_bit_int();
  else {
    skip_spaces_or_relax();
    switch (curcmd) {

    case LETTER:
    case OTHER_CHAR:
      cur_val = delcode(curchr);
      break;

    case DELIM_NUM:
      scan_twenty_seven_bit_int();
      break;

    default:
      cur_val = -1;
      break;
    }
  }
  if (cur_val < 0) {   /*1161:*/
    print_err(S(905));
    help6(S(906),
          S(907),
          S(908),
          S(909),
          S(910),
          S(911));
    backerror();
    cur_val = 0;
  }
  /*:1161*/
  smallfam(p) = (cur_val / 1048576L) & 15;
  smallchar(p) = (cur_val / 4096) & 255;
  largefam(p) = (cur_val / 256) & 15;
  largechar(p) = cur_val & 255;
}
/*:1160*/

/*1163:*/
Static void mathradical(void)
{
  int i=0;
  tailappend(getnode(radicalnoadsize));
  type(tail) = radicalnoad;
  subtype(tail) = NORMAL;
#ifdef BIG_CHARNODE
  for(i=0;i<CHAR_NODE_SIZE;i++) {
#endif
  mem[nucleus(tail) + i - MEM_MIN].hh = emptyfield;
  mem[subscr(tail) + i - MEM_MIN].hh = emptyfield;
  mem[supscr(tail) + i - MEM_MIN].hh = emptyfield;
#ifdef BIG_CHARNODE
  }
#endif
  scandelimiter(leftdelimiter(tail), true);
  scanmath(nucleus(tail));
}
/*:1163*/

/*1165:*/
Static void mathac(void)
{
  if (curcmd == ACCENT) {   /*1166:*/
    print_err(S(912));
    print_esc(S(913));
    print(S(914));
    help2(S(915),
          S(916));
    error();
  }
  /*:1166*/
  tailappend(getnode(accentnoadsize));
  type(tail) = accentnoad;
  subtype(tail) = NORMAL;
  mem[nucleus(tail) - MEM_MIN].hh = emptyfield;
  mem[subscr(tail) - MEM_MIN].hh = emptyfield;
  mem[supscr(tail) - MEM_MIN].hh = emptyfield;
  mathtype(accentchr(tail)) = mathchar;
  scan_fifteen_bit_int();
  character(accentchr(tail)) = cur_val & 255;
  if (cur_val >= varcode && faminrange) {
    fam(accentchr(tail)) = curfam;
  } else
    fam(accentchr(tail)) = (cur_val / 256) & 15;
  scanmath(nucleus(tail));
}
/*:1165*/

/*1172:*/
Static void appendchoices(void)
{
  tailappend(newchoice());
  saveptr++;
  saved(-1) = 0;
  pushmath(mathchoicegroup);
  scan_left_brace();
}
/*:1172*/

/*1174:*/
/*1184:*/
Static HalfWord finmlist(HalfWord p)
{
  Pointer q;

  if (incompleatnoad != 0) {   /*1185:*/
    mathtype(denominator(incompleatnoad)) = submlist;
    info(denominator(incompleatnoad)) = link(head);
    if (p == 0)
      q = incompleatnoad;
    else {
      q = info(numerator(incompleatnoad));
      if (type(q) != leftnoad)
	confusion(S(419));
      info(numerator(incompleatnoad)) = link(q);
      link(q) = incompleatnoad;
      link(incompleatnoad) = p;
    }
  }  /*:1185*/
  else {
    link(tail) = p;
    q = link(head);
  }
  popnest();
  return q;
}
/*:1184*/

Static void buildchoices(void) {
    Pointer p;

    unsave();
    p = finmlist(0);
    switch (saved(-1)) {
        case 0: displaymlist(tail) = p; break;
        case 1: textmlist(tail) = p; break;
        case 2: scriptmlist(tail) = p; break;
        case 3:
            scriptscriptmlist(tail) = p;
            saveptr--;
            return;
            break;
    }
    (saved(-1))++;
    pushmath(mathchoicegroup);
    scan_left_brace();
}
/*:1174*/

/*1176:*/
Static void subsup(void)
{
  SmallNumber t;
  Pointer p;

  t = EMPTY;
  p = 0;
  if (tail != head) {
    if (scriptsallowed(tail)) {
	if(curcmd == SUP_MARK) {
		p = supscr(tail);
	} else {
		p = subscr(tail);
	}
      t = mathtype(p);
    }
  }
  if (p == 0 || t != EMPTY) {   /*1177:*/
    tailappend(newnoad());
    if(curcmd == SUP_MARK) {
	p = supscr(tail);
    } else {
	p = subscr(tail);
    }
    if (t != EMPTY) {
      if (curcmd == SUP_MARK) {
	print_err(S(917));
	help1(S(918));
      } else {
	print_err(S(919));
	help1(S(920));
      }
      error();
    }
  }
  /*:1177*/
  scanmath(p);
}
/*:1176*/

/*1181:*/
Static void mathfraction(void)
{
  SmallNumber c;

  c = curchr;
  if (incompleatnoad != 0) {   /*1183:*/
    if (c >= delimitedcode) {
      scandelimiter(garbage, false);
      scandelimiter(garbage, false);
    }
    if (c % delimitedcode == abovecode) {
      SCAN_NORMAL_DIMEN();
    }
    print_err(S(921));
    help3(S(922),
          S(923),
          S(924));
    error();
    return;
  }  /*:1183*/
  incompleatnoad = getnode(fractionnoadsize);
  type(incompleatnoad) = fractionnoad;
  subtype(incompleatnoad) = NORMAL;
  mathtype(numerator(incompleatnoad)) = submlist;
  info(numerator(incompleatnoad)) = link(head);
  mem[denominator(incompleatnoad) - MEM_MIN].hh = emptyfield;
  mem[leftdelimiter(incompleatnoad) - MEM_MIN].qqqq = nulldelimiter;
  mem[rightdelimiter(incompleatnoad) - MEM_MIN].qqqq = nulldelimiter;
  link(head) = 0;
  tail = head;   /*1182:*/
  if (c >= delimitedcode) {
    scandelimiter(leftdelimiter(incompleatnoad), false);
    scandelimiter(rightdelimiter(incompleatnoad), false);
  }
  switch (c % delimitedcode) {   /*:1182*/

  case abovecode:
    SCAN_NORMAL_DIMEN();
    thickness(incompleatnoad) = cur_val;
    break;

  case overcode:
    thickness(incompleatnoad) = defaultcode;
    break;

  case atopcode:
    thickness(incompleatnoad) = 0;
    break;
  }
}
/*:1181*/

/*1191:*/
Static void mathleftright(void)
{
  SmallNumber t;
  Pointer p;

  t = curchr;
  if (t == rightnoad && curgroup != mathleftgroup) {   /*1192:*/
    if (curgroup != mathshiftgroup) {
      offsave();
      return;
    }
    scandelimiter(garbage, false);
    print_err(S(558));
    print_esc(S(419));
    help1(S(925));
    error();
    return;
  }
  /*:1192*/
  p = newnoad();
  type(p) = t;
  scandelimiter(delimiter(p), false);
  if (t == leftnoad) {
    pushmath(mathleftgroup);
    link(head) = p;
    tail = p;
    return;
  }
  p = finmlist(p);
  unsave();
  tailappend(newnoad());
  type(tail) = innernoad;
  mathtype(nucleus(tail)) = submlist;
  info(nucleus(tail)) = p;
}
/*:1191*/

/*1194:*/
Static void aftermath(void)
{
  Boolean l, danger;
  long m;
  Pointer p, a;
  /*1198:*/
  Pointer b, r, t;   /*:1198*/
  Scaled w, z, e, q, d, s;
  SmallNumber g1, g2;

  danger = false;   /*1195:*/
  if ((fontparams[famfnt(TEXT_SIZE + 2) ] < totalmathsyparams) |
      (fontparams[famfnt(SCRIPT_SIZE + 2) ] < totalmathsyparams) |
      (fontparams[famfnt(SCRIPT_SCRIPT_SIZE + 2) ] <
       totalmathsyparams)) {
    print_err(S(926));
    help3(S(927),
          S(928),
          S(929));
    error();
    flushmath();
    danger = true;
  } else if ((fontparams[famfnt(TEXT_SIZE + 3) ] <
	      totalmathexparams) |
	     (fontparams[famfnt(SCRIPT_SIZE + 3) ] <
	      totalmathexparams) |
	     (fontparams[famfnt(SCRIPT_SCRIPT_SIZE + 3) ] <
	      totalmathexparams)) {
    print_err(S(930));
    help3(S(931),
          S(932),
          S(933));
    error();
    flushmath();
    danger = true;
  }
  m = mode;
  l = false;
  p = finmlist(0);
  if (mode == -m) {   /*1197:*/
    get_x_token();
    if (curcmd != MATH_SHIFT) {   /*:1197*/
      print_err(S(745));
      help2(S(746),
            S(747));
      backerror();
    }
    curmlist = p;
    curstyle = textstyle;
    mlistpenalties = false;
    mlisttohlist();
    a = hpack(link(temphead), 0, additional);
    unsave();
    saveptr--;
    if (saved(0) == 1)
      l = true;
    danger = false;   /*1195:*/
    if ((fontparams[famfnt(TEXT_SIZE + 2) ] < totalmathsyparams) |
	(fontparams[famfnt(SCRIPT_SIZE + 2) ] < totalmathsyparams) |
	(fontparams[famfnt(SCRIPT_SCRIPT_SIZE + 2) ] <
	 totalmathsyparams)) {
      print_err(S(926));
      help3(S(927),
            S(928),
            S(929));
      error();
      flushmath();
      danger = true;
    } else if ((fontparams[famfnt(TEXT_SIZE + 3) ] <
		totalmathexparams) |
	       (fontparams[famfnt(SCRIPT_SIZE + 3) ] <
		totalmathexparams) |
	       (fontparams[famfnt(SCRIPT_SCRIPT_SIZE + 3) ] <
		totalmathexparams)) {
      print_err(S(930));
      help3(S(931),
            S(932),
            S(933));
      error();
      flushmath();
      danger = true;
    }
    m = mode;
    p = finmlist(0);
  } else
    a = 0;
  if (m < 0) {   /*1196:*/
    tailappend(newmath(mathsurround, before));
    curmlist = p;
    curstyle = textstyle;
    mlistpenalties = (mode > 0);
    mlisttohlist();
    link(tail) = link(temphead);
    while (link(tail) != 0)
      tail = link(tail);
    tailappend(newmath(mathsurround, after));
    spacefactor = 1000;
    unsave();
    return;
  }
  /*:1196*/
  if (a == 0) {   /*1197:*/
    get_x_token();
    if (curcmd != MATH_SHIFT) {
      print_err(S(745));
      help2(S(746),
            S(747));
      backerror();
    }
  }
  /*:1197*/
  /*1199:*/
  curmlist = p;
  curstyle = displaystyle;
  mlistpenalties = false;
  mlisttohlist();
  p = link(temphead);
  adjusttail = adjusthead;
  b = hpack(p, 0, additional);
  p = listptr(b);
  t = adjusttail;
  adjusttail = 0;
  w = width(b);
  z = displaywidth;
  s = displayindent;
  if (a == 0 || danger) {
    e = 0;
    q = 0;
  } else {
    e = width(a);
    q = e + mathquad(TEXT_SIZE);
  }
  if (w + q > z) {   /*1201:*/
    if (e != 0 && (w - totalshrink[0] + q <= z ||
		   totalshrink[FIL - NORMAL] != 0 ||
		   totalshrink[FILL - NORMAL] != 0 ||
		   totalshrink[FILLL - NORMAL] != 0)) {
      freenode(b, boxnodesize);
      b = hpack(p, z - q, exactly);
    } else {
      e = 0;
      if (w > z) {
	freenode(b, boxnodesize);
	b = hpack(p, z, exactly);
      }
    }
    w = width(b);
  }
  /*:1201*/
  /*1202:*/
  d = half(z - w);
  if (e > 0 && d < e * 2) {   /*:1202*/
    d = half(z - w - e);
    if (p != 0) {
      if (!ischarnode(p)) {
	if (type(p) == GLUE_NODE)
	  d = 0;
      }
    }
  }
  /*1203:*/
  tailappend(newpenalty(predisplaypenalty));
  if (d + s <= predisplaysize || l) {
    g1 = ABOVE_DISPLAY_SKIP_CODE;
    g2 = BELOW_DISPLAY_SKIP_CODE;
  } else {
    g1 = ABOVE_DISPLAY_SHORT_SKIP_CODE;
    g2 = BELOW_DISPLAY_SHORT_SKIP_CODE;
  }
  if (l && e == 0) {   /*1204:*/
    shiftamount(a) = s;
    appendtovlist(a);
    tailappend(newpenalty(INF_PENALTY));
  } else {
    tailappend(newparamglue(g1));   /*:1203*/
  }
  if (e != 0) {
    r = newkern(z - w - e - d);
    if (l) {
      link(a) = r;
      link(r) = b;
      b = a;
      d = 0;
    } else {
      link(b) = r;
      link(r) = a;
    }
    b = hpack(b, 0, additional);
  }
  shiftamount(b) = s + d;   /*:1204*/
  appendtovlist(b);   /*1205:*/
  if (a != 0 && e == 0 && !l) {
    tailappend(newpenalty(INF_PENALTY));
    shiftamount(a) = s + z - width(a);
    appendtovlist(a);
    g2 = 0;
  }
  if (t != adjusthead) {
    link(tail) = link(adjusthead);
    tail = t;
  }
  tailappend(newpenalty(postdisplaypenalty));
  if (g2 > 0) {   /*:1199*/
    tailappend(newparamglue(g2));   /*:1205*/
  }
  resumeafterdisplay();

  /*:1195*/
  /*:1195*/
}
/*:1194*/

/*1200:*/
Static void resumeafterdisplay(void)
{
  if (curgroup != mathshiftgroup)
    confusion(S(934));
  unsave();
  prevgraf += 3;
  pushnest();
  mode = H_MODE;
  spacefactor = 1000;
  setcurlang();
  clang = curlang;
  prevgraf = (normmin(lefthyphenmin) * 64 + normmin(righthyphenmin)) * 65536L +
	     curlang;
      /*443:*/
  get_x_token();
  if (curcmd != SPACER)   /*:443*/
    backinput();
  if (nest_ptr == 1)
    buildpage();
}
/*:1200*/


/*
 * [#1215]: Declare subprocedures for prefixed command
 * 
 * xref[10]
 *  1215, 1229, 1236, 1243, 1244, 
 *  1245, 1246, 1247, 1257, 1265
 *  used in [#1211]
 */

// #1215
Static void getrtoken(void) {
_Lrestart:
    do {
        gettoken();
    } while (curtok == spacetoken);
    if (curcs != 0 && curcs <= frozencontrolsequence) return;
    print_err(S(935));
    help5(S(936), S(937), S(938), S(939), S(940));
    if (curcs == 0) backinput();
    curtok = CS_TOKEN_FLAG + frozenprotection;
    inserror();
    goto _Lrestart;
} // #1215: getrtoken

// #1229
Static void trapzeroglue(void) {
    if (!( (width(cur_val) == 0) 
        && (stretch(cur_val) == 0) 
        && (shrink(cur_val) == 0)
    )) return;

    addglueref(zeroglue);
    delete_glue_ref(cur_val);
    cur_val = zeroglue;
} // #1229: trapzeroglue

// #1236
Static void doregistercommand(SmallNumber a) {
    Pointer l = 0 /* XXXX */, q, r, s;
    char p;

    q = curcmd; /*1237:*/
    if (q != REGISTER) {
        get_x_token();
        if (curcmd >= ASSIGN_INT && curcmd <= ASSIGN_MU_GLUE) {
            l = curchr;
            p = curcmd - ASSIGN_INT;
            goto _Lfound;
        }
        if (curcmd != REGISTER) {
            print_err(S(602));
            printcmdchr(curcmd, curchr);
            print(S(603));
            printcmdchr(q, 0);
            help1(S(941));
            error();
            goto _Lexit;
        }
    }
    p = curchr;
    scan_eight_bit_int();
    switch (p) {
        case INT_VAL: l = cur_val + countbase; break;
        case DIMEN_VAL: l = cur_val + SCALED_BASE; break;
        case GLUE_VAL: l = cur_val + skipbase; break;
        case MU_VAL: l = cur_val + muskipbase; break;
    }

_Lfound: /*:1237*/
    if (q == REGISTER)
        scan_optional_equals();
    else
        scankeyword(S(942));
    arith_error = false;
    if (q < MULTIPLY) { /*1238:*/
        if (p < GLUE_VAL) {
            if (p == INT_VAL)
                scan_int();
            else {
                SCAN_NORMAL_DIMEN();
            }
            if (q == ADVANCE) cur_val += eqtb[l - activebase].int_;
        } else { /*:1238*/
            scan_glue(p);
            if (q == ADVANCE) { /*1239:*/
                q = newspec(cur_val);
                r = equiv(l);
                delete_glue_ref(cur_val);
                width(q) += width(r);
                if (stretch(q) == 0) stretchorder(q) = NORMAL;
                if (stretchorder(q) == stretchorder(r))
                    stretch(q) += stretch(r);
                else if ((stretchorder(q) < stretchorder(r)) &
                         (stretch(r) != 0)) {
                    stretch(q) = stretch(r);
                    stretchorder(q) = stretchorder(r);
                }
                if (shrink(q) == 0) shrinkorder(q) = NORMAL;
                if (shrinkorder(q) == shrinkorder(r))
                    shrink(q) += shrink(r);
                else if ((shrinkorder(q) < shrinkorder(r)) & (shrink(r) != 0)) {
                    shrink(q) = shrink(r);
                    shrinkorder(q) = shrinkorder(r);
                }
                cur_val = q;
            }
            /*:1239*/
        }
    } else { /*1240:*/
        scan_int();
        if (p < GLUE_VAL) {
            if (q == MULTIPLY) {
                if (p == INT_VAL) {
                    cur_val = mult_integers(eqtb[l-activebase].int_, cur_val);
                } else {
                    cur_val = nx_plus_y(eqtb[l-activebase].int_, cur_val, 0);
                }
            } else {
                cur_val = x_over_n(eqtb[l - activebase].int_, cur_val);
            }
        } else {
            s = equiv(l);
            r = newspec(s);
            if (q == MULTIPLY) {
                width(r)   = nx_plus_y(width(s), cur_val, 0);
                stretch(r) = nx_plus_y(stretch(s), cur_val, 0);
                shrink(r)  = nx_plus_y(shrink(s), cur_val, 0);
            } else {
                width(r)   = x_over_n(width(s), cur_val);
                stretch(r) = x_over_n(stretch(s), cur_val);
                shrink(r)  = x_over_n(shrink(s), cur_val);
            }
            cur_val = r;
        }
    } /*:1240*/
    if (arith_error) {
        print_err(S(943));
        help2(S(944), S(945));
        error();
        goto _Lexit;
    }
    if (p < GLUE_VAL) {
        worddefine(l, cur_val);
    } else {
        trapzeroglue();
        define(l, GLUE_REF, cur_val);
    }
_Lexit:;
} // #1236: doregistercommand

// #1243
Static void alteraux(void) {
    HalfWord c;

    if (curchr != labs(mode)) {
        reportillegalcase();
        return;
    }
    c = curchr;
    scan_optional_equals();
    if (c == V_MODE) {
        SCAN_NORMAL_DIMEN();
        prevdepth = cur_val;
        return;
    }
    scan_int();
    if (cur_val > 0 && cur_val <= 32767) {
        spacefactor = cur_val;
        return;
    }
    print_err(S(946));
    help1(S(947));
    int_error(cur_val);
} // #1243: alteraux

// #1244
Static void alterprevgraf(void) {
    int p;

    nest[nest_ptr] = cur_list;
    p = nest_ptr;
    while (abs(nest[p].modefield) != V_MODE)
        p--;
    scan_optional_equals();
    scan_int();
    if (cur_val >= 0) {
        nest[p].pgfield = cur_val;
        cur_list = nest[nest_ptr];
        return;
    }
    print_err(S(773));
    print_esc(S(948));
    help1(S(949));
    int_error(cur_val);
} // #1244: alterprevgraf

// #1245
Static void alterpagesofar(void) {
    int c;

    c = curchr;
    scan_optional_equals();
    SCAN_NORMAL_DIMEN();
    pagesofar[c] = cur_val;
} // #1245: alterpagesofar

// #1246
Static void alterinteger(void) {
    char c;

    c = curchr;
    scan_optional_equals();
    scan_int();
    if (c == 0)
        deadcycles = cur_val;
    else
        insertpenalties = cur_val;
} // #1246: alterinteger

// #1247
Static void alterboxdimen(void) {
    SmallNumber c;
    EightBits b;

    c = curchr;
    scan_eight_bit_int();
    b = cur_val;
    scan_optional_equals();
    SCAN_NORMAL_DIMEN();
    if (box(b) != 0) mem[box(b) + c - MEM_MIN].sc = cur_val;
} // #1247: alterboxdimen

// #1257
Static void newfont(SmallNumber a) {
    Pointer u;
    Scaled s;
    InternalFontNumber f;
    StrNumber t;
    enum Selector old_setting;
    /* XXXX  StrNumber flushablestring; */

    if (job_name == 0) openlogfile();
    getrtoken();
    u = curcs;
    if (u >= hashbase) {
        t = text(u);
    } else if (u >= singlebase) {
        if (u == nullcs)
            t = S(950);
        else
            t = u - singlebase;
    } else {
        old_setting = selector;
        selector = NEW_STRING;
        print(S(950));
        print(u - activebase);
        selector = old_setting;
        str_room(1);
        t = makestring();
    }
    define(u, SET_FONT, NULL_FONT);
    scan_optional_equals();
    scanfilename(); /*1258:*/

    name_in_progress = true;
    if (scankeyword(S(951))) { /*1259:*/
        SCAN_NORMAL_DIMEN();
        s = cur_val;
        if (s <= 0 || s >= 134217728L) {
            print_err(S(952));
            print_scaled(s);
            print(S(953));
            help2(S(954), S(955));
            error();
            s = UNITY * 10;
        }
        /*:1259*/
    } else if (scankeyword(S(956))) {
        scan_int();
        s = -cur_val;
        if (cur_val <= 0 || cur_val > 32768L) {
            print_err(S(486));
            help1(S(487));
            int_error(cur_val);
            s = -1000;
        }
    } else {
        s = -1000;
    }
    name_in_progress = false;
    /*:1258*/

    /*1260:*/
    #if 0
        flushablestring = str_ptr - 1;
    #endif
    for (f = 1; f <= fontptr; f++) {
        if (str_eq_str(get_fontname(f), curname) /* &
	        str_eq_str(fontarea[f ], curarea) */ ) {
            /*:1260*/
            #if 0
                if (curname == flushablestring) {
                    flush_string();
                    curname = fontname[f ];
                }
            #endif
            if (s > 0) {
                if (s == get_fontsize(f)) goto _Lcommonending;
            } else if (get_fontsize(f) ==
                       xn_over_d(get_fontdsize(f), -s, 1000)) {
                goto _Lcommonending;
            }
        } // if(str_eq_str(...))
    }     // for (f = 1; f <= fontptr; f++)
    f = readfontinfo(u, curname, curarea, s);

_Lcommonending:
    equiv(u) = f;
    eqtb[fontidbase + f - activebase] = eqtb[u - activebase];
    set_fontidtext(f, t);
} // #1257: newfont

// #1265
Static void newinteraction(void) {
    println();
    interaction = curchr; /*75:*/
    if (interaction == BATCH_MODE) {
        selector = NO_PRINT;
    } else {
        selector = TERM_ONLY;
        /*:75*/
    }
    if (log_opened) selector += 2;
} // #1265: newinteraction


// [#1211]
Static void prefixedcommand(void) {
    SmallNumber a;
    InternalFontNumber f;
    HalfWord j;
    FontIndex k;
    Pointer p, q;
    long n;
    Boolean e;

    a = 0;
    while (curcmd == PREFIX) {
        if (!((a / curchr) & 1)) a += curchr;
        skip_spaces_or_relax();
        if (curcmd > MAX_NON_PREFIXED_COMMAND) /*1212:*/
            continue;
        /*:1212*/
        print_err(S(957));
        printcmdchr(curcmd, curchr);
        print_char('\'');
        help1(S(958));
        backerror();
        goto _Lexit;
    }                                    /*1213:*/
    if (curcmd != DEF && (a & 3) != 0) { /*:1213*/
        print_err(S(602));
        print_esc(S(959));
        print(S(960));
        print_esc(S(961));
        print(S(962));
        printcmdchr(curcmd, curchr);
        print_char('\'');
        help1(S(963));
        error();
    }
    /*1214:*/
    if (globaldefs != 0) {
        if (globaldefs < 0) {
            if (global) {
                a -= 4;
            }
        } else { /*:1214*/
            if (!global) {
                a += 4;
            }
        }
    }
    switch (curcmd) { /*1217:*/
        case SET_FONT: /*:1217*/ define(curfontloc, DATA, curchr); break;

        /*1218:*/
        case DEF: /*:1218*/
            if ((curchr & 1) && !global && globaldefs >= 0) {
                a += 4;
            }
            e = (curchr >= 2);
            getrtoken();
            p = curcs;
            q = scantoks(true, e);
            define(p, CALL + (a & 3), defref);
            break;
            /*1221:*/

        case LET:
            n = curchr;
            getrtoken();
            p = curcs;
            if (n == NORMAL) {
                do {
                    gettoken();
                } while (curcmd == SPACER);
                if (curtok == othertoken + '=') {
                    gettoken();
                    if (curcmd == SPACER) gettoken();
                }
            } else {
                gettoken();
                q = curtok;
                gettoken();
                backinput();
                curtok = q;
                backinput();
            }
            if (curcmd >= CALL) {
                addtokenref(curchr);
            }
            define(p, curcmd, curchr);
            break;

        /*:1221*/
        /*1224:*/
        case SHORTHAND_DEF:
            n = curchr;
            getrtoken();
            p = curcs;
            define(p, RELAX, 256);
            scan_optional_equals();
            switch (n) {

                case chardefcode:
                    scan_char_num();
                    define(p, CHAR_GIVEN, cur_val);
                    break;

                case mathchardefcode:
                    scan_fifteen_bit_int();
                    define(p, MATH_GIVEN, cur_val);
                    break;

                default:
                    scan_eight_bit_int();
                    switch (n) {

                        case countdefcode:
                            define(p, ASSIGN_INT, countbase + cur_val);
                            break;

                        case dimendefcode:
                            define(p, ASSIGN_DIMEN, SCALED_BASE + cur_val);
                            break;

                        case skipdefcode:
                            define(p, ASSIGN_GLUE, skipbase + cur_val);
                            break;

                        case muskipdefcode:
                            define(p, ASSIGN_MU_GLUE, muskipbase + cur_val);
                            break;

                        case toksdefcode:
                            define(p, ASSIGN_TOKS, toksbase + cur_val);
                            break;
                    }
                    break;
            }
            break;
            /*:1224*/

        /*1225:*/
        case READ_TO_CS: /*:1225*/
            scan_int();
            n = cur_val;
            if (!scankeyword(S(697))) {
                print_err(S(856));
                help2(S(964), S(965));
                error();
            }
            getrtoken();
            p = curcs;
            readtoks(n, p);
            define(p, CALL, cur_val);
            break;
            /*1226:*/

        case TOKS_REGISTER:
        case ASSIGN_TOKS: /*:1226*/
            q = curcs;
            if (curcmd == TOKS_REGISTER) {
                scan_eight_bit_int();
                p = toksbase + cur_val;
            } else
                p = curchr;
            scan_optional_equals();
            skip_spaces_or_relax();
            if (curcmd != LEFT_BRACE) { /*1227:*/
                int cur_chr = curchr;
                if (curcmd == TOKS_REGISTER) {
                    scan_eight_bit_int();
                    curcmd = ASSIGN_TOKS;
                    cur_chr = toksbase + cur_val;
                }
                if (curcmd == ASSIGN_TOKS) {
                    q = equiv(cur_chr);
                    if (q == 0) {
                        define(p, UNDEFINED_CS, 0);
                    } else {
                        addtokenref(q);
                        define(p, CALL, q);
                    }
                    goto _Ldone;
                }
            }
            /*:1227*/
            backinput();
            curcs = q;
            q = scantoks(false, false);
            if (link(defref) == 0) {
                define(p, UNDEFINED_CS, 0);
                FREE_AVAIL(defref);
            } else {
                if (p == outputroutineloc) {
                    link(q) = get_avail();
                    q = link(q);
                    info(q) = rightbracetoken + '}';
                    q = get_avail();
                    info(q) = leftbracetoken + '{';
                    link(q) = link(defref);
                    link(defref) = q;
                }
                define(p, CALL, defref);
            }
            break;
            /*1228:*/

        case ASSIGN_INT:
            p = curchr;
            scan_optional_equals();
            scan_int();
            worddefine(p, cur_val);
            break;

        case ASSIGN_DIMEN:
            p = curchr;
            scan_optional_equals();
            SCAN_NORMAL_DIMEN();
            worddefine(p, cur_val);
            break;

        case ASSIGN_GLUE:
        case ASSIGN_MU_GLUE: /*:1228*/
            p = curchr;
            n = curcmd;
            scan_optional_equals();
            if (n == ASSIGN_MU_GLUE)
                scan_glue(MU_VAL);
            else
                scan_glue(GLUE_VAL);
            trapzeroglue();
            define(p, GLUE_REF, cur_val);
            break;
            /*1232:*/

        case DEF_CODE: /*:1232*/
            /*1233:*/
            if (curchr == catcodebase)
                n = MAX_CHAR_CODE;
            else if (curchr == mathcodebase)
                n = 32768L;
            else if (curchr == sfcodebase)
                n = 32767;
            else if (curchr == delcodebase)
                n = 16777215L;
            else
                n = 255; /*:1233*/
            p = curchr;
            scan_char_num();
            p += cur_val;
            scan_optional_equals();
            scan_int();
            if ((cur_val < 0 && p < delcodebase) || cur_val > n) {
                print_err(S(966));
                print_int(cur_val);
                if (p < delcodebase)
                    print(S(967));
                else
                    print(S(968));
                print_int(n);
                help1(S(969));
                error();
                cur_val = 0;
            }
            if (p < mathcodebase) {
                define(p, DATA, cur_val);
            } else if (p < delcodebase) {
                define(p, DATA, cur_val);
            } else {
                worddefine(p, cur_val);
            }
            break;
            /*1234:*/

        case DEF_FAMILY: /*:1234*/
            p = curchr;
            scan_four_bit_int();
            p += cur_val;
            scan_optional_equals();
            scanfontident();
            define(p, DATA, cur_val);
            break;
            /*1235:*/

        case REGISTER:
        case ADVANCE:
        case MULTIPLY:
        case DIVIDE: /*:1235*/
            doregistercommand(a);
            break;
            /*1241:*/

        case SET_BOX: /*:1241*/
            scan_eight_bit_int();
            if (global) {
                n = cur_val + 256;
            } else
                n = cur_val;
            scan_optional_equals();
            if (set_box_allowed)
                scanbox(boxflag + n);
            else {
                print_err(S(597));
                print_esc(S(970));
                help2(S(971), S(972));
                error();
            }
            break;
            /*1242:*/

        case SET_AUX: alteraux(); break;
        case SET_PREV_GRAF: alterprevgraf(); break;
        case SET_PAGE_DIMEN: alterpagesofar(); break;
        case SET_PAGE_INT: alterinteger(); break;
        case SET_BOX_DIMEN:  alterboxdimen(); break;
            /*1248:*/

        case SET_SHAPE: /*:1248*/
            scan_optional_equals();
            scan_int();
            n = cur_val;
            if (n <= 0)
                p = 0;
            else {
                p = getnode(n * 2 + 1);
                info(p) = n;
                for (j = 1; j <= n; j++) {
                    SCAN_NORMAL_DIMEN();
                    mem[p + j * 2 - MEM_MIN - 1].sc = cur_val;
                    SCAN_NORMAL_DIMEN();
                    mem[p + j * 2 - MEM_MIN].sc = cur_val;
                }
            }
            define(parshapeloc, SHAPE_REF, p);
            break;

        case HYPH_DATA:
            if (curchr == 1) {
            #ifdef tt_INIT
                /// #1252
                newpatterns();
                goto _Ldone;
            #endif // #1252: tt_INIT
            } else {
                newhyphexceptions();
                goto _Ldone;
            }
            break;
            /*1253:*/

        case ASSIGN_FONT_DIMEN:
            findfontdimen(true);
            k = cur_val;
            scan_optional_equals();
            SCAN_NORMAL_DIMEN();
            fontinfo[k].sc = cur_val;
            break;

        case ASSIGN_FONT_INT:
            n = curchr;
            scanfontident();
            f = cur_val;
            scan_optional_equals();
            scan_int();
            if (n == 0)
                set_hyphenchar(f, cur_val);
            else
                set_skewchar(f, cur_val);
            break;
            /*:1253*/

        /*1256:*/
        case DEF_FONT: newfont(a); break;

        /*:1256*/
        /*1264:*/
        case SET_INTERACTION: newinteraction(); break;

        /*:1264*/
        default: confusion(S(973)); break;
    } // switch (curcmd)

_Ldone:
    /*1269:*/
    if (aftertoken != 0) { /*:1269*/
        curtok = aftertoken;
        backinput();
        aftertoken = 0;
    }
_Lexit:;
} // #1211: prefixedcommand


/*1270:*/
Static void doassignments(void) {
    while (true) {
        skip_spaces_or_relax();
        if (curcmd <= MAX_NON_PREFIXED_COMMAND) break;
        set_box_allowed = false;
        prefixedcommand();
        set_box_allowed = true;
    }
}
/*:1270*/

/*1275:*/
Static void openorclosein(void) {
    int c;
    int n;

    c = curchr;
    scan_four_bit_int();
    n = cur_val;
    if (readopen[n] != closed) {
        aclose(&readfile[n]);
        readopen[n] = closed;
    }
    if (c == 0) return;
    scan_optional_equals();
    scanfilename();
    if (curext == S(385)) curext = S(669);
    packfilename(curname, S(677), curext);
    if (a_open_in(&readfile[n])) readopen[n] = justopen;
}
/*:1275*/

/*1279:*/
Static void issuemessage(void) {
    enum Selector old_setting;
    char c;
    StrNumber s;

    c = curchr;
    link(garbage) = scantoks(false, true);
    old_setting = selector;
    selector = NEW_STRING;
    tokenshow(defref);
    selector = old_setting;
    flushlist(defref);
    str_room(1);
    s = makestring();
    if (c == 0) { /*1280:*/
        if (term_offset + str_length(s) > MAX_PRINT_LINE - 2) {
            println();
        } else if (term_offset > 0 || file_offset > 0)
            print_char(' ');
        slow_print(s);
        fflush(stdout);
        /*1283:*/
    } else {      /*:1283*/
        print_err(S(385));
        slow_print(s);
        if (errhelp != 0)
            use_err_help = true;
        else if (longhelpseen) {
            help1(S(974));
        } else {
            if (interaction < ERROR_STOP_MODE) longhelpseen = true;
            help4(S(975), S(976), S(977), S(978));
        }
        error();
        use_err_help = false;
    }
    /*:1280*/
    flush_string();
}
/*:1279*/

/*1288:*/
Static void shiftcase(void) {
    Pointer b, p;
    EightBits c;

    b = curchr;
    p = scantoks(false, false);
    p = link(defref);
    while (p != 0) { /*1289:*/
        HalfWord t = info(p);
        if (t < CS_TOKEN_FLAG + singlebase) { /*:1289*/
            c = t & (dwa_do_8 - 1);
            if (equiv(b + c) != 0) info(p) = t - c + equiv(b + c);
        }
        p = link(p);
    }
    backlist(link(defref));
    FREE_AVAIL(defref);
}
/*:1288*/

/*1293:*/
Static void showwhatever(void) {
    switch (curchr) {
        case showlists:
            begindiagnostic();
            showactivities();
            break;

        case showboxcode: /*1296:*/
            scan_eight_bit_int();
            begindiagnostic();
            printnl(S(979));
            print_int(cur_val);
            print_char('=');
            if (box(cur_val) == 0)
                print(S(465));
            else
                showbox(box(cur_val));
            break;
            /*:1296*/

        case showcode: /*1294:*/
            gettoken();
            printnl(S(980));
            if (curcs != 0) {
                sprint_cs(curcs);
                print_char('=');
            }
            printmeaning(curchr, curcmd);
            goto _Lcommonending;
            break;
            /*:1294*/
            /*1297:*/

        default:
            (void)thetoks();
            printnl(S(980));
            tokenshow(temphead);
            flushlist(link(temphead));
            goto _Lcommonending; /*:1297*/
            break;
    } /*1298:*/
    enddiagnostic(true);
    print_err(S(981));
    if (selector == TERM_AND_LOG) {
        if (tracingonline <= 0) { /*:1298*/
            selector = TERM_ONLY;
            print(S(982));
            selector = TERM_AND_LOG;
        }
    }
_Lcommonending:
    if (interaction < ERROR_STOP_MODE) {
        help_ptr = 0;
        errorcount--;
    } else if (tracingonline > 0) {
        help3(S(983), S(984), S(985));
    } else {
        help5(S(983), S(984), S(985), S(986), S(987));
    }
    error();
}
/*:1293*/


#ifdef tt_INIT
/// 455#1302: Declare action procedures for use by main control
Static void storefmtfile(void) { /*1304:*/
    long j, k, l, x;
    Pointer p, q;
    MemoryWord pppfmtfile;
    if (saveptr != 0) {
        print_err(S(988));
        help1(S(989));
        succumb();
    }
    /*:1304*/
    /*1328:*/
    selector = NEW_STRING;
    print(S(990));
    print(job_name);
    print_char(' ');
    print_int(year % 100);
    print_char('.');
    print_int(month);
    print_char('.');
    print_int(day);
    print_char(')');
    if (interaction == BATCH_MODE)
        selector = LOG_ONLY;
    else
        selector = TERM_AND_LOG;
    str_room(1);
    format_ident = makestring();
    packjobname(formatextension);
    while (!wopenout(&fmtfile))
        promptfilename(S(991), formatextension);
    printnl(S(992));
    slow_print(wmakenamestring());
    flush_string();
    printnl(S(385));        /*:1328*/
    slow_print(format_ident); /*1307:*/
    pppfmtfile.int_ = 371982687L;
    pput(pppfmtfile);
    pppfmtfile.int_ = MEM_BOT;
    pput(pppfmtfile);
    pppfmtfile.int_ = MEM_TOP;
    pput(pppfmtfile);
    pppfmtfile.int_ = EQTB_SIZE;
    pput(pppfmtfile);
    pppfmtfile.int_ = HASH_PRIME;
    pput(pppfmtfile);
    pppfmtfile.int_ = HYPH_SIZE;
    pput(pppfmtfile); /*:1307*/
    str_dump(fmtfile);

    sort_avail(); // #131

    var_used = 0;
    pppfmtfile.int_ = lo_mem_max;
    pput(pppfmtfile);
    pppfmtfile.int_ = rover;
    pput(pppfmtfile);
    p = MEM_BOT;
    q = rover;
    x = 0;
    do {
        for (k = p; k <= q + 1; k++) {
            pppfmtfile = mem[k - MEM_MIN];
            pput(pppfmtfile);
        }
        x += q - p + 2;
        var_used += q - p;
        p = q + nodesize(q);
        q = rlink(q);
    } while (q != rover);
    var_used += lo_mem_max - p;
    dyn_used = mem_end - hi_mem_min + 1;
    for (k = p; k <= lo_mem_max; k++) {
        pppfmtfile = mem[k - MEM_MIN];
        pput(pppfmtfile);
    }
    x += lo_mem_max - p + 1;
    pppfmtfile.int_ = hi_mem_min;
    pput(pppfmtfile);
    pppfmtfile.int_ = avail;
    pput(pppfmtfile);
    for (k = hi_mem_min; k <= mem_end; k++) {
        pppfmtfile = mem[k - MEM_MIN];
        pput(pppfmtfile);
    }
    x += mem_end - hi_mem_min + 1;
    p = avail;
    while (p != 0) {
        dyn_used -= CHAR_NODE_SIZE;
        p = link(p);
    }
    pppfmtfile.int_ = var_used;
    pput(pppfmtfile);
    pppfmtfile.int_ = dyn_used;
    pput(pppfmtfile);
    println();
    print_int(x);
    print(S(993));
    print_int(var_used);
    print_char('&');    /*:1311*/
    print_int(dyn_used); /*1313:*/
    /*1315:*/
    k = activebase;

    do { /*1316:*/
        j = k;
        while (j < intbase - 1) {
            if ((equiv(j) == equiv(j + 1)) & (eqtype(j) == eqtype(j + 1)) &
                (eqlevel(j) == eqlevel(j + 1)))
                goto _Lfound1;
            j++;
        }
        l = intbase;
        goto _Ldone1;
_Lfound1:
        j++;
        l = j;
        while (j < intbase - 1) {
            if ((equiv(j) != equiv(j + 1)) | (eqtype(j) != eqtype(j + 1)) |
                (eqlevel(j) != eqlevel(j + 1)))
                goto _Ldone1;
            j++;
        }
_Ldone1:
        pppfmtfile.int_ = l - k;
        pput(pppfmtfile);
        while (k < l) {
            pppfmtfile = eqtb[k - activebase];
            pput(pppfmtfile);
            k++;
        }
        k = j + 1;
        pppfmtfile.int_ = k - l;
        pput(pppfmtfile); /*:1315*/
    } while (k != intbase);
    do {
        j = k;
        while (j < EQTB_SIZE) {
            if (eqtb[j - activebase].int_ == eqtb[j - activebase + 1].int_)
                goto _Lfound2;
            j++;
        }
        l = EQTB_SIZE + 1;
        goto _Ldone2;
_Lfound2:
        j++;
        l = j;
        while (j < EQTB_SIZE) {
            if (eqtb[j - activebase].int_ != eqtb[j - activebase + 1].int_)
                goto _Ldone2;
            j++;
        }
_Ldone2:
        pppfmtfile.int_ = l - k;
        pput(pppfmtfile);
        while (k < l) {
            pppfmtfile = eqtb[k - activebase];
            pput(pppfmtfile);
            k++;
        }
        k = j + 1;
        pppfmtfile.int_ = k - l;
        pput(pppfmtfile); /*:1316*/
    } while (k <= EQTB_SIZE);

    pppfmtfile.int_ = parloc;
    pput(pppfmtfile);
    pppfmtfile.int_ = writeloc;
    pput(pppfmtfile); /*1318:*/
    pppfmtfile.int_ = hash_used;
    pput(pppfmtfile);
    cs_count = frozencontrolsequence - hash_used - 1;
    for (p = hashbase; p <= hash_used; p++) {
        if (text(p) != 0) {
            pppfmtfile.int_ = p;
            pput(pppfmtfile);
            pppfmtfile.hh = hash[p - hashbase];
            pput(pppfmtfile);
            cs_count++;
        }
    }
    for (p = hash_used + 1; p < UNDEFINED_CONTROL_SEQUENCE; p++) {
        pppfmtfile.hh = hash[p - hashbase];
        pput(pppfmtfile);
    }
    pppfmtfile.int_ = cs_count;
    pput(pppfmtfile);
    println();
    print_int(cs_count); /*:1318*/
    /*:1313*/
    print(S(994));
    fonts_dump(fmtfile);
    /*1324:*/
    pppfmtfile.int_ = hyphcount;
    pput(pppfmtfile);
    for (k = 0; k <= HYPH_SIZE; k++) {
        if (hyphword[k] != 0) {
            pppfmtfile.int_ = k;
            pput(pppfmtfile);
            pppfmtfile.int_ = hyphword[k];
            pput(pppfmtfile);
            pppfmtfile.int_ = hyphlist[k];
            pput(pppfmtfile);
        }
    }
    println();
    print_int(hyphcount);
    print(S(995));
    if (hyphcount != 1) print_char('s');
    if (trie_not_ready) inittrie();
    pppfmtfile.int_ = triemax;
    pput(pppfmtfile);
    for (k = 0; k <= triemax; k++) {
        pppfmtfile.hh = trie[k];
        pput(pppfmtfile);
    }
    pppfmtfile.int_ = trieopptr;
    pput(pppfmtfile);
    for (k = 0; k < trieopptr; k++) {
        pppfmtfile.int_ = hyfdistance[k];
        pput(pppfmtfile);
        pppfmtfile.int_ = hyfnum[k];
        pput(pppfmtfile);
        pppfmtfile.int_ = hyfnext[k];
        pput(pppfmtfile);
    }
    printnl(S(996));
    print_int(triemax);
    print(S(997));
    print_int(trieopptr);
    print(S(998));
    if (trieopptr != 1) print_char('s');
    print(S(999));
    print_int(TRIE_OP_SIZE);
    for (k = 255; k >= 0; k--) {            /*1326:*/
        if (trieused[k] > MIN_QUARTER_WORD) { /*:1324*/
            printnl(S(675));
            print_int(trieused[k] - MIN_QUARTER_WORD);
            print(S(1000));
            print_int(k);
            pppfmtfile.int_ = k;
            pput(pppfmtfile);
            pppfmtfile.int_ = trieused[k] - MIN_QUARTER_WORD;
            pput(pppfmtfile);
        }
    }
    pppfmtfile.int_ = interaction;
    pput(pppfmtfile);
    pppfmtfile.int_ = format_ident;
    pput(pppfmtfile);
    pppfmtfile.int_ = 69069L;
    pput(pppfmtfile);
    tracingstats = 0; /*:1326*/
    /*1329:*/
    /*:1329*/
    w_close(&fmtfile);
} // storefmtfile
#endif // #1302: tt_INIT


/*1348:*/
/*1349:*/
Static void newwhatsit(SmallNumber s, SmallNumber w) {
    Pointer p;

    p = getnode(w);
    type(p) = WHATSIT_NODE;
    subtype(p) = s;
    link(tail) = p;
    tail = p;
}
/*:1349*/

/*1350:*/
Static void newwritewhatsit(SmallNumber w) {
    newwhatsit(curchr, w);
    if (w != writenodesize)
        scan_four_bit_int();
    else {
        scan_int();
        if (cur_val < 0)
            cur_val = 17;
        else if (cur_val > 15)
            cur_val = 16;
    }
    writestream(tail) = cur_val;
}
/*:1350*/

Static void doextension(void)
{
  long k;
  Pointer p;

  switch (curchr) {

  case opennode:   /*1351:*/
    newwritewhatsit(opennodesize);
    scan_optional_equals();
    scanfilename();
    openname(tail) = curname;
    openarea(tail) = curarea;
    openext(tail) = curext;
    break;
    /*:1351*/

  case writenode:   /*1352:*/
    k = curcs;
    newwritewhatsit(writenodesize);
    curcs = k;
    p = scantoks(false, false);
    writetokens(tail) = defref;
    break;
    /*:1352*/

  case closenode:   /*1353:*/
    newwritewhatsit(writenodesize);
    writetokens(tail) = 0;
    break;
    /*:1353*/

  case specialnode:   /*1354:*/
    newwhatsit(specialnode, writenodesize);
    writestream(tail) = 0;
    p = scantoks(false, true);
    writetokens(tail) = defref;
    break;
    /*:1354*/

  case immediatecode:   /*1375:*/
    get_x_token();
    if (curcmd == EXTENSION && curchr <= closenode) {
      p = tail;
      doextension();
      outwhat(tail);
      flush_node_list(tail);
      tail = p;
      link(p) = 0;
    } else
      backinput();
    break;
    /*:1375*/

  case setlanguagecode:   /*1377:*/
    if (labs(mode) != H_MODE)
      reportillegalcase();
    else {   /*:1377*/
      newwhatsit(languagenode, smallnodesize);
      scan_int();
      if (cur_val <= 0)
	clang = 0;
      else if (cur_val > 255)
	clang = 0;
      else
	clang = cur_val;
      whatlang(tail) = clang;
      whatlhm(tail) = normmin(lefthyphenmin);
      whatrhm(tail) = normmin(righthyphenmin);
    }
    break;

  default:
    confusion(S(1001));
    break;
  }
}
/*:1348*/

/*1376:*/
Static void fixlanguage(void) {
    ASCIICode l;

    if (language <= 0)
        l = 0;
    else if (language > 255)
        l = 0;
    else
        l = language;
    if (l == clang) return;
    newwhatsit(languagenode, smallnodesize);
    whatlang(tail) = l;
    clang = l;
    whatlhm(tail) = normmin(lefthyphenmin);
    whatrhm(tail) = normmin(righthyphenmin);
}
/*:1376*/



/*1068:*/
Static void handlerightbrace(void) {
    Pointer p, q;
    Scaled d;
    long f;

    switch (curgroup) {

        case simplegroup:
            unsave();
            break;

        case bottomlevel:
            print_err(S(1002));
            help2(S(1003), S(1004));
            error();
            break;

        case semisimplegroup:
        case mathshiftgroup:
        case mathleftgroup:
            extrarightbrace();
            break;

        /*1085:*/
        case hboxgroup:
            package(0);
            break;

        case adjustedhboxgroup:
            adjusttail = adjusthead;
            package(0);
            break;

        case vboxgroup:
            endgraf();
            package(0);
            break;

        case vtopgroup: /*:1085*/
            endgraf();
            package(vtopcode);
            break;
            /*1100:*/

        case insertgroup:
            endgraf();
            q = splittopskip;
            addglueref(q);
            d = splitmaxdepth;
            f = floatingpenalty;
            unsave();
            saveptr--;
            p = vpack(link(head), 0, additional);
            popnest();
            if (saved(0) < 255) {
                tailappend(getnode(insnodesize));
                type(tail) = INS_NODE;
                subtype(tail) = saved(0);
                height(tail) = height(p) + depth(p);
                insptr(tail) = listptr(p);
                splittopptr(tail) = q;
                depth(tail) = d;
                floatcost(tail) = f;
            } else {
                tailappend(getnode(smallnodesize));
                type(tail) = ADJUST_NODE;
                subtype(tail) = 0;
                adjustptr(tail) = listptr(p);
                delete_glue_ref(q);
            }
            freenode(p, boxnodesize);
            if (nest_ptr == 0) buildpage();
            break;

        case outputgroup: /*1026:*/
            if (LOC != 0 ||
                (token_type != OUTPUT_TEXT && token_type != BACKED_UP)) { /*:1027*/
                print_err(S(1005));
                help2(S(1006), S(682));
                error();
                do {
                    gettoken();
                } while (LOC != 0);
            }
            endtokenlist();
            endgraf();
            unsave();
            outputactive = false;
            insertpenalties = 0; /*1028:*/
            if (box(255) != 0) { /*:1028*/
                print_err(S(1007));
                print_esc(S(464));
                print_int(255);
                help3(S(1008), S(1009), S(1010));
                boxerror(255);
            }
            if (tail != head) {
                link(pagetail) = link(head);
                pagetail = tail;
            }
            if (link(pagehead) != 0) {
                if (link(contribhead) == 0) contribtail = pagetail;
                link(pagetail) = link(contribhead);
                link(contribhead) = link(pagehead);
                link(pagehead) = 0;
                pagetail = pagehead;
            }
            popnest();
            buildpage();
            break;
            /*:1026*/
            /*:1100*/
            /*1118:*/

        case discgroup: /*:1118*/
            builddiscretionary();
            break;
            /*1132:*/

        case aligngroup: /*:1132*/
            backinput();
            curtok = CS_TOKEN_FLAG + frozencr;
            print_err(S(554));
            print_esc(S(737));
            print(S(555));
            help1(S(1011));
            inserror();
            break;
            /*1133:*/

        case noaligngroup: /*:1133*/
            endgraf();
            unsave();
            alignpeek();
            break;
            /*1168:*/

        case vcentergroup: /*:1168*/
            endgraf();
            unsave();
            saveptr -= 2;
            p = vpack(link(head), saved(1), saved(0));
            popnest();
            tailappend(newnoad());
            type(tail) = vcenternoad;
            mathtype(nucleus(tail)) = subbox;
            info(nucleus(tail)) = p;
            break;
            /*1173:*/

        case mathchoicegroup: /*:1173*/
            buildchoices();
            break;
            /*1186:*/

        case mathgroup:
            unsave();
            saveptr--;
            mathtype(saved(0)) = submlist;
            p = finmlist(0);
            info(saved(0)) = p;
            if (p != 0) {
                if (link(p) == 0) {
                    if (type(p) == ordnoad) {
                        if (mathtype(subscr(p)) == EMPTY) {
                            if (mathtype(supscr(p)) == EMPTY) {
                                mem[saved(0) - MEM_MIN].hh =
                                    mem[nucleus(p) - MEM_MIN].hh;
                                freenode(p, noadsize);
                            }
                        }
                    } else if (type(p) == accentnoad) {
                        if (saved(0) == nucleus(tail)) {
                            if (type(tail) == ordnoad) { /*1187:*/
                                q = head;
                                while (link(q) != tail)
                                    q = link(q);
                                link(q) = p;
                                freenode(tail, noadsize);
                                tail = p;
                            }
                            /*:1187*/
                        }
                    }
                }
            }
            break;
            /*:1186*/

        default:
            confusion(S(1012));
            break;
    }

    /*1027:*/
} /*:1068*/

#ifdef tt_DEBUG
// 交互式 debug 环境
/// p470#1338
void debughelp(void) {
    long k, l, m, n;

    while (true) {
        printnl(S(1253)); // "debug # (−1 to exit):"
        fflush(stdout);

        fscanf(stdin, " %ld", &m);
        if (m < 0) {
            return;
        }
        if (m == 0) {
            goto _Lbreakpoint_;
        _Lbreakpoint_:
            m = 0;
            continue;
        }

        fscanf(stdin, " %ld", &n);
        switch (m) {
            /// #1339
            // display mem[n] in all forms
            case 1: printword(mem[n - MEM_MIN]); break;
            case 2: print_int(info(n)); break;
            case 3: print_int(link(n)); break;
            case 4: printword(eqtb[n - activebase]); break;
            case 5: printword(fontinfo[n]); break;
            case 6: printword(savestack[n]); break;
            // show a box, abbreviated by show box depth and show box breadth
            case 7: showbox(n); break;
            case 8: {
                breadth_max = 10000;
                depth_threshold = str_adjust_to_room(POOL_SIZE) - 10;
                shownodelist(n);
                break;
            }
            case 9: showtokenlist(n, 0, 1000); break;
            case 10: slow_print(n); break;
            // check wellformedness; print new busy locations if n > 0
            case 11: checkmem(n > 0); break;
            // look for pointers to n
            case 12: searchmem(n); break;
            case 13: {
                fscanf(stdin, " %ld", &l);
                printcmdchr(n, l);
                break;
            }
            case 14: {
                for (k = 0; k <= n; k++)
                    print(buffer[k]);
                break;
            }
            case 15: {
                font_in_short_display = NULL_FONT;
                shortdisplay(n);
                break;
            }
            case 16: panicking = !panicking; break;

            default: print('?'); break;
        } // switch (m)
    }     // while (true)
} // #1338: debughelp
#endif // #1338: tt_DEBUG


/*
 * main 一次调用部分
 *
 *  + initialize
 *  + init_prim
 *  + S1337_Get_the_first_line_of_input_and_prepare_to_start
 *      + open_fmt_file
 *      + load_fmt_file
 *      ++ startinput
 *  + main_control
 *  + final_cleanup
 *  + close_files_and_terminate
 *
 */

// [#1030]: governs T E X’s activities
// xref[17]:
//      [1030], 1032, 1040, 1041,
//  1052, 1054, 1055, 1056, 1057,
//  1126, 1134, 1208, 1290, 1332,
//  1337, 1344, 1347
Static void main_control(void) {
    Integer t;

    if (everyjob != 0) begintokenlist(everyjob, EVERY_JOB_TEXT);

_LN_main_control__big_switch:
    get_x_token();

_LN_main_control__reswitch:
    // #1031: Give diagnostic information, if requested
    if (interrupt != 0) {
        if (OK_to_interrupt) {
            backinput();
            checkinterrupt();
            goto _LN_main_control__big_switch;
        }
    }
    #ifdef tt_DEBUG
        if (panicking) checkmem(false);
    #endif // #1031: tt_DEBUG
    if (tracingcommands > 0) /*:1031*/
        showcurcmdchr();
    switch (labs(mode) + curcmd) {
        case H_MODE + LETTER:
        case H_MODE + OTHER_CHAR:
        case H_MODE + CHAR_GIVEN:
            goto _Lmainloop;
            break;

        case H_MODE + CHAR_NUM:
            scan_char_num();
            curchr = cur_val;
            goto _Lmainloop;
            break;

        case H_MODE + NO_BOUNDARY:
            get_x_token();
            if (curcmd == LETTER || curcmd == OTHER_CHAR ||
                curcmd == CHAR_GIVEN || curcmd == CHAR_NUM)
                cancelboundary = true;
            goto _LN_main_control__reswitch;
            break;

        case H_MODE + SPACER:
            if (spacefactor == 1000) goto _Lappendnormalspace_;
            appspace();
            break;

        case H_MODE + EX_SPACE:
        case M_MODE + EX_SPACE: /*1045:*/
            goto _Lappendnormalspace_;
            break;

        case V_MODE:
        case H_MODE:
        case M_MODE:
        case V_MODE + SPACER:
        case M_MODE + SPACER:
        case M_MODE + NO_BOUNDARY:
            /* blank case */
            break;

        case V_MODE + IGNORE_SPACES:
        case H_MODE + IGNORE_SPACES:
        case M_MODE + IGNORE_SPACES:
            skip_spaces();
            goto _LN_main_control__reswitch;
            break;

        case V_MODE + STOP: /*1048:*/
            if (itsallover()) goto _Lexit;
            break;

        case V_MODE + VMOVE:
        case H_MODE + HMOVE:
        case M_MODE + HMOVE:
        case V_MODE + LAST_ITEM:
        case H_MODE + LAST_ITEM:
        case M_MODE + LAST_ITEM:
        case V_MODE + VADJUST:
        case V_MODE + ITAL_CORR:
        case V_MODE + EQ_NO:
        case H_MODE + EQ_NO:
        case V_MODE + MAC_PARAM:
        case H_MODE + MAC_PARAM:
        case M_MODE + MAC_PARAM: /*:1048*/
            reportillegalcase();
            break;
            /*1046:*/

        case V_MODE + SUP_MARK:
        case H_MODE + SUP_MARK:
        case V_MODE + SUB_MARK:
        case H_MODE + SUB_MARK:
        case V_MODE + MATH_CHAR_NUM:
        case H_MODE + MATH_CHAR_NUM:
        case V_MODE + MATH_GIVEN:
        case H_MODE + MATH_GIVEN:
        case V_MODE + MATH_COMP:
        case H_MODE + MATH_COMP:
        case V_MODE + DELIM_NUM:
        case H_MODE + DELIM_NUM:
        case V_MODE + LEFT_RIGHT:
        case H_MODE + LEFT_RIGHT:
        case V_MODE + ABOVE:
        case H_MODE + ABOVE:
        case V_MODE + RADICAL:
        case H_MODE + RADICAL:
        case V_MODE + MATH_STYLE:
        case H_MODE + MATH_STYLE:
        case V_MODE + MATH_CHOICE:
        case H_MODE + MATH_CHOICE:
        case V_MODE + VCENTER:
        case H_MODE + VCENTER:
        case V_MODE + NON_SCRIPT:
        case H_MODE + NON_SCRIPT:
        case V_MODE + MKERN:
        case H_MODE + MKERN:
        case V_MODE + LIMIT_SWITCH:
        case H_MODE + LIMIT_SWITCH:
        case V_MODE + MSKIP:
        case H_MODE + MSKIP:
        case V_MODE + MATH_ACCENT:
        case H_MODE + MATH_ACCENT:
        case M_MODE + ENDV:
        case M_MODE + PAR_END:
        case M_MODE + STOP:
        case M_MODE + VSKIP:
        case M_MODE + UN_VBOX:
        case M_MODE + VALIGN:
        case M_MODE + HRULE: /*:1046*/
            insertdollarsign();
            break;

        /*1056:*/
        case V_MODE + HRULE:
        case H_MODE + VRULE:
        case M_MODE + VRULE: /*:1056*/
            tailappend(scan_rule_spec());
            if (labs(mode) == V_MODE)
                prevdepth = ignoredepth;
            else if (labs(mode) == H_MODE)
                spacefactor = 1000;
            break;
            /*1057:*/

        case V_MODE + VSKIP:
        case H_MODE + HSKIP:
        case M_MODE + HSKIP:
        case M_MODE + MSKIP:
            appendglue();
            break;

        case V_MODE + KERN:
        case H_MODE + KERN:
        case M_MODE + KERN:
        case M_MODE + MKERN: /*:1057*/
            appendkern();
            break;
            /*1063:*/

        case V_MODE + LEFT_BRACE:
        case H_MODE + LEFT_BRACE:
            newsavelevel(simplegroup);
            break;

        case V_MODE + BEGIN_GROUP:
        case H_MODE + BEGIN_GROUP:
        case M_MODE + BEGIN_GROUP:
            newsavelevel(semisimplegroup);
            break;

        case V_MODE + END_GROUP:
        case H_MODE + END_GROUP:
        case M_MODE + END_GROUP: /*:1063*/
            if (curgroup == semisimplegroup)
                unsave();
            else
                offsave();
            break;
            /*1067:*/

        case V_MODE + RIGHT_BRACE:
        case H_MODE + RIGHT_BRACE:
        case M_MODE + RIGHT_BRACE:
            handlerightbrace();
            break;

        /*:1067*/
        /*1073:*/
        case V_MODE + HMOVE:
        case H_MODE + VMOVE:
        case M_MODE + VMOVE:
            t = curchr;
            SCAN_NORMAL_DIMEN();
            if (t == 0)
                scanbox(cur_val);
            else
                scanbox(-cur_val);
            break;

        case V_MODE + LEADER_SHIP:
        case H_MODE + LEADER_SHIP:
        case M_MODE + LEADER_SHIP:
            scanbox(leaderflag - aleaders + curchr);
            break;

        case V_MODE + MAKE_BOX:
        case H_MODE + MAKE_BOX:
        case M_MODE + MAKE_BOX:
            beginbox(0);
            break;

        /*:1073*/
        /*1090:*/
        case V_MODE + START_PAR:
            newgraf(curchr > 0);
            break;

        case V_MODE + LETTER:
        case V_MODE + OTHER_CHAR:
        case V_MODE + CHAR_NUM:
        case V_MODE + CHAR_GIVEN:
        case V_MODE + MATH_SHIFT:
        case V_MODE + UN_HBOX:
        case V_MODE + VRULE:
        case V_MODE + ACCENT:
        case V_MODE + DISCRETIONARY:
        case V_MODE + HSKIP:
        case V_MODE + VALIGN:
        case V_MODE + EX_SPACE:
        case V_MODE + NO_BOUNDARY: /*:1090*/
            backinput();
            newgraf(true);
            break;
            /*1092:*/

        case H_MODE + START_PAR:
        case M_MODE + START_PAR: /*:1092*/
            indentinhmode();
            break;
            /*1094:*/

        case V_MODE + PAR_END:
            normalparagraph();
            if (mode > 0) buildpage();
            break;

        case H_MODE + PAR_END:
            if (align_state < 0) offsave();
            endgraf();
            if (mode == V_MODE) buildpage();
            break;

        case H_MODE + STOP:
        case H_MODE + VSKIP:
        case H_MODE + HRULE:
        case H_MODE + UN_VBOX:
        case H_MODE + HALIGN: /*:1094*/
            headforvmode();
            break;
            /*1097:*/

        case V_MODE + INSERT:
        case H_MODE + INSERT:
        case M_MODE + INSERT:
        case H_MODE + VADJUST:
        case M_MODE + VADJUST:
            begininsertoradjust();
            break;

        case V_MODE + MARK:
        case H_MODE + MARK:
        case M_MODE + MARK: /*:1097*/
            makemark();
            break;

        /*1102:*/
        case V_MODE + BREAK_PENALTY:
        case H_MODE + BREAK_PENALTY:
        case M_MODE + BREAK_PENALTY:
            appendpenalty();
            break;

        /*:1102*/
        /*1104:*/
        case V_MODE + REMOVE_ITEM:
        case H_MODE + REMOVE_ITEM:
        case M_MODE + REMOVE_ITEM: /*:1104*/
            deletelast();
            break;

        /*1109:*/
        case V_MODE + UN_VBOX:
        case H_MODE + UN_HBOX:
        case M_MODE + UN_HBOX:
            unpackage();
            break;

        /*:1109*/
        /*1112:*/
        case H_MODE + ITAL_CORR:
            appenditaliccorrection();
            break;

        case M_MODE + ITAL_CORR: /*:1112*/
            tailappend(newkern(0));
            break;
            /*1116:*/

        case H_MODE + DISCRETIONARY:
        case M_MODE + DISCRETIONARY: /*:1116*/
            appenddiscretionary();
            break;

        /*1122:*/
        case H_MODE + ACCENT:
            makeaccent();
            break;

        /*:1122*/
        /*1126:*/
        case V_MODE + CAR_RET:
        case H_MODE + CAR_RET:
        case M_MODE + CAR_RET:
        case V_MODE + TAB_MARK:
        case H_MODE + TAB_MARK:
        case M_MODE + TAB_MARK:
            alignerror();
            break;

        case V_MODE + NO_ALIGN:
        case H_MODE + NO_ALIGN:
        case M_MODE + NO_ALIGN:
            noalignerror();
            break;

        case V_MODE + OMIT:
        case H_MODE + OMIT:
        case M_MODE + OMIT: /*:1126*/
            omiterror();
            break;
            /*1130:*/

        case V_MODE + HALIGN:
        case H_MODE + VALIGN:
            initalign();
            break;

        case M_MODE + HALIGN:
            if (privileged()) {
                if (curgroup == mathshiftgroup)
                    initalign();
                else
                    offsave();
            }
            break;

        case V_MODE + ENDV:
        case H_MODE + ENDV: /*:1130*/
            doendv();
            break;
            /*1134:*/

        case V_MODE + END_CS_NAME:
        case H_MODE + END_CS_NAME:
        case M_MODE + END_CS_NAME: /*:1134*/
            cserror();
            break;
            /*1137:*/

        case H_MODE + MATH_SHIFT: /*:1137*/
            initmath();
            break;
            /*1140:*/

        case M_MODE + EQ_NO: /*:1140*/
            if (privileged()) {
                if (curgroup == mathshiftgroup)
                    starteqno();
                else
                    offsave();
            }
            break;
            /*1150:*/

        case M_MODE + LEFT_BRACE: /*:1150*/
            tailappend(newnoad());
            backinput();
            scanmath(nucleus(tail));
            break;
            /*1154:*/

        case M_MODE + LETTER:
        case M_MODE + OTHER_CHAR:
        case M_MODE + CHAR_GIVEN:
            setmathchar(mathcode(curchr));
            break;

        case M_MODE + CHAR_NUM:
            scan_char_num();
            curchr = cur_val;
            setmathchar(mathcode(curchr));
            break;

        case M_MODE + MATH_CHAR_NUM:
            scan_fifteen_bit_int();
            setmathchar(cur_val);
            break;

        case M_MODE + MATH_GIVEN:
            setmathchar(curchr);
            break;

        case M_MODE + DELIM_NUM: /*:1154*/
            scan_twenty_seven_bit_int();
            setmathchar(cur_val / 4096);
            break;
            /*1158:*/

        case M_MODE + MATH_COMP:
            tailappend(newnoad());
            type(tail) = curchr;
            scanmath(nucleus(tail));
            break;

        case M_MODE + LIMIT_SWITCH: /*:1158*/
            mathlimitswitch();
            break;
            /*1162:*/

        case M_MODE + RADICAL: /*:1162*/
            mathradical();
            break;
            /*1164:*/

        case M_MODE + ACCENT:
        case M_MODE + MATH_ACCENT: /*:1164*/
            mathac();
            break;
            /*1167:*/

        case M_MODE + VCENTER:
            scanspec(vcentergroup, false);
            normalparagraph();
            pushnest();
            mode = -V_MODE;
            prevdepth = ignoredepth;
            if (everyvbox != 0) begintokenlist(everyvbox, EVERY_VBOX_TEXT);
            break;
            /*:1167*/

        /*1171:*/
        case M_MODE + MATH_STYLE:
            tailappend(newstyle(curchr));
            break;

        case M_MODE + NON_SCRIPT:
            tailappend(newglue(zeroglue));
            subtype(tail) = condmathglue;
            break;

        case M_MODE + MATH_CHOICE:
            appendchoices();
            break;

        /*:1171*/
        /*1175:*/
        case M_MODE + SUB_MARK:
        case M_MODE + SUP_MARK:
            subsup();
            break;

        /*:1175*/
        /*1180:*/
        case M_MODE + ABOVE: /*:1180*/
            mathfraction();
            break;
            /*1190:*/

        case M_MODE + LEFT_RIGHT:
            mathleftright();
            break;

        /*:1190*/
        /*1193:*/
        case M_MODE + MATH_SHIFT:
            if (curgroup == mathshiftgroup)
                aftermath();
            else
                offsave();
            break;

        /*:1193*/
        /*1210:*/
        case V_MODE + TOKS_REGISTER:
        case H_MODE + TOKS_REGISTER:
        case M_MODE + TOKS_REGISTER:
        case V_MODE + ASSIGN_TOKS:
        case H_MODE + ASSIGN_TOKS:
        case M_MODE + ASSIGN_TOKS:
        case V_MODE + ASSIGN_INT:
        case H_MODE + ASSIGN_INT:
        case M_MODE + ASSIGN_INT:
        case V_MODE + ASSIGN_DIMEN:
        case H_MODE + ASSIGN_DIMEN:
        case M_MODE + ASSIGN_DIMEN:
        case V_MODE + ASSIGN_GLUE:
        case H_MODE + ASSIGN_GLUE:
        case M_MODE + ASSIGN_GLUE:
        case V_MODE + ASSIGN_MU_GLUE:
        case H_MODE + ASSIGN_MU_GLUE:
        case M_MODE + ASSIGN_MU_GLUE:
        case V_MODE + ASSIGN_FONT_DIMEN:
        case H_MODE + ASSIGN_FONT_DIMEN:
        case M_MODE + ASSIGN_FONT_DIMEN:
        case V_MODE + ASSIGN_FONT_INT:
        case H_MODE + ASSIGN_FONT_INT:
        case M_MODE + ASSIGN_FONT_INT:
        case V_MODE + SET_AUX:
        case H_MODE + SET_AUX:
        case M_MODE + SET_AUX:
        case V_MODE + SET_PREV_GRAF:
        case H_MODE + SET_PREV_GRAF:
        case M_MODE + SET_PREV_GRAF:
        case V_MODE + SET_PAGE_DIMEN:
        case H_MODE + SET_PAGE_DIMEN:
        case M_MODE + SET_PAGE_DIMEN:
        case V_MODE + SET_PAGE_INT:
        case H_MODE + SET_PAGE_INT:
        case M_MODE + SET_PAGE_INT:
        case V_MODE + SET_BOX_DIMEN:
        case H_MODE + SET_BOX_DIMEN:
        case M_MODE + SET_BOX_DIMEN:
        case V_MODE + SET_SHAPE:
        case H_MODE + SET_SHAPE:
        case M_MODE + SET_SHAPE:
        case V_MODE + DEF_CODE:
        case H_MODE + DEF_CODE:
        case M_MODE + DEF_CODE:
        case V_MODE + DEF_FAMILY:
        case H_MODE + DEF_FAMILY:
        case M_MODE + DEF_FAMILY:
        case V_MODE + SET_FONT:
        case H_MODE + SET_FONT:
        case M_MODE + SET_FONT:
        case V_MODE + DEF_FONT:
        case H_MODE + DEF_FONT:
        case M_MODE + DEF_FONT:
        case V_MODE + REGISTER:
        case H_MODE + REGISTER:
        case M_MODE + REGISTER:
        case V_MODE + ADVANCE:
        case H_MODE + ADVANCE:
        case M_MODE + ADVANCE:
        case V_MODE + MULTIPLY:
        case H_MODE + MULTIPLY:
        case M_MODE + MULTIPLY:
        case V_MODE + DIVIDE:
        case H_MODE + DIVIDE:
        case M_MODE + DIVIDE:
        case V_MODE + PREFIX:
        case H_MODE + PREFIX:
        case M_MODE + PREFIX:
        case V_MODE + LET:
        case H_MODE + LET:
        case M_MODE + LET:
        case V_MODE + SHORTHAND_DEF:
        case H_MODE + SHORTHAND_DEF:
        case M_MODE + SHORTHAND_DEF:
        case V_MODE + READ_TO_CS:
        case H_MODE + READ_TO_CS:
        case M_MODE + READ_TO_CS:
        case V_MODE + DEF:
        case H_MODE + DEF:
        case M_MODE + DEF:
        case V_MODE + SET_BOX:
        case H_MODE + SET_BOX:
        case M_MODE + SET_BOX:
        case V_MODE + HYPH_DATA:
        case H_MODE + HYPH_DATA:
        case M_MODE + HYPH_DATA:
        case V_MODE + SET_INTERACTION:
        case H_MODE + SET_INTERACTION:
        case M_MODE + SET_INTERACTION: /*:1210*/
            prefixedcommand();
            break;
            /*1268:*/

        case V_MODE + AFTER_ASSIGNMENT:
        case H_MODE + AFTER_ASSIGNMENT:
        case M_MODE + AFTER_ASSIGNMENT: /*:1268*/
            gettoken();
            aftertoken = curtok;
            break;
            /*1271:*/

        case V_MODE + AFTER_GROUP:
        case H_MODE + AFTER_GROUP:
        case M_MODE + AFTER_GROUP: /*:1271*/
            gettoken();
            saveforafter(curtok);
            break;
            /*1274:*/

        case V_MODE + IN_STREAM:
        case H_MODE + IN_STREAM:
        case M_MODE + IN_STREAM: /*:1274*/
            openorclosein();
            break;

        /*1276:*/
        case V_MODE + MESSAGE:
        case H_MODE + MESSAGE:
        case M_MODE + MESSAGE:
            issuemessage();
            break;

        /*:1276*/
        /*1285:*/
        case V_MODE + CASE_SHIFT:
        case H_MODE + CASE_SHIFT:
        case M_MODE + CASE_SHIFT:
            shiftcase();
            break;

        /*:1285*/
        /*1290:*/
        case V_MODE + XRAY:
        case H_MODE + XRAY:
        case M_MODE + XRAY:
            showwhatever();
            break;

        /*:1290*/
        /*1347:*/
        case V_MODE + EXTENSION:
        case H_MODE + EXTENSION:
        case M_MODE + EXTENSION: /*:1347*/
            doextension();
            break;
            /*:1045*/
    }
    goto _LN_main_control__big_switch;

/// [#1030] 均从上方跳入
_Lmainloop:
    // [#1034]: Append character `cur_chr` and the following characters (if any)
    // to the current hlist in the current font; 
    // goto `_reswitch`
    //      when a non-character has been fetched
    adjustspacefactor();
    mainf = curfont;
    bchar = fontbchar[mainf];
    falsebchar = fontfalsebchar[mainf];
    if (mode > 0) {
        if (language != clang) fixlanguage();
    }
    FAST_GET_AVAIL(ligstack);
    font(ligstack) = mainf;
    curl = curchr;
    character(ligstack) = curl;
    curq = tail;
    if (cancelboundary) {
        cancelboundary = false;
        maink = nonaddress;
    } else
        maink = bcharlabel[mainf];
    if (maink == nonaddress) goto _Lmainloopmove2;
    curr = curl;
    curl = NON_CHAR;
    goto _Lmainligloop1;

_Lmainloopwrapup:  /*1035:*/
    wrapup(rthit); /*:1035*/

_Lmainloopmove:    /*1036:*/
    if (ligstack == 0) goto _LN_main_control__reswitch;
    curq = tail;
    curl = character(ligstack);

_Lmainloopmove1:
    if (!ischarnode(ligstack)) goto _Lmainloopmovelig;

_Lmainloopmove2:
    if (curchr < fontbc[mainf] || curchr > fontec[mainf]) {
        charwarning(mainf, curchr);
        FREE_AVAIL(ligstack);
        goto _LN_main_control__big_switch;
    }
    maini = charinfo(mainf, curl);
    if (!charexists(maini)) {
        charwarning(mainf, curchr);
        FREE_AVAIL(ligstack);
        goto _LN_main_control__big_switch;
    }
    tailappend(ligstack); /*:1036*/

_Lmainlooplookahead:      /*1038:*/
    getnext();
    if (curcmd == LETTER) goto _Lmainlooplookahead1;
    if (curcmd == OTHER_CHAR) goto _Lmainlooplookahead1;
    if (curcmd == CHAR_GIVEN) goto _Lmainlooplookahead1;
    xtoken();
    if (curcmd == LETTER) goto _Lmainlooplookahead1;
    if (curcmd == OTHER_CHAR) goto _Lmainlooplookahead1;
    if (curcmd == CHAR_GIVEN) goto _Lmainlooplookahead1;
    if (curcmd == CHAR_NUM) {
        scan_char_num();
        curchr = cur_val;
        goto _Lmainlooplookahead1;
    }
    if (curcmd == NO_BOUNDARY) bchar = NON_CHAR;
    curr = bchar;
    ligstack = 0;
    goto _Lmainligloop;

_Lmainlooplookahead1:
    adjustspacefactor();
    FAST_GET_AVAIL(ligstack);
    font(ligstack) = mainf;
    curr = curchr;
    character(ligstack) = curr;
    if (curr == falsebchar) curr = NON_CHAR; /*:1038*/

_Lmainligloop:                              /*1039:*/
    if (chartag(maini) != LIG_TAG) {
        goto _Lmainloopwrapup;
    }
    maink = ligkernstart(mainf, maini);
    mainj = fontinfo[maink].qqqq;
    if (skipbyte(mainj) <= stopflag) goto _Lmainligloop2;
    maink = ligkernrestart(mainf, mainj);

_Lmainligloop1:
    mainj = fontinfo[maink].qqqq;

_Lmainligloop2:
    if (nextchar(mainj) == curr) {
        if (skipbyte(mainj) <= stopflag) { /*1040:*/
            if (opbyte(mainj) >= kernflag) {
                wrapup(rthit);
                tailappend(newkern(charkern(mainf, mainj)));
                goto _Lmainloopmove;
            }
            if (curl == NON_CHAR)
                lfthit = true;
            else if (ligstack == 0)
                rthit = true;
            checkinterrupt();
            switch (opbyte(mainj)) {

                case MIN_QUARTER_WORD + 1:
                case MIN_QUARTER_WORD + 5:
                    curl = rembyte(mainj);
                    maini = charinfo(mainf, curl);
                    ligaturepresent = true;
                    break;

                case MIN_QUARTER_WORD + 2:
                case MIN_QUARTER_WORD + 6:
                    curr = rembyte(mainj);
                    if (ligstack == 0) {
                        ligstack = newligitem(curr);
                        bchar = NON_CHAR;
                    } else if (ischarnode(ligstack)) {
                        mainp = ligstack;
                        ligstack = newligitem(curr);
                        ligptr(ligstack) = mainp;
                    } else
                        character(ligstack) = curr;
                    break;

                case MIN_QUARTER_WORD + 3:
                    curr = rembyte(mainj);
                    mainp = ligstack;
                    ligstack = newligitem(curr);
                    link(ligstack) = mainp;
                    break;

                case MIN_QUARTER_WORD + 7:
                case MIN_QUARTER_WORD + 11:
                    wrapup(false);
                    curq = tail;
                    curl = rembyte(mainj);
                    maini = charinfo(mainf, curl);
                    ligaturepresent = true;
                    break;

                default:
                    curl = rembyte(mainj);
                    ligaturepresent = true;
                    if (ligstack == 0)
                        goto _Lmainloopwrapup;
                    else
                        goto _Lmainloopmove1;
                    break;
            }
            if (opbyte(mainj) > MIN_QUARTER_WORD + 4) {
                if (opbyte(mainj) != MIN_QUARTER_WORD + 7) goto _Lmainloopwrapup;
            }
            if (curl < NON_CHAR) goto _Lmainligloop;
            maink = bcharlabel[mainf];
            goto _Lmainligloop1;
        }
        /*:1040*/
    }
    if (skipbyte(mainj) == MIN_QUARTER_WORD)
        maink++;
    else {
        if (skipbyte(mainj) >= stopflag) goto _Lmainloopwrapup;
        maink += skipbyte(mainj) - MIN_QUARTER_WORD + 1;
    }
    goto _Lmainligloop1; /*:1039*/

_Lmainloopmovelig:       /*1037:*/
    mainp = ligptr(ligstack);
    if (mainp > 0) {
        tailappend(mainp);
    }
    temp_ptr = ligstack;
    ligstack = link(temp_ptr);
    freenode(temp_ptr, smallnodesize);
    maini = charinfo(mainf, curl);
    ligaturepresent = true;
    if (ligstack == 0) {
        if (mainp > 0) goto _Lmainlooplookahead;
        curr = bchar;
    } else
        curr = character(ligstack);
    goto _Lmainligloop;          /*:1037*/
                                 /*:1034*/

/// [#1030]
_Lappendnormalspace_:
    // #1041: Append a normal inter-word space to the current list, 
    // then goto big switch
    if (spaceskip == zeroglue) { /*1042:*/
        mainp = fontglue[curfont];
        if (mainp == 0) { /*:1042*/
            FontIndex mmaink;
            mainp = newspec(zeroglue);
            mmaink = parambase[curfont] + SPACE_CODE;
    #if 1
            maink = mmaink;
    #endif
            width(mainp) = fontinfo[mmaink].sc;
            stretch(mainp) = fontinfo[mmaink + 1].sc;
            shrink(mainp) = fontinfo[mmaink + 2].sc;
            fontglue[curfont] = mainp;
        }
        temp_ptr = newglue(mainp);
    } else
        temp_ptr = newparamglue(SPACE_SKIP_CODE);
    link(tail) = temp_ptr;
    tail = temp_ptr;
    goto _LN_main_control__big_switch;

_Lexit:;
} // #1030: main_control

// #1303: call open_fmt@func.c
Static Boolean open_fmt_file(void) { return open_fmt(&fmtfile); }

Static Boolean load_fmt_file(void) { /*1308:*/
    Boolean Result;
    long j, k, x;
    Pointer p, q;
    /* FourQuarters w; */
    MemoryWord pppfmtfile;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x != 371982687L) goto _Lbadfmt_;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x != MEM_BOT) goto _Lbadfmt_;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x != MEM_TOP) goto _Lbadfmt_;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x != EQTB_SIZE) goto _Lbadfmt_;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x != HASH_PRIME) goto _Lbadfmt_;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x != HYPH_SIZE) /*1310:*/
        goto _Lbadfmt_;
    if (!str_undump(fmtfile, stdout)) goto _Lbadfmt_;
    /*1312:*/
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x < lomemstatmax + 1000 || x >= himemstatmin) goto _Lbadfmt_;
    lo_mem_max = x;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x <= lomemstatmax || x > lo_mem_max) goto _Lbadfmt_;
    rover = x;
    p = MEM_BOT;
    q = rover;
    do {
        for (k = p; k <= q + 1; k++) {
            pget(pppfmtfile);
            mem[k - MEM_MIN] = pppfmtfile;
        }
        p = q + nodesize(q);
        if ((p > lo_mem_max) | ((q >= rlink(q)) & (rlink(q) != rover)))
            goto _Lbadfmt_;
        q = rlink(q);
    } while (q != rover);
    for (k = p; k <= lo_mem_max; k++) {
        pget(pppfmtfile);
        mem[k - MEM_MIN] = pppfmtfile;
    }
    if (MEM_MIN < MEM_BOT - 2) {
        p = llink(rover);
        q = MEM_MIN + 1;
        link(MEM_MIN) = 0;
        info(MEM_MIN) = 0;
        rlink(p) = q;
        llink(rover) = q;
        rlink(q) = rover;
        llink(q) = p;
        link(q) = emptyflag;
        nodesize(q) = MEM_BOT - q;
    }
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x <= lo_mem_max || x > himemstatmin) goto _Lbadfmt_;
    hi_mem_min = x;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if ((unsigned long)x > MEM_TOP) goto _Lbadfmt_;
    avail = x;
    mem_end = MEM_TOP;
    for (k = hi_mem_min; k <= mem_end; k++) {
        pget(pppfmtfile);
        mem[k - MEM_MIN] = pppfmtfile;
    }
    pget(pppfmtfile);
    var_used = pppfmtfile.int_;
    pget(pppfmtfile);
    dyn_used = pppfmtfile.int_; /*:1312*/
    /*1314:*/
    /*1317:*/
    k = activebase;
    do {
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if (x < 1 || k + x > EQTB_SIZE + 1) goto _Lbadfmt_;
        for (j = k; j < k + x; j++) {
            pget(pppfmtfile);
            eqtb[j - activebase] = pppfmtfile;
        }
        k += x;
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if (x < 0 || k + x > EQTB_SIZE + 1) goto _Lbadfmt_;
        for (j = k; j < k + x; j++)
            eqtb[j - activebase] = eqtb[k - activebase - 1];
        k += x; /*:1317*/
    } while (k <= EQTB_SIZE);
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x < hashbase || x > frozencontrolsequence) goto _Lbadfmt_;
    parloc = x;
    partoken = CS_TOKEN_FLAG + parloc;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x < hashbase || x > frozencontrolsequence) /*1319:*/
        goto _Lbadfmt_;
    writeloc = x;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x < hashbase || x > frozencontrolsequence) goto _Lbadfmt_;
    hash_used = x;
    p = hashbase - 1;
    do {
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if (x <= p || x > hash_used) goto _Lbadfmt_;
        p = x;
        pget(pppfmtfile);
        hash[p - hashbase] = pppfmtfile.hh;
    } while (p != hash_used);
    for (p = hash_used + 1; p < UNDEFINED_CONTROL_SEQUENCE; p++) {
        pget(pppfmtfile);
        hash[p - hashbase] = pppfmtfile.hh;
    }
    pget(pppfmtfile);
    cs_count = pppfmtfile.int_; /*:1319*/
    /*:1314*/
    if (!fonts_undump(fmtfile, stdout)) goto _Lbadfmt_;
    /*1325:*/
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if ((unsigned long)x > HYPH_SIZE) goto _Lbadfmt_;
    hyphcount = x;
    for (k = 1; k <= hyphcount; k++) {
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if ((unsigned long)x > HYPH_SIZE) goto _Lbadfmt_;
        j = x;
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if (!str_valid(x)) goto _Lbadfmt_;
        hyphword[j] = x;
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if ((unsigned long)x > MAX_HALF_WORD) goto _Lbadfmt_;
        hyphlist[j] = x;
    }
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x < 0) goto _Lbadfmt_;
    if (x >TRIE_SIZE) {
        fprintf(stdout, "---! Must increase the trie size\n");
        goto _Lbadfmt_;
    }
    j = x;
    #ifdef tt_INIT
        triemax = j;
    #endif // #1325.1: tt_INIT
    for (k = 0; k <= j; k++) {
        pget(pppfmtfile);
        trie[k] = pppfmtfile.hh;
    }
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x < 0) goto _Lbadfmt_;
    if (x > TRIE_OP_SIZE) {
        fprintf(stdout, "---! Must increase the trie op size\n");
        goto _Lbadfmt_;
    }
    j = x;
    #ifdef tt_INIT
        trieopptr = j;
    #endif // #1325.2: tt_INIT
    for (k = 1; k <= j; k++) {
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if ((unsigned long)x > 63) goto _Lbadfmt_;
        hyfdistance[k - 1] = x;
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if ((unsigned long)x > 63) goto _Lbadfmt_;
        hyfnum[k - 1] = x;
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if ((unsigned long)x > MAX_QUARTER_WORD) goto _Lbadfmt_;
        hyfnext[k - 1] = x;
    }
    #ifdef tt_INIT
        for (k = 0; k <= 255; k++)
            trieused[k] = MIN_QUARTER_WORD;
    #endif // #1325.3: tt_INIT
    k = 256;
    while (j > 0) {
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if (x >= k) goto _Lbadfmt_;
        k = x;
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if (x < 1 || x > j) goto _Lbadfmt_;
        #ifdef tt_INIT
            trieused[k] = x;
        #endif // #1325.4: tt_INIT
        j -= x;
        opstart[k] = j - MIN_QUARTER_WORD;
    }
    #ifdef tt_INIT
        trie_not_ready = false;
    #endif // #1325.5: tt_INIT
       /*:1325*/

    /*1327:*/
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if ((unsigned long)x > ERROR_STOP_MODE) goto _Lbadfmt_;
    interaction = x;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (!str_valid(x)) goto _Lbadfmt_;
    format_ident = x;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if ((x != 69069L) | feof(fmtfile)) goto _Lbadfmt_; /*:1327*/
    Result = true;
    goto _Lexit;

_Lbadfmt_:
    fprintf(stdout, "(Fatal format file error; I'm stymied)\n");
    Result = false;

_Lexit:
    return Result;
} // #1303: load_fmt_file

// #37: initterminal @lexer.c

// #1333
Static void close_files_and_terminate(void) {
    Integer k; // all-purpose index

    // #1378: Finish the extensions
    for (k = 0; k <= 15; k++) {
        if (writeopen[k])
            aclose(&write_file[k]);
    }

    #ifdef tt_STAT
        if (tracingstats > 0 && log_opened) {
            // #1334: Output statistics about this job
            fprintf(log_file, " \n");
            fprintf(log_file, "Here is how much of TeX's memory you used:\n");
            str_print_stats(log_file);
            fprintf(log_file,
                    " %ld words of memory out of %ld\n",
                    lo_mem_max - MEM_MIN + mem_end - hi_mem_min + 2L,
                    mem_end - MEM_MIN + 1L);
            fprintf(log_file,
                    " %ld multiletter control sequences out of %ld\n",
                    cs_count,
                    (long)HASH_SIZE);
            fprintf(log_file,
                    " %d words of font info for %d font",
                    fmemptr,
                    fontptr);
            if (fontptr != 1) {
                fprintf(log_file, "s");
            }
            fprintf(log_file,
                    ", out of %ld for %ld\n",
                    (long)FONT_MEM_SIZE,
                    (long)(FONT_MAX));
            fprintf(log_file, " %d hyphenation exception", hyphcount);
            if (hyphcount != 1) {
                fprintf(log_file, "s");
            }
            fprintf(log_file, " out of %ld\n", (long)HYPH_SIZE);
            fprintf(log_file,
                    " %di,%dn,%ldp,%db,%ds stack positions out of "
                    "%ldi,%ldn,%ldp,%ldb,%lds\n",
                    get_maxinstack(),
                    max_nest_stack,
                    maxparamstack,
                    max_buf_stack + 1,
                    maxsavestack + 6,
                    (long)STACK_SIZE,
                    (long)NEST_SIZE,
                    (long)PARAM_SIZE,
                    (long)BUF_SIZE,
                    (long)SAVE_SIZE);
        }  // if (tracingstats > 0 && log_opened)
    #endif // #1333: tt_STAT

    // #642: Finish the DVI file
    while (curs > -1) {
        if (curs > 0) {
            dviout_POP();
        } else {
            dviout_EOP();
            totalpages++;
        }
        curs--;
    }

    if (totalpages == 0) {
        printnl(S(1013)); // "No pages of output."
    } else { /*:642*/
        long total_dvi_bytes;
        dviout_POST(); // beginning of the postamble
        _dvi_lastbop(); // post location
        // conversion ratio for sp
        dvi_four(25400000L);
        dvi_four(473628672L);
        // magnification factor
        preparemag();
        dvi_four(mag);
        dvi_four(maxv);
        dvi_four(maxh);
        dviout(maxpush / 256);
        dviout(maxpush % 256);
        dviout((totalpages / 256) % 256);
        dviout(totalpages % 256);
        while (fontptr > 0) {
            if (fontused[fontptr]) dvi_font_def(fontptr);
            fontptr--;
        }

        total_dvi_bytes = dviflush();
        printnl(S(1014)); // "Output written on "
        slow_print(output_file_name);
        print(S(303));  // " ("
        print_int(totalpages);
        print(S(1015)); // " page"
        if (totalpages != 1) print_char('s');
        print(S(1016)); // ", "
        print_int(total_dvi_bytes);
        print(S(1017)); // " bytes)."
    } // if (totalpages == 0) - else

    if (!log_opened) return;
    putc('\n', log_file);
    aclose(&log_file);
    selector -= 2;
    if (selector != TERM_ONLY) return;
    printnl(S(1018));
    slow_print(logname);
    print_char('.');
    println();
} // #1333: void close_files_and_terminate(void)

/// p468#1335: Last-minute procedures
Static void final_cleanup(void) {
    SmallNumber c;

    c = curchr;
    if (job_name == 0) openlogfile();
    while (inputptr > 0) {
        if (STATE == TOKEN_LIST)
            endtokenlist();
        else
            endfilereading();
    }
    while (openparens > 0) {
        print(S(1019));
        openparens--;
    }
    if (curlevel > levelone) {
        printnl('(');
    #ifndef USE_REAL_STR
        print_esc(S(1020)); // "end occurred "
        print(S(1021));     // "inside a group at level "
    #else
        print_esc_str("end occurred ");
        print_str("inside a group at level ");
    #endif // USE_REAL_STR
        print_int(curlevel - levelone);
        print_char(')');
    }
    while (condptr != 0) {
        printnl('(');
        print_esc(S(1020));
        print(S(1022));
        printcmdchr(IF_TEST, curif);
        if (ifline != 0) {
            print(S(1023));
            print_int(ifline);
        }
        print(S(1024));
        ifline = iflinefield(condptr);
        curif = subtype(condptr);
        temp_ptr = condptr;
        condptr = link(condptr);
        freenode(temp_ptr, ifnodesize);
    }
    if (history != SPOTLESS) {
        if (history == WARNING_ISSUED || interaction < ERROR_STOP_MODE) {
            if (selector == TERM_AND_LOG) {
                selector = TERM_ONLY;
                printnl(S(1025));
                selector = TERM_AND_LOG;
            }
        }
    }
    if (c == 1) {
    #ifdef tt_INIT
            for (int i = topmarkcode; i <= splitbotmarkcode; i++) {
                if (curmark[i] != 0) delete_token_ref(curmark[i]);
            }
            storefmtfile();
    #endif // #1335: tt_INIT
    }
} // #1335: final_cleanup

#ifdef tt_INIT
// TeX 原语定义
/// p468#1336: initialize all the primitives
Static void init_prim(void) {
    /*226:*/
    primitive(S(341), ASSIGN_GLUE, gluebase);
    primitive(S(342), ASSIGN_GLUE, gluebase + BASELINE_SKIP_CODE);
    primitive(S(343), ASSIGN_GLUE, gluebase + PAR_SKIP_CODE);
    primitive(S(344), ASSIGN_GLUE, gluebase + ABOVE_DISPLAY_SKIP_CODE);
    primitive(S(345), ASSIGN_GLUE, gluebase + BELOW_DISPLAY_SKIP_CODE);
    primitive(S(346), ASSIGN_GLUE, gluebase + ABOVE_DISPLAY_SHORT_SKIP_CODE);
    primitive(S(347), ASSIGN_GLUE, gluebase + BELOW_DISPLAY_SHORT_SKIP_CODE);
    primitive(S(348), ASSIGN_GLUE, gluebase + LEFT_SKIP_CODE);
    primitive(S(349), ASSIGN_GLUE, gluebase + RIGHT_SKIP_CODE);
    primitive(S(350), ASSIGN_GLUE, gluebase + TOP_SKIP_CODE);
    primitive(S(351), ASSIGN_GLUE, gluebase + SPLIT_TOP_SKIP_CODE);
    primitive(S(352), ASSIGN_GLUE, gluebase + TAB_SKIP_CODE);
    primitive(S(353), ASSIGN_GLUE, gluebase + SPACE_SKIP_CODE);
    primitive(S(354), ASSIGN_GLUE, gluebase + XSPACE_SKIP_CODE);
    primitive(S(355), ASSIGN_GLUE, gluebase + PAR_FILL_SKIP_CODE);
    primitive(S(356), ASSIGN_MU_GLUE, gluebase + THIN_MU_SKIP_CODE);
    primitive(S(357), ASSIGN_MU_GLUE, gluebase + MED_MU_SKIP_CODE);
    primitive(S(358), ASSIGN_MU_GLUE, gluebase + THICK_MU_SKIP_CODE);
    /*:226*/
    /*230:*/
    primitive(S(1026), ASSIGN_TOKS, outputroutineloc);
    primitive(S(1027), ASSIGN_TOKS, everyparloc);
    primitive(S(1028), ASSIGN_TOKS, everymathloc);
    primitive(S(1029), ASSIGN_TOKS, everydisplayloc);
    primitive(S(1030), ASSIGN_TOKS, everyhboxloc);
    primitive(S(1031), ASSIGN_TOKS, everyvboxloc);
    primitive(S(1032), ASSIGN_TOKS, everyjobloc);
    primitive(S(1033), ASSIGN_TOKS, everycrloc);
    primitive(S(1034), ASSIGN_TOKS, errhelploc); /*:230*/
    /*238:*/
    primitive(S(1035), ASSIGN_INT, intbase);
    primitive(S(1036), ASSIGN_INT, intbase + tolerancecode);
    primitive(S(1037), ASSIGN_INT, intbase + linepenaltycode);
    primitive(S(1038), ASSIGN_INT, intbase + hyphenpenaltycode);
    primitive(S(1039), ASSIGN_INT, intbase + exhyphenpenaltycode);
    primitive(S(1040), ASSIGN_INT, intbase + clubpenaltycode);
    primitive(S(1041), ASSIGN_INT, intbase + widowpenaltycode);
    primitive(S(1042), ASSIGN_INT, intbase + displaywidowpenaltycode);
    primitive(S(1043), ASSIGN_INT, intbase + brokenpenaltycode);
    primitive(S(1044), ASSIGN_INT, intbase + binoppenaltycode);
    primitive(S(1045), ASSIGN_INT, intbase + relpenaltycode);
    primitive(S(1046), ASSIGN_INT, intbase + predisplaypenaltycode);
    primitive(S(1047), ASSIGN_INT, intbase + postdisplaypenaltycode);
    primitive(S(1048), ASSIGN_INT, intbase + interlinepenaltycode);
    primitive(S(1049), ASSIGN_INT, intbase + doublehyphendemeritscode);
    primitive(S(1050), ASSIGN_INT, intbase + finalhyphendemeritscode);
    primitive(S(1051), ASSIGN_INT, intbase + adjdemeritscode);
    primitive(S(1052), ASSIGN_INT, intbase + magcode);
    primitive(S(1053), ASSIGN_INT, intbase + delimiterfactorcode);
    primitive(S(1054), ASSIGN_INT, intbase + loosenesscode);
    primitive(S(1055), ASSIGN_INT, intbase + timecode);
    primitive(S(1056), ASSIGN_INT, intbase + daycode);
    primitive(S(1057), ASSIGN_INT, intbase + monthcode);
    primitive(S(1058), ASSIGN_INT, intbase + yearcode);
    primitive(S(1059), ASSIGN_INT, intbase + showboxbreadthcode);
    primitive(S(1060), ASSIGN_INT, intbase + showboxdepthcode);
    primitive(S(1061), ASSIGN_INT, intbase + hbadnesscode);
    primitive(S(1062), ASSIGN_INT, intbase + vbadnesscode);
    primitive(S(1063), ASSIGN_INT, intbase + pausingcode);
    primitive(S(1064), ASSIGN_INT, intbase + tracingonlinecode);
    primitive(S(1065), ASSIGN_INT, intbase + tracingmacroscode);
    primitive(S(1066), ASSIGN_INT, intbase + tracingstatscode);
    primitive(S(1067), ASSIGN_INT, intbase + tracingparagraphscode);
    primitive(S(1068), ASSIGN_INT, intbase + tracingpagescode);
    primitive(S(1069), ASSIGN_INT, intbase + tracingoutputcode);
    primitive(S(1070), ASSIGN_INT, intbase + tracinglostcharscode);
    primitive(S(1071), ASSIGN_INT, intbase + tracingcommandscode);
    primitive(S(1072), ASSIGN_INT, intbase + tracingrestorescode);
    primitive(S(1073), ASSIGN_INT, intbase + uchyphcode);
    primitive(S(1074), ASSIGN_INT, intbase + outputpenaltycode);
    primitive(S(1075), ASSIGN_INT, intbase + maxdeadcyclescode);
    primitive(S(1076), ASSIGN_INT, intbase + hangaftercode);
    primitive(S(1077), ASSIGN_INT, intbase + floatingpenaltycode);
    primitive(S(1078), ASSIGN_INT, intbase + globaldefscode);
    primitive(S(333), ASSIGN_INT, intbase + curfamcode);
    primitive(S(1079), ASSIGN_INT, intbase + ESCAPE_CHARcode);
    primitive(S(1080), ASSIGN_INT, intbase + defaulthyphencharcode);
    primitive(S(1081), ASSIGN_INT, intbase + defaultskewcharcode);
    primitive(S(1082), ASSIGN_INT, intbase + endlinecharcode);
    primitive(S(1083), ASSIGN_INT, intbase + newlinecharcode);
    primitive(S(1084), ASSIGN_INT, intbase + languagecode);
    primitive(S(1085), ASSIGN_INT, intbase + lefthyphenmincode);
    primitive(S(1086), ASSIGN_INT, intbase + righthyphenmincode);
    primitive(S(1087), ASSIGN_INT, intbase + holdinginsertscode);
    primitive(S(1088), ASSIGN_INT, intbase + errorcontextlinescode);
    /*:238*/
    /*248:*/
    primitive(S(1089), ASSIGN_DIMEN, dimenbase);
    primitive(S(1090), ASSIGN_DIMEN, dimenbase + mathsurroundcode);
    primitive(S(1091), ASSIGN_DIMEN, dimenbase + lineskiplimitcode);
    primitive(S(1092), ASSIGN_DIMEN, dimenbase + hsizecode);
    primitive(S(1093), ASSIGN_DIMEN, dimenbase + vsizecode);
    primitive(S(1094), ASSIGN_DIMEN, dimenbase + maxdepthcode);
    primitive(S(1095), ASSIGN_DIMEN, dimenbase + splitmaxdepthcode);
    primitive(S(1096), ASSIGN_DIMEN, dimenbase + boxmaxdepthcode);
    primitive(S(1097), ASSIGN_DIMEN, dimenbase + hfuzzcode);
    primitive(S(1098), ASSIGN_DIMEN, dimenbase + vfuzzcode);
    primitive(S(1099), ASSIGN_DIMEN, dimenbase + delimitershortfallcode);
    primitive(S(1100), ASSIGN_DIMEN, dimenbase + nulldelimiterspacecode);
    primitive(S(1101), ASSIGN_DIMEN, dimenbase + scriptspacecode);
    primitive(S(1102), ASSIGN_DIMEN, dimenbase + predisplaysizecode);
    primitive(S(1103), ASSIGN_DIMEN, dimenbase + displaywidthcode);
    primitive(S(1104), ASSIGN_DIMEN, dimenbase + displayindentcode);
    primitive(S(1105), ASSIGN_DIMEN, dimenbase + overfullrulecode);
    primitive(S(1106), ASSIGN_DIMEN, dimenbase + hangindentcode);
    primitive(S(1107), ASSIGN_DIMEN, dimenbase + hoffsetcode);
    primitive(S(1108), ASSIGN_DIMEN, dimenbase + voffsetcode);
    primitive(S(1109), ASSIGN_DIMEN, dimenbase + emergencystretchcode);
    /*:248*/
    /*265:*/
    primitive(' ', EX_SPACE, 0);
    primitive('/', ITAL_CORR, 0);
    primitive(S(417), ACCENT, 0);
    primitive(S(1110), ADVANCE, 0);
    primitive(S(1111), AFTER_ASSIGNMENT, 0);
    primitive(S(1112), AFTER_GROUP, 0);
    primitive(S(1113), BEGIN_GROUP, 0);
    primitive(S(1114), CHAR_NUM, 0);
    primitive(S(262), CS_NAME, 0);
    primitive(S(1115), DELIM_NUM, 0);
    primitive(S(1116), DIVIDE, 0);
    primitive(S(263), END_CS_NAME, 0);
    primitive(S(836), END_GROUP, 0);
    text(frozenendgroup) = S(836);
    eqtb[frozenendgroup - activebase] = eqtb[cur_val - activebase];
    primitive(S(1117), EXPAND_AFTER, 0);
    primitive(S(1118), DEF_FONT, 0);
    primitive(S(1119), ASSIGN_FONT_DIMEN, 0);
    primitive(S(724), HALIGN, 0);
    primitive(S(863), HRULE, 0);
    primitive(S(1120), IGNORE_SPACES, 0);
    primitive(S(374), INSERT, 0);
    primitive(S(402), MARK, 0);
    primitive(S(913), MATH_ACCENT, 0);
    primitive(S(1121), MATH_CHAR_NUM, 0);
    primitive(S(404), MATH_CHOICE, 0);
    primitive(S(1122), MULTIPLY, 0);
    primitive(S(897), NO_ALIGN, 0);
    primitive(S(1123), NO_BOUNDARY, 0);
    primitive(S(1124), NO_EXPAND, 0);
    primitive(S(388), NON_SCRIPT, 0);
    primitive(S(900), OMIT, 0);
    primitive(S(462), SET_SHAPE, 0);
    primitive(S(761), BREAK_PENALTY, 0);
    primitive(S(948), SET_PREV_GRAF, 0);
    primitive(S(416), RADICAL, 0);
    primitive(S(656), READ_TO_CS, 0);
    primitive(S(1125), RELAX, 256);
    text(frozenrelax) = S(1125);
    eqtb[frozenrelax - activebase] = eqtb[cur_val - activebase];
    primitive(S(970), SET_BOX, 0);
    primitive(S(604), THE, 0);
    primitive(S(463), TOKS_REGISTER, 0);
    primitive(S(403), VADJUST, 0);
    primitive(S(1126), VALIGN, 0);
    primitive(S(415), VCENTER, 0);
    primitive(S(1127), VRULE, 0); /*:265*/
    /*334:*/
    primitive(S(760), PAR_END, 256);
    parloc = cur_val;
    partoken = CS_TOKEN_FLAG + parloc; /*:334*/
    /*376:*/
    primitive(S(1128), INPUT, 0);
    primitive(S(1129), INPUT, 1); /*:376*/
    /*384:*/
    primitive(S(1130), TOP_BOT_MARK, topmarkcode);
    primitive(S(1131), TOP_BOT_MARK, firstmarkcode);
    primitive(S(1132), TOP_BOT_MARK, botmarkcode);
    primitive(S(1133), TOP_BOT_MARK, splitfirstmarkcode);
    primitive(S(1134), TOP_BOT_MARK, splitbotmarkcode); /*:384*/
    /*411:*/
    primitive(S(472), REGISTER, INT_VAL);
    primitive(S(474), REGISTER, DIMEN_VAL);
    primitive(S(460), REGISTER, GLUE_VAL);
    primitive(S(461), REGISTER, MU_VAL); /*:411*/
    /*416:*/
    primitive(S(1135), SET_AUX, H_MODE);
    primitive(S(1136), SET_AUX, V_MODE);
    primitive(S(1137), SET_PAGE_INT, 0);
    primitive(S(1138), SET_PAGE_INT, 1);
    primitive(S(1139), SET_BOX_DIMEN, widthoffset);
    primitive(S(1140), SET_BOX_DIMEN, heightoffset);
    primitive(S(1141), SET_BOX_DIMEN, depthoffset);
    primitive(S(1142), LAST_ITEM, INT_VAL);
    primitive(S(1143), LAST_ITEM, DIMEN_VAL);
    primitive(S(1144), LAST_ITEM, GLUE_VAL);
    primitive(S(1145), LAST_ITEM, INPUT_LINE_NO_CODE);
    primitive(S(1146), LAST_ITEM, BADNESS_CODE); /*:416*/
    /*468:*/
    primitive(S(1147), CONVERT, numbercode);
    primitive(S(1148), CONVERT, romannumeralcode);
    primitive(S(1149), CONVERT, stringcode);
    primitive(S(1150), CONVERT, meaningcode);
    primitive(S(1151), CONVERT, fontnamecode);
    primitive(S(1152), CONVERT, jobnamecode); /*:468*/
    /*487:*/
    primitive(S(658), IF_TEST, IF_CHAR_CODE);
    primitive(S(1153), IF_TEST, IF_CAT_CODE);
    primitive(S(1154), IF_TEST, IF_INT_CODE);
    primitive(S(1155), IF_TEST, IF_DIM_CODE);
    primitive(S(1156), IF_TEST, IF_ODD_CODE);
    primitive(S(1157), IF_TEST, IF_VMODE_CODE);
    primitive(S(1158), IF_TEST, IF_HMODE_CODE);
    primitive(S(1159), IF_TEST, IF_MMODE_CODE);
    primitive(S(1160), IF_TEST, IF_INNER_CODE);
    primitive(S(1161), IF_TEST, IF_VOID_CODE);
    primitive(S(1162), IF_TEST, IF_HBOX_CODE);
    primitive(S(1163), IF_TEST, IF_VBOX_CODE);
    primitive(S(1164), IF_TEST, IF_X_CODE);
    primitive(S(1165), IF_TEST, IF_EOF_CODE);
    primitive(S(1166), IF_TEST, IF_TRUE_CODE);
    primitive(S(1167), IF_TEST, IF_FALSE_CODE);
    primitive(S(1168), IF_TEST, IF_CASE_CODE); /*:487*/
    /*491:*/
    primitive(S(1169), FI_OR_ELSE, ficode);
    text(frozenfi) = S(1169);
    eqtb[frozenfi - activebase] = eqtb[cur_val - activebase];
    primitive(S(664), FI_OR_ELSE, orcode);
    primitive(S(1170), FI_OR_ELSE, elsecode); /*:491*/
    /*553:*/
    primitive(S(1171), SET_FONT, NULL_FONT);
    text(FROZEN_NULL_FONT) = S(1171);
    eqtb[FROZEN_NULL_FONT - activebase] = eqtb[cur_val - activebase]; /*:553*/
    /*780:*/
    primitive(S(1172), TAB_MARK, spancode);
    primitive(S(737), CAR_RET, crcode);
    text(frozencr) = S(737);
    eqtb[frozencr - activebase] = eqtb[cur_val - activebase];
    primitive(S(1173), CAR_RET, crcrcode);
    text(frozenendtemplate) = S(1174);
    text(frozenendv) = S(1174);
    eqtype(frozenendv) = ENDV;
    equiv(frozenendv) = nulllist;
    eqlevel(frozenendv) = levelone;
    eqtb[frozenendtemplate - activebase] = eqtb[frozenendv - activebase];
    eqtype(frozenendtemplate) = END_TEMPLATE; /*:780*/
    /*983:*/
    primitive(S(1175), SET_PAGE_DIMEN, 0);
    primitive(S(1176), SET_PAGE_DIMEN, 1);
    primitive(S(1177), SET_PAGE_DIMEN, 2);
    primitive(S(1178), SET_PAGE_DIMEN, 3);
    primitive(S(1179), SET_PAGE_DIMEN, 4);
    primitive(S(1180), SET_PAGE_DIMEN, 5);
    primitive(S(1181), SET_PAGE_DIMEN, 6);
    primitive(S(1182), SET_PAGE_DIMEN, 7); /*:983*/
    /*1052:*/
    primitive(S(1183), STOP, 0);
    primitive(S(1184), STOP, 1); /*:1052*/
    /*1058:*/
    primitive(S(1185), HSKIP, SKIP_CODE);
    primitive(S(1186), HSKIP, FIL_CODE);
    primitive(S(1187), HSKIP, FILL_CODE);
    primitive(S(1188), HSKIP, SS_CODE);
    primitive(S(1189), HSKIP, FIL_NEG_CODE);
    primitive(S(1190), VSKIP, SKIP_CODE);
    primitive(S(1191), VSKIP, FIL_CODE);
    primitive(S(1192), VSKIP, FILL_CODE);
    primitive(S(1193), VSKIP, SS_CODE);
    primitive(S(1194), VSKIP, FIL_NEG_CODE);
    primitive(S(389), MSKIP, MSKIP_CODE);
    primitive(S(391), KERN, explicit);
    primitive(S(393), MKERN, muglue); /*:1058*/
    /*1071:*/
    primitive(S(1195), HMOVE, 1);
    primitive(S(1196), HMOVE, 0);
    primitive(S(1197), VMOVE, 1);
    primitive(S(1198), VMOVE, 0);
    primitive(S(464), MAKE_BOX, boxcode);
    primitive(S(1199), MAKE_BOX, copycode);
    primitive(S(1200), MAKE_BOX, lastboxcode);
    primitive(S(797), MAKE_BOX, vsplitcode);
    primitive(S(1201), MAKE_BOX, vtopcode);
    primitive(S(799), MAKE_BOX, vtopcode + V_MODE);
    primitive(S(1202), MAKE_BOX, vtopcode + H_MODE);
    primitive(S(1203), LEADER_SHIP, aleaders - 1);
    primitive(S(1204), LEADER_SHIP, aleaders);
    primitive(S(1205), LEADER_SHIP, cleaders);
    primitive(S(1206), LEADER_SHIP, xleaders); /*:1071*/
    /*1088:*/
    primitive(S(1207), START_PAR, 1);
    primitive(S(1208), START_PAR, 0); /*:1088*/
    /*1107:*/
    primitive(S(1209), REMOVE_ITEM, PENALTY_NODE);
    primitive(S(1210), REMOVE_ITEM, KERN_NODE);
    primitive(S(1211), REMOVE_ITEM, GLUE_NODE);
    primitive(S(1212), UN_HBOX, boxcode);
    primitive(S(1213), UN_HBOX, copycode);
    primitive(S(1214), UN_VBOX, boxcode);
    primitive(S(1215), UN_VBOX, copycode); /*:1107*/
    /*1114:*/
    primitive('-', DISCRETIONARY, 1);
    primitive(S(400), DISCRETIONARY, 0); /*:1114*/
    /*1141:*/
    primitive(S(1216), EQ_NO, 0);
    primitive(S(1217), EQ_NO, 1); /*:1141*/
    /*1156:*/
    primitive(S(405), MATH_COMP, ordnoad);
    primitive(S(406), MATH_COMP, opnoad);
    primitive(S(407), MATH_COMP, binnoad);
    primitive(S(408), MATH_COMP, relnoad);
    primitive(S(409), MATH_COMP, opennoad);
    primitive(S(410), MATH_COMP, closenoad);
    primitive(S(411), MATH_COMP, punctnoad);
    primitive(S(412), MATH_COMP, innernoad);
    primitive(S(414), MATH_COMP, undernoad);
    primitive(S(413), MATH_COMP, overnoad);
    primitive(S(1218), LIMIT_SWITCH, NORMAL);
    primitive(S(420), LIMIT_SWITCH, limits);
    primitive(S(421), LIMIT_SWITCH, nolimits); /*:1156*/
    /*1169:*/
    primitive(S(336), MATH_STYLE, displaystyle);
    primitive(S(337), MATH_STYLE, textstyle);
    primitive(S(338), MATH_STYLE, scriptstyle);
    primitive(S(339), MATH_STYLE, scriptscriptstyle); /*:1169*/
    /*1178:*/
    primitive(S(1219), ABOVE, abovecode);
    primitive(S(1220), ABOVE, overcode);
    primitive(S(1221), ABOVE, atopcode);
    primitive(S(1222), ABOVE, delimitedcode);
    primitive(S(1223), ABOVE, delimitedcode + overcode);
    primitive(S(1224), ABOVE, delimitedcode + atopcode); /*:1178*/
    /*1188:*/
    primitive(S(418), LEFT_RIGHT, leftnoad);
    primitive(S(419), LEFT_RIGHT, rightnoad);
    text(frozenright) = S(419);
    eqtb[frozenright - activebase] = eqtb[cur_val - activebase]; /*:1188*/
    /*1208:*/
    primitive(S(959), PREFIX, 1);
    primitive(S(961), PREFIX, 2);
    primitive(S(1225), PREFIX, 4);
    primitive(S(1226), DEF, 0);
    primitive(S(1227), DEF, 1);
    primitive(S(1228), DEF, 2);
    primitive(S(1229), DEF, 3);
    /*:1208*/
    /*1219:*/
    primitive(S(1230), LET, NORMAL);
    primitive(S(1231), LET, NORMAL + 1); /*:1219*/
    /*1222:*/
    primitive(S(1232), SHORTHAND_DEF, chardefcode);
    primitive(S(1233), SHORTHAND_DEF, mathchardefcode);
    primitive(S(1234), SHORTHAND_DEF, countdefcode);
    primitive(S(1235), SHORTHAND_DEF, dimendefcode);
    primitive(S(1236), SHORTHAND_DEF, skipdefcode);
    primitive(S(1237), SHORTHAND_DEF, muskipdefcode);
    primitive(S(1238), SHORTHAND_DEF, toksdefcode); /*:1222*/
    /*1230:*/
    primitive(S(467), DEF_CODE, catcodebase);
    primitive(S(471), DEF_CODE, mathcodebase);
    primitive(S(468), DEF_CODE, lccodebase);
    primitive(S(469), DEF_CODE, uccodebase);
    primitive(S(470), DEF_CODE, sfcodebase);
    primitive(S(473), DEF_CODE, delcodebase);
    primitive(S(266), DEF_FAMILY, mathfontbase);
    primitive(S(267), DEF_FAMILY, mathfontbase + SCRIPT_SIZE);
    primitive(S(268), DEF_FAMILY, mathfontbase + SCRIPT_SCRIPT_SIZE);
    /*:1230*/
    /*1250:*/
    primitive(S(787), HYPH_DATA, 0);
    primitive(S(774), HYPH_DATA, 1); /*:1250*/
    /*1254:*/
    primitive(S(1239), ASSIGN_FONT_INT, 0);
    primitive(S(1240), ASSIGN_FONT_INT, 1); /*:1254*/
    /*1262:*/
    primitive(S(281), SET_INTERACTION, BATCH_MODE);
    primitive(S(282), SET_INTERACTION, NON_STOP_MODE);
    primitive(S(283), SET_INTERACTION, SCROLL_MODE);
    primitive(S(1241), SET_INTERACTION, ERROR_STOP_MODE); /*:1262*/
    /*1272:*/
    primitive(S(1242), IN_STREAM, 1);
    primitive(S(1243), IN_STREAM, 0); /*:1272*/
    /*1277:*/
    primitive(S(1244), MESSAGE, 0);
    primitive(S(1245), MESSAGE, 1); /*:1277*/
    /*1286:*/
    primitive(S(1246), CASE_SHIFT, lccodebase);
    primitive(S(1247), CASE_SHIFT, uccodebase); /*:1286*/
    /*1291:*/
    primitive(S(1248), XRAY, showcode);
    primitive(S(1249), XRAY, showboxcode);
    primitive(S(1250), XRAY, showthecode);
    primitive(S(1251), XRAY, showlists); /*:1291*/
    /*1344:*/
    primitive(S(378), EXTENSION, opennode);
    primitive(S(379), EXTENSION, writenode);
    writeloc = cur_val;
    primitive(S(380), EXTENSION, closenode);
    primitive(S(381), EXTENSION, specialnode);
    primitive(S(1252), EXTENSION, immediatecode);
    primitive(S(382), EXTENSION, setlanguagecode); /*:1344*/
} // #1336: init_prim
#endif // #1336: tt_INIT

// #4: this procedure gets things started properly
Static void initialize(void) {
    // Local variables for initialization
    Integer i;
    Integer k; // index into mem, eqtb, etc.
    HyphPointer z; // runs through the exception dictionary

    /// p5#8: Initialize whatever TEX might access

    /// p11#21 Set initial values of key variables
    {
        /// [#21, 23]
        for (i = 0; i <= 255; i++) {
            xchr[i] = (TextChar)i;
        }
        /// [#24]
        for (i = FIRST_TEXT_CHAR; i <= LAST_TEXT_CHAR; i++) {
            xord[i] = INVALID_CODE;
        }
        for (i = 128; i <= 255; i++) {
            xord[xchr[i]] = i;
        }
        for (i = 0; i <= 126; i++) {
            xord[xchr[i]] = i;
        }
        
        /*74:*/
        interaction = ERROR_STOP_MODE; /*:74*/
        /*77:*/
        deletions_allowed = true;
        set_box_allowed = true;
        errorcount = 0; /*:77*/
        /*80:*/
        help_ptr = 0;
        use_err_help = false; /*:80*/
        /*97:*/
        interrupt = 0;
        OK_to_interrupt = true; /*:97*/

        /// p#95: 166
        #ifdef tt_DEBUG
            was_mem_end = MEM_MIN;
            was_lo_max = MEM_MIN;
            was_hi_min = MEM_MAX;
            panicking = false;
        #endif // #166: tt_DEBUG

        /*215:*/
        nest_ptr = 0;
        max_nest_stack = 0;
        mode = V_MODE;
        head = contribhead;
        tail = contribhead;
        prevdepth = ignoredepth;
        modeline = 0;
        prevgraf = 0;
        shown_mode = 0; /*991:*/
        pagecontents = EMPTY;
        pagetail = pagehead;
        link(pagehead) = 0;
        lastglue = MAX_HALF_WORD;
        lastpenalty = 0;
        lastkern = 0;
        pagedepth = 0;
        pagemaxdepth = 0; /*:991*/
        /*:215*/
        /*254:*/
        for (k = intbase; k <= EQTB_SIZE; k++) {
            xeqlevel[k - intbase] = levelone;
        }
        /*:254*/
        /*257:*/

        next(hashbase) = 0;
        text(hashbase) = 0;
        for (k = hashbase + 1; k < UNDEFINED_CONTROL_SEQUENCE; k++) /*:257*/
            hash[k - hashbase] = hash[0];
        /*272:*/
        saveptr = 0;
        curlevel = levelone;
        curgroup = bottomlevel;
        curboundary = 0;
        maxsavestack = 0; /*:272*/
        /*287:*/
        magset = 0; /*:287*/
        /*383:*/
        topmark = 0;
        firstmark = 0;
        botmark = 0;
        splitfirstmark = 0;
        splitbotmark = 0; /*:383*/
        /*439:*/
        cur_val = 0;
        cur_val_level = INT_VAL;
        radix = 0;
        cur_order = 0; /*:439*/
        /*481:*/
        for (k = 0; k <= 16; k++) /*:481*/
            readopen[k] = closed;
        /*490:*/
        condptr = 0;
        iflimit = NORMAL;
        curif = 0;
        ifline = 0; /*:490*/
        /*521:*/
        memcpy(TEXformatdefault, "TeXformats:plain.fmt", FORMAT_DEFAULT_LENGTH);
        /*:521*/
        /*551:*/
        for (int k = 0; k <= FONT_MAX; k++) {
            fontused[k] = false;
        }
        /*:551*/
        /*556:*/
        nullcharacter.b0 = MIN_QUARTER_WORD;
        nullcharacter.b1 = MIN_QUARTER_WORD;
        nullcharacter.b2 = MIN_QUARTER_WORD;
        nullcharacter.b3 = MIN_QUARTER_WORD; /*:556*/
        /*593:*/
        totalpages = 0;
        maxv = 0;
        maxh = 0;
        maxpush = 0;
        doingleaders = false;
        deadcycles = 0;
        curs = -1; /*:593*/
        dviout_init();
        /*648:*/
        adjusttail = 0;
        lastbadness = 0; /*:648*/
        /*662:*/
        packbeginline = 0; /*:662*/
        /*685:*/
        emptyfield.rh = EMPTY;
        emptyfield.UU.lh = 0;
        nulldelimiter.b0 = 0;
        nulldelimiter.b1 = MIN_QUARTER_WORD;
        nulldelimiter.b2 = 0;
        nulldelimiter.b3 = MIN_QUARTER_WORD; /*:685*/
        /*771:*/
        alignptr = 0;
        curalign = 0;
        curspan = 0;
        curloop = 0;
        curhead = 0;
        curtail = 0; /*:771*/
        /*928:*/
        for (z = 0; z <= HYPH_SIZE; z++) {
            hyphword[z] = 0;
            hyphlist[z] = 0;
        }
        hyphcount = 0; /*:928*/
        /*990:*/
        outputactive = false;
        insertpenalties = 0; /*:990*/
        /*1033:*/
        ligaturepresent = false;
        cancelboundary = false;
        lfthit = false;
        rthit = false;
        insdisc = false; /*:1033*/
        /*1267:*/
        aftertoken = 0; /*:1267*/
        /*1282:*/
        longhelpseen = false; /*:1282*/
        format_ident = 0; // #1300
        /*1343:*/
        for (k = 0; k <= 17; k++) /*:1343*/
            writeopen[k] = false;
    } // end block p11#21

    /// p59#164: Initialize table entries (done by INITEX only)
    #ifdef tt_INIT
        /// 164, 222, 228, 232, 240, 250, 258, 552, 946, 951, 1216, 1301, and 1369.
        for (k = MEM_BOT + 1; k <= lomemstatmax; k++)
            mem[k - MEM_MIN].sc = 0; // all glue dimensions are zeroed

        k = MEM_BOT;
        while (k <= lomemstatmax) {
            // set first words of glue specifications
            gluerefcount(k) = 1;
            stretchorder(k) = NORMAL;
            shrinkorder(k) = NORMAL;
            k += gluespecsize;
        }

        stretch(filglue)    =  UNITY; stretchorder(filglue)    = FIL;
        stretch(fillglue)   =  UNITY; stretchorder(fillglue)   = FILL;
        stretch(ssglue)     =  UNITY; stretchorder(ssglue)     = FIL;
        shrink(ssglue)      =  UNITY; shrinkorder(ssglue)      = FIL;
        stretch(filnegglue) = -UNITY; stretchorder(filnegglue) = FIL;

        // now initialize the dynamic memory
        rover = lomemstatmax + 1;
        link(rover) = emptyflag;

        nodesize(rover) = 1000; // which is a 1000-word available node
        llink(rover) = rover;
        rlink(rover) = rover;
        lo_mem_max = rover + 1000;
        link(lo_mem_max) = 0;
        info(lo_mem_max) = 0;

        for (k = himemstatmin; k <= MEM_TOP; k++) {
            // clear list heads
            mem[k - MEM_MIN].sc = 0;
            type(k) = charnodetype;
        }

        // Initialize the special list heads and constant nodes
        {
            /// #790
            info(omittemplate) = endtemplatetoken; // ink(omit template) = null
            /// #797
            link(endspan) = MAX_QUARTER_WORD + 1;
            info(endspan) = 0;
            /// #820
            type(lastactive) = hyphenated;
            linenumber(lastactive) = MAX_HALF_WORD;
            subtype(lastactive) = 0; // the subtype is never examined by the algorithm
            /// #981
            subtype(pageinshead) = MIN_QUARTER_WORD + 255;
            type(pageinshead) = splitup;
            link(pageinshead) = pageinshead;
            /// #988
            type(pagehead) = GLUE_NODE;
            subtype(pagehead) = NORMAL;
        }

        /// p59#164
        avail = 0;
        mem_end = MEM_TOP;
        hi_mem_min = himemstatmin; // initialize the one-word memory
        var_used = lomemstatmax - MEM_BOT + 1;
        dyn_used = himemstatusage; // initialize statistics

        /// p82#222
        eqtype(UNDEFINED_CONTROL_SEQUENCE) = UNDEFINED_CS;
        equiv(UNDEFINED_CONTROL_SEQUENCE) = 0;
        eqlevel(UNDEFINED_CONTROL_SEQUENCE) = levelzero;
        for (k = activebase; k < UNDEFINED_CONTROL_SEQUENCE; k++)
            eqtb[k - activebase] = eqtb[UNDEFINED_CONTROL_SEQUENCE - activebase];

        /// #228
        equiv(gluebase) = zeroglue;
        eqlevel(gluebase) = levelone;
        eqtype(gluebase) = GLUE_REF;
        for (k = gluebase + 1; k < localbase; k++)
            eqtb[k - activebase] = eqtb[gluebase - activebase];
        gluerefcount(zeroglue) += localbase - gluebase;

        // [#232]
        parshapeptr = 0;
        eqtype(parshapeloc) = SHAPE_REF;
        eqlevel(parshapeloc) = levelone;
        for (k = outputroutineloc; k <= toksbase + 255; k++)
            eqtb[k - activebase] = eqtb[UNDEFINED_CONTROL_SEQUENCE - activebase];
        box(0) = 0;
        eqtype(boxbase) = BOX_REF;
        eqlevel(boxbase) = levelone;
        for (k = boxbase + 1; k <= boxbase + 255; k++)
            eqtb[k - activebase] = eqtb[boxbase - activebase];
        curfont = NULL_FONT;
        eqtype(curfontloc) = DATA;
        eqlevel(curfontloc) = levelone;
        for (k = mathfontbase; k <= mathfontbase + 47; k++)
            eqtb[k - activebase] = eqtb[curfontloc - activebase];
        
        equiv(catcodebase) = 0;
        eqtype(catcodebase) = DATA;
        eqlevel(catcodebase) = levelone;
        for (k = catcodebase + 1; k < intbase; k++) {
            eqtb[k - activebase] = eqtb[catcodebase - activebase];
        }

        // CatCode init
        for (k = 0; k <= 255; k++) {
            catcode(k) = OTHER_CHAR; // 默认值 cat_12
            mathcode(k) = k;
            sfcode(k) = 1000;
        }
        catcode('\\') = ESCAPE;
        // TODO: 为什么忽略了一部分 catcode？
        // ? cat_1 LEFT_BRACE
        // ? cat_2 RIGHT_BRACE
        // ? cat_3 MATH_SHIFT
        // ? cat_4 TAB_MARK
        catcode(CARRIAGE_RETURN) = CAR_RET; // cat_5
        // ? cat_6 MAC_PARAM
        // ? cat_7 SUP_MARK
        // ? cat_8 SUB_MARK
        catcode(NULL_CODE) = IGNORE; // cat_9
        catcode(' ') = SPACER; // cat_10
        // cat_11 见下方
        // cat_12 为默认值
        // ? cat_13
        catcode('%') = COMMENT; // cat_14
        catcode(INVALID_CODE) = INVALID_CHAR; // cat_15
        
        for (k = '0'; k <= '9'; k++) {
            mathcode(k) = k + varcode;
        }
        for (k = 'A'; k <= 'Z'; k++) {
            catcode(k) = LETTER; // cat_11
            catcode(k + 'a' - 'A') = LETTER;
            mathcode(k) = k + varcode + 256;
            mathcode(k + 'a' - 'A') = k + 'a' - 'A' + varcode + 256;
            lccode(k) = k + 'a' - 'A';
            lccode(k + 'a' - 'A') = k + 'a' - 'A';
            uccode(k) = k;
            uccode(k + 'a' - 'A') = k;
            sfcode(k) = 999;
        } // for (k = 'A'; k <= 'Z'; k++)

        // #240
        for (k = intbase; k < delcodebase; k++)
            eqtb[k - activebase].int_ = 0;
        mag = 1000;
        tolerance = 10000;
        hangafter = 1;
        maxdeadcycles = 25;
        ESCAPE_CHAR = '\\';
        end_line_char = CARRIAGE_RETURN;
        for (k = 0; k <= 255; k++)
            delcode(k) = -1;
        delcode('.') = 0; // this null delimiter is used in error recovery

        // #250
        for (k = dimenbase; k <= EQTB_SIZE; k++)
            eqtb[k - activebase].sc = 0;

        // #258
        hash_used = frozencontrolsequence; // nothing is used
        cs_count = 0;
        eqtype(frozendontexpand) = DONT_EXPAND;
        text(frozendontexpand) = S(257);

        // #552
        fonts_init();

        // #946
        for (k = -TRIE_OP_SIZE; k <= TRIE_OP_SIZE; k++)
            trieophash[k + TRIE_OP_SIZE] = 0;
        for (k = 0; k <= 255; k++)
            trieused[k] = MIN_QUARTER_WORD;
        trieopptr = 0;

        // #951
        trie_not_ready = true;
        trieroot = 0;
        triec[0] = 0;
        trieptr = 0;

        // #1216
        text(frozenprotection) = S(258);
        // #1301
        format_ident = S(259); // " (INITEX)"
        // #1369
        text(endwrite) = S(260);
        eqlevel(endwrite) = levelone;
        eqtype(endwrite) = OUTER_CALL;
        equiv(endwrite) = 0;


    #endif // #164: tt_INIT
} // #4: initialize


/* ----------------------------------------------------------------------------
 * main 函数及其辅助函数(从 main 中拆分的函数)
 *
 *  + int main(int argc, char* argv[])
 *      + Integer S14_Check_the_constant_values_for_consistency(void)
 *      + void S55_Initialize_the_output_routines(void)
 *      + Boolean S1337_Get_the_first_line_of_input_and_prepare_to_start(void)
 */

// [p8#14] 检查常量范围是否正确。
// 有误则返回错误代码 bad
// Check the "constant" values for consistency
// xref: [14], 111, 290, 522, 1249, used in 1332@main
static Integer S14_Check_the_constant_values_for_consistency(void) {
    // bad: is some “constant” wrong?
    // xref: [13], 14, 111, 290, 522, 1249, 1332
    Integer bad = 0;

    /// [#14]
    if (    HALF_ERROR_LINE < 30 
        ||  HALF_ERROR_LINE > (ERROR_LINE - 15)
    ) bad = 1;
    if (MAX_PRINT_LINE < 60) bad = 2;
    if ((DVI_BUF_SIZE % 8) != 0) bad = 3;
    if ((MEM_BOT + 1100) > MEM_TOP) bad = 4;
    if (HASH_PRIME > HASH_SIZE) bad = 5;
    if (MAX_IN_OPEN >= 128) bad = 6;
    if (MEM_TOP < (256 + 11)) bad = 7; // we will want null list > 255

    /// [#111]
    #ifdef tt_INIT
        if (MEM_MIN != MEM_BOT || MEM_MAX != MEM_TOP) bad = 10;
    #endif // #111: tt_INIT
    if (MEM_MIN > MEM_BOT || MEM_MAX < MEM_TOP) bad = 10;
    if (MIN_QUARTER_WORD > 0 || MAX_QUARTER_WORD < 127) bad = 11;
    if (MIN_HALF_WORD > 0 || MAX_HALF_WORD < 32767) bad = 12;
    if (    MIN_QUARTER_WORD < MIN_HALF_WORD 
        ||  MAX_QUARTER_WORD > MAX_HALF_WORD
    ) bad = 13;
    if (    MEM_MIN <  MIN_HALF_WORD
        ||  MEM_MAX >= MAX_HALF_WORD 
        || (MEM_BOT - MEM_MIN) > (MAX_HALF_WORD + 1)
    ) bad = 14;
    if (FONT_BASE < MIN_QUARTER_WORD || FONT_MAX > MAX_QUARTER_WORD) bad = 15;
    if (FONT_MAX > FONT_BASE + 256) bad = 16;
    if (SAVE_SIZE > MAX_HALF_WORD || MAX_STRINGS > MAX_HALF_WORD) bad = 17;
    if (BUF_SIZE > MAX_HALF_WORD) bad = 18;
    if ((MAX_QUARTER_WORD - MIN_QUARTER_WORD) < 255) bad = 19;

    /// [#290]
    if ((CS_TOKEN_FLAG + UNDEFINED_CONTROL_SEQUENCE) > MAX_HALF_WORD) bad = 21;
    /// [#522]
    if (FORMAT_DEFAULT_LENGTH > FILE_NAME_SIZE) bad = 31;
    /// [#1249]
    if ((2 * MAX_HALF_WORD) < (MEM_TOP - MEM_MIN)) bad = 41;

    return bad;
} // S14_Check_the_constant_values_for_consistency

// [p24#55]: <Initialize the output routines> 
// [55], 61, 528, 533 Used in section 1332@main.
static void S55_Initialize_the_output_routines(void) {
    // #55
    selector = TERM_ONLY;
    tally = 0;
    term_offset = 0;
    file_offset = 0;

    // #61
    fprintf(stdout, "%s", TEX_BANNER); // wterm(banner);
    if (format_ident == 0) {
        fprintf(stdout, " (no format preloaded)\n");
    } else {
        slow_print(format_ident);
        println();
    }
    fflush(stdout); // update terminal;

    // #528
    job_name = 0;
    name_in_progress = false;
    log_opened = false;

    // #533
    output_file_name = 0;
} // S55_Initialize_the_output_routines

// [p469#1337]: Get the first line of input and prepare to START
// return has_error?
static Boolean S1337_Get_the_first_line_of_input_and_prepare_to_start(void) {
    const Boolean HAS_ERROR = true, NO_ERROR = false;

    // [#331]: Initialize the input routines
    if (init_lexer()==HAS_ERROR) return HAS_ERROR;

    /// [#1337]
    // TODO: check if
    if (    need_to_load_format 
        || (format_ident == 0) 
        || (buffer[LOC] == '&')
        ) {
        if (format_ident != 0) initialize(); // erase preloaded format
        if (!open_fmt_file()) return HAS_ERROR;
        if (!load_fmt_file()) {
            w_close(&fmtfile);
            return HAS_ERROR;
        }
        w_close(&fmtfile);
        while ((LOC < LIMIT) && (buffer[LOC] == ' '))
            LOC++;
    }
    if (end_line_char_inactive) {
        LIMIT--;
    } else {
        buffer[LIMIT] = end_line_char;
    }
    fix_date_and_time(&tex_time, &day, &month, &year);

    /// [#765]: Compute the magic offset
    // _NOT_USE_
    // ??? magic offset ← str start[math spacing] − 9 ∗ ord noad

    /// [#75]: Initialize the print selector based on interaction
    if (interaction == BATCH_MODE) {
        selector = NO_PRINT;
    } else {
        selector = TERM_ONLY;
    }

    /// [#1337]
    if ((LOC < LIMIT) && (catcode(buffer[LOC]) != ESCAPE)) {
        startinput(); // \input assumed
    }

    return NO_ERROR;
} // S1337_Get_the_first_line_of_input_and_prepare_to_start

/// 入口函数
/// [p466#1332]: TeX starts and ends here.
int main(int argc, char* argv[]) {
    // #13: a sacrifice of purity for economy
    // TODO: remove this
    Integer ready_already = 0;
    int bad = 0;
    Boolean has_error = false;

    // 附加的初始化
    PASCAL_MAIN(argc, argv);
    if (setjmp(_JMP_global__end_of_TEX)) goto _LN_main__end_of_TEX;
    if (setjmp(_JMP_global__final_end)) goto _LN_main__final_end;

    /// [#1332]: TeX start here
    history = FATAL_ERROR_STOP; // in case we quit during initialization
    // t_open_out; // 无需打开输出流，直接使用 stdout
    if (ready_already == 314159L) goto _LN_main__start_of_TEX;
    // 常量范围检查
    bad = S14_Check_the_constant_values_for_consistency();
    if (bad > 0) {
        fprintf(
            stdout,
            "Ouch---my internal constants have been clobbered!---case %d\n",
            bad);
        goto _LN_main__final_end;
    }

    initialize(); // set global variables to their starting values
    #ifdef tt_INIT
        if (!get_strings_started()) goto _LN_main__final_end;
        init_prim(); // call primitive for each primitive
        str_set_init_ptrs(); // @str.c
        fix_date_and_time(&tex_time, &day, &month, &year); // @func.c
    #endif // #1332: tt_INIT
    ready_already = 314159L;

_LN_main__start_of_TEX:
    S55_Initialize_the_output_routines();
    has_error = S1337_Get_the_first_line_of_input_and_prepare_to_start();
    if (has_error) goto _LN_main__final_end;
    // #1332
    history = SPOTLESS; // ready to go!
    main_control();     // come to life
    final_cleanup();    // prepare for death

_LN_main__end_of_TEX:
    close_files_and_terminate();

_LN_main__final_end:
    ready_already = 0;
    exit(EXIT_SUCCESS);
} // main

/* End. */