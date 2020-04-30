#pragma once
#ifndef INC_TEX_TYPES
/// tex 相关的类型
#define INC_TEX_TYPES
#include <stdbool.h>
#include <stdint.h>


#ifdef _STDBOOL
// use <stdbool.h>
typedef bool Boolean; ///< 布尔类型定义
#else
// fallback
typedef unsigned char Boolean; ///< 布尔类型定义, 没有标准布尔类型的 fallback.
#define true  1 ///< 布尔值"真", 没有标准布尔类型的 fallback.
#define false 0 ///< 布尔值"假", 没有标准布尔类型的 fallback.
#endif // _STDBOOL


/*
    Pascal data types

ref:
    - [Standard type - Lazarus wiki](https://wiki.freepascal.org/Standard_type)
    - [Integer (computer science) -
Wikipedia](https://en.wikipedia.org/wiki/Integer_(computer_science)#Common_integral_data_types)

Types:
    UChar(8), Char(8), SChar(8)
    Integer(64), UInteger(64), Pointer(64)
    Real(64)
*/

typedef double Real; ///< `Real::double(64)` .

#ifdef _STDINT
// use <stdint.h>
typedef uint8_t     UChar;      ///< `UChar(8)` = [0, 255]
typedef UChar       Char;       ///< `Char(8)` = [0, 255]
typedef int8_t      SChar;      ///< `SChar(8)` = [−127, +127]

typedef int64_t     Integer;    ///< `Integer(64)` = @f$ [−2^{64}, 2^{64}) @f$
typedef uint64_t    UInteger;   ///< `UInteger(64)` = @f$ [0, 2^{64}) @f$
typedef size_t      Pointer;    ///< `Pointer::size_t(64)`

typedef int16_t     Int16;  ///< `Int16(16)` = @f$ [-2^{15}, 2^{15}) @f$
typedef uint16_t    UInt16; ///< `UInt16(16)` = @f$ [0, 2^{16}) @f$ = [0, 65536)
#else
// fallback
typedef unsigned char   UChar;      ///< `UChar(8)` = [0, 255]
typedef UChar           Char;       ///< `Char(8)` = [0, 255]
typedef signed char     SChar;      ///< `SChar(8)` = [−127, +127]

typedef long            Integer;    ///< `Integer::long(32/64)`
typedef unsigned long   UInteger;   ///< `Integer::ulong(32/64)`
typedef unsigned int    Pointer;    ///< `Pointer::Uint(32)`

typedef short           Int16;  ///< `Int16(16)` = @f$ [-2^{15}, 2^{15}) @f$
typedef unsigned short  UInt16; ///< `UInt16(16)` = @f$ [0, 2^{16}) @f$ = [0, 65536)
#endif // _STDINT


/*
    #18: Types in the outer block

Sections:
    18, 25, 38, 101, 109,
    113, 150, 212, 269, 300,
    [548], [594], 920, 925.

All types:
    #18: ASCIICode::UChar(8)
    #25: EightBits::UChar(8)
    #38: PackedASCIICode::UChar(8)
    #101 :Scaled::Integer(64)
    #101: NonNegativeInteger::UInteger(64)
    #101: SmallNumber::UChar(8)
    #109: GlueRatio::Real(64)
    #113: QuarterWord(8=>16)
    #113: HalfWord(16=>32)
    #113: TwoHalves(64)
    #113: FourQuarters(64)
    #113: MemoryWord(64)
    #150: GlueOrd(8)
    #212: ListStateRecord(366)
    #269: GroupCode(8)
    #300: InStateRecord(1344)
    #920: TriePointer::Pointer(64)
    #925: HyphPointer::Pointer(64)

*/

typedef UChar ASCIICode;       ///< #18: `ASCIICode::UChar(8)` = [0, 255]
typedef UChar EightBits;       ///< #25: `EightBits::UChar(8)` = [0, 255]
typedef UChar PackedASCIICode; ///< #38: `PackedASCIICode::UChar(8)` = [0, 255]

