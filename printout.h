#pragma once

// printout only
#define chrcmd(x) (print(x), print(chrcode))

// [printout], tex
extern void printparam(long n);
extern void printcmdchr(QuarterWord cmd, HalfWord chrcode);
extern void printlengthparam(long n);
