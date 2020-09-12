#include "global_const.h"
#include "tex_types.h"
#include "global.h"
#include "str.h"
#include "mem.h"
#include "eqtb.h"
#include "io.h"
#include "hash.h"
#include "hyphen.h"
#include "error.h"
#include "print.h"
#include "str.h"
#include "macros.h"
#include "extension.h"
#include "dump.h"


/** @addtogroup S1299x1329_P455x464
 * @{
 */
// #1299: a string that is printed right after the banner
// format ident: 35, 61, 536, [1299], 1300, 1301, 1326, 1327, 1328, 1337
StrNumber format_ident;

/*1305:*/
FILE* fmtfile = NULL;
/*:1305*/


void dump_init() {
    format_ident = 0; // #1300
}


#ifdef tt_INIT
/// 455#1302: Declare action procedures for use by main control
void storefmtfile(void) { /*1304:*/
    long j, k, l, x;
    Pointer p, q;
    MemoryWord pppfmtfile;
    if (saveptr != 0) {
        print_err(S(988)); // "You can't dump inside a group"
        help1(S(989)); // "`{...\\dump}' is a no-no."
        succumb();
    }
    /*:1304*/
    /*1328:*/
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
    if (interaction == BATCH_MODE)
        selector = LOG_ONLY;
    else
        selector = TERM_AND_LOG;
    str_room(1);
    format_ident = makestring();
    packjobname(formatextension);
    while (!wopenout(&fmtfile)) {
        // "format file name"
        promptfilename(S(991), formatextension);
    }
    printnl(S(992)); // "Beginning to dump on file "
    slow_print(wmakenamestring());
    flush_string();
    printnl(S(385)); // ""
    /*:1328*/

    slow_print(format_ident); /*1307:*/
    pppfmtfile.int_ = 371982687L;
    pput(pppfmtfile);
    pppfmtfile.int_ = MEM_BOT;
    pput(pppfmtfile);
    pppfmtfile.int_ = MEM_TOP;
    pput(pppfmtfile);
    pppfmtfile.int_ = EQTB_SIZE;
    pput(pppfmtfile);
    pppfmtfile.int_ = HASH_PRIME;
    pput(pppfmtfile);
    pppfmtfile.int_ = HYPH_SIZE;
    pput(pppfmtfile); /*:1307*/
    str_dump(fmtfile);

    sort_avail(); // #131

    var_used = 0;
    pppfmtfile.int_ = lo_mem_max;
    pput(pppfmtfile);
    pppfmtfile.int_ = rover;
    pput(pppfmtfile);
    p = MEM_BOT;
    q = rover;
    x = 0;
    do {
        for (k = p; k <= q + 1; k++) {
            pppfmtfile = mem[k - MEM_MIN];
            pput(pppfmtfile);
        }
        x += q - p + 2;
        var_used += q - p;
        p = q + node_size(q);
        q = rlink(q);
    } while (q != rover);
    var_used += lo_mem_max - p;
    dyn_used = mem_end - hi_mem_min + 1;
    for (k = p; k <= lo_mem_max; k++) {
        pppfmtfile = mem[k - MEM_MIN];
        pput(pppfmtfile);
    }
    x += lo_mem_max - p + 1;
    pppfmtfile.int_ = hi_mem_min;
    pput(pppfmtfile);
    pppfmtfile.int_ = avail;
    pput(pppfmtfile);
    for (k = hi_mem_min; k <= mem_end; k++) {
        pppfmtfile = mem[k - MEM_MIN];
        pput(pppfmtfile);
    }
    x += mem_end - hi_mem_min + 1;
    p = avail;
    while (p != 0) {
        dyn_used -= CHAR_NODE_SIZE;
        p = link(p);
    }
    pppfmtfile.int_ = var_used;
    pput(pppfmtfile);
    pppfmtfile.int_ = dyn_used;
    pput(pppfmtfile);
    println();
    print_int(x);
    print(S(993)); // " memory locations dumped; current usage is "
    print_int(var_used);
    print_char('&');    /*:1311*/
    print_int(dyn_used); /*1313:*/
    /*1315:*/
    k = ACTIVE_BASE;

    do { /*1316:*/
        j = k;
        while (j < INT_BASE - 1) {
            if ((equiv(j) == equiv(j + 1)) & (eq_type(j) == eq_type(j + 1)) &
                (eq_level(j) == eq_level(j + 1)))
                goto _Lfound1;
            j++;
        }
        l = INT_BASE;
        goto _Ldone1;
_Lfound1:
        j++;
        l = j;
        while (j < INT_BASE - 1) {
            if ((equiv(j) != equiv(j + 1)) | (eq_type(j) != eq_type(j + 1)) |
                (eq_level(j) != eq_level(j + 1)))
                goto _Ldone1;
            j++;
        }
_Ldone1:
        pppfmtfile.int_ = l - k;
        pput(pppfmtfile);
        while (k < l) {
            pppfmtfile = eqtb[k - ACTIVE_BASE];
            pput(pppfmtfile);
            k++;
        }
        k = j + 1;
        pppfmtfile.int_ = k - l;
        pput(pppfmtfile); /*:1315*/
    } while (k != INT_BASE);
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
        pppfmtfile.int_ = l - k;
        pput(pppfmtfile);
        while (k < l) {
            pppfmtfile = eqtb[k - ACTIVE_BASE];
            pput(pppfmtfile);
            k++;
        }
        k = j + 1;
        pppfmtfile.int_ = k - l;
        pput(pppfmtfile); /*:1316*/
    } while (k <= EQTB_SIZE);

    pppfmtfile.int_ = parloc;
    pput(pppfmtfile);
    pppfmtfile.int_ = writeloc;
    pput(pppfmtfile); /*1318:*/
    pppfmtfile.int_ = hash_used;
    pput(pppfmtfile);
    cs_count = FROZEN_CONTROL_SEQUENCE - hash_used - 1;
    for (p = HASH_BASE; p <= hash_used; p++) {
        if (get_text(p) != 0) {
            pppfmtfile.int_ = p;
            pput(pppfmtfile);
            pppfmtfile.hh = hash[p - HASH_BASE];
            pput(pppfmtfile);
            cs_count++;
        }
    }
    for (p = hash_used + 1; p < UNDEFINED_CONTROL_SEQUENCE; p++) {
        pppfmtfile.hh = hash[p - HASH_BASE];
        pput(pppfmtfile);
    }
    pppfmtfile.int_ = cs_count;
    pput(pppfmtfile);
    println();
    print_int(cs_count); /*:1318*/
    /*:1313*/
    print(S(994)); // " multiletter control sequences"
    fonts_dump(fmtfile);
    /*1324:*/
    pppfmtfile.int_ = hyphcount;
    pput(pppfmtfile);
    for (k = 0; k <= HYPH_SIZE; k++) {
        if (hyphword[k] != 0) {
            pppfmtfile.int_ = k;
            pput(pppfmtfile);
            pppfmtfile.int_ = hyphword[k];
            pput(pppfmtfile);
            pppfmtfile.int_ = hyphlist[k];
            pput(pppfmtfile);
        }
    }
    println();
    print_int(hyphcount);
    print(S(995)); // " hyphenation exception"
    if (hyphcount != 1) print_char('s');
    if (trie_not_ready) inittrie();
    pppfmtfile.int_ = triemax;
    pput(pppfmtfile);
    for (k = 0; k <= triemax; k++) {
        pppfmtfile.hh = trie[k];
        pput(pppfmtfile);
    }
    pppfmtfile.int_ = trieopptr;
    pput(pppfmtfile);
    for (k = 0; k < trieopptr; k++) {
        pppfmtfile.int_ = hyfdistance[k];
        pput(pppfmtfile);
        pppfmtfile.int_ = hyfnum[k];
        pput(pppfmtfile);
        pppfmtfile.int_ = hyfnext[k];
        pput(pppfmtfile);
    }
    printnl(S(996)); // "Hyphenation trie of length "
    print_int(triemax);
    print(S(997)); // " has "
    print_int(trieopptr);
    print(S(998)); // " op"
    if (trieopptr != 1) print_char('s');
    print(S(999)); // " out of "
    print_int(TRIE_OP_SIZE);
    for (k = 255; k >= 0; k--) {            /*1326:*/
        if (trieused[k] > MIN_QUARTER_WORD) { /*:1324*/
            printnl(S(675));                  // "  "
            print_int(trieused[k] - MIN_QUARTER_WORD);
            print(S(1000)); // " for language "
            print_int(k);
            pppfmtfile.int_ = k;
            pput(pppfmtfile);
            pppfmtfile.int_ = trieused[k] - MIN_QUARTER_WORD;
            pput(pppfmtfile);
        }
    }
    pppfmtfile.int_ = interaction;
    pput(pppfmtfile);
    pppfmtfile.int_ = format_ident;
    pput(pppfmtfile);
    pppfmtfile.int_ = 69069L;
    pput(pppfmtfile);
    tracingstats = 0; /*:1326*/
    /*1329:*/
    /*:1329*/
    w_close(&fmtfile);
} // storefmtfile
#endif // #1302: tt_INIT


