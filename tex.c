#include "tex_header.h"


/*
 * functions
 * 
 */

/*
# Basic printing procedures[13]:
    [57], [58], [59],   60,   [62],
    [63], [64], [65], [262], [263],
    [518], [699], [1355].
    println,  printchar,      print,               printnl,
    printesc, print_the_digs, print_int,
    print_file_name, print_size, print_write_whatsit
*/

// #262: prints a purported control sequence
// [Basic printing procedures]


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
void print_write_whatsit(StrNumber s, Pointer p) {
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


/** @addtogroup S464x486_P174x180
 * @{
 */

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
    Selector old_setting;
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
HalfWord scantoks(Boolean macrodef, Boolean xpand) {
    HalfWord t, s, unbalance, hashbrace;
    Pointer p;

    if (macrodef) {
        scanner_status = DEFINING;
    } else {
        scanner_status = ABSORBING;
    }

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
                    print_err(S(643)); // "You already have nine parameters"
                    // "I'm going to ignore the # sign you just used."
                    help1(S(644));
                    error();
                } else {
                    t++;
                    if (curtok != t) {
                        // "Parameters must be numbered consecutively"
                        print_err(S(645));
                        // "I've inserted the digit you should have used after the #."
                        // "Type `1' to delete what you did use."
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
            print_err(S(566)); // "Missing { inserted"
            align_state++;
            // "Where was the left brace? You said something like `\\def\\a}'"
            // "which I'm going to interpret as `\\def\\a{}'."
            help2(S(648), S(649));
            error();
            goto _Lfound;
        } /*:475*/

    _Ldone:;
    } else {
        scan_left_brace();
    } /*:474*/

    /*477:*/
    unbalance = 1;
    while (true) {   /*:477*/
        if (xpand) { /*478:*/
            while (true) {
                getnext();
                if (curcmd <= MAX_COMMAND) goto _Ldone2;
                if (curcmd != THE) {
                    expand();
                } else {
                    Pointer q = thetoks();
                    if (link(temphead) != 0) {
                        link(p) = link(temphead);
                        p = q;
                    }
                }
            }

        _Ldone2:
            xtoken();
        } else {
            gettoken();
        }

        /*:478*/
        if (curtok < rightbracelimit) {
            if (curcmd < RIGHT_BRACE) {
                unbalance++;
            } else {
                unbalance--;
                if (unbalance == 0) goto _Lfound;
            }
        } else if (curcmd == MAC_PARAM) {
            if (macrodef) { /*479:*/
                s = curtok;
                if (xpand) {
                    get_x_token();
                } else {
                    gettoken();
                }
                if (curcmd != MAC_PARAM) {
                    if (curtok <= ZERO_TOKEN || curtok > t) {
                        // "Illegal parameter number in definition of "
                        print_err(S(650));
                        sprint_cs(warning_index);
                        /*
                         * "You meant to type ## instead of # right?"
                         * "Or maybe a } was forgotten somewhere earlier and things"
                         * "are all screwed up? I'm going to assume that you meant ##."
                         */
                        help3(S(651), S(652), S(653));
                        backerror();
                        curtok = s;
                    } else {
                        curtok = outparamtoken - '0' + curchr;
                    }
                }
            } /*:479*/
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


/// [#482] constructs a token list like that for any macro definition,
///     and makes cur val point to it.
/// Parameter r points to the control sequence 
///     that will receive this token list.
Static void readtoks(long n, HalfWord r) {
    Pointer p; // tail of the token list
    long s;    // saved value of align_state
    /* SmallNumber */ int m; // stream number

    scanner_status = DEFINING;
    warning_index = r;
    defref = get_avail();
    tokenrefcount(defref) = 0;
    p = defref; // the reference count
    STORE_NEW_TOKEN(p, endmatchtoken);

    // OLD: `if ((unsigned long)n > 15)`
    if (n < 0 || n > 15) {
        m = 16;
    } else {
        m = n;
    }

    s = align_state;
    align_state = 1000000L; // disable tab marks, etc.

    do { /// [#483] Input and store tokens from the next line of the file
        beginfilereading();
        NAME = m + 1;
        if (readopen[m] == closed) {
            /// [#484] Input for \read from the terminal.
            if (interaction > NON_STOP_MODE) {
                if (n < 0) {
                    print(S(385)); // ""
                    term_input();
                } else {
                    println();
                    sprint_cs(r);
                    print('=');
                    term_input();
                    n = -1;
                }
            } else {
                // "*** (cannot \read from terminal in nonstop modes)"
                fatalerror(S(654));
            } // if (interaction <=> NON_STOP_MODE)
        } else if (readopen[m] == justopen) {
            /// [#485] Input the first line of read_file[m].
            if (inputln(readfile[m], false)) {
                readopen[m] = NORMAL;
            } else {
                aclose(&readfile[m]);
                readopen[m] = closed;
            }
        } else {
            /// [#486] Input the next line of read_file[m].
            if (!inputln(readfile[m], true)) {
                aclose(&readfile[m]);
                readopen[m] = closed;
                if (align_state != 1000000L) {
                    runaway();
                    print_err(S(655)); // "File ended within "
                    print_esc(S(656)); // "read"
                    help1(S(657));     // "This \\read has unbalanced braces."
                    align_state = 1000000L;
                    error();
                }
            }
        } // if (readopen[m] <=>)

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
            // cur_cmd = cur_chr = 0 will occur at the end of the line
            if (curtok == 0) goto _Ldone;
            if (align_state < 1000000L) { // unmatched ‘}’ aborts the line
                do {
                    gettoken();
                } while (curtok != 0);
                align_state = 1000000L;
                goto _Ldone;
            }
            STORE_NEW_TOKEN(p, curtok);
        } // while (true)

    _Ldone: /*:483*/    
        endfilereading();
    } while (align_state != 1000000L);

    cur_val = defref;
    scanner_status = NORMAL;
    align_state = s;
} /* [#482] readtoks */
/** @}*/ // end group S464x486_P174x180


/** @addtogroup S1340x1379_P472x481
 * [ #1340. Extensions. ]
 * @{
 */

// #1368
Static void specialout(HalfWord p) {
    Selector old_setting;

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
    Selector old_setting;
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
        print_err(S(680)); // "Unbalanced write command"
        // "On this page there's a \\write with fewer real {'s than }'s."
        // "I can't handle that very well; good luck."
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
    if (write_open[j]) {
        selector = j;
    } else {
        if (j == 17 && selector == TERM_AND_LOG) selector = LOG_ONLY;
        printnl(S(385)); // ""
    }
    tokenshow(defref);
    println();
    flush_list(defref);
    selector = old_setting;
}
/*:1370*/

/*1373:*/
void outwhat(HalfWord p) {
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
                    if (write_open[j]) aclose(&write_file[j]);
                    if (subtype(p) == closenode) {
                        write_open[j] = false;
                    } else if (j < 16) {
                        curname = openname(p);
                        curarea = openarea(p);
                        curext = openext(p);
                        if (curext == S(385)) curext = S(669);
                        packfilename(curname, curarea, curext);
                        while (!a_open_out(&write_file[j]))
                            promptfilename(S(683), S(669));
                        write_open[j] = true;
                    }
                }
            }
            break;

        case specialnode: 
            specialout(p); 
            break;

        case languagenode:
            /* blank case */
            break;

        default:
            confusion(S(684)); // "ext4"
            break;
    }
}
/*:1373*/

/** @}*/ // end group S1340x1379_P472x481


/*1030:*/

/** @addtogroup S1029x1054_P383x394
 * @{
 */

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
            mainp = fontglue[cur_font];
            if (mainp == 0) {
                FontIndex mmaink;
                mainp = newspec(zeroglue);
                mmaink = parambase[cur_font] + SPACE_CODE;
            #if 1
                maink = mmaink;
            #endif
                width(mainp) = fontinfo[maink].sc;
                stretch(mainp) = fontinfo[mmaink + 1].sc;
                shrink(mainp) = fontinfo[mmaink + 2].sc;
                fontglue[cur_font] = mainp;
            }
        }
        mainp = newspec(mainp); /*1044:*/
        if (spacefactor >= 2000) width(mainp) += extraspace(cur_font);
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
    print_err(S(827)); // "Missing $ inserted"
    // "I've inserted a begin-math/end-math symbol since I think"
    // "you left one out. Proceed with fingers crossed."
    help2(S(828), S(829));
    inserror();
} // #1047: insertdollarsign

// #1049
Static void youcant(void) {
    print_err(S(602)); // "You can't use `"
    printcmdchr(curcmd, curchr);
    print(S(830)); // "' in "
    print_mode(mode);
} // #1049: youcant

