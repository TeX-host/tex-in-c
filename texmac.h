#pragma once
#ifndef TEXMAC_H
#define TEXMAC_H

// tex.c 里使用的宏
#undef labs
#define null 0

#define formatextension S(256)
#define checkinterrupt() ((interrupt != 0) ? (pause_for_instructions(), 0) : 0)

/* P2C compatibility */
#define P_clrbits_B(trie, idx, z, w) trie[(idx) >> 3] &= ~(1 << ((idx)&7))
#define P_getbits_UB(trie, h, z, w) (trie[(h) >> 3] & (1 << ((h)&7)))
#define P_putbits_UB(trie, h, y, z, w) trie[(h) >> 3] |= 1 << ((h)&7)

#define wopenout a_open_out

#define link(x) (mem[(x)].hh.rh)
#define info(x) (mem[(x)].hh.UU.lh)
#define type(x) (mem[(x)].hh.UU.U2.b0)

/*
#define foocharinfo(x,y) (&fontinfo[charbase[x]+(y)].qqqq)
*/
#define ligkernstart(x, y) (ligkernbase[(x)] + rembyte(y))
#define ligkernrestart(x, y)                                                   \
    (ligkernbase[(x)] + opbyte(y) * 256 + rembyte(y) - kernbaseoffset + 32768L)
#define exteninfo(f, q) (fontinfo[extenbase[(f)] + rembyte(q)].qqqq)

#define isempty(x) (link(x) == emptyflag)

#define isrunning(x) ((x) == nullflag) /* {tests for a running dimension} */

#define hashisfull (hash_used == hashbase)
#define equiv(x) (eqtb[(x)-activebase].hh.rh)

/* Glue parameters */
#define gluepar(x)  equiv(gluebase+(x)) 
#define lineskip  gluepar(LINE_SKIP_CODE)
#define baselineskip  gluepar(BASELINE_SKIP_CODE)
#define parskip  gluepar(PAR_SKIP_CODE)
#define abovedisplayskip  gluepar(ABOVE_DISPLAY_SKIP_CODE)
#define belowdisplayskip  gluepar(BELOW_DISPLAY_SKIP_CODE)
#define abovedisplayshortskip  gluepar(ABOVE_DISPLAY_SHORT_SKIP_CODE)
#define belowdisplayshortskip  gluepar(BELOW_DISPLAY_SHORT_SKIP_CODE)
#define leftskip  gluepar(LEFT_SKIP_CODE)
#define rightskip  gluepar(RIGHT_SKIP_CODE)
#define topskip  gluepar(TOP_SKIP_CODE)
#define splittopskip  gluepar(SPLIT_TOP_SKIP_CODE)
#define tabskip  gluepar(TAB_SKIP_CODE)
#define spaceskip  gluepar(SPACE_SKIP_CODE)
#define xspaceskip  gluepar(XSPACE_SKIP_CODE)
#define parfillskip  gluepar(PAR_FILL_SKIP_CODE)
#define thinmuskip  gluepar(THIN_MU_SKIP_CODE)
#define medmuskip  gluepar(MED_MU_SKIP_CODE)
#define thickmuskip  gluepar(THICK_MU_SKIP_CODE)

