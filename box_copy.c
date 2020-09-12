#include "tex_types.h"
#include "tex_constant.h"
#include "mem.h"
#include "error.h" // confusion
#include "extension.h" // [const] opennode, [macro] writetokens
#include "box.h"


/** @addtogroup S203x206_P71x72
 * @{
 */

/*204:*/
HalfWord copynodelist(HalfWord p) {
    Pointer h, q, r = 0 /* XXXX */;
    char words;

    h = get_avail();
    q = h;
    while (p != 0) {
        /// [#205] Make a copy of node p in node r.
        words = 1; // this setting occurs in more branches than any other
        if (ischarnode(p)) {
            r = get_avail();
        #ifdef BIG_CHARNODE
            words = CHAR_NODE_SIZE;
        #endif
        } else {
            // Case statement to copy different types and set words to 
            //  the number of initial words not yet copied
            switch (type(p)) {
                case HLIST_NODE:
                case VLIST_NODE:
                case UNSET_NODE:
                    r = get_node(boxnodesize);
                    // copy the last two words
                    mem[r - MEM_MIN + 6] = mem[p - MEM_MIN + 6];
                    mem[r - MEM_MIN + 5] = mem[p - MEM_MIN + 5];
                    // this affects mem[r + 5]
                    listptr(r) = copynodelist(listptr(p)); 
                    words = 5;
                    break;

                case RULE_NODE:
                    r = get_node(rulenodesize);
                    words = rulenodesize;
                    break;

                case INS_NODE:
                    r = get_node(insnodesize);
                    mem[r - MEM_MIN + 4] = mem[p - MEM_MIN + 4];
                    addglueref(splittopptr(p));
                    // this affects mem[r + 4]
                    insptr(r) = copynodelist(insptr(p));
                    words = insnodesize - 1;
                    break;

                case WHATSIT_NODE:
                    switch (subtype(p)) {
                        // [#1357] Make a partial copy of the whatsit node p  
                        //  and make r point to it;  set words to the number of
                        //  initial words not yet copied.
                        case opennode:
                            r = get_node(opennodesize);
                            words = opennodesize;
                            break;

                        case writenode:
                        case specialnode:
                            r = get_node(writenodesize);
                            addtokenref(writetokens(p));
                            words = writenodesize;
                            break;

                        case closenode:
                        case languagenode:
                            r = get_node(smallnodesize);
                            words = smallnodesize;
                            break;

                        default:
                            confusion(S(429)); // "ext2"
                            break;
                    } // switch (subtype(p)
                    break;

                case GLUE_NODE:
                    r = get_node(smallnodesize);
                    addglueref(glueptr(p));
                    glueptr(r) = glueptr(p);
                    leaderptr(r) = copynodelist(leaderptr(p));
                    break;

                case KERN_NODE:
                case MATH_NODE:
                case PENALTY_NODE:
                    r = get_node(smallnodesize);
                    words = smallnodesize;
                    break;

                case LIGATURE_NODE:
                    // copy font and character
                    r = get_node(smallnodesize);
                    mem[ligchar(r) - MEM_MIN] = mem[ligchar(p) - MEM_MIN];
                    ligptr(r) = copynodelist(ligptr(p));
                    break;

                case DISC_NODE:
                    r = get_node(smallnodesize);
                    prebreak(r) = copynodelist(prebreak(p));
                    postbreak(r) = copynodelist(postbreak(p));
                    break;

                case MARK_NODE:
                    r = get_node(smallnodesize);
                    addtokenref(markptr(p));
                    words = smallnodesize;
                    break;

                case ADJUST_NODE:
                    r = get_node(smallnodesize);
                    adjustptr(r) = copynodelist(adjustptr(p));
                    // words = 1 = smallnodesize − 1
                    break;

                default:
                    confusion(S(430)); // "copying"
                    break;
            } // switch (type(p)
        } // if (<=>ischarnode(p))

        while (words > 0) {
            words--;
            mem[r + words - MEM_MIN] = mem[p + words - MEM_MIN];
        } // while (words > 0)

        link(q) = r;
        q = r;
        p = link(p);
    } // while (p != 0)

    link(q) = null;
    q = link(h);
    FREE_AVAIL(h);
    return q;
} // copynodelist

/** @}*/ // end group S203x206_P71x72