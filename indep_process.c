
#include "main_ctrl.h"

/** @addtogroup S1208x1298_P435x454
 * @{
 */

/*1266:*/
HalfWord aftertoken;
/*:1266*/
/*1281:*/
Static Boolean longhelpseen;
/*:1281*/

/// [#1267, #1282]
void mode_indep_init() {
    /// [#1267]
    aftertoken = 0;
    /// [#1282]
    longhelpseen = false;
} /* mode_indep_init */

/// [#1216]
void mode_indep_init_once() {
    set_text(FROZEN_PROTECTION, S(258)); // "inaccessible"
} /* mode_indep_init_once */

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
        get_token();
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
            if (q == ADVANCE) cur_val += eqtb[l].int_;
        } else {
            scan_glue(p);
            if (q == ADVANCE) {
                /// [#1239] Compute the sum of two glue specs.
                q = new_spec(cur_val);
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
                cur_val = x_over_n(eqtb[l].int_, cur_val);
            }
        } else {
            s = equiv(l);
            r = new_spec(s);
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
        if (str_eq_str(get_fontname(f), cur_name) /* &
	        str_eq_str(fontarea[f ], cur_area) */ ) {
            /*:1260*/
            #if 0
                if (cur_name == flushablestring) {
                    flush_string();
                    cur_name = fontname[f ];
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
    f = readfontinfo(u, cur_name, cur_area, s);

_Lcommonending:
    equiv(u) = f;
    eqtb[FONT_ID_BASE + f] = eqtb[u];
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
void prefixedcommand(void) {
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
                    get_token();
                } while (curcmd == SPACER);
                if (curtok == othertoken + '=') {
                    get_token();
                    if (curcmd == SPACER) get_token();
                }
            } else {
                // look ahead, then back up
                get_token();
                q = curtok;
                get_token();
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
void openorclosein(void) {
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
    if (cur_ext == S(385)) cur_ext = S(669);
    packfilename(cur_name, S(677), cur_ext);
    if (a_open_in(&readfile[n])) readopen[n] = justopen;
}
/*:1275*/

/*1279:*/
void issuemessage(void) {
    Selector old_setting;
    char c;
    StrNumber s;

    c = curchr;
    link(garbage) = scantoks(false, true);
    old_setting = selector;
    selector = NEW_STRING;
    token_show(defref);
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
void shiftcase(void) {
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
void showwhatever(void) {
    switch (curchr) {
        case showlists:
            begin_diagnostic();
            showactivities();
            break;

        case showboxcode: /*1296:*/
            scan_eight_bit_int();
            begin_diagnostic();
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
            get_token();
            printnl(S(980));
            if (curcs != 0) {
                sprint_cs(curcs);
                print_char('=');
            }
            print_meaning(curcmd, curchr);
            goto _Lcommonending;
            break;
            /*:1294*/
            /*1297:*/

        default:
            (void)thetoks();
            printnl(S(980));
            token_show(temphead);
            flush_list(link(temphead));
            goto _Lcommonending; /*:1297*/
            break;
    } /*1298:*/

    end_diagnostic(true);
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