/* Integer parameters */
#define intpar(x)  (eqtb[intbase+(x)-activebase].int_) 
#define pretolerance  intpar(pretolerancecode)
#define tolerance  intpar(tolerancecode)
#define linepenalty  intpar(linepenaltycode)
#define hyphenpenalty  intpar(hyphenpenaltycode)
#define exhyphenpenalty  intpar(exhyphenpenaltycode)
#define clubpenalty  intpar(clubpenaltycode)
#define widowpenalty  intpar(widowpenaltycode)
#define displaywidowpenalty  intpar(displaywidowpenaltycode)
#define brokenpenalty  intpar(brokenpenaltycode)
#define binoppenalty  intpar(binoppenaltycode)
#define relpenalty  intpar(relpenaltycode)
#define predisplaypenalty  intpar(predisplaypenaltycode)
#define postdisplaypenalty  intpar(postdisplaypenaltycode)
#define interlinepenalty  intpar(interlinepenaltycode)
#define doublehyphendemerits  intpar(doublehyphendemeritscode)
#define finalhyphendemerits  intpar(finalhyphendemeritscode)
#define adjdemerits  intpar(adjdemeritscode)
#define mag  intpar(magcode)
#define delimiterfactor  intpar(delimiterfactorcode)
#define looseness  intpar(loosenesscode)
#define tex_time  intpar(timecode)
#define day  intpar(daycode)
#define month  intpar(monthcode)
#define year  intpar(yearcode)
#define showboxbreadth  intpar(showboxbreadthcode)
#define showboxdepth  intpar(showboxdepthcode)
#define hbadness  intpar(hbadnesscode)
#define vbadness  intpar(vbadnesscode)
#define pausing  intpar(pausingcode)
#define tracingonline  intpar(tracingonlinecode)
#define tracingmacros  intpar(tracingmacroscode)
#define tracingstats  intpar(tracingstatscode)
#define tracingparagraphs  intpar(tracingparagraphscode)
#define tracingpages  intpar(tracingpagescode)
#define tracingoutput  intpar(tracingoutputcode)
#define tracinglostchars  intpar(tracinglostcharscode)
#define tracingcommands  intpar(tracingcommandscode)
#define tracingrestores  intpar(tracingrestorescode)
#define uchyph  intpar(uchyphcode)
#define outputpenalty  intpar(outputpenaltycode)
#define maxdeadcycles  intpar(maxdeadcyclescode)
#define hangafter  intpar(hangaftercode)
#define floatingpenalty  intpar(floatingpenaltycode)
#define globaldefs  intpar(globaldefscode)
#define curfam  intpar(curfamcode)
#define ESCAPE_CHAR  intpar(ESCAPE_CHARcode)
#define defaulthyphenchar  intpar(defaulthyphencharcode)
#define defaultskewchar  intpar(defaultskewcharcode)
#define end_line_char  intpar(endlinecharcode)
#define newlinechar  intpar(newlinecharcode)
#define language  intpar(languagecode)
#define lefthyphenmin  intpar(lefthyphenmincode)
#define righthyphenmin  intpar(righthyphenmincode)
#define holdinginserts  intpar(holdinginsertscode)
#define errorcontextlines  intpar(errorcontextlinescode)

/* Dimen pars */
#define dimenpar(x)  (eqtb[dimenbase+(x)-activebase].int_) 
#define parindent  dimenpar(parindentcode)
#define mathsurround  dimenpar(mathsurroundcode)
#define lineskiplimit  dimenpar(lineskiplimitcode)
#define hsize  dimenpar(hsizecode)
#define vsize  dimenpar(vsizecode)
#define maxdepth  dimenpar(maxdepthcode)
#define splitmaxdepth  dimenpar(splitmaxdepthcode)
#define boxmaxdepth  dimenpar(boxmaxdepthcode)
#define hfuzz  dimenpar(hfuzzcode)
#define vfuzz  dimenpar(vfuzzcode)
#define delimitershortfall  dimenpar(delimitershortfallcode)
#define nulldelimiterspace  dimenpar(nulldelimiterspacecode)
#define scriptspace  dimenpar(scriptspacecode)
#define predisplaysize  dimenpar(predisplaysizecode)
#define displaywidth  dimenpar(displaywidthcode)
#define displayindent  dimenpar(displayindentcode)
#define overfullrule  dimenpar(overfullrulecode)
#define hangindent  dimenpar(hangindentcode)
#define hoffset  dimenpar(hoffsetcode)
#define voffset  dimenpar(voffsetcode)
#define emergencystretch  dimenpar(emergencystretchcode)
/*
*/

#define parshapeptr  equiv(parshapeloc)
#define outputroutine  equiv(outputroutineloc)
#define everypar  equiv(everyparloc)
#define everymath  equiv(everymathloc)
#define everydisplay  equiv(everydisplayloc)
#define everyhbox  equiv(everyhboxloc)
#define everyvbox  equiv(everyvboxloc)
#define everyjob  equiv(everyjobloc)
#define everycr  equiv(everycrloc)
#define errhelp  equiv(errhelploc)
#define curfont  equiv(curfontloc)
/*
*/
#define faminrange ((curfam >= 0) && (curfam < 16))
#define setpagesofarzero(x) (pagesofar[(x)] = 0)
#define setheightzero(x)                                                       \
    (activeheight[(x)-1] = 0) /* {initialize the height to zero} */
