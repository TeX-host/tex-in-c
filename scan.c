#include "texmac.h" // [macro] link, 
#include "texfunc.h"
#include "lexer.h" // [var] curcmd, 
#include "tex.h"
#include "tex_inc.h"
#include "global.h"
#include "macros.h"
#include "expand.h"
#include "scan.h"

/** @defgroup S402x463 PART 26: BASIC SCANNING SUBROUTINES
 * [ #402~463 ]
 *
 * + scanleftbrace
 * + muerror
 * +
 */

void skip_spaces(void) { /*406:*/
    do {
        get_x_token();
    } while (curcmd == SPACER); /*:406*/
}

void skip_spaces_or_relax(void) { /*404:*/
    do {
        get_x_token();
    } while (curcmd == SPACER || curcmd == relax); /*:404*/
}

/// [ #402 : scan left brace ]

/*403:*/
void scanleftbrace(void) {
    skip_spaces_or_relax();
    if (curcmd == LEFT_BRACE) return;
    printnl(S(292));
    print(S(566));
    help4(S(567), S(568), S(569), S(570));
    backerror();
    curtok = leftbracetoken + '{';
    curcmd = LEFT_BRACE;
    curchr = '{';
    align_state++;
}
/*:403*/

/*405:*/
void scanoptionalequals(void) {
    skip_spaces();
    if (curtok != othertoken + '=') backinput();
}
/*:405*/

/*407:*/
Boolean scankeyword(StrNumber s) {
    Boolean Result;
#if 1
    Pointer p = get_avail();
    Pointer my_backup_head = p;
#else
    Pointer my_backup_head = backuphead;
    Pointer p = backuphead;
#endif
#if 0
  PoolPtr k;
#else
    int k;
    int k_e;
#endif
    link(p) = 0;
#if 0
  k = str_start[s];
  while (k < str_end(s) ) {
    get_x_token();
    if ((curcs == 0) & ((curchr == str_pool[k]) |
			(curchr == str_pool[k] - 'a' + 'A'))) {
#else
    k = 0;
    k_e = str_length(s);
    while (k < k_e) {
        int str_c = str_getc(s, k);
        get_x_token();
        if ((curcs == 0) &
            ((curchr == str_c) | (curchr == str_c - 'a' + 'A'))) {
#endif
    STORE_NEW_TOKEN(p, curtok);
    k++;
    continue;
}
else {
    if (curcmd == SPACER && p == my_backup_head) continue;
    backinput();
    if (p != my_backup_head) {
        backlist(link(my_backup_head));
    }
    Result = false;
    goto _Lexit;
}
}
flushlist(link(my_backup_head));
Result = true;
_Lexit :

    FREE_AVAIL(my_backup_head);

return Result;
}
/*:407*/

/*408:*/
void muerror(void) {
    printnl(S(292));
    print(S(571));
    help1(S(572));
    error();
}
/*:408*/

/*409:*/

/*433:*/
void scaneightbitint(void) {
    scanint();
    if ((unsigned long)curval <= 255) return;
    printnl(S(292));
    print(S(573));
    help2(S(574), S(575));
    int_error(curval);
    curval = 0;
}
/*:433*/

/*434:*/
void scancharnum(void) {
    scanint();
    if ((unsigned long)curval <= 255) return;
    printnl(S(292));
    print(S(576));
    help2(S(577), S(575));
    int_error(curval);
    curval = 0;
}
/*:434*/

/*435:*/
void scanfourbitint(void) {
    scanint();
    if ((unsigned long)curval <= 15) return;
    printnl(S(292));
    print(S(578));
    help2(S(579), S(575));
    int_error(curval);
    curval = 0;
}
/*:435*/

/*436:*/
void scanfifteenbitint(void) {
    scanint();
    if ((unsigned long)curval <= 32767) return;
    printnl(S(292));
    print(S(580));
    help2(S(581), S(575));
    int_error(curval);
    curval = 0;
}
/*:436*/

/*437:*/
void scantwentysevenbitint(void) {
    scanint();
    if ((unsigned long)curval <= 134217727L) return;
    printnl(S(292));
    print(S(582));
    help2(S(583), S(575));
    int_error(curval);
    curval = 0;
}
/*:437*/
