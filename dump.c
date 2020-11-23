#include "tex_constant.h" // [const] MEM_MIN, TRIE_SIZE, TRIE_OP_SIZE, MEM_BOT, MEM_TOP, HASH_PRIME, HYPH_SIZE, MIN_QUARTER_WORD, MAX_QUARTER_WORD, MAX_HALF_WORD
#include "str.h"
#include "mem.h"
#include "eqtb.h"
#include "io.h"
#include "hash.h"
#include "hyphen.h"
#include "error.h"
#include "print.h"
#include "str.h"
#include "extension.h"
#include "lexer.h" // [var] parloc, partoken
#include "dump.h"


/** @addtogroup S1299x1329_P455x464
 * @{
 */
// #1299: a string that is printed right after the banner
// format ident: 35, 61, 536, [1299], 1300, 1301, 1326, 1327, 1328, 1337
StrNumber format_ident;
/// [#1305] [G_var] for input or output of format information.
FILE* fmt_file = NULL;


/// [#1300]
void dump_init() { format_ident = 0; } /* dump_init */

/// [#1301]
void dump_init_once() {
    format_ident = S(259); // " (INITEX)"
} /* dump_init_once */

/// [#1305]
void dump_wd(MemoryWord wd) {
    MemoryWord fmt_var = wd;
    fwrite(&fmt_var, 8, 1, fmt_file);
}
void dump_int(Integer int_) {
    MemoryWord fmt_var;
    fmt_var.int_ = int_;
    fwrite(&fmt_var, 8, 1, fmt_file);
}
void dump_hh(TwoHalves hh) {
    MemoryWord fmt_var;
    fmt_var.hh = hh;
    fwrite(&fmt_var, 8, 1, fmt_file);
}
void dump_qqqq(FourQuarters qqqq) {
    MemoryWord fmt_var;
    fmt_var.qqqq = qqqq;
    fwrite(&fmt_var, 8, 1, fmt_file);
}

/// [#1306]
MemoryWord undump_wd() {
    MemoryWord fmt_var;
    fread(&fmt_var, 8, 1, fmt_file);
    return fmt_var;
}
Integer undump_int() {
    MemoryWord fmt_var;
    fread(&fmt_var, 8, 1, fmt_file);
    return fmt_var.int_;
}
TwoHalves undump_hh() {
    MemoryWord fmt_var;
    fread(&fmt_var, 8, 1, fmt_file);
    return fmt_var.hh;
}
FourQuarters undump_qqqq() {
    MemoryWord fmt_var;
    fread(&fmt_var, 8, 1, fmt_file);
    return fmt_var.qqqq;
}