#define setcurlang()                                                           \
    ((language <= 0)                                                           \
         ? (curlang = 0)                                                       \
         : ((language > 255) ? (curlang = 0) : (curlang = language)))
#define storebackground(x) (activewidth[(x)-1] = background[(x)-1])
#define updateactive(x) (activewidth[(x)-1] += mem[r + (x)].sc)
#define copytocuractive(x) (curactivewidth[(x)-1] = activewidth[(x)-1])
#define updatewidth(x) (curactivewidth[(x)-1] += mem[r + (x)].sc)
#define setbreakwidthtobackground(x) (breakwidth[(x)-1] = background[(x)-1])
#define converttobreakwidth(x)                                                 \
    (mem[prevr + (x)].sc += -curactivewidth[(x)-1] + breakwidth[(x)-1])
#define storebreakwidth(x) (activewidth[(x)-1] = breakwidth[(x)-1])
#define newdeltatobreakwidth(x)                                                \
    (mem[q + (x)].sc = breakwidth[(x)-1] - curactivewidth[(x)-1])
#define newdeltafrombreakwidth(x)                                              \
    (mem[q + (x)].sc = curactivewidth[(x)-1] - breakwidth[(x)-1])
#define combinetwodeltas(x)                                                    \
    (mem[prevr + (x)].sc = mem[prevr + (x)].sc + mem[r + (x)].sc)
#define downdatewidth(x) (curactivewidth[(x)-1] -= mem[prevr + (x)].sc)
#define checkshrinkage(x)                                                      \
    ((shrinkorder(x) != NORMAL) && (shrink(x) != 0) ? (x) = finiteshrink(x) : 0)
#define vetglue(x)                                                             \
    (gluetemp = (x),                                                           \
     ((gluetemp > (1000000000.0))                                              \
          ? (gluetemp = 1000000000.0)                                          \
          : ((gluetemp < -1000000000.0) ? (gluetemp = -1000000000.0) : 0)))

// [#360]: \endlinechar 行终止符无效，不添加换行符
#define end_line_char_inactive ((end_line_char < 0) || (end_line_char > 255))
#define ishex(x)                                                               \
    ((((x) >= '0') && ((x) <= '9')) || (((x) >= 'a') && ((x) <= 'f')))


#define precedesbreak(x) (type(x) < MATH_NODE)
#define nondiscardable(x) (type(x) < MATH_NODE)
// 无需打开输出流，直接使用 stdout
// #define topenin() (termin = stdin) // 33, 37
// #define topenout() (termout = stdout) // 33, 1332
#define global (a >= 4)
#define define(x, y, z)                                                        \
    ((a >= 4) ? geqdefine((x), (y), (z)) : eqdefine((x), (y), (z)))
#define worddefine(x, y)                                                       \
    ((a >= 4) ? geqworddefine((x), (y)) : eqworddefine((x), (y)))

// #866
#define actwidth  activewidth[0]    // length from first active node to current node


#define trieroot  (triel[0])        // root of the linked trie
#define activeheight  activewidth /*new name for the six distance variables*/
#define curheight  (activeheight[0]) /*the natural height*/
#define pagegoal  (pagesofar[0]) /*desired height of information on page being built*/
#define pagetotal  (pagesofar[1]) /*height of the current page*/
#define pageshrink  (pagesofar[6]) /*shrinkability of the current page*/
#define pagedepth  (pagesofar[7]) /*depth of the current page*/
#define contribtail  (nest[0].tailfield) /*tail of the contribution list*/

#if 0
#define ischarnode(x) ((x) >= hi_mem_min)
#else
#define ischarnode(x)                                                          \
    (((((x) >= hi_mem_min) != (type(x) == charnodetype)) ? niezgodnosc(x)      \
                                                         : 0),                 \
     ((x) >= hi_mem_min))
#endif

#define set_as_char_node(x) (type(x)=charnodetype) 
#define unset_is_char_node(x) 0 /* (mem[(x)].is_char_node=0)*/
/* Makra specjalne */
#define param(x,y) (fontinfo[(x)+parambase[y]].sc)
#define mathsy(x,y) (fontinfo[(x)+parambase[famfnt(2+(y))]].sc)
#define vpack(x,y,z)  vpackage((x),(y),(z),MAX_DIMEN) /* special case of unconstrained depth}*/

