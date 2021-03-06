#include <stdlib.h> // abs
#include "lexer.h"
#include "mem.h"
#include "print.h"
#include "error.h"
#include "expand.h"
#include "scan.h" // scan_left_brace
#include "io.h"   // TERM_OUT
#include "dump.h"
#include "hyphen.h"


/// [ #942~966: PART 43: INITIALIZING THE HYPHENATION TABLES ]
/// #943, 947, 950
#ifdef tt_INIT
/// #943
// trie op codes for quadruples
Static short trieophash[TRIE_OP_SIZE + TRIE_OP_SIZE + 1];
// largest opcode used so far for this language
Static QuarterWord trieused[256];
// language part of a hashed quadruple
Static ASCIICode trieoplang[TRIE_OP_SIZE];
// opcode corresponding to a hashed quadruple
Static QuarterWord trieopval[TRIE_OP_SIZE];
// number of stored ops so far
Static UInt16 trieopptr;

/// #947
// characters to match
Static PackedASCIICode triec[TRIE_SIZE + 1];
// operations to perform
Static QuarterWord trieo[TRIE_SIZE + 1];
// left subtrie links
Static TriePointer triel[TRIE_SIZE + 1];
// right subtrie links
Static TriePointer trier[TRIE_SIZE + 1];
// the number of nodes in the trie
Static TriePointer trieptr;
// used to identify equivalent subtries
Static TriePointer triehash[TRIE_SIZE + 1];

/// #950
// does a family START here?
Static UChar trietaken[(TRIE_SIZE + 7) / 8];
// the first possible slot for each character
Static TriePointer triemin[256];
// largest location used in trie
Static TriePointer triemax;
// is the trie still in linked form?
// xref: 891, [950], 951, 960, 966, 1324, 1325
Static Boolean trie_not_ready;
#endif // #943,947,950: tt_INIT


/** @addtogroup S942x966_P350x359
 *
 * [p350#942]: Declare procedures for preprocessing hyphenation patterns.
 *
 * xref[8]
 *  944, 948, 949, 953, 957,
 *  959, 960, 966.
 * @{
 */

#ifdef tt_INIT

/// [#946, #951]
void hyphen_init_once() {
    int k;

    /// [#946]
    for (k = -TRIE_OP_SIZE; k <= TRIE_OP_SIZE; k++) {
        trieophash[k + TRIE_OP_SIZE] = 0;
    }
    for (k = 0; k <= 255; k++){ 
        trieused[k] = MIN_QUARTER_WORD;
    }
    trieopptr = 0;

    /// [#951]
    trie_not_ready = true;
    trieroot = 0;
    triec[0] = 0;
    trieptr = 0;
} /* hyphen_init_once */

/// p351#944
QuarterWord newtrieop(SmallNumber d, SmallNumber n, QuarterWord v) {
    QuarterWord Result;
    short h;
    QuarterWord u;
    short l;

    h = abs(n + d * 313 + v * 361 + curlang * 1009) %
            (TRIE_OP_SIZE + TRIE_OP_SIZE) -
        TRIE_OP_SIZE;
    while (true) {
        l = trieophash[h + TRIE_OP_SIZE];
        if (l == 0) {
            // "pattern memory ops"
            if (trieopptr == TRIE_OP_SIZE) overflow(S(767), TRIE_OP_SIZE);
            u = trieused[curlang];
            if (u == MAX_QUARTER_WORD) {
                // "pattern memory ops per language"
                overflow(S(768), MAX_QUARTER_WORD - MIN_QUARTER_WORD);
            }
            trieopptr++;
            u++;
            trieused[curlang] = u;
            hyfdistance[trieopptr - 1] = d;
            hyfnum[trieopptr - 1] = n;
            hyfnext[trieopptr - 1] = v;
            trieoplang[trieopptr - 1] = curlang;
            trieophash[h + TRIE_OP_SIZE] = trieopptr;
            trieopval[trieopptr - 1] = u;
            Result = u;
            break;
        }
        if (hyfdistance[l - 1] == d && hyfnum[l - 1] == n &&
            hyfnext[l - 1] == v && trieoplang[l - 1] == curlang) {
            Result = trieopval[l - 1];
            break;
        }
        if (h > -TRIE_OP_SIZE)
            h--;
        else
            h = TRIE_OP_SIZE;
    } // while (true)

    return Result;
} // #944: newtrieop

