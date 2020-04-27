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

#define NORMAL 0

// [ ARITHMETIC WITH SCALED DIMENSIONS ]

/// [#108]: infinitely bad value
#define INF_BAD 10000


// [ p30~37#72~98: REPORTING ERRORS ]

/** [p30#73]: four settings, 
 * representing increasing amounts of user interaction.
 * 
 * [REPORTING ERRORS]
 */
enum UserInteractionMode {
    BATCH_MODE,      ///< omits all stops and omits terminal output
    NON_STOP_MODE,   ///< omits all stops
    SCROLL_MODE,     ///< omits error stops
    ERROR_STOP_MODE, ///< stops at every opportunity to interact
}; // [#73] enum UserInteractionMode

/** [p31#76]: four possible values for
 * the worst level of error that has been detected.
 * 
 * [REPORTING ERRORS]
 */
enum ErrorLevel {
    SPOTLESS = 0,         ///< nothing has been amiss yet
    WARNING_ISSUED,       ///< begin_diagnostic has been called
    ERROR_MESSAGE_ISSUED, ///< error has been called
    FATAL_ERROR_STOP,     ///< termination was premature
}; // [#76] enum ErrorLevel


/* [ p50~57#133~161: DATA STRUCTURES FOR BOXES AND THEIR FRIENDS ]
 */

enum NodeType {
    HLIST_NODE,  ///< #135: type of hlist nodes
    VLIST_NODE,  ///< #137: type of vlist nodes
    RULE_NODE,   ///< #138: type of rule nodes
    INS_NODE,    ///< #140: type of insertion nodes
    MARK_NODE,   ///< #141: type of a mark node
    ADJUST_NODE, ///< #142: type of an adjust node

    LIGATURE_NODE, ///< #143: type of a ligature node
    DISC_NODE,     ///< #145: type of a discretionary node
    WHATSIT_NODE,  ///< #146: type of special extension nodes
    MATH_NODE,     ///< #147: type of a math node
    GLUE_NODE, ///< #150: number of words to allocate for a glue specification

    KERN_NODE,    ///< #155: type of a kern node
    PENALTY_NODE, ///< #157: type of a penalty node
    UNSET_NODE,   ///< #159: type for an unset node
}; // enum NodeType

/// [#138]: number of words to allocate for a rule node
#define rulenodesize    4
/// [#138]: -(2^30)  signifies a missing item
#define nullflag        (-1073741824L)
/// [#140]: number of words to allocate for an insertion
#define insnodesize     5
/// [#141]: number of words to allocate for most node types
#define smallnodesize   2
/// [#147]: subtype for math node that introduces a formula
#define before          0
/// [#147]: subtype for math node that winds up a formula
#define after           1
/// [#150]: number of words to allocate for a glue specification
#define gluespecsize    4

/** [#150]: the orders of infinity (normal, `fil`, `fill`, or `filll`)
 * corresponding to the stretch and shrink values.
 * 
 */
enum InfinityOrder {
    FIL = 1, ///< first-order infinity
    FILL,    ///< second-order infinity
    FILLL    ///< third-order infinity
}; // [#150] enum InfinityOrder

/// [#155]: subtype of kern nodes from `\kern` and `\/`
#define explicit    1
/// [#155]: subtype of kern nodes from accents
#define acckern     2

/// [#157]: "infinite" penalty value
#define INF_PENALTY     INF_BAD
/// [#157]: "negatively infinite penalty value
#define EJECT_PENALTY   (-INF_PENALTY)

/** [p83#224] Region 3 of `eqtb` contains the 256 `\skip` registers
 * 
 * 值得注意的点: `muskip` 比其他参数大
 */
enum SkipRegisters {
    LINE_SKIP_CODE = 0,      ///< interline glue if baseline skip is infeasible
    BASELINE_SKIP_CODE,      ///< desired glue between baselines
    PAR_SKIP_CODE,           ///< extra glue just above a paragraph
    ABOVE_DISPLAY_SKIP_CODE, ///< extra glue just above displayed math
    BELOW_DISPLAY_SKIP_CODE, ///< extra glue just below displayed math.

