#pragma once
#ifndef TEX_H
// 待整理的常量。应合并到 tex_constant.h
#define TEX_H
#include "tex_types.h"
#include "tex_constant.h"

// 标记字符串序号
#define S(x) (x)
#define Static static

// #11: Constants in the outer block
#define dwa_do_8  ((int)16*1024*1024)
// [30_000 => 3_000_000]
// greatest index in TeX's internal `mem` array
#define MEM_MAX         3000000
// smallest index in TeX's internal `mem` array
#define MEM_MIN         0
// [500=>5000]
// maximum number of characters simultaneously present in
// current lines of open files and in 
// control sequences between \csname and \endcsname
#define BUF_SIZE        5000
// width of context lines on terminal error messages
#define ERROR_LINE      72
// [30, ERROR_LINE-15=57]
// width of first lines of contexts in terminal error messages
#define HALF_ERROR_LINE 42
// [60, ...]
// width of longest text lines output; should be at least 60
#define MAX_PRINT_LINE  79
// maximum number of simultaneous input sources 
#define stacksize       200
// maximum number of input files and
// error insertions that can be going on simultaneously
#define MAX_IN_OPEN     6
// [#12] smallest internal font number
#define FONT_BASE       0
// maximum internal font number
#define FONT_MAX        75
// [20_000 => 200_000]
// number of words of font_info for all fonts
#define FONT_MEM_SIZE   200000
// maximum number of simultaneous macro parameters
#define paramsize       60
// maximum number of semantic levels simultaneously active
#define nestsize        40

// space for saving values outside of current group
#define SAVE_SIZE        600
// [8_000 => 131_000]
// space for hyphenation patterns;
// should be larger for INITEX than it is in production versions of TeX
#define TRIE_SIZE       131000
// [500=>5000]
// space for "opcodes" in the hyphenation patterns
#define trieopsize      5000
// [40=>240] 
// file names shouldn’t be longer than this
#define FILE_NAME_SIZE    240

#define TEX_BANNER      "This is TeX, Version 3.14159"
// string of length file name size; 
// tells where the string pool appears
#define poolname        "TeXformats:TEX.POOL                     "

// [#12] smallest index in the `mem` array dumped by INITEX
#define MEM_BOT     0
// [#12] [30_000 => 3_000_000] 
// largest index in the `mem` array dumped by INITEX
#define MEM_TOP     3000000
// [#12] [2100 => 210_000]
// maximum number of control sequences
#define HASH_SIZE   210000
// [#12] [1777 => 171_553]
// a prime number equal to about 85% of `HASH_SIZE`
#define HASH_PRIME  171553
// [#12] another prime; the number of \hyphenation exceptions
#define HYPH_SIZE   307

// [#16] symbolic name for a null constant
#define empty           0
// [#19] ordinal number of the smallest element of text_char
#define firsttextchar   0
// [#19] ordinal number of the largest element of text_char
#define lasttextchar    255
// [#22] ASCII code that might disappear
#define nullcode        0
// [#22] ASCII code used at end of line
#define carriagereturn  13
// [#22] ASCII code that many systems prohibit in text files
#define invalidcode     127


// [#110]: smallest allowable value in a QuarterWord
#define MIN_QUARTER_WORD    0
// [#110]: largest allowable value in a QuarterWord
// 1/4 word = (8bit)[0, 255]
#define MAX_QUARTER_WORD    255
// [#110]: smallest allowable value in a HalfWord
#define MIN_HALF_WORD       0
// [#110]: largest allowable value in a HalfWord
// 1/2 word = (16bit)[0, 65535]
// mutst > 65535 (2^16-1)
#define MAX_HALF_WORD       655350000L

// [#124] the `link` of an empty variable-size node
#define emptyflag       MAX_HALF_WORD
// ??? nodesize

// [#162] list of insertion data for current page
#define pageinshead     (MEM_TOP-charnodesize+1)
// [#162] vlist of items not yet on current page
#define contribhead     (pageinshead-charnodesize)
// [#162] vlist for current page
#define pagehead        (contribhead-charnodesize)
// [#162] head of a temporary list of some kind
#define temphead        (pagehead-charnodesize)
// [#162] head of a temporary list of another kind
#define holdhead        (temphead-charnodesize)
// [#162] head of adjustment list returned by hpack
#define adjusthead      (holdhead-charnodesize)
// [#162] head of active list in line break, needs two words
#define active          (adjusthead-charnodesize-charnodesize)
// [#162]
#define lastactive      active
// [#162] head of preamble list for alignments
#define alignhead       (active-charnodesize)
// [#162] tail of spanned-width lists
#define endspan         (alignhead-charnodesize)
// [#162] a constant token list
#define omittemplate    (endspan-charnodesize)
// [#162] permanently empty list
#define nulllist        (omittemplate-charnodesize)
// [#162] a ligature masquerading as a `char_node`
#define ligtrick        (nulllist-charnodesize)
// [#162] used for scrap information
#define garbage         (ligtrick)
// [#162] head of token list built by `scan_keyword`
#define backuphead      (ligtrick-charnodesize)
// [#162] smallest statically allocated word in the one-word `mem`
#define himemstatmin    (backuphead)


