#include <stdio.h>  // FILE
#include <stdlib.h> // malloc, exit, labs, free
#include "tex.h"
    // [type] EightBits, FourQuarters, EightBits, SmallNumber
    // true
#include "str.h"
    // [type] StrNumber
    // [func] str_map, str_length
#include "fonts.h"
    // [type] InternalFontNumber,
    // [var] fontptr, fontused,
    // [func] get_fontsize, get_fontdsize, get_fontname
#include "dvicmd.h" // [enum] DVICommands
#include "global_const.h" // [macro] assert, UMAXOF
#include "io.h"           // [macro] TERM_ERR [func] a_open_out
#include "dviout.h" // [export]

/// [#587]: identifies the kind of DVI files described here
#define ID_BYTE 2
/// [#605]: number of words per entry in the down and right stacks
#define MOVEMENT_NODE_SIZE 3

/// p224#608
enum DVISetting {
    Y_HERE = 1, ///< info when the movement entry points to a y command
    Z_HERE,     ///< info when the movement entry points to a z command
    YZ_OK,      ///< info corresponding to an unconstrained down command
    Y_OK,       ///< info corresponding to a down that can’t become a z
    Z_OK,       ///< info corresponding to a down that can’t become a y
    D_FIXED,    ///< info corresponding to a down that can’t change.
};

/// P226#611
enum StackState {
    NONE_SEEN = 0, ///< no y here or z here nodes have been encountered yet
    Y_SEEN = 6,    ///< we have seen y here but not z here
    Z_SEEN = 12    ///< we have seen z here but not y here
};

typedef struct move* MovePtr;
#undef HalfWord
struct move {
    MovePtr linkf;
    long widthf;
    long locationf;
    char infof;
};

/// DVI byte number for a movement command
#define LOCATION(x) ((x)->locationf)
#define WIDTH(x)    ((x)->widthf)
#define LINK(x)     ((x)->linkf)
#define INFO(x)     ((x)->infof)
#define FREE_NODE(x, y) free(x)

/// [#594]: an index into the output buffer.
/// [0, DVI_BUF_SIZE=800]
typedef Pointer DVI_Index;
static_assert(UMAXOF(DVI_Index) >= DVI_BUF_SIZE,
              "DVI_Index = [0, DVI_BUF_SIZE=800]");
static EightBits dvibuf[DVI_BUF_SIZE + 1]; ///< buffer for DVI output
static DVI_Index half_buf,  ///< half of dvi buf size
                 dvi_limit, ///< end of the current half buffer
                 dvi_ptr;   ///< the next available buffer address.

/// `DVI_BUF_SIZE` times the number of times
/// the output buffer has been fully emptied.
static Integer dvi_offset;
static Integer dvigone; ///< the number of bytes already output to dvi file
static FILE* dvifile;

/// LOCATION of previous bop in the DVI output.
/// last_bop: [592], 593, 640, 642
static Integer last_bop; 

static MovePtr down_ptr, right_ptr;


/*
 * 函数定义
 * 
 */

/// 初始化 dviout 内部变量
void dviout_init(void) {
    // #593
    last_bop = -1;
    // #596
    half_buf = DVI_BUF_SIZE / 2;
    dvi_limit = DVI_BUF_SIZE;
    dvi_ptr = 0;
    dvi_offset = 0;
    dvigone = 0;
    // #606
    down_ptr = NULL;
    right_ptr = NULL;
} // dviout_init

/// [#597]: The actual output of dvi_buf[a，b] to dvi file.
static void write_dvi(DVI_Index a, DVI_Index b) {
    fwrite(&dvibuf[a], b - a + 1, 1, dvifile);
} // #597: write_dvi

/// [#598]: outputs half of the buffer.
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

/// [#598]
void dviout(int x) {
    dvibuf[dvi_ptr] = x;
    dvi_ptr++;
    if (dvi_ptr == dvi_limit) dvi_swap();
}

/// [#600]: outputs four bytes in two’s complement notation,
/// without risking arithmetic overflow.
/// TODO: check
void dvi_four(Integer x) {
    dviout(x >> 24);
    dviout((x >> 16) & 255);
    dviout((x >> 8) & 255);
    dviout(x & 255);
}

/// [#601]
void dvi_pop(Integer l) {
    if (l == (dvi_offset + dvi_ptr) && dvi_ptr > 0)
        dvi_ptr--;
    else {
        dviout(POP);
    }
}

void dviout_helper(ASCIICode c) { dviout(c); }

