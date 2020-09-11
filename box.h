#pragma once
#ifndef INC_BOX_H
/** Box related.
 * 
 * [#133] Data structures for boxes and their friends.
 * 
 */
#define INC_BOX_H
#include "str.h" // [type] StrNumber

/** @addtogroup S133x161_P50x57
 * @{
 */
/// [p50#133] identifies what kind of node this is.
#define type(x)         (mem[(x)].hh.UU.U2.b0)
/// [p50#133] secondary identification in some cases.
#define subtype(x)      (mem[(x)].hh.UU.U2.b1)

/// [p50#134] does the argument point to a char node?
#define ischarnode(x)   ((x) >= hi_mem_min)

#ifdef BIG_CHARNODE
/// [50#134] the font code in a `charnode`.
#define font(x)         link(x + 1)
/* #define font(x)	info(x+1) */
#else
#define font(x)         type(x)
#endif // #ifdef BIG_CHARNODE

/// [50#134] the character code in a `charnode`.
#define character(x)    subtype(x)

enum NodeType {
    HLIST_NODE,  ///< #135: type of hlist nodes
    VLIST_NODE,  ///< #137: type of vlist nodes
    RULE_NODE,   ///< #138: type of rule nodes
    INS_NODE,    ///< #140: type of insertion nodes
    MARK_NODE,   ///< #141: type of a mark node
    ADJUST_NODE, ///< #142: type of an adjust node

    LIGATURE_NODE, ///< #143: type of a ligature node
    DISC_NODE,     ///< #145: type of a discretionary node
    WHATSIT_NODE,  ///< #146: type of special extension nodes
    MATH_NODE,     ///< #147: type of a math node
    GLUE_NODE, ///< #150: number of words to allocate for a glue specification

    KERN_NODE,    ///< #155: type of a kern node
    PENALTY_NODE, ///< #157: type of a penalty node
    UNSET_NODE,   ///< #159: type for an unset node
}; // enum NodeType

/// [p51#135] number of words to allocate for a box node.
#define boxnodesize     7
/// [p51#135] position of width field in a box node.
#define widthoffset     1
/// [p51#135] position of depth field in a box node.
#define depthoffset     2
/// [p51#135] position of height field in a box node.
#define heightoffset    3

/// [p51#135] width of the box, in sp.
#define width(x)        mem[x + widthoffset].sc
/// [p51#135] depth of the box, in sp.
#define depth(x)        mem[x + depthoffset].sc
/// [p51#135] height of the box, in sp.
#define height(x)       mem[x + heightoffset].sc
/// [p51#135] repositioning distance, in sp.

#define shiftamount(x)  mem[x + 4].sc
/// [p51#135] position of list ptr field in a box node.
#define listoffset      5
/// [p51#135] beginning of the list inside the box.
#define listptr(x)      link(x + listoffset)
/// [p51#135] applicable order of INFINITY.
#define glueorder(x)    subtype(x + listoffset)
/// [p51#135] stretching or shrinking.
#define gluesign(x)     type(x + listoffset)

/// [p51#135] the most common case when several cases are named.
#define NORMAL          0
/// [p51#135] glue setting applies to the stretch components.
#define stretching      1
/// [p51#135] glue setting applies to the shrink components.
#define shrinking       2
/// [p51#135]  position of #glueset in a box node.
#define glueoffset      6
/// [p51#135] a word of type |glueratio| for glue setting.
#define glueset(x)      mem[x + glueoffset].gr

/// [#138]: number of words to allocate for a rule node
#define rulenodesize    4
/// [#138]: -(2^30)  signifies a missing item
#define nullflag        (-1073741824L)
/// [p51#138] tests for a running dimension.
#define isrunning(x)    ((x) == nullflag)


/// [p52#140]: number of words to allocate for an insertion
#define insnodesize     5
/// [p52#140] the |floatingpenalty| to be used.
#define floatcost(x)    mem[x + 1].int_
/// [p52#140] the vertical list to be inserted.
#define insptr(x)       info(x + 4)
/// [p52#140] the |splittopskip| to be used.
#define splittopptr(x)  link(x + 4)

/// [#141]: number of words to allocate for most node types
#define smallnodesize   2
/// [p52#141] head of the token list for a mark.
#define markptr(x)      mem[x + 1].int_

/// [p52#142] vertical list to be moved out of horizontal list.
#define adjustptr       markptr

/// [p52#143] the word where the ligature is to be found.
#define ligchar(x)      ((x) + 1)
/// [p52#143] the list of characters.
#define ligptr(x)       link(ligchar(x))


