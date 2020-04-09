#pragma once
#ifndef _INC_GLOBAL_CONST
#define _INC_GLOBAL_CONST

#define dvibufsize 800

/// #7
#if defined(_DEBUG) || !defined(__OPTIMIZE__)
// debug mode
#define tt_DEBUG
#define tt_STAT
/// p5#8: init xref[16]:
///      111,
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