#define scriptsallowed(x)  ((type(x)>=ordnoad)&&(type(x)<leftnoad))
// #define lig_kern_start(x)  (lig_kern_base[x]+rem_byte) /* {beginning of
// lig/kern program} */

#define ligchar(x)  ((x)+1) /* {the word where the ligature is to be found} */
/*
#define charexists(x)  ((x).b0>MIN_QUARTER_WORD)
#define chartag(x)  ((qo((x).b2)) % 4)
*/
#define tailappend(x)  (link(tail)=(x), tail=link(tail))


/** @addtogroup S203x206_P71x72
 * @{
 */

/// [#203] new reference to a token list.
#define addtokenref(x)  (tokenrefcount(x)++)
/// [#203] new reference to a glue spec.
#define addglueref(x)   (gluerefcount(x)++)
/** @}*/ // end group S203x206_P71x72


#define qi(x) (x)
#define qo(x) (x)

/** @addtogroup S211x219_P77x80
 * @{
 */

#define mode  cur_list.modefield /* current mode}*/
#define head  cur_list.headfield /* header node of current list}*/
#define tail  cur_list.tailfield /* final node on current list}*/
#define prevgraf  cur_list.pgfield /* number of paragraph lines accumulated}*/
#define aux  cur_list.auxfield /* auxiliary data about the current list}*/
#define prevdepth  aux.sc /* the name of |aux| in vertical mode}*/
#define spacefactor  aux.hh.UU.lh /* part of |aux| in horizontal mode}*/
#define clang  aux.hh.rh /* the other part of |aux| in horizontal mode}*/
#define incompleatnoad  aux.int_ /* the name of |aux| in math mode}*/
#define modeline  cur_list.mlfield /* source file line number at beginning of list}*/
/** @}*/ // end group S211x219_P77x80


#define topmark  curmark[topmarkcode- topmarkcode]
#define firstmark  curmark[firstmarkcode - topmarkcode]
#define botmark  curmark[botmarkcode - topmarkcode]
#define splitfirstmark  curmark[splitfirstmarkcode - topmarkcode]
#define splitbotmark  curmark[splitbotmarkcode- topmarkcode]


#define stopflag  qi(128) /* value indicating `\.{STOP}' in a lig/kern program}*/
#define kernflag  qi(128) /* op code for a kern step}*/
#define defaultrulethickness  mathex(8) /* thickness of \.{\\over} bars}*/
#define bigopspacing1  mathex(9) /* minimum clearance above a displayed op}*/
#define bigopspacing2  mathex(10) /* minimum clearance below a displayed op}*/
#define bigopspacing3  mathex(11) /* minimum baselineskip above displayed op}*/
#define bigopspacing4  mathex(12) /* minimum baselineskip below displayed op}*/
#define bigopspacing5  mathex(13) /* padding above and below displayed limits}*/
#define preamble  link(alignhead) /* the current preamble list}*/



#define slant(x)  param(SLANT_CODE,x) /* slant to the right, per unit distance upward}*/
#define space(x)  param(SPACE_CODE,x) /* NORMAL space between words}*/
#define spacestretch(x)  param(SPACE_STRETCH_CODE,x) /* stretch between words}*/
#define spaceshrink(x)  param(SPACE_SHRINK_CODE,x) /* shrink between words}*/
#define xheight(x)  param(X_HEIGHT_CODE,x) /* one ex}*/
#define quad(x)  param(QUAD_CODE,x) /* one em}*/
#define extraspace(x)  param(EXTRA_SPACE_CODE,x) /* additional space at end of sentence}*/


