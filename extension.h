#pragma once
#ifndef INC_EXTENSION_H
///
#define INC_EXTENSION_H
#include "lexer.h" // [macro] CS_TOKEN_FLAG
#include "eqtb.h"  // [macro] END_WRITE
#include "hyphen.h" // [var] curlang, lhyf, rhyf
#include "tex_types.h"

/** @addtogroup S1340x1379_P472x481
 * @{
 */
#define writenodesize   2
#define opennodesize    3

// [p472#1341]
// enum WhatsitsSubtype {};
#define opennode        0
#define writenode       1
#define closenode       2
#define specialnode     3
#define languagenode    4

/// [p472#1341]
#define whatlang(x)  link(x+1) /* language number, in the range |0..255|}*/
#define whatlhm(x)  type(x+1) /* minimum left fragment, in the range |1..63|}*/
#define whatrhm(x)  subtype(x+1) /* minimum right fragment, in the range |1..63|}*/
#define writetokens(x)    link(x+1) /* reference count of token list to write}*/
#define writestream(x)    info(x+1) /* stream number (0 to 17)}*/
#define openname(x)    link(x+1) /* string number of file name to open}*/
#define openarea(x)    info(x+2) /* string number of file area for |openname|}*/
#define openext(x)    link(x+2) /* string number of file extension for |openname|}*/

// [p473#1344]
#define immediatecode   4
#define setlanguagecode  5

/// [p471#1362]
#define advpast(x)                        \
    {                                     \
        if (subtype(x) == languagenode) { \
            curlang = whatlang(x);        \
            lhyf = whatlhm(x);            \
            rhyf = whatrhm(x);            \
        }                                 \
    }

/// [p478#1371]
#define endwritetoken   (CS_TOKEN_FLAG + END_WRITE)
/** @}*/ // end group S1340x1379_P472x481


extern FILE* write_file[16];
extern Boolean write_open[18];
extern Pointer writeloc;

extern void extension_init();
extern void extension_init_once();
extern void doextension(void);
extern void print_write_whatsit(StrNumber s, Pointer p);
extern void fixlanguage(void);
extern void outwhat(HalfWord p);

#endif // INC_EXTENSION_H