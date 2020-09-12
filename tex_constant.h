#pragma once
#ifndef INC_TEX_CONSTANT
/** TeX 中使用的常量定义头文件.
 * 
 * notes:
 *  + 尽量使用 enum，并给一个合适的名字
 *  + 先按 section 序号排序。然后改为按功能分组排序
 * 
 */
#define INC_TEX_CONSTANT
#include "box.h" // [enum] NodeType

/** @addtogroup S1x16_P3x9
 * @{
 */

/// [#2] printed when TEX starts.
/// TODO: update to `3.14159265`
#define TEX_BANNER      "This is TeX, Version 3.14159"


/* [#11]: Constants in the outer block.
    The following parameters can be changed at compile time 
        to extend or reduce TEX’s capacity. 
    They may have different values in INITEX and 
        in production versions of TEX.
*/

/** [#11] greatest index in TeX's internal #mem array.
 *  must be strictly less than #MAX_HALF_WORD;
 *  must be equal to #MEM_TOP in INITEX,
 *  otherwise ≥ #MEM_TOP.
 *
 *  [30_000 => 3_000_000]
 */
#define MEM_MAX         3000000
/** [#11] smallest index in TeX's internal #mem array.
 *  must be #MAX_HALF_WORD or more;
 *  must be equal to #MEM_BOT in INITEX,
 *  otherwise ≤ #MEM_BOT.
 */
#define MEM_MIN         0
/** [#11] maximum number of characters simultaneously present.
 *  in current lines of open files
 *  and in control sequences between `\csname` and `\endcsname`;
 *  must not exceed #MAX_HALF_WORD.
 *
 * [500=>5000]
 */
#define BUF_SIZE        5000
/** [#11] width of context lines on terminal error messages.
 */
#define ERROR_LINE      72
/** [#11] width of first lines of contexts in terminal error messages.
 *
 * should be between [30, #ERROR_LINE-15=57].
 */
#define HALF_ERROR_LINE 42
/** [#11] width of longest text lines output.
 *
 * should be [60, ...]
 */
#define MAX_PRINT_LINE  79
/** [#11] maximum number of simultaneous input sources.
 */
#define STACK_SIZE      200
/** [#11] maximum number of input files and
 *  error insertions that can be going on simultaneously.
 */
#define MAX_IN_OPEN     6
/** [#11] maximum internal font number.
 */
#define FONT_MAX        75
/** [#11] number of words of #fontinfo for all fonts.
 *
 * [20_000 => 200_000]
 */
#define FONT_MEM_SIZE   200000
/** [#11] maximum number of simultaneous macro parameters.
 */
#define PARAM_SIZE      60
/** [#11] maximum number of semantic levels simultaneously active.
 */
#define NEST_SIZE       40
/** [#11] space for saving values outside of current group.
 *  must be at most #MAX_HALF_WORD.
 */
#define SAVE_SIZE       600
/** [#11] space for hyphenation patterns.
 *  should be larger for INITEX than it is in production versions of TeX.
 *
 * [8_000 => 131_000]
 */
#define TRIE_SIZE       131000
/** [#11] space for "opcodes" in the hyphenation patterns.
 *
 * [500=>5000]
 */
#define TRIE_OP_SIZE    5000
/** [#11] file names shouldn’t be longer than this.
 *
 * [40=>240]
 */
#define FILE_NAME_SIZE  240
/** [#11] string of length #FILE_NAME_SIZE;
 *  tells where the string pool appears
 */
#define POOL_NAME       "TeXformats:TEX.POOL                     "

/** [#12] smallest index in the #mem array dumped by INITEX.
 *  must not be less than #MEM_MIN.
 */
#define MEM_BOT         0
/** [#12] largest index in the #mem array dumped by INITEX.
 *  must be substantially larger than
 *  #MEM_BOT and not greater than #MEM_MAX
 *
 * [30_000 => 3_000_000]
 */
#define MEM_TOP         3000000
/** [#12] smallest internal font number.
 */
#define FONT_BASE       0
/** [#12] maximum number of control sequences.
 *  must not be less than #MIN_QUARTER_WORD.
 *
 * [2100 => 210_000]
 */
#define HASH_SIZE       210000
/** [#12] a prime number equal to about 85% of #HASH_SIZE.
 *  it should be at most about (#MEM_MAX − #MEM_MIN)/10 .
 *
 * [1777 => 171_553]
 */
#define HASH_PRIME      171553
/** [#12] another prime; the number of `\hyphenation` exceptions.
 */
#define HYPH_SIZE       307

/** [#16] symbolic name for a null constant.
 */
#define EMPTY           0
/** @}*/ // end group S1x16_P3x9

/** @addtogroup S110x114_P42x43
 * @{
 */

