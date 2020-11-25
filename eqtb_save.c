#include "lexer.h" // [var] curtok
#include "error.h" // [func] overflow, confusion, int_error
#include "mem.h"   // [func] free_node
    // [macro] info
#include "print.h" // [func] print_char, print, print_err, print_int
    // printnl
#include "box.h"    // [func] delete_token_ref
#include "eqtb.h"


/** @addtogroup S268x288_P109x114
 * @{
 */
/// [ #268~288: SAVING AND RESTORING EQUIVALENTS ]
/// [#271]
MemoryWord savestack[SAVE_SIZE + 1];
UInt16 saveptr;              // first unused entry on save stack
UInt16 maxsavestack;  // maximum usage of save stack
QuarterWord curlevel; // current nesting level for groups
GroupCode curgroup;   // current group type
UInt16 curboundary;   // where the current level begins
// [#286] if nonzero,
// this magnification should be used henceforth
Integer magset;

/// [#272, #287]
void eqtb_save_init() {
    /// [#272]
    saveptr = 0;
    curlevel = LEVEL_ONE;
    curgroup = bottomlevel;
    curboundary = 0;
    maxsavestack = 0;
    /// [#287]
    magset = 0;
}

/*274:*/
void new_save_level(GroupCode c) {
    if (saveptr > maxsavestack) {
        maxsavestack = saveptr;
        // "save size"
        if (maxsavestack > SAVE_SIZE - 6) overflow(S(476), SAVE_SIZE);
    }
    savetype(saveptr) = levelboundary;
    savelevel(saveptr) = curgroup;
    saveindex(saveptr) = curboundary;
    if (curlevel == MAX_QUARTER_WORD) {
        // "grouping levels"
        overflow(S(477), MAX_QUARTER_WORD - MIN_QUARTER_WORD);
    }
    curboundary = saveptr;
    curlevel++;
    saveptr++;
    curgroup = c;
}
/*:274*/

/*275:*/
void eq_destroy(MemoryWord w) {
    Pointer q;

    switch (eq_type_field(w)) {
        case CALL:
        case LONG_CALL:
        case OUTER_CALL:
        case LONG_OUTER_CALL: delete_token_ref(equiv_field(w)); break;

        case GLUE_REF: delete_glue_ref(equiv_field(w)); break;

        case SHAPE_REF:
            q = equiv_field(w);
            if (q != 0) free_node(q, info(q) + info(q) + 1);
            break;

        case BOX_REF: flush_node_list(equiv_field(w)); break;
    }
}
/*:275*/

/*276:*/
void eq_save(HalfWord p, QuarterWord l) {
    if (saveptr > maxsavestack) {
        maxsavestack = saveptr;
        // "save size"
        if (maxsavestack > SAVE_SIZE - 6) overflow(S(476), SAVE_SIZE);
    }
    if (l == LEVEL_ZERO) {
        savetype(saveptr) = restorezero;
    } else {
        savestack[saveptr] = eqtb[p];
        saveptr++;
        savetype(saveptr) = restoreoldvalue;
    }
    savelevel(saveptr) = l;
    saveindex(saveptr) = p;
    saveptr++;
}
/*:276*/

/*277:*/
void eq_define(HalfWord p, QuarterWord t, HalfWord e) {
    if (eq_level(p) == curlevel)
        eq_destroy(eqtb[p]);
    else if (curlevel > LEVEL_ONE)
        eq_save(p, eq_level(p));
    eq_level(p) = curlevel;
    eq_type(p) = t;
    equiv(p) = e;
}
/*:277*/

/*278:*/
void eq_word_define(HalfWord p, long w) {
    if (xeqlevel[p - INT_BASE] != curlevel) {
        eq_save(p, xeqlevel[p - INT_BASE]);
        xeqlevel[p - INT_BASE] = curlevel;
    }
    eqtb[p].int_ = w;
}
/*:278*/

/*279:*/
void geq_define(HalfWord p, QuarterWord t, HalfWord e) {
    eq_destroy(eqtb[p]);
    eq_level(p) = LEVEL_ONE;
    eq_type(p) = t;
    equiv(p) = e;
}

