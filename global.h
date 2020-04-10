#pragma once
#ifndef GLOBAL_H
#define GLOBAL_H
#include <setjmp.h> // setjmp
#include "str.h"    // StrNumber

// funcs, inputln, [tex]
extern ASCIICode buffer[];
extern short last;
extern short first;

// inputln, [tex]
extern short maxbufstack;
extern jmp_buf _JLfinalend;
extern StrNumber formatident;
extern InStateRecord curinput;
extern ASCIICode xord[256];

// funcs, [tex]
extern unsigned char nameoffile[filenamesize];

#endif // #ifndef GLOBAL_H