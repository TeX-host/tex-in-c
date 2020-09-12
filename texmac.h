#pragma once
#ifndef TEXMAC_H
/// tex.c 里使用的宏
#define TEXMAC_H
#include "mem.h" // [macro] link


#define exteninfo(f, q) (fontinfo[extenbase[(f)] + rembyte(q)].qqqq)


/// [p43#112]
#define qi(x) (x)
#define qo(x) (x)




// [p69#201]: 使用封装好的函数
#if 0
#define karmafastdeleteglueref(x)                            \
    (gluerefcount(x) == 0 ? (free_node((x), gluespecsize), 0) \
                          : gluerefcount(x)--)
#endif


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


/// [p223#605]
// #define location(x)  mem[x+2].int_ /* \.{DVI} byte number for a movement command}*/


/// [p231#625]
#define vetglue(x)                    \
    (gluetemp = (x),                  \
     ((gluetemp > (1000000000.0))     \
          ? (gluetemp = 1000000000.0) \
          : ((gluetemp < -1000000000.0) ? (gluetemp = -1000000000.0) : 0)))


/// [p267#725]
#define newhlist(x) mem[nucleus(x)].int_ /* the translation of an mlist}*/


// [#360]: \endlinechar 行终止符无效，不添加换行符
#define end_line_char_inactive ((end_line_char < 0) || (end_line_char > 255))

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