#define varcode         28672
/*
#define CS_TOKEN_FLAG     4095
*/
#define CS_TOKEN_FLAG     (dwa_do_8*16-1)

#define maxdimen        1073741823L
#define nullflag        (-1073741824L)
#define infinity        2147483647L
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

// #207:  
enum CatCode {
    ESCAPE,      // ['\\'] escape delimiter
    LEFT_BRACE,  // ['{'] beginning of a group
    RIGHT_BRACE, // ['}'] ending of a group
    MATH_SHIFT,  // ['$'] mathematics shift character
    TAB_MARK,    // ['&' | "\span"] alignment delimiter

    CAR_RET = 5, // ['\r' | "\cr" | "\crcr"] end of line
    MAC_PARAM,   // ['#'] macro parameter symbol
    SUP_MARK,    // ['^'] superscript
    SUB_MARK,    // ['_'] subscript
    IGNORE,      // ['\0' | "^^@"] characters to ignore

    SPACER = 10, // [' ' | '\t'] characters equivalent to blank space
    LETTER,      // [A-Za-z] characters regarded as letters
    OTHER_CHAR,  // none of the special character types
    ACTIVE_CHAR, // ['~'] characters that invoke macros
    COMMENT,     // ['%'] characters that introduce comments

    INVALID_CHAR = 15, // ['\127' | "^^?"] characters that shouldn't appear
}; // #207: CatCode

// [#207]
#define relax       ESCAPE       // [" \relax"] do nothing
#define outparam    CAR_RET      // output a macro parameter
#define endv        IGNORE       // end of <v_j>  list in alignment template
#define parend      ACTIVE_CHAR  // ["\par"] end of paragraph
#define match       ACTIVE_CHAR  // match a macro parameter
#define endmatch    COMMENT      // end of parameters to macro
#define stop        COMMENT      // ["\end" | "\dump"] end of job
#define delimnum    INVALID_CHAR // ["\delimiter"] specify delimiter numerically
#define maxcharcode INVALID_CHAR // largest catcode for individual characters


// #207
// enum TexCommandCode {

// };
#define charnum         16
#define mathcharnum     17
#define mark_           18
#define xray            19
#define makebox         20

#define hmove           21
#define vmove           22
#define unhbox          23
#define unvbox          24
#define removeitem      25

#define hskip           26
#define vskip           27
#define mskip           28
#define kern            29
#define mkern           30

#define leadership      31
#define halign          32
#define valign          33
#define noalign         34
#define vrule           35

#define hrule           36
#define insert_         37
#define vadjust         38
#define ignorespaces    39
#define afterassignment  40

#define aftergroup      41
#define breakpenalty    42
#define startpar        43
#define italcorr        44
#define accent          45

#define mathaccent      46
#define discretionary   47
#define eqno            48
#define leftright       49
#define mathcomp        50

#define limitswitch     51
#define above           52
#define mathstyle       53
#define mathchoice      54
#define nonscript       55

#define vcenter         56
#define caseshift       57
#define message         58
#define extension       59
#define instream        60

#define begingroup      61
#define endgroup        62
#define omit            63
#define exspace         64
#define noboundary      65

#define radical         66
#define endcsname       67
#define mininternal     68
#define chargiven       68
#define mathgiven       69
#define lastitem        70
#define maxnonprefixedcommand  70

#define toksregister    71
#define assigntoks      72
#define assignint       73
#define assigndimen     74
#define assignglue      75
#define assignmuglue    76
#define assignfontdimen  77
#define assignfontint   78
#define setaux          79
#define setprevgraf     80
#define setpagedimen    81
#define setpageint      82
#define setboxdimen     83
#define setshape        84
#define defcode         85
#define deffamily       86
#define setfont         87
#define deffont         88
#define register_       89
#define maxinternal     89
#define advance         90
#define multiply        91
#define divide          92
#define prefix          93
#define let             94
#define shorthanddef    95
#define readtocs        96
#define def             97
#define setbox          98
#define hyphdata        99
#define setinteraction  100

#define maxcommand      100
#define undefinedcs     (maxcommand + 1)
#define expandafter     (maxcommand + 2)
#define noexpand        (maxcommand + 3)
#define input           (maxcommand + 4)
#define iftest          (maxcommand + 5)
#define fiorelse        (maxcommand + 6)
#define csname          (maxcommand + 7)
#define convert         (maxcommand + 8)
#define the             (maxcommand + 9)
#define topbotmark      (maxcommand + 10)
#define call            (maxcommand + 11)
#define longcall        (maxcommand + 12)
#define outercall       (maxcommand + 13)
#define longoutercall   (maxcommand + 14)
#define endtemplate     (maxcommand + 15)
#define dontexpand      (maxcommand + 16)
#define glueref         (maxcommand + 17)
#define shaperef        (maxcommand + 18)
#define boxref          (maxcommand + 19)
#define data            (maxcommand + 20)


