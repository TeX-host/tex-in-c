#include "tex_constant.h" // [const] MEM_MIN, MIN_QUARTER_WORD
#include "eqtb.h" // glue_par
#include "box.h"


/** @addtogroup S133x161_P50x57
 * @{
 */
/** [#136]  creates a new box node.
 *
 * The #new_null_box function returns a pointer to an hlist_node
 *  in which all subfields have the values corresponding
 *  to ‘\\hbox{}’.
 * The subtype field is set to #MIN_QUARTER_WORD,
 *  since that’s the desired span count value
 *  if this hlist_node is changed to an unset_node.
 */
Pointer new_null_box(void) {
    Pointer p;

    p = get_node(boxnodesize);
    type(p) = HLIST_NODE;
    subtype(p) = MIN_QUARTER_WORD;

    width(p) = 0;
    depth(p) = 0;
    height(p) = 0;
    shiftamount(p) = 0;
    listptr(p) = null;

    gluesign(p) = NORMAL;
    glueorder(p) = NORMAL;
    glueset(p) = 0.0;
    return p;
}

/// [#139]
Pointer new_rule(void) {
    Pointer p; // the new node

    p = get_node(rulenodesize);
    type(p) = RULE_NODE;
    subtype(p) = 0; // the subtype is not used

    width(p) = nullflag;
    depth(p) = nullflag;
    height(p) = nullflag;
    return p;
}

/** [#144]
 * creates a ligature node having given contents of the font,
 *  character, and lig_ptr fields.
 */
Pointer new_ligature(QuarterWord f, QuarterWord c, Pointer q) {
    Pointer p;

    p = get_node(smallnodesize);
    type(p) = LIGATURE_NODE;
    font_ligchar(p) = f;

    character_ligchar(p) = c;
    ligptr(p) = q;
    subtype(p) = 0;
    return p;
}

/** [#144]
 * returns a two-word node having a given character field.
 * Such nodes are used for temporary processing as ligatures 
 *  are being created.
 */
Pointer new_lig_item(QuarterWord c) {
    Pointer p;

    p = get_node(smallnodesize);
    character(p) = c;
    ligptr(p) = null;
    return p;
}

/** [#145] creates an empty disc node.
 *
 */
Pointer new_disc(void) {
    Pointer p;

    p = get_node(smallnodesize);
    type(p) = DISC_NODE;
    replacecount(p) = 0;
    prebreak(p) = null;
    postbreak(p) = null;
    return p;
}

/** [#147]
 * A math node, which occurs only in horizontal lists, 
 *  appears before and after mathematical formulas.
 * The subtype field is before before the formula and 
 *  after after it. 
 * There is a width field, which represents the amount of
 *  surrounding space inserted by \\mathsurround.
*/
Pointer new_math(Scaled w, SmallNumber s) {
    Pointer p;

    p = get_node(smallnodesize);
    type(p) = MATH_NODE;
    subtype(p) = s;
    width(p) = w;
    return p;
}

/** [#151] duplicates a glue specification.
 *
 */
Pointer new_spec(Pointer p) {
    Pointer q;

    q = get_node(gluespecsize);
    mem[q - MEM_MIN] = mem[p - MEM_MIN];
    gluerefcount(q) = null;

    width(q) = width(p);
    stretch(q) = stretch(p);
    shrink(q) = shrink(p);
    return q;
}


/// [#152] creates a glue node for a given parameter 
/// identified by its code number;
Pointer new_param_glue(SmallNumber n) {
    Pointer p, q;

    p = get_node(smallnodesize);
    type(p) = GLUE_NODE;
    subtype(p) = n + 1;
    leaderptr(p) = 0;
    // [#224] Current mem equivalent of glue parameter number n
    q = glue_par(n);
    glueptr(p) = q;
    (gluerefcount(q))++;
    return p;
}

/// [#153] 
/// argument points to a glue specification.
Pointer new_glue(Pointer q) {
    Pointer p;

    p = get_node(smallnodesize);
    type(p) = GLUE_NODE;
    subtype(p) = NORMAL;
    leaderptr(p) = null;
    glueptr(p) = q;
    (gluerefcount(q))++;
    return p;
}

/** [#154] creates a glue node for one of the current 
 *  glue parameters. 
 * 
 * but it makes a fresh copy of the glue specification, 
 *  since that specification will probably be subject to change,
 *  while the parameter will stay put.
 */
Pointer new_skip_param(SmallNumber n) {
    Pointer p;

    /// [#224] Current mem equivalent of glue parameter number n.
    temp_ptr = new_spec(glue_par(n));
    p = new_glue(temp_ptr);
    gluerefcount(temp_ptr) = 0;
    subtype(p) = n + 1;
    return p;
}

/// [#156] creates a kern node having a given width.
Pointer new_kern(long w) {
    Pointer p;

    p = get_node(smallnodesize);
    type(p) = KERN_NODE;
    subtype(p) = NORMAL;
    width(p) = w;
    return p;
}

/// [#158]
Pointer new_penalty(long m) {
    Pointer p;

    p = get_node(smallnodesize);
    type(p) = PENALTY_NODE;
    subtype(p) = 0; // the subtype is not used
    penalty(p) = m;
    return p;
}

/** @}*/ // end group S133x161_P50x57