// #1050
Static void reportillegalcase(void) {
    youcant();
    /*
     * (831) "Sorry but I'm not programmed to handle this case;"
     * (832) "I'll just pretend that you didn't ask for it."
     * (833) "If you're in the wrong mode you might be able to"
     * (834) "return to the right one by typing `I}' or `I$' or `I\\par'."
     */
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
/** @}*/ // end group S1029x1054_P383x394

/** @addtogroup S1055x1135_P395x416
 * @{
 */

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
        print_err(S(558)); // "Extra "
        printcmdchr(curcmd, curchr);
        // "Things are pretty mixed up but I think the worst is over."
        help1(S(835));
        error();
        return;
    } /*:1066*/

    backinput();
    p = get_avail();
    link(temphead) = p;
    print_err(S(554)); // "Missing "
    /*1065:*/
    switch (curgroup) { /*:1065*/
        case semisimplegroup:
            info(p) = CS_TOKEN_FLAG + FROZEN_END_GROUP;
            print_esc(S(836)); // "endgroup"
            break;

        case mathshiftgroup:
            info(p) = mathshifttoken + '$';
            print_char('$');
            break;

        case mathleftgroup:
            info(p) = CS_TOKEN_FLAG + FROZEN_RIGHT;
            link(p) = get_avail();
            p = link(p);
            info(p) = othertoken + '.';
            print_esc(S(837)); // "right."
            break;

        default:
            info(p) = rightbracetoken + '}';
            print_char('}');
            break;
    }
    print(S(555)); // " inserted"
    inslist(link(temphead));
    /*
     * (838) "I've inserted something that you may have forgotten."
     * (839) "(See the <inserted text> above.)"
     * (840) "With luck this will get me unwedged. But if you"
     * (841) "really didn't forget anything try typing `2' now; then"
     * (842) "my insertion and my current dilemma will both disappear."
     */
    help5(S(838), S(839), S(840), S(841), S(842));
    error();
} // #1064: offsave

// [#1069]:
Static void extrarightbrace(void) {
    print_err(S(843)); // "Extra } or forgotten "
    switch (curgroup) {
        case semisimplegroup: print_esc(S(836)); break; // "endgroup"
        case mathshiftgroup: print_char('$'); break;
        case mathleftgroup: print_esc(S(419)); break; // "right"
    }
    /*
     * (844) "I've deleted a group-closing symbol because it seems to be"
     * (845) "spurious as in `$x}$'. But perhaps the } is legitimate and"
     * (846) "you forgot something else as in `\\hbox{$x}'. In such cases"
     * (847) "the way to recover is to insert both the forgotten and the"
     * (848) "deleted material e.g. by typing `I$}'."
     */
    help5(S(844), S(845), S(846), S(847), S(848));
    error();
    align_state++;
} // #1069: extrarightbrace