/// [p53#145] how many subsequent nodes to replace.
#define replacecount    subtype 
/// [p53#145] text that precedes a discretionary break.
#define prebreak        llink   
/// [p53#145] text that follows a discretionary break.
#define postbreak       rlink


/// [#147]: subtype for math node that introduces a formula
#define before          0
/// [#147]: subtype for math node that winds up a formula
#define after           1

/// [p54#148]
#define precedesbreak(x)    (type(x) < MATH_NODE)
/// [p54#148]
#define nondiscardable(x)   (type(x) < MATH_NODE)

/// [p54#149] special subtype to suppress glue in the next node.
#define condmathglue    98
/// [p54#149] subtype for math glue.
#define muglue          99
/// [p54#149] subtype for aligned leaders.
#define aleaders        100
/// [p54#149] subtype for centered leaders.
#define cleaders        101
/// [p54#149] subtype for expanded leaders.
#define xleaders        102
/// [p54#149] pointer to a glue specification.
#define glueptr         llink   
/// [p54#149] pointer to box or rule node for leaders.
#define leaderptr       rlink


/// [#150]: number of words to allocate for a glue specification
#define gluespecsize    4
/// [p55#150] reference count of a glue specification.
#define gluerefcount(x) link(x)
/// [p55#150] the stretchability of this glob of glue.
#define stretch(x)      mem[x + 2].sc
/// [p55#150] the shrinkability of this glob of glue.
#define shrink(x)       mem[x + 3].sc
/// [p55#150] order of INFINITY for stretching.
#define stretchorder(x) type(x)
/// [p55#150] order of INFINITY for shrinking.
#define shrinkorder(x)  subtype(x)
/** [#150]: the orders of INFINITY (normal, `fil`, `fill`, or `filll`)
 * corresponding to the stretch and shrink values.
 * 
 */
enum InfinityOrder {
    FIL = 1, ///< first-order INFINITY
    FILL,    ///< second-order INFINITY
    FILLL    ///< third-order INFINITY
}; // [#150] enum InfinityOrder


/// [#155]: subtype of kern nodes from `\kern` and `\/`
#define explicit        1
/// [#155]: subtype of kern nodes from accents
#define acckern         2

/// [#157]: "infinite" penalty value
#define INF_PENALTY     INF_BAD
/// [#157]: "negatively infinite penalty value
#define EJECT_PENALTY   (-INF_PENALTY)
/// [p56#157] the added cost of breaking a list here.
#define penalty(x)      mem[x + 1].int_ 


/// [p57#159] total stretch in an unset node.
#define gluestretch(x)  mem[x + glueoffset].sc    
/// [p57#159] total shrink in an unset node.
#define glueshrink      shiftamount 
/// [p57#159] indicates the number of spanned columns.
#define spancount       subtype
/** @} end group S133x161_P50x57 */

/** @addtogroup S173x198_P62x68
 * @{
 */
/// [p64#180] str_room need not be checked; see #show_box
#define nodelistdisplay(x) (append_char('.'), shownodelist(x), flush_char())
/** @}*/ // end group S173x198_P62x68

/** @addtogroup S199x202_P69x70
 * @{
 */
// [#200] reference count preceding a token list
#define tokenrefcount(x) info(x)
/** @}*/ // end group S199x202_P69x70

/** @addtogroup S203x206_P71x72
 * @{
 */

/// [#203] new reference to a token list.
#define addtokenref(x)  (tokenrefcount(x)++)
/// [#203] new reference to a glue spec.
#define addglueref(x)   (gluerefcount(x)++)
/** @}*/ // end group S203x206_P71x72


// box_display
extern Integer font_in_short_display;
extern Integer depth_threshold;
extern Integer breadth_max;

// [box], tex, scan
extern Pointer newnullbox(void);
extern Pointer newrule(void);
extern Pointer newligature(QuarterWord f, QuarterWord c, Pointer q);
extern Pointer newligitem(QuarterWord c);
extern Pointer newdisc(void);
extern Pointer newmath(Scaled w, SmallNumber s);
extern Pointer newspec(Pointer p);
extern Pointer newparamglue(SmallNumber n);
extern Pointer newglue(Pointer q);
extern Pointer newskipparam(SmallNumber n);
extern Pointer newkern(long w);
extern Pointer newpenalty(long m);

// box_display
extern void shortdisplay(Pointer p);
extern void printspec(long p, StrNumber s);
extern void shownodelist(Integer p);
extern void showbox(HalfWord p);

// box_destroy
extern void delete_token_ref(HalfWord p);
extern void delete_glue_ref(HalfWord p);
extern void flush_node_list(HalfWord p);

// box_copy
extern HalfWord copynodelist(HalfWord p);

#endif /* INC_BOX_H */