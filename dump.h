#pragma once
#ifndef INC_DUMP_H
/// [#1299] Dumping and undumping the tables.
#define INC_DUMP_H
#include <stdio.h>     // FILE, fread, fwrite
#include "global.h"    // [type] Boolean; // [macor] pput. pget

/** @addtogroup S1299x1329_P455x464
 * @{
 */
/// [#1303] lable _not_use_
// #define badfmt 6666

/// [#1305]
// fonts, str, tex
#define pget(x) fread(&x, 8, 1, fmtfile)
#define pput(x) fwrite(&x, 8, 1, fmtfile)
/** @}*/ // end group S1299x1329_P455x464


extern StrNumber format_ident;
extern FILE* fmtfile;

extern void dump_init();
extern void dump_init_once();
extern void store_fmt_file(void);
extern Boolean load_fmt_file(void);

#endif /* INC_PAGE_H */