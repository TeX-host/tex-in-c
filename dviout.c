#include <stdio.h>  // FILE
#include <stdlib.h> // malloc, exit, labs, free
#include "tex.h"
    // [type] EightBits, FourQuarters, EightBits, SmallNumber
    // true
#include "str.h"
    // [type] StrASCIICode, StrNumber
    // [func] str_map, str_length
#include "fonts.h"
    // [type] InternalFontNumber,
    // [var] fontptr, fontused,
    // [func] get_fontsize, get_fontdsize, get_fontname
#include "funcs.h"  // [func] a_open_out
#include "dvicmd.h" // [enum] DVICommands
#include "dviout.h" // [export]

// #587: identifies the kind of DVI files described here
#define ID_BYTE 2
// #605: number of words per entry in the down and right stacks
#define MOVEMENT_NODE_SIZE 3

// p224#608
enum DVISetting {
    Y_HERE = 1, // info when the movement entry points to a y command
    Z_HERE,     // info when the movement entry points to a z command
    YZ_OK,      // info corresponding to an unconstrained down command
    Y_OK,       // info corresponding to a down that can’t become a z
    Z_OK,       // info corresponding to a down that can’t become a y
    D_FIXED,    // info corresponding to a down that can’t change
};

// P226#611
#define NONE_SEEN   0
#define Y_SEEN      6
#define Z_SEEN      12

// DVI byte number for a movement command
#define location(x) ((x)->locationf)
#define width(x) ((x)->widthf)
#define link(x) ((x)->linkf)
#define info(x) ((x)->infof)
#define freenode(x, y) free(x)

// #594: an index into the output buffer
// [0, DVI_BUF_SIZE=800]
typedef Pointer DVI_Index;
static EightBits dvibuf[DVI_BUF_SIZE + 1];
static DVI_Index half_buf, dvi_limit, dvi_ptr;
static long dvi_offset, dvigone;
static FILE* dvifile;
static long lastbop;

typedef struct move* MovePtr;
#undef HalfWord
#undef Pointer
struct move {
    MovePtr linkf;
    long widthf;
    long locationf;
    char infof;
};
static MovePtr down_ptr, rightptr;


/*
    functions
*/

int dvi_openout(void) { return a_open_out(&dvifile); }

static MovePtr get_move_node(void) {
    MovePtr pom = (MovePtr)malloc(sizeof(*pom));
    if (pom) {
        return pom;
    } else {
        fprintf(stderr, "dviout: Out of memory\n");
        exit(31);
    }
}
/* End quick hack */


// dviout(PUSH)
inline void dvi_out_push(void) { dviout(PUSH); }
// dviout(POP)
inline void dvi_out_pop(void) { dviout(POP); }
// dviout(EOP)
inline void dvi_out_eop(void) { dviout(EOP); }


void dvi_set_font(int f) {
    if (!fontused[f]) {
        dvi_font_def(f);
        fontused[f] = true;
    }
    f--;
    if (f < 64) {
        dviout(f + FNT_NUM_0);
    } else if (f < 255) {
        dviout(FNT1);
        dviout(f);
    }
    #if 0
    else { 
            dviout(fnt2);
            dviout(f>>8);
            dviout(f&0xff);
        }
    #endif
}

void dvi_set_char(long c) {
    if (c >= 128) {
        dviout(SET1);
    }
    dviout(c);
}

void dvi_setrule(long ruleht, long rulewd) {
    dviout(SET_RULE);
    dvi_four(ruleht);
    dvi_four(rulewd);
}

void dvi_putrule(long ruleht, long rulewd) {
    dviout(PUT_RULE);
    dvi_four(ruleht);
    dvi_four(rulewd);
}

void dvi_pre(long num, long den, long mag) {
    dviout(PRE);
    dviout(ID_BYTE);
    dvi_four(num);
    dvi_four(den);
    dvi_four(mag);
}

// #597: 
static void write_dvi(DVI_Index a, DVI_Index b) {
    fwrite(&dvibuf[a], b - a + 1, 1, dvifile);
} // #597: write_dvi

void dviout_init(void) {
    /*596:*/
    half_buf = DVI_BUF_SIZE / 2;
    dvi_limit = DVI_BUF_SIZE;
    dvi_ptr = 0;
    dvi_offset = 0;
    dvigone = 0; /*:596*/
    /*606:*/
    down_ptr = 0;
    rightptr = 0; /*:606*/
    lastbop = -1;
}

