#include "tex_types.h"
#include "tex_constant.h"
#include "mem.h" // [var] mem
#include "eqtb.h" // glue_par
#include "box.h"


/*136:*/
Pointer newnullbox(void) {
    Pointer p;

    p = get_node(boxnodesize);
    type(p) = HLIST_NODE;
    subtype(p) = MIN_QUARTER_WORD;
    width(p) = 0;
    depth(p) = 0;
    height(p) = 0;
    shiftamount(p) = 0;
    listptr(p) = 0;
    gluesign(p) = NORMAL;
    glueorder(p) = NORMAL;
    glueset(p) = 0.0;
    return p;
}
/*:136*/

/*139:*/
Pointer newrule(void) {
    Pointer p;

    p = get_node(rulenodesize);
    type(p) = RULE_NODE;
    subtype(p) = 0;
    width(p) = nullflag;
    depth(p) = nullflag;
    height(p) = nullflag;
    return p;
}
/*:139*/

/*144:*/
Pointer newligature(QuarterWord f, QuarterWord c, Pointer q) {
    Pointer p;

    p = get_node(smallnodesize);
    type(p) = LIGATURE_NODE;
    font_ligchar(p) = f;
    character_ligchar(p) = c;
    ligptr(p) = q;
    subtype(p) = 0;
    return p;
}


Pointer newligitem(QuarterWord c) {
    Pointer p;

    p = get_node(smallnodesize);
    character(p) = c;
    ligptr(p) = 0;
    return p;
}
/*:144*/

/*145:*/
Pointer newdisc(void) {
    Pointer p;

    p = get_node(smallnodesize);
    type(p) = DISC_NODE;
    replacecount(p) = 0;
    prebreak(p) = 0;
    postbreak(p) = 0;
    return p;
}
/*:145*/

/*147:*/
Pointer newmath(long w, SmallNumber s) {
    Pointer p;

    p = get_node(smallnodesize);
    type(p) = MATH_NODE;
    subtype(p) = s;
    width(p) = w;
    return p;
}
/*:147*/

/*151:*/
Pointer newspec(Pointer p) {
    Pointer q;

    q = get_node(gluespecsize);
    mem[q - MEM_MIN] = mem[p - MEM_MIN];
    gluerefcount(q) = 0;
    width(q) = width(p);
    stretch(q) = stretch(p);
    shrink(q) = shrink(p);
    return q;
}
/*:151*/

/*152:*/
Pointer newparamglue(SmallNumber n) {
    Pointer p, q;

    p = get_node(smallnodesize);
    type(p) = GLUE_NODE;
    subtype(p) = n + 1;
    leaderptr(p) = 0;
    q = glue_par(n); /*224:*/
    /*:224*/
    glueptr(p) = q;
    (gluerefcount(q))++;
    return p;
}
/*:152*/

/*153:*/
Pointer newglue(Pointer q) {
    Pointer p;

    p = get_node(smallnodesize);
    type(p) = GLUE_NODE;
    subtype(p) = NORMAL;
    leaderptr(p) = 0;
    glueptr(p) = q;
    (gluerefcount(q))++;
    return p;
}
/*:153*/

/*154:*/
Pointer newskipparam(SmallNumber n) {
    Pointer p;

    temp_ptr = newspec(glue_par(n)); /*224:*/
    /*:224*/
    p = newglue(temp_ptr);
    gluerefcount(temp_ptr) = 0;
    subtype(p) = n + 1;
    return p;
}
/*:154*/

/*156:*/
Pointer newkern(long w) {
    Pointer p;

    p = get_node(smallnodesize);
    type(p) = KERN_NODE;
    subtype(p) = NORMAL;
    width(p) = w;
    return p;
}
/*:156*/

/*158:*/
Pointer newpenalty(long m) {
    Pointer p;

    p = get_node(smallnodesize);
    type(p) = PENALTY_NODE;
    subtype(p) = 0;
    penalty(p) = m;
    return p;
}
/*:158*/