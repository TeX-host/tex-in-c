#pragma once
#ifndef TEXMAC_H
/// tex.c 里使用的宏
#define TEXMAC_H
#include "mem.h" // [macro] link


// ?
#undef labs



#define exteninfo(f, q) (fontinfo[extenbase[(f)] + rembyte(q)].qqqq)

/** @addtogroup S25x37_P13x18
 * @{
 */
/// [p14#27]  open a file for output.
#define wopenout a_open_out
// [p17#33] 无需打开输出流，直接使用 stdout
// #define topenin() (termin = stdin) // 33, 37
// #define topenout() (termout = stdout) // 33, 1332
/** @}*/ // end group S25x37_P13x18


/// [p43#112]
#define qi(x) (x)
#define qo(x) (x)




// [p69#201]: 使用封装好的函数
#if 0
#define karmafastdeleteglueref(x)                            \
    (gluerefcount(x) == 0 ? (free_node((x), gluespecsize), 0) \
                          : gluerefcount(x)--)
#endif

/// [p78#214]
#define tailappend(x) (link(tail) = (x), tail = link(tail))


/** @addtogroup S268x288_P109x114
 * @{
 */

/// [p109#268] classifies a |savestack| entry
#define savetype(x)     savestack[x].hh.UU.U2.b0
/// [p109#268] saved level for regions 5 and 6, or group code
#define savelevel(x)    savestack[x].hh.UU.U2.b1
/// [p109#268] |eqtb| location or |savestack| location
#define saveindex(x)    savestack[x].hh.rh

/// [p111#274]
#define saved(x) savestack[saveptr + x].int_
/** @}*/ // end group S268x288_P109x114


// [p131#322] leave an input level, re-enter the old
#define popinput() (inputptr--, cur_input = inputstack[inputptr])
// [p131#323] backs up a simple token list
#define backlist(x) begintokenlist((x), BACKED_UP)
// [p131#323] inserts a simple token list
#define inslist(x) begintokenlist((x), INSERTED)

/// [p138#352]
#define ishex(x) \
    ((((x) >= '0') && ((x) <= '9')) || (((x) >= 'a') && ((x) <= 'f')))

/// [p148#382]
#define topmark curmark[topmarkcode - topmarkcode]
#define firstmark curmark[firstmarkcode - topmarkcode]
#define botmark curmark[botmarkcode - topmarkcode]
#define splitfirstmark curmark[splitfirstmarkcode - topmarkcode]
#define splitbotmark curmark[splitbotmarkcode - topmarkcode]

/// [p182#489]
#define iflinefield(x) mem[x + 1].int_


/** @addtogroup S539x582_P196x213
 * @{
 */

/// [p199#545]
#define stopflag  qi(128) /* value indicating `\.{STOP}' in a lig/kern program}*/
#define kernflag  qi(128) /* op code for a kern step}*/
#define skipbyte(x) x.b0
#define nextchar(x) x.b1

/// [p199#564]
#define exttop(x) x.b0 /* |top| piece in a recipe}*/
#define extmid(x) x.b1 /* |mid| piece in a recipe}*/
#define extbot(x) x.b2 /* |bot| piece in a recipe}*/
#define extrep(x) x.b3 /* |rep| piece in a recipe}*/
/** @}*/ // end group S539x582_P196x213


/** @addtogroup S203x206_P71x72
 * @{
 */

/// [#203] new reference to a token list.
#define addtokenref(x)  (tokenrefcount(x)++)
/// [#203] new reference to a glue spec.
#define addglueref(x)   (gluerefcount(x)++)
/** @}*/ // end group S203x206_P71x72



/*
/// [p203#554]
#define charexists(x)  ((x).b0>MIN_QUARTER_WORD)
#define chartag(x)  ((qo((x).b2)) % 4)
*/

/// [p203#554]
#define heightdepth(x) qo(x.b1)

/// [p204#557] beginning of lig/kern program
#define ligkernstart(x, y) (ligkernbase[(x)] + rembyte(y))
/// [p204#557]
#define ligkernrestart(x, y) \
    (ligkernbase[(x)] + opbyte(y) * 256 + rembyte(y) - kernbaseoffset + 32768L)
// #define lig_kern_start(x)  (lig_kern_base[x]+rem_byte)