#ifdef tt_INIT
/// [455#1302]: Declare action procedures for use by main control
void store_fmt_file(void) {
    Integer j, k, l; // all-purpose indices
    Pointer p, q;    // all-purpose pointers
    Integer x;       // something to dump

    /** [#1304] If dumping is not allowed, abort. */
    if (saveptr != 0) {
        print_err(S(988)); // "You can't dump inside a group"
        help1(S(989)); // "`{...\\dump}' is a no-no."
        succumb();
    }

    /** [#1328] Create the format ident, open the format file, 
     *  and inform the user that dumping has begun.
     */
    selector = NEW_STRING;
    print(S(990)); // " (preloaded format="
    print(job_name);
    print_char(' ');
    print_int(year % 100);
    print_char('.');
    print_int(month);
    print_char('.');
    print_int(day);
    print_char(')');

    if (interaction == BATCH_MODE) {
        selector = LOG_ONLY;
    } else {
        selector = TERM_AND_LOG;
    }

    str_room(1);
    format_ident = makestring();
    packjobname(formatextension);
    while (!wopenout(&fmt_file)) {
        // "format file name"
        promptfilename(S(991), formatextension);
    }
    printnl(S(992)); // "Beginning to dump on file "
    slow_print(wmakenamestring());
    flush_string();
    printnl(S(385)); // ""
    slow_print(format_ident);

    /** [#1307] Dump constants for consistency check. */
    dump_int(371982687L);
    dump_int(MEM_BOT);
    dump_int(MEM_TOP);
    dump_int(EQTB_SIZE);
    dump_int(HASH_PRIME);
    dump_int(HYPH_SIZE);

    /** [#1309] Dump the string pool. */
    str_dump(fmt_file);

    /// [#1311] Dump the dynamic memory.
    /// ## dump [MEM_BOT, lo_mem_max]
    sort_avail();
    var_used = 0;
    dump_int(lo_mem_max);
    dump_int(rover);
    p = MEM_BOT; // begin
    q = rover; // end
    x = 0; // temp var; memory locations

    do {
        for (k = p; k <= q + 1; k++) {
            dump_wd(mem[k - MEM_MIN]);
        }
        x += q - p + 2;
        var_used += q - p;
        p = q + node_size(q);
        q = rlink(q);
    } while (q != rover);
    var_used += lo_mem_max - p;
    dyn_used = mem_end - hi_mem_min + 1;
    for (k = p; k <= lo_mem_max; k++) {
        dump_wd(mem[k - MEM_MIN]);
    }
    x += lo_mem_max - p + 1;

    /// ## dump [hi_mem_min, mem_end]
    dump_int(hi_mem_min);
    dump_int(avail);
    for (k = hi_mem_min; k <= mem_end; k++) {
        dump_wd(mem[k - MEM_MIN]);
    }
    x += mem_end - hi_mem_min + 1;

    p = avail;
    while (p != 0) {
        dyn_used -= CHAR_NODE_SIZE;
        p = link(p);
    }
    dump_int(var_used);
    dump_int(dyn_used);

    println();
    print_int(x);
    print(S(993)); // " memory locations dumped; current usage is "
    print_int(var_used);
    print_char('&');
    print_int(dyn_used);

    /** [#1313] Dump the table of equivalents.
     *
     *  1. [#1315] Dump regions 1 to 4 of eqtb.
     *  2. [#1316] Dump regions 5 and 6 of eqtb.
     *  3. [#1318] Dump the hash table.
     */
    /// 1. [#1315] Dump regions 1 to 4 of eqtb.
    k = ACTIVE_BASE;
    do {
        j = k;
        while (j < INT_BASE - 1) {
            if ((equiv(j) == equiv(j + 1)) 
                & (eq_type(j) == eq_type(j + 1)) 
                & (eq_level(j) == eq_level(j + 1)))
                goto _Lfound1;
            j++;
        }
        l = INT_BASE;
        goto _Ldone1; // j = INT_BASE − 1

    _Lfound1:
        j++;
        l = j;
        while (j < INT_BASE - 1) {
            if ((equiv(j) != equiv(j + 1)) 
                | (eq_type(j) != eq_type(j + 1)) 
                | (eq_level(j) != eq_level(j + 1)))
                goto _Ldone1;
            j++;
        }

    _Ldone1:
        dump_int(l - k);
        while (k < l) {
            dump_wd(eqtb[k - ACTIVE_BASE]);
            k++;
        }
        k = j + 1;
        dump_int(k - l);
    } while (k != INT_BASE);

    /** 2. [#1316] Dump regions 5 and 6 of eqtb. */
    do {
        j = k;
        while (j < EQTB_SIZE) {
            if (eqtb[j - ACTIVE_BASE].int_ == eqtb[j - ACTIVE_BASE + 1].int_)
                goto _Lfound2;
            j++;
        }
        l = EQTB_SIZE + 1;
        goto _Ldone2;

    _Lfound2:
        j++;
        l = j;
        while (j < EQTB_SIZE) {
            if (eqtb[j - ACTIVE_BASE].int_ != eqtb[j - ACTIVE_BASE + 1].int_)
                goto _Ldone2;
            j++;
        }

    _Ldone2:
        dump_int(l - k);
        while (k < l) {
            dump_wd(eqtb[k - ACTIVE_BASE]);
            k++;
        }
        k = j + 1;
        dump_int(k - l); /*:1316*/
    } while (k <= EQTB_SIZE);

    dump_int(parloc);
    dump_int(writeloc);

    /** 3. [#1318] Dump the hash table */
    dump_int(hash_used);
    cs_count = FROZEN_CONTROL_SEQUENCE - hash_used - 1;
    for (p = HASH_BASE; p <= hash_used; p++) {
        if (get_text(p) != 0) {
            dump_int(p);
            dump_hh(hash[p - HASH_BASE]);
            cs_count++;
        }
    }
    for (p = hash_used + 1; p < UNDEFINED_CONTROL_SEQUENCE; p++) {
        dump_hh(hash[p - HASH_BASE]);
    }
    dump_int(cs_count);
    println();
    print_int(cs_count); 
    print(S(994)); // " multiletter control sequences"

    /** [#1320] Dump the font information. */
    fonts_dump();

    /** [#1324] Dump the hyphenation tables. */
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

    if (trie_not_ready) inittrie();
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
            printnl(S(675));                  // "  "
            print_int(trieused[k] - MIN_QUARTER_WORD);
            print(S(1000)); // " for language "
            print_int(k);
            dump_int(k);
            dump_int(trieused[k] - MIN_QUARTER_WORD);
        }
    }

    /** [1326] Dump a couple more things and the closing check word. */
    dump_int(interaction);
    dump_int(format_ident);
    dump_int(69069L);
    tracingstats = 0;

    /** [#1329] Close the format file. */
    w_close(&fmt_file);
} // store_fmt_file
#endif // #1302: tt_INIT


