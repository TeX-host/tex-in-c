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
#include <stdio.h> // FILE
#include "tex.h" // [macro] dwa_do_8
#include "tex_types.h"
#include "tex_constant.h" // [const] BUF_SIZE, HALF_ERROR_LINE, STACK_SIZE, MAX_IN_OPEN, PARAM_SIZE, NEST_SIZE, EMPTY, MIN_QUARTER_WORD
// [enum] CatCode

/** @addtogroup S25x37_P13x18
 * @{
 */
/// [p18#36]  location of first unread character in #buffer.
#define LOC     cur_input.locfield
/** @}*/ // end group S25x37_P13x18

/** @addtogroup S211x219_P77x80
 * @{
 */
// [#211]
#define V_MODE          1                          // vertical mode
#define H_MODE          (V_MODE + MAX_COMMAND + 1) // horizontal mode
#define M_MODE          (H_MODE + MAX_COMMAND + 1) // math mode

/// [#212] prev_depth value that is ignored
#define ignoredepth     (-65536000L)

/** [#212]: ListStateRecord(366) .
 *
 *  field:
 *  + `modefield :: Int16(16)`
 *  + `headfield :: Pointer(64)`
 *  + `tailfield :: Pointer(64)`
 *  + `pgfield   :: Integer(64)`
 *  + `mlfield   :: Integer(64)`
 *  + `auxfield  :: MemoryWord(64)`
 */
typedef struct {
    Int16 modefield;              ///< (16) only need `[-203, 203]`
    Pointer headfield, tailfield; ///< (64)*2= 128
    Integer pgfield, mlfield;     ///< (64)*2= 128
    MemoryWord auxfield;          ///< (64)
} ListStateRecord; // (16+128+128+64) = (336)

/// [#213] current mode
#define mode            cur_list.modefield
/// [#213] header node of current list
#define head            cur_list.headfield
/// [#213] final node on current list
#define tail            cur_list.tailfield
/// [#213] number of paragraph lines accumulated
#define prevgraf        cur_list.pgfield
/// [#213] auxiliary data about the current list
#define aux             cur_list.auxfield
/// [#213] the name of |aux| in vertical mode
#define prevdepth       aux.sc
/// [#213] part of |aux| in horizontal mode
#define spacefactor     aux.hh.UU.lh
/// [#213] the other part of |aux| in horizontal mode
#define clang           aux.hh.rh
/// [#213] the name of |aux| in math mode
#define incompleatnoad  aux.int_
/// [#213] source file line number at beginning of list
#define modeline        cur_list.mlfield

/// [p78#214]
#define tailappend(x)   (link(tail) = (x), tail = link(tail))
/** @}*/ // end group S211x219_P77x80

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
/** @addtogroup S321x331_P131x133
 *  @{
 */
// [p131#322] leave an input level, re-enter the old
#define popinput() (inputptr--, cur_input = inputstack[inputptr])
// [p131#323] backs up a simple token list
#define backlist(x) begintokenlist((x), BACKED_UP)
// [p131#323] inserts a simple token list
#define inslist(x)  begintokenlist((x), INSERTED)
/** @}*/ // end group S321x331_P131x133
/** @addtogroup S332x365_P134x143
 *  @{
 */
/// [p136#341] label _not_use_
// #define switch_ 25
// #define startcs 26
// [p141#358]
#define noexpandflag    257

/// [p138#352]
#define ishex(x) \
    ((((x) >= '0') && ((x) <= '9')) || (((x) >= 'a') && ((x) <= 'f')))

/// [#360]: \endlinechar 行终止符无效，不添加换行符
#define end_line_char_inactive ((end_line_char < 0) || (end_line_char > 255))

/** @}*/ // end group S332x365_P134x143


// [#213] semantic
extern ListStateRecord nest[NEST_SIZE + 1];
extern UChar nest_ptr;
extern UChar max_nest_stack;
extern ListStateRecord cur_list;


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

extern Pointer parloc;
extern HalfWord partoken;
extern Boolean force_eof;


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


// [#211] semantic

extern void lexer_semantic_init();
extern void print_mode(Integer m);
extern void pushnest(void);
extern void popnest(void);
extern void showactivities(void); 

#endif // INC_LEXER_H