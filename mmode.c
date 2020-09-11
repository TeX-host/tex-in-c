#include "tex.h"
#include "mem.h"
#include "pack.h"
#include "print.h"
#include "mmode.h"


/** @addtogroup S680x698_P249x257
 * @{
 */
/*684:*/
TwoHalves emptyfield;
FourQuarters nulldelimiter;
/*:684*/


// #686
HalfWord newnoad(void) {
    Pointer p;
    int i = 0;
    p = get_node(noadsize);
    type(p) = ordnoad;
    subtype(p) = NORMAL;
#ifdef BIG_CHARNODE
    for (i = 0; i < CHAR_NODE_SIZE; i++) {
#endif
        mem[nucleus(p) + i - MEM_MIN].hh = emptyfield;
        mem[subscr(p) + i - MEM_MIN].hh = emptyfield;
        mem[supscr(p) + i - MEM_MIN].hh = emptyfield;
#ifdef BIG_CHARNODE
    }
#endif
    return p;
}
/*:686*/

/*688:*/
HalfWord newstyle(SmallNumber s) {
    Pointer p;

    p = get_node(stylenodesize);
    type(p) = stylenode;
    subtype(p) = s;
    width(p) = 0;
    depth(p) = 0;
    return p;
} /*:688*/


/*689:*/
HalfWord newchoice(void) {
    Pointer p;

    p = get_node(stylenodesize);
    type(p) = choicenode;
    subtype(p) = 0;
    displaymlist(p) = 0;
    textmlist(p) = 0;
    scriptmlist(p) = 0;
    scriptscriptmlist(p) = 0;
    return p;
}
/*:689*/


/// [#691] prints family and character.
void printfamandchar(HalfWord p) {
    print_esc(S(333));
    print_int(fam(p));
    print_char(' ');
    print(character(p) - MIN_QUARTER_WORD);
} // [#691] printfamandchar

/// [#691] prints a delimiter as 24-bit hex value.
void printdelimiter(HalfWord p) {
    long a;

    a = smallfam(p) * 256 + smallchar(p) - MIN_QUARTER_WORD;
    a = a * 4096 + largefam(p) * 256 + largechar(p) - MIN_QUARTER_WORD;
    if (a < 0)
        print_int(a);
    else
        print_hex(a);
} // [#691] printdelimiter

/// [#692] display a noad field.
void printsubsidiarydata(HalfWord p, ASCIICode c) {
    if (cur_length() >= depth_threshold) {
        if (mathtype(p) != EMPTY) print(S(334));
        return;
    }
    append_char(c);
    temp_ptr = p;
    switch (mathtype(p)) {
        case mathchar:
            println();
            printcurrentstring();
            printfamandchar(p);
            break;

        case subbox: 
            showinfo(); 
            break;

        case submlist:
            if (info(p) == 0) {
                println();
                printcurrentstring();
                print(S(335));
            } else
                showinfo();
            break;
    }
    flush_char();
} // [#692] printsubsidiarydata

/*693:*/
void showinfo(void) { shownodelist(info(temp_ptr)); }
/*:693*/

/// [#694]
void printstyle(Integer c) {
    switch (c / 2) {
        case 0: print_esc(S(336)); break;
        case 1: print_esc(S(337)); break;
        case 2: print_esc(S(338)); break;
        case 3: print_esc(S(339)); break;

        default: print(S(340)); break;
    }
} // [#694] printstyle


/** @}*/ // end group S680x698_P249x257