void geq_word_define(HalfWord p, long w) {
    eqtb[p].int_ = w;
    xeqlevel[p - INT_BASE] = LEVEL_ONE;
}
/*:279*/

/*280:*/
void save_for_after(HalfWord t) {
    if (curlevel <= LEVEL_ONE) return;
    if (saveptr > maxsavestack) {
        maxsavestack = saveptr;
        // "save size"
        if (maxsavestack > SAVE_SIZE - 6) overflow(S(476), SAVE_SIZE);
    }
    savetype(saveptr) = inserttoken;
    savelevel(saveptr) = LEVEL_ZERO;
    saveindex(saveptr) = t;
    saveptr++;
}
/*:280*/

/*281:*/
#ifdef tt_STAT
/// #284
void restore_trace(HalfWord p, StrNumber s) {
    begin_diagnostic();
    print_char('{');
    print(s);
    print_char(' ');
    show_eqtb(p);
    print_char('}');
    end_diagnostic(false);
}
#endif // #284: tt_STAT


void unsave(void) {
    Pointer p;
    QuarterWord l = 0 /* XXXX */;

    if (curlevel <= LEVEL_ONE) {
        confusion(S(478)); // "curlevel"
        return;
    }
    curlevel--; /*282:*/
    while (true) {
        saveptr--;
        if (savetype(saveptr) == levelboundary) break;
        p = saveindex(saveptr);
        if (savetype(saveptr) == inserttoken) { /*326:*/
            HalfWord t = curtok;
            curtok = p;
            backinput();
            curtok = t;
            continue;
        }                                           /*:326*/
        if (savetype(saveptr) == restoreoldvalue) { /*283:*/
            l = savelevel(saveptr);
            saveptr--;
        } else {
            savestack[saveptr] = eqtb[UNDEFINED_CONTROL_SEQUENCE];
        }
        if (p < INT_BASE) {
            if (eq_level(p) == LEVEL_ONE) {
                eq_destroy(savestack[saveptr]);
                #ifdef tt_STAT
                    if (tracingrestores > 0) restore_trace(p, S(479));
                #endif // #283.1: tt_STAT
            } else {
                eq_destroy(eqtb[p]);
                eqtb[p] = savestack[saveptr];
                #ifdef tt_STAT
                    if (tracingrestores > 0) restore_trace(p, S(480));
                #endif // #283.2: tt_STAT
            }
            continue;
        }
        if (xeqlevel[p - INT_BASE] != LEVEL_ONE) {
            eqtb[p] = savestack[saveptr];
            xeqlevel[p - INT_BASE] = l; 
            #ifdef tt_STAT
                if (tracingrestores > 0) restore_trace(p, S(480));
            #endif // #283.3: tt_STAT
                /*:283*/
        } else {                            
            #ifdef tt_STAT
                if (tracingrestores > 0) restore_trace(p, S(479));
            #endif // #283.4: tt_STAT
        }
    } // while (true)

    curgroup = savelevel(saveptr);
    curboundary = saveindex(saveptr); /*:282*/
}
/*:281*/

/*288:*/
void prepare_mag(void) {
    if (magset > 0 && mag != magset) {
        print_err(S(481)); // "Incompatible magnification ("
        print_int(mag);
        print(S(482)); // ");"
        printnl(S(483)); // " the previous value will be retained"
        // "I can handle only one magnification ratio per job. So I've"
        // "reverted to the magnification you used earlier on this run."
        help2(S(484), S(485));
        int_error(magset);
        geq_word_define(INT_BASE + magcode, magset);
    }

    if (mag <= 0 || mag > 32768L) {
        print_err(S(486)); // "Illegal magnification has been changed to 1000"
        // "The magnification ratio must be between 1 and 32768."
        help1(S(487));
        int_error(mag);
        geq_word_define(INT_BASE + magcode, 1000);
    }
    magset = mag;
}
/*:288*/

/** @}*/ // end group S268x288_P109x114