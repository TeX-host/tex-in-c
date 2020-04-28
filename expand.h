#pragma once
#ifndef INC_EXPAND_H
/** expand .
 * 
 */
#define INC_EXPAND_H


/** @addtogroup S487x510
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
};

/// number of words in stack entry for conditionals
#define ifnodesize 2

/// [#489]
enum IfCode {
    ifcode = 1, ///< code for `\if`... being evaluated
    ficode,     ///< code for `\fi`
    elsecode,   ///< code for `\else`
    orcode,     ///< code for `\or`
};              // [#489] enum IfCode

/* @} */ // end group S487x510


extern Pointer condptr;
// init only
extern char iflimit;
// init + final cleanup
extern SmallNumber curif;
extern Integer ifline;
// lexer
extern Integer skipline;
extern char longstate;

extern void expand(void);
extern void get_x_token(void);
extern void xtoken(void);


#endif // INC_EXPAND_H