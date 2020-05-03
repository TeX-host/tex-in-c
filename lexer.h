#pragma once
#ifndef INC_LEXER_H
/** PART 20~24: lexer.
 *
 *  + @ref S289x296_P115x118 "PART 20: TOKEN LISTS"
 *  + @ref S297x299_P119x120 "PART 21: INTRODUCTION TO THE SYNTACTIC
 * ROUTINES"
 *  + @ref S300x320_P121x130 "PART 22: INPUT STACKS AND STATES"
 *  + @ref S321x331_P131x133 "PART 23: MAINTAINING THE INPUT STACKS"
 *  + @ref S332x365_P134x143 "PART 24: GETTING THE NEXT TOKEN"
 */
#define INC_LEXER_H
#include <stdio.h>
#include "tex.h" // [macro] dwa_do_8


/** @addtogroup S25x37_P13x18
 * @{
 */
/// [p18#36]  location of first unread character in #buffer.
#define LOC     cur_input.locfield
/** @}*/ // end group S25x37_P13x18


/** @addtogroup S289x296_P115x118
 *  @{
 */

/// [#298] amount added to the #eqtb location in a token that
///     stands for a control sequence.
/// 
///  is a multiple of 256, less 1.
/// [4096 - 1 => dwa_do_8 * 16 - 1]
#define CS_TOKEN_FLAG   (dwa_do_8 * 16 - 1)

#define leftbracetoken  (dwa_do_8 * LEFT_BRACE)
#define leftbracelimit  (dwa_do_8 * (LEFT_BRACE + 1))
#define rightbracetoken (dwa_do_8 * RIGHT_BRACE)
#define rightbracelimit (dwa_do_8 * (RIGHT_BRACE + 1))
#define mathshifttoken  (dwa_do_8 * MATH_SHIFT)
#define tabtoken        (dwa_do_8 * TAB_MARK)
#define outparamtoken   (dwa_do_8 * OUT_PARAM)
#define spacetoken      (dwa_do_8 * SPACER + ' ')
#define lettertoken     (dwa_do_8 * LETTER)
#define othertoken      (dwa_do_8 * OTHER_CHAR)
#define matchtoken      (dwa_do_8 * MATCH)
#define endmatchtoken   (dwa_do_8 * END_MATCH)
/** @}*/ // end group S289x296_P115x118


/** @addtogroup S300x320_P121x130
 *  @{
 */

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

/// [p124#305]: scanner status
enum ScannerStatus {
    SKIPPING = 1, ///< when passing conditional text
    DEFINING,     ///< when reading a macro definition
    MATCHING,     ///< when reading macro arguments
    ALIGNING,     ///< when reading an alignment preamble
    ABSORBING     ///< when reading a balanced text
};

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

/// [p125#307]
#define TOKEN_LIST 0

/** [#302] current scanner state.
 *
 * 1. `MID_LINE`    is the normal state.
 * 2. `SKIP_BLANKS` is like MID_LINE, but blanks are ignored.
 * 3. `NEW_LINE`    is the state at the beginning of a line.
 *
 * xref
 *  + 赋值[16]:
 *      323, 325, 328, 331, 343,
 *      347[3], 349, 352, 353, 354[3],
 *      483, 537,
 *  + 相等判断[8]:
 *      `==`: 325, 337, 390, 1335,
 *      `!=`: 311, 312[2], 330, 341,
 *  + 文本引用[6]: 87, 300, 303, 307, 344, 346,
 */
#define STATE   cur_input.statefield 
#define IINDEX  cur_input.indexfield ///< reference for #buffer information
#define START   cur_input.startfield ///< starting position in #buffer
#define LIMIT   cur_input.limitfield ///< end of current line in #buffer
#define NAME    cur_input.namefield  ///< name of the current file.

/// [#303] `state` code when scanning a line of characters.
#define MID_LINE    1
/// [#303] `state` code when ignoring blanks.
#define SKIP_BLANKS (MAX_CHAR_CODE + 2)
/// [#303] `state` code at start of line.
#define NEW_LINE    (MAX_CHAR_CODE + MAX_CHAR_CODE + 3)

/// [#304] are we reading from the terminal?
#define terminal_input (NAME == 0) 
/// [#304] the current alphafile variable.
#define curfile     (inputfile[IINDEX - 1])

/// [p125#307] type of current token list.
#define token_type  IINDEX 
/// [p125#307] base of macro parameters in paramstack.
#define param_start LIMIT
/** @}*/ // end group S300x320_P121x130


extern EightBits curcmd;
extern HalfWord curchr;
extern Pointer curcs;
extern HalfWord curtok;

extern InStateRecord inputstack[STACK_SIZE + 1];
extern UChar inputptr;
extern InStateRecord cur_input;

extern char openparens;
extern Integer line;
extern FILE* inputfile[MAX_IN_OPEN];
extern char scanner_status;
extern Pointer warning_index;
extern Pointer defref;
extern Pointer paramstack[PARAM_SIZE + 1];
extern int paramptr;
extern Integer maxparamstack;
extern Integer align_state;
extern UChar baseptr;


extern void showtokenlist(Integer p, Integer q, Integer l);
extern void tokenshow(HalfWord p);
extern void printmeaning(int cur_chr, int cur_cmd);
extern void showcurcmdchr(void);
extern void runaway(void);
extern void showcontext(void);
extern Boolean init_lexer(void);

extern void begintokenlist(HalfWord p, QuarterWord t);
extern void endtokenlist(void);
extern void backinput(void);
extern void backerror(void);
extern void inserror(void);
extern void beginfilereading(void);
extern void endfilereading(void);
extern void clearforerrorprompt(void);

extern void getnext(void);
extern void firm_up_the_line(void);
extern void gettoken(void);

extern int pack_tok(int cs, int cmd, int chr);
extern UChar get_maxinstack();

#endif // INC_LEXER_H