/// [#110]: smallest allowable value in a #QuarterWord.
#define MIN_QUARTER_WORD    0
/// [#110]: largest allowable value in a #QuarterWord.
/// 1/4 word = (8bit)[0, 255]
#define MAX_QUARTER_WORD    255
/// [#110]: smallest allowable value in a #HalfWord.
#define MIN_HALF_WORD       0
/// [#110]: largest allowable value in a #HalfWord.
/// 1/2 word = (16bit)[0, 65535]
/// must > 65535 (2^16-1)
#define MAX_HALF_WORD       655350000L
/** @}*/ // end group S110x114_P42x43


/** @addtogroup S207x210_P73x76
 * @{
 */

/// [#207]:
enum CatCode {
    ESCAPE,      ///< [#207] [`\\`] escape delimiter.
    LEFT_BRACE,  ///< [#207] [`{`] beginning of a group.
    RIGHT_BRACE, ///< [#207] [`}`] ending of a group.
    MATH_SHIFT,  ///< [#207] [`$`] mathematics shift character.
    TAB_MARK,    ///< [#207] [`&` | `\span`] alignment delimiter.

    CAR_RET = 5, ///< [#207] [`\r` | `\cr` | `\crcr`] end of line.
    MAC_PARAM,   ///< [#207] [`#`] macro parameter symbol.
    SUP_MARK,    ///< [#207] [`^`] superscript.
    SUB_MARK,    ///< [#207] [`_`] subscript.
    IGNORE,      ///< [#207] [`\0` | `^^@`] characters to ignore.

    /// [#207] [` ` | `\t`] characters equivalent to blank space.
    SPACER = 10, 
    LETTER,      ///< [#207] [A-Za-z] characters regarded as letters.
    OTHER_CHAR,  ///< [#207] none of the special character types.
    ACTIVE_CHAR, ///< [#207] [`~`] characters that invoke macros.
    COMMENT,     ///< [#207] [`%`] characters that introduce comments.

    /// [#207] [`\127` | `^^?`] characters that shouldn't appear.
    INVALID_CHAR = 15, 
}; // [#207]: CatCode

/// [#208]
enum TexCommandCode {
    // [p73#207]
    RELAX = ESCAPE,      ///< [#207] [`\relax`] do nothing.
    OUT_PARAM = CAR_RET, ///< [#207] output a macro parameter.
    /// [#207] end of `<v_j>` list in alignment template.
    ENDV = IGNORE,

    PAR_END = ACTIVE_CHAR, ///< [#207] [`\par`] end of paragraph.
    MATCH = ACTIVE_CHAR,   ///< [#207] match a macro parameter.

    END_MATCH = COMMENT, ///< [#207] end of parameters to macro.
    STOP = COMMENT,      ///< [#207] [`\end` | `\dump`] end of job.

    /// [#207] [`\delimiter`] specify delimiter numerically.
    DELIM_NUM = INVALID_CHAR,
    /// [#207] largest catcode for individual characters.
    MAX_CHAR_CODE = INVALID_CHAR,


    // [p74#208]

    /// [#208] [`\char`] character specified numerically.
    CHAR_NUM = 16,
    /// [#208] [`\mathchar`] explicit math code.
    MATH_CHAR_NUM,
    MARK,     ///< [#208] [`\mark`] mark definition.
    XRAY,     ///< [#208] [`\show`, `\showbox`, ...] peek inside of TEX.
    MAKE_BOX, ///< [#208] [`\box`, `\copy`, `\hbox`, ...] make a box.

    /// [#208] [`\moveleft`, `\moveright`] horizontal motion.
    HMOVE = 21,
    VMOVE,   ///< [#208] [`\raise`, `\lower`] vertical motion.
    UN_HBOX, ///< [#208] [`\unhbox`, `\unhcopy`] unglue a box.
    UN_VBOX, ///< [#208] [`\unvbox`, `\unvcopy`] unglue a box.
    /// [#208] [`\unpenalty`, `\unkern`, `\unskip`] nullify last item.
    REMOVE_ITEM,

    HSKIP = 26, ///< [#208] [`\hskip`, `\hfil`, ...] horizontal glue.
    VSKIP,      ///< [#208] [`\vskip`, `\vfil`, ...] vertical glue.
    MSKIP,      ///< [#208] [`\mskip`] math glue.
    KERN,       ///< [#208] [`\kern`] fixed space.
    MKERN,      ///< [#208] [`\mkern`] math kern.

