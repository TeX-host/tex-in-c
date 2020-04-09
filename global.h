#pragma once
#ifndef GLOBAL_H
#define GLOBAL_H
#include <setjmp.h> // setjmp

// tex.c#L52
extern unsigned char nameoffile[filenamesize];
extern ASCIIcode buffer[];
extern short last;
extern short first;
extern short maxbufstack;
extern jmp_buf _JLfinalend;
extern strnumber formatident;
extern instaterecord curinput;
extern ASCIIcode xord[256];

#endif // #ifndef GLOBAL_H