/// #948
TriePointer trienode(TriePointer p) {
    TriePointer Result, h, q;

    h = abs(triec[p] + trieo[p] * 1009 + triel[p] * 2718 + trier[p] * 3142) %
       TRIE_SIZE;
    while (true) {
        q = triehash[h];
        if (q == 0) {
            triehash[h] = p;
            Result = p;
            break;
        }
        if (triec[q] == triec[p] && trieo[q] == trieo[p] &&
            triel[q] == triel[p] && trier[q] == trier[p]) {
            Result = q;
            break;
        }
        if (h > 0)
            h--;
        else
            h =TRIE_SIZE;
    } // while (true)

    return Result;
} // #948: trienode

/// #949
TriePointer compresstrie(TriePointer p) {
    if (p == 0) {
        return 0;
    } else {
        triel[p] = compresstrie(triel[p]);
        trier[p] = compresstrie(trier[p]);
        return (trienode(p));
    }
} // #949: compresstrie

/// #953
void firstfit(TriePointer p) {
    TriePointer h, z, q;
    ASCIICode c;
    TriePointer l, r;
    short ll;

    c = triec[p];
    z = triemin[c];
    while (true) {
        h = z - c;               /*954:*/
        if (triemax < h + 256) { /*:954*/
            // "pattern memory"
            if (TRIE_SIZE <= h + 256) overflow(S(769), TRIE_SIZE);
            do {
                triemax++;
                P_clrbits_B(trietaken, triemax - 1, 0, 3);
                trielink(triemax) = triemax + 1;
                trieback(triemax) = triemax - 1;
            } while (triemax != h + 256);
        }
        if (P_getbits_UB(trietaken, h - 1, 0, 3)) /*955:*/
            goto _Lnotfound;
        q = trier[p];
        while (q > 0) {
            if (trielink(h + triec[q]) == 0) goto _Lnotfound;
            q = trier[q];
        }
        goto _Lfound; /*:955*/

    _Lnotfound:
        z = trielink(z);
    } // while (true)

_Lfound: /*956:*/
    P_putbits_UB(trietaken, h - 1, 1, 0, 3);
    triehash[p] = h;
    q = p;
    do {
        z = h + triec[q];
        l = trieback(z);
        r = trielink(z);
        trieback(r) = l;
        trielink(l) = r;
        trielink(z) = 0;
        if (l < 256) {
            if (z < 256) {
                ll = z;
            } else {
                ll = 256;
            }
            do {
                triemin[l] = r;
                l++;
            } while (l != ll);
        }
        q = trier[q]; /*:956*/
    } while (q != 0);
} // #953: firstfit

/// #957
void triepack(TriePointer p) {
    TriePointer q;

    do {
        q = triel[p];
        if (q > 0 && triehash[q] == 0) {
            firstfit(q);
            triepack(q);
        }
        p = trier[p];
    } while (p != 0);
} // #957: triepack

/// #959
void triefix(TriePointer p) {
    TriePointer q;
    ASCIICode c;
    TriePointer z;

    z = triehash[p];
    do {
        q = triel[p];
        c = triec[p];
        trielink(z + c) = triehash[q];
        triechar(z + c) = c;
        trieop(z + c) = trieo[p];
        if (q > 0) triefix(q);
        p = trier[p];
    } while (p != 0);
} // #959: triefix

