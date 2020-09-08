#include "texmac.h"
#include "global.h" // [var] eqtb
#include "scan.h"   // [var] cur_val
#include "error.h"  // [func] overflow
#include "print.h"  // [func] print_esc, print_char, print
#include "hash.h"


/** @addtogroup S256x267_P102x108
 * @{
 */
/// [#256]
TwoHalves hash[UNDEFINED_CONTROL_SEQUENCE - HASH_BASE]; // hash table
Pointer hash_used; // allocation pointer for hash
// ? no_new_control_sequence // are new identifiers legal?
Integer cs_count; // total number of known identifiers


/** [#259] search the hash table.
 *
 * that matches a given string of length `l > 1`
 * appearing in `buffer[j, (j + l − 1)]
 *
 * If the identifier is found, 
 *  the corresponding hash table address is returned.
 * if variable no_new_cs==true, 
 *  the dummy address UNDEFINED_CONTROL_SEQUENCE is returned.
 * Otherwise the identifier is inserted into the hash table
 *  and its location is returned.
 *
 * xref[3]: 264, 356, 374
 */
HalfWord idlookup_p(ASCIICode buf_ptr[], Integer len, Boolean no_new_cs) {
    // start index; 已经在调用时整合到 buf_ptr 和 len 中
    Integer j = 0; 

    Integer hash_code; // hash code
    Pointer p; // index in hash array

    /// [#261] Compute the hash code hash_code.
    hash_code = buf_ptr[j];
    for (size_t k = (j + 1); k <= (j + len - 1); k++) {
        hash_code = hash_code + hash_code + buf_ptr[k];
        while (hash_code >= HASH_PRIME) {
            hash_code -= HASH_PRIME;
        }
    } // [#261]

    // we start searching here; 
    // note that `0 ≤ hash_code < HASH_PRIME`
    p = hash_code + HASH_BASE;
    while (true) {
        if (text(p) > 0 && str_bcmp(buf_ptr, len, text(p))) break;
        if (next(p) == 0) {
            if (no_new_cs) {
                p = UNDEFINED_CONTROL_SEQUENCE;
            } else {
                // [#260] Insert a new control sequence after p, 
                //  then make p point to it
                if (text(p) > 0) {
                    do { // search for an empty location in hash
                        if (hash_is_full) {
                            // "hash size
                            overflow(S(475), HASH_SIZE); 
                        }
                        hash_used--;
                    } while (text(hash_used) != 0);
                    next(p) = hash_used;
                    p = hash_used;
                } // if (text(p) > 0)
                text(p) = str_insert(buf_ptr, len);

                #ifdef tt_STAT
                    cs_count++;
                #endif // #260: tt_STAT
            } // if (no_new_cs <>)
            break;
        } // if (next(p) == 0)
    
        p = next(p);
    } // while (true)

    return p;
} /* [#259] idlookup_p */

/// [#262] prints a purported control sequence.
void print_cs(long p) {
    if (p < HASH_BASE) {
        // single character
        if (p >= ACTIVE_BASE) {
            if (p == NULL_CS) {
                print_esc(S(262)); // "csname"
                print_esc(S(263)); // "endcsname"
                print_char(' ');
            } else {
                print_esc(p - ACTIVE_BASE);
                if (cat_code(p - ACTIVE_BASE) == LETTER) {
                    print_char(' ');
                }
            } // if (p == NULL_CS) - else
        } else {
            if (p < ACTIVE_BASE) {
                print_esc(S(264)); // "IMPOSSIBLE."
            } else {
                print(p - ACTIVE_BASE);
            } // if (p < ACTIVE_BASE) - else
        } // if (p >= ACTIVE_BASE) - else
    } else if (p >= UNDEFINED_CONTROL_SEQUENCE) {
        print_esc(S(264)); // "IMPOSSIBLE."
    } else if (!str_valid(text(p))) {
        print_esc(S(265)); // "NONEXISTENT."
    } else {
        print_esc(text(p));
        print_char(' ');
    } // if - elseif - ... - else
} /* [#262]: print_cs */

/// [#263]: prints a control sequence.
void sprint_cs(Pointer p) {
    if (p >= HASH_BASE) {
        print_esc(text(p));
        return;
    }
    if (p < ACTIVE_BASE) {
        print(p - ACTIVE_BASE);
        return;
    }

    if (p < NULL_CS) {
        print_esc(p - ACTIVE_BASE);
    } else {
        print_esc(S(262)); // "csname"
        print_esc(S(263)); // "endcsname"
    }
} /* [#263]: sprint_cs */

/// [p105#264]
#ifdef tt_INIT
void primitive(StrNumber s, QuarterWord c, HalfWord o) {
    if (s < 256) {
        cur_val = s + ACTIVE_BASE;
    } else {
        // TODO:
        // k ← str START[s]; l ← str START[s + 1] − k;
        // for j ← 0 to l − 1 do buffer[j] ← so(str pool[k + j]);
        cur_val = idlookup_s(s, false);
        flush_string();
        text(cur_val) = s;
    }
    eq_level(cur_val) = LEVEL_ONE;
    eq_type(cur_val) = c;
    equiv(cur_val) = o;
} /* [#264]: primitive */
#endif // #264: tt_INIT

/** @}*/ // end group S256x267_P102x108
