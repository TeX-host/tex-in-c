#pragma once
#ifndef INC_SHIP_OUT
#define INC_SHIP_OUT
#include "tex_types.h" // [type] Scaled, Pointer, GlueOrd, Boolean, QuarterWord
#include "tex_inc.h" // [type] NEW_STRING
#include "str.h"     // [type] StrNumber
#include "fonts.h"   // [type] InternalFontNumber


// [shipout], tex
// void vlist_out(
//     Pointer tempptr,
//     Scaled dvih,
//     Scaled dviv,
//     Scaled curh,
//     Scaled curv,
//     Scaled maxh,
//     Scaled maxv,
//     Scaled ruleht,
//     Scaled ruledp,
//     Scaled rulewd,
//     Integer curs,
//     Integer maxpush,
//     Integer lq,
//     Integer lr,
//     InternalFontNumber dvif,
//     Boolean doingleaders,
//     MemoryWord mem[],
//     Pointer himemmin
// );

void ship_out(
    HalfWord p,
    Scaled dvih,
    Scaled dviv,
    Scaled curh,
    Scaled curv,
    Scaled maxh,
    Scaled maxv,
    Scaled ruleht,
    Scaled ruledp,
    Scaled rulewd,
    Integer lq,
    Integer lr,
    Integer curs,
    Integer varused,
    Integer dynused,
    Integer maxpush,
    Integer totalpages,
    Integer deadcycles,
    Pointer tempptr,
    Pointer himemmin,
    Pointer lomemmax,
    MemoryWord mem[],
    MemoryWord eqtb[],
    char term_offset,
    char file_offset,
    StrNumber jobname,
    StrNumber outputfilename,
    InternalFontNumber dvif,
    Boolean doingleaders,
    enum Selector selector
);

#define shipout(p) ship_out( p, dvih, dviv, curh, curv, maxh, maxv, ruleht, ruledp, rulewd, lq, lr, curs, varused, dynused, maxpush, totalpages, deadcycles, tempptr, himemmin, lomemmax, mem, eqtb, term_offset, file_offset, jobname, outputfilename, dvif, doingleaders, selector )

#endif // INC_SHIP_OUT