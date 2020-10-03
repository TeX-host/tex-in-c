#pragma once
#ifndef INC_ALGIN_H
/// 
#define INC_ALGIN_H
#include "global.h"
    // [type] Pointer, Boolean
#include "mem.h"       // [var] mem; [macro] info, link
#include "box.h" // [const] heightoffset, depthoffset, listoffset, alignhead
#include "lexer.h"     // [const] CS_TOKEN_FLAG
#include "eqtb.h"      // [const] FROZEN_END_TEMPLATE


/** @addtogroup S768x812_P285x301
 * @{
 */
/// [p286#769] pointer to <uj> token list.
#define upart(x)        mem[x + heightoffset].int_
/// [p286#769] pointer to <vj> token list.
#define vpart(x)        mem[x + depthoffset].int_
/// [p286#769] info to remember during template.
#define extrainfo(x)    info(x + listoffset)

/// [p287#770] the current preamble list.
#define preamble        link(alignhead)

// [p287#770] number of mem words to save alignment states
#define alignstacknodesize  5
// [p289#780]
#define spancode            256
#define crcode              257
#define crcrcode            (crcode + 1)
#define endtemplatetoken    (CS_TOKEN_FLAG + FROZEN_END_TEMPLATE)
// [p294#797]
#define spannodesize        2
/** @}*/ // end group S768x812_P285x301


extern Pointer curalign;

extern void align_init();
extern void initalign();
extern Boolean fincol();
extern void finrow();
extern void alignpeek();

#endif /* INC_ALGIN_H */