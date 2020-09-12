#include "tex_types.h"
#include "mem.h"
#include "eqtb.h"
#include "linebreak.h"
#include "pack.h"
#include "print.h"
#include "error.h"
#include "expand.h" // [macro] splitbotmark, splitfirstmark
#include "texmath.h" // [macro] INF_BAD
#include "mmode.h"
#include "page.h"


/** @addtogroup S967x979_P360x365
 * @{
 */
/// [#971] 
Scaled bestheightplusdepth;

/*968:*/
HalfWord prunepagetop(HalfWord p) {
    Pointer prevp, q;

    prevp = temphead;
    link(temphead) = p;
    while (p != 0) {
        switch (type(p)) {
            case HLIST_NODE:
            case VLIST_NODE:
            case RULE_NODE: /*969:*/
                q = newskipparam(SPLIT_TOP_SKIP_CODE);
                link(prevp) = q;
                link(q) = p;
                if (width(temp_ptr) > height(p))
                    width(temp_ptr) -= height(p);
                else
                    width(temp_ptr) = 0;
                p = 0;
                break;
                /*:969*/

            case WHATSIT_NODE:
            case MARK_NODE:
            case INS_NODE:
                prevp = p;
                p = link(prevp);
                break;

            case GLUE_NODE:
            case KERN_NODE:
            case PENALTY_NODE:
                q = p;
                p = link(q);
                link(q) = 0;
                link(prevp) = p;
                flush_node_list(q);
                break;

            default:
                confusion(S(791)); // "pruning"
                break;
        }
    }
    return (link(temphead));
}
/*:968*/

/*970:*/
HalfWord vertbreak(HalfWord p, long h, long d) {
    Pointer prevp, q, r, bestplace = p /* XXXX */;
    long pi = 0 /* XXXX */, b, leastcost;
    Scaled prevdp;
    SmallNumber t;

    prevp = p;
    leastcost = awfulbad;
    setheightzero(1);
    setheightzero(2);
    setheightzero(3);
    setheightzero(4);
    setheightzero(5);
    setheightzero(6);
    prevdp = 0;
    while (true) {  /*972:*/
        if (p == 0) { /*974:*/
            pi = EJECT_PENALTY;
        } else {                 /*973:*/
            switch (type(p)) { /*:973*/
                case HLIST_NODE:
                case VLIST_NODE:
                case RULE_NODE:
                    curheight += prevdp + height(p);
                    prevdp = depth(p);
                    goto _Lnotfound;
                    break;

                case WHATSIT_NODE:   /*1365:*/
                    goto _Lnotfound; /*:1365*/
                    break;

                case GLUE_NODE:
                    if (!precedesbreak(prevp)) goto _Lupdateheights_;
                    pi = 0;
                    break;

                case KERN_NODE:
                    if (link(p) == 0) {
                        t = PENALTY_NODE;
                    } else {
                        t = type(link(p));
                    }
                    if (t != GLUE_NODE) goto _Lupdateheights_;
                    pi = 0;
                    break;

                case PENALTY_NODE: 
                    pi = penalty(p); 
                    break;

                case MARK_NODE:
                case INS_NODE: 
                    goto _Lnotfound; 
                    break;

                default:
                    confusion(S(792)); // "vertbreak"
                    break;
            }
        }
    
        if (pi < INF_PENALTY) { /*:974*/ /*975:*/
            if (curheight < h) {
                if (activeheight[2] != 0 || activeheight[3] != 0 ||
                    activeheight[4] != 0)
                    b = 0;
                else
                    b = badness(h - curheight, activeheight[1]);
            } else if (curheight - h > activeheight[5])
                b = awfulbad;
            else
                b = badness(curheight - h, activeheight[5]); /*:975*/
            if (b < awfulbad) {
                if (pi <= EJECT_PENALTY)
                    b = pi;
                else if (b < INF_BAD)
                    b += pi;
                else
                    b = deplorable;
            }
            if (b <= leastcost) {
                bestplace = p;
                leastcost = b;
                bestheightplusdepth = curheight + prevdp;
            }
            if (b == awfulbad || pi <= EJECT_PENALTY) goto _Ldone;
        }

        if ((type(p) < GLUE_NODE) | (type(p) > KERN_NODE)) goto _Lnotfound;

    _Lupdateheights_: /*976:*/
        if (type(p) == KERN_NODE) {
            q = p;
        } else {
            q = glueptr(p);
            activeheight[stretchorder(q) + 1] += stretch(q);
            activeheight[5] += shrink(q);
            if ((shrinkorder(q) != NORMAL) & (shrink(q) != 0)) {
                // "Infinite glue shrinkage found in box being split"
                print_err(S(793));
                /*
                 * (794) "The box you are \\vsplitting contains some infinitely"
                 * (795) "shrinkable glue e.g. `\\vss' or `\\vskip 0pt minus
                 * 1fil'." (796) "Such glue doesn't belong there; but you can
                 * safely proceed" (753) "since the offensive shrinkability has
                 * been made finite."
                 */
                help4(S(794), S(795), S(796), S(753));
                error();
                r = newspec(q);
                shrinkorder(r) = NORMAL;
                delete_glue_ref(q);
                glueptr(p) = r;
                q = r;
            }
        }
        curheight += prevdp + width(q);
        prevdp = 0; /*:976*/

    _Lnotfound:
        if (prevdp > d) { /*:972*/
            curheight += prevdp - d;
            prevdp = d;
        }
        prevp = p;
        p = link(prevp);
    }

_Ldone:
    return bestplace;
}
/*:970*/

/*977:*/
HalfWord vsplit(EightBits n, long h) {
    HalfWord Result;
    Pointer v, p, q;

    v = box(n);
    if (splitfirstmark != 0) {
        delete_token_ref(splitfirstmark);
        splitfirstmark = 0;
        delete_token_ref(splitbotmark);
        splitbotmark = 0;
    }
    /*978:*/
    if (v == 0) {
        Result = 0;
        goto _Lexit;
    }
    if (type(v) != VLIST_NODE) { /*:978*/
        print_err(S(385)); // ""
        print_esc(S(797));
        print(S(798));
        print_esc(S(799));
        help2(S(800), S(801));
        error();
        Result = 0;
        goto _Lexit;
    }
    q = vertbreak(listptr(v), h, splitmaxdepth); /*979:*/
    p = listptr(v);
    if (p == q)
        listptr(v) = 0;
    else {
        while (true) {
            if (type(p) == MARK_NODE) {
                if (splitfirstmark == 0) {
                    splitfirstmark = markptr(p);
                    splitbotmark = splitfirstmark;
                    tokenrefcount(splitfirstmark) += 2;
                } else {
                    delete_token_ref(splitbotmark);
                    splitbotmark = markptr(p);
                    addtokenref(splitbotmark);
                }
            }
            if (link(p) == q) {
                link(p) = 0;
                goto _Ldone;
            }
            p = link(p);
        }
    }
_Ldone: /*:979*/
    q = prunepagetop(q);
    p = listptr(v);
    free_node(v, boxnodesize);
    if (q == 0)
        box(n) = 0;
    else {
        box(n) = vpack(q, 0, additional);
    }
    Result = vpackage(p, h, exactly, splitmaxdepth);
_Lexit:
    return Result;
}
/*:977*/

/** @}*/ // end group S967x979_P360x365