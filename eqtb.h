#pragma once
#ifndef INC_EQTB_H
//
#define INC_EQTB_H
#include "tex_types.h"
#include "tex_constant.h"


/** @addtogroup S220x255_P81x101
 * @{
 */

/** [p82#222] [region 1] eqtb[ACTIVE_BASE, (HASH_BASE - 1)] holds
 *  current equivalents of single-character control sequences.
 */
/// [p82#222] beginning of [region 1], for active character equivalents.
#define ACTIVE_BASE     1
/// [p82#222] equivalents of one-character control sequences.
#define SINGLE_BASE     (ACTIVE_BASE + 256)
/// [p82#222] equivalent of \\csname\\endcsname
#define NULL_CS         (ACTIVE_BASE + 256)

/// [p81#221]
#define eq_level_field(x)   x.hh.UU.U2.b1
#define eq_type_field(x)    x.hh.UU.U2.b0
#define equiv_field(x)      x.hh.rh

/// ::QuarterWord = EqLevel, level of definition.
#define eq_level(x) eq_level_field(eqtb[x - ACTIVE_BASE])
/// ::QuarterWord = TexCommandCode, command code for equivalent.
#define eq_type(x)  eq_type_field(eqtb[x - ACTIVE_BASE])
/** [p81#221] equiv::HalfWord, equivalent value.
 *
 * may be
 *  + a font number,
 *  + a pointer into mem,
 *  + a variety of other things
 */
#define equiv(x)    equiv_field(eqtb[x - ACTIVE_BASE])
// [#221]
typedef enum _EqLevel {
    /// level for undefined quantities.
    LEVEL_ZERO = MIN_QUARTER_WORD,
    /// outermost level for defined quantities.
    LEVEL_ONE,
} EqLevel;


/** [p82#222] [region 2] eqtb[HASH_BASE, (GLUE_BASE - 1)] holds
 *  current equivalents of multiletter control sequences.
 */
/// [p82#222] beginning of [region 2], for the hash table.
#define HASH_BASE   (NULL_CS + 1)

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


/** [p83#224] [region 3] eqtb[HASH_BASE, (GLUE_BASE - 1)] holds 
 *  current equivalents of glue parameters like the current baselineskip.
 */
/// [p82#222] beginning of region 3
#define GLUE_BASE       (UNDEFINED_CONTROL_SEQUENCE + 1)
/// [p83#224] table of 256 “skip” registers
#define SKIP_BASE       (GLUE_BASE + GLUE_PARS)
/// [p83#224] table of 256 “muskip” registers.
#define MU_SKIP_BASE    (SKIP_BASE + 256)

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

/// [p83#224] |mem| location of glue specification.
#define skip(x)     equiv(SKIP_BASE + x)
/// [p83#224] |mem| location of math glue spec.
#define muskip(x)   equiv(MU_SKIP_BASE + x)
/// [p83#224] mem location of glue specification.
#define glue_par(x) equiv(GLUE_BASE + x)

#define lineskip                glue_par(LINE_SKIP_CODE)
#define baselineskip            glue_par(BASELINE_SKIP_CODE)
#define parskip                 glue_par(PAR_SKIP_CODE)
#define abovedisplayskip        glue_par(ABOVE_DISPLAY_SKIP_CODE)
#define belowdisplayskip        glue_par(BELOW_DISPLAY_SKIP_CODE)
#define abovedisplayshortskip   glue_par(ABOVE_DISPLAY_SHORT_SKIP_CODE)
#define belowdisplayshortskip   glue_par(BELOW_DISPLAY_SHORT_SKIP_CODE)
#define leftskip                glue_par(LEFT_SKIP_CODE)
#define rightskip               glue_par(RIGHT_SKIP_CODE)
#define topskip                 glue_par(TOP_SKIP_CODE)
#define splittopskip            glue_par(SPLIT_TOP_SKIP_CODE)
#define tabskip                 glue_par(TAB_SKIP_CODE)
#define spaceskip               glue_par(SPACE_SKIP_CODE)
#define xspaceskip              glue_par(XSPACE_SKIP_CODE)
#define parfillskip             glue_par(PAR_FILL_SKIP_CODE)
#define thinmuskip              glue_par(THIN_MU_SKIP_CODE)
#define medmuskip               glue_par(MED_MU_SKIP_CODE)
#define thickmuskip             glue_par(THICK_MU_SKIP_CODE)