long get_dvi_mark(void) { return dvi_offset + dvi_ptr; }

void dvibop(long* counts) {
    long pageloc = dvi_offset + dvi_ptr;
    int k;
    dviout(BOP);
    for (k = 0; k <= 9; k++)
        dvi_four(counts[k]);
    dvi_four(lastbop);
    lastbop = pageloc;
}

void dvipost(long num,
             long den,
             long mag,
             long maxv,
             long maxh,
             int maxpush,
             int totalpages,
             int fontptr) {
    dviout(POST);
    dvi_four(lastbop);
    lastbop = dvi_offset + dvi_ptr - 5;
    dvi_four(num);
    dvi_four(den);
    dvi_four(mag);
    dvi_four(maxv);
    dvi_four(maxh);
    dviout(maxpush / 256);
    dviout(maxpush & 255);
    dviout((totalpages / 256) & 255);
    dviout(totalpages & 255); /*643:*/
    while (fontptr > 0) {     /*:643*/
        if (fontused[fontptr]) dvi_font_def(fontptr);
        fontptr--;
    }
}

long dviflush(void) {
    int k;
    dviout(POST_POST);
    dvi_four(lastbop);
    dviout(ID_BYTE);
    k = ((DVI_BUF_SIZE - dvi_ptr) & 3) + 4;
    while (k > 0) {
        dviout(223);
        k--;
    } /*599:*/
    if (dvi_limit == half_buf) write_dvi(half_buf, DVI_BUF_SIZE - 1);
    if (dvi_ptr > 0) write_dvi(0, dvi_ptr - 1);
    fclose(dvifile);
    return dvi_offset + dvi_ptr;
}

// #598: outputs half of the buffer
static void dvi_swap(void) {
    if (dvi_limit == DVI_BUF_SIZE) {
        write_dvi(0, half_buf - 1);
        dvi_limit = half_buf;
        dvi_offset += DVI_BUF_SIZE;
        dvi_ptr = 0;
    } else {
        write_dvi(half_buf, DVI_BUF_SIZE - 1);
        dvi_limit = DVI_BUF_SIZE;
    }
    dvigone += half_buf;
} // #598: dvi_swap

// #598
void dviout(int x) {
    dvibuf[dvi_ptr] = x;
    dvi_ptr++;
    if (dvi_ptr == dvi_limit) dvi_swap();
}

// #600: outputs four bytes in two’s complement notation, 
// without risking arithmetic overflow.
// TODO: check
void dvi_four(Integer x) {
    dviout(x >> 24);
    dviout((x >> 16) & 255);
    dviout((x >> 8) & 255);
    dviout(x & 255);
}

// #601
void dvi_pop(Integer l) {
    if (l == (dvi_offset + dvi_ptr) && dvi_ptr > 0)
        dvi_ptr--;
    else {
        dvi_out_pop();
    }
}

void dviout_helper(StrASCIICode c) { dviout(c); }

// #602
void dvi_font_def(InternalFontNumber f) {
    FourQuarters fck = get_fontcheck(f);
    StrNumber fnm = get_fontname(f);

    dviout(FNT_DEF_1);
    dviout(f - FONT_BASE - 1);

    dviout(fck.b0);
    dviout(fck.b1);
    dviout(fck.b2);
    dviout(fck.b3);

    dvi_four(get_fontsize(f));
    dvi_four(get_fontdsize(f));

    dviout(0);
    dviout(str_length(fnm));
    str_map(fnm, dviout_helper);
} // #602: dvi_font_def