/// [p204#558]
#define param(x, y) (fontinfo[(x) + parambase[y]].sc)
#define slant(x)  param(SLANT_CODE,x) /* slant to the right, per unit distance upward}*/
#define space(x)  param(SPACE_CODE,x) /* NORMAL space between words}*/
#define spacestretch(x)  param(SPACE_STRETCH_CODE,x) /* stretch between words}*/
#define spaceshrink(x)  param(SPACE_SHRINK_CODE,x) /* shrink between words}*/
#define xheight(x)  param(X_HEIGHT_CODE,x) /* one ex}*/
#define quad(x)  param(QUAD_CODE,x) /* one em}*/
#define extraspace(x)  param(EXTRA_SPACE_CODE,x) /* additional space at end of sentence}*/



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

/// [p223#605]
// #define location(x)  mem[x+2].int_ /* \.{DVI} byte number for a movement command}*/


/// [p230#829]
#define copytocuractive(x) (curactivewidth[(x)-1] = activewidth[(x)-1])
/// [p231#625]
#define vetglue(x)                    \
    (gluetemp = (x),                  \
     ((gluetemp > (1000000000.0))     \
          ? (gluetemp = 1000000000.0) \
          : ((gluetemp < -1000000000.0) ? (gluetemp = -1000000000.0) : 0)))


/** @addtogroup S680x698_P249x257
 * @{
 */

#if 1
/// [p250#681]
#define nucleus(x)  ((x)+CHAR_NODE_SIZE) /* the |nucleus| field of a noad}*/
#define supscr(x)  (nucleus(x)+CHAR_NODE_SIZE) /* the |supscr| field of a noad}*/
#define subscr(x)  (supscr(x)+CHAR_NODE_SIZE) /* the |subscr| field of a noad}*/
/// [p252#683]
#define leftdelimiter(x)  (subscr(x)+CHAR_NODE_SIZE) /* first delimiter field of a noad}*/
#define rightdelimiter(x)  (leftdelimiter(x)+CHAR_NODE_SIZE) /* second delimiter field of a fraction noad}*/
/// [p253#687]
#define accentchr(x)  (subscr(x)+CHAR_NODE_SIZE) /* the |accentchr| field of an accent noad}*/
#else
#define nucleus(x)  ((x)+1) /* the |nucleus| field of a noad}*/
#define supscr(x)  ((x)+2) /* the |supscr| field of a noad}*/
#define subscr(x)  ((x)+7) /* the |subscr| field of a noad}*/
#define leftdelimiter(x)  ((x)+4) /* first delimiter field of a noad}*/
#define rightdelimiter(x)  ((x)+5) /* second delimiter field of a fraction noad}*/
#define accentchr(x)  ((x)+4) /* the |accentchr| field of an accent noad}*/
#endif

/// [p250#681] a |halfword| in |mem|.
#define mathtype  link
#define fam(x)  type(x)
/// [p252#683]
#define smallfam(x)  mem[x].qqqq.b0 /* |fam| for ``small'' delimiter}*/
#define smallchar(x)  mem[x].qqqq.b1 /* |character| for ``small'' delimiter}*/
#define largefam(x)  mem[x].qqqq.b2 /* |fam| for ``large'' delimiter}*/
#define largechar(x) mem[x].qqqq.b3 /* |character| for ``large'' delimiter}*/
#define thickness(x) (mem[nucleus(x)-MEM_MIN].sc) /* |thickness| field in a fraction noad}*/
#define numerator  supscr /* |numerator| field in a fraction noad}*/
#define denominator  subscr /* |denominator| field in a fraction noad}*/
/// [p253#687]
#define delimiter  nucleus /* |delimiter| field in left and right noads}*/
/// [p253#687]
#define scriptsallowed(x)  ((type(x)>=ordnoad)&&(type(x)<leftnoad))
/// [p245#668]
#define vpack(x, y, z)                                                  \
    vpackage((x), (y), (z), MAX_DIMEN) /* special case of unconstrained \
                                          depth}*/
/// [p254#689]
#define displaymlist(x)  info(x+1) /* mlist to be used in display style}*/
#define textmlist(x)  link(x+1) /* mlist to be used in text style}*/
#define scriptmlist(x)  info(x+2) /* mlist to be used in script style}*/
#define scriptscriptmlist(x)  link(x+2) /* mlist to be used in scriptscript style}*/
/** @}*/ // end group S680x698_P249x257


/// [p258#700]
#define mathsy(x, y) (fontinfo[(x) + parambase[fam_fnt(2 + (y))]].sc)
#define mathxheight(x) mathsy(5, x) /* height of `\.x'}*/
#define mathquad(x) mathsy(6, x)    /* \.{18mu}}*/
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