/** [p87#230] [region 4] eqtb[GLUE_BASE, (LOCAL_BASE - 1)] holds
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

// [p87#230]
#define parshapeptr     equiv(PAR_SHAPE_LOC)
#define outputroutine   equiv(OUTPUT_ROUTINE_LOC)
#define everypar        equiv(EVERY_PAR_LOC)
#define everymath       equiv(EVERY_MATH_LOC)
#define everydisplay    equiv(EVERY_DISPLAY_LOC)
#define everyhbox       equiv(EVERY_HBOX_LOC)
#define everyvbox       equiv(EVERY_VBOX_LOC)
#define everyjob        equiv(EVERY_JOB_LOC)
#define everycr         equiv(EVERY_CR_LOC)
#define errhelp         equiv(ERR_HELP_LOC)

/// [p87#230]
#define toks(x)         equiv(TOKS_BASE + x)
#define box(x)          equiv(BOX_BASE + x)
#define cur_font        equiv(CUR_FONT_LOC)
#define fam_fnt(x)      equiv(MATH_FONT_BASE + x)
#define cat_code(x)     equiv(CAT_CODE_BASE + x)
#define lc_code(x)      equiv(LC_CODE_BASE + x)
#define uc_code(x)      equiv(UC_CODE_BASE + x)
#define sf_code(x)      equiv(SF_CODE_BASE + x)
/// Note: |mathcode(c)| is the true math code plus |minhalfword|
#define math_code(x)    equiv(MATH_CODE_BASE + x)

/// [#232]
#define NULL_FONT       FONT_BASE
/// [#232] math code meaning “use the current family”
#define VAR_CODE        28672


/** [p91#236] [region 5] eqtb[INT_BASE, (DIMEN_BASE - 1)] holds
 *  current equivalents of fullword integer parameters like
 *  the current hyphenation penalty.
 */
/// [p87#230] beginning of [region 5].
#define INT_BASE        (MATH_CODE_BASE + 256)
/// [p92#236] 256 user \\count registers.
#define COUNT_BASE      (INT_BASE + INT_PARS)
/// [p92#236] 256 delimiter code mappings.
#define DEL_CODE_BASE   (COUNT_BASE + 256)

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

    INT_PARS = 55, ///< total number of integer parameters
}; // [#236] enum DelimiterCodesTable

/// [p92]
#define del_code(x)     eqtb[DEL_CODE_BASE + x - ACTIVE_BASE].int_
#define count(x)        eqtb[COUNT_BASE + x - ACTIVE_BASE].int_
/// [p92] Integer parameters
#define int_par(x)      eqtb[INT_BASE + x - ACTIVE_BASE].int_

#define pretolerance            int_par(pretolerancecode)
#define tolerance               int_par(tolerancecode)

#define linepenalty             int_par(linepenaltycode)
#define hyphenpenalty           int_par(hyphenpenaltycode)
#define exhyphenpenalty         int_par(exhyphenpenaltycode)
#define clubpenalty             int_par(clubpenaltycode)
#define widowpenalty            int_par(widowpenaltycode)
#define displaywidowpenalty     int_par(displaywidowpenaltycode)
#define brokenpenalty           int_par(brokenpenaltycode)
#define binoppenalty            int_par(binoppenaltycode)
#define relpenalty              int_par(relpenaltycode)
#define predisplaypenalty       int_par(predisplaypenaltycode)
#define postdisplaypenalty      int_par(postdisplaypenaltycode)
#define interlinepenalty        int_par(interlinepenaltycode)

#define doublehyphendemerits    int_par(doublehyphendemeritscode)
#define finalhyphendemerits     int_par(finalhyphendemeritscode)
#define adjdemerits             int_par(adjdemeritscode)
#define mag                     int_par(magcode)
#define delimiterfactor         int_par(delimiterfactorcode)
#define looseness               int_par(loosenesscode)
#define tex_time                int_par(timecode)
#define day                     int_par(daycode)
#define month                   int_par(monthcode)
#define year                    int_par(yearcode)
#define showboxbreadth          int_par(showboxbreadthcode)
#define showboxdepth            int_par(showboxdepthcode)
#define hbadness                int_par(hbadnesscode)
#define vbadness                int_par(vbadnesscode)
#define pausing                 int_par(pausingcode)

#define tracingonline           int_par(tracingonlinecode)
#define tracingmacros           int_par(tracingmacroscode)
#define tracingstats            int_par(tracingstatscode)
#define tracingparagraphs       int_par(tracingparagraphscode)
#define tracingpages            int_par(tracingpagescode)
#define tracingoutput           int_par(tracingoutputcode)
#define tracinglostchars        int_par(tracinglostcharscode)
#define tracingcommands         int_par(tracingcommandscode)
#define tracingrestores         int_par(tracingrestorescode)