/*1070:*/
void normalparagraph(void) {
    if (looseness   != 0) eqworddefine(INT_BASE   + loosenesscode,  0);
    if (hangindent  != 0) eqworddefine(DIMEN_BASE + hangindentcode, 0);
    if (hangafter   != 1) eqworddefine(INT_BASE   + hangaftercode,  1);
    if (parshapeptr != 0) eqdefine(PAR_SHAPE_LOC, SHAPE_REF, 0);
} /*:1070*/

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
      eqdefine(BOX_BASE - boxflag + boxcontext, BOX_REF, curbox);
    else   /*:1077*/
      geqdefine(BOX_BASE - boxflag + boxcontext - 256, BOX_REF, curbox);
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
  print_err(S(849)); // "Leaders not followed by proper glue"
  // "You should say `\\leaders <box or rule><hskip or vskip>'."
  // "I found the <box or rule> but there's no suitable"
  // "<hskip or vskip> so I'm ignoring these leaders."
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
        if (!scankeyword(S(697))) { // "to"
            print_err(S(856)); // "Missing `to' inserted"
            // "I'm working on `\\vsplit<box number> to <dimen>';"
            // "will look for the <dimen> next."
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
  print_err(S(859)); // "A <box> was supposed to be here"
  // "I was expecting to see \\hbox or \\vbox or \\copy or \\box or"
  // "something like that. So you might find something missing in"
  // "your output. But keep trying; you can fix this later."
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

Static void headforvmode(void) {
    if (mode < 0) {
        if (curcmd != HRULE) {
            offsave();
            return;
        }
        print_err(S(602)); // "You can't use `"
        print_esc(S(863)); // "hrule"
        print(S(864));     // "' here except with leaders"
        // "To put a horizontal rule in an hbox or an alignment"
        // "you should use \\leaders or \\hrulefill (see The TeXbook)."
        help2(S(865), S(866));
        error();
        return;
    }

    backinput();
    curtok = partoken;
    backinput();
    token_type = INSERTED;
} // headforvmode

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
      print_err(S(867)); // "You can't "
      print_esc(S(374)); // "insert"
      print_int(255);
      // "I'm changing to \\insert0; box 255 is special."
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
  p = get_node(smallnodesize);
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
        print_err(S(872)); // "Incompatible list can't be unboxed"
        // "Sorry Pandora. (You sneaky devil.)"
        // "I refuse to unbox an \\hbox in vertical mode or vice versa."
        // "And I can't open any boxes in math mode."
        help3(S(873), S(874), S(875));
        error();
        return;
    }

    if (c == copycode)
        link(tail) = copynodelist(listptr(p));
    else {
        link(tail) = listptr(p);
        box(cur_val) = 0;
        free_node(p, boxnodesize);
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
    c = get_hyphenchar(cur_font);
    if (c >= 0) {
      if (c < 256)
	prebreak(tail) = newcharacter(cur_font, c);
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
                        print_err(S(876)); // "Improper discretionary list"
                        // "Discretionary lists must contain only boxes and kerns."
                        help1(S(877)); 
                        error();
                        begindiagnostic();
                        // "The following discretionary sublist has been deleted:"
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
                print_err(S(879)); // "Illegal math "
                print_esc(S(400)); // "discretionary"
                // "Sorry: The third part of a discretionary break must be"
                // "empty in math formulas. I had to delete your third part."
                help2(S(880), S(881));
                flush_node_list(p);
                n = 0;
                error();
            } else
                link(tail) = p;
            if (n <= MAX_QUARTER_WORD)
                replacecount(tail) = n;
            else {
                print_err(S(882)); // "Discretionary list is too long"
                // "Wow---I never thought anybody would tweak me here."
                // "You can't seriously need such a huge discretionary list?"
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
  f = cur_font;
  p = newcharacter(f, cur_val);
  if (p == 0)
    return;
  x = xheight(f);
  s = slant(f) / 65536.0;
  a = charwidth(f, charinfo(f, character(p)));
  doassignments();   /*1124:*/
  q = 0;
  f = cur_font;
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

/// [#1127]
Static void alignerror(void) {
    if (labs(align_state) > 2) {
        // [#1128] Express consternation over the fact 
        // that no alignment is in progress
        print_err(S(885)); // "Misplaced "
        printcmdchr(curcmd, curchr);
        if (curtok == (tabtoken + '&')) {
            /*
             * (886) "I can't figure out why you would want to use a tab mark"
             * (887) "here. If you just want an ampersand the remedy is"
             * (888) "simple: Just type `I\\&' now. But if some right brace"
             * (889) "up above has ended a previous alignment prematurely"
             * (890) "you're probably due for more error messages and you"
             * (891) "might try typing `S' now just to see what is salvageable."
             */
            help6(S(886), S(887), S(888), S(889), S(890), S(891));
        } else {
            /*
             * (886) "I can't figure out why you would want to use a tab mark"
             * (892) "or \\cr or \\span just now. If something like a right brace"
             * (889) "up above has ended a previous alignment prematurely" 
             * (890) "you're probably due for more error messages and you" 
             * (891) "might try typing `S' now just to see what is salvageable."
             */
            help5(S(886), S(892), S(889), S(890), S(891));
        } // if ( curtok <> (tabtoken + '&') )
        error();
    } else {
        backinput();
        if (align_state < 0) {
            print_err(S(566)); // "Missing { inserted"
            align_state++;
            curtok = leftbracetoken + '{';
        } else {
            print_err(S(893)); // "Missing } inserted"
            align_state--;
            curtok = rightbracetoken + '}';
        } // if (align_state <> 0)
        /*
         * "I've put in what seems to be necessary to fix"
         * "the current column of the current alignment."
         * "Try to go on since this might almost work."
         */
        help3(S(894), S(895), S(896));
        inserror();
    } // if (labs(align_state) <> 2)
} // [#1127] alignerror

/*1129:*/
Static void noalignerror(void) {
    print_err(S(885)); // "Misplaced "
    print_esc(S(897)); // "noalign"
    // "I expect to see \\noalign only after the \\cr of"
    // "an alignment. Proceed and I'll ignore this case."
    help2(S(898), S(899));
    error();
}

Static void omiterror(void) {
    print_err(S(885)); // "Misplaced "
    print_esc(S(900)); // "omit"
    // "I expect to see \\omit only after tab marks or the \\cr of"
    // "an alignment. Proceed and I'll ignore this case."
    help2(S(901), S(899));
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
Static void cserror(void) {
    print_err(S(558)); // "Extra "
    print_esc(S(263)); // "endcsname"
    help1(S(902));     // "I'm ignoring this since I wasn't doing a \\csname."
    error();
} /*:1135*/
/** @}*/ // end group S1055x1135_P395x416


/** @addtogroup S1136x1207_P417x434
 * @{
 */

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
      v = shiftamount(justbox) + quad(cur_font) * 2;
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
    eqworddefine(INT_BASE + curfamcode, -1);
    eqworddefine(DIMEN_BASE + predisplaysizecode, w);
    eqworddefine(DIMEN_BASE + displaywidthcode, l);
    eqworddefine(DIMEN_BASE + displayindentcode, s);
    if (everydisplay != 0)
      begintokenlist(everydisplay, EVERY_DISPLAY_TEXT);
    if (nest_ptr == 1)
      buildpage();
    return;
  }
  /*:1145*/
  backinput();   /*1139:*/
  pushmath(mathshiftgroup);
  eqworddefine(INT_BASE + curfamcode, -1);
  if (everymath != 0)   /*:1139*/
    begintokenlist(everymath, EVERY_MATH_TEXT);
}
/*:1138*/

/*1142:*/
Static void starteqno(void) {
    saved(0) = curchr;
    saveptr++; /*1139:*/
    pushmath(mathshiftgroup);
    eqworddefine(INT_BASE + curfamcode, -1);
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
            c = math_code(curchr);
            if (c == 32768L) { /*1152:*/
                curcs = curchr + ACTIVE_BASE;
                curcmd = eq_type(curcs);
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
    if (c >= VAR_CODE && faminrange) {
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
    curcs = curchr + ACTIVE_BASE;
    curcmd = eq_type(curcs);
    curchr = equiv(curcs);
    xtoken();
    backinput();
    return;
  }  /*:1152*/
  p = newnoad();
  mathtype(nucleus(p)) = mathchar;
  character(nucleus(p)) = c & 255;
  fam(nucleus(p)) = (c / 256) & 15;
  if (c >= VAR_CODE) {
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
    print_err(S(903)); // "Limit controls must follow a math operator"
    // "I'm ignoring this misplaced \\limits or \\nolimits command."
    help1(S(904));
    error();
}
/*:1159*/

/*1160:*/
Static void scandelimiter(HalfWord p, Boolean r) {
    if (r)
        scan_twenty_seven_bit_int();
    else {
        skip_spaces_or_relax();
        switch (curcmd) {

            case LETTER:
            case OTHER_CHAR: cur_val = del_code(curchr); break;

            case DELIM_NUM: scan_twenty_seven_bit_int(); break;

            default: cur_val = -1; break;
        }
    }
    if (cur_val < 0) {     /*1161:*/
        print_err(S(905)); // "Missing delimiter (. inserted)"
        /*
         * "I was expecting to see something like `(' or `\\{' or"
         * "`\\}' here. If you typed e.g. `{' instead of `\\{' you"
         * "should probably delete the `{' by typing `1' now so that"
         * "braces don't get unbalanced. Otherwise just proceed."
         * "Acceptable delimiters are characters whose \\delcode is"
         * "nonnegative or you can use `\\delimiter <delimiter code>'."
         */
        help6(S(906), S(907), S(908), S(909), S(910), S(911));
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
  tailappend(get_node(radicalnoadsize));
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
Static void mathac(void) {
    if (curcmd == ACCENT) { /*1166:*/
        print_err(S(912)); // "Please use "
        print_esc(S(913)); // "mathaccent"
        print(S(914));     // " for accents in math mode"
        // "I'm changing \\accent to \\mathaccent here; wish me luck."
        // "(Accents are not the same in formulas as they are in text.)"
        help2(S(915), S(916));
        error();
    } /*:1166*/

    tailappend(get_node(accentnoadsize));
    type(tail) = accentnoad;
    subtype(tail) = NORMAL;
    mem[nucleus(tail) - MEM_MIN].hh = emptyfield;
    mem[subscr(tail) - MEM_MIN].hh = emptyfield;
    mem[supscr(tail) - MEM_MIN].hh = emptyfield;
    mathtype(accentchr(tail)) = mathchar;
    scan_fifteen_bit_int();
    character(accentchr(tail)) = cur_val & 255;
    if (cur_val >= VAR_CODE && faminrange) {
        fam(accentchr(tail)) = curfam;
    } else {
        fam(accentchr(tail)) = (cur_val / 256) & 15;
    }
    scanmath(nucleus(tail));
} /*:1165*/

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
Static HalfWord finmlist(HalfWord p) {
    Pointer q;

    if (incompleatnoad != 0) { /*1185:*/
        mathtype(denominator(incompleatnoad)) = submlist;
        info(denominator(incompleatnoad)) = link(head);
        if (p == 0) {
            q = incompleatnoad;
        } else {
            q = info(numerator(incompleatnoad));
            // "right"
            if (type(q) != leftnoad) confusion(S(419));
            info(numerator(incompleatnoad)) = link(q);
            link(q) = incompleatnoad;
            link(incompleatnoad) = p;
        }
    } else {/*:1185*/
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
Static void subsup(void) {
    SmallNumber t;
    Pointer p;

    t = EMPTY;
    p = 0;
    if (tail != head) {
        if (scriptsallowed(tail)) {
            if (curcmd == SUP_MARK) {
                p = supscr(tail);
            } else {
                p = subscr(tail);
            }
            t = mathtype(p);
        }
    }
    if (p == 0 || t != EMPTY) { /*1177:*/
        tailappend(newnoad());
        if (curcmd == SUP_MARK) {
            p = supscr(tail);
        } else {
            p = subscr(tail);
        }
        if (t != EMPTY) {
            if (curcmd == SUP_MARK) {
                print_err(S(917)); // "Double superscript"
                // "I treat `x^1^2' essentially like `x^1{}^2'."
                help1(S(918));
            } else {
                print_err(S(919)); // "Double subscript"
                help1(S(920)); // "I treat `x_1_2' essentially like `x_1{}_2'."
            }
            error();
        }
    }
    /*:1177*/
    scanmath(p);
}
/*:1176*/

/*1181:*/
Static void mathfraction(void) {
    SmallNumber c;

    c = curchr;
    if (incompleatnoad != 0) { /*1183:*/
        if (c >= delimitedcode) {
            scandelimiter(garbage, false);
            scandelimiter(garbage, false);
        }
        if (c % delimitedcode == abovecode) {
            SCAN_NORMAL_DIMEN();
        }
        print_err(S(921)); // "Ambiguous; you need another { and }"
        // "I'm ignoring this fraction specification since I don't"
        // "know whether a construction like `x \\over y \\over z'"
        // "means `{x \\over y} \\over z' or `x \\over {y \\over z}'."
        help3(S(922), S(923), S(924));
        error();
        return;
    } /*:1183*/

    incompleatnoad = get_node(fractionnoadsize);
    type(incompleatnoad) = fractionnoad;
    subtype(incompleatnoad) = NORMAL;
    mathtype(numerator(incompleatnoad)) = submlist;
    info(numerator(incompleatnoad)) = link(head);
    mem[denominator(incompleatnoad) - MEM_MIN].hh = emptyfield;
    mem[leftdelimiter(incompleatnoad) - MEM_MIN].qqqq = nulldelimiter;
    mem[rightdelimiter(incompleatnoad) - MEM_MIN].qqqq = nulldelimiter;
    link(head) = 0;
    tail = head; /*1182:*/
    if (c >= delimitedcode) {
        scandelimiter(leftdelimiter(incompleatnoad), false);
        scandelimiter(rightdelimiter(incompleatnoad), false);
    }

    switch (c % delimitedcode) { /*:1182*/
        case abovecode:
            SCAN_NORMAL_DIMEN();
            thickness(incompleatnoad) = cur_val;
            break;

        case overcode: thickness(incompleatnoad) = defaultcode; break;

        case atopcode: thickness(incompleatnoad) = 0; break;
    }
}
/*:1181*/

/*1191:*/
Static void mathleftright(void) {
    SmallNumber t;
    Pointer p;

    t = curchr;
    /*1192:*/
    if (t == rightnoad && curgroup != mathleftgroup) {
        if (curgroup != mathshiftgroup) {
            offsave();
            return;
        }
        scandelimiter(garbage, false);
        print_err(S(558)); // "Extra "
        print_esc(S(419)); // "right"
        help1(S(925)); // "I'm ignoring a \\right that had no matching \\left."
        error();
        return;
    } /*:1192*/

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
Static void aftermath(void) {
    Boolean l, danger;
    long m;
    Pointer p, a;
    /*1198:*/
    Pointer b, r, t; /*:1198*/
    Scaled w, z, e, q, d, s;
    SmallNumber g1, g2;

    danger = false; /*1195:*/
    if ((fontparams[fam_fnt(TEXT_SIZE + 2)] < totalmathsyparams) |
        (fontparams[fam_fnt(SCRIPT_SIZE + 2)] < totalmathsyparams) |
        (fontparams[fam_fnt(SCRIPT_SCRIPT_SIZE + 2)] < totalmathsyparams)) {
        print_err(S(926)); // "Math formula deleted: Insufficient symbol fonts"
        // "Sorry but I can't typeset math unless \\textfont 2"
        // "and \\scriptfont 2 and \\scriptscriptfont 2 have all"
        // "the \\fontdimen values needed in math symbol fonts."
        help3(S(927), S(928), S(929));
        error();
        flushmath();
        danger = true;
    } else if ((fontparams[fam_fnt(TEXT_SIZE + 3)] < totalmathexparams) |
               (fontparams[fam_fnt(SCRIPT_SIZE + 3)] < totalmathexparams) |
               (fontparams[fam_fnt(SCRIPT_SCRIPT_SIZE + 3)] <
                totalmathexparams)) {
        print_err(S(930)); // "Math formula deleted: Insufficient extension fonts"
        // "Sorry but I can't typeset math unless \\textfont 3"
        // "and \\scriptfont 3 and \\scriptscriptfont 3 have all"
        // "the \\fontdimen values needed in math extension fonts."
        help3(S(931), S(932), S(933));
        error();
        flushmath();
        danger = true;
    }

    m = mode;
    l = false;
    p = finmlist(0);
    if (mode == -m) { /*1197:*/
        get_x_token();
        if (curcmd != MATH_SHIFT) { /*:1197*/
            print_err(S(745)); // "Display math should end with $$"
            // "Bad space factor"
            // "I allow only values in the range 1..32767 here."
            help2(S(746), S(747));
            backerror();
        }
        curmlist = p;
        curstyle = textstyle;
        mlistpenalties = false;
        mlisttohlist();
        a = hpack(link(temphead), 0, additional);
        unsave();
        saveptr--;
        if (saved(0) == 1) l = true;
        danger = false; /*1195:*/

        if ((fontparams[fam_fnt(TEXT_SIZE + 2)] < totalmathsyparams) |
            (fontparams[fam_fnt(SCRIPT_SIZE + 2)] < totalmathsyparams) |
            (fontparams[fam_fnt(SCRIPT_SCRIPT_SIZE + 2)] < totalmathsyparams)) {
            print_err(S(926)); // "Math formula deleted: Insufficient symbol fonts"
            // "Sorry but I can't typeset math unless \\textfont 2"
            // "and \\scriptfont 2 and \\scriptscriptfont 2 have all"
            // "the \\fontdimen values needed in math symbol fonts."
            help3(S(927), S(928), S(929));
            error();
            flushmath();
            danger = true;
        } else if ((fontparams[fam_fnt(TEXT_SIZE + 3)] < totalmathexparams) |
                   (fontparams[fam_fnt(SCRIPT_SIZE + 3)] < totalmathexparams) |
                   (fontparams[fam_fnt(SCRIPT_SCRIPT_SIZE + 3)] <
                    totalmathexparams)) {
            print_err(S(930)); // "Math formula deleted: Insufficient extension fonts"
            // "Sorry but I can't typeset math unless \\textfont 3"
            // "and \\scriptfont 3 and \\scriptscriptfont 3 have all"
            // "the \\fontdimen values needed in math extension fonts."
            help3(S(931), S(932), S(933));
            error();
            flushmath();
            danger = true;
        }
        m = mode;
        p = finmlist(0);
    } else {
        a = 0;
    }

    if (m < 0) { /*1196:*/
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
    if (a == 0) { /*1197:*/
        get_x_token();
        if (curcmd != MATH_SHIFT) {
            print_err(S(745)); // "Display math should end with $$"
            // "The `$' that I just saw supposedly matches a previous `$$'."
            // "So I shall assume that you typed `$$' both times."
            help2(S(746), S(747));
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

    if (w + q > z) { /*1201:*/
        if (e != 0 &&
            (w - totalshrink[0] + q <= z || totalshrink[FIL - NORMAL] != 0 ||
             totalshrink[FILL - NORMAL] != 0 ||
             totalshrink[FILLL - NORMAL] != 0)) {
            free_node(b, boxnodesize);
            b = hpack(p, z - q, exactly);
        } else {
            e = 0;
            if (w > z) {
                free_node(b, boxnodesize);
                b = hpack(p, z, exactly);
            }
        }
        w = width(b);
    }

    /*:1201*/
    /*1202:*/
    d = half(z - w);
    if (e > 0 && d < e * 2) { /*:1202*/
        d = half(z - w - e);
        if (p != 0) {
            if (!ischarnode(p)) {
                if (type(p) == GLUE_NODE) d = 0;
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

    if (l && e == 0) { /*1204:*/
        shiftamount(a) = s;
        appendtovlist(a);
        tailappend(newpenalty(INF_PENALTY));
    } else {
        tailappend(newparamglue(g1)); /*:1203*/
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

    shiftamount(b) = s + d; /*:1204*/
    appendtovlist(b);       /*1205:*/
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
    if (g2 > 0) {                     /*:1199*/
        tailappend(newparamglue(g2)); /*:1205*/
    }
    resumeafterdisplay();

    /*:1195*/
    /*:1195*/
}
/*:1194*/

/*1200:*/
void resumeafterdisplay(void) {
    // "display"
    if (curgroup != mathshiftgroup) confusion(S(934));

    unsave();
    prevgraf += 3;
    pushnest();
    mode = H_MODE;
    spacefactor = 1000;
    setcurlang();
    clang = curlang;
    prevgraf =
        (normmin(lefthyphenmin) * 64 + normmin(righthyphenmin)) * 65536L +
        curlang;
    /*443:*/
    get_x_token();
    if (curcmd != SPACER) backinput();
    /*:443*/
    if (nest_ptr == 1) buildpage();
}
/*:1200*/
/** @}*/ // end group S1136x1207_P417x434


/** @addtogroup S1208x1298_P435x454
 * @{
 */

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
    if (curcs != 0 && curcs <= FROZEN_CONTROL_SEQUENCE) return;

    print_err(S(935)); // "Missing control sequence inserted"
    /*
     * (936) "Please don't say `\\def cs{...}' say `\\def\\cs{...}'."
     * (937) "I've inserted an inaccessible control sequence so that your"
     * (938) "definition will be completed without mixing me up too badly."
     * (939) "You can recover graciously from this error if you're"
     * (940) "careful; see exercise 27.2 in The TeXbook."
     */
    help5(S(936), S(937), S(938), S(939), S(940));
    if (curcs == 0) backinput();
    curtok = CS_TOKEN_FLAG + FROZEN_PROTECTION;
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

/// [#1236]
Static void doregistercommand(SmallNumber a) {
    Pointer l = 0 /* XXXX */, q, r, s; // for list manipulation
    char p; // type of register involved

    q = curcmd;
    /// [#1237] Compute the register location l and its type p;
    /// but return if invalid.
    if (q != REGISTER) {
        get_x_token();
        if (curcmd >= ASSIGN_INT && curcmd <= ASSIGN_MU_GLUE) {
            l = curchr;
            p = curcmd - ASSIGN_INT;
            goto _Lfound;
        }

        if (curcmd != REGISTER) {
            print_err(S(602)); // "You can't use `"
            printcmdchr(curcmd, curchr);
            print(S(603)); // "' after "
            printcmdchr(q, 0);
            // "I'm forgetting what you said and not changing anything."
            help1(S(941));
            error();
            goto _Lexit;
        }
    } // if (q != REGISTER)

    p = curchr;
    scan_eight_bit_int();
    switch (p) {
        case INT_VAL: l = cur_val + COUNT_BASE; break;
        case DIMEN_VAL: l = cur_val + SCALED_BASE; break;
        case GLUE_VAL: l = cur_val + SKIP_BASE; break;
        case MU_VAL: l = cur_val + MU_SKIP_BASE; break;
    } // switch (p)

_Lfound: /*:1237*/
    if (q == REGISTER) {
        scan_optional_equals();
    } else {
        scankeyword(S(942)); // "by"
    }

    arith_error = false;
    if (q < MULTIPLY) {
        /// [#1238] Compute result of register or advance, put it in cur_val.
        if (p < GLUE_VAL) {
            if (p == INT_VAL) {
                scan_int();
            } else {
                SCAN_NORMAL_DIMEN();
            }
            if (q == ADVANCE) cur_val += eqtb[l - ACTIVE_BASE].int_;
        } else {
            scan_glue(p);
            if (q == ADVANCE) {
                /// [#1239] Compute the sum of two glue specs.
                q = newspec(cur_val);
                r = equiv(l);
                delete_glue_ref(cur_val);
                width(q) += width(r);

                if (stretch(q) == 0) stretchorder(q) = NORMAL;
                if (stretchorder(q) == stretchorder(r)) {
                    stretch(q) += stretch(r);
                } else if ((stretchorder(q) < stretchorder(r)) 
                            & (stretch(r) != 0)) {
                    stretch(q) = stretch(r);
                    stretchorder(q) = stretchorder(r);
                }

                if (shrink(q) == 0) shrinkorder(q) = NORMAL;
                if (shrinkorder(q) == shrinkorder(r)) {
                    shrink(q) += shrink(r);
                } else if ((shrinkorder(q) < shrinkorder(r)) 
                            & (shrink(r) != 0)) {
                    shrink(q) = shrink(r);
                    shrinkorder(q) = shrinkorder(r);
                }
                cur_val = q;
            } // if (q == ADVANCE)
        }     // [#1238] if (p <=> GLUE_VAL)
    } else {
        /// [#1240] Compute result of multiply or divide, put it in cur_val.
        scan_int();
        if (p < GLUE_VAL) {
            if (q == MULTIPLY) {
                if (p == INT_VAL) {
                    cur_val = mult_integers(eqtb[l-ACTIVE_BASE].int_, cur_val);
                } else {
                    cur_val = nx_plus_y(eqtb[l-ACTIVE_BASE].int_, cur_val, 0);
                }
            } else {
                cur_val = x_over_n(eqtb[l - ACTIVE_BASE].int_, cur_val);
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
        } // if (p <=> GLUE_VAL)
    } // if (q <=> MULTIPLY)

    if (arith_error) {
        print_err(S(943)); // "Arithmetic overflow"
        // "I can't carry out that multiplication or division"
        // "since the result is out of range."
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
    print_err(S(946)); // "Bad space factor"
    // "I allow only values in the range 1..32767 here."
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
    print_err(S(773)); // "Bad "
    print_esc(S(948)); // "prevgraf"
    help1(S(949)); // "I allow only nonnegative values here."
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
    Selector old_setting;
    /* XXXX  StrNumber flushablestring; */

    if (job_name == 0) openlogfile();
    getrtoken();
    u = curcs;
    if (u >= HASH_BASE) {
        t = get_text(u);
    } else if (u >= ACTIVE_BASE) {
        if (u == NULL_CS)
            t = S(950); // "FONT"
        else
            t = u - ACTIVE_BASE;
    } else {
        old_setting = selector;
        selector = NEW_STRING;
        print(S(950)); // "FONT"
        print(u - ACTIVE_BASE);
        selector = old_setting;
        str_room(1);
        t = makestring();
    }
    define(u, SET_FONT, NULL_FONT);
    scan_optional_equals();
    scanfilename(); /*1258:*/

    name_in_progress = true;
    if (scankeyword(S(951))) { // "at"
        /*1259:*/
        SCAN_NORMAL_DIMEN();
        s = cur_val;
        if (s <= 0 || s >= 134217728L) {
            print_err(S(952)); // "Improper `at' size ("
            print_scaled(s);
            print(S(953)); // "pt) replaced by 10pt"
            // "I can only handle fonts at positive sizes that are"
            // "less than 2048pt so I've changed what you said to 10pt."
            help2(S(954), S(955));
            error();
            s = UNITY * 10;
        }
        /*:1259*/
    } else if (scankeyword(S(956))) { // "scaled"
        scan_int();
        s = -cur_val;
        if (cur_val <= 0 || cur_val > 32768L) {
            // "Illegal magnification has been changed to 1000"
            print_err(S(486));
            // "The magnification ratio must be between 1 and 32768."
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
    eqtb[FONT_ID_BASE + f - ACTIVE_BASE] = eqtb[u - ACTIVE_BASE];
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


/// [#1211] If the user says, e.g., `\\global\\global`, 
/// the redundancy is silently accepted.
Static void prefixedcommand(void) {
    SmallNumber a; ///< accumulated prefix codes so far.
    InternalFontNumber f; ///< identifies a font.
    HalfWord j; ///< index into a \\parshape specification.
    FontIndex k; ///< index into font_info.
    Pointer p, q; ///< for temporary short-term use.
    long n; ///< ditto.
    Boolean e; ///< should a definition be expanded? or was \\let not done?

    a = 0;
    while (curcmd == PREFIX) {
        // if (not_odd(a // curchr))
        if (!((a / curchr) & 1)) a += curchr;
        skip_spaces_or_relax();
        if (curcmd > MAX_NON_PREFIXED_COMMAND) continue;

        /// [#1212] Discard erroneous prefixes and return.
        print_err(S(957)); // "You can't use a prefix with `"
        printcmdchr(curcmd, curchr);
        print_char('\'');
        // "I'll pretend you didn't say \\long or \\outer or \\global."
        help1(S(958));
        backerror();
        goto _Lexit;
        /// end [#1212]
    } // while (curcmd == PREFIX)

    /// [#1213]  Discard the prefixes \\long and \\outer if they are irrelevant.
    if (curcmd != DEF && (a % 4) != 0) {
        print_err(S(602)); // "You can't use `"
        print_esc(S(959)); // "long"
        print(S(960));     // "' or `"
        print_esc(S(961)); // "outer"
        print(S(962));     // "' with `"
        printcmdchr(curcmd, curchr);
        print_char('\'');
        // "I'll pretend you didn't say \\long or \\outer here."
        help1(S(963));
        error();
    } // end [#1213]

    /// [#1214] Adjust for the setting of \\globaldefs.
    if (globaldefs != 0) {
        if (globaldefs < 0) {
            if (a >= 4) {
                a -= 4;
            }
        } else {
            if (!(a >= 4)) {
                a += 4;
            }
        }
    } /* end [#1214] if (globaldefs != 0) */

    switch (curcmd) {
        /// [#1217] Assignments.
        case SET_FONT:
            define(CUR_FONT_LOC, DATA, curchr);
            break;

        /// [#1218]
        case DEF:
            if ((curchr & 1) && !(a >= 4) && globaldefs >= 0) {
                a += 4;
            }
            e = (curchr >= 2);
            getrtoken();
            p = curcs;
            q = scantoks(true, e);
            define(p, CALL + (a & 3), defref);
            break;

        /// [#1221]
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
                // look ahead, then back up
                gettoken();
                q = curtok;
                gettoken();
                backinput();
                curtok = q;
                backinput();
            } // note that back input doesn’t affect `cur_cmd`, `cur_chr`
            if (curcmd >= CALL) {
                addtokenref(curchr);
            }
            define(p, curcmd, curchr);
            break;

        /// [#1224]
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
                            define(p, ASSIGN_INT, COUNT_BASE + cur_val);
                            break;

                        case dimendefcode:
                            define(p, ASSIGN_DIMEN, SCALED_BASE + cur_val);
                            break;

                        case skipdefcode:
                            define(p, ASSIGN_GLUE, SKIP_BASE + cur_val);
                            break;

                        case muskipdefcode:
                            define(p, ASSIGN_MU_GLUE, MU_SKIP_BASE + cur_val);
                            break;

                        case toksdefcode:
                            define(p, ASSIGN_TOKS, TOKS_BASE + cur_val);
                            break;

                        /* there are no other cases */
                    } // switch (n)
                    break;
            } // switch (n)
            break;

        /// [#1225]
        case READ_TO_CS:
            scan_int();
            n = cur_val;
            if (!scankeyword(S(697))) { // "to"
                print_err(S(856)); // "Missing `to' inserted"
                // "You should have said `\\read<number> to \\cs'."
                // "I'm going to look for the \\cs now."
                help2(S(964), S(965));
                error();
            }
            getrtoken();
            p = curcs;
            readtoks(n, p);
            define(p, CALL, cur_val);
            break;

        /// [#1226]
        case TOKS_REGISTER:
        case ASSIGN_TOKS:
            q = curcs;
            if (curcmd == TOKS_REGISTER) {
                scan_eight_bit_int();
                p = TOKS_BASE + cur_val;
            } else {
                // p = `every_par_loc` or `output_routine_loc` or ...
                p = curchr; 
            }
            scan_optional_equals();
            skip_spaces_or_relax();
            if (curcmd != LEFT_BRACE) { 
                /// [#1227] If the right-hand side is a token parameter 
                /// or token register, finish the assignment and goto done
                int cur_chr = curchr;
                if (curcmd == TOKS_REGISTER) {
                    scan_eight_bit_int();
                    curcmd = ASSIGN_TOKS;
                    cur_chr = TOKS_BASE + cur_val;
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
            } /* [#1227] if (curcmd != LEFT_BRACE) */

            backinput();
            curcs = q;
            q = scantoks(false, false);
            if (link(defref) == 0) { // empty list: revert to the default
                define(p, UNDEFINED_CS, 0);
                FREE_AVAIL(defref);
            } else {
                if (p == OUTPUT_ROUTINE_LOC) {
                    // enclose in curlies
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

        /// [#1228]
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
        case ASSIGN_MU_GLUE:
            p = curchr;
            n = curcmd;
            scan_optional_equals();
            if (n == ASSIGN_MU_GLUE) {
                scan_glue(MU_VAL);
            } else {
                scan_glue(GLUE_VAL);
            }
            trapzeroglue();
            define(p, GLUE_REF, cur_val);
            break;

        /// [#1232]
        case DEF_CODE:
            /// [#1233] Let n be the largest legal code value, based on cur_chr.
            if (curchr == CAT_CODE_BASE) {
                n = MAX_CHAR_CODE;
            } else if (curchr == MATH_CODE_BASE) {
                n = 32768L;
            } else if (curchr == SF_CODE_BASE) {
                n = 32767;
            } else if (curchr == DEL_CODE_BASE) {
                n = 16777215L;
            } else {
                n = 255; 
            } // end [#1233]

            p = curchr;
            scan_char_num();
            p += cur_val;
            scan_optional_equals();
            scan_int();
            if ((cur_val < 0 && p < DEL_CODE_BASE) || cur_val > n) {
                print_err(S(966)); // "Invalid code ("
                print_int(cur_val);
                if (p < DEL_CODE_BASE) {
                    print(S(967)); // ") should be in the range 0.."
                } else {
                    print(S(968)); // ") should be at most "
                }
                print_int(n);
                // "I'm going to use 0 instead of that illegal code value."
                help1(S(969));
                error();
                cur_val = 0;
            }
            if (p < MATH_CODE_BASE) {
                define(p, DATA, cur_val);
            } else if (p < DEL_CODE_BASE) {
                define(p, DATA, cur_val);
            } else {
                worddefine(p, cur_val);
            }
            break;

        /// [#1234]
        case DEF_FAMILY:
            p = curchr;
            scan_four_bit_int();
            p += cur_val;
            scan_optional_equals();
            scanfontident();
            define(p, DATA, cur_val);
            break;

        /// [#1235]
        case REGISTER:
        case ADVANCE:
        case MULTIPLY:
        case DIVIDE:
            doregistercommand(a);
            break;

        /// [#1241]
        case SET_BOX:
            scan_eight_bit_int();
            if (a >= 4) {
                n = cur_val + 256;
            } else {
                n = cur_val;
            }
            scan_optional_equals();
            if (set_box_allowed) {
                scanbox(boxflag + n);
            } else {
                print_err(S(597)); // "Improper "
                print_esc(S(970)); // "setbox"
                // "Sorry \\setbox is not allowed after \\halign in a display"
                // "or between \\accent and an accented character."
                help2(S(971), S(972));
                error();
            }
            break;

        /// [#1242]
        case SET_AUX: alteraux(); break;
        case SET_PREV_GRAF: alterprevgraf(); break;
        case SET_PAGE_DIMEN: alterpagesofar(); break;
        case SET_PAGE_INT: alterinteger(); break;
        case SET_BOX_DIMEN:  alterboxdimen(); break;

        /// [#1248]
        case SET_SHAPE:
            scan_optional_equals();
            scan_int();
            n = cur_val;
            if (n <= 0) {
                p = 0;
            } else {
                p = get_node(n * 2 + 1);
                info(p) = n;
                for (j = 1; j <= n; j++) {
                    SCAN_NORMAL_DIMEN();
                    mem[p + j * 2 - MEM_MIN - 1].sc = cur_val; // indentation
                    SCAN_NORMAL_DIMEN();
                    mem[p + j * 2 - MEM_MIN].sc = cur_val; // width
                }
            }
            define(PAR_SHAPE_LOC, SHAPE_REF, p);
            break;

        /// [#1252]
        case HYPH_DATA:
            if (curchr == 1) {
            #ifdef tt_INIT
                /// #1252
                newpatterns();
                goto _Ldone;
            #endif // #1252: tt_INIT
            /* NOT_USE ???
                print_err_str("Patterns can be loaded only by INITEX");
                error();
                do {
                    get_token(); // flush the patterns
                } while (curcmd != RIGHT_BRACE);
            */
            } else {
                newhyphexceptions();
                goto _Ldone;
            }
            break;

        /// [#1253]
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
            if (n == 0) {
                set_hyphenchar(f, cur_val);
            } else {
                set_skewchar(f, cur_val);
            }
            break;

        /// [#1256]
        case DEF_FONT:
            newfont(a); 
            break;

        /// [#1264]
        case SET_INTERACTION:
            newinteraction();
            break;

        default:
            confusion(S(973)); // "prefix"
            break;
    } // switch (curcmd)

_Ldone:
    /// [#1269] Insert a token saved by \\afterassignment, if any.
    if (aftertoken != 0) {
        curtok = aftertoken;
        backinput();
        aftertoken = 0;
    }

_Lexit:;
} /* [#1211]: prefixedcommand */


/*1270:*/
void doassignments(void) {
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
    Selector old_setting;
    char c;
    StrNumber s;

    c = curchr;
    link(garbage) = scantoks(false, true);
    old_setting = selector;
    selector = NEW_STRING;
    tokenshow(defref);
    selector = old_setting;
    flush_list(defref);
    str_room(1);
    s = makestring();
    if (c == 0) { /*1280:*/
        newline_or_space(str_length(s));;

        slow_print(s);
        update_terminal();
        /*1283:*/
    } else {      /*:1283*/
        print_err(S(385)); // ""
        slow_print(s);
        if (errhelp != 0)
            use_err_help = true;
        else if (longhelpseen) {
            help1(S(974));
        } else {
            if (interaction < ERROR_STOP_MODE) longhelpseen = true;
            /*
             * (975) "This error message was generated by an \\errmessage"
             * (976) "command so I can't give any explicit help."
             * (977) "Pretend that you're Hercule Poirot: Examine all clues"
             * (978) "and deduce the truth by order and method."
             */
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
        if (t < CS_TOKEN_FLAG + ACTIVE_BASE) { /*:1289*/
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
            flush_list(link(temphead));
            goto _Lcommonending; /*:1297*/
            break;
    } /*1298:*/

    enddiagnostic(true);
    print_err(S(981)); // "OK"
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
        /*
         * (983) "This isn't an error message; I'm just \\showing something."
         * (984) "Type `I\\show...' to show more (e.g. \\show\\cs"
         * (985) "\\showthe\\count10 \\showbox255 \\showlists)."
         * (986) "And type `I\\tracingonline=1\\show...' to show boxes and"
         * (987) "lists on your terminal as well as in the transcript file."
         */
        help5(S(983), S(984), S(985), S(986), S(987));
    }
    error();
}
/*:1293*/
/** @}*/ // end group S1208x1298_P435x454


/** @addtogroup S1340x1379_P472x481
 * @{
 */

/*1348:*/
/*1349:*/
Static void newwhatsit(SmallNumber s, SmallNumber w) {
    Pointer p;

    p = get_node(w);
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

Static void doextension(void) {
    long k;
    Pointer p;

    switch (curchr) {
        case opennode: /*1351:*/
            newwritewhatsit(opennodesize);
            scan_optional_equals();
            scanfilename();
            openname(tail) = curname;
            openarea(tail) = curarea;
            openext(tail) = curext;
            break;
            /*:1351*/

        case writenode: /*1352:*/
            k = curcs;
            newwritewhatsit(writenodesize);
            curcs = k;
            p = scantoks(false, false);
            writetokens(tail) = defref;
            break;
            /*:1352*/

        case closenode: /*1353:*/
            newwritewhatsit(writenodesize);
            writetokens(tail) = 0;
            break;
            /*:1353*/

        case specialnode: /*1354:*/
            newwhatsit(specialnode, writenodesize);
            writestream(tail) = 0;
            p = scantoks(false, true);
            writetokens(tail) = defref;
            break;
            /*:1354*/

        case immediatecode: /*1375:*/
            get_x_token();
            if (curcmd == EXTENSION && curchr <= closenode) {
                p = tail;
                doextension();
                outwhat(tail);
                flush_node_list(tail);
                tail = p;
                link(p) = 0;
            } else {
                backinput();
            }
            break;
            /*:1375*/

        case setlanguagecode: /*1377:*/
            if (labs(mode) != H_MODE) {
                reportillegalcase();
            } else { /*:1377*/
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
            confusion(S(1001)); // "ext1"
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
/** @}*/ // end group S1340x1379_P472x481


/** @addtogroup S1055x1135_P395x416
 * @{
 */
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
            print_err(S(1002)); // "Too many }'s"
            // "You've closed more groups than you opened."
            // "Such booboos are generally harmless so keep going."
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
                tailappend(get_node(insnodesize));
                type(tail) = INS_NODE;
                subtype(tail) = saved(0);
                height(tail) = height(p) + depth(p);
                insptr(tail) = listptr(p);
                splittopptr(tail) = q;
                depth(tail) = d;
                floatcost(tail) = f;
            } else {
                tailappend(get_node(smallnodesize));
                type(tail) = ADJUST_NODE;
                subtype(tail) = 0;
                adjustptr(tail) = listptr(p);
                delete_glue_ref(q);
            }
            free_node(p, boxnodesize);
            if (nest_ptr == 0) buildpage();
            break;

        case outputgroup: /*1026:*/
            if (LOC != 0 ||
                (token_type != OUTPUT_TEXT && token_type != BACKED_UP)) { /*:1027*/
                print_err(S(1005)); // "Unbalanced output routine"
                // "Your sneaky output routine has problematic {'s and/or }'s."
                // "I can't handle that very well; good luck."
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
                print_err(S(1007)); // "Output routine didn't use all of "
                print_esc(S(464));  // "box"
                print_int(255);
                // "Your \\output commands should empty \\box255"
                // "e.g. by saying `\\shipout\\box255'."
                // "Proceed; I'll discard its present contents."
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
            curtok = CS_TOKEN_FLAG + FROZEN_CR;
            print_err(S(554)); // "Missing "
            print_esc(S(737)); // "cr"
            print(S(555)); // " inserted"
            // "I'm guessing that you meant to end an alignment here."
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
                                free_node(p, noadsize);
                            }
                        }
                    } else if (type(p) == accentnoad) {
                        if (saved(0) == nucleus(tail)) {
                            if (type(tail) == ordnoad) { /*1187:*/
                                q = head;
                                while (link(q) != tail)
                                    q = link(q);
                                link(q) = p;
                                free_node(tail, noadsize);
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
            confusion(S(1012)); // "rightbrace"
            break;
    }

    /*1027:*/
} /*:1068*/

/** @}*/ // end group S1055x1135_P395x416

/** @addtogroup S1338x1339_P470x471
 * @{
 */
#ifdef tt_DEBUG
// 交互式 debug 环境
/// p470#1338
void debughelp(void) {
    long k, l, m, n;

    while (true) {
        printnl(S(1253)); // "debug # (−1 to exit):"
        update_terminal();

        fscanf(TERM_IN, " %ld", &m);
        if (m < 0) {
            return;
        }
        if (m == 0) {
            goto _Lbreakpoint_;
        _Lbreakpoint_:
            m = 0;
            continue;
        }

        fscanf(TERM_IN, " %ld", &n);
        switch (m) {
            /// #1339
            // display mem[n] in all forms
            case 1: printword(mem[n - MEM_MIN]); break;
            case 2: print_int(info(n)); break;
            case 3: print_int(link(n)); break;
            case 4: printword(eqtb[n - ACTIVE_BASE]); break;
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
            case 11: check_mem(n > 0); break;
            // look for pointers to n
            case 12: search_mem(n); break;
            case 13: {
                fscanf(TERM_IN, " %ld", &l);
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
/** @}*/ // end group S1338x1339_P470x471


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
        if (panicking) check_mem(false);
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
            setmathchar(math_code(curchr));
            break;

        case M_MODE + CHAR_NUM:
            scan_char_num();
            curchr = cur_val;
            setmathchar(math_code(curchr));
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
    mainf = cur_font;
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
    free_node(temp_ptr, smallnodesize);
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
        mainp = fontglue[cur_font];
        if (mainp == 0) { /*:1042*/
            FontIndex mmaink;
            mainp = newspec(zeroglue);
            mmaink = parambase[cur_font] + SPACE_CODE;
    #if 1
            maink = mmaink;
    #endif
            width(mainp) = fontinfo[mmaink].sc;
            stretch(mainp) = fontinfo[mmaink + 1].sc;
            shrink(mainp) = fontinfo[mmaink + 2].sc;
            fontglue[cur_font] = mainp;
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

// #37: initterminal @lexer.c

// #1333
Static void close_files_and_terminate(void) {
    Integer k; // all-purpose index

    // #1378: Finish the extensions
    for (k = 0; k <= 15; k++) {
        if (write_open[k]) aclose(&write_file[k]);
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
    if (curlevel > LEVEL_ONE) {
        printnl('(');
    #ifndef USE_REAL_STR
        print_esc(S(1020)); // "end occurred "
        print(S(1021));     // "inside a group at level "
    #else
        print_esc_str("end occurred ");
        print_str("inside a group at level ");
    #endif // USE_REAL_STR
        print_int(curlevel - LEVEL_ONE);
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
        free_node(temp_ptr, ifnodesize);
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
    primitive(S(341), ASSIGN_GLUE, GLUE_BASE);
    primitive(S(342), ASSIGN_GLUE, GLUE_BASE + BASELINE_SKIP_CODE);
    primitive(S(343), ASSIGN_GLUE, GLUE_BASE + PAR_SKIP_CODE);
    primitive(S(344), ASSIGN_GLUE, GLUE_BASE + ABOVE_DISPLAY_SKIP_CODE);
    primitive(S(345), ASSIGN_GLUE, GLUE_BASE + BELOW_DISPLAY_SKIP_CODE);
    primitive(S(346), ASSIGN_GLUE, GLUE_BASE + ABOVE_DISPLAY_SHORT_SKIP_CODE);
    primitive(S(347), ASSIGN_GLUE, GLUE_BASE + BELOW_DISPLAY_SHORT_SKIP_CODE);
    primitive(S(348), ASSIGN_GLUE, GLUE_BASE + LEFT_SKIP_CODE);
    primitive(S(349), ASSIGN_GLUE, GLUE_BASE + RIGHT_SKIP_CODE);
    primitive(S(350), ASSIGN_GLUE, GLUE_BASE + TOP_SKIP_CODE);
    primitive(S(351), ASSIGN_GLUE, GLUE_BASE + SPLIT_TOP_SKIP_CODE);
    primitive(S(352), ASSIGN_GLUE, GLUE_BASE + TAB_SKIP_CODE);
    primitive(S(353), ASSIGN_GLUE, GLUE_BASE + SPACE_SKIP_CODE);
    primitive(S(354), ASSIGN_GLUE, GLUE_BASE + XSPACE_SKIP_CODE);
    primitive(S(355), ASSIGN_GLUE, GLUE_BASE + PAR_FILL_SKIP_CODE);
    primitive(S(356), ASSIGN_MU_GLUE, GLUE_BASE + THIN_MU_SKIP_CODE);
    primitive(S(357), ASSIGN_MU_GLUE, GLUE_BASE + MED_MU_SKIP_CODE);
    primitive(S(358), ASSIGN_MU_GLUE, GLUE_BASE + THICK_MU_SKIP_CODE);
    /*:226*/
    /*230:*/
    primitive(S(1026), ASSIGN_TOKS, OUTPUT_ROUTINE_LOC);
    primitive(S(1027), ASSIGN_TOKS, EVERY_PAR_LOC);
    primitive(S(1028), ASSIGN_TOKS, EVERY_MATH_LOC);
    primitive(S(1029), ASSIGN_TOKS, EVERY_DISPLAY_LOC);
    primitive(S(1030), ASSIGN_TOKS, EVERY_HBOX_LOC);
    primitive(S(1031), ASSIGN_TOKS, EVERY_VBOX_LOC);
    primitive(S(1032), ASSIGN_TOKS, EVERY_JOB_LOC);
    primitive(S(1033), ASSIGN_TOKS, EVERY_CR_LOC);
    primitive(S(1034), ASSIGN_TOKS, ERR_HELP_LOC); /*:230*/
    /*238:*/
    primitive(S(1035), ASSIGN_INT, INT_BASE);
    primitive(S(1036), ASSIGN_INT, INT_BASE + tolerancecode);
    primitive(S(1037), ASSIGN_INT, INT_BASE + linepenaltycode);
    primitive(S(1038), ASSIGN_INT, INT_BASE + hyphenpenaltycode);
    primitive(S(1039), ASSIGN_INT, INT_BASE + exhyphenpenaltycode);
    primitive(S(1040), ASSIGN_INT, INT_BASE + clubpenaltycode);
    primitive(S(1041), ASSIGN_INT, INT_BASE + widowpenaltycode);
    primitive(S(1042), ASSIGN_INT, INT_BASE + displaywidowpenaltycode);
    primitive(S(1043), ASSIGN_INT, INT_BASE + brokenpenaltycode);
    primitive(S(1044), ASSIGN_INT, INT_BASE + binoppenaltycode);
    primitive(S(1045), ASSIGN_INT, INT_BASE + relpenaltycode);
    primitive(S(1046), ASSIGN_INT, INT_BASE + predisplaypenaltycode);
    primitive(S(1047), ASSIGN_INT, INT_BASE + postdisplaypenaltycode);
    primitive(S(1048), ASSIGN_INT, INT_BASE + interlinepenaltycode);
    primitive(S(1049), ASSIGN_INT, INT_BASE + doublehyphendemeritscode);
    primitive(S(1050), ASSIGN_INT, INT_BASE + finalhyphendemeritscode);
    primitive(S(1051), ASSIGN_INT, INT_BASE + adjdemeritscode);
    primitive(S(1052), ASSIGN_INT, INT_BASE + magcode);
    primitive(S(1053), ASSIGN_INT, INT_BASE + delimiterfactorcode);
    primitive(S(1054), ASSIGN_INT, INT_BASE + loosenesscode);
    primitive(S(1055), ASSIGN_INT, INT_BASE + timecode);
    primitive(S(1056), ASSIGN_INT, INT_BASE + daycode);
    primitive(S(1057), ASSIGN_INT, INT_BASE + monthcode);
    primitive(S(1058), ASSIGN_INT, INT_BASE + yearcode);
    primitive(S(1059), ASSIGN_INT, INT_BASE + showboxbreadthcode);
    primitive(S(1060), ASSIGN_INT, INT_BASE + showboxdepthcode);
    primitive(S(1061), ASSIGN_INT, INT_BASE + hbadnesscode);
    primitive(S(1062), ASSIGN_INT, INT_BASE + vbadnesscode);
    primitive(S(1063), ASSIGN_INT, INT_BASE + pausingcode);
    primitive(S(1064), ASSIGN_INT, INT_BASE + tracingonlinecode);
    primitive(S(1065), ASSIGN_INT, INT_BASE + tracingmacroscode);
    primitive(S(1066), ASSIGN_INT, INT_BASE + tracingstatscode);
    primitive(S(1067), ASSIGN_INT, INT_BASE + tracingparagraphscode);
    primitive(S(1068), ASSIGN_INT, INT_BASE + tracingpagescode);
    primitive(S(1069), ASSIGN_INT, INT_BASE + tracingoutputcode);
    primitive(S(1070), ASSIGN_INT, INT_BASE + tracinglostcharscode);
    primitive(S(1071), ASSIGN_INT, INT_BASE + tracingcommandscode);
    primitive(S(1072), ASSIGN_INT, INT_BASE + tracingrestorescode);
    primitive(S(1073), ASSIGN_INT, INT_BASE + uchyphcode);
    primitive(S(1074), ASSIGN_INT, INT_BASE + outputpenaltycode);
    primitive(S(1075), ASSIGN_INT, INT_BASE + maxdeadcyclescode);
    primitive(S(1076), ASSIGN_INT, INT_BASE + hangaftercode);
    primitive(S(1077), ASSIGN_INT, INT_BASE + floatingpenaltycode);
    primitive(S(1078), ASSIGN_INT, INT_BASE + globaldefscode);
    primitive(S(333), ASSIGN_INT, INT_BASE + curfamcode);
    primitive(S(1079), ASSIGN_INT, INT_BASE + ESCAPE_CHARcode);
    primitive(S(1080), ASSIGN_INT, INT_BASE + defaulthyphencharcode);
    primitive(S(1081), ASSIGN_INT, INT_BASE + defaultskewcharcode);
    primitive(S(1082), ASSIGN_INT, INT_BASE + endlinecharcode);
    primitive(S(1083), ASSIGN_INT, INT_BASE + newlinecharcode);
    primitive(S(1084), ASSIGN_INT, INT_BASE + languagecode);
    primitive(S(1085), ASSIGN_INT, INT_BASE + lefthyphenmincode);
    primitive(S(1086), ASSIGN_INT, INT_BASE + righthyphenmincode);
    primitive(S(1087), ASSIGN_INT, INT_BASE + holdinginsertscode);
    primitive(S(1088), ASSIGN_INT, INT_BASE + errorcontextlinescode);
    /*:238*/
    /*248:*/
    primitive(S(1089), ASSIGN_DIMEN, DIMEN_BASE);
    primitive(S(1090), ASSIGN_DIMEN, DIMEN_BASE + mathsurroundcode);
    primitive(S(1091), ASSIGN_DIMEN, DIMEN_BASE + lineskiplimitcode);
    primitive(S(1092), ASSIGN_DIMEN, DIMEN_BASE + hsizecode);
    primitive(S(1093), ASSIGN_DIMEN, DIMEN_BASE + vsizecode);
    primitive(S(1094), ASSIGN_DIMEN, DIMEN_BASE + maxdepthcode);
    primitive(S(1095), ASSIGN_DIMEN, DIMEN_BASE + splitmaxdepthcode);
    primitive(S(1096), ASSIGN_DIMEN, DIMEN_BASE + boxmaxdepthcode);
    primitive(S(1097), ASSIGN_DIMEN, DIMEN_BASE + hfuzzcode);
    primitive(S(1098), ASSIGN_DIMEN, DIMEN_BASE + vfuzzcode);
    primitive(S(1099), ASSIGN_DIMEN, DIMEN_BASE + delimitershortfallcode);
    primitive(S(1100), ASSIGN_DIMEN, DIMEN_BASE + nulldelimiterspacecode);
    primitive(S(1101), ASSIGN_DIMEN, DIMEN_BASE + scriptspacecode);
    primitive(S(1102), ASSIGN_DIMEN, DIMEN_BASE + predisplaysizecode);
    primitive(S(1103), ASSIGN_DIMEN, DIMEN_BASE + displaywidthcode);
    primitive(S(1104), ASSIGN_DIMEN, DIMEN_BASE + displayindentcode);
    primitive(S(1105), ASSIGN_DIMEN, DIMEN_BASE + overfullrulecode);
    primitive(S(1106), ASSIGN_DIMEN, DIMEN_BASE + hangindentcode);
    primitive(S(1107), ASSIGN_DIMEN, DIMEN_BASE + hoffsetcode);
    primitive(S(1108), ASSIGN_DIMEN, DIMEN_BASE + voffsetcode);
    primitive(S(1109), ASSIGN_DIMEN, DIMEN_BASE + emergencystretchcode);
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
    set_text(FROZEN_END_GROUP, S(836));
    eqtb[FROZEN_END_GROUP - ACTIVE_BASE] = eqtb[cur_val - ACTIVE_BASE];
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
    set_text(FROZEN_RELAX, S(1125));
    eqtb[FROZEN_RELAX - ACTIVE_BASE] = eqtb[cur_val - ACTIVE_BASE];
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
    set_text(FROZEN_FI, S(1169));
    eqtb[FROZEN_FI - ACTIVE_BASE] = eqtb[cur_val - ACTIVE_BASE];
    primitive(S(664), FI_OR_ELSE, orcode);
    primitive(S(1170), FI_OR_ELSE, elsecode); /*:491*/
    /*553:*/
    primitive(S(1171), SET_FONT, NULL_FONT);
    set_text(FROZEN_NULL_FONT, S(1171));
    eqtb[FROZEN_NULL_FONT - ACTIVE_BASE] = eqtb[cur_val - ACTIVE_BASE]; /*:553*/
    /*780:*/
    primitive(S(1172), TAB_MARK, spancode);
    primitive(S(737), CAR_RET, crcode);
    set_text(FROZEN_CR, S(737));
    eqtb[FROZEN_CR - ACTIVE_BASE] = eqtb[cur_val - ACTIVE_BASE];
    primitive(S(1173), CAR_RET, crcrcode);
    set_text(FROZEN_END_TEMPLATE, S(1174));
    set_text(FROZEN_ENDV, S(1174));
    eq_type(FROZEN_ENDV) = ENDV;
    equiv(FROZEN_ENDV) = nulllist;
    eq_level(FROZEN_ENDV) = LEVEL_ONE;
    eqtb[FROZEN_END_TEMPLATE - ACTIVE_BASE] = eqtb[FROZEN_ENDV - ACTIVE_BASE];
    eq_type(FROZEN_END_TEMPLATE) = END_TEMPLATE; /*:780*/
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
    set_text(FROZEN_RIGHT, S(419));
    eqtb[FROZEN_RIGHT - ACTIVE_BASE] = eqtb[cur_val - ACTIVE_BASE]; /*:1188*/
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
    primitive(S(467), DEF_CODE, CAT_CODE_BASE);
    primitive(S(471), DEF_CODE, MATH_CODE_BASE);
    primitive(S(468), DEF_CODE, LC_CODE_BASE);
    primitive(S(469), DEF_CODE, UC_CODE_BASE);
    primitive(S(470), DEF_CODE, SF_CODE_BASE);
    primitive(S(473), DEF_CODE, DEL_CODE_BASE);
    primitive(S(266), DEF_FAMILY, MATH_FONT_BASE);
    primitive(S(267), DEF_FAMILY, MATH_FONT_BASE + SCRIPT_SIZE);
    primitive(S(268), DEF_FAMILY, MATH_FONT_BASE + SCRIPT_SCRIPT_SIZE);
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
    primitive(S(1246), CASE_SHIFT, LC_CODE_BASE);
    primitive(S(1247), CASE_SHIFT, UC_CODE_BASE); /*:1286*/
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
        
        error_init();

        /// p#95: 166
        #ifdef tt_DEBUG
            mem_var_init();
        #endif // #166: tt_DEBUG

        lexer_semantic_init();
        page_builder_init();

        eqtb_init();
        hash_var_init();
        eqtb_save_init();
        expand_init();
        scan_init();
        build_token_init();
        cond_process_init();
        fname_init();
        font_init();
        dviout_init();
        pack_init();

        /*662:*/
        packbeginline = 0; /*:662*/
        mmode_init();
        align_init();
        hyphen_init();

        // [#990] see upper `page_builder_init`
        
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
        
        dump_init();
        
        for (k = 0; k <= 17; k++) { // [#1343]
            write_open[k] = false;
        }
    } // end block p11#21

    /// p59#164: Initialize table entries (done by INITEX only)
    #ifdef tt_INIT
        /// 164, 222, 228, 232, 240, 250, 258, 552, 946, 951, 1216, 1301, and 1369.

        mem_init_once();
        eqtb_init_once();
        hash_init_once();
        fonts_init_once();
        hyphen_init_once();

        // #1216
        set_text(FROZEN_PROTECTION, S(258));

        // #1301
        format_ident = S(259); // " (INITEX)"

        // #1369
        set_text(END_WRITE, S(260));
        eq_level(END_WRITE) = LEVEL_ONE;
        eq_type(END_WRITE) = OUTER_CALL;
        equiv(END_WRITE) = 0;

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
    // [#55. #61]
    print_mod_init();

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

    error_selector_init();

    /// [#1337]
    if ((LOC < LIMIT) && (cat_code(buffer[LOC]) != ESCAPE)) {
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
            TERM_OUT,
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