    /// glue [above] displayed math following short lines
    ABOVE_DISPLAY_SHORT_SKIP_CODE = 5,
    ///  glue [below] displayed math following short lines
    BELOW_DISPLAY_SHORT_SKIP_CODE,
    LEFT_SKIP_CODE,  ///< glue at left of justified lines
    RIGHT_SKIP_CODE, ///< glue at right of justified lines
    TOP_SKIP_CODE,   ///< glue at top of main pages

    SPLIT_TOP_SKIP_CODE = 10, ///< glue at top of split pages
    TAB_SKIP_CODE,            ///< glue between aligned entries
    SPACE_SKIP_CODE,          ///< glue between words (if not zero glue)
    XSPACE_SKIP_CODE,         ///< glue after sentences (if not zero glue)
    PAR_FILL_SKIP_CODE,       ///< glue on last line of paragraph

    THIN_MU_SKIP_CODE = 15, ///< thin space in math formula
    MED_MU_SKIP_CODE,       ///< medium space in math formula
    THICK_MU_SKIP_CODE,     ///< thick space in math formula
    GLUE_PARS               ///< total number of glue parameters
}; // [p83#224] enum SkipRegisters

/** [#236] Region 5 of `eqtb` contains
 * the integer parameters and registers defined here,
 * as well as the del code table.
 * 
 */
enum DelimiterCodesTable {
    pretolerancecode = 0, ///< badness tolerance before hyphenation
    tolerancecode,        ///< badness tolerance after hyphenation
    linepenaltycode,      ///< added to the badness of every line
    hyphenpenaltycode,    ///< penalty for break after discretionary hyphen
    exhyphenpenaltycode,  ///< penalty for break after explicit hyphen

    clubpenaltycode,         ///< penalty for creating a club line
    widowpenaltycode,        ///< penalty for creating a widow line
    displaywidowpenaltycode, ///< ditto, just before a display
    brokenpenaltycode,       ///< penalty for breaking a page at a broken line
    binoppenaltycode,        ///< penalty for breaking after a binary operation.

    /// penalty for breaking after a relation
    relpenaltycode = 10,
    /// penalty for breaking just before a displayed formula
    predisplaypenaltycode,
    /// penalty for breaking just after a displayed formula
    postdisplaypenaltycode,
    interlinepenaltycode,     ///< additional penalty between lines
    doublehyphendemeritscode, ///< demerits for double hyphen break

    finalhyphendemeritscode, ///< demerits for final hyphen break
    adjdemeritscode,         ///< demerits for adjacent incompatible lines
    magcode,                 ///< magnification ratio
    delimiterfactorcode,     ///< ratio for variable-size delimiters
    loosenesscode,           ///< change in number of lines for a paragraph

    timecode = 20,      ///< current time of day
    daycode,            ///< current day of the month
    monthcode,          ///< current month of the year
    yearcode,           ///< current year of our Lord
    showboxbreadthcode, ///< nodes per level in `show_box`

    showboxdepthcode,  ///< maximum level in `show_box`
    hbadnesscode,      ///< hboxes exceeding this badness will be shown by `hpack`
    vbadnesscode,      ///< vboxes exceeding this badness will be shown by `vpack`
    pausingcode,       ///< pause after each line is read from a file
    tracingonlinecode, ///< show diagnostic output on terminal

    tracingmacroscode = 30, ///< show macros as they are being expanded
    tracingstatscode,       ///< show memory usage if TeX knows it
    tracingparagraphscode,  ///< show line-break calculations
    tracingpagescode,       ///< show page-break calculations
    tracingoutputcode,      ///< show boxes when they are shipped out

    tracinglostcharscode, ///< show characters that aren’t in the font
    tracingcommandscode,  ///< show command codes at `big_switch`
    tracingrestorescode,  ///< show equivalents when they are restored
    uchyphcode,           ///< hyphenate words beginning with a capital letter
    outputpenaltycode,    ///< penalty found at current page break

