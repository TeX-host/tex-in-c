#pragma once
#ifndef INC_HYPHEN_H
///
#define INC_HYPHEN_H
#include "global_const.h"
#include "tex.h"
#include "fonts.h" // [type] InternalFontNumber
#include "eqtb.h" // [macro] language

/** @addtogroup S891x899_P330x333
 * @{
 */

/** @}*/ // end group S891x899_P330x333

/** @addtogroup S900x918_P334x343
 * @{
 */
/// [p337#908]
#define appendcharnodetot(x)   \
    {                          \
        link(t) = get_avail(); \
        t = link(t);           \
        font(t) = hf;          \
        character(t) = x;      \
    }

/// [p337#908]
#define setcurr()                 \
    {                             \
        if (j < n) {              \
            curr = qi(hu[j + 1]); \
        } else {                  \
            curr = bchar;         \
        }                         \
        if (hyf[j] & 1) {         \
            currh = hchar;        \
        } else {                  \
            currh = NON_CHAR;     \
        }                         \
    }

/// [p339#910]
#define wraplig(x)                             \
    if (ligaturepresent) {                     \
        p = newligature(hf, curl, link(curq)); \
        if (lfthit) {                          \
            subtype(p) = 2;                    \
            lfthit = false;                    \
        }                                      \
        if (x)                                 \
            if (ligstack == 0) {               \
                (subtype(p))++;                \
                rthit = false;                 \
            }                                  \
        link(curq) = p;                        \
        t = p;                                 \
        ligaturepresent = false;               \
    }

/// [p339#910] if |ligstack| isn't |null| we have |currh=NON_CHAR|
#define popligstack()                              \
    {                                              \
        if (ligptr(ligstack) > null) {             \
            /* this is a charnode for |hu[j+1]| */ \
            link(t) = ligptr(ligstack);            \
            t = link(t);                           \
            j++;                                   \
        }                                          \
        p = ligstack;                              \
        ligstack = link(p);                        \
        free_node(p, smallnodesize);                \
        if (ligstack == null) {                    \
            setcurr();                             \
        } else {                                   \
            curr = character(ligstack);            \
        }                                          \
    }

/// [p341#914]
#define advancemajortail()           \
    {                                \
        majortail = link(majortail); \
        rcount++;                    \
    }

/** @}*/ // end group S900x918_P334x343

/** @addtogroup S919x941_P344x349
 * @{
 */

/// [p344#921] `downward` link in a trie
#define trielink(x)     trie[x].rh
/// [p344#921] character matched at this trie location
#define triechar(x)     trie[x].UU.U2.b1
/// [p344#921] program for hyphenation at this trie location
#define trieop(x)       trie[x].UU.U2.b0

/// [p347#934]
#define setcurlang()     \
    ((language <= 0)     \
         ? (curlang = 0) \
         : ((language > 255) ? (curlang = 0) : (curlang = language)))

/** @}*/ // end group S919x941_P344x349

/** @addtogroup S942x966_P350x359
 * @{
 */

/// [p352#947]
#define trieroot       (triel[0]) // root of the linked trie

/// [p353#950] backward links in |trie| holes
#define trieback(x)     trie[x].UU.lh

/** @}*/ // end group S942x966_P350x359


// pre-hyphen
extern short hc[66];
extern int hn;
extern InternalFontNumber hf;
extern short hu[64];
extern Integer hyfchar;
extern ASCIICode curlang, initcurlang;
extern Integer lhyf, rhyf, initlhyf, initrhyf;
extern HalfWord hyfbchar;

// post-hyphen
extern char hyf[65];
extern Boolean initlig, initlft;
extern SmallNumber hyphenpassed;
extern HalfWord curl, curr;
extern Boolean ligaturepresent, lfthit, rthit;

extern void hyphenate(void);

// hyphen
extern TwoHalves trie[TRIE_SIZE + 1];
extern SmallNumber hyfdistance[TRIE_OP_SIZE];
extern SmallNumber hyfnum[TRIE_OP_SIZE];
extern QuarterWord hyfnext[TRIE_OP_SIZE];
extern short opstart[256];

extern StrNumber hyphword[HYPH_SIZE + 1];
extern Pointer hyphlist[HYPH_SIZE + 1];
extern HyphPointer hyphcount;

extern void hyphen_init();
extern void newhyphexceptions(void);


// hyphen-init
extern short trieophash[TRIE_OP_SIZE + TRIE_OP_SIZE + 1];
extern QuarterWord trieused[256];
extern ASCIICode trieoplang[TRIE_OP_SIZE];
extern QuarterWord trieopval[TRIE_OP_SIZE];
extern UInt16 trieopptr;

extern PackedASCIICode triec[TRIE_SIZE + 1];
extern QuarterWord trieo[TRIE_SIZE + 1];
extern TriePointer triel[TRIE_SIZE + 1];
extern TriePointer trier[TRIE_SIZE + 1];
extern TriePointer trieptr;

extern TriePointer triehash[TRIE_SIZE + 1];

extern UChar trietaken[(TRIE_SIZE + 7) / 8];
extern TriePointer triemin[256];
extern TriePointer triemax;
extern Boolean trie_not_ready;

extern void hyphen_init_once();
extern QuarterWord newtrieop(SmallNumber d, SmallNumber n, QuarterWord v);
extern TriePointer trienode(TriePointer p);
extern TriePointer compresstrie(TriePointer p);
extern void firstfit(TriePointer p);
extern void triepack(TriePointer p);
extern void triefix(TriePointer p);
extern void newpatterns(void);
extern void inittrie(void);


#endif /* INC_HYPHEN_H */