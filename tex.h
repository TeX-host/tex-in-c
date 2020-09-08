#pragma once
#ifndef TEX_H
// 待整理的常量。应合并到 tex_constant.h
#define TEX_H
#include "tex_types.h"
#include "tex_constant.h"

// 标记字符串序号
#define S(x) (x)
#define Static static
#define dwa_do_8  ((int)16*1024*1024)


#define defaultcode     1073741824L
#define awfulbad        1073741823L
#define boxflag         1073741824L
#define ignoredepth     (-65536000L)

#define condmathglue    98
#define muglue          99
#define aleaders        100
#define cleaders        101
#define xleaders        102

#define himemstatusage  14



/** @addtogroup S211x219_P77x80
 * @{
 */

// [#211]
#define V_MODE           1 // vertical mode
#define H_MODE           (V_MODE + MAX_COMMAND + 1) // horizontal mode
#define M_MODE           (H_MODE + MAX_COMMAND + 1) // math mode
/** @}*/ // end group S211x219_P77x80

/** @addtogroup S220x255_P81x101
 * @{
 */

// [#221]
typedef enum _EqLevel {
    /// level for undefined quantities.
    LEVEL_ZERO = MIN_QUARTER_WORD,
    /// outermost level for defined quantities.
    LEVEL_ONE,
} EqLevel;


/** [p82#222] [region 1] eqtb[ACTIVE_BASE, (HASH_BASE - 1)] holds
 *  current equivalents of single-character control sequences.
 */
/// [p82#222] beginning of [region 1], for active character equivalents.
#define ACTIVE_BASE                  1
/// [p82#222] equivalents of one-character control sequences.
#define SINGLE_BASE                 (ACTIVE_BASE + 256)
/// [p82#222] equivalent of \\csname\\endcsname
#define NULL_CS                     (ACTIVE_BASE + 256)


/** [p82#222] [region 2] eqtb[HASH_BASE, (GLUE_BASE - 1)] holds
 *  current equivalents of multiletter control sequences.
 */
/// [p82#222] beginning of [region 2], for the hash table.
#define HASH_BASE                   (NULL_CS + 1)

typedef enum _FrozenControlSeq {
    /// for error recovery.
    FROZEN_CONTROL_SEQUENCE = (HASH_BASE + HASH_SIZE),
    /// inaccessible but definable.
    FROZEN_PROTECTION = FROZEN_CONTROL_SEQUENCE,

    FROZEN_CR,           ///< permanent ‘\\cr’.
    FROZEN_END_GROUP,    ///< permanent ‘\\endgroup’.
    FROZEN_RIGHT,        ///< permanent ‘\\right’.
    FROZEN_FI,           ///< permanent ‘\\fi’.
    FROZEN_END_TEMPLATE, ///< permanent ‘\\endtemplate’.

    FROZEN_ENDV,        ///< second permanent ‘\\endtemplate’.
    FROZEN_RELAX,       ///< permanent ‘\\relax’.
    END_WRITE,          ///< permanent ‘\\endwrite’.
    FROZEN_DONT_EXPAND, ///< permanent ‘\\notexpanded’.
    FROZEN_NULL_FONT,   ///< permanent ‘\\nullfont’.
} FrozenControlSeq;

/// begins table of 257 permanent font identifiers.
#define FONT_ID_BASE                (FROZEN_NULL_FONT - FONT_BASE)
/// dummy location.
#define UNDEFINED_CONTROL_SEQUENCE  (FROZEN_NULL_FONT + 257)


/** [p82#222] [region 3] eqtb[HASH_BASE, (GLUE_BASE - 1)] holds 
 *  current equivalents of glue parameters like the current baselineskip.
 */
/// [p82#222] beginning of region 3
#define GLUE_BASE       (UNDEFINED_CONTROL_SEQUENCE + 1)
/// [p83#224] table of 256 “skip” registers
#define SKIP_BASE       (GLUE_BASE + GLUE_PARS)
/// [p83#224] table of 256 “muskip” registers.
#define MU_SKIP_BASE    (SKIP_BASE + 256)


/** [p82#222] [region 4] eqtb[GLUE_BASE, (LOCAL_BASE - 1)] holds
 *  current equivalents of local halfword quantities like
 *  + the current box registers,
 *  + the current “catcodes,”
 *  + the current font,
 *  + and a pointer to the current paragraph shape.
 */
/// [p83#224] beginning of [region 4].
#define LOCAL_BASE      (MU_SKIP_BASE + 256)
// [p87#230]
typedef enum _TokenListLoc {
    /// specifies paragraph shape.
    PAR_SHAPE_LOC = LOCAL_BASE,

    OUTPUT_ROUTINE_LOC, ///< points to token list for \\output.
    EVERY_PAR_LOC,      ///< points to token list for \\everypar.
    EVERY_MATH_LOC,     ///< points to token list for \\everymath.
    EVERY_DISPLAY_LOC,  ///< points to token list for \\everydisplay.
    EVERY_HBOX_LOC,     ///< points to token list for \\everyhbox.

    EVERY_VBOX_LOC, ///< points to token list for \\everyvbox.
    EVERY_JOB_LOC,  ///< points to token list for \\everyjob.
    EVERY_CR_LOC,   ///< points to token list for \\everycr.
    ERR_HELP_LOC,   ///< points to token list for \\errhelp.

    /// table of 256 token list registers.
    TOKS_BASE,
} TokenListLoc;

