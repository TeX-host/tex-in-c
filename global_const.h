#pragma once
#ifndef _INC_GLOBAL_CONST
#define _INC_GLOBAL_CONST

#define dvibufsize 800

/// #7
//  MSVC debug mode || not in (GCC -O3 mode)
#if defined(_DEBUG) || !defined(__OPTIMIZE__)
// debug mode
/// 95#7: debug...gubed;
/// xref[10]:
///      78,  84,  93,  114,  165, 
///     166, 167, 172, 1031, 1338
#define tt_DEBUG
/// p5#7: stat...tats
/// xref[19]:
///          120,  121,  122,  123, 
///     125, 130,  252,  260,  283, 
///     284, 639,  829,  845,  855, 
///     863, 987, 1005, 1010, 1333
#define tt_STAT
/// p5#8: init...tini;
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
#endif

#endif // _INC_GLOBAL_CONST