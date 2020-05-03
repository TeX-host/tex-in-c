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


/** @addtogroup S17x24_P10x12
 * @{
 */

/// [#19] ordinal number of the smallest element of #TextChar.
#define FIRST_TEXT_CHAR 0
/// [#19] ordinal number of the largest element of #TextChar.
#define LAST_TEXT_CHAR  255
/// [#22] ASCII code that might disappear.
#define NULL_CODE       0
/// [#22] ASCII code used at end of line.
#define CARRIAGE_RETURN '\r'
/// [#22] ASCII code that many systems prohibit in text files.
#define INVALID_CODE    127
/** @}*/ // end group S17x24_P10x12


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


/** @addtogroup S115x132_P44x49
 * @{
 */

/// [#124] the `link` of an empty variable-size node.
#define emptyflag       MAX_HALF_WORD
// ??? nodesize
/** @}*/ // end group S115x132_P44x49


/** @addtogroup S162x172_P58x61
 * @{
 */

/// [#162] specification for `0pt plus 0pt minus 0pt`.
#define zeroglue        MEM_BOT
/// [#162] `0pt plus 1fil minus 0pt`.
#define filglue         (zeroglue + gluespecsize)
/// [#162] `0pt plus 1fill minus 0pt`.
#define fillglue        (filglue + gluespecsize)
/// [#162] `0pt plus 1fil minus 1fil`.
#define ssglue          (fillglue + gluespecsize)
/// [#162] `0pt plus −1fil minus 0pt`.
#define filnegglue      (ssglue + gluespecsize)
/// [#162] largest statically allocated word in the variable-size #mem.
#define lomemstatmax    (filnegglue + gluespecsize - 1)

/// [#162] list of insertion data for current page.
#define pageinshead     (MEM_TOP-CHAR_NODE_SIZE+1)
/// [#162] vlist of items not yet on current page.
#define contribhead     (pageinshead-CHAR_NODE_SIZE)
/// [#162] vlist for current page.
#define pagehead        (contribhead-CHAR_NODE_SIZE)
/// [#162] head of a temporary list of some kind.
#define temphead        (pagehead-CHAR_NODE_SIZE)
/// [#162] head of a temporary list of another kind.
#define holdhead        (temphead-CHAR_NODE_SIZE)
/// [#162] head of adjustment list returned by hpack.
#define adjusthead      (holdhead-CHAR_NODE_SIZE)
/// [#162] head of active list in line break, needs two words.
#define active          (adjusthead-CHAR_NODE_SIZE-CHAR_NODE_SIZE)
/// [#162] head of preamble list for alignments.
#define alignhead       (active-CHAR_NODE_SIZE)
/// [#162] tail of spanned-width lists.
#define endspan         (alignhead-CHAR_NODE_SIZE)
/// [#162] a constant token list.
#define omittemplate    (endspan-CHAR_NODE_SIZE)
/// [#162] permanently empty list.
#define nulllist        (omittemplate-CHAR_NODE_SIZE)
/// [#162] a ligature masquerading as a `char_node`.
#define ligtrick        (nulllist-CHAR_NODE_SIZE)
/// [#162] used for scrap information.
#define garbage         (ligtrick)
/// [#162] head of token list built by `scan_keyword`.
#define backuphead      (ligtrick-CHAR_NODE_SIZE)
/// [#162] smallest statically allocated word in the one-word #mem.
#define himemstatmin    (backuphead)
/** @}*/ // end group S162x172_P58x61


/// [#232]
#define varcode         28672
#define NULL_FONT       0
#define nullflag        (-1073741824L)

#define defaultcode     1073741824L
#define awfulbad        1073741823L
#define boxflag         1073741824L
#define ignoredepth     (-65536000L)

#define boxnodesize     7
#define widthoffset     1
#define depthoffset     2
#define heightoffset    3
#define listoffset      5

#define stretching      1
#define shrinking       2
#define glueoffset      6

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
#define levelzero       MIN_QUARTER_WORD
#define levelone        (levelzero + 1)

/// p82#222
#define activebase                  1
#define singlebase                  (activebase + 256)
#define nullcs                      (singlebase + 256)
#define hashbase                    (nullcs + 1)
#define frozencontrolsequence       (hashbase + HASH_SIZE)

#define frozenprotection            frozencontrolsequence
#define frozencr                    (frozencontrolsequence + 1)
#define frozenendgroup              (frozencontrolsequence + 2)
#define frozenright                 (frozencontrolsequence + 3)
#define frozenfi                    (frozencontrolsequence + 4)
#define frozenendtemplate           (frozencontrolsequence + 5)
#define frozenendv                  (frozencontrolsequence + 6)
#define frozenrelax                 (frozencontrolsequence + 7)
#define endwrite                    (frozencontrolsequence + 8)
#define frozendontexpand            (frozencontrolsequence + 9)
#define FROZEN_NULL_FONT              (frozencontrolsequence + 10)
#define fontidbase                  (FROZEN_NULL_FONT)
#define UNDEFINED_CONTROL_SEQUENCE    (FROZEN_NULL_FONT + 257)
#define gluebase                    (UNDEFINED_CONTROL_SEQUENCE + 1)

#define skipbase        (gluebase + GLUE_PARS)
#define muskipbase      (skipbase + 256)
#define localbase       (muskipbase + 256)


// [p87#230]
#define parshapeloc     localbase
#define outputroutineloc  (localbase + 1)
#define everyparloc     (localbase + 2)
#define everymathloc    (localbase + 3)
#define everydisplayloc  (localbase + 4)
#define everyhboxloc    (localbase + 5)
#define everyvboxloc    (localbase + 6)
#define everyjobloc     (localbase + 7)
#define everycrloc      (localbase + 8)
#define errhelploc      (localbase + 9)
#define toksbase        (localbase + 10)

#define boxbase         (toksbase + 256)
#define curfontloc      (boxbase + 256)
#define mathfontbase    (curfontloc + 1)
#define catcodebase     (mathfontbase + 48)
#define lccodebase      (catcodebase + 256)
#define uccodebase      (lccodebase + 256)
#define sfcodebase      (uccodebase + 256)
#define mathcodebase    (sfcodebase + 256)
#define intbase         (mathcodebase + 256)
/** @}*/ // end group S220x255_P81x101

// [p92]
#define countbase       (intbase + intpars)
#define delcodebase     (countbase + 256)
#define dimenbase       (delcodebase + 256)


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
#define endtemplatetoken    (CS_TOKEN_FLAG + frozenendtemplate)
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
#define endwritetoken   (CS_TOKEN_FLAG + endwrite)


#endif // #ifndef TEX_H