Boolean load_fmt_file(void) { /*1308:*/
    Boolean Result;
    long j, k, x;
    Pointer p, q;
    /* FourQuarters w; */
    MemoryWord pppfmtfile;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x != 371982687L) goto _Lbadfmt_;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x != MEM_BOT) goto _Lbadfmt_;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x != MEM_TOP) goto _Lbadfmt_;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x != EQTB_SIZE) goto _Lbadfmt_;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x != HASH_PRIME) goto _Lbadfmt_;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x != HYPH_SIZE) /*1310:*/
        goto _Lbadfmt_;
    if (!str_undump(fmtfile, TERM_OUT)) goto _Lbadfmt_;
    /*1312:*/
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x < lomemstatmax + 1000 || x >= himemstatmin) goto _Lbadfmt_;
    lo_mem_max = x;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x <= lomemstatmax || x > lo_mem_max) goto _Lbadfmt_;
    rover = x;
    p = MEM_BOT;
    q = rover;
    do {
        for (k = p; k <= q + 1; k++) {
            pget(pppfmtfile);
            mem[k - MEM_MIN] = pppfmtfile;
        }
        p = q + node_size(q);
        if ((p > lo_mem_max) | ((q >= rlink(q)) & (rlink(q) != rover)))
            goto _Lbadfmt_;
        q = rlink(q);
    } while (q != rover);
    for (k = p; k <= lo_mem_max; k++) {
        pget(pppfmtfile);
        mem[k - MEM_MIN] = pppfmtfile;
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
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x <= lo_mem_max || x > himemstatmin) goto _Lbadfmt_;
    hi_mem_min = x;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if ((unsigned long)x > MEM_TOP) goto _Lbadfmt_;
    avail = x;
    mem_end = MEM_TOP;
    for (k = hi_mem_min; k <= mem_end; k++) {
        pget(pppfmtfile);
        mem[k - MEM_MIN] = pppfmtfile;
    }
    pget(pppfmtfile);
    var_used = pppfmtfile.int_;
    pget(pppfmtfile);
    dyn_used = pppfmtfile.int_; /*:1312*/
    /*1314:*/
    /*1317:*/
    k = ACTIVE_BASE;
    do {
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if (x < 1 || k + x > EQTB_SIZE + 1) goto _Lbadfmt_;
        for (j = k; j < k + x; j++) {
            pget(pppfmtfile);
            eqtb[j - ACTIVE_BASE] = pppfmtfile;
        }
        k += x;
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if (x < 0 || k + x > EQTB_SIZE + 1) goto _Lbadfmt_;
        for (j = k; j < k + x; j++)
            eqtb[j - ACTIVE_BASE] = eqtb[k - ACTIVE_BASE - 1];
        k += x; /*:1317*/
    } while (k <= EQTB_SIZE);
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x < HASH_BASE || x > FROZEN_CONTROL_SEQUENCE) goto _Lbadfmt_;
    parloc = x;
    partoken = CS_TOKEN_FLAG + parloc;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x < HASH_BASE || x > FROZEN_CONTROL_SEQUENCE) /*1319:*/
        goto _Lbadfmt_;
    writeloc = x;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x < HASH_BASE || x > FROZEN_CONTROL_SEQUENCE) goto _Lbadfmt_;
    hash_used = x;
    p = HASH_BASE - 1;
    do {
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if (x <= p || x > hash_used) goto _Lbadfmt_;
        p = x;
        pget(pppfmtfile);
        hash[p - HASH_BASE] = pppfmtfile.hh;
    } while (p != hash_used);
    for (p = hash_used + 1; p < UNDEFINED_CONTROL_SEQUENCE; p++) {
        pget(pppfmtfile);
        hash[p - HASH_BASE] = pppfmtfile.hh;
    }
    pget(pppfmtfile);
    cs_count = pppfmtfile.int_; /*:1319*/
    /*:1314*/
    if (!fonts_undump(fmtfile, TERM_OUT)) goto _Lbadfmt_;
    /*1325:*/
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if ((unsigned long)x > HYPH_SIZE) goto _Lbadfmt_;
    hyphcount = x;
    for (k = 1; k <= hyphcount; k++) {
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if ((unsigned long)x > HYPH_SIZE) goto _Lbadfmt_;
        j = x;
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if (!str_valid(x)) goto _Lbadfmt_;
        hyphword[j] = x;
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if ((unsigned long)x > MAX_HALF_WORD) goto _Lbadfmt_;
        hyphlist[j] = x;
    }
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x < 0) goto _Lbadfmt_;
    if (x >TRIE_SIZE) {
        fprintf(TERM_OUT, "---! Must increase the trie size\n");
        goto _Lbadfmt_;
    }
    j = x;
    #ifdef tt_INIT
        triemax = j;
    #endif // #1325.1: tt_INIT
    for (k = 0; k <= j; k++) {
        pget(pppfmtfile);
        trie[k] = pppfmtfile.hh;
    }
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x < 0) goto _Lbadfmt_;
    if (x > TRIE_OP_SIZE) {
        fprintf(TERM_OUT, "---! Must increase the trie op size\n");
        goto _Lbadfmt_;
    }
    j = x;
    #ifdef tt_INIT
        trieopptr = j;
    #endif // #1325.2: tt_INIT
    for (k = 1; k <= j; k++) {
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if ((unsigned long)x > 63) goto _Lbadfmt_;
        hyfdistance[k - 1] = x;
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if ((unsigned long)x > 63) goto _Lbadfmt_;
        hyfnum[k - 1] = x;
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if ((unsigned long)x > MAX_QUARTER_WORD) goto _Lbadfmt_;
        hyfnext[k - 1] = x;
    }
    #ifdef tt_INIT
        for (k = 0; k <= 255; k++)
            trieused[k] = MIN_QUARTER_WORD;
    #endif // #1325.3: tt_INIT
    k = 256;
    while (j > 0) {
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if (x >= k) goto _Lbadfmt_;
        k = x;
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if (x < 1 || x > j) goto _Lbadfmt_;
        #ifdef tt_INIT
            trieused[k] = x;
        #endif // #1325.4: tt_INIT
        j -= x;
        opstart[k] = j - MIN_QUARTER_WORD;
    }
    #ifdef tt_INIT
        trie_not_ready = false;
    #endif // #1325.5: tt_INIT
       /*:1325*/

    /*1327:*/
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if ((unsigned long)x > ERROR_STOP_MODE) goto _Lbadfmt_;
    interaction = x;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (!str_valid(x)) goto _Lbadfmt_;
    format_ident = x;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if ((x != 69069L) | feof(fmtfile)) goto _Lbadfmt_; /*:1327*/
    Result = true;
    goto _Lexit;

_Lbadfmt_:
    fprintf(TERM_OUT, "(Fatal format file error; I'm stymied)\n");
    Result = false;

_Lexit:
    return Result;
} // #1303: load_fmt_file

/** @}*/ // end group S1299x1329_P455x464