/// [p258#701]
#define mathex(x) fontinfo[x + parambase[fam_fnt(3 + cursize)]].sc
#define defaultrulethickness  mathex(8) /* thickness of \.{\\over} bars}*/
#define bigopspacing1  mathex(9) /* minimum clearance above a displayed op}*/
#define bigopspacing2  mathex(10) /* minimum clearance below a displayed op}*/
#define bigopspacing3  mathex(11) /* minimum baselineskip above displayed op}*/
#define bigopspacing4  mathex(12) /* minimum baselineskip below displayed op}*/
#define bigopspacing5  mathex(13) /* padding above and below displayed limits}*/

/// [p259#702]
#define crampedstyle(x)  2*(x / 2)+cramped /* cramp the style}*/
#define substyle(x)  2*(x / 4)+scriptstyle+cramped /* smaller and cramped}*/
#define supstyle(x)  2*(x / 4)+scriptstyle+(x % 2) /* smaller}*/
#define numstyle(x)  x+2-2*(x / 6) /* smaller unless already script-script}*/
#define denomstyle(x)  2*(x / 2)+cramped+2-2*(x / 6) /* smaller, cramped}*/
/// [p267#725]
#define newhlist(x) mem[nucleus(x)].int_ /* the translation of an mlist}*/


/// [p286#769]
#define upart(x)  mem[x+heightoffset].int_ /* pointer to \<uj> token list}*/
#define vpart(x)  mem[x+depthoffset].int_ /* pointer to \<vj> token list}*/
#define extrainfo(x)  info(x+listoffset) /* info to remember during template}*/

/// [p287#770]
#define preamble link(alignhead) /* the current preamble list}*/

/// [p304#819]
#define fitness  subtype /* |veryloosefit..tightfit| on final line for this break}*/
#define breaknode  rlink /* pointer to the corresponding passive node}*/
#define linenumber  llink /* line that begins at this breakpoint}*/
#define totaldemerits(x)  mem[x+2].int_ /* the quantity that \TeX\ minimizes}*/
/// [p304#821]
#define curbreak  rlink /* in passive node, points to position of this breakpoint}*/
#define prevbreak  llink /* points to passive node that should precede this one}*/
#define serial  info /* serial number for symbolic identification}*/

/// [p306#825]
#define checkshrinkage(x) \
    ((shrinkorder(x) != NORMAL) && (shrink(x) != 0) ? (x) = finiteshrink(x) : 0)


/// [p309#832]
#define updatewidth(x) (curactivewidth[(x)-1] += mem[r + (x)].sc)
/// [p310#837]
#define setbreakwidthtobackground(x) (breakwidth[(x)-1] = background[(x)-1])
/// [p312#943]
#define converttobreakwidth(x) \
    (mem[prevr + (x)].sc += -curactivewidth[(x)-1] + breakwidth[(x)-1])
#define storebreakwidth(x) (activewidth[(x)-1] = breakwidth[(x)-1])
#define newdeltatobreakwidth(x) \
    (mem[q + (x)].sc = breakwidth[(x)-1] - curactivewidth[(x)-1])
/// [p312#944]
#define newdeltafrombreakwidth(x) \
    (mem[q + (x)].sc = curactivewidth[(x)-1] - breakwidth[(x)-1])

/// [p318#860]
#define combinetwodeltas(x) \
    (mem[prevr + (x)].sc = mem[prevr + (x)].sc + mem[r + (x)].sc)
#define downdatewidth(x) (curactivewidth[(x)-1] -= mem[prevr + (x)].sc)
/// [p318#861]
#define updateactive(x) (activewidth[(x)-1] += mem[r + (x)].sc)
/// [p320#864]
#define storebackground(x) (activewidth[(x)-1] = background[(x)-1])

/// [p321#866]
#define kernbreak()                                                       \
    {                                                                     \
        if (!ischarnode(link(curp)) && (autobreaking)) {                  \
            if (type(link(curp)) == GLUE_NODE) trybreak(0, unhyphenated); \
        }                                                                 \
        actwidth += width(curp);                                          \
    }

/// [p325#877]
#define nextbreak  prevbreak /* new name for |prevbreak| after links are reversed}*/


/// [p337#908]
#define appendcharnodetot(x)   \
    {                          \
        link(t) = get_avail(); \
        t = link(t);           \
        font(t) = hf;          \
        character(t) = x;      \
    }

/// [p337#908]
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

/// [p339#910]
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

