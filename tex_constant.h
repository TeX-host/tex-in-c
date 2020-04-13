#pragma once
#ifndef INC_TEX_CONSTANT
/* TeX 中使用的常量定义。
 * 
 * notes:
 *  + 尽量使用 enum，并给一个合适的名字
 *  + 先按 section 序号排序。然后改为按功能分组排序
 * 
 */
#define INC_TEX_CONSTANT

#define NORMAL 0

// [ ARITHMETIC WITH SCALED DIMENSIONS ]
// #108: infinitely bad value
#define INF_BAD 10000

/*
 * [ p30~37#72~98: REPORTING ERRORS ]
 * 
 */

// p30#73:  four settings, representing increasing amounts of user interaction
// [REPORTING ERRORS]
enum UserInteractionMode {
    BATCH_MODE,     // omits all stops and omits terminal output
    NON_STOP_MODE,  // omits all stops
    SCROLL_MODE,    // omits error stops
    ERROR_STOP_MODE // stops at every opportunity to interact
};

// p31#76:  four possible values for
// the worst level of error that has been detected
// [REPORTING ERRORS]
enum ErrorLevel {
    SPOTLESS = 0,         // nothing has been amiss yet
    WARNING_ISSUED,       // begin_diagnostic has been called
    ERROR_MESSAGE_ISSUED, // error has been called
    FATAL_ERROR_STOP,     // termination was premature
};


/*
 * [ p50~57#133~161: DATA STRUCTURES FOR BOXES AND THEIR FRIENDS ]
 * 
 */
// #define HLIST_NODE 0
enum NodeType {
    HLIST_NODE,
    VLIST_NODE,    // #137: type of vlist nodes
    RULE_NODE,     // #138: type of rule nodes
    INS_NODE,      // #140: type of insertion nodes
    MARK_NODE,     // #141: type of a mark node
    ADJUST_NODE,   // #142: type of an adjust node

    LIGATURE_NODE, // #143: type of a ligature node
    DISC_NODE,     // #145: type of a discretionary node
    WHATSIT_NODE,  // #146: type of special extension nodes
    MATH_NODE,     // #147: type of a math node
    GLUE_NODE,     // #150: number of words to allocate for a glue specification

    KERN_NODE,     // #155: type of a kern node
    PENALTY_NODE,  // #157: type of a penalty node
    UNSET_NODE,    // #159: type for an unset node
};
// #138: number of words to allocate for a rule node
#define rulenodesize    4
// #138: -(2^30)  signifies a missing item
#define nullflag        (-1073741824L)
// #140: number of words to allocate for an insertion
#define insnodesize     5
// #141: number of words to allocate for most node types
#define smallnodesize   2
// #147: subtype for math node that introduces a formula
#define before          0
// #147: subtype for math node that winds up a formula
#define after           1
// #150: number of words to allocate for a glue specification
#define gluespecsize    4
// #150:  the orders of infinity (normal, fil, fill, or filll) 
// corresponding to the stretch and shrink values.
enum InfinityOrder {
    FIL = 1, // first-order infinity
    FILL,    // second-order infinity
    FILLL    // third-order infinity
};
// #155: subtype of kern nodes from \kern and \/
#define explicit    1
// #155: subtype of kern nodes from accents
#define acckern     2

// #157: "infinite" penalty value
#define INF_PENALTY     INF_BAD
// #157: "negatively infinite penalty value
#define EJECT_PENALTY   (-INF_PENALTY)


// [ #300~320: INPUT STACKS AND STATES ]

// p124#305: scanner status
enum ScannerStatus {
    SKIPPING = 1, // when passing conditional text
    DEFINING,     // when reading a macro definition
    MATCHING,     // when reading macro arguments
    ALIGNING,     // when reading an alignment preamble
    ABSORBING     // when reading a balanced text
};

// p125#307
#define TOKEN_LIST 0
// p125#307
enum TokenType {
    PARAMETER,   // parameter
    U_TEMPLATE,  // <u_j> template
    V_TEMPLATE,  // <v_j> template
    BACKED_UP,   // text to be reread
    INSERTED,    // inserted texts
    MACRO,       // defined control sequences
    OUTPUT_TEXT, // output routines

    EVERY_PAR_TEXT,     // \everypar
    EVERY_MATH_TEXT,    // \everymath
    EVERY_DISPLAY_TEXT, // \everydisplay
    EVERY_HBOX_TEXT,    // \everyhbox
    EVERY_VBOX_TEXT,    // \everyvbox
    EVERY_JOB_TEXT,     // \everyjob
    EVERY_CR_TEXT,      // \everycr
    MARK_TEXT,          // \topmark, etc.
    WRITE_TEXT          // \write
};


// [ #487~510: CONDITIONAL PROCESSING ]

// p181#487: Conditional processing
enum ConditionPrimitives {
    IF_CHAR_CODE,  // \if
    IF_CAT_CODE,   // \ifcat
    IF_INT_CODE,   // \ifnum
    IF_DIM_CODE,   // \ifdim
    IF_ODD_CODE,   // \ifodd
    IF_VMODE_CODE, // \ifvmode
    IF_HMODE_CODE, // \ifhmode
    IF_MMODE_CODE, // \ifmmode
    IF_INNER_CODE, // \ifinner
    IF_VOID_CODE,  // \ifvoid
    IF_HBOX_CODE,  // \ifhbox
    IF_VBOX_CODE,  // \ifvbox
    IF_X_CODE,     // \ifx
    IF_EOF_CODE,   // \ifeof
    IF_TRUE_CODE,  // \iftrue
    IF_FALSE_CODE, // \iffalse
    IF_CASE_CODE   // \ifcase
};


// [ #539~582: FONT METRIC DATA ]

// p198#544: tag field in a char_info_word
// that explain how to interpret the remainder field.
enum CharTag {
    NO_TAG,   // vanilla character
    LIG_TAG,  // character has a ligature/kerning program
    LIST_TAG, // character has a successor in a charlist
    EXT_TAG   // character is extensible
};

// p200#547
enum TFMParamVal {
    TFM_PARAM_MISSING, // TeX sets the missing parameters to zero

    SLANT_CODE,
    SPACE_CODE,
    SPACE_STRETCH_CODE,
    SPACE_SHRINK_CODE,
    X_HEIGHT_CODE,
    QUAD_CODE,
    EXTRA_SPACE_CODE
};


// [ #1055~1135: BUILDING BOXES AND LISTS ]

// #1058
enum hvSkipDiff {
    FIL_CODE,
    FILL_CODE,
    SS_CODE,
    FIL_NEG_CODE,
    SKIP_CODE,
    MSKIP_CODE
};


#endif // INC_TEX_CONSTANT