#pragma once
#ifndef _INC_GLOBAL_CONST
/// 全局常量
#define _INC_GLOBAL_CONST

/// 使用 TeX82 的参数
// #define tt_USE_TEX82

// #7
//  MSVC debug mode || not in (GCC -O3 mode)
#if defined(_DEBUG) || !defined(__OPTIMIZE__)
// debug mode

/// [95#7]: `debug...gubed` .
/// xref[10]:
///      78,  84,  93,  114,  165, 
///     166, 167, 172, 1031, 1338
#define tt_DEBUG
/// [p5#7]: `stat...tats` .
/// xref[19]:
///          120,  121,  122,  123, 
///     125, 130,  252,  260,  283, 
///     284, 639,  829,  845,  855, 
///     863, 987, 1005, 1010, 1333
#define tt_STAT
/// [p5#8]: `init...tini` .
/// xref[16]:  111,
///       47,   50,  131,  264,  891,
///      942,  943,  947,  950, 1252,
///     1302, 1325, 1332, 1335, 1336
#define tt_INIT
#else
// release mode

#undef tt_DEBUG
#undef tt_STAT
#undef tt_INIT
#define NDEBUG // no assertsion
#endif

#define Static static

/// 辅助宏定义, 用于拆分模块是使用.
#define USE_SPLIT_MOD

/// 直接使用字符串，而不是 StrNumber
#define USE_REAL_STR


#define dwa_do_8    ((int)16 * 1024 * 1024)

#endif // _INC_GLOBAL_CONST


// 预定义分组，固定排序。

/**
 * @defgroup S1x16_P3x9 PART 1: Introduction
 * @defgroup S17x24_P10x12 PART 2: The character set
 * @defgroup S25x37_P13x18 PART 3: Input and output
 * @defgroup S38x53_P19x23 PART 4: String handling
 * @defgroup S54x71_P24x29 PART 5: On-line and off-line printing
 * @defgroup S72x98_P30x37 PART 6: Reporting errors
 * @defgroup S99x109_P38x41 PART 7: Arithmetic with scaled dimensions
 * @defgroup S110x114_P42x43 PART 8: Packed data
 * @defgroup S115x132_P44x49 PART 9: Dynamic memory allocation
 * @defgroup S133x161_P50x57 PART 10: Data structures for boxes and their friends
 * @defgroup S162x172_P58x61 PART 11: Memory layout
 * @defgroup S173x198_P62x68 PART 12: Displaying boxes
 * @defgroup S199x202_P69x70 PART 13: Destroying boxes
 * @defgroup S203x206_P71x72 PART 14: Copying boxes
 * @defgroup S207x210_P73x76 PART 15: The command codes
 * @defgroup S211x219_P77x80 PART 16: The semantic nest
 * @defgroup S220x255_P81x101 PART 17: The table of equivalents
 * @defgroup S256x267_P102x108 PART 18: The hash table
 * @defgroup S268x288_P109x114 PART 19: Saving and restoring equivalents
 * @defgroup S289x296_P115x118 PART 20: Token lists
 * @defgroup S297x299_P119x120 PART 21: Introduction to the syntactic routines
 * @defgroup S300x320_P121x130 PART 22: Input stacks and states
 * @defgroup S321x331_P131x133 PART 23: Maintaining the input stacks
 * @defgroup S332x365_P134x143 PART 24: Getting the next token
 * @defgroup S366x401_P144x154 PART 25: Expanding the next token
 * @defgroup S402x463_P155x173 PART 26: Basic scanning subroutines
 * @defgroup S464x486_P174x180 PART 27: Building token lists
 * @defgroup S487x510_P181x187 PART 28: Conditional processing
 * @defgroup S511x538_P188x195 PART 29: File names
 * @defgroup S539x582_P196x213 PART 30: Font metric data
 * @defgroup S583x591_P214x219 PART 31: Device-independent file format
 * @defgroup S592x643_P220x238 PART 32: Shipping pages out
 * @defgroup S644x679_P239x248 PART 33: Packaging
 * @defgroup S680x698_P249x257 PART 34: Data structures for math mode
 * @defgroup S699x718_P258x264 PART 35: Subroutines for math mode
 * @defgroup S719x767_P265x284 PART 36: Typesetting math formulas
 * @defgroup S768x812_P285x301 PART 37: Alignment
 * @defgroup S813x861_P302x318 PART 38: Breaking paragraphs into lines
 * @defgroup S862x890_P319x329 PART 39: Breaking paragraphs into lines, continued
 * @defgroup S891x899_P330x333 PART 40: Pre-hyphenation
 * @defgroup S900x918_P334x343 PART 41: Post-hyphenation
 * @defgroup S919x941_P344x349 PART 42: Hyphenation
 * @defgroup S942x966_P350x359 PART 43: Initializing the hyphenation tables
 * @defgroup S967x979_P360x365 PART 44: Breaking vertical lists into pages
 * @defgroup S980x1028_P366x382 PART 45: The page builder
 * @defgroup S1029x1054_P383x394 PART 46: The chief executive
 * @defgroup S1055x1135_P395x416 PART 47: Building boxes and lists
 * @defgroup S1136x1207_P417x434 PART 48: Building math lists
 * @defgroup S1208x1298_P435x454 PART 49: Mode-independent processing
 * @defgroup S1299x1329_P455x464 PART 50: Dumping and undumping the tables
 * @defgroup S1330x1337_P465x469 PART 51: The main program
 * @defgroup S1338x1339_P470x471 PART 52: Debugging
 * @defgroup S1340x1379_P472x481 PART 53: Extensions
 */