/// [p339#910] if |ligstack| isn't |null| we have |currh=NON_CHAR|
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
        free_node(p, smallnodesize);                \
        if (ligstack == null) {                    \
            setcurr();                             \
        } else {                                   \
            curr = character(ligstack);            \
        }                                          \
    }

/// [p341#914]
#define advancemajortail()           \
    {                                \
        majortail = link(majortail); \
        rcount++;                    \
    }

/// [p344#921] `downward` link in a trie
#define trielink(x)     trie[x].rh
/// [p344#921] character matched at this trie location
#define triechar(x)     trie[x].UU.U2.b1
/// [p344#921] program for hyphenation at this trie location
#define trieop(x)       trie[x].UU.U2.b0

/// [p347#934]
#define setcurlang()     \
    ((language <= 0)     \
         ? (curlang = 0) \
         : ((language > 255) ? (curlang = 0) : (curlang = language)))

/// [p352#947]
#define trieroot (triel[0]) // root of the linked trie

/// [p353#950] backward links in |trie| holes
#define trieback(x)     trie[x].UU.lh

// [#360]: \endlinechar 行终止符无效，不添加换行符
#define end_line_char_inactive ((end_line_char < 0) || (end_line_char > 255))

/// [p361#970] initialize the height to zero.
#define setheightzero(x) (activeheight[(x)-1] = 0)
/// [p361#970]
#define activeheight activewidth    /*new name for the six distance variables*/
#define curheight (activeheight[0]) /*the natural height*/


/// [p367#981] an insertion for this class will break here if anywhere
#define brokenptr(x)    link(x+1)
#define brokenins(x)    info(x+1) /* this insertion might break at |brokenptr|}*/
#define lastinsptr(x)   link(x+2) /* the most recent insertion for this |subtype|}*/
#define bestinsptr(x)   info(x+2) /* the optimum most recent insertion}*/

/// [p369#987]
#define setpagesofarzero(x) (pagesofar[(x)] = 0)

/// [p368#982]
#define pagegoal \
    (pagesofar[0]) /*desired height of information on page being built*/
#define pagetotal (pagesofar[1])  /*height of the current page*/
#define pageshrink (pagesofar[6]) /*shrinkability of the current page*/
#define pagedepth (pagesofar[7])  /*depth of the current page*/

/// [p371#995]
#define contribtail (nest[0].tailfield) /*tail of the contribution list*/


/// [p386#1034]
#define adjustspacefactor()              \
    {                                    \
        mains = sf_code(curchr);          \
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

/// [p387#1035] the PARAMETER is either |rthit| or |false|
#define packlig(x)                                    \
    {                                                 \
        mainp = newligature(mainf, curl, link(curq)); \
        if (lfthit) {                                 \
            subtype(mainp) = 2;                       \
            lfthit = false;                           \
        }                                             \
        if ((x) && (ligstack == null)) {              \
            subtype(mainp)++;                         \
            rthit = false;                            \
        }                                             \
        link(curq) = mainp;                           \
        tail = mainp;                                 \
        ligaturepresent = false;                      \
    }

/// [p387#1035]
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


/// [p420#1151]
#define faminrange ((curfam >= 0) && (curfam < 16))

/// [p437#1214]
/// 1218, 1241
#define define(x, y, z) \
    ((a >= 4) ? geqdefine((x), (y), (z)) : eqdefine((x), (y), (z)))
#define worddefine(x, y) \
    ((a >= 4) ? geqworddefine((x), (y)) : eqworddefine((x), (y)))


/// [p471#1362]
#define advpast(x)                        \
    {                                     \
        if (subtype(x) == languagenode) { \
            curlang = whatlang(x);        \
            lhyf = whatlhm(x);            \
            rhyf = whatrhm(x);            \
        }                                 \
    }

/// [p472#1341]
#define whatlang(x)  link(x+1) /* language number, in the range |0..255|}*/
#define whatlhm(x)  type(x+1) /* minimum left fragment, in the range |1..63|}*/
#define whatrhm(x)  subtype(x+1) /* minimum right fragment, in the range |1..63|}*/
#define writetokens(x)    link(x+1) /* reference count of token list to write}*/
#define writestream(x)    info(x+1) /* stream number (0 to 17)}*/
#define openname(x)    link(x+1) /* string number of file name to open}*/
#define openarea(x)    info(x+2) /* string number of file area for |openname|}*/
#define openext(x)    link(x+2) /* string number of file extension for |openname|}*/


// #866
#define actwidth activewidth[0] // length from first active node to current node


#endif // TEXMAC_H