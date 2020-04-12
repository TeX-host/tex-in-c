#pragma once
#ifndef GLOBAL_H
#define GLOBAL_H
#include <setjmp.h> // setjmp
#include "str.h"    // StrNumber

// funcs, inputln, [tex]
extern ASCIICode buffer[];
extern UInt16 last;
extern UInt16 first;

// inputln, [tex]
extern UInt16 max_buf_stack;
extern jmp_buf _JLfinalend;
extern StrNumber formatident;
extern InStateRecord curinput;
extern ASCIICode xord[256];

// funcs, [tex]
extern Char name_of_file[filenamesize + 1];

#endif // #ifndef GLOBAL_H