#define V_MODE           1 // vertical mode
#define H_MODE           (V_MODE + maxcommand + 1) // horizontal mode
#define M_MODE           (H_MODE + maxcommand + 1) // math mode

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

#define skipbase        (gluebase + gluepars)
#define muskipbase      (skipbase + 256)
#define localbase       (muskipbase + 256)
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

#define countbase       (intbase + intpars)
#define delcodebase     (countbase + 256)
#define dimenbase       (delcodebase + 256)

#define scaledbase      (dimenbase + dimenpars)
#define eqtbsize        (scaledbase + 255)


// #269
// enum GroupCode {};
#define bottomlevel     0
#define simplegroup     1
#define hboxgroup       2
#define adjustedhboxgroup  3
#define vboxgroup       4
#define vtopgroup       5
#define aligngroup      6
#define noaligngroup    7
#define outputgroup     8
#define mathgroup       9
#define discgroup       10
#define insertgroup     11
#define vcentergroup    12
#define mathchoicegroup  13
#define semisimplegroup  14
#define mathshiftgroup  15
#define mathleftgroup   16
#define MAX_GROUP_CODE      16


#define leftbracetoken  (1*dwa_do_8)
#define leftbracelimit  (2*dwa_do_8)
#define rightbracetoken  (2*dwa_do_8)
#define rightbracelimit  (3*dwa_do_8)
#define mathshifttoken  (3*dwa_do_8)
#define tabtoken        (4*dwa_do_8)
#define outparamtoken   (5*dwa_do_8)
#define spacetoken      (10*dwa_do_8 + 32)
#define lettertoken     (11*dwa_do_8)
#define othertoken      (12*dwa_do_8)
#define matchtoken      (13*dwa_do_8)
#define endmatchtoken   (14*dwa_do_8)

// #303
// `state` code when scanning a line of characters
#define MID_LINE    1
// `state` code when ignoring blanks
#define SKIP_BLANKS (maxcharcode + 2)
// `state` code at start of line
#define NEW_LINE    (maxcharcode + maxcharcode + 3)

#define switch_         25
#define startcs         26
#define noexpandflag    257
#define topmarkcode     0
#define firstmarkcode   1
#define botmarkcode     2
#define splitfirstmarkcode  3
#define splitbotmarkcode  4
#define intval          0
#define dimenval        1
#define glueval         2
#define muval           3
#define identval        4
#define tokval          5

#define inputlinenocode  (glueval + 1)
#define badnesscode     (glueval + 2)
#define octaltoken      (othertoken + 39)
#define hextoken        (othertoken + 34)
#define alphatoken      (othertoken + 96)
#define pointtoken      (othertoken + 46)
#define continentalpointtoken  (othertoken + 44)
#define zerotoken       (othertoken + 48)
#define Atoken          (lettertoken + 65)
#define otherAtoken     (othertoken + 65)

#define attachfraction  88
#define attachsign      89
#define defaultrule     26214

#define closed          2
#define justopen        1

#define FORMAT_DEFAULT_LENGTH  20
#define formatarealength  11
#define formatextlength  4



#define nonaddress      0
#define badtfm          11
#define movepast        13
#define finrule         14
#define nextp           15
#define exactly         0
#define additional      1
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

#define totalmathsyparams  22
#define totalmathexparams  13
#define donewithnoad    80
#define donewithnode    81
#define checkdimensions  82
#define deleteq         83
#define alignstacknodesize  5
#define spancode        256
#define crcode          257

#define crcrcode        (crcode + 1)

#define spannodesize    2
#define tightfit        3
#define loosefit        1
#define veryloosefit    0
#define decentfit       2
#define activenodesize  3
#define unhyphenated    0
#define hyphenated      1
#define passivenodesize  2
#define deltanodesize   7
#define deltanode       2
#define deactivate      60
#define updateheights   90
#define insertsonly     1
#define boxthere        2
#define pageinsnodesize  4
#define inserting       0
#define splitup         1
#define contribute      80
#define bigswitch       60
#define appendnormalspace  120



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

// #1341
// enum WhatsitsSubtype {};
#define opennode        0
#define writenode       1
#define closenode       2
#define specialnode     3
#define immediatecode   4
#define setlanguagecode  5

#define languagenode    4

#define endtemplatetoken  (CS_TOKEN_FLAG + frozenendtemplate)

#define kernbaseoffset  32768
#define zeroglue        MEM_BOT
#define filglue         (zeroglue + gluespecsize)
#define fillglue        (filglue + gluespecsize)
#define ssglue          (fillglue + gluespecsize)
#define filnegglue      (ssglue + gluespecsize)

#define lomemstatmax    (filnegglue + gluespecsize - 1)
#define deplorable      100000L
#define shipoutflag     (boxflag + 512)
#define leaderflag      (boxflag + 513)

#define NULL_FONT        0

#define endwritetoken   (CS_TOKEN_FLAG + endwrite)

#endif // #ifndef TEX_H