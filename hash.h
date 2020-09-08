#pragma once
#ifndef INC_HASH_H
//
#define INC_HASH_H
#include "tex.h"
#include "str.h"


/** @addtogroup S256x267_P102x108
 * @{
 */
/// [p102#256] link for coalesced lists
#define next(x)         hash[x - HASH_BASE].UU.lh
/// [p102#256] string number for control sequence name
#define text(x)         hash[x - HASH_BASE].rh
/// [p102#256] test if all positions are occupied
#define hash_is_full    (hash_used == HASH_BASE)
/** @}*/ // end group S256x267_P102x108


extern TwoHalves hash[UNDEFINED_CONTROL_SEQUENCE - HASH_BASE];
extern Pointer hash_used;
extern Integer cs_count;


extern HalfWord idlookup_p(ASCIICode buf_ptr[], Integer len, Boolean no_new_cs);
extern void print_cs(long p);
extern void sprint_cs(Pointer p);
extern void primitive(StrNumber s, QuarterWord c, HalfWord o);

#endif /* INC_HASH_H */