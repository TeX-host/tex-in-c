#include "tex_constant.h" // [const] EMPTY, MIN_QUARTER_WORD, MAX_HALF_WORD
#include "eqtb.h" // vsize
#include "mem.h"
#include "linebreak.h"
#include "pack.h"
#include "print.h"
#include "error.h"
#include "expand.h" // [macro] botmark, firstmark, topmark
#include "texmath.h" // [macro] INF_BAD; [func] print_scaled, x_over_n, badness
#include "mmode.h"
#include "dviout.h"
#include "main_ctrl.h"
#include "scan.h" // MAX_DIMEN
#include "page.h"

/** @addtogroup S980x1028_P366x382
 * @{
 */

/// [#980] the final node on the current page
Pointer pagetail;
/// [#980] what is on the current page so far?
char pagecontents;
/// [#980] maximum box depth on page being built
Scaled pagemaxdepth;
/// [#980] break here to get the best page known so far
Pointer bestpagebreak;
/// [#980] the score for this currently best page
Integer leastpagecost;
/// [#980] its page goal
Scaled bestsize;

/// [#982]
Scaled pagesofar[8];
Pointer lastglue;
Scaled lastkern;
Integer lastpenalty;
Integer insertpenalties;

/// [#989]
Boolean outputactive;


/// [#991] page builder init
static void _new_cur_page() {
    pagecontents = EMPTY;
    pagetail = pagehead;
    link(pagehead) = null;
    lastglue = MAX_HALF_WORD;
    lastpenalty = 0;
    lastkern = 0;
    pagedepth = 0;
    pagemaxdepth = 0;
} /* _new_cur_page */

/// [#990, #991]
void page_builder_init() {
    /// [#990]
    outputactive = false;
    insertpenalties = 0;
    /// [#991]
    _new_cur_page();
} /* page_builder_init */

/// [#981, #988]
void page_builder_init_once() {
    /// #981
    subtype(pageinshead) = MIN_QUARTER_WORD + 255;
    type(pageinshead) = splitup;
    link(pageinshead) = pageinshead;
    /// #988
    type(pagehead) = GLUE_NODE;
    subtype(pagehead) = NORMAL;
}
 
/*985:*/
void printtotals(void)
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
void freezepagespecs(SmallNumber s) {
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
    begin_diagnostic();
    printnl(S(804));
    print_scaled(pagegoal);
    print(S(805));
    print_scaled(pagemaxdepth);
    end_diagnostic(false);
} /*:987*/


/*992:*/
void boxerror(EightBits n)
{
  error();
  begin_diagnostic();
  printnl(S(690));
  showbox(box(n));
  end_diagnostic(true);
  flush_node_list(box(n));
  box(n) = 0;
}
/*:992*/

/*993:*/
void ensurevbox(EightBits n) {
    Pointer p;

    p = box(n);
    if (p == 0) return;
    if (type(p) != HLIST_NODE) return;
    print_err(S(806)); // "Insertions can only be added to a vbox"
    // "Tut tut: You're trying to \\insert into a"
    // "\\box register that now contains an \\hbox."
    // "Proceed and I'll discard its present contents."
    help3(S(807), S(808), S(809));
    boxerror(n);
}
/*:993*/

/*994:*/
/*1012:*/
void fireup(HalfWord c)
{  /*1013:*/
  Pointer p, q, r, s, prevp, savesplittopskip;
  UChar n;
  Boolean wait;
  long savevbadness;
  Scaled savevfuzz;

  if (type(bestpagebreak) == PENALTY_NODE) {
    geq_word_define(INT_BASE + outputpenaltycode, penalty(bestpagebreak));
    penalty(bestpagebreak) = INF_PENALTY;
  } else   /*:1013*/
    geq_word_define(INT_BASE + outputpenaltycode, INF_PENALTY);
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
      print_err(S(385)); // ""
      print_esc(S(464));
      print(S(810));
      help2(S(811), S(809));
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
		  free_node(temp_ptr, boxnodesize);
		  wait = true;
		}
	      }
	    }
	    bestinsptr(r) = 0;
	    n = subtype(r) - MIN_QUARTER_WORD;
	    temp_ptr = listptr(box(n));
	    free_node(box(n), boxnodesize);
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
	  free_node(p, insnodesize);
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
  if (lastglue != MAX_HALF_WORD)
    delete_glue_ref(lastglue);

  _new_cur_page();

  if (q != holdhead) {   /*:1017*/
    link(pagehead) = link(holdhead);
    pagetail = q;
  }
  /*1019:*/
  r = link(pageinshead);
  while (r != pageinshead) {
    q = link(r);
    free_node(r, pageinsnodesize);
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
      new_save_level(outputgroup);
      normalparagraph();
      scan_left_brace();
      goto _Lexit;
    }
    /*:1024*/
    print_err(S(812)); // "Output loop---"
    print_int(deadcycles);
    print(S(813)); // " consecutive dead cycles"
    // "I've concluded that your \\output is awry; it never does a"
    // "\\shipout so I'm shipping \\box255 out myself. Next time"
    // "increase \\maxdeadcycles if you want me to be more patient!"
    help3(S(814), S(815), S(816));
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

void buildpage(void) {
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
                    q = get_node(pageinsnodesize);
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
                        // "Infinite glue shrinkage inserted from "
                        print_err(S(817));
                        // "skip"
                        print_esc(S(460));
                        print_int(n);
                        // "The correction glue for page breaking with insertions"
                        // "must have finite shrinkability. But you may proceed"
                        // "since the offensive shrinkability has been made finite."
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
                                begin_diagnostic();
                                printnl(S(820)); // "% split"
                                print_int(n);
                                print(S(821)); // " to "
                                print_scaled(w);
                                print_char(',');
                                print_scaled(bestheightplusdepth);
                                print(S(764)); // " p="
                                if (q == 0)
                                    print_int(EJECT_PENALTY);
                                else if (type(q) == PENALTY_NODE)
                                    print_int(penalty(q));
                                else
                                    print_char('0');
                                end_diagnostic(false);
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
                confusion(S(822)); // "page"
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
                    begin_diagnostic();
                    printnl('%');
                    print(S(758)); // " t="
                    printtotals();
                    print(S(823)); // " g="
                    print_scaled(pagegoal);
                    print(S(763)); // " b="
                    if (b == awfulbad)
                        print_char('*');
                    else
                        print_int(b);
                    print(S(764)); // " p="
                    print_int(pi);
                    print(S(824)); // " c="
                    if (c == awfulbad)
                        print_char('*');
                    else
                        print_int(c);
                    if (c <= leastpagecost) print_char('#');
                    end_diagnostic(false);
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
                // "Infinite glue shrinkage found on current page"
                print_err(S(825));
                /*
                 * (826) "The page about to be output contains some infinitely"
                 * (795) "shrinkable glue e.g. `\\vss' or `\\vskip 0pt minus 1fil'."
                 * (796) "Such glue doesn't belong there; but you can safely proceed"
                 * (753) "since the offensive shrinkability has been made finite."
                 */
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
/** @}*/ // end group S980x1028_P366x382