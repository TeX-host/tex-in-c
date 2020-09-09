#include "tex_types.h"
#include "tex_constant.h"
#include "mem.h" // [var] mem
#include "texmac.h" // [macro] writetokens, displaymlist, textmlist, 
    // scriptmlist, scriptscriptmlist, mathtype, supscr, subscr
#include "error.h"   // confusion
#include "box.h"

/** @addtogroup S199x202_P69x70
 * @{
 */

// [#200] p points to the reference count of a token list that
//  is losing one reference
void delete_token_ref(HalfWord p) {
    if (tokenrefcount(p) == 0) {
        flush_list(p);
    } else {
        tokenrefcount(p)--;
    }
} // [#200] delete_token_ref

// [#201] p points to a glue specification
void delete_glue_ref(HalfWord p) {
    if (gluerefcount(p) == 0) {
        free_node(p, gluespecsize);
    } else {
        gluerefcount(p)--;
    }
} // [#201] delete_glue_ref

// [#202] erase list of nodes starting at p
void flush_node_list(HalfWord p) {
    Pointer q;

    while (p != 0) {
        q = link(p);
        if (ischarnode(p)) {
            FREE_AVAIL(p);
        } else {
            switch (type(p)) {
                case HLIST_NODE:
                case VLIST_NODE:
                case UNSET_NODE:
                    flush_node_list(listptr(p));
                    free_node(p, boxnodesize);
                    goto _Ldone;
                    break;

                case RULE_NODE:
                    free_node(p, rulenodesize);
                    goto _Ldone;
                    break;

                case INS_NODE:
                    flush_node_list(insptr(p));
                    delete_glue_ref(splittopptr(p));
                    free_node(p, insnodesize);
                    goto _Ldone;
                    break;

                case WHATSIT_NODE:
                    // #1358: Wipe out the whatsit node p and goto done
                    switch (subtype(p)) {
                        case opennode: 
                            free_node(p, opennodesize); 
                            break;

                        case writenode:
                        case specialnode:
                            delete_token_ref(writetokens(p));
                            free_node(p, writenodesize);
                            goto _Ldone;
                            break;

                        case closenode:
                        case languagenode: 
                            free_node(p, smallnodesize); 
                            break;

                        default:
                            confusion(S(427)); // "ext3"
                            break;
                    } // switch (subtype(p))
                    goto _Ldone;
                    break;

                case GLUE_NODE:
                    delete_glue_ref(glueptr(p));
                    if (leaderptr(p) != 0) flush_node_list(leaderptr(p));
                    break;

                case KERN_NODE:
                case MATH_NODE:
                case PENALTY_NODE:
                    /* blank case */
                    break;

                case LIGATURE_NODE: flush_node_list(ligptr(p)); break;
                case MARK_NODE: delete_token_ref(markptr(p)); break;

                case DISC_NODE:
                    flush_node_list(prebreak(p));
                    flush_node_list(postbreak(p));
                    break;

                // #698: Cases of flush node list that arise in mlists only
                case ADJUST_NODE:
                    flush_node_list(adjustptr(p));
                    break;

                case stylenode:
                    free_node(p, stylenodesize);
                    goto _Ldone;
                    break;

                case choicenode:
                    flush_node_list(displaymlist(p));
                    flush_node_list(textmlist(p));
                    flush_node_list(scriptmlist(p));
                    flush_node_list(scriptscriptmlist(p));
                    free_node(p, stylenodesize);
                    goto _Ldone;
                    break;

                case ordnoad:
                case opnoad:
                case binnoad:
                case relnoad:
                case opennoad:
                case closenoad:
                case punctnoad:
                case innernoad:
                case radicalnoad:
                case overnoad:
                case undernoad:
                case vcenternoad:
                case accentnoad:
                    if (mathtype(nucleus(p)) >= subbox)
                        flush_node_list(info(nucleus(p)));
                    if (mathtype(supscr(p)) >= subbox)
                        flush_node_list(info(supscr(p)));
                    if (mathtype(subscr(p)) >= subbox)
                        flush_node_list(info(subscr(p)));
                    
                    if (type(p) == radicalnoad) {
                        free_node(p, radicalnoadsize);
                    } else if (type(p) == accentnoad) {
                        free_node(p, accentnoadsize);
                    } else {
                        free_node(p, noadsize);
                    }
                    goto _Ldone;
                    break;

                case leftnoad:
                case rightnoad:
                    free_node(p, noadsize);
                    goto _Ldone;
                    break;

                case fractionnoad:
                    flush_node_list(info(numerator(p)));
                    flush_node_list(info(denominator(p)));
                    free_node(p, fractionnoadsize);
                    goto _Ldone;
                    break;

                default: 
                    confusion(S(428)); // "flushing"
                    break;
            }
            free_node(p, smallnodesize);
        _Ldone:;
        }
        p = q;
    } // while (p != 0)
} // [#202] flush_node_list
/** @}*/ // end group S199x202_P69x70