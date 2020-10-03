#ifndef INC_MEM_H

#define INC_MEM_H
#include "global.h"
#include "tex_constant.h" // [const] MEM_MAX, MEM_MIN, MEM_BOT, MEM_TOP, HYPH_SIZE, MAX_QUARTER_WORD, MAX_HALF_WORD


/** @addtogroup S115x132_P44x49
 * @{
 */
/// [p44#115] the #null pointer.
/// null ≡ min_halfword.
#define null 0

/// [p45#118] the `link` field of a memory word.
#define link(x) (mem[(x)].hh.rh)
/// [p45#118] the `info` field of a memory word.
#define info(x) (mem[(x)].hh.UU.lh)

/// [p46#124] the `link` of an empty variable-size node.
#define empty_flag      MAX_HALF_WORD
/// [p46#124] tests for empty node.
#define is_empty(x)    (link(x) == empty_flag)
/// [p46#124] the size field in empty variable-size nodes.
#define node_size       info
/// [p46#124] left link in doubly-linked list of empty nodes.
#define llink(x)        info(x+1)
/// [p46#124] right link in doubly-linked list of empty nodes.
#define rlink(x)        link(x+1)

#ifdef tt_STAT
/// [p45#121] single-word node liberation.
/// a one-word node is recycled by calling FREE_AVAIL.
#define FREE_AVAIL(x)   (link(x) = avail, avail = (x), dyn_used -= CHAR_NODE_SIZE)

/// [#122] avoid get_avail() if possible, to save time.
#define FAST_GET_AVAIL(x)               \
    do {                                \
        if (((x) = avail) != null) {    \
            type(x) = charnodetype;     \
            avail = link(x);            \
            link(x) = null;             \
            dyn_used += CHAR_NODE_SIZE; \
        } else {                        \
            (x) = get_avail();          \
        }                               \
    } while (0)
// end #define FAST_GET_AVAIL(x)
#else
// not defined(tt_STAT)

#define FREE_AVAIL(x) (link(x) = avail, avail = (x))

#define FAST_GET_AVAIL(x)           \
    do {                            \
        if (((x) = avail)) {        \
            type(x) = charnodetype; \
            avail = link(x);        \
            link(x) = 0;            \
        } else {                    \
            (x) = get_avail();      \
        }                           \
    } while (0)
// end #define FAST_GET_AVAIL(x)
#endif // #121,122: tt_STAT
/** @}*/ // end group S115x132_P44x49

/* P2C compatibility */
#define P_clrbits_B(trie, idx, z, w) trie[(idx) >> 3] &= ~(1 << ((idx)&7))
#define P_getbits_UB(trie, h, z, w) (trie[(h) >> 3] & (1 << ((h)&7)))
#define P_putbits_UB(trie, h, y, z, w) trie[(h) >> 3] |= 1 << ((h)&7)

/** @addtogroup S162x172_P58x61
 * @{
 */
/// [#162] specification for `0pt plus 0pt minus 0pt`.
#define zeroglue        MEM_BOT
/// [#162] `0pt plus 1fil minus 0pt`.
#define filglue         (zeroglue + gluespecsize)
/// [#162] `0pt plus 1fill minus 0pt`.
#define fillglue        (filglue + gluespecsize)
/// [#162] `0pt plus 1fil minus 1fil`.
#define ssglue          (fillglue + gluespecsize)
/// [#162] `0pt plus −1fil minus 0pt`.
#define filnegglue      (ssglue + gluespecsize)
/// [#162] largest statically allocated word in the variable-size #mem.
#define lomemstatmax    (filnegglue + gluespecsize - 1)

/// [#162] list of insertion data for current page.
#define pageinshead     (MEM_TOP-CHAR_NODE_SIZE+1)
/// [#162] vlist of items not yet on current page.
#define contribhead     (pageinshead-CHAR_NODE_SIZE)
/// [#162] vlist for current page.
#define pagehead        (contribhead-CHAR_NODE_SIZE)
/// [#162] head of a temporary list of some kind.
#define temphead        (pagehead-CHAR_NODE_SIZE)
/// [#162] head of a temporary list of another kind.
#define holdhead        (temphead-CHAR_NODE_SIZE)
/// [#162] head of adjustment list returned by hpack.
#define adjusthead      (holdhead-CHAR_NODE_SIZE)
/// [#162] head of active list in line break, needs two words.
#define active          (adjusthead-CHAR_NODE_SIZE-CHAR_NODE_SIZE)
/// [#162] head of preamble list for alignments.
#define alignhead       (active-CHAR_NODE_SIZE)
/// [#162] tail of spanned-width lists.
#define endspan         (alignhead-CHAR_NODE_SIZE)
/// [#162] a constant token list.
#define omittemplate    (endspan-CHAR_NODE_SIZE)
/// [#162] permanently empty list.
#define nulllist        (omittemplate-CHAR_NODE_SIZE)
/// [#162] a ligature masquerading as a `char_node`.
#define ligtrick        (nulllist-CHAR_NODE_SIZE)
/// [#162] used for scrap information.
#define garbage         (ligtrick)
/// [#162] head of token list built by `scan_keyword`.
#define backuphead      (ligtrick-CHAR_NODE_SIZE)
/// [#162] smallest statically allocated word in the one-word #mem.
#define himemstatmin    (backuphead)
/// [#162] the number of one-word nodes always present.
/// [ori:14] 
/// TODO: check 14*CHAR_NODE_SIZE
#define himemstatusage  14
/** @}*/ // end group S162x172_P58x61


// tex only
extern Pointer temp_ptr;
extern Integer var_used;
extern Integer dyn_used;
extern Pointer avail; // tex/expand
extern Pointer rover;

// tex, lexer
extern Pointer lo_mem_max;
extern Pointer hi_mem_min;
extern Pointer mem_end;

// tex|texmac|tex_inc, scan, 
extern MemoryWord mem[MEM_MAX - MEM_MIN + 1];

// [ #162~172: MEMORY LAYOUT ]
#ifdef tt_DEBUG
extern UChar free_cells[(MEM_MAX - MEM_MIN + 8) / 8];
extern UChar was_free[(MEM_MAX - MEM_MIN + 8) / 8];
extern Pointer was_mem_end, was_lo_max, was_hi_min;
extern Boolean panicking;
#endif // #165: tt_DEBUG


extern Pointer get_lo_mem_max(void);
extern Pointer get_avail(void);
extern void flush_list(HalfWord p);
extern HalfWord get_node(Integer s);
extern void free_node(Pointer p, HalfWord s);
extern void sort_avail(void);

// tex only
extern void check_mem(Boolean printlocs);
extern void search_mem(Pointer p);
extern void mem_init_once();
extern void mem_var_init();

#endif /* INC_MEM_H */