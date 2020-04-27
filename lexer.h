#pragma once
#ifndef INC_LEXER_H
/// PART 21: INTRODUCTION TO THE SYNTACTIC ROUTINES.
/// lexer
#define INC_LEXER_H
#include <stdio.h>
#include "tex.h"

/** [#300]: InStateRecord(1344) .
 *
 * field:
 *  + QuarterWord(16): statefield, indexfield
 *  + HalfWord(32): startfield, locfield, limitfield, namefield
 *  + QuarterWord(16): tok_type
 *  + Pointer(64): tok_list, tok_loc, tok_name, tok_param
 */
typedef struct {
    QuarterWord statefield, indexfield;
    HalfWord startfield, locfield, limitfield, namefield;
    QuarterWord tok_type;
    Pointer tok_list, tok_loc, tok_name, tok_param;
} InStateRecord;


// [ #300~320: INPUT STACKS AND STATES ]

/// [p124#305]: scanner status
enum ScannerStatus {
    SKIPPING = 1, ///< when passing conditional text
    DEFINING,     ///< when reading a macro definition
    MATCHING,     ///< when reading macro arguments
    ALIGNING,     ///< when reading an alignment preamble
    ABSORBING     ///< when reading a balanced text
};

/// [p125#307]
#define TOKEN_LIST 0

/// [p125#307]
enum TokenType {
    PARAMETER,   ///< parameter
    U_TEMPLATE,  ///< `<u_j>` template
    V_TEMPLATE,  ///< `<v_j>` template
    BACKED_UP,   ///< text to be reread
    INSERTED,    ///< inserted texts
    MACRO,       ///< defined control sequences
    OUTPUT_TEXT, ///< output routines

    EVERY_PAR_TEXT,     ///< `\everypar`
    EVERY_MATH_TEXT,    ///< `\everymath`
    EVERY_DISPLAY_TEXT, ///< `\everydisplay`
    EVERY_HBOX_TEXT,    ///< `\everyhbox`
    EVERY_VBOX_TEXT,    ///< `\everyvbox`
    EVERY_JOB_TEXT,     ///< `\everyjob`
    EVERY_CR_TEXT,      ///< `\everycr`
    MARK_TEXT,          ///< `\topmark`, etc.
    WRITE_TEXT          ///< `\write`
};


// p121#302
#define LOC     cur_input.locfield
// [#302] current scanner state
//
// 1. `MID_LINE`    is the normal state.
// 2. `SKIP_BLANKS` is like MID_LINE, but blanks are ignored.
// 3. `NEW_LINE`    is the state at the beginning of a line.
//
// xref
//  + 赋值[16]:
//      323, 325, 328, 331, 343,
//      347[3], 349, 352, 353, 354[3],
//      483, 537,
//  + 相等判断[8]: 
//      == 325, 337, 390, 1335,
//      != 311, 312[2], 330, 341, 
//  + 文本引用[6]: 87, 300, 303, 307, 344, 346, 
//  
#define STATE   cur_input.statefield 
#define IINDEX  cur_input.indexfield // reference for buffer information
#define START   cur_input.startfield // starting position in |buffer|
#define LIMIT   cur_input.limitfield // end of current line in |buffer|
#define NAME    cur_input.namefield  // name of the current file
// #304
#define terminal_input  (NAME==0)       // are we reading from the terminal? 
#define curfile  (inputfile[IINDEX-1])  // the current |alphafile| variable
// p125#307
#define token_type   IINDEX // type of current token list
#define param_start  LIMIT  // base of macro parameters in |paramstack|

extern EightBits curcmd;
extern HalfWord curchr;
extern Pointer curcs;
extern HalfWord curtok;

extern InStateRecord inputstack[stacksize + 1];
extern UChar inputptr;
extern UChar maxinstack;
InStateRecord cur_input;
extern char inopen;
extern char openparens;
extern Integer line;
extern FILE* inputfile[MAX_IN_OPEN];
extern Integer linestack[MAX_IN_OPEN];
extern char scanner_status;
extern Pointer warning_index;
extern Pointer defref;
extern Pointer paramstack[paramsize + 1];
extern int paramptr;
extern Integer maxparamstack;
extern Integer align_state;
extern UChar baseptr;


extern void showtokenlist(Integer p, Integer q, Integer l);
extern void tokenshow(HalfWord p);

#endif // INC_LEXER_H