/// [#602]
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

static MovePtr get_move_node(void) {
    MovePtr pom = (MovePtr)malloc(sizeof(*pom));
    if (pom) {
        return pom;
    } else {
        fprintf(TERM_ERR, "dviout: Out of memory\n");
        exit(31);
    }
}

/// [p224#607]
/// produces a DVI command for some specified downward or rightward motion.
/// 
/// parameters:
///  + w: the amount of motion
///  + dvicmd: DOWN1 or RIGHT1
static void movement(Scaled w, enum DVICommands dvicmd) {
    /// have we seen a y or z?
    SmallNumber mstate; 
    /// current and top nodes on the stack
    MovePtr p, q;
    /// index into dvi buf , modulo dvi buf size
    Integer k;

    // malloc new node
    // TODO: check impl.
    //  original code `get_node(movement_node_size)`
    q = get_move_node();
    WIDTH(q) = w;
    LOCATION(q) = dvi_offset + dvi_ptr;
    if (dvicmd == DOWN1) {
        LINK(q) = down_ptr;
        down_ptr = q;
    } else {
        LINK(q) = right_ptr;
        right_ptr = q;
    }

    // [p226#611]
    // Look at the other stack entries until deciding 
    // what sort of DVI command to generate; 
    // goto found if node p is a "hit"
    p = LINK(q);
    mstate = NONE_SEEN;
    while (p != NULL) {
        if (WIDTH(p) == w) {
            // [#612]: Consider a node with matching width; 
            // goto found if it’s a hit
            switch (mstate + INFO(p)) {
                case NONE_SEEN + YZ_OK:
                case NONE_SEEN + Y_OK:
                case Z_SEEN + YZ_OK:
                case Z_SEEN + Y_OK:
                    if (LOCATION(p) < dvigone) {
                        goto _L_not_found;
                    } else {
                        // [#613]: Change buffered instruction to y or w 
                        // and goto found
                        k = LOCATION(p) - dvi_offset;
                        if (k < 0) k += DVI_BUF_SIZE;
                        dvibuf[k] += (Y1 - DOWN1);
                        INFO(p) = Y_HERE;
                        goto _Lfound;
                    }
                    break;

                case NONE_SEEN + Z_OK:
                case Y_SEEN + YZ_OK:
                case Y_SEEN + Z_OK:
                    if (LOCATION(p) < dvigone) {
                        goto _L_not_found;
                    } else { 
                        // [#614]: Change buffered instruction to z or x
                        // and goto found
                        k = LOCATION(p) - dvi_offset;
                        if (k < 0) k += DVI_BUF_SIZE;
                        dvibuf[k] += (Z1 - DOWN1);
                        INFO(p) = Z_HERE;
                        goto _Lfound;
                    }
                    break;

                case NONE_SEEN + Y_HERE:
                case NONE_SEEN + Z_HERE:
                case Y_SEEN + Z_HERE:
                case Z_SEEN + Y_HERE:
                    goto _Lfound;
                    break;

                default: break;
            } // switch (mstate + INFO(p))
        } else { // WIDTH(p) != w
            // [#611]
            switch (mstate + INFO(p)) {
                case NONE_SEEN + Y_HERE:
                    mstate = Y_SEEN;
                    break;

                case NONE_SEEN + Z_HERE:
                    mstate = Z_SEEN;
                    break;

                case Y_SEEN + Z_HERE:
                case Z_SEEN + Y_HERE:
                    goto _L_not_found;
                    break;

                default: break;
            }
        }
        p = LINK(p);
    } // while (p != NULL)

_L_not_found:
    // [p225#610]: Generate a `down` or `right` command for w and return
    INFO(q) = YZ_OK;
    if (labs(w) >= 8388608L) {
        dviout(dvicmd + 3); // DOWN4 or RIGHT4
        dvi_four(w);
        return;
    }
    if (labs(w) >= 32768L) {
        dviout(dvicmd + 2); // DOWN3 or RIGHT3
        if (w < 0) w += 16777216L;
        dviout(w / 65536L);
        w %= 65536L;
        goto _L2;
    }
    if (labs(w) >= 128) {
        dviout(dvicmd + 1); // DOWN2 or RIGHT2
        if (w < 0) w += 65536L;
        goto _L2;
    }
    dviout(dvicmd); // DOWN1 or RIGHT1
    if (w < 0) w += 256;
    goto _L1;

_L2:
    dviout(w / 256);

_L1:
    dviout(w % 256);
    return;

// _not_found end


_Lfound:
    // [p225#609]:
    // Generate a y0 or z0 command 
    // in order to reuse a previous appearance of w
    INFO(q) = INFO(p);
    if (INFO(q) == Y_HERE) {
        dviout(dvicmd + Y0 - DOWN1); // Y0 or W0
        while (LINK(q) != p) {
            q = LINK(q);
            switch (INFO(q)) {
                case YZ_OK: INFO(q) = Z_OK; break;
                case Y_OK:  INFO(q) = D_FIXED; break;
                default: break;
            } // switch (INFO(q))
        } // while (LINK(q) != p)
    } else { // INFO(q) != Y_HERE
        dviout(dvicmd + Z0 - DOWN1); // Z0 or X0
        while (LINK(q) != p) {
            q = LINK(q);
            switch (INFO(q)) {
                case YZ_OK: INFO(q) = Y_OK; break;
                case Z_OK:  INFO(q) = D_FIXED; break;
                default: break;
            } // switch (INFO(q))
        } // while (LINK(q) != p)
    } // _Lfound: if (INFO(q) == Y_HERE)
} // #607: movement

