#include "main_ctrl.h"

Static void extrarightbrace(void);
Static void package(SmallNumber c);
Static void endgraf(void);
Static void builddiscretionary(void);

/** @addtogroup S1055x1135_P395x416
 * @{
 */
/*1074:*/
Static Pointer curbox;
/*:1074*/

// #1060
void appendglue(void) {
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
    tailappend(new_glue(cur_val));
    if (s < SKIP_CODE) return;

    (gluerefcount(cur_val))--;
    if (s > SKIP_CODE) subtype(tail) = muglue;
} // #1060: appendglue

// #1061
void appendkern(void) {
    QuarterWord s;

    s = curchr;
    scan_dimen(s == muglue, false, false);
    tailappend(new_kern(cur_val));
    subtype(tail) = s;
} // #1061: appendkern

// #1064:
void offsave(void) {
    Pointer p;

    if (cur_group == bottomlevel) { /*1066:*/
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
    switch (cur_group) { /*:1065*/
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

/*1068:*/
void handlerightbrace(void) {
    Pointer p, q;
    Scaled d;
    long f;

    switch (cur_group) {
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
            save_ptr--;
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
                    get_token();
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
            save_ptr -= 2;
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
            save_ptr--;
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


// [#1069]:
void extrarightbrace(void) {
    print_err(S(843)); // "Extra } or forgotten "
    switch (cur_group) {
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
    if (looseness   != 0) eq_word_define(INT_BASE   + loosenesscode,  0);
    if (hangindent  != 0) eq_word_define(DIMEN_BASE + hangindentcode, 0);
    if (hangafter   != 1) eq_word_define(INT_BASE   + hangaftercode,  1);
    if (parshapeptr != 0) eq_define(PAR_SHAPE_LOC, SHAPE_REF, 0);
} /*:1070*/

/*1075:*/
Static void boxend(long boxcontext) {
    Pointer p;

    if (boxcontext < boxflag) { /*1076:*/
        if (curbox == 0) return;
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
            if (mode > 0) buildpage();
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
    }                               /*:1076*/
    if (boxcontext < shipoutflag) { /*1077:*/
        if (boxcontext < boxflag + 256)
            eq_define(BOX_BASE - boxflag + boxcontext, BOX_REF, curbox);
        else /*:1077*/
            geq_define(BOX_BASE - boxflag + boxcontext - 256, BOX_REF, curbox);
        return;
    }
    if (curbox == 0) return;
    if (boxcontext <= shipoutflag) /*1078:*/
    {
        shipout(curbox);
        return;
    }
    /*:1078*/
    skip_spaces_or_relax();
    if ((curcmd == HSKIP && labs(mode) != V_MODE) ||
        (curcmd == VSKIP && labs(mode) == V_MODE) ||
        (curcmd == MSKIP && labs(mode) == M_MODE)) {
        appendglue();
        subtype(tail) = boxcontext - leaderflag + aleaders;
        leaderptr(tail) = curbox;
        return;
    }
    print_err(S(849)); // "Leaders not followed by proper glue"
    // "You should say `\\leaders <box or rule><hskip or vskip>'."
    // "I found the <box or rule> but there's no suitable"
    // "<hskip or vskip> so I'm ignoring these leaders."
    help3(S(850), S(851), S(852));
    backerror();
    flush_node_list(curbox);
}
/*:1075*/

/*1079:*/
void beginbox(long boxcontext) {
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
            if (!is_char_node(tail)) {
                if ((type(tail) == HLIST_NODE) |
                    (type(tail) == VLIST_NODE)) { /*1081:*/
                    q = head;
                    do {
                        p = q;
                        if (!is_char_node(q)) {
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
void scanbox(long boxcontext)
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
void package(SmallNumber c)
{
  Scaled h, d;
  Pointer p;

  d = boxmaxdepth;
  unsave();
  save_ptr -= 3;
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
SmallNumber normmin(long h) {
    if (h <= 0)
        return 1;
    else if (h >= 63)
        return 63;
    else
        return h;
}


void newgraf(Boolean indented)
{
  prevgraf = 0;
  if (mode == V_MODE || head != tail) {
    tailappend(new_param_glue(PAR_SKIP_CODE));
  }
  pushnest();
  mode = H_MODE;
  spacefactor = 1000;
  setcurlang();
  clang = curlang;
  prevgraf = (normmin(lefthyphenmin) * 64 + normmin(righthyphenmin)) * 65536L +
	     curlang;
  if (indented) {
    tail = new_null_box();
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
void indentinhmode(void)
{
  Pointer p, q;

  if (curchr <= 0)
    return;
  p = new_null_box();
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

void headforvmode(void) {
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
void endgraf(void)
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
void begininsertoradjust(void)
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
  save_ptr++;
  new_save_level(insertgroup);
  scan_left_brace();
  normalparagraph();
  pushnest();
  mode = -V_MODE;
  prevdepth = ignoredepth;
}
/*:1099*/


/*1101:*/
void makemark(void)
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
void appendpenalty(void)
{
  scan_int();
  tailappend(new_penalty(cur_val));
  if (mode == V_MODE)
    buildpage();
}
/*:1103*/

/*1105:*/
void deletelast(void) {
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
        if (!is_char_node(tail)) {
        if (type(tail) == curchr) {
            q = head;
            do {
                p = q;
                if (!is_char_node(q)) {
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
void unpackage(void) {
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
void appenditaliccorrection(void) {
    Pointer p;
    InternalFontNumber f;
    int c;

    if (tail != head) {
        if (is_char_node(tail)) {
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
            tailappend(new_kern(charitalic(f, charinfo(f, character(p)))));
        */
        tailappend(new_kern(charitalic(f, charinfo(f, c))));
        subtype(tail) = explicit;
    }
}
/*:1113*/

/*1117:*/
void appenddiscretionary(void)
{
  long c;

  tailappend(new_disc());
  if (curchr == 1) {
    c = get_hyphenchar(cur_font);
    if (c >= 0) {
      if (c < 256)
	prebreak(tail) = newcharacter(cur_font, c);
    }
    return;
  }
  save_ptr++;
  saved(-1) = 0;
  new_save_level(discgroup);
  scan_left_brace();
  pushnest();
  mode = -H_MODE;
  spacefactor = 1000;
}
/*:1117*/

/*1119:*/
void builddiscretionary(void) {
    Pointer p, q;
    long n;

    unsave(); /*1121:*/
    q = head;
    p = link(q);
    n = 0;

    while (p != 0) {
        if (!is_char_node(p)) {
            if (type(p) > RULE_NODE) {
                if (type(p) != KERN_NODE) {
                    if (type(p) != LIGATURE_NODE) {
                        print_err(S(876)); // "Improper discretionary list"
                        // "Discretionary lists must contain only boxes and kerns."
                        help1(S(877)); 
                        error();
                        begin_diagnostic();
                        // "The following discretionary sublist has been deleted:"
                        printnl(S(878));
                        showbox(p);
                        end_diagnostic(true);
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
            save_ptr--;
            goto _Lexit;
            break;
            /*:1120*/
    }
    (saved(-1))++;
    new_save_level(discgroup);
    scan_left_brace();
    pushnest();
    mode = -H_MODE;
    spacefactor = 1000;
_Lexit:;
}
/*:1119*/

/*1123:*/
void makeaccent(void)
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
    r = new_kern(delta);
    subtype(r) = acckern;
    link(tail) = r;
    link(r) = p;
    tail = new_kern(-a - delta);
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
void alignerror(void) {
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
void noalignerror(void) {
    print_err(S(885)); // "Misplaced "
    print_esc(S(897)); // "noalign"
    // "I expect to see \\noalign only after the \\cr of"
    // "an alignment. Proceed and I'll ignore this case."
    help2(S(898), S(899));
    error();
}

void omiterror(void) {
    print_err(S(885)); // "Misplaced "
    print_esc(S(900)); // "omit"
    // "I expect to see \\omit only after tab marks or the \\cr of"
    // "an alignment. Proceed and I'll ignore this case."
    help2(S(901), S(899));
    error();
}
/*:1129*/

/*1131:*/
void doendv(void)
{
  if (cur_group != aligngroup) {
    offsave();
    return;
  }
  endgraf();
  if (fincol())
    finrow();
}
/*:1131*/

/*1135:*/
void cserror(void) {
    print_err(S(558)); // "Extra "
    print_esc(S(263)); // "endcsname"
    help1(S(902));     // "I'm ignoring this since I wasn't doing a \\csname."
    error();
} /*:1135*/

/** @}*/ // end group S1055x1135_P395x416