    maxdeadcyclescode = 40, ///< bound on consecutive dead cycles of output
    hangaftercode,          ///< hanging indentation changes after this many lines
    floatingpenaltycode,    ///< penalty for insertions heldover after a split
    globaldefscode,         ///< override `\global` specifications
    curfamcode,             ///< current family

    ESCAPE_CHARcode,       ///< escape character for token output
    defaulthyphencharcode, ///< value of `\hyphenchar` when a font is loaded
    defaultskewcharcode,   ///< value of `\skewchar` when a font is loaded
    endlinecharcode,       ///< character placed at the right end of the buffer
    newlinecharcode,       ///< character that prints as `println`

    languagecode = 50,     ///< current hyphenation table
    lefthyphenmincode,     ///< minimum left hyphenation fragment size
    righthyphenmincode,    ///< minimum right hyphenation fragment size
    holdinginsertscode,    ///< do not remove insertion nodes from `\box255`
    errorcontextlinescode, ///< maximum intermediate line pairs shown

    intpars = 55, ///< total number of integer parameters
}; // [#236] enum DelimiterCodesTable

/** [p99#247]: final region of `eqtb`
 * contains the dimension parameters defined here,
 * and the 256 `\dimen` registers.
 * 
 */
enum DimensionRegisters {
    parindentcode = 0, ///< indentation of paragraphs
    mathsurroundcode,  ///< space around math in text
    lineskiplimitcode, ///< threshold for line skip instead of baseline skip
    hsizecode,         ///< line width in horizontal mode
    vsizecode,         ///< page height in vertical mode

    maxdepthcode,      ///< maximum depth of boxes on main pages
    splitmaxdepthcode, ///< maximum depth of boxes on split pages
    boxmaxdepthcode,   ///< maximum depth of explicit vboxes
    hfuzzcode,         ///< tolerance for overfull hbox messages
    vfuzzcode,         ///< tolerance for overfull vbox messages.

    /// maximum amount uncovered by variable delimiters
    delimitershortfallcode = 10,
    nulldelimiterspacecode, ///< blank space in null delimiters
    scriptspacecode,        ///< extra space after subscript or superscript
    predisplaysizecode,     ///< length of text preceding a display
    displaywidthcode,       ///< length of line for displayed equation

    displayindentcode, ///< indentation of line for displayed equation
    overfullrulecode,  ///< width of rule that identifies overfull hboxes
    hangindentcode,    ///< amount of hanging indentation
    hoffsetcode,       ///< amount of horizontal offset when shipping pages out
    voffsetcode,       ///< amount of vertical offset when shipping pages out

    /// reduces badnesses on final pass of line-breaking
    emergencystretchcode = 20,
    dimenpars, ///< total number of dimension parameters
}; // [p99#247] enum DimensionRegisters

/// [#268]
enum SaveType {
    restoreoldvalue, ///< when a value should be restored later
    restorezero,     ///< when an undefined entry should be restored
    inserttoken,     ///< when a token is being saved for later use
    levelboundary,   ///< corresponding to beginning of group
}; // [#268] SaveType

/** [#269] group codes 
 * that are used to discriminate between different kinds of groups.
 * 
 */
enum GroupCode {
    bottomlevel = 0,   ///< the outside world
    simplegroup,       ///< local structure only
    hboxgroup,         ///< `\hbox{...}`
    adjustedhboxgroup, ///< `\hbox{...}` in vertical mode
    vboxgroup,         ///< `\vbox{...}`

    vtopgroup = 5, ///< `\vtop{...}`
    aligngroup,    ///< `\halign{...}`, `\valign{...}`
    noaligngroup,  ///< `\noalign{...}`
    outputgroup,   ///< output routine
    mathgroup,     ///< `^{...}`

    discgroup = 10,  ///< `\discretionary{...}{...}{...}`
    insertgroup,     ///< `\insert{...}`, `\vadjust{...}`
    vcentergroup,    ///< `\vcenter{...}`
    mathchoicegroup, ///< `\mathchoice{...}{...}{...}{...}`
    semisimplegroup, ///< `\begingroup...\endgroup`