#define uchyph                  int_par(uchyphcode)
#define outputpenalty           int_par(outputpenaltycode)
#define maxdeadcycles           int_par(maxdeadcyclescode)
#define hangafter               int_par(hangaftercode)
#define floatingpenalty         int_par(floatingpenaltycode)
#define globaldefs              int_par(globaldefscode)
#define curfam                  int_par(curfamcode)
#define ESCAPE_CHAR             int_par(ESCAPE_CHARcode)
#define defaulthyphenchar       int_par(defaulthyphencharcode)
#define defaultskewchar         int_par(defaultskewcharcode)
#define end_line_char           int_par(endlinecharcode)
#define newlinechar             int_par(newlinecharcode)
#define language                int_par(languagecode)
#define lefthyphenmin           int_par(lefthyphenmincode)
#define righthyphenmin          int_par(righthyphenmincode)
#define holdinginserts          int_par(holdinginsertscode)
#define errorcontextlines       int_par(errorcontextlinescode)


/** [p99#247] [region 6] eqtb[DIMEN_BASE, EQTB_SIZE] holds
 *  current equivalents of fullword dimension parameters like
 *  the current hsize or amount of hanging indentation.
 */
/// [p92#236] beginning of region 6.
#define DIMEN_BASE      (DEL_CODE_BASE + 256)
/// [#247]
#define SCALED_BASE     (DIMEN_BASE + dimenpars)
#define EQTB_SIZE       (SCALED_BASE + 255)

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

// [p99#247] Dimen pars
#define dimen(x)                eqtb[SCALED_BASE + x - ACTIVE_BASE].sc
#define dimen_par(x)            eqtb[DIMEN_BASE + x - ACTIVE_BASE].int_

#define parindent               dimen_par(parindentcode)
#define mathsurround            dimen_par(mathsurroundcode)
#define lineskiplimit           dimen_par(lineskiplimitcode)
#define hsize                   dimen_par(hsizecode)
#define vsize                   dimen_par(vsizecode)
#define maxdepth                dimen_par(maxdepthcode)
#define splitmaxdepth           dimen_par(splitmaxdepthcode)
#define boxmaxdepth             dimen_par(boxmaxdepthcode)
#define hfuzz                   dimen_par(hfuzzcode)
#define vfuzz                   dimen_par(vfuzzcode)
#define delimitershortfall      dimen_par(delimitershortfallcode)
#define nulldelimiterspace      dimen_par(nulldelimiterspacecode)
#define scriptspace             dimen_par(scriptspacecode)
#define predisplaysize          dimen_par(predisplaysizecode)
#define displaywidth            dimen_par(displaywidthcode)
#define displayindent           dimen_par(displayindentcode)
#define overfullrule            dimen_par(overfullrulecode)
#define hangindent              dimen_par(hangindentcode)
#define hoffset                 dimen_par(hoffsetcode)
#define voffset                 dimen_par(voffsetcode)
#define emergencystretch        dimen_par(emergencystretchcode)
/** @}*/ // end group S220x255_P81x101

/** @addtogroup S268x288_P109x114
 * @{
 */
/// [p109#268] classifies a |savestack| entry
#define savetype(x)     savestack[x].hh.UU.U2.b0
/// [p109#268] saved level for regions 5 and 6, or group code
#define savelevel(x)    savestack[x].hh.UU.U2.b1
/// [p109#268] |eqtb| location or |savestack| location
#define saveindex(x)    savestack[x].hh.rh

/// [p111#274]
#define saved(x)        savestack[saveptr + x].int_

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
/** @}*/ // end group S268x288_P109x114

// eqtb
extern MemoryWord eqtb[EQTB_SIZE - ACTIVE_BASE + 1];
extern QuarterWord xeqlevel[EQTB_SIZE - INT_BASE + 1];

// eqtv_save
extern MemoryWord savestack[SAVE_SIZE + 1];
extern UInt16 saveptr;
extern UInt16 maxsavestack;
extern QuarterWord curlevel;
extern GroupCode curgroup;


extern void eqtb_init();
extern void eqtb_init_once();
extern void print_skip_param(Integer n);
extern void begindiagnostic(void);
extern void enddiagnostic(Boolean blankline);
extern void showeqtb(HalfWord n);

// fonts only
extern Integer get_defaultskewchar(void);
extern Integer get_defaulthyphenchar(void);

extern void eqtb_save_init();
extern void newsavelevel(GroupCode c);
extern void eqdefine(HalfWord p, QuarterWord t, HalfWord e);
extern void eqworddefine(HalfWord p, long w);
extern void geqdefine(HalfWord p, QuarterWord t, HalfWord e);
extern void geqworddefine(HalfWord p, long w);
extern void saveforafter(HalfWord t);
extern void unsave(void);
extern void preparemag(void);

extern Boolean use_independence_date;

#endif /* INC_EQTB_H */