/** [#1303] Read dumps in.
 *
 *  [#524] Declare the function called open fmt file.
 */
Boolean load_fmt_file(void) {
    Integer j, k; // all-purpose indices
    Pointer p, q; // all-purpose pointers
    Integer x;    // something to dump

    /** [#1308] Undump constants for consistency check. */
    if (undump_int() != 371982687L) goto _LN_badfmt;
    if (undump_int() != MEM_BOT) goto _LN_badfmt;
    if (undump_int() != MEM_TOP) goto _LN_badfmt;
    if (undump_int() != EQTB_SIZE) goto _LN_badfmt;
    if (undump_int() != HASH_PRIME) goto _LN_badfmt;
    if (undump_int() != HYPH_SIZE) goto _LN_badfmt;

    /** [#1310] Undump the string pool. */
    if (!str_undump(fmt_file, TERM_OUT)) goto _LN_badfmt;

    /*1312:*/
    x = undump_int();
    if (x < lomemstatmax + 1000 || x >= himemstatmin) goto _LN_badfmt;
    lo_mem_max = x;
    x = undump_int();
    if (x <= lomemstatmax || x > lo_mem_max) goto _LN_badfmt;
    rover = x;

    p = MEM_BOT;
    q = rover;
    do {
        for (k = p; k <= q + 1; k++) {
            mem[k - MEM_MIN] = undump_wd();
        }
        p = q + node_size(q);
        if ((p > lo_mem_max) | ((q >= rlink(q)) & (rlink(q) != rover)))
            goto _LN_badfmt;
        q = rlink(q);
    } while (q != rover);
    for (k = p; k <= lo_mem_max; k++) {
        mem[k - MEM_MIN] = undump_wd();
    }

    if (MEM_MIN < MEM_BOT - 2) {
        p = llink(rover);
        q = MEM_MIN + 1;
        link(MEM_MIN) = 0;
        info(MEM_MIN) = 0;
        rlink(p) = q;
        llink(rover) = q;
        rlink(q) = rover;
        llink(q) = p;
        link(q) = empty_flag;
        node_size(q) = MEM_BOT - q;
    }

    x = undump_int();
    if (x <= lo_mem_max || x > himemstatmin) goto _LN_badfmt;
    hi_mem_min = x;
    x = undump_int();
    if ((unsigned long)x > MEM_TOP) goto _LN_badfmt;
    avail = x;
    mem_end = MEM_TOP;
    for (k = hi_mem_min; k <= mem_end; k++) {
        mem[k - MEM_MIN] = undump_wd();
    }
    var_used = undump_int();
    dyn_used = undump_int();

    /** [#1314] Undump the table of equivalents.
     *
     *  1. [#1317] Undump regions 1 to 6 of eqtb.
     *  2. [#1319] Undump the hash table.
     */
    /// 1. [#1317] Undump regions 1 to 6 of eqtb.
    k = ACTIVE_BASE;
    do {
        x = undump_int();
        if (x < 1 || k + x > EQTB_SIZE + 1) goto _LN_badfmt;
        for (j = k; j < k + x; j++) {
            eqtb[j - ACTIVE_BASE] = undump_wd();
        }
        k += x;
        x = undump_int();
        if (x < 0 || k + x > EQTB_SIZE + 1) goto _LN_badfmt;
        for (j = k; j < k + x; j++)
            eqtb[j - ACTIVE_BASE] = eqtb[k - ACTIVE_BASE - 1];
        k += x;
    } while (k <= EQTB_SIZE);
    
    x = undump_int();
    if (x < HASH_BASE || x > FROZEN_CONTROL_SEQUENCE) goto _LN_badfmt;
    parloc = x;
    partoken = CS_TOKEN_FLAG + parloc;
    x = undump_int();
    if (x < HASH_BASE || x > FROZEN_CONTROL_SEQUENCE) goto _LN_badfmt;
    writeloc = x;

    /// 2. [#1319] Undump the hash table.
    x = undump_int();
    if (x < HASH_BASE || x > FROZEN_CONTROL_SEQUENCE) goto _LN_badfmt;
    hash_used = x;

    p = HASH_BASE - 1;
    do {
        x = undump_int();
        if (x <= p || x > hash_used) goto _LN_badfmt;
        p = x;
        hash[p - HASH_BASE] = undump_hh();
    } while (p != hash_used);
    for (p = hash_used + 1; p < UNDEFINED_CONTROL_SEQUENCE; p++) {
        hash[p - HASH_BASE] = undump_hh();
    }
    cs_count = undump_int();

    /** [#1321] Undump the font information. */
    if (!fonts_undump()) goto _LN_badfmt;

    /** [#1325] Undump the hyphenation tables. */
    x = undump_int();
    if ((unsigned long)x > HYPH_SIZE) goto _LN_badfmt;
    hyphcount = x;
    for (k = 1; k <= hyphcount; k++) {
        x = undump_int();
        if ((unsigned long)x > HYPH_SIZE) goto _LN_badfmt;
        j = x;
        x = undump_int();
        if (!str_valid(x)) goto _LN_badfmt;
        hyphword[j] = x;
        x = undump_int();
        if ((unsigned long)x > MAX_HALF_WORD) goto _LN_badfmt;
        hyphlist[j] = x;
    }

    x = undump_int();
    if (x < 0) goto _LN_badfmt;
    if (x > TRIE_SIZE) {
        fprintf(TERM_OUT, "---! Must increase the trie size\n");
        goto _LN_badfmt;
    }
    j = x;
    #ifdef tt_INIT
        triemax = j;
    #endif // #1325.1: tt_INIT
    for (k = 0; k <= j; k++) {
        trie[k] = undump_hh();
    }

    x = undump_int();
    if (x < 0) goto _LN_badfmt;
    if (x > TRIE_OP_SIZE) {
        fprintf(TERM_OUT, "---! Must increase the trie op size\n");
        goto _LN_badfmt;
    }
    j = x;
    #ifdef tt_INIT
        trieopptr = j;
    #endif // #1325.2: tt_INIT
    for (k = 1; k <= j; k++) {
        x = undump_int();
        if ((unsigned long)x > 63) goto _LN_badfmt;
        hyfdistance[k - 1] = x; // a small number
        x = undump_int();
        if ((unsigned long)x > 63) goto _LN_badfmt;
        hyfnum[k - 1] = x;
        x = undump_int();
        if ((unsigned long)x > MAX_QUARTER_WORD) goto _LN_badfmt;
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
        if (x >= k) goto _LN_badfmt;
        k = x;
        x = undump_int();
        if (x < 1 || x > j) goto _LN_badfmt;
        #ifdef tt_INIT
            trieused[k] = x;
        #endif // #1325.4: tt_INIT
        j -= x;
        opstart[k] = j - MIN_QUARTER_WORD;
    }
    #ifdef tt_INIT
        trie_not_ready = false;
    #endif // #1325.5: tt_INIT

    /** [#1327] Undump a couple more things and the closing check word. */
    x = undump_int();
    if ((unsigned long)x > ERROR_STOP_MODE) goto _LN_badfmt;
    interaction = x;
    x = undump_int();
    if (!str_valid(x)) goto _LN_badfmt;
    format_ident = x;
    x = undump_int();
    if ((x != 69069L) | feof(fmt_file)) goto _LN_badfmt;
    return true;

_LN_badfmt:
    fprintf(TERM_OUT, "(Fatal format file error; I'm stymied)\n");
    return false;
} // #1303: load_fmt_file

/** @}*/ // end group S1299x1329_P455x464