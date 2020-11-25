#include "main_ctrl.h"


/** @addtogroup S1136x1207_P417x434
 * @{
 */

/*1136:*/
Static void pushmath(GroupCode c)
{
  pushnest();
  mode = -M_MODE;
  incompleatnoad = 0;
  new_save_level(c);
}
/*:1136*/

/*1138:*/
void initmath(void)
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
    eq_word_define(INT_BASE + curfamcode, -1);
    eq_word_define(DIMEN_BASE + predisplaysizecode, w);
    eq_word_define(DIMEN_BASE + displaywidthcode, l);
    eq_word_define(DIMEN_BASE + displayindentcode, s);
    if (everydisplay != 0)
      begintokenlist(everydisplay, EVERY_DISPLAY_TEXT);
    if (nest_ptr == 1)
      buildpage();
    return;
  }
  /*:1145*/
  backinput();   /*1139:*/
  pushmath(mathshiftgroup);
  eq_word_define(INT_BASE + curfamcode, -1);
  if (everymath != 0)   /*:1139*/
    begintokenlist(everymath, EVERY_MATH_TEXT);
}
/*:1138*/

/*1142:*/
void starteqno(void) {
    saved(0) = curchr;
    save_ptr++; /*1139:*/
    pushmath(mathshiftgroup);
    eq_word_define(INT_BASE + curfamcode, -1);
    if (everymath != 0) /*:1139*/
        begintokenlist(everymath, EVERY_MATH_TEXT);
}
/*:1142*/

/*1151:*/
void scanmath(HalfWord p) {
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
            save_ptr++;
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
void setmathchar(long c)
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
void mathlimitswitch(void) {
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
void mathradical(void)
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
void mathac(void) {
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
void appendchoices(void)
{
  tailappend(newchoice());
  save_ptr++;
  saved(-1) = 0;
  pushmath(mathchoicegroup);
  scan_left_brace();
}
/*:1172*/

/*1174:*/
/*1184:*/
HalfWord finmlist(HalfWord p) {
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

void buildchoices(void) {
    Pointer p;

    unsave();
    p = finmlist(0);
    switch (saved(-1)) {
        case 0: displaymlist(tail) = p; break;
        case 1: textmlist(tail) = p; break;
        case 2: scriptmlist(tail) = p; break;
        case 3:
            scriptscriptmlist(tail) = p;
            save_ptr--;
            return;
            break;
    }
    (saved(-1))++;
    pushmath(mathchoicegroup);
    scan_left_brace();
}
/*:1174*/

/*1176:*/
void subsup(void) {
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
void mathfraction(void) {
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
void mathleftright(void) {
    SmallNumber t;
    Pointer p;

    t = curchr;
    /*1192:*/
    if (t == rightnoad && cur_group != mathleftgroup) {
        if (cur_group != mathshiftgroup) {
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
void aftermath(void) {
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
        save_ptr--;
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
        tailappend(new_math(mathsurround, before));
        curmlist = p;
        curstyle = textstyle;
        mlistpenalties = (mode > 0);
        mlisttohlist();
        link(tail) = link(temphead);
        while (link(tail) != 0)
            tail = link(tail);
        tailappend(new_math(mathsurround, after));
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
    tailappend(new_penalty(predisplaypenalty));
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
        tailappend(new_penalty(INF_PENALTY));
    } else {
        tailappend(new_param_glue(g1)); /*:1203*/
    }

    if (e != 0) {
        r = new_kern(z - w - e - d);
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
        tailappend(new_penalty(INF_PENALTY));
        shiftamount(a) = s + z - width(a);
        appendtovlist(a);
        g2 = 0;
    }
    if (t != adjusthead) {
        link(tail) = link(adjusthead);
        tail = t;
    }
    tailappend(new_penalty(postdisplaypenalty));
    if (g2 > 0) {                     /*:1199*/
        tailappend(new_param_glue(g2)); /*:1205*/
    }
    resumeafterdisplay();

    /*:1195*/
    /*:1195*/
}
/*:1194*/

/*1200:*/
void resumeafterdisplay(void) {
    // "display"
    if (cur_group != mathshiftgroup) confusion(S(934));

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