#define mathxheight(x)  mathsy(5,x) /* height of `\.x'}*/
#define mathquad(x)  mathsy(6,x) /* \.{18mu}}*/
#define num1(x)  mathsy(8,x) /* numerator shift-up in display styles}*/
#define num2(x)  mathsy(9,x) /* numerator shift-up in non-display, non-\.{\\atop}}*/
#define num3(x)  mathsy(10,x) /* numerator shift-up in non-display \.{\\atop}}*/
#define denom1(x)  mathsy(11,x) /* denominator shift-down in display styles}*/
#define denom2(x)  mathsy(12,x) /* denominator shift-down in non-display styles}*/
#define sup1(x)  mathsy(13,x) /* superscript shift-up in uncramped display style}*/
#define sup2(x)  mathsy(14,x) /* superscript shift-up in uncramped non-display}*/
#define sup3(x)  mathsy(15,x) /* superscript shift-up in cramped styles}*/
#define sub1(x)  mathsy(16,x) /* subscript shift-down if superscript is absent}*/
#define sub2(x)  mathsy(17,x) /* subscript shift-down if superscript is present}*/
#define supdrop(x)  mathsy(18,x) /* superscript baseline below top of large box}*/
#define subdrop(x)  mathsy(19,x) /* subscript baseline below bottom of large box}*/
#define delim1(x)  mathsy(20,x) /* size of \.{\\atopwithdelims} delimiters*/
/* in display styles}*/
#define delim2(x)  mathsy(21,x) /* size of \.{\\atopwithdelims} delimiters in non-displays}*/
#define axisheight(x)  mathsy(22,x) /* height of fraction lines above the baseline}*/

/** @addtogroup S115x132_P44x49
 * @{
 */

#define nodesize    info /* the size field in empty variable-size nodes}*/
#define llink(x)    info(x+1) /* left link in doubly-linked list of empty nodes}*/
#define rlink(x)    link(x+1) /* right link in doubly-linked list of empty nodes}*/
/** @}*/                      // end group S115x132_P44x49


#define subtype(x)    mem[x].hh.UU.U2.b1 /* secondary identification in some cases}*/



#ifdef BIG_CHARNODE

#if 1
/* #define font(x)	info(x+1) */
#define font(x) link(x + 1)
/* #define character(x) link(x+1) */
/* #define character(x)    info(x+1) */
#define character(x) subtype(x)
#else
#define font(x) type(x)
#define character(x) subtype(x)
#endif

#else
#define font(x) type(x)         /* the font code in a |charnode|}*/
#define character(x) subtype(x) /* the character code in a |charnode|}*/
#endif // #ifdef BIG_CHARNODE