/// #960
void newpatterns(void) {
    unsigned char k, l; /* INT */
    Boolean digitsensed;
    QuarterWord v;
    TriePointer p, q;
    Boolean firstchild;
    ASCIICode c;

    if (trie_not_ready) {
        setcurlang();
        scan_left_brace();
        /*961:*/
        k = 0;
        hyf[0] = 0;
        digitsensed = false;
        while (true) {
            get_x_token();
            switch (curcmd) {
                case LETTER:
                case OTHER_CHAR: /*962:*/
                    if (digitsensed | (curchr < '0') | (curchr > '9')) {
                        int cur_chr;
                        if (curchr == '.') {
                            cur_chr = 0;
                        } else {
                            cur_chr = lc_code(curchr);
                            if (cur_chr == 0) {
                                print_err(S(770)); // "Nonletter"
                                help1(S(771));     // "(See Appendix H.)"
                                error();
                            }
                        }
                        if (k < 63) {
                            k++;
                            hc[k] = cur_chr;
                            hyf[k] = 0;
                            digitsensed = false;
                        }
                    } else if (k < 63) {
                        hyf[k] = curchr - '0';
                        digitsensed = true;
                    }
                    break;

                case SPACER:
                case RIGHT_BRACE:
                    if (k > 0) { /*963:*/
                        if (hc[1] == 0) hyf[0] = 0;
                        if (hc[k] == 0) hyf[k] = 0;
                        l = k;
                        v = MIN_QUARTER_WORD;
                        while (true) {
                            if (hyf[l] != 0) v = newtrieop(k - l, hyf[l], v);
                            if (l <= 0) goto _Ldone1;
                            l--;
                        }
                    _Ldone1: /*:965*/
                        q = 0;
                        hc[0] = curlang;
                        while (l <= k) {
                            c = hc[l];
                            l++;
                            p = triel[q];
                            firstchild = true;
                            while (p > 0 && c > triec[p]) {
                                q = p;
                                p = trier[q];
                                firstchild = false;
                            }
                            if (p == 0 || c < triec[p]) { /*964:*/
                                if (trieptr == TRIE_SIZE) {
                                    // "pattern memory"
                                    overflow(S(769), TRIE_SIZE);
                                }
                                trieptr++;
                                trier[trieptr] = p;
                                p = trieptr;
                                triel[p] = 0;
                                if (firstchild)
                                    triel[q] = p;
                                else
                                    trier[q] = p;
                                triec[p] = c;
                                trieo[p] = MIN_QUARTER_WORD;
                            }
                            /*:964*/
                            q = p;
                        }
                        if (trieo[q] != MIN_QUARTER_WORD) {
                            print_err(S(772)); // "Duplicate pattern"
                            help1(S(771));     // "(See Appendix H.)"
                            error();
                        }
                        trieo[q] = v;
                    }
                    /*965:*/
                    /*:963*/
                    if (curcmd == RIGHT_BRACE) goto _Ldone;
                    k = 0;
                    hyf[0] = 0;
                    digitsensed = false;
                    break;

                default:
                    print_err(S(773)); // "Bad "
                    print_esc(S(774)); // "patterns"
                    help1(S(771));     // "(See Appendix H.)"
                    error();
                    break;
            } // switch (curcmd)
        } // while (true)
    _Ldone: /*:961*/
        return;
    } // if (trie_not_ready)

    print_err(S(775)); // "Too late for "
    print_esc(S(774)); // "patterns"
    // "All patterns must be given before typesetting begins."
    help1(S(776));
    error();
    link(garbage) = scantoks(false, false);
    flush_list(defref);
    /*:962*/
} // #960: newpatterns

/// #966
void inittrie(void) { /*952:*/
    TriePointer p;
    long j, k, t;
    TriePointer r, s;
    TwoHalves h;

    if (!trie_not_ready) return;

    /*945:*/
    opstart[0] = -MIN_QUARTER_WORD;
    for (j = 1; j <= 255; j++)
        opstart[j] = opstart[j - 1] + trieused[j - 1] - MIN_QUARTER_WORD;
    for (j = 1; j <= trieopptr; j++)
        trieophash[j + TRIE_OP_SIZE] =
            opstart[trieoplang[j - 1]] + trieopval[j - 1];
    for (j = 1; j <= trieopptr; j++) {
        while (trieophash[j + TRIE_OP_SIZE] > j) { /*:945*/
            k = trieophash[j + TRIE_OP_SIZE];
            t = hyfdistance[k - 1];
            hyfdistance[k - 1] = hyfdistance[j - 1];
            hyfdistance[j - 1] = t;
            t = hyfnum[k - 1];
            hyfnum[k - 1] = hyfnum[j - 1];
            hyfnum[j - 1] = t;
            t = hyfnext[k - 1];
            hyfnext[k - 1] = hyfnext[j - 1];
            hyfnext[j - 1] = t;
            trieophash[j + TRIE_OP_SIZE] = trieophash[k + TRIE_OP_SIZE];
            trieophash[k + TRIE_OP_SIZE] = k;
        }
    }
    for (p = 0; p <= TRIE_SIZE; p++)
        triehash[p] = 0;
    trieroot = compresstrie(trieroot);
    for (p = 0; p <= trieptr; p++)
        triehash[p] = 0;
    for (p = 0; p <= 255; p++)
        triemin[p] = p + 1;
    trielink(0) = 1;
    triemax = 0; /*:952*/
    if (trieroot != 0) {
        firstfit(trieroot);
        triepack(trieroot);
    }
    /*958:*/
    h.rh = 0;
    h.UU.U2.b0 = MIN_QUARTER_WORD;
    h.UU.U2.b1 = MIN_QUARTER_WORD;
    if (trieroot == 0) {
        for (r = 0; r <= 256; r++)
            trie[r] = h;
        triemax = 256;
    } else {
        triefix(trieroot);
        r = 0;
        do {
            s = trielink(r);
            trie[r] = h;
            r = s;
        } while (r <= triemax);
    }
    triechar(0) = '?'; /*:958*/
    trie_not_ready = false;
} // #966: inittrie

