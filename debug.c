#include <math.h>
#include "global.h"
#include "tex_constant.h" // [const] MEM_MIN
#include "texmath.h" // [macro] UNITY; [func] print_scaled
#include "io.h"
#include "mem.h"
#include "eqtb.h"
#include "print.h"
#include "printout.h"
#include "fonts.h" // fontinfo
#include "lexer.h" // showtokenlist
#include "box.h"   // [func] showbox, shownodelist, shortdisplay; 
                   // [var] breadth_max, depth_threshold;
#include "debug.h"


/** @addtogroup S110x114_P42x43
 * @{
 */

/// p43#114
#ifdef tt_DEBUG
Static void print_word(MemoryWord w) {
    print_int(w.int_);
    print_char(' ');
    print_scaled(w.sc);
    print_char(' ');
    /*                  round(UNITY * w.gr) */
    print_scaled( (long)floor(UNITY * w.gr + 0.5) );
    println();

    print_int(w.hh.UU.lh);
    print_char('=');
    print_int(w.hh.UU.U2.b0);
    print_char(':');
    print_int(w.hh.UU.U2.b1);
    print_char(';');

    print_int(w.hh.rh);
    print_char(' ');
    print_int(w.qqqq.b0);
    print_char(':');
    print_int(w.qqqq.b1);
    print_char(':');
    print_int(w.qqqq.b2);
    print_char(':');
    print_int(w.qqqq.b3);
} // print_word
#endif // #114: tt_DEBUG
/** @}*/ // end group S110x114_P42x43

/** @addtogroup S1338x1339_P470x471
 * @{
 */
#ifdef tt_DEBUG
// 交互式 debug 环境
/// [p470#1338] routine to display various things.
void debug_help(void) {
    Integer k, l, m, n;

    while (true) {
        printnl(S(1253)); // "debug # (−1 to exit):"
        update_terminal();

        fscanf(TERM_IN, " %lld", &m);
        if (m < 0) {
            return;
        } else if (m == 0) {
            goto _Lbreakpoint_;

        /// a location where you can set a breakpoint.
        _Lbreakpoint_:
            m = 0;
            continue;
        }

        /* assert(m >= 0); */
        fscanf(TERM_IN, " %lld", &n);
        switch (m) {
            /** [#1339] Numbered cases for debug help. */
            // display mem[n] in all forms
            case 1: print_word(mem[n - MEM_MIN]); break;
            case 2: print_int(info(n)); break;
            case 3: print_int(link(n)); break;
            case 4: print_word(eqtb[n]); break;
            case 5: print_word(fontinfo[n]); break;
            case 6: print_word(save_stack[n]); break;
            // show a box, abbreviated by show box depth and show box breadth
            case 7: showbox(n); break;
            case 8: { // show a box in its entirety
                breadth_max = 10000;
                depth_threshold = str_adjust_to_room(POOL_SIZE) - 10;
                shownodelist(n);
                break;
            }
            case 9: showtokenlist(n, null, 1000); break;
            case 10: slow_print(n); break;
            // check wellformedness; print new busy locations if n > 0
            case 11: check_mem(n > 0); break;
            // look for pointers to n
            case 12: search_mem(n); break;
            case 13: {
                fscanf(TERM_IN, " %lld", &l);
                printcmdchr(n, l);
                break;
            }
            case 14: {
                for (k = 0; k <= n; k++) {
                    print(buffer[k]);
                }
                break;
            }
            case 15: {
                font_in_short_display = NULL_FONT;
                shortdisplay(n);
                break;
            }
            case 16: panicking = !panicking; break;

            default: print('?'); break;
        } // switch (m)
    } // while (true)
} // #1338: debug_help
#endif // #1338: tt_DEBUG

/** @}*/ // end group S1338x1339_P470x471