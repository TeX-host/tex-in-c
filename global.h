#pragma once
#ifndef GLOBAL_H
#define GLOBAL_H
#include <setjmp.h> // setjmp

// funcs, inputln, [tex]
extern ASCIIcode buffer[];
extern short last;
extern short first;

// inputln, [tex]
extern short maxbufstack;
extern jmp_buf _JLfinalend;
extern strnumber formatident;
extern instaterecord curinput;
extern ASCIIcode xord[256];

// funcs, [tex]
extern unsigned char nameoffile[filenamesize];

#endif // #ifndef GLOBAL_H