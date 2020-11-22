#pragma once
#ifndef INC_DUMP_H
/// [#1299] Dumping and undumping the tables.
#define INC_DUMP_H
#include <stdio.h>     // FILE, fread, fwrite
#include "global.h"    // [type] Boolean;


extern StrNumber format_ident;
extern FILE* fmt_file;

extern void dump_init();
extern void dump_init_once();
extern void store_fmt_file(void);
extern Boolean load_fmt_file(void);

extern void dump_wd(MemoryWord wd);
extern void dump_int(Integer int_);
extern void dump_hh(TwoHalves hh);
extern void dump_qqqq(FourQuarters qqqq);
extern MemoryWord undump_wd();
extern Integer undump_int();
extern TwoHalves undump_hh();
extern FourQuarters undump_qqqq();

#endif /* INC_PAGE_H */