#define width(x)    mem[x+widthoffset].sc /* width of the box, in sp}*/
#define depth(x)    mem[x+depthoffset].sc /* depth of the box, in sp}*/
#define height(x)    mem[x+heightoffset].sc /* height of the box, in sp}*/
#define shiftamount(x)    mem[x+4].sc /* repositioning distance, in sp}*/
#define listptr(x)    link(x+listoffset) /* beginning of the list inside the box}*/
#define glueorder(x)    subtype(x+listoffset) /* applicable order of INFINITY}*/
#define gluesign(x)    type(x+listoffset) /* stretching or shrinking}*/
#define glueset(x)    mem[x+glueoffset].gr
/* a word of type |glueratio| for glue setting}*/
#define floatcost(x)  mem[x+1].int_ /* the |floatingpenalty| to be used}*/
#define insptr(x)  info(x+4) /* the vertical list to be inserted}*/
#define splittopptr(x)  link(x+4) /* the |splittopskip| to be used}*/
#define markptr(x)  mem[x+1].int_ /* head of the token list for a mark}*/
#define adjustptr  markptr /* vertical list to be moved out of horizontal list}*/
#define ligptr(x)  link(ligchar(x)) /* the list of characters}*/
#define replacecount  subtype /* how many subsequent nodes to replace}*/
#define prebreak  llink /* text that precedes a discretionary break}*/
#define postbreak  rlink /* text that follows a discretionary break}*/
#define glueptr  llink /* pointer to a glue specification}*/
#define leaderptr  rlink /* pointer to box or rule node for leaders}*/
#define gluerefcount(x)    link(x) /* reference count of a glue specification}*/
#define stretch(x)    mem[x+2].sc /* the stretchability of this glob of glue}*/
#define shrink(x)    mem[x+3].sc /* the shrinkability of this glob of glue}*/
#define stretchorder(x)    type(x) /* order of INFINITY for stretching}*/
#define shrinkorder(x)    subtype(x) /* order of INFINITY for shrinking}*/
#define penalty(x)    mem[x+1].int_ /* the added cost of breaking a list here}*/
#define gluestretch(x)  mem[x+glueoffset].sc /* total stretch in an unset node}*/
#define glueshrink  shiftamount /* total shrink in an unset node}*/
#define spancount  subtype /* indicates the number of spanned columns}*/
// [#200] reference count preceding a token list
#define tokenrefcount(x) info(x)
#define eqlevelfield(x)  x.hh.UU.U2.b1
#define eqtypefield(x)  x.hh.UU.U2.b0
#define equivfield(x)  x.hh.rh
#define eqlevel(x)  eqlevelfield(eqtb[x-activebase]) /* level of definition}*/
#define eqtype(x)  eqtypefield(eqtb[x-activebase]) /* command code for equivalent}*/
#define skip(x)  equiv(skipbase+x) /* |mem| location of glue specification}*/
#define muskip(x)  equiv(muskipbase+x) /* |mem| location of math glue spec}*/
#define toks(x)  equiv(toksbase+x)
#define box(x)  equiv(boxbase+x)
#define famfnt(x) equiv(mathfontbase + x)
#define catcode(x) equiv(catcodebase + x)
#define lccode(x) equiv(lccodebase + x)
#define uccode(x) equiv(uccodebase + x)
#define sfcode(x) equiv(sfcodebase + x)
#define mathcode(x) equiv(mathcodebase + x)
/*  Note: |mathcode(c)| is the true math code plus |minhalfword|} */
#define delcode(x)  eqtb[delcodebase+x-activebase].int_
#define count(x)  eqtb[countbase+x-activebase].int_
#define dimen(x)  eqtb[SCALED_BASE+x-activebase].sc
#define next(x)    hash[x-hashbase].UU.lh /* link for coalesced lists}*/
#define text(x)    hash[x-hashbase].rh /* string number for control sequence name}*/
#define savetype(x)  savestack[x].hh.UU.U2.b0 /* classifies a |savestack| entry}*/
#define savelevel(x)  savestack[x].hh.UU.U2.b1
/* saved level for regions 5 and 6, or group code}*/
#define saveindex(x)  savestack[x].hh.rh
/* |eqtb| location or |savestack| location}*/

#define largechar(x)  mem[x].qqqq.b3 /* |character| for ``large'' delimiter}*/

// #define location(x)  mem[x+2].int_ /* \.{DVI} byte number for a movement command}*/

#define heightdepth(x) qo(x.b1)
#define saved(x) savestack[saveptr + x].int_
#define skipbyte(x) x.b0
#define nextchar(x) x.b1
#define exttop(x) x.b0 /* |top| piece in a recipe}*/
#define extmid(x) x.b1 /* |mid| piece in a recipe}*/
#define extbot(x) x.b2 /* |bot| piece in a recipe}*/
#define extrep(x) x.b3 /* |rep| piece in a recipe}*/

#if 1
#define nucleus(x)  ((x)+CHAR_NODE_SIZE) /* the |nucleus| field of a noad}*/
#define supscr(x)  (nucleus(x)+CHAR_NODE_SIZE) /* the |supscr| field of a noad}*/
#define subscr(x)  (supscr(x)+CHAR_NODE_SIZE) /* the |subscr| field of a noad}*/
#define leftdelimiter(x)  (subscr(x)+CHAR_NODE_SIZE) /* first delimiter field of a noad}*/
#define rightdelimiter(x)  (leftdelimiter(x)+CHAR_NODE_SIZE) /* second delimiter field of a fraction noad}*/
#define accentchr(x)  (subscr(x)+CHAR_NODE_SIZE) /* the |accentchr| field of an accent noad}*/
#else
#define nucleus(x)  ((x)+1) /* the |nucleus| field of a noad}*/
#define supscr(x)  ((x)+2) /* the |supscr| field of a noad}*/
#define subscr(x)  ((x)+7) /* the |subscr| field of a noad}*/
#define leftdelimiter(x)  ((x)+4) /* first delimiter field of a noad}*/
#define rightdelimiter(x)  ((x)+5) /* second delimiter field of a fraction noad}*/
#define accentchr(x)  ((x)+4) /* the |accentchr| field of an accent noad}*/
#endif


