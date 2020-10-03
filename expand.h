#pragma once
#ifndef INC_EXPAND_H
/** expand .
 *
 * + @ref S366x401_P144x154 "PART 25: EXPANDING THE NEXT TOKEN"
 * + @ref S464x486_P174x180 "PART 27: BUILDING TOKEN LISTS"
 * + @ref S487x510_P181x187 "PART 28: CONDITIONAL PROCESSING"
 */
#define INC_EXPAND_H
#include <stdio.h> // FILE
#include "tex_types.h"

/** @addtogroup S366x401_P144x154
 * @{
 */
/// [p145#371]
#define STORE_NEW_TOKEN(p, x) \
    do {                      \
        int q = get_avail();  \
        link(p) = q;          \
        info(q) = (x);        \
        p = q;                \
    } while (0)
// end #define STORE_NEW_TOKEN(p, x)

/// [p145#371]
#define FAST_STORE_NEW_TOKEN(p, x) \
    do {                           \
        int q;                     \
        FAST_GET_AVAIL(q);         \
        link(p) = q;               \
        info(q) = (x);             \
        p = q;                     \
    } while (0)
// end #define FAST_STORE_NEW_TOKEN(p, x)
/// [p148#382]
#define topmarkcode         0
#define firstmarkcode       1
#define botmarkcode         2
#define splitfirstmarkcode  3
#define splitbotmarkcode    4
#define topmark             curmark[topmarkcode - topmarkcode]
#define firstmark           curmark[firstmarkcode - topmarkcode]
#define botmark             curmark[botmarkcode - topmarkcode]
#define splitfirstmark      curmark[splitfirstmarkcode - topmarkcode]
#define splitbotmark        curmark[splitbotmarkcode - topmarkcode]
/** @} end group S366x401_P144x154 */
/** @addtogroup S464x486_P174x180
 * @{
 */
// [p179#480]
#define closed          2
#define justopen        1
/** @} end group S464x486_P174x180 */
/** @addtogroup S487x510_P181x187
 * @{
 */

/// [p181#487]: Conditional processing
enum ConditionPrimitives {
    IF_CHAR_CODE,  ///< `\if`
    IF_CAT_CODE,   ///< `\ifcat`
    IF_INT_CODE,   ///< `\ifnum`
    IF_DIM_CODE,   ///< `\ifdim`
    IF_ODD_CODE,   ///< `\ifodd`
    IF_VMODE_CODE, ///< `\ifvmode`
    IF_HMODE_CODE, ///< `\ifhmode`
    IF_MMODE_CODE, ///< `\ifmmode`
    IF_INNER_CODE, ///< `\ifinner`
    IF_VOID_CODE,  ///< `\ifvoid`
    IF_HBOX_CODE,  ///< `\ifhbox`
    IF_VBOX_CODE,  ///< `\ifvbox`
    IF_X_CODE,     ///< `\ifx`
    IF_EOF_CODE,   ///< `\ifeof`
    IF_TRUE_CODE,  ///< `\iftrue`
    IF_FALSE_CODE, ///< `\iffalse`
    IF_CASE_CODE   ///< `\ifcase`
}; // [p181#487]: enum ConditionPrimitives

/// number of words in stack entry for conditionals.
#define ifnodesize 2

/// [#489]
enum IfCode {
    ifcode = 1, ///< code for `\if`... being evaluated
    ficode,     ///< code for `\fi`
    elsecode,   ///< code for `\else`
    orcode,     ///< code for `\or`
}; // [#489] enum IfCode

/// [p182#489]
#define iflinefield(x)  mem[x + 1].int_
/** @} */ // end group S487x510_P181x187

extern FILE* readfile[16];
extern char readopen[17];

extern Pointer condptr;
// init only
extern char iflimit;
// init + final cleanup
extern SmallNumber curif;
extern Integer ifline;
// lexer
extern Integer skipline;
extern char longstate;
extern Pointer curmark[splitbotmarkcode - topmarkcode + 1];

extern HalfWord thetoks(void);
extern HalfWord scantoks(Boolean macrodef, Boolean xpand);
extern void readtoks(long n, HalfWord r);
extern void expand_init();
extern void build_token_init();
extern void cond_process_init();
extern void expand(void);
extern void get_x_token(void);
extern void xtoken(void);


#endif // INC_EXPAND_H