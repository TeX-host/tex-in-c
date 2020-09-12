#pragma once
#ifndef INC_HASH_H
//
#define INC_HASH_H
#include "charset.h" // [type] ASCIICode
#include "tex.h"
#include "str.h"
#include "fonts.h" // InternalFontNumber
#include "eqtb.h"  // UNDEFINED_CONTROL_SEQUENCE, HASH_BASE

extern TwoHalves hash[UNDEFINED_CONTROL_SEQUENCE - HASH_BASE];
extern Pointer hash_used;
extern Integer cs_count;

extern void hash_var_init();
extern void hash_init_once();
extern StrNumber get_text(size_t x);
extern void set_text(size_t x, StrNumber s);
extern StrNumber fontidtext(InternalFontNumber x);
extern void set_fontidtext(InternalFontNumber x, StrNumber s);
extern HalfWord idlookup_p(ASCIICode buf_ptr[], Integer len, Boolean no_new_cs);
extern void print_cs(long p);
extern void sprint_cs(Pointer p);
extern void primitive(StrNumber s, QuarterWord c, HalfWord o);

#endif /* INC_HASH_H */