// #607
void movement(Scaled w, EightBits o) {
    // have we seen a y or z?
    SmallNumber mstate; 
    // current and top nodes on the stack
    MovePtr p, q;
    // index into dvi buf , modulo dvi buf size
    Integer k;

    // new node for the top of the stack
    q = get_move_node();
    width(q) = w;
    location(q) = dvi_offset + dvi_ptr;
    if (o == DOWN1) {
        link(q) = down_ptr;
        down_ptr = q;
    } else {
        link(q) = rightptr;
        rightptr = q;
    }

    // p226#611
    p = link(q);
    mstate = NONE_SEEN;
    while (p != 0) {
        if (width(p) == w) { // #612
            switch (mstate + info(p)) {
                case NONE_SEEN + YZ_OK:
                case NONE_SEEN + Y_OK:
                case Z_SEEN + YZ_OK:
                case Z_SEEN + Y_OK:
                    if (location(p) < dvigone) {
                        goto _Lnotfound;
                    } else { // #613
                        k = location(p) - dvi_offset;
                        if (k < 0) k += DVI_BUF_SIZE;
                        dvibuf[k] += Y1 - DOWN1;
                        info(p) = Y_HERE;
                        goto _Lfound;
                    }
                    break;

                case NONE_SEEN + Z_OK:
                case Y_SEEN + YZ_OK:
                case Y_SEEN + Z_OK:
                    if (location(p) < dvigone) {
                        goto _Lnotfound;
                    } else { // #614
                        k = location(p) - dvi_offset;
                        if (k < 0) k += DVI_BUF_SIZE;
                        dvibuf[k] += Z1 - DOWN1;
                        info(p) = Z_HERE;
                        goto _Lfound;
                    }
                    break;

                case NONE_SEEN + Y_HERE:
                case NONE_SEEN + Z_HERE:
                case Y_SEEN + Z_HERE:
                case Z_SEEN + Y_HERE:
                    goto _Lfound;
                    break;
            } // switch (mstate + info(p))
        } else { // #611
            switch (mstate + info(p)) {
                case NONE_SEEN + Y_HERE:
                    mstate = Y_SEEN;
                    break;

                case NONE_SEEN + Z_HERE:
                    mstate = Z_SEEN;
                    break;

                case Y_SEEN + Z_HERE:
                case Z_SEEN + Y_HERE:
                    goto _Lnotfound;
                    break;
            }
        }
        p = link(p);
    } // while (p != 0)

_Lnotfound:
    // p225#610: Generate a down or right command for w and return
    info(q) = YZ_OK;
    if (labs(w) >= 8388608L) {
        dviout(o + 3); // down4 or right4
        dvi_four(w);
        return;
    }
    if (labs(w) >= 32768L) {
        dviout(o + 2); // down3 or right3
        if (w < 0) w += 16777216L;
        dviout(w / 65536L);
        w %= 65536L;
        goto _L2;
    }
    if (labs(w) >= 128) {
        dviout(o + 1); // down2 or right2
        if (w < 0) w += 65536L;
        goto _L2;
    }
    dviout(o); // down1 or right1
    if (w < 0) w += 256;
    goto _L1;

_L2:
    dviout(w / 256);

_L1:
    dviout(w & 255);
    return;

_Lfound:
    // p225#609: Generate a y0 or z0 command in order to reuse a previous
    // appearance of w
    info(q) = info(p);
    if (info(q) == Y_HERE) {
        dviout(o + Y0 - DOWN1);
        while (link(q) != p) {
            q = link(q);
            switch (info(q)) {
                case YZ_OK:
                    info(q) = Z_OK;
                    break;

                case Y_OK:
                    info(q) = D_FIXED;
                    break;
            } // switch (info(q))
        } // while (link(q) != p)
    } else { /*:609*/
        dviout(o + Z0 - DOWN1);
        while (link(q) != p) {
            q = link(q);
            switch (info(q)) {
                case YZ_OK:
                    info(q) = Y_OK;
                    break;

                case Z_OK:
                    info(q) = D_FIXED;
                    break;
            } // switch (info(q))
        } // while (link(q) != p)
    } // _Lfound: if (info(q) == Y_HERE)
} // #607: movement

// #615: delete movement nodes with location ≥ l
void prune_movements(Integer l) {
    MovePtr p;

    while (down_ptr != 0) {
        if (location(down_ptr) < l) break;
        p = down_ptr;
        down_ptr = link(p);
        freenode(p, MOVEMENT_NODE_SIZE);
    }

    while (rightptr != 0) {
        if (location(rightptr) < l) break;
        p = rightptr;
        rightptr = link(p);
        freenode(p, MOVEMENT_NODE_SIZE);
    }
} // #615: prune_movements

// #616
void synch_h(Scaled curh, Scaled dvih) {
    if (curh != dvih) {
        movement(curh - dvih, RIGHT1);
        dvih = curh;
    }
}
void synch_v(Scaled curv, Scaled dviv) {
    if (curv != dviv) {
        movement(curv - dviv, DOWN1);
        dviv = curv;
    }
}
