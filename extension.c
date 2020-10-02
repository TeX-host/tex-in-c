#include <stdlib.h> // labs
#include "io.h"
#include "mem.h"
#include "error.h"
#include "print.h"
#include "lexer.h" // [macro] inslist
#include "dviout.h"
#include "expand.h"
#include "texfunc.h"
#include "main_ctrl.h"
#include "extension.h"

/** @addtogroup S1340x1379_P472x481
 * @{
 */
/// [#1342] Hold 16 possible `\write` streams.
/// Selector [0~15]
FILE* write_file[16];
/// [#1342] Set to true if that file is open.
/// + j=0..16: j-th file is open if and only if write open[j] = true.
/// + write_open[16]: stream number greater than 15
/// + write_open[17]: negative stream number
Boolean write_open[18];

/*1345:*/
Pointer writeloc;
/*:1345*/

/// [#1343]
void extension_init() {
    for (size_t k = 0; k <= 17; k++) { 
        write_open[k] = false;
    }
} /* extension_init */

/// [#1369]
void extension_init_once() {
    set_text(END_WRITE, S(260)); // "endwrite"
    eq_level(END_WRITE) = LEVEL_ONE;
    eq_type(END_WRITE) = OUTER_CALL;
    equiv(END_WRITE) = null;
} /* extension_init_once */

/*1348:*/
/*1349:*/
void newwhatsit(SmallNumber s, SmallNumber w) {
    Pointer p;

    p = get_node(w);
    type(p) = WHATSIT_NODE;
    subtype(p) = s;
    link(tail) = p;
    tail = p;
}
/*:1349*/

/*1350:*/
void newwritewhatsit(SmallNumber w) {
    newwhatsit(curchr, w);
    if (w != writenodesize)
        scan_four_bit_int();
    else {
        scan_int();
        if (cur_val < 0)
            cur_val = 17;
        else if (cur_val > 15)
            cur_val = 16;
    }
    writestream(tail) = cur_val;
}
/*:1350*/

void doextension(void) {
    long k;
    Pointer p;

    switch (curchr) {
        case opennode: /*1351:*/
            newwritewhatsit(opennodesize);
            scan_optional_equals();
            scanfilename();
            openname(tail) = curname;
            openarea(tail) = curarea;
            openext(tail) = curext;
            break;
            /*:1351*/

        case writenode: /*1352:*/
            k = curcs;
            newwritewhatsit(writenodesize);
            curcs = k;
            p = scantoks(false, false);
            writetokens(tail) = defref;
            break;
            /*:1352*/

        case closenode: /*1353:*/
            newwritewhatsit(writenodesize);
            writetokens(tail) = 0;
            break;
            /*:1353*/

        case specialnode: /*1354:*/
            newwhatsit(specialnode, writenodesize);
            writestream(tail) = 0;
            p = scantoks(false, true);
            writetokens(tail) = defref;
            break;
            /*:1354*/

        case immediatecode: /*1375:*/
            get_x_token();
            if (curcmd == EXTENSION && curchr <= closenode) {
                p = tail;
                doextension();
                outwhat(tail);
                flush_node_list(tail);
                tail = p;
                link(p) = 0;
            } else {
                backinput();
            }
            break;
            /*:1375*/

        case setlanguagecode: /*1377:*/
            if (labs(mode) != H_MODE) {
                reportillegalcase();
            } else { /*:1377*/
                newwhatsit(languagenode, smallnodesize);
                scan_int();
                if (cur_val <= 0)
                    clang = 0;
                else if (cur_val > 255)
                    clang = 0;
                else
                    clang = cur_val;
                whatlang(tail) = clang;
                whatlhm(tail) = normmin(lefthyphenmin);
                whatrhm(tail) = normmin(righthyphenmin);
            }
            break;

        default:
            confusion(S(1001)); // "ext1"
            break;
    }
}
/*:1348*/

/*1376:*/
void fixlanguage(void) {
    ASCIICode l;

    if (language <= 0)
        l = 0;
    else if (language > 255)
        l = 0;
    else
        l = language;
    if (l == clang) return;
    newwhatsit(languagenode, smallnodesize);
    whatlang(tail) = l;
    clang = l;
    whatlhm(tail) = normmin(lefthyphenmin);
    whatrhm(tail) = normmin(righthyphenmin);
}
/*:1376*/

// #1368
void specialout(HalfWord p) {
    Selector old_setting;

    synchh();
    synchv();
    old_setting = selector;
    selector = NEW_STRING;
    showtokenlist(link(writetokens(p)), 0, POOL_SIZE /* - pool_ptr */);
    selector = old_setting;
    str_room(1);
    
    int p_len = cur_length(); /* XXXX - Assumes byte=ASCIICode */

    if (p_len < 256) {
        dviout_XXX1();
        dviout(p_len);
    } else {
        dviout_XXX4();
        dvi_four(p_len);
    }
    str_cur_map(dviout_helper);
} // #1368: specialout

/*1370:*/
void writeout(HalfWord p) { /*1371:*/
    Selector old_setting;
    long oldmode;
    /* SmallNumber */ int j; /* INT */
    Pointer q, r;

    q = get_avail();
    info(q) = rightbracetoken + '}';
    r = get_avail();
    link(q) = r;
    info(r) = endwritetoken;
    inslist(q);
    begintokenlist(writetokens(p), WRITE_TEXT);
    q = get_avail();
    info(q) = leftbracetoken + '{';
    inslist(q);
    oldmode = mode;
    mode = 0;
    curcs = writeloc;
    q = scantoks(false, true);
    gettoken();
    if (curtok != endwritetoken) { /*1372:*/
        print_err(S(680)); // "Unbalanced write command"
        // "On this page there's a \\write with fewer real {'s than }'s."
        // "I can't handle that very well; good luck."
        help2(S(681), S(682));
        error();
        do {
            gettoken();
        } while (curtok != endwritetoken);
    }
    /*:1372*/
    mode = oldmode; /*:1371*/
    endtokenlist();
    old_setting = selector;
    j = writestream(p);
    if (write_open[j]) {
        selector = j;
    } else {
        if (j == 17 && selector == TERM_AND_LOG) selector = LOG_ONLY;
        printnl(S(385)); // ""
    }
    tokenshow(defref);
    println();
    flush_list(defref);
    selector = old_setting;
}
/*:1370*/

/*1373:*/
void outwhat(HalfWord p) {
    /* SmallNumber */ int j; /* INT */

    switch (subtype(p)) {
        case opennode:
        case writenode:
        case closenode:          /*1374:*/
            if (!doingleaders) { /*:1374*/
                j = writestream(p);
                if (subtype(p) == writenode)
                    writeout(p);
                else {
                    if (write_open[j]) aclose(&write_file[j]);
                    if (subtype(p) == closenode) {
                        write_open[j] = false;
                    } else if (j < 16) {
                        curname = openname(p);
                        curarea = openarea(p);
                        curext = openext(p);
                        if (curext == S(385)) curext = S(669);
                        packfilename(curname, curarea, curext);
                        while (!a_open_out(&write_file[j]))
                            promptfilename(S(683), S(669));
                        write_open[j] = true;
                    }
                }
            }
            break;

        case specialnode: 
            specialout(p); 
            break;

        case languagenode:
            /* blank case */
            break;

        default:
            confusion(S(684)); // "ext4"
            break;
    }
}
/*:1373*/

/** @}*/ // end group S1340x1379_P472x481