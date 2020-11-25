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
MemoryWord save_stack[SAVE_SIZE + 1];
UInt16 save_ptr;              // first unused entry on save stack
UInt16 max_save_stack;  // maximum usage of save stack
QuarterWord cur_level; // current nesting level for groups
GroupCode cur_group;   // current group type
UInt16 cur_boundary;   // where the current level begins
// [#286] if nonzero,
// this magnification should be used henceforth
Integer mag_set;

/// [#272, #287]
void eqtb_save_init() {
    /// [#272]
    save_ptr = 0;
    cur_level = LEVEL_ONE;
    cur_group = bottomlevel;
    cur_boundary = 0;
    max_save_stack = 0;
    /// [#287]
    mag_set = 0;
}

/*274:*/
void new_save_level(GroupCode c) {
    if (save_ptr > max_save_stack) {
        max_save_stack = save_ptr;
        // "save size"
        if (max_save_stack > SAVE_SIZE - 6) overflow(S(476), SAVE_SIZE);
    }
    savetype(save_ptr) = levelboundary;
    savelevel(save_ptr) = cur_group;
    saveindex(save_ptr) = cur_boundary;
    if (cur_level == MAX_QUARTER_WORD) {
        // "grouping levels"
        overflow(S(477), MAX_QUARTER_WORD - MIN_QUARTER_WORD);
    }
    cur_boundary = save_ptr;
    cur_level++;
    save_ptr++;
    cur_group = c;
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
    if (save_ptr > max_save_stack) {
        max_save_stack = save_ptr;
        // "save size"
        if (max_save_stack > SAVE_SIZE - 6) overflow(S(476), SAVE_SIZE);
    }
    if (l == LEVEL_ZERO) {
        savetype(save_ptr) = restorezero;
    } else {
        save_stack[save_ptr] = eqtb[p];
        save_ptr++;
        savetype(save_ptr) = restoreoldvalue;
    }
    savelevel(save_ptr) = l;
    saveindex(save_ptr) = p;
    save_ptr++;
}
/*:276*/

/*277:*/
void eq_define(HalfWord p, QuarterWord t, HalfWord e) {
    if (eq_level(p) == cur_level)
        eq_destroy(eqtb[p]);
    else if (cur_level > LEVEL_ONE)
        eq_save(p, eq_level(p));
    eq_level(p) = cur_level;
    eq_type(p) = t;
    equiv(p) = e;
}
/*:277*/

/*278:*/
void eq_word_define(HalfWord p, long w) {
    if (xeqlevel[p - INT_BASE] != cur_level) {
        eq_save(p, xeqlevel[p - INT_BASE]);
        xeqlevel[p - INT_BASE] = cur_level;
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
    if (cur_level <= LEVEL_ONE) return;
    if (save_ptr > max_save_stack) {
        max_save_stack = save_ptr;
        // "save size"
        if (max_save_stack > SAVE_SIZE - 6) overflow(S(476), SAVE_SIZE);
    }
    savetype(save_ptr) = inserttoken;
    savelevel(save_ptr) = LEVEL_ZERO;
    saveindex(save_ptr) = t;
    save_ptr++;
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

    if (cur_level <= LEVEL_ONE) {
        confusion(S(478)); // "cur_level"
        return;
    }
    cur_level--; /*282:*/
    while (true) {
        save_ptr--;
        if (savetype(save_ptr) == levelboundary) break;
        p = saveindex(save_ptr);
        if (savetype(save_ptr) == inserttoken) { /*326:*/
            HalfWord t = curtok;
            curtok = p;
            backinput();
            curtok = t;
            continue;
        }                                           /*:326*/
        if (savetype(save_ptr) == restoreoldvalue) { /*283:*/
            l = savelevel(save_ptr);
            save_ptr--;
        } else {
            save_stack[save_ptr] = eqtb[UNDEFINED_CONTROL_SEQUENCE];
        }
        if (p < INT_BASE) {
            if (eq_level(p) == LEVEL_ONE) {
                eq_destroy(save_stack[save_ptr]);
                #ifdef tt_STAT
                    if (tracingrestores > 0) restore_trace(p, S(479));
                #endif // #283.1: tt_STAT
            } else {
                eq_destroy(eqtb[p]);
                eqtb[p] = save_stack[save_ptr];
                #ifdef tt_STAT
                    if (tracingrestores > 0) restore_trace(p, S(480));
                #endif // #283.2: tt_STAT
            }
            continue;
        }
        if (xeqlevel[p - INT_BASE] != LEVEL_ONE) {
            eqtb[p] = save_stack[save_ptr];
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

    cur_group = savelevel(save_ptr);
    cur_boundary = saveindex(save_ptr); /*:282*/
}
/*:281*/

/*288:*/
void prepare_mag(void) {
    if (mag_set > 0 && mag != mag_set) {
        print_err(S(481)); // "Incompatible magnification ("
        print_int(mag);
        print(S(482)); // ");"
        printnl(S(483)); // " the previous value will be retained"
        // "I can handle only one magnification ratio per job. So I've"
        // "reverted to the magnification you used earlier on this run."
        help2(S(484), S(485));
        int_error(mag_set);
        geq_word_define(INT_BASE + magcode, mag_set);
    }

    if (mag <= 0 || mag > 32768L) {
        print_err(S(486)); // "Illegal magnification has been changed to 1000"
        // "The magnification ratio must be between 1 and 32768."
        help1(S(487));
        int_error(mag);
        geq_word_define(INT_BASE + magcode, 1000);
    }
    mag_set = mag;
}
/*:288*/

/** @}*/ // end group S268x288_P109x114