/// [p227#615]: delete movement nodes with LOCATION ≥ l.
void prune_movements(Integer l) {
    MovePtr p;

    while (down_ptr != NULL) {
        if (LOCATION(down_ptr) < l) break; // goto => break
        p = down_ptr;
        down_ptr = LINK(p);
        FREE_NODE(p, MOVEMENT_NODE_SIZE);
    }

    while (right_ptr != NULL) {
        if (LOCATION(right_ptr) < l) break;
        p = right_ptr;
        right_ptr = LINK(p);
        FREE_NODE(p, MOVEMENT_NODE_SIZE);
    }
} // #615: prune_movements

/// [#616]
/// 返回 cur_h 在 tex.c 中通过赋值更新 dvi_h.
Scaled synch_h(Scaled cur_h, Scaled dvi_h) {
    if (cur_h != dvi_h) {
        movement(cur_h - dvi_h, RIGHT1);
        // dvi_h = cur_h;
    }
    return cur_h; 
}

// 返回 cur_v 在 tex.c 中通过赋值更新 dvi_v.
Scaled synch_v(Scaled cur_v, Scaled dvi_v) {
    if (cur_v != dvi_v) {
        movement(cur_v - dvi_v, DOWN1);
        // dvi_v = cur_v;
    }
    return cur_v; 
}


// 辅助函数
// 帮助访问/修改 dviout 的内部变量

inline void dviout_ID_BYTE(void)    { dviout(ID_BYTE); }
inline void dviout_SET_RULE(void)   { dviout(SET_RULE); }
inline void dviout_PUT_RULE(void)   { dviout(PUT_RULE); }
inline void dviout_EOP(void)        { dviout(EOP); }
inline void dviout_PUSH(void)       { dviout(PUSH); }
inline void dviout_POP(void)        { dviout(POP); }
inline void dviout_PRE(void)        { dviout(PRE); }
inline void dviout_POST(void)       { dviout(POST); }
inline void dviout_XXX1(void)       { dviout(XXX1); }
inline void dviout_XXX4(void)       { dviout(XXX4); }

long get_dvi_mark(void) { return dvi_offset + dvi_ptr; }
Boolean dvi_openout(void) { return a_open_out(&dvifile); }

/// used in [p236#638]: shipout
void _dvibop(long counts[]) {
    // page_loc: [638], 640
    long pageloc = dvi_offset + dvi_ptr;
    dviout(BOP);
    for (int k = 0; k <= 9; k++) {
        dvi_four(counts[k]);
    }
    dvi_four(last_bop);
    last_bop = pageloc;
}

void _dvi_lastbop(void) {
    dvi_four(last_bop);
    last_bop = dvi_offset + dvi_ptr - 5;
}

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

long dviflush(void) {
    int k;
    dviout(POST_POST);
    dvi_four(last_bop);
    dviout(ID_BYTE);
    k = ((DVI_BUF_SIZE - dvi_ptr) % 4) + 4; // the number of 223’s
    while (k > 0) {
        dviout(223);
        k--;
    }
    // #599: Empty the last bytes out of dvi buf
    if (dvi_limit == half_buf) write_dvi(half_buf, DVI_BUF_SIZE - 1);
    if (dvi_ptr > 0) write_dvi(0, dvi_ptr - 1);
    fclose(dvifile);
    return dvi_offset + dvi_ptr;
}