/// [p87#230] table of 256 box registers.
#define BOX_BASE        (TOKS_BASE + 256)
/// [p87#230] internal font number outside math mode.
#define CUR_FONT_LOC    (BOX_BASE + 256)
/// [p87#230] table of 48 math font numbers.
#define MATH_FONT_BASE  (CUR_FONT_LOC + 1)
/// [p87#230]  table of 256 command codes (the “catcodes”).
#define CAT_CODE_BASE   (MATH_FONT_BASE + 48)
/// [p87#230] table of 256 lowercase mappings.
#define LC_CODE_BASE    (CAT_CODE_BASE + 256)
/// [p87#230] table of 256 uppercase mappings.
#define UC_CODE_BASE    (LC_CODE_BASE + 256)
/// [p87#230] table of 256 spacefactor mappings.
#define SF_CODE_BASE    (UC_CODE_BASE + 256)
/// [p87#230] table of 256 math mode mappings.
#define MATH_CODE_BASE  (SF_CODE_BASE + 256)

/// [#232]
#define NULL_FONT       FONT_BASE
/// [#232] math code meaning “use the current family”
#define VAR_CODE        28672


/** [p87#230] [region 5] eqtb[INT_BASE, (DIMEN_BASE - 1)] holds
 *  current equivalents of fullword integer parameters like
 *  the current hyphenation penalty.
 */
/// [p87#230] beginning of [region 5].
#define INT_BASE         (MATH_CODE_BASE + 256)
/** @}*/ // end group S220x255_P81x101
/// [p92#236]
#define countbase       (INT_BASE + intpars)
#define delcodebase     (countbase + 256)

/** [p92#236] [region 6] eqtb[DIMEN_BASE, EQTB_SIZE] holds
 *  current equivalents of fullword dimension parameters like
 *  the current hsize or amount of hanging indentation.
 */
/// [p92#236] beginning of region 6.
#define DIMEN_BASE       (delcodebase + 256)


// [p136#341]
#define switch_         25
#define startcs         26

// [p141#358]
#define noexpandflag    257

// [p148#382]
#define topmarkcode         0
#define firstmarkcode       1
#define botmarkcode         2
#define splitfirstmarkcode  3
#define splitbotmarkcode    4

// [p179#480]
#define closed          2
#define justopen        1

/// [p190#520] `.fmt`, the extension, as a constant.
#define formatextension         S(256)
#define FORMAT_DEFAULT_LENGTH   20
#define formatarealength        11
#define formatextlength         4

// [p201#549]
#define nonaddress      0
// [p204#557]
#define kernbaseoffset  32768
// [p205#560]
#define badtfm          11
// [p229#619]
#define movepast        13
#define finrule         14
#define nextp           15
// [p239#644]
#define exactly         0
#define additional      1
// [p250#681]
#define mathchar        1
#define subbox          2
#define submlist        3
#define mathtextchar    4


/// p258#699: Subroutines for math mode
// enum FontSizeCode {};
// size code for the largest size in a family
#define TEXT_SIZE           0
// size code for the medium size in a family
#define SCRIPT_SIZE         16
// size code for the smallest size in a family
#define SCRIPT_SCRIPT_SIZE  32

// [p258#700]
#define totalmathsyparams  22
// [p258#701]
#define totalmathexparams  13
// [p267#725]
#define donewithnoad        80
#define donewithnode        81
#define checkdimensions     82
#define deleteq             83
// [p287#770]
#define alignstacknodesize  5
// [p289#780]
#define spancode            256
#define crcode              257
#define crcrcode            (crcode + 1)
#define endtemplatetoken    (CS_TOKEN_FLAG + FROZEN_END_TEMPLATE)
// [p294#797]
#define spannodesize    2
// [p303#817]
#define tightfit        3
#define loosefit        1
#define veryloosefit    0
#define decentfit       2
// [p304#819]
#define activenodesize  3
#define unhyphenated    0
#define hyphenated      1
#define lastactive      active
// [p304#821]
#define passivenodesize  2
// [p304#822]
#define deltanodesize   7
#define deltanode       2
// [p308#829]
#define deactivate      60
// [p361#970]
#define updateheights   90
// [p363#974]
#define deplorable      100000L
// [p366#980]
#define insertsonly     1
#define boxthere        2
// [p367#981]
#define pageinsnodesize  4
#define inserting        0
#define splitup          1
// [p371#994]
#define contribute      80

// [p384#1030] jmp label
// #define bigswitch            60
// #define appendnormalspace    120

/// [p400#1071]
#define shipoutflag     (boxflag + 512)
#define leaderflag      (boxflag + 513)

// #1291
// enum ChrCode {};
#define boxcode         0
#define copycode        1
#define lastboxcode     2
#define vsplitcode      3
#define vtopcode        4
// #1178
#define abovecode       0
#define overcode        1
#define atopcode        2
#define delimitedcode   3

// #1222
// enum ShortHandDef {};
#define chardefcode     0
#define mathchardefcode  1
#define countdefcode    2
#define dimendefcode    3
#define skipdefcode     4
#define muskipdefcode   5
#define toksdefcode     6

// #1291
#define showcode        0
#define showboxcode     1
#define showthecode     2
#define showlists       3

#define badfmt          6666
#define breakpoint      888
#define writenodesize   2
#define opennodesize    3

// [p472#1341]
// enum WhatsitsSubtype {};
#define opennode        0
#define writenode       1
#define closenode       2
#define specialnode     3
#define languagenode    4
// [p473#1344]
#define immediatecode   4
#define setlanguagecode  5

/// [p478#1371]
#define endwritetoken   (CS_TOKEN_FLAG + END_WRITE)


#endif // #ifndef TEX_H