/// [p38#101]: `Scaled::Integer(64)` = @f$ [−2^{64}, 2^{64}) @f$
/// we need Pascal's `Integer(32/64)`,
/// dependent upon the bit size of the target machine .
typedef Integer Scaled;
/// [p38#101]: `NonNegativeInteger::UInteger(64)` = @f$ [0, 2^{64}) @f$
/// only need: (31bit)[0, 2^31) .
typedef UInteger NonNegativeInteger;
/// [p38#101]: `SmallNumber::UChar(8)` = [0, 255]
/// only need: (6bit)[0, 63] .
typedef UChar SmallNumber;
/// [p41#109]: `GlueRatio::Real(64)`
/// one-word representation of a glue expansion factor.
typedef Real GlueRatio;


/** @addtogroup S110x114
 * @{
 */

// [p43#113]
// 这里类型的大小人为扩大了一倍
#ifdef _STDINT
// use <stdint.h>
typedef uint16_t QuarterWord; ///< [p43#113]: `QuarterWord(8=>16)` .
typedef uint32_t HalfWord;    ///< [p43#113]: `HalfWord(16=>32)` .
#else
// fallback
typedef unsigned short QuarterWord; ///< [p43#113]: `QuarterWord(8=>16)` .
typedef int HalfWord;               ///< [p43#113]: `HalfWord(16=>32)` .
#endif // _STDINT

// _NOT_USE_
#if false
typedef UChar TwoChoices;   ///< p43#113: only need: (2bit)[1, 2]
typedef UChar fourchoices;  ///< p43#113: only need: (4bit)[1, 4]
typedef char ManyChoices;   ///< _NOT_USE_
#endif

/// [p43#113]: TwoHalves(64) .
///  th.rh, th.UU.lh, 
///  th.UU.U2.b0, th.UU.U2.b1
typedef struct {
    HalfWord rh; ///< (32)
    union {
        HalfWord lh; ///< (32)
        struct {
            QuarterWord b0, b1; ///< (16)
        } U2; ///< (16*2=32)
    } UU; ///< (32)
} TwoHalves; ///< (32*2=64)

/// [p43#113]: FourQuarters(64) .
///  fq.b0, fq.b1, fq.b2, fq.b3
/// 
/// TODO: 应当还原为 16 bit
///  font.c 中会依次读入 char 然后赋值
typedef struct {
    QuarterWord b0, b1, b2, b3; ///< (16)
} FourQuarters; ///< (16*4=64)

/// [p43#113]: MemoryWord(64) .
typedef union {
    Integer int_; ///< (64)
    GlueRatio gr; ///< (64)
    TwoHalves hh; ///< (64)
    FourQuarters qqqq; ///< (64)
    Integer sc; ///< (64)
} MemoryWord;
/** @}*/ // end group S110x114


/// [#150]: GlueOrd(8)
/// INFINITY to the 0, 1, 2, or 3 power.
/// 
/// only need: `[normal, filll] = [0, 3]`
typedef UChar GlueOrd;

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
    Int16 modefield; ///< (16) only need `[-203, 203]`
    Pointer headfield, tailfield; ///< (64)*2= 128
    Integer pgfield, mlfield; ///< (64)*2= 128
    MemoryWord auxfield; ///< (64)
} ListStateRecord; // (16+128+128+64) = (336)

/// #269: `GroupCode(8)`
/// save level for a level boundary.
/// 
/// only need `[0, MAX_GROUP_CODE] = [0, 16]`
typedef UChar GroupCode;

/// [#920]: `TriePointer::Pointer(64)`
/// an index into trie; `[0, TRIE_SIZE=131000]`.
typedef Pointer TriePointer;
/// [#925]: `HyphPointer::Pointer(64)`
/// an index into the ordered hash table; `[0, HYPH_SIZE=307]`.
typedef Pointer HyphPointer;

#endif // INC_TEX_TYPES