    /// [#208] [`\shipout`, `\leaders`, ...] use a box.
    LEADER_SHIP = 31,
    HALIGN,   ///< [#208] [`\halign`] horizontal table alignment.
    VALIGN,   ///< [#208] [`\valign`] vertical table alignment.
    NO_ALIGN, ///< [#208] [`\noalign`] temporary escape from alignment.
    VRULE,    ///< [#208] [`\vrule`] vertical rule.

    HRULE = 36, ///< [#208] [`\hrule`] horizontal rule.
    INSERT,     ///< [#208] [`\insert`] vlist inserted in box.
    /// [#208] [`\vadjust`] vlist inserted in enclosing paragraph.
    VADJUST,
    /// [#208] [`\ignorespaces`] gobble #SPACER tokens.
    IGNORE_SPACES,
    /// [#208] [`\afterassignment`] save till assignment is done.
    AFTER_ASSIGNMENT,

    /// [#208] [`\aftergroup`] save till group is done.
    AFTER_GROUP = 41,
    BREAK_PENALTY, ///< [#208] [`\penalty`] additional badness.
    START_PAR,     ///< [#208] [`\indent`, `\noindent`] begin paragraph.
    ITAL_CORR,     ///< [#208] [`\/`] italic correction.
    ACCENT,        ///< [#208] [`\accent`] attach accent in text.

    /// [#208] [`\mathaccent`] attach accent in math.
    MATH_ACCENT = 46,
    /// [#208] [`\−`, `\discretionary`] discretionary texts.
    DISCRETIONARY,
    EQ_NO,      ///< [#208] [`\eqno`, `\leqno`] equation number.
    LEFT_RIGHT, ///< [#208] [`\left`, `\right`] variable delimiter.
    MATH_COMP,  ///< [#208] [`\mathbin`,...] component of formula.

    /// [#208] [`\displaylimits`,...] diddle limit conventions.
    LIMIT_SWITCH = 51,
    /// [#208] [`\above`, `\atop`,...] generalized fraction.
    ABOVE,
    MATH_STYLE,  ///< [#208] [`\displaystyle`, ...] style specification.
    MATH_CHOICE, ///< [#208] [`\mathchoice`] choice specification.
    NON_SCRIPT,  ///< [#208] [`\nonscript`] conditional math glue.

    /// [#208] [`\vcenter`] vertically center a vbox.
    VCENTER = 56,
    /// [#208] [`\lowercase`, `\uppercase`] force specific case.
    CASE_SHIFT,
    MESSAGE,   ///< [#208] [`\message`, `\errmessage`] send to user.
    EXTENSION, ///< [#208] [`\write`, `\special`,...] extensions to TEX.
    IN_STREAM, ///< [#208] [`\openin`, `\closein`] files for reading.

    /// [#208] [`\begingroup`] begin local grouping.
    BEGIN_GROUP = 61,
    END_GROUP,   ///< [#208] [`\endgroup`] end local grouping.
    OMIT,        ///< [#208] [`\omit`] omit alignment template.
    EX_SPACE,    ///< [#208] [`\ `] explicit space.
    NO_BOUNDARY, ///< [#208] [`\noboundary`] suppress boundary ligatures.

    // [p75#208]

    /// [#208] [`\radical`] square root and similar signs.
    RADICAL = 66,
    /// [#208] [`\endcsname`] end control sequence.
    END_CS_NAME = 67,
    /// [#208] the smallest code that can follow `\the`.
    MIN_INTERNAL = 68,
    /// [#208] character code defined by `\chardef`.
    CHAR_GIVEN = 68,
    /// [#208] math code defined by `\mathchardef`.
    MATH_GIVEN = 69,
    /// [#208] [`\lastpenalty`, `\lastker`, `\lastskip`] most recent item.
    LAST_ITEM = 70,

    /// [#208] largest command code that can’t be `\global`.
    MAX_NON_PREFIXED_COMMAND = 70,


    // [p75#209]

    /// [#209] [`\toks`] token list register.
    TOKS_REGISTER = 71,
    /// [#209] [`\output`, `\everypar`,...] special token list.
    ASSIGN_TOKS,
    /// [#209] [`\tolerance`, `\day`,...] user-defined integer.
    ASSIGN_INT,
    /// [#209] [`\hsize`,...] user-defined length.
    ASSIGN_DIMEN,
    /// [#209] [`\baselineskip`,...] user-defined glue.
    ASSIGN_GLUE,

    /// [#209] [`\thinmuskip`,...] user-defined muglue.
    ASSIGN_MU_GLUE = 76,
    /// [#209] [`\fontdimen`] user-defined font dimension.
    ASSIGN_FONT_DIMEN,
    /// [#209] [`\hyphenchar`, `\skewchar`] user-defined font integer.
    ASSIGN_FONT_INT,
    /// [#209] [`\spacefactor`, `\prevdepth`] specify state info.
    SET_AUX,
    /// [#209] [`\prevgraf`] specify state info.
    SET_PREV_GRAF,

