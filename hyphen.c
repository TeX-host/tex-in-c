#include "str.h"
#include "mem.h"
#include "expand.h"
#include "print.h"
#include "error.h"
#include "scan.h" // scan_left_brace, scan_char_num
#include "lexer.h" // curchr, curcmd
#include "hyphen.h"


/** @addtogroup S919x941_P344x349
 * @{
 */
/*921:*/
TwoHalves trie[TRIE_SIZE + 1];
SmallNumber hyfdistance[TRIE_OP_SIZE];
SmallNumber hyfnum[TRIE_OP_SIZE];
QuarterWord hyfnext[TRIE_OP_SIZE];
short opstart[256];
/*:921*/
/*926:*/
StrNumber hyphword[HYPH_SIZE + 1];
Pointer hyphlist[HYPH_SIZE + 1];
HyphPointer hyphcount;
/*:926*/


/// [#p346#928
void hyphen_init() {
    for (size_t z = 0; z <= HYPH_SIZE; z++) {
        hyphword[z] = 0;
        hyphlist[z] = 0;
    }
    hyphcount = 0;
}

/// [#934] enters new exceptions.
void newhyphexceptions(void) {
    // [0..64] length of current word; not always a `small_number`
    unsigned char n;
    unsigned char j; // [0..64] an index into `hc`
    HyphPointer h; // an index into `hyph_word` and `hyph_list`
    StrNumber k; // an index into `str_start`
    StrNumber s, t; // strings being compared or stored
    Pointer p; // head of a list of hyphen positions
    Pointer q; // used when creating a new node for list `p`

    // a left brace must follow \hyphenation
    scan_left_brace();
    setcurlang(); 

    /// [#935] Enter as many hyphenation exceptions as are listed,
    /// until coming to a right brace; then return
    n = 0;
    p = 0;

    while (true) { /*:935*/
        int cur_chr;
        get_x_token();
        cur_chr = curchr;

    _LN_newhyphexceptions__reswitch:
        switch (curcmd) {
            case LETTER:
            case OTHER_CHAR:
            case CHAR_GIVEN:
                /// [#937] Append a new letter or hyphen.
                if (cur_chr == '-') { /*938:*/
                    // [#938] Append the value `n` to list `p`.
                    if (n < 63) {
                        q = get_avail();
                        link(q) = p;
                        info(q) = n;
                        p = q;
                    }
                } else {
                    if (lc_code(cur_chr) == 0) {
                        print_err(S(783)); // "Not a letter"
                        // "Letters in \\hyphenation words must have \\lccode>0."
                        // "Proceed; I'll ignore the character I just read."
                        help2(S(784), S(785));
                        error();
                    } else if (n < 63) {
                        n++;
                        hc[n] = lc_code(cur_chr);
                    }
                } // emd [#937]
                break;

            case CHAR_NUM:
                scan_char_num();
                cur_chr = cur_val;
                curcmd = CHAR_GIVEN;
                goto _LN_newhyphexceptions__reswitch;
                break;

            case SPACER:
            case RIGHT_BRACE:
                if (n > 1) {
                    /// [#939] Enter a hyphenation exception
                    n++;
                    hc[n] = curlang;
                    h = 0;
                    for (j = 1; j <= n; j++) {
                        h = (h + h + hc[j]) % HYPH_SIZE;
                    }

                    
                    s = str_ins(hc + 1, n);
                    /// [#940] Insert the pair (s, p) into the exception table.
                    // "exception dictionary"
                    if (hyphcount == HYPH_SIZE) overflow(S(786), HYPH_SIZE);
                    hyphcount++;
                    while (hyphword[h] != 0) {
                        /// [#941]  If the string hyph word [h] is less than or
                        /// equal to s, interchange (hyph word [h], hyph
                        /// list[h]) with (s, p)
                        k = hyphword[h];
                        if (str_length(k) < str_length(s)) goto _Lfound; 
                        if (str_length(k) > str_length(s)) goto _Lnotfound;
                        
                        {
                            int ress = str_cmp(k, s);
                            if (ress < 0) goto _Lfound;
                            if (ress > 0) goto _Lnotfound;
                        }
                    
                    _Lfound:
                        q = hyphlist[h];
                        hyphlist[h] = p;
                        p = q;
                        t = hyphword[h];
                        hyphword[h] = s;
                        s = t;

                    _Lnotfound: /// end [#941]
                        if (h > 0) {
                            h--;
                        } else {
                            h = HYPH_SIZE;
                        }
                    } // while (hyphword[h] != 0)

                    hyphword[h] = s;
                    hyphlist[h] = p; /*:940*/
                } // if (n > 1)
                /// end [#939]

                if (curcmd == RIGHT_BRACE) goto _Lexit;
                n = 0;
                p = 0;
                break;

            default: // [#936] Give improper \hyphenation error.
                print_err(S(597)); // "Improper "
                print_esc(S(787)); // "hyphenation"
                print(S(788));     // " will be flushed"
                // "Hyphenation exceptions must contain only letters"
                // "and hyphens. But continue; I'll forgive and forget."
                help2(S(789), S(790));
                error(); /*:936*/
                break;
        } // switch (curcmd)
    } // while (true)

_Lexit:;
} /* [#934] newhyphexceptions */


/** @}*/ // end group S919x941_P344x349