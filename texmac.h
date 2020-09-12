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


// #866
#define actwidth activewidth[0] // length from first active node to current node


#endif // TEXMAC_H