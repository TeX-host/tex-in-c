#pragma once
#ifndef MACROS_H
/// 全局常用的宏
#define MACROS_H

// fonts, str, tex
#define pget(x) fread(&x, 8, 1, fmtfile)
#define pput(x) fwrite(&x, 8, 1, fmtfile)

// fonts, tex

/// a |halfword| code that can't match a real character}
#define NON_CHAR 256

#endif // #ifndef MACROS_H