    /// [#209] [`\pagegoal`,...] specify state info.
    SET_PAGE_DIMEN = 81,
    /// [#209] [`\deadcycles`, `\insertpenalties`] specify state info.
    SET_PAGE_INT,
    /// [#209] [`\wd`, `\ht`, `\dp`] change dimension of box.
    SET_BOX_DIMEN,
    /// [#209] [`\parshape`] specify fancy paragraph shape.
    SET_SHAPE,
    /// [#209] [`\catcode`,...] define a character code.
    DEF_CODE,

    /// [#209] [`\textfont`,] declare math fonts.
    DEF_FAMILY = 86,
    /// [#209] set current font ( font identifiers ).
    SET_FONT,
    /// [#209] [`\font`] define a font file.
    DEF_FONT,
    /// [#209] [`\count`, `\dimen`,] internal register.
    REGISTER = 89,
    /// [#209] the largest code that can follow `\the`.
    MAX_INTERNAL = 89,
    /// [#209] [`\advance`] ADVANCE a register or parameter.
    ADVANCE,

    /// [#209] [`\multiply`] multiply a register or parameter.
    MULTIPLY = 91,
    /// [#209] [`\divide`] divide a register or parameter.
    DIVIDE,
    /// [#209] [`\global`, `\long`, `\outer`] qualify a definition.
    PREFIX,
    /// [#209] [`\let`, `\futurelet`] assign a command code.
    LET,
    /// [#209] [`\chardef`, `\countdef`,...] code definition.
    SHORTHAND_DEF,

    /// [#209] [`\read`] read into a control sequence.
    READ_TO_CS = 96,
    /// [#209] [`\def`, `\gdef`, `\xdef`, `\edef`] macro definition.
    DEF,
    /// [#209] [`\setbox`] set a box.
    SET_BOX,
    /// [#209] [`\hyphenation`, `\patterns`] hyphenation data.
    HYPH_DATA,
    /// [#209] [`\batchmode`,...] define level of interaction.
    SET_INTERACTION = 100,

    /// [#209] the largest command code seen at big switch.
    MAX_COMMAND = 100,


    // [p76#210]

    /// [#210] initial state of most #eq_type fields.
    UNDEFINED_CS = 101,
    EXPAND_AFTER, ///< [#210] [`\expandafter`] special expansion.
    NO_EXPAND,    ///< [#210] `\noexpand`[] special nonexpansion.
    INPUT,        ///< [#210] [`\input`, `\endinput`] input a source file.
    IF_TEST,      ///< [#210] [`\if`, `\ifcase`,] conditional text.

    /// [#210] [`\else`,...] delimiters for conditionals.
    FI_OR_ELSE = 106,
    CS_NAME, ///< [#210] [`\csname`] make a control sequence from tokens.
    CONVERT, ///< [#210] [`\number`, `\string`,...] convert to text.
    THE,     ///< [#210] [`\the`] expand an internal quantity.
    /// [#210] [`\topmark`,...] inserted mark.
    TOP_BOT_MARK,

    CALL = 111,      ///< [#210] non-long, non-outer control sequence.
    LONG_CALL,       ///< [#210] long, non-outer control sequence.
    OUTER_CALL,      ///< [#210] non-long, outer control sequence.
    LONG_OUTER_CALL, ///< [#210] long, outer control sequence.
    END_TEMPLATE,    ///< [#210] end of an alignment template.

    /// [#210] the following token was marked by `\noexpand`.
    DONT_EXPAND = 116,
    /// [#210] the equivalent points to a glue specification.
    GLUE_REF,  
    /// [#210] the equivalent points to a parshape specification.
    SHAPE_REF, 
    /// [#210] the equivalent points to a box node, or is #null.
    BOX_REF,  
    /// [#210] the equivalent is simply a halfword number.
    DATA,      

}; // [#208] enum TexCommandCode
/** @}*/ // end group S207x210_P73x76


/// [#468] command code for ...
enum CmdCode {
    numbercode = 0,   ///< `\number`
    romannumeralcode, ///< `\romannumeral`
    stringcode,       ///< `\string`
    meaningcode,      ///< `\meaning`
    fontnamecode,     ///< `\fontname`
    jobnamecode = 5,  ///< `\jobname`
}; // [#468] enum CmdCode


// [ #1055~1135: BUILDING BOXES AND LISTS ]

/// [#1058]
enum hvSkipDiff {
    FIL_CODE,
    FILL_CODE,
    SS_CODE,
    FIL_NEG_CODE,
    SKIP_CODE,
    MSKIP_CODE
};


#endif // INC_TEX_CONSTANT