#define mathtype  link /* a |halfword| in |mem|}*/
#define fam(x)  type(x)
#define smallfam(x)  mem[x].qqqq.b0 /* |fam| for ``small'' delimiter}*/
#define smallchar(x)  mem[x].qqqq.b1 /* |character| for ``small'' delimiter}*/
#define largefam(x)  mem[x].qqqq.b2 /* |fam| for ``large'' delimiter}*/
#define thickness(x) (mem[nucleus(x)-MEM_MIN].sc) /* |thickness| field in a fraction noad}*/
#define numerator  supscr /* |numerator| field in a fraction noad}*/
#define denominator  subscr /* |denominator| field in a fraction noad}*/
#define delimiter  nucleus /* |delimiter| field in left and right noads}*/
#define displaymlist(x)  info(x+1) /* mlist to be used in display style}*/
#define textmlist(x)  link(x+1) /* mlist to be used in text style}*/
#define scriptmlist(x)  info(x+2) /* mlist to be used in script style}*/
#define scriptscriptmlist(x)  link(x+2) /* mlist to be used in scriptscript style}*/
#define crampedstyle(x)  2*(x / 2)+cramped /* cramp the style}*/
#define substyle(x)  2*(x / 4)+scriptstyle+cramped /* smaller and cramped}*/
#define supstyle(x)  2*(x / 4)+scriptstyle+(x % 2) /* smaller}*/
#define numstyle(x)  x+2-2*(x / 6) /* smaller unless already script-script}*/
#define denomstyle(x)  2*(x / 2)+cramped+2-2*(x / 6) /* smaller, cramped}*/
#define upart(x)  mem[x+heightoffset].int_ /* pointer to \<uj> token list}*/
#define vpart(x)  mem[x+depthoffset].int_ /* pointer to \<vj> token list}*/
#define extrainfo(x)  info(x+listoffset) /* info to remember during template}*/
#define fitness  subtype /* |veryloosefit..tightfit| on final line for this break}*/
#define breaknode  rlink /* pointer to the corresponding passive node}*/
#define linenumber  llink /* line that begins at this breakpoint}*/
#define totaldemerits(x)  mem[x+2].int_ /* the quantity that \TeX\ minimizes}*/
#define curbreak  rlink /* in passive node, points to position of this breakpoint}*/
#define prevbreak  llink /* points to passive node that should precede this one}*/
#define serial  info /* serial number for symbolic identification}*/
#define nextbreak  prevbreak /* new name for |prevbreak| after links are reversed}*/
#define trielink(x)  trie[x].rh /* ``downward'' link in a trie}*/
#define triechar(x)  trie[x].UU.U2.b1 /* character matched at this trie location}*/
#define trieop(x)  trie[x].UU.U2.b0 /* program for hyphenation at this trie location}*/
#define brokenptr(x)  link(x+1)
/* an insertion for this class will break here if anywhere}*/
#define trieback(x)  trie[x].UU.lh /* backward links in |trie| holes}*/
#define brokenins(x)  info(x+1) /* this insertion might break at |brokenptr|}*/
#define lastinsptr(x)  link(x+2) /* the most recent insertion for this |subtype|}*/
#define bestinsptr(x)  info(x+2) /* the optimum most recent insertion}*/
#define mathex(x)  fontinfo[x+parambase[famfnt(3+cursize)]].sc 
#define newhlist(x)  mem[nucleus(x)].int_ /* the translation of an mlist}*/
#define iflinefield(x)  mem[x+1].int_
#define whatlang(x)  link(x+1) /* language number, in the range |0..255|}*/
#define whatlhm(x)  type(x+1) /* minimum left fragment, in the range |1..63|}*/
#define whatrhm(x)  subtype(x+1) /* minimum right fragment, in the range |1..63|}*/
#define writetokens(x)    link(x+1) /* reference count of token list to write}*/
#define writestream(x)    info(x+1) /* stream number (0 to 17)}*/
#define openname(x)    link(x+1) /* string number of file name to open}*/
#define openarea(x)    info(x+2) /* string number of file area for |openname|}*/
#define openext(x)    link(x+2) /* string number of file extension for |openname|}*/

// #201: 使用封装好的函数
#if 0
#define karmafastdeleteglueref(x)                            \
    (gluerefcount(x) == 0 ? (freenode((x), gluespecsize), 0) \
                          : gluerefcount(x)--)
