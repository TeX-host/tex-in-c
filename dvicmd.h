#pragma once
#ifndef INC_DVI_CMD
#define INC_DVI_CMD

// p215#585
// a list of all the commands that may appear in a DVI file
enum DVICommands {
    // typeset character 0 and move right
    SET_CHAR_0 = 0,
    // EMIT SET_CHAR_[2~127]

    SET1 = 128, // typeset a character and move right
    SET2,
    SET3,
    SET4,
    SET_RULE,   // typeset a rule and move right

    PUT1 = 133,
    PUT2,
    PUT3,
    PUT4,
    PUT_RULE,   // typeset a rule

    NOP = 138,  // no operation
    BOP,        // beginning of page
    EOP,        // ending of page
    PUSH,       // save the current positions
    POP,        // restore previous positions

    // move right
    RIGHT1 = 143,
    RIGHT2,
    RIGHT3,
    RIGHT4,

    W0 = 147,   // move right by w
    W1,         // move right and set w
    W2,
    W3,
    W4,

    X0 = 152,   // move right by x
    X1,         // move right and set x
    X2,
    X3,
    X4,

    // move down
    DOWN1 = 157, 
    DOWN2,
    DOWN3,
    DOWN4,

    Y0 = 161,   // move right by y
    Y1,         // move right and set y
    Y2,
    Y3,
    Y4,

    Z0 = 166,   // move right by z
    Z1,         // move right and set z
    Z2,
    Z3,
    Z4,

    // set current font to 0
    FNT_NUM_0 = 171,
    FNT_NUM_1,
    // emit fnt_num_(2~234)

    FNT1 = 253, // set current font
    FNT2,
    FNT3,
    FNT4,

    XXX1 = 239, // extension to DVI primitives
    XXX2,
    XXX3,
    XXX4,       // potentially long extension to DVI primitives

    // define the meaning of a font number
    FNT_DEF_1 = 243,
    FNT_DEF_2,
    FNT_DEF_3,
    FNT_DEF_4,

    PRE = 247,          // preamble
    POST = 248,         // postamble beginning
    POST_POST = 249,    // postamble ending

    // [250~255] are undefined at the present time
}; // enum DVICommands

#endif // INC_DVI_CMD