#endif // #942: tt_INIT


/** [#1324] Dump the hyphenation tables.
 * 
 */
void hypen_dump() {
    Integer k;

    dump_int(hyphcount);
    for (k = 0; k <= HYPH_SIZE; k++) {
        if (hyphword[k] != 0) {
            dump_int(k);
            dump_int(hyphword[k]);
            dump_int(hyphlist[k]);
        }
    }
    println();
    print_int(hyphcount);
    print(S(995)); // " hyphenation exception"
    if (hyphcount != 1) print_char('s');

    inittrie();
    dump_int(triemax);
    for (k = 0; k <= triemax; k++) {
        dump_hh(trie[k]);
    }
    dump_int(trieopptr);
    for (k = 0; k < trieopptr; k++) {
        dump_int(hyfdistance[k]);
        dump_int(hyfnum[k]);
        dump_int(hyfnext[k]);
    }

    printnl(S(996)); // "Hyphenation trie of length "
    print_int(triemax);
    print(S(997)); // " has "
    print_int(trieopptr);
    print(S(998)); // " op"
    if (trieopptr != 1) print_char('s');
    print(S(999)); // " out of "
    print_int(TRIE_OP_SIZE);
    for (k = 255; k >= 0; k--) {
        if (trieused[k] > MIN_QUARTER_WORD) {
            printnl(S(675)); // "  "
            print_int(trieused[k] - MIN_QUARTER_WORD);
            print(S(1000)); // " for language "
            print_int(k);
            dump_int(k);
            dump_int(trieused[k] - MIN_QUARTER_WORD);
        }
    }
}

/** [#1325] Undump the hyphenation tables. */
Boolean hyphen_undump() {
    Integer j, k; // all-purpose indices
    Integer x;    // something to dump

    x = undump_int();
    if ((unsigned long)x > HYPH_SIZE) return false;
    hyphcount = x;
    for (k = 1; k <= hyphcount; k++) {
        x = undump_int();
        if ((unsigned long)x > HYPH_SIZE) return false;
        j = x;
        x = undump_int();
        if (!str_valid(x)) return false;
        hyphword[j] = x;
        x = undump_int();
        if ((unsigned long)x > MAX_HALF_WORD) return false;
        hyphlist[j] = x;
    }

    x = undump_int();
    if (x < 0) return false;
    if (x > TRIE_SIZE) {
        fprintf(TERM_OUT, "---! Must increase the trie size\n");
        return false;
    }
    j = x;
    #ifdef tt_INIT
        triemax = j;
    #endif // #1325.1: tt_INIT
    for (k = 0; k <= j; k++) {
        trie[k] = undump_hh();
    }

    x = undump_int();
    if (x < 0) return false;
    if (x > TRIE_OP_SIZE) {
        fprintf(TERM_OUT, "---! Must increase the trie op size\n");
        return false;
    }
    j = x;
    #ifdef tt_INIT
        trieopptr = j;
    #endif // #1325.2: tt_INIT
    for (k = 1; k <= j; k++) {
        x = undump_int();
        if ((unsigned long)x > 63) return false;
        hyfdistance[k - 1] = x; // a small number
        x = undump_int();
        if ((unsigned long)x > 63) return false;
        hyfnum[k - 1] = x;
        x = undump_int();
        if ((unsigned long)x > MAX_QUARTER_WORD) return false;
        hyfnext[k - 1] = x;
    }

    #ifdef tt_INIT
        for (k = 0; k <= 255; k++) {
            trieused[k] = MIN_QUARTER_WORD;
        }
    #endif // #1325.3: tt_INIT
    k = 256;
    while (j > 0) {
        x = undump_int();
        if (x >= k) return false;
        k = x;
        x = undump_int();
        if (x < 1 || x > j) return false;
        #ifdef tt_INIT
            trieused[k] = x;
        #endif // #1325.4: tt_INIT
        j -= x;
        opstart[k] = j - MIN_QUARTER_WORD;
    }
    #ifdef tt_INIT
        trie_not_ready = false;
    #endif // #1325.5: tt_INIT

    return true;
}

/** @}*/ // end group S942x966_P350x359