#endif

#define nodelistdisplay(x) (append_char('.'), shownodelist(x), flush_char())


// leave an input level, re-enter the old
#define popinput() (inputptr--, cur_input = inputstack[inputptr])
// backs up a simple token list
#define backlist(x) begintokenlist((x), BACKED_UP)
// inserts a simple token list
#define inslist(x) begintokenlist((x), INSERTED)

#define wrapup(x)                                       \
    if (curl < NON_CHAR) {                              \
        if (character(tail) == get_hyphenchar(mainf)) { \
            if (link(curq) > 0) insdisc = true;         \
        }                                               \
        if (ligaturepresent) {                          \
            packlig(x);                                 \
        }                                               \
        if (insdisc) {                                  \
            insdisc = false;                            \
            if (mode > 0) {                             \
                tailappend(newdisc());                  \
            }                                           \
        }                                               \
    }

#define adjustspacefactor()              \
    {                                    \
        mains = sfcode(curchr);          \
        if (mains == 1000) {             \
            spacefactor = 1000;          \
        } else if (mains < 1000) {       \
            if (mains > 0) {             \
                spacefactor = mains;     \
            }                            \
        } else if (spacefactor < 1000) { \
            spacefactor = 1000;          \
        } else                           \
            spacefactor = mains;         \
    }

#define packlig(x)                                      \
    { /*{the PARAMETER is either |rthit| or |false|} */ \
        mainp = newligature(mainf, curl, link(curq));   \
        if (lfthit) {                                   \
            subtype(mainp) = 2;                         \
            lfthit = false;                             \
        }                                               \
        if ((x) && (ligstack == null)) {                \
            subtype(mainp)++;                           \
            rthit = false;                              \
        }                                               \
        link(curq) = mainp;                             \
        tail = mainp;                                   \
        ligaturepresent = false;                        \
    }

#define kernbreak()                                                       \
    {                                                                     \
        if (!ischarnode(link(curp)) && (autobreaking)) {                  \
            if (type(link(curp)) == GLUE_NODE) trybreak(0, unhyphenated); \
        }                                                                 \
        actwidth += width(curp);                                          \
    }

#define appendcharnodetot(x)   \
    {                          \
        link(t) = get_avail(); \
        t = link(t);           \
        font(t) = hf;          \
        character(t) = x;      \
    }

#define setcurr()                 \
    {                             \
        if (j < n) {              \
            curr = qi(hu[j + 1]); \
        } else {                  \
            curr = bchar;         \
        }                         \
        if (hyf[j] & 1) {         \
            currh = hchar;        \
        } else {                  \
            currh = NON_CHAR;     \
        }                         \
    }

#define wraplig(x)                             \
    if (ligaturepresent) {                     \
        p = newligature(hf, curl, link(curq)); \
        if (lfthit) {                          \
            subtype(p) = 2;                    \
            lfthit = false;                    \
        }                                      \
        if (x)                                 \
            if (ligstack == 0) {               \
                (subtype(p))++;                \
                rthit = false;                 \
            }                                  \
        link(curq) = p;                        \
        t = p;                                 \
        ligaturepresent = false;               \
    }

#define popligstack()                              \
    {                                              \
        if (ligptr(ligstack) > null) {             \
            /* this is a charnode for |hu[j+1]| */ \
            link(t) = ligptr(ligstack);            \
            t = link(t);                           \
            j++;                                   \
        }                                          \
        p = ligstack;                              \
        ligstack = link(p);                        \
        freenode(p, smallnodesize);                \
        if (ligstack == null) {                    \
            setcurr();                             \
        } else {                                   \
            curr = character(ligstack);            \
        }                                          \
    } /* {if |ligstack| isn't |null| we have |currh=NON_CHAR|} */

#define advancemajortail()           \
    {                                \
        majortail = link(majortail); \
        rcount++;                    \
    }

#define advpast(x)                        \
    {                                     \
        if (subtype(x) == languagenode) { \
            curlang = whatlang(x);        \
            lhyf = whatlhm(x);            \
            rhyf = whatrhm(x);            \
        }                                 \
    }

#endif // TEXMAC_H