    mathshiftgroup = 15, ///< `$...$`
    mathleftgroup = 16,  ///< `\left...\right`
    MAX_GROUP_CODE = 16,
}; // [#269] GroupCode

/// [#468] command code for ...
enum CmdCode {
    numbercode = 0,   ///< `\number`
    romannumeralcode, ///< `\romannumeral`
    stringcode,       ///< `\string`
    meaningcode,      ///< `\meaning`
    fontnamecode,     ///< `\fontname`
    jobnamecode = 5,  ///< `\jobname`
}; // [#468] enum CmdCode

// [ #487~510: CONDITIONAL PROCESSING ]

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
}; // [#489] enum IfCode


// [ #539~582: FONT METRIC DATA ]

/** [p198#544]: tag field in a char_info_word
 * that explain how to interpret the remainder field.
 */ 
enum CharTag {
    NO_TAG,   ///< vanilla character
    LIG_TAG,  ///< character has a ligature/kerning program
    LIST_TAG, ///< character has a successor in a charlist
    EXT_TAG   ///< character is extensible
};

/// [p200#547]
enum TFMParamVal {
    TFM_PARAM_MISSING, ///< TeX sets the missing parameters to zero

    SLANT_CODE,
    SPACE_CODE,
    SPACE_STRETCH_CODE,
    SPACE_SHRINK_CODE,
    X_HEIGHT_CODE,
    QUAD_CODE,
    EXTRA_SPACE_CODE
};


// [ #~ : PART 34: DATA STRUCTURES FOR MATH MODE ]

/** [p251#682] type of noad classified XXX.
 * 
 * Each portion of a formula is classified as 
 * Ord, Op, Bin, Rel, Ope, Clo, Pun, or Inn,
 * for purposes of spacing and line breaking.
 */
enum NoadType {
    /// [#682] [UNSET_NODE=13 + 3 = 16]
    /// type of a noad classified Ord.
    ordnoad = (UNSET_NODE + 3),

    // [#682]
    opnoad,    ///< noad classified Op
    binnoad,   ///< noad classified Bin
    relnoad,   ///< noad classified Rel
    opennoad,  ///< noad classified Ope
    closenoad, ///< noad classified Clo
    punctnoad, ///< noad classified Pun
    innernoad, ///< noad classified Inn.

    // [#683]
    radicalnoad,  ///< noad for square roots
    fractionnoad, ///< noad for generalized fractions.

    // [#687]
    undernoad,   ///< noad for underlining
    overnoad,    ///< noad for overlining
    accentnoad,  ///< noad for accented subformulas
    vcenternoad, ///< noad for `\vcenter`
    leftnoad,    ///< noad for `\left`
    rightnoad,   ///< noad for `\right`
}; // [#682] enum NoadType

/// [#682] `subtype` of `op_noad` whose scripts are to be above, below
#define limits   1
/// [#682] `subtype` of `op_noad` whose scripts are to be normal
#define nolimits 2

/// [#681] number of words in a normal noad
#define noadsize            (4 * CHAR_NODE_SIZE)
/// [#683] number of mem words in a radical noad
#define radicalnoadsize     (5 * CHAR_NODE_SIZE)
/// [#683] number of mem words in a fraction noad
#define fractionnoadsize    (6 * CHAR_NODE_SIZE)
/// [#687] number of mem words in an accent noad
#define accentnoadsize      (5 * CHAR_NODE_SIZE)

/// [#688] type of a style node
#define stylenode (UNSET_NODE + 1)
/// [#688] number of words in a style node
#define stylenodesize 3

// [#688]
enum StyleNodeSubtype {
    displaystyle = 0,      ///< subtype for `\displaystyle`
    textstyle = 2,         ///< subtype for `\textstyle`
    scriptstyle = 4,       ///< subtype for `\scriptstyle`
    scriptscriptstyle = 6, ///< subtype for `\scriptscriptstyle`
}; // [#688] StyleNodeSubtype

/// [#688] add this to an uncramped style if you want to cramp it
#define cramped 1
/// [#689] type of a choice node
#define choicenode (UNSET_NODE + 2)


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