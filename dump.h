#pragma once
#ifndef INC_DUMP_H
//
#define INC_DUMP_H
#include "tex_types.h" // [type] Boolean

/** @addtogroup S1299x1329_P455x464
 * @{
 */
/// [#1303] lable _not_use_
// #define badfmt 6666
/** @}*/ // end group S1299x1329_P455x464


extern StrNumber format_ident;
extern FILE* fmtfile;

extern void dump_init();
extern void dump_init_once();
extern void storefmtfile(void);
extern Boolean load_fmt_file(void);

#endif /* INC_PAGE_H */