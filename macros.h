#pragma once
#ifndef MACROS_H
/// 全局常用的宏
#define MACROS_H

// fonts, str, tex
#define pget(x) fread(&x, 8, 1, fmtfile)
#define pput(x) fwrite(&x, 8, 1, fmtfile)

/// [p43#112]
#define qi(x) (x)
#define qo(x) (x)

#endif // #ifndef MACROS_H