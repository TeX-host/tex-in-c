#include <stdio.h>  // FILE
#include <stdarg.h> // va_start, va_arg, va_end,
#include <stdlib.h> // labs, abs, exit, EXIT_SUCCESS
#include <string.h> // memcpy
#include <math.h>   // fabs

#define charnodetype 0xfff
#undef BIG_CHARNODE
#define BIG_CHARNODE
#define BIG_NOAD

#ifdef BIG_CHARNODE
#define charnodesize 2
#define font_ligchar(x) type(ligchar(x))
#define character_ligchar(x) subtype(ligchar(x))
#else
#define charnodesize 1
#define font_ligchar(x) font(ligchar(x))
#define character_ligchar(x) character(ligchar(x))
#endif // #ifdef BIG_CHARNODE

#include "global_const.h"
#include "tex_inc.h"
#include "tex.h" // [export]
#include "str.h"
#include "texmac.h"
#include "macros.h"
#include "funcs.h"
#include "global.h"
#include "printout.h"
#include "fonts.h"
#include "inputln.h" // [func] inputln
#include "dviout.h"
#include "texfunc.h" // [export]

#define formatextension  S(256)
#define checkinterrupt() ((interrupt != 0) ? (pause_for_instructions(), 0) : 0)


long tex_round(double d) { return (long)(floor(d + 0.5)); }

int pack_tok(int cs, int cmd, int chr) {
    if (cs == 0) {
        return dwa_do_8 * cmd + chr;
    } else {
        return cstokenflag + cs;
    }
}

unsigned char nameoffile[filenamesize];
ASCIICode buffer[bufsize + 1];
short first;
short last;
short maxbufstack;
StrNumber formatident;
InStateRecord curinput;
jmp_buf _JLfinalend;
ASCIICode xord[256];
/*13:*/
Static Scaled maxh, maxv, ruleht, ruledp, rulewd;

/*:13*/


/*20:*/

Static Char xchr[256];
/*:20*/
/*26:*/
Static int namelength;
/*:26*/
/*30:*/
/*:30*/
/*32:*/
// Static FILE *termin = NULL, *termout = NULL;
/*:32*/

/// p24#54: On-line and off-line printing
/// Global variables: in tex.c only
Static FILE* log_file = NULL;   // transcript of T E X session
// Static unsigned char selector; // where to print a message
Static enum Selector selector; // where to print a message
// ? dig[23] // digits in a number being output
Static long tally; // the number of characters recently printed
Static char term_offset; // the number of characters on the current terminal line
Static char file_offset; // the number of characters on the current file line
Static ASCIICode trickbuf[ERROR_LINE + 1]; // circular buffer for pseudoprinting
Static long trickcount, // threshold for pseudoprinting, explained later
            firstcount; // another variable for pseudoprinting


/*73:*/
Static char interaction;
/*:73*/
/*76:*/
Static Boolean deletionsallowed, setboxallowed;
Static enum History history;
Static SChar errorcount;
/*:76*/
/*79:*/
Static StrNumber helpline[6];
Static unsigned char helpptr;
Static Boolean useerrhelp;
/*:79*/
/*96:*/
Static long interrupt;
Static Boolean OKtointerrupt;
/*:96*/

/// #104
// has arithmetic overflow occurred recently?
Static Boolean arith_error = false;
Static Scaled tex_remainder = 0; // amount subtracted to get an exact division

/*115:*/
Static Pointer tempptr, lomemmax, himemmin;
/*:115*/
/*116:*/
Static MemoryWord mem[memmax - memmin + 1];
/*:116*/
/*117:*/
Static long varused, dynused;
/*:117*/
/*118:*/
Static Pointer avail, memend;
/*:118*/
/*124:*/
Static Pointer rover, hashused, curcs, warningindex, defref;
/*:124*/

/// p95#165
#ifdef tt_DEBUG
Static UChar   free_[(memmax - memmin + 8) / 8]; // free cells
Static UChar wasfree[(memmax - memmin + 8) / 8]; // previously free cells
// previous mem end, lo mem max, and hi mem min
Static Pointer wasmemend, waslomax, washimin;
Static Boolean panicking; // do we want to check memory constantly?
#endif // #165: tt_DEBUG

/*173:*/
Static long fontinshortdisplay;
/*:173*/
/*181:*/
Static long depththreshold, breadthmax;
/*:181*/
/*213:*/
Static ListStateRecord nest[nestsize + 1];
Static unsigned char nestptr; /* INT */
Static char maxneststack;
Static ListStateRecord curlist;
Static short shownmode;
/*:213*/
/*246:*/
Static char diag_oldsetting;   /*:246*/
/*253:*/
Static MemoryWord eqtb[eqtbsize - activebase + 1];
/*
extern MemoryWord eqtb[];
MemoryWord * eqtb_foo(void) { return eqtb;}
*/
Static QuarterWord xeqlevel[eqtbsize - intbase + 1];
/*:253*/
/*256:*/
Static TwoHalves hash[undefinedcontrolsequence - hashbase];
Static long cscount;   /*:256*/
/*271:*/
Static MemoryWord savestack[savesize + 1];
Static short saveptr;
Static short maxsavestack;
Static QuarterWord curlevel;
Static GroupCode curgroup;
Static short curboundary;
/*:271*/
/*286:*/
Static long magset;
/*:286*/
/*297:*/
Static EightBits curcmd;
Static HalfWord curchr, curtok;   /*:297*/
/*301:*/
Static InStateRecord inputstack[stacksize + 1];
Static UChar inputptr;
Static UChar maxinstack;
/*:301*/
/*304:*/
Static char inopen;
Static char openparens;
Static FILE *inputfile[maxinopen];
Static long line;
Static long linestack[maxinopen];
/*:304*/
/*305:*/
Static char scannerstatus;
/*:305*/
/*308:*/
Static Pointer paramstack[paramsize + 1];
Static /* char */ int paramptr; /* INT */
Static long maxparamstack;
/*:308*/
/*309:*/
Static long alignstate;
/*:309*/
/*310:*/
Static UChar baseptr;
/*:310*/
/*333:*/
Static Pointer parloc;
Static HalfWord partoken;
/*:333*/
/*361:*/
Static Boolean forceeof;   /*:361*/
/*382:*/
Static Pointer curmark[splitbotmarkcode - topmarkcode + 1];   /*:382*/
/*387:*/
Static char longstate;
/*:387*/
/*388:*/
/*:388*/
/*410:*/
Static long curval;
Static char curvallevel;
/*:410*/
/*438:*/
Static SmallNumber radix;
/*:438*/
/*447:*/
Static GlueOrd curorder;
/*:447*/
/*480:*/
Static FILE *readfile[16];
Static char readopen[17];
/*:480*/
/*489:*/
Static Pointer condptr;
Static char iflimit;
Static SmallNumber curif;
Static long ifline;
/*:489*/
/*493:*/
Static long skipline;
/*:493*/
/*512:*/
Static StrNumber curname, curarea, curext;
/*:512*/
/*513:*/
Static StrNumber extdelimiter;
/*:513*/
/*520:*/
Static Char TEXformatdefault[formatdefaultlength];   /*:520*/
/*527:*/
Static Boolean nameinprogress;
Static StrNumber jobname;
Static Boolean logopened;   /*:527*/
/*532:*/
Static StrNumber outputfilename, logname;   /*:532*/
/*555:*/
Static FourQuarters nullcharacter;
/*:555*/
/*592:*/
Static long totalpages, maxpush, deadcycles;
Static Boolean doingleaders;
Static long lq, lr;
/*:592*/
/*616:*/
Static Scaled dvih, dviv, curh, curv, curmu;
Static InternalFontNumber dvif;
Static long curs;
/*:616*/
/*646:*/
Static Scaled totalstretch[filll - normal + 1],
	      totalshrink[filll - normal + 1];
Static long lastbadness;
/*:646*/
/*647:*/
Static Pointer adjusttail;
/*:647*/
/*661:*/
Static long packbeginline;
/*:661*/
/*684:*/
Static TwoHalves emptyfield;
Static FourQuarters nulldelimiter;
/*:684*/
/*719:*/
Static Pointer curmlist;
Static SmallNumber curstyle, cursize;
Static Boolean mlistpenalties;
/*:719*/
/*724:*/
Static InternalFontNumber curf;
Static QuarterWord curc;
Static FourQuarters curi;
/*:724*/
/*764:*/
/*:764*/
/*770:*/
Static Pointer curalign, curspan, curloop, alignptr, curhead, curtail;
/*:770*/
/*814:*/
Static Pointer justbox;
/*:814*/
/*821:*/
Static Pointer passive, printednode;
Static HalfWord passnumber;
/*:821*/
/*823:*/
Static Scaled activewidth[6];
Static Scaled curactivewidth[6];
Static Scaled background[6];
Static Scaled breakwidth[6];
/*:823*/
/*825:*/
Static Boolean noshrinkerroryet, secondpass, finalpass;
/*:825*/
/*828:*/
Static Pointer curp;
Static long threshold;
/*:828*/
/*833:*/
Static long minimaldemerits[tightfit - veryloosefit + 1];
Static long minimumdemerits;
Static Pointer bestplace[tightfit - veryloosefit + 1];
Static HalfWord bestplline[tightfit - veryloosefit + 1];
/*:833*/
/*839:*/
Static Scaled discwidth, firstwidth, secondwidth, firstindent, secondindent;
/*:839*/
/*847:*/
Static HalfWord easyline, lastspecialline;
/*:847*/
/*872:*/
Static Pointer bestbet, ha, hb, initlist, curq, ligstack;
Static long fewestdemerits;
Static HalfWord bestline;
Static long actuallooseness, linediff;
/*:872*/
/*892:*/
Static short hc[66];
Static /* SmallNumber */ int hn;
Static InternalFontNumber hf;
Static short hu[64];
Static long hyfchar;
Static ASCIICode curlang, initcurlang;
Static long lhyf, rhyf, initlhyf, initrhyf;
Static HalfWord hyfbchar;
/*:892*/
/*900:*/
Static char hyf[65];
Static Boolean initlig, initlft;
/*:900*/
/*905:*/
Static SmallNumber hyphenpassed;
/*:905*/
/*907:*/
Static HalfWord curl, curr;
Static Boolean ligaturepresent, lfthit, rthit;
/*:907*/
/*921:*/
Static TwoHalves trie[triesize + 1];
Static SmallNumber hyfdistance[trieopsize];
Static SmallNumber hyfnum[trieopsize];
Static QuarterWord hyfnext[trieopsize];
Static short opstart[256];
/*:921*/
/*926:*/
Static StrNumber hyphword[hyphsize + 1];
Static Pointer hyphlist[hyphsize + 1];
Static HyphPointer hyphcount;
/*:926*/


/// #943, 947, 950
#ifdef tt_INIT
/// #943
// trie op codes for quadruples
Static short trieophash[trieopsize + trieopsize + 1];
// largest opcode used so far for this language
Static QuarterWord trieused[256];
// language part of a hashed quadruple
Static ASCIICode trieoplang[trieopsize];
// opcode corresponding to a hashed quadruple
Static QuarterWord trieopval[trieopsize];
// number of stored ops so far
Static short trieopptr;

/// #947
// characters to match
Static PackedASCIICode triec[triesize + 1];
// operations to perform
Static QuarterWord trieo[triesize + 1];
// left subtrie links
Static TriePointer triel[triesize + 1];
// right subtrie links
Static TriePointer trier[triesize + 1];
// the number of nodes in the trie
Static TriePointer trieptr;
// used to identify equivalent subtries
Static TriePointer triehash[triesize + 1];

/// #950
// does a family start here?
Static UChar trietaken[(triesize + 7) / 8];
// the first possible slot for each character
Static TriePointer triemin[256];
// largest location used in trie
Static TriePointer triemax;
// is the trie still in linked form?
// xref: 891, [950], 951, 960, 966, 1324, 1325
Static Boolean trie_not_ready;
#endif // #943,947,950: tt_INIT


/*971:*/
Static Scaled bestheightplusdepth, pagemaxdepth, bestsize, lastkern;
/*:971*/
/*980:*/
Static Pointer pagetail, bestpagebreak, lastglue, mainp;
Static char pagecontents;
Static long leastpagecost;
/*:980*/
/*982:*/
Static Scaled pagesofar[8];
Static long lastpenalty, insertpenalties;
/*:982*/
/*989:*/
Static Boolean outputactive;
/*:989*/
/*1032:*/
Static InternalFontNumber mainf;
Static FourQuarters maini, mainj;
Static FontIndex maink;
Static long mains;
Static HalfWord bchar, falsebchar;
Static Boolean cancelboundary, insdisc;
/*:1032*/
/*1074:*/
Static Pointer curbox;
/*:1074*/
/*1266:*/
Static HalfWord aftertoken;
/*:1266*/
/*1281:*/
Static Boolean longhelpseen;
/*:1281*/
/*1299:*/
/*:1299*/
/*1305:*/
Static FILE *fmtfile = NULL;
/*:1305*/
/*1342:*/
Static FILE *write_file[16];
Static Boolean writeopen[18];
/*:1342*/
/*1345:*/
Static Pointer writeloc;   /*:1345*/


int niezgodnosc(int x) { return x; }

int get_defaulthyphenchar(void) { return defaulthyphenchar; }

int get_defaultskewchar(void) { return defaultskewchar; }

Pointer get_lomemmax(void) { return lomemmax; }

StrNumber fontidtext(InternalFontNumber x) { return text(fontidbase + x); }

void set_fontidtext(InternalFontNumber x, StrNumber t) {
    text(fontidbase + x) = t;
}

void set_help(int k, ...) {
    va_list ap;
    va_start(ap, k);
    helpptr = k;
    k--;
    while (k >= 0) {
        helpline[k] = va_arg(ap, int);
        k--;
    }
    va_end(ap);
}

Static int hex_to_i(int c, int cc) {
    int res = (c <= '9') ? (c - '0') : (c - 'a' + 10);
    res *= 16;
    res += (cc <= '9') ? (cc - '0') : (cc - 'a' + 10);
    return res;
}


Static void initialize(void) { /*:927*/
    /*19:*/
    long i;
    /*:19*/
    /*163:*/
    long k;
    /*:163*/
    /*927:*/
    HyphPointer z;

/// p5#8: Initialize whatever T E X might access

/// p11#21 Set initial values of key variables
{
    /// 21,  23,  24,  74,  77,   80,  97, 166, 215, 254,
    /// 257, 272, 287, 383, 439, 481, 490, 521, 551, 556,
    /// 593, 596, 606, 648, 662, 685, 771, 928, 990, 1033,
    /// 1267, 1282, 1300, 1343.
    /*23:*/
    for (i = 0; i <= 255; i++)
        xchr[i] = (Char)i;
    /*:23*/
    /*24:*/
    for (i = firsttextchar; i <= lasttextchar; i++)
        xord[(Char)i] = invalidcode;
    for (i = 128; i <= 255; i++)
        xord[xchr[i]] = i;
    for (i = 0; i <= 126; i++) /*:24*/
        xord[xchr[i]] = i;
    /*74:*/
    interaction = errorstopmode; /*:74*/
    /*77:*/
    deletionsallowed = true;
    setboxallowed = true;
    errorcount = 0; /*:77*/
    /*80:*/
    helpptr = 0;
    useerrhelp = false; /*:80*/
    /*97:*/
    interrupt = 0;
    OKtointerrupt = true; /*:97*/

    /// p#95: 166
#ifdef tt_DEBUG
    wasmemend = memmin;
    waslomax = memmin;
    washimin = memmax;
    panicking = false;
#endif // #166: tt_DEBUG

    /*215:*/
    nestptr = 0;
    maxneststack = 0;
    mode = vmode;
    head = contribhead;
    tail = contribhead;
    prevdepth = ignoredepth;
    modeline = 0;
    prevgraf = 0;
    shownmode = 0; /*991:*/
    pagecontents = empty;
    pagetail = pagehead;
    link(pagehead) = 0;
    lastglue = maxhalfword;
    lastpenalty = 0;
    lastkern = 0;
    pagedepth = 0;
    pagemaxdepth = 0; /*:991*/
    /*:215*/
    /*254:*/
    for (k = intbase; k <= eqtbsize; k++) {
        xeqlevel[k - intbase] = levelone;
    }
    /*:254*/
    /*257:*/

    next(hashbase) = 0;
    text(hashbase) = 0;
    for (k = hashbase + 1; k < undefinedcontrolsequence; k++) /*:257*/
        hash[k - hashbase] = hash[0];
    /*272:*/
    saveptr = 0;
    curlevel = levelone;
    curgroup = bottomlevel;
    curboundary = 0;
    maxsavestack = 0; /*:272*/
    /*287:*/
    magset = 0; /*:287*/
    /*383:*/
    topmark = 0;
    firstmark = 0;
    botmark = 0;
    splitfirstmark = 0;
    splitbotmark = 0; /*:383*/
    /*439:*/
    curval = 0;
    curvallevel = intval;
    radix = 0;
    curorder = 0; /*:439*/
    /*481:*/
    for (k = 0; k <= 16; k++) /*:481*/
        readopen[k] = closed;
    /*490:*/
    condptr = 0;
    iflimit = normal;
    curif = 0;
    ifline = 0; /*:490*/
    /*521:*/
    memcpy(TEXformatdefault, "TeXformats:plain.fmt", formatdefaultlength);
    /*:521*/
    /*551:*/
    for (int k = 0; k <= fontmax; k++) {
        fontused[k] = false;
    }
    /*:551*/
    /*556:*/
    nullcharacter.b0 = minquarterword;
    nullcharacter.b1 = minquarterword;
    nullcharacter.b2 = minquarterword;
    nullcharacter.b3 = minquarterword; /*:556*/
    /*593:*/
    totalpages = 0;
    maxv = 0;
    maxh = 0;
    maxpush = 0;
    doingleaders = false;
    deadcycles = 0;
    curs = -1; /*:593*/
    dviout_init();
    /*648:*/
    adjusttail = 0;
    lastbadness = 0; /*:648*/
    /*662:*/
    packbeginline = 0; /*:662*/
    /*685:*/
    emptyfield.rh = empty;
    emptyfield.UU.lh = 0;
    nulldelimiter.b0 = 0;
    nulldelimiter.b1 = minquarterword;
    nulldelimiter.b2 = 0;
    nulldelimiter.b3 = minquarterword; /*:685*/
    /*771:*/
    alignptr = 0;
    curalign = 0;
    curspan = 0;
    curloop = 0;
    curhead = 0;
    curtail = 0; /*:771*/
    /*928:*/
    for (z = 0; z <= hyphsize; z++) {
        hyphword[z] = 0;
        hyphlist[z] = 0;
    }
    hyphcount = 0; /*:928*/
    /*990:*/
    outputactive = false;
    insertpenalties = 0; /*:990*/
    /*1033:*/
    ligaturepresent = false;
    cancelboundary = false;
    lfthit = false;
    rthit = false;
    insdisc = false; /*:1033*/
    /*1267:*/
    aftertoken = 0; /*:1267*/
    /*1282:*/
    longhelpseen = false; /*:1282*/
    /*1300:*/
    formatident = 0; /*:1300*/
    /*1343:*/
    for (k = 0; k <= 17; k++) /*:1343*/
        writeopen[k] = false;
} // end block p11#21

/// p59#164: Initialize table entries (done by INITEX only)
#ifdef tt_INIT
    /// 164, 222, 228, 232, 240, 250, 258, 552, 946, 951, 1216, 1301, and 1369.
    for (k = membot + 1; k <= lomemstatmax; k++)
        mem[k - memmin].sc = 0; // all glue dimensions are zeroed

    k = membot;
    while (k <= lomemstatmax) {
        // set first words of glue specifications
        gluerefcount(k) = 1;
        stretchorder(k) = normal;
        shrinkorder(k) = normal;
        k += gluespecsize;
    }

    stretch(filglue)    =  UNITY; stretchorder(filglue)    = fil;
    stretch(fillglue)   =  UNITY; stretchorder(fillglue)   = fill;
    stretch(ssglue)     =  UNITY; stretchorder(ssglue)     = fil;
    shrink(ssglue)      =  UNITY; shrinkorder(ssglue)      = fil;
    stretch(filnegglue) = -UNITY; stretchorder(filnegglue) = fil;

    // now initialize the dynamic memory
    rover = lomemstatmax + 1;
    link(rover) = emptyflag;

    nodesize(rover) = 1000; // which is a 1000-word available node
    llink(rover) = rover;
    rlink(rover) = rover;
    lomemmax = rover + 1000;
    link(lomemmax) = 0;
    info(lomemmax) = 0;

    for (k = himemstatmin; k <= memtop; k++) {
        // clear list heads
        mem[k - memmin].sc = 0;
        type(k) = charnodetype;
    }

    // Initialize the special list heads and constant nodes
    {
        /// #790
        info(omittemplate) = endtemplatetoken; // ink(omit template) = null
        /// #797
        link(endspan) = maxquarterword + 1;
        info(endspan) = 0;
        /// #820
        type(lastactive) = hyphenated;
        linenumber(lastactive) = maxhalfword;
        subtype(lastactive) = 0; // the subtype is never examined by the algorithm
        /// #981
        subtype(pageinshead) = minquarterword + 255;
        type(pageinshead) = splitup;
        link(pageinshead) = pageinshead;
        /// #988
        type(pagehead) = gluenode;
        subtype(pagehead) = normal;
    }

    /// p59#164
    avail = 0;
    memend = memtop;
    himemmin = himemstatmin; // initialize the one-word memory
    varused = lomemstatmax - membot + 1;
    dynused = himemstatusage; // initialize statistics

    /// p82#222
    eqtype(undefinedcontrolsequence) = undefinedcs;
    equiv(undefinedcontrolsequence) = 0;
    eqlevel(undefinedcontrolsequence) = levelzero;
    for (k = activebase; k < undefinedcontrolsequence; k++)
        eqtb[k - activebase] = eqtb[undefinedcontrolsequence - activebase];

    /// #228
    equiv(gluebase) = zeroglue;
    eqlevel(gluebase) = levelone;
    eqtype(gluebase) = glueref;
    for (k = gluebase + 1; k < localbase; k++)
        eqtb[k - activebase] = eqtb[gluebase - activebase];
    gluerefcount(zeroglue) += localbase - gluebase;

    // #232
    parshapeptr = 0;
    eqtype(parshapeloc) = shaperef;
    eqlevel(parshapeloc) = levelone;
    for (k = outputroutineloc; k <= toksbase + 255; k++)
        eqtb[k - activebase] = eqtb[undefinedcontrolsequence - activebase];
    box(0) = 0;
    eqtype(boxbase) = boxref;
    eqlevel(boxbase) = levelone;
    for (k = boxbase + 1; k <= boxbase + 255; k++)
        eqtb[k - activebase] = eqtb[boxbase - activebase];
    curfont = nullfont;
    eqtype(curfontloc) = data;
    eqlevel(curfontloc) = levelone;
    for (k = mathfontbase; k <= mathfontbase + 47; k++)
        eqtb[k - activebase] = eqtb[curfontloc - activebase];
    equiv(catcodebase) = 0;
    eqtype(catcodebase) = data;
    eqlevel(catcodebase) = levelone;
    for (k = catcodebase + 1; k < intbase; k++)
        eqtb[k - activebase] = eqtb[catcodebase - activebase];
    for (k = 0; k <= 255; k++) {
        catcode(k) = otherchar;
        mathcode(k) = k;
        sfcode(k) = 1000;
    }
    catcode(carriagereturn) = carret;
    catcode(' ') = spacer;
    catcode('\\') = escape;
    catcode('%') = comment;
    catcode(invalidcode) = invalidchar;
    catcode(nullcode) = ignore;
    for (k = '0'; k <= '9'; k++)
        mathcode(k) = k + varcode;
    for (k = 'A'; k <= 'Z'; k++) {
        catcode(k) = letter;
        catcode(k + 'a' - 'A') = letter;
        mathcode(k) = k + varcode + 256;
        mathcode(k + 'a' - 'A') = k + 'a' - 'A' + varcode + 256;
        lccode(k) = k + 'a' - 'A';
        lccode(k + 'a' - 'A') = k + 'a' - 'A';
        uccode(k) = k;
        uccode(k + 'a' - 'A') = k;
        sfcode(k) = 999;
    }

    // #240
    for (k = intbase; k < delcodebase; k++)
        eqtb[k - activebase].int_ = 0;
    mag = 1000;
    tolerance = 10000;
    hangafter = 1;
    maxdeadcycles = 25;
    ESCAPE_CHAR = '\\';
    endlinechar = carriagereturn;
    for (k = 0; k <= 255; k++)
        delcode(k) = -1;
    delcode('.') = 0; // this null delimiter is used in error recovery

    // #250
    for (k = dimenbase; k <= eqtbsize; k++)
        eqtb[k - activebase].sc = 0;

    // #258
    hashused = frozencontrolsequence; // nothing is used
    cscount = 0;
    eqtype(frozendontexpand) = dontexpand;
    text(frozendontexpand) = S(257);

    // #552
    fonts_init();

    // #946
    for (k = -trieopsize; k <= trieopsize; k++)
        trieophash[k + trieopsize] = 0;
    for (k = 0; k <= 255; k++)
        trieused[k] = minquarterword;
    trieopptr = 0;

    // #951
    trie_not_ready = true;
    trieroot = 0;
    triec[0] = 0;
    trieptr = 0;

    // #1216
    text(frozenprotection) = S(258);
    // #1301
    formatident = S(259);
    // #1369
    text(endwrite) = S(260);
    eqlevel(endwrite) = levelone;
    eqtype(endwrite) = outercall;
    equiv(endwrite) = 0;


#endif // #164: tt_INIT
} // initialize


/*
    #54. On-line and off-line printing.

# Basic printing procedures:
    [57], [58], [59],   60,   [62],
    [63], [64], [65], [262], [263],
    [518], [699], 1355.
    println,  printchar,      print,               printnl,
    printesc, print_the_digs, print_int, print_cs, sprint_cs,
    print_file_name, print_size, print_write_whatsit
*/
/// #57
void println(void) {
    switch (selector) {
        case TERM_AND_LOG:
            putc('\n', stdout);
            putc('\n', log_file);
            term_offset = 0;
            file_offset = 0;
            break;

        case LOG_ONLY:
            putc('\n', log_file);
            file_offset = 0;
            break;

        case TERM_ONLY:
            putc('\n', stdout);
            term_offset = 0;
            break;

        case NO_PRINT:
        case PSEUDO:
        case NEW_STRING:
            /* blank case */
            break;

        default:
            putc('\n', write_file[selector]);
            break;
    } // switch (selector)
} // #57: println

/// #58
void print_char(ASCIICode s) {
    if (s == newlinechar) {
        if (selector < PSEUDO) {
            println();
            return;
        }
    }

    switch (selector) {
        case TERM_AND_LOG:
            fwrite(&xchr[s], 1, 1, stdout);
            fwrite(&xchr[s], 1, 1, log_file);
            term_offset++;
            file_offset++;
            if (term_offset == MAX_PRINT_LINE) {
                putc('\n', stdout);
                term_offset = 0;
            }
            if (file_offset == MAX_PRINT_LINE) {
                putc('\n', log_file);
                file_offset = 0;
            }
            break;

        case LOG_ONLY:
            fwrite(&xchr[s], 1, 1, log_file);
            file_offset++;
            if (file_offset == MAX_PRINT_LINE) println();
            break;

        case TERM_ONLY:
            fwrite(&xchr[s], 1, 1, stdout);
            term_offset++;
            if (term_offset == MAX_PRINT_LINE) println();
            break;

        case NO_PRINT:
            /* blank case */
            break;

        case PSEUDO:
            if (tally < trickcount) {
                trickbuf[tally % ERROR_LINE] = s;
            }
            break;

        case NEW_STRING:
            append_char(s);
            break;

        default:
            fwrite(&xchr[s], 1, 1, write_file[selector]);
            break;
    }
    tally++;
} // #58: print_char

/// p26#59: prints string s
void print(StrNumber s) {
    long nl; // new-line character to restore

    if (0 <= s && s <= 255) {
        if (selector > PSEUDO) {
            print_char(s); // internal strings are not expanded
            return;
        }
        if (s == newlinechar) { /// #244
            if (selector < PSEUDO) {
                println();
                return;
            }
        }

        // temporarily disable new-line character
        nl = newlinechar;
        newlinechar = -1;
        str_print(s);
        newlinechar = nl;
        return;
    } else {
        // this can’t happen
        str_print(s);
    } // if (0 <= s && s <= 255) - else
} // #59: print

/// #62: prints string s at beginning of line
void printnl(StrNumber s) {
    if ((term_offset > 0 && (selector & 1)) ||
        (file_offset > 0 && selector >= LOG_ONLY))
        println();
    print(s);
} // #62: printnl

/// #63: prints escape character
void print_esc(StrNumber s) {
    long c;

    c = ESCAPE_CHAR;
    if (0 <= c && c <= 255) {
        print(c);
    }
    slow_print(s);
} // #63: print_esc

/// #64: prints dig[k − 1]...dig[0]
Static void print_the_digs(EightBits k, char dig[]) {
    while (k > 0) {
        k--;
        if (dig[k] < 10) {
            print_char('0' + dig[k]);
        } else {
            print_char('A' + dig[k] - 10);
        }
    } // while (k > 0)
} // #64: print_the_digs

/// #65: prints an integer in decimal form
void print_int(long n) {
    int k = 0; // index to current digit; we assume that n < 10^23
    long m; // used to negate n in possibly dangerous cases
    char dig[23];

    if (n < 0) {
        print_char('-');
        if (n > -100000000L) {
            n = -n;
        } else {
            m = -n - 1;
            n = m / 10;
            m = (m % 10) + 1;
            k = 1;
            if (m < 10) {
                dig[0] = m;
            } else {
                dig[0] = 0;
                n++;
            }
        } // if (n > -100000000L) - else
    } // if (n < 0)

    do {
        dig[k] = n % 10;
        n /= 10;
        k++;
    } while (n != 0);
    print_the_digs(k, dig);
} // #65: print_int

/// #262: prints a purported control sequence
Static void print_cs(long p) {
    if (p < hashbase) {
        if (p >= singlebase) {
            if (p == nullcs) {
                print_esc(S(262));
                print_esc(S(263));
                return;
            }
            print_esc(p - singlebase);
            if (catcode(p - singlebase) == letter) print_char(' ');
            return;
        }
        if (p < activebase)
            print_esc(S(264));
        else
            print(p - activebase);
        return;
    }
    if (p >= undefinedcontrolsequence) {
        print_esc(S(264));
        return;
    }
    if (!str_valid(text(p)))
        print_esc(S(265));
    else {
        print_esc(text(p));
        print_char(' ');
    }
} // #262: print_cs

/// #263: prints a control sequence
void sprint_cs(HalfWord p) {
    if (p >= hashbase) {
        print_esc(text(p));
        return;
    }
    if (p < singlebase) {
        print(p - activebase);
        return;
    }
    if (p < nullcs)
        print_esc(p - singlebase);
    else {
        print_esc(S(262));
        print_esc(S(263));
    }
} // #263: sprint_cs

/// #518
void print_file_name(StrNumber n, StrNumber a, StrNumber e) {
    slow_print(a);
    slow_print(n);
    slow_print(e);
} // #518: print_file_name

/// #699
void print_size(long s) {
    if (s == TEXT_SIZE) {
        print_esc(S(266));
        return;
    }
    if (s == SCRIPT_SIZE)
        print_esc(S(267));
    else
        print_esc(S(268));
} // #699: print_size

/// #1355
Static void print_write_whatsit(StrNumber s, HalfWord p) {
    print_esc(s);
    if (writestream(p) < 16) {
        print_int(writestream(p));
    } else if (writestream(p) == 16) {
        print_char('*');
    } else {
        print_char('-');
    }
} // #1355: print_write_whatsit


/// p31#78
Static void normalize_selector(void);
Static void gettoken(void);
Static void term_input(void);
Static void showcontext(void);
Static void beginfilereading(void);
Static void openlogfile(void);
Static void closefilesandterminate(void);
Static void clearforerrorprompt(void);
Static void giveerrhelp(void);

#ifdef tt_DEBUG
Static void debughelp(void);
#endif // 78: tt_DEBUG
Static jmp_buf _JLendofTEX;
/*:78*/

/*81:*/
Static void jumpout(void) { longjmp(_JLendofTEX, 1); }
/*:81*/

/*82:*/
void error(void) {
    if (history < ERROR_MESSAGE_ISSUED) history = ERROR_MESSAGE_ISSUED;
    print_char('.');
    showcontext();
    if (interaction == errorstopmode) { /*83:*/
        while (true) {                  /*:83*/
            ASCIICode c;
_Llabcontinue:
            clearforerrorprompt();
            print(S(269));
            term_input();
            if (last == first) goto _Lexit;
            c = buffer[first];
            /*84:*/
            if (c >= 'a') c += 'A' - 'a';
            switch (c) {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    if (deletionsallowed) { /*88:*/
                        long s1 = curtok;
                        long s2 = curcmd;
                        long s3 = curchr;
                        long s4 = alignstate;
                        alignstate = 1000000L;
                        OKtointerrupt = false;
                        if ((last > first + 1) & (buffer[first + 1] >= '0') &
                            (buffer[first + 1] <= '9'))
                            c = c * 10 + buffer[first + 1] - '0' * 11;
                        else
                            c -= '0';
                        while (c > 0) {
                            gettoken();
                            c--;
                        }
                        curtok = s1;
                        curcmd = s2;
                        curchr = s3;
                        alignstate = s4;
                        OKtointerrupt = true;
                        help2(S(270), S(271));
                        showcontext();
                        goto _Llabcontinue;
                    }
                    /*:88*/
                    break;

                case 'D':
#ifdef tt_DEBUG
                    debughelp();
                    goto _Llabcontinue;
#endif // #84: tt_DEBUG
                    break;

                case 'E':
                    if (baseptr > 0) {
                        printnl(S(272));
                        slow_print(inputstack[baseptr].namefield);
                        print(S(273));
                        print_int(line);
                        interaction = scrollmode;
                        jumpout();
                    }
                    break;

                case 'H': /*89:*/
                    if (useerrhelp) {
                        giveerrhelp();
                        useerrhelp = false;
                    } else {
                        if (helpptr == 0) {
                            help2(S(274), S(275));
                        }
                        do {
                            helpptr--;
                            print(helpline[helpptr]);
                            println();
                        } while (helpptr != 0);
                    }
                    help4(S(276), S(275), S(277), S(278));
                    goto _Llabcontinue;
                    break;
                    /*:89*/

                case 'I': /*87:*/
                    beginfilereading();
                    if (last > first + 1) {
                        loc = first + 1;
                        buffer[first] = ' ';
                    } else {
                        print(S(279));
                        term_input();
                        loc = first;
                    }
                    first = last;
                    curinput.limitfield = last - 1;
                    goto _Lexit;
                    break;
                    /*:87*/

                case 'Q':
                case 'R':
                case 'S': /*86:*/
                    errorcount = 0;
                    interaction = batchmode + c - 'Q';
                    print(S(280));
                    switch (c) {
                        case 'Q':
                            print_esc(S(281));
                            selector--;
                            break;

                        case 'R':
                            print_esc(S(282));
                            break;

                        case 'S':
                            print_esc(S(283));
                            break;
                    }
                    print(S(284));
                    println();
                    fflush(stdout);
                    goto _Lexit;
                    break;
                    /*:86*/

                case 'X':
                    interaction = scrollmode;
                    jumpout();
                    break;
            } /*85:*/
            print(S(285));
            printnl(S(286));
            printnl(S(287));
            if (baseptr > 0) print(S(288));
            if (deletionsallowed) printnl(S(289));
            printnl(S(290)); /*:85*/
            /*:84*/
        }
    }
    errorcount++;
    if (errorcount == 100) {
        printnl(S(291));
        history = FATAL_ERROR_STOP;
        jumpout();
    } /*90:*/
    if (interaction > batchmode) selector--;
    if (useerrhelp) {
        println();
        giveerrhelp();
    } else {
        while (helpptr > 0) {
            helpptr--;
            printnl(helpline[helpptr]);
        }
    }
    println();
    if (interaction > batchmode) /*:90*/
        selector++;
    println();
_Lexit:;
}
/*:82*/

/// p36#93: Error handling procedures
/// xref: 93, 94, 95, 1304
Static void succumb(void) {
    if (interaction == errorstopmode) interaction = scrollmode;
    if (logopened) error();
#ifdef tt_DEBUG
    if (interaction > batchmode) debughelp();
#endif // #93: tt_DEBUG
    history = FATAL_ERROR_STOP;
    jumpout();
}

/// p36#93: 
Static void fatalerror(StrNumber s) {
    normalize_selector();
    printnl(S(292));
    print(S(293));
    helpptr = 1;
    helpline[0] = s;
    succumb();
}


/*94:*/
void overflow(StrNumber s, long n) {
    normalize_selector();
    printnl(S(292));
    print(S(294));
    print(s);
    print_char('=');
    print_int(n);
    print_char(']');
    help2(S(295), S(296));
    succumb();
}
/*:94*/


/*95:*/
Static void confusion(StrNumber s) {
    normalize_selector();
    if (history < ERROR_MESSAGE_ISSUED) {
        printnl(S(292));
        print(S(297));
        print(s);
        print_char(')');
        help1(S(298));
    } else {
        printnl(S(292));
        print(S(299));
        help2(S(300), S(301));
    }
    succumb();
}
/*:95*/


/*28:*/
Static void aclose(FILE **f)
{
  if (*f != NULL)
    fclose(*f);
  *f = NULL;
}

Static void wclose(FILE **f)
{
  if (*f != NULL)
    fclose(*f);
  *f = NULL;
}
/*:28*/


/*37:*/
Static Boolean initterminal(void)
{
    // topenin();
  if (initinc(1)) {
    loc = first;
    return true;
  }
  while (true) {
    fprintf(stdout, "**");
    fflush(stdout);
    if (!inputln(stdin, true)) {
      putc('\n', stdout);
      fprintf(stdout,"! End of file on the terminal... why?");
      return false;
    }
    loc = first;
    while ((loc < last) & (buffer[loc] == ' '))
      loc++;
    if (loc < last) {
      return true;
    }
    fprintf(stdout, "Please type the name of your input file.\n");
  }
}
/*:37*/


/// #66
Static void print_two(long n) {
    n = labs(n) % 100;
    print_char('0' + n / 10);
    print_char('0' + n % 10);
}

/// #67
void print_hex(long n) {
    int k;
    char digs[23];

    k = 0;
    print_char('"');
    do {
        digs[k] = n & 15;
        n /= 16;
        k++;
    } while (n != 0);
    print_the_digs(k, digs);
}

/// #69
Static void print_roman_int(long n) {
    int j, k;
    NonNegativeInteger u, v;
    static char romstr[] = "m2d5c2l5x2v5i";

    j = 0;
    v = 1000;
    while (true) {
        while (n >= v) {
            print_char(romstr[j]);
            n -= v;
        }
        if (n <= 0) return;
        k = j + 2;
        u = v / (romstr[k - 1] - '0');
        if (romstr[k - 1] == '2') {
            k += 2;
            u /= romstr[k - 1] - '0';
        }
        if (n + u >= v) {
            print_char(romstr[k]);
            n += u;
        } else {
            j += 2;
            v /= romstr[j - 1] - '0';
        }
    }
}

/// #71: gets a line from the terminal
Static void term_input(void) {
    short k;

    fflush(stdout);
    if (!inputln(stdin, true)) fatalerror(S(302));
    term_offset = 0;
    selector--;
    if (last != first) {
        for (k = first; k < last; k++)
            print(buffer[k]);
    }
    println();
    selector++;
}


/*
    #72~98. Reporting errors.
*/
/// #91
Static void int_error(long n) {
    print(S(303));
    print_int(n);
    print_char(')');
    error();
}
/// #92
Static void normalize_selector(void) {
    if (logopened)
        selector = TERM_AND_LOG;
    else
        selector = TERM_ONLY;
    if (jobname == 0) openlogfile();
    if (interaction == batchmode) selector--;
}
/// #98
Static void pause_for_instructions(void) {
    if (!OKtointerrupt) return;
    interaction = errorstopmode;
    if (selector == LOG_ONLY || selector == NO_PRINT) selector++;
    printnl(S(292));
    print(S(304));
    help3(S(305), S(306), S(307));
    deletionsallowed = false;
    error();
    deletionsallowed = true;
    interrupt = 0;
}

/*
    #99: Arithmetic with scaled dimensions.

export:
    + void print_scaled(Scaled s)
    + Scaled xn_over_d(Scaled x, long n, long d)
others:
    - Static long half(Scaled x)
    - Static Scaled round_decimals(int k, char digs[])
    - Static Scaled mult_and_add(long n, Scaled x, Scaled y, Scaled max_ans)
    - Static Scaled x_over_n(Scaled x, Scaled n)
    - Static HalfWord badness(Scaled t, Scaled s)

*/
/// #100
Static long half(Scaled x) {
    if (x & 1)
        return ((x + 1) / 2);
    else
        return (x / 2);
} // #100: half

/// #102
Static Scaled round_decimals(int k, char digs[]) {
    Scaled a = 0;

    while (k > 0) {
        k--;
        a = (a + digs[k] * TWO) / 10;
    }
    return ((a + 1) / 2);
} // #102: round_decimals

/// #103: prints scaled real, rounded to five digits
void print_scaled(Scaled s) {
    Scaled delta;

    if (s < 0) {
        // print the sign, if negative
        print_char('-');
        s = -s;
    }
    print_int(s / UNITY); // print the integer part
    print_char('.');
    // s = (s & (UNITY - 1)) * 10 + 5;
    s = 10 * (s % UNITY) + 5;
    delta = 10;
    do {
        if (delta > UNITY) s -= 17232; // 2^15 - 50000, round the last digit
        print_char('0' + s / UNITY);
        // s = (s & (UNITY - 1)) * 10;
        s = 10 * (s % UNITY) + 5;
        delta *= 10;
    } while (s > delta);
} // #103: print_scaled

/// #105: return `nx + y`
Static Scaled mult_and_add(long n, Scaled x, Scaled y, Scaled max_ans) {
    if (n < 0) {
        x = -x;
        n = -n;
    }
    if (n == 0) {
        return y;
    } else { // n > 0
        if (x <= (max_ans - y) / n && -x <= (max_ans + y) / n) {
            return (n * x + y);
        } else {
            arith_error = true;
            return 0;
        }
    } // if (n <> 0)
} // #105: mult_and_add

/// #106: `x / n`
Static Scaled x_over_n(Scaled x, Scaled n) {
    Scaled result;
    Boolean negative = false;

    if (n == 0) {
        // 除 0 错误
        arith_error = true;
        result = 0;
        tex_remainder = x;
    } else {
        if (n < 0) {
            x = -x;
            n = -n;
            negative = true;
        }
        // n > 0
        if (x >= 0) {
            result = x / n;
            tex_remainder = x % n;
        } else {
            result = -(-x / n);
            tex_remainder = -(-x % n);
        }
    }
    if (negative) tex_remainder = -tex_remainder;
    return result;
} // #106: x_over_n

/// #107: `x * (n / d)`
Scaled xn_over_d(Scaled x, long n, long d) {
    Scaled result;
    Boolean positive; // was x >= 0?
    NonNegativeInteger t, u, v;

    if (x >= 0) {
        positive = true;
    } else {
        x = -x;
        positive = false;
    }
    t = (x % 32768L) * n;
    u = (x / 32768L) * n + (t / 32768L);
    v = (u % d) * 32768L + (t % 32768L);
    if ((u / d) >= 32768L) {
        arith_error = true;
    } else {
        u = (u / d) * 32768L + (v / d);
    }
    if (positive) {
        result = u;
        tex_remainder = (v % d);
    } else {
        result = -u;
        tex_remainder = -(v % d);
    }
    return result;
} // #107: xn_over_d

/// #108: compute the “badness” of glue, , given t >= 0
// when a total t is supposed to be made from amounts that sum to s
// badness = 100 * (t/s)^3
// badness(t + 1,s) ≥ badness(t,s) ≥ badness(t,s + 1)
Static HalfWord badness(Scaled t, Scaled s) {
    Scaled r; // approximation to α*t/s, where α^3 ≈ 100 * 2^18

    if (t == 0) {
        return 0;
    } else if (s <= 0) {
        return INF_BAD;
    } else {
        if (t <= 7230584L) {
            r = t * 297 / s; // 297^3 = 99.94 * 2^18
        } else if (s >= 1663497L) {
            r = t / (s / 297);
        } else {
            r = t;
        }
        if (r > 1290) { // 1290^3 < 2^31 < 1291^3
            return INF_BAD;
        } else {
            return ((r * r * r + 131072L) / 262144L);
        }
    }
} // #108:badness


/// p43#114
#ifdef tt_DEBUG
Static void printword(MemoryWord w) {
    print_int(w.int_);
    print_char(' ');
    print_scaled(w.sc);
    print_char(' ');
    print_scaled( (long)floor(UNITY * w.gr + 0.5) );
    println();
    print_int(w.hh.UU.lh);
    print_char('=');
    print_int(w.hh.UU.U2.b0);
    print_char(':');
    print_int(w.hh.UU.U2.b1);
    print_char(';');
    print_int(w.hh.rh);
    print_char(' ');
    print_int(w.qqqq.b0);
    print_char(':');
    print_int(w.qqqq.b1);
    print_char(':');
    print_int(w.qqqq.b2);
    print_char(':');
    print_int(w.qqqq.b3);
}
#endif // #114: tt_DEBUG

/*119:*/
/*292:*/
Static void showtokenlist(long p, long q, long l)
{
  long m, c;
  ASCIICode matchchr, n;

  matchchr = '#';
  n = '0';
  tally = 0;
  while (p != 0 && tally < l) {
    if (p == q) {   /*320:*/
      settrickcount();
    }
    /*:320*/
    /*293:*/
    if (p < himemmin || p > memend) {
      print_esc(S(308));
      goto _Lexit;
    }
    if (info(p) >= cstokenflag) {
      print_cs(info(p) - cstokenflag);
    } else {   /*:293*/
      m = info(p) / dwa_do_8;
      c = (info(p)) & (dwa_do_8-1);
      if (info(p) < 0) {
	print_esc(S(309));
      } else {  /*294:*/
	switch (m) {   /*:294*/

	case leftbrace:
	case rightbrace:
	case mathshift:
	case tabmark:
	case supmark:
	case submark:
	case spacer:
	case letter:
	case otherchar:
	  print(c);
	  break;

	case macparam:
	  print(c);
	  print(c);
	  break;

	case outparam:
	  print(matchchr);
	  if (c > 9) {
	    print_char('!');
	    goto _Lexit;
	  }
	  print_char(c + '0');
	  break;

	case match:
	  matchchr = c;
	  print(c);
	  n++;
	  print_char(n);
	  if (n > '9')
	    goto _Lexit;
	  break;

	case endmatch:
	  print(S(310));
	  break;

	default:
	  print_esc(S(309));
	  break;
	}
      }
    }
    p = link(p);
  }
  if (p != 0)
    print_esc(S(311));
_Lexit: ;
}
/*:292*/

/*306:*/
Static void runaway(void)
{
  Pointer p=0 /* XXXX */;

  if (scannerstatus <= skipping)
    return;
  printnl(S(312));
  switch (scannerstatus) {

  case defining:
    print(S(313));
    p = defref;
    break;

  case matching:
    print(S(314));
    p = temphead;
    break;

  case aligning:
    print(S(315));
    p = holdhead;
    break;

  case absorbing:
    print(S(316));
    p = defref;
    break;
  }
  print_char('?');
  println();
  showtokenlist(link(p), 0, ERROR_LINE - 10);
}
/*:306*/
/*:119*/

/// p46#120: single-word node allocation
Static Pointer get_avail(void) {
    Pointer p;

    p = avail;
    if (p != 0)
        avail = link(avail);
    else if (memend + charnodesize <= memmax) {
        p = memend + 1;
        memend += charnodesize;
    } else {
        p = himemmin - 1;
        himemmin -= charnodesize;
        if (himemmin <= lomemmax) {
            runaway();
            overflow(S(317), memmax - memmin + 1);
        }
    }
    set_as_char_node(p);
    link(p) = 0;
    #ifdef tt_STAT
        dynused += charnodesize;
    #endif // #120: tt_STAT
    return p;
} // #120: get_avail

/// p46#123: makes list of single-word nodes available
Static void flushlist(HalfWord p) {
    Pointer q, r;

    if (p == 0) return;
    r = p;
    do {
        q = r;
        r = link(r);            
        #ifdef tt_STAT
            dynused -= charnodesize;
        #endif // #123: tt_STAT
    } while (r != 0);
    link(q) = avail;
    avail = p;
} // #123: flushlist


/// #125
Static HalfWord getnode(long s) {
    HalfWord Result;
    Pointer p, q;
    long r, t;

_Lrestart:
    p = rover;
    do { /*127:*/
        q = p + nodesize(p);
        while (isempty(q)) {
            t = rlink(q);
            if (q == rover) rover = t;
            llink(t) = llink(q);
            rlink(llink(q)) = t;
            q += nodesize(q);
        }
        r = q - s;
        if (r > p + 1) { /*128:*/
            nodesize(p) = r - p;
            rover = p;
            goto _Lfound;
        }
        /*:128*/
        if (r == p) {
            if (rlink(p) != p) { /*129:*/
                rover = rlink(p);
                t = llink(p);
                llink(rover) = t;
                rlink(t) = rover;
                goto _Lfound;
            }
            /*:129*/
        }
        nodesize(p) = q - p; /*:127*/
        p = rlink(p);
    } while (p != rover);
    if (s == 1073741824L) {
        Result = maxhalfword;
        goto _Lexit;
    }
    if (lomemmax + 2 < himemmin) {
        if (lomemmax + 2 <= membot + maxhalfword) { /*126:*/
            if (himemmin - lomemmax >= 1998)
                t = lomemmax + 1000;
            else
                t = lomemmax + (himemmin - lomemmax) / 2 + 1;
            p = llink(rover);
            q = lomemmax;
            rlink(p) = q;
            llink(rover) = q;
            if (t > membot + maxhalfword) t = membot + maxhalfword;
            rlink(q) = rover;
            llink(q) = p;
            link(q) = emptyflag;
            nodesize(q) = t - lomemmax;
            lomemmax = t;
            link(lomemmax) = 0;
            info(lomemmax) = 0;
            rover = q;
            goto _Lrestart;
        }
        /*:126*/
    }
    overflow(S(317), memmax - memmin + 1);
_Lfound:
    link(r) = 0;
    #ifdef tt_STAT
        varused += s;
    #endif // #125: tt_STAT
    Result = r;
_Lexit:
    return Result;
} // #125: getnode

/// p48#130: variable-size node liberation
Static void freenode(Pointer p, HalfWord s) {
    Pointer q;

    nodesize(p) = s;
    link(p) = emptyflag;
    q = llink(rover);
    llink(p) = q;
    rlink(p) = rover;
    llink(rover) = p;
    rlink(q) = p;
    #ifdef tt_STAT
        varused -= s;
    #endif // #130: tt_STAT
} // #130: freenode


#ifdef tt_INIT // #131,132
/// p49#131: sorts the available variable-size nodes by location
Static void sort_avail(void)
{   // used at #1311
    Pointer p, q, r,  // indices into mem
            oldrover; // initial rover setting

    p = getnode(1073741824L); // merge adjacent free areas
    p = rlink(rover);
    rlink(rover) = maxhalfword;
    oldrover = rover;

    while (p != oldrover) {
        /// #132: Sort p into the list starting at rover
        /// and advance p to rlink(p)
        if (p < rover) {
            q = p;
            p = rlink(q);
            rlink(q) = rover;
            rover = q;
        } else {
            q = rover;
            while (rlink(q) < p)
                q = rlink(q);
            r = rlink(p);
            rlink(p) = rlink(q);
            rlink(q) = p;
            p = r;
        } // if (p < rover) - else
    } // while (p != oldrover)

    p = rover;
    while (rlink(p) != maxhalfword) {
        llink(rlink(p)) = p;
        p = rlink(p);
    } // while (rlink(p) != maxhalfword)
    rlink(p) = rover;
    llink(rover) = p;
} // sort_avail
#endif // #131: tt_INIT


/*136:*/
Static Pointer newnullbox(void)
{
  Pointer p;

  p = getnode(boxnodesize);
  type(p) = hlistnode;
  subtype(p) = minquarterword;
  width(p) = 0;
  depth(p) = 0;
  height(p) = 0;
  shiftamount(p) = 0;
  listptr(p) = 0;
  gluesign(p) = normal;
  glueorder(p) = normal;
  glueset(p) = 0.0;
  return p;
}
/*:136*/

/*139:*/
Static Pointer newrule(void)
{
  Pointer p;

  p = getnode(rulenodesize);
  type(p) = rulenode;
  subtype(p) = 0;
  width(p) = nullflag;
  depth(p) = nullflag;
  height(p) = nullflag;
  return p;
}
/*:139*/

/*144:*/
Static Pointer newligature(QuarterWord f, QuarterWord c, Pointer q)
{
  Pointer p;

  p = getnode(smallnodesize);
  type(p) = ligaturenode;
  font_ligchar(p) = f;
  character_ligchar(p) = c;
  ligptr(p) = q;
  subtype(p) = 0;
  return p;
}


Static Pointer newligitem(QuarterWord c)
{
  Pointer p;

  p = getnode(smallnodesize);
  character(p) = c;
  ligptr(p) = 0;
  return p;
}
/*:144*/

/*145:*/
Static Pointer newdisc(void)
{
  Pointer p;

  p = getnode(smallnodesize);
  type(p) = discnode;
  replacecount(p) = 0;
  prebreak(p) = 0;
  postbreak(p) = 0;
  return p;
}
/*:145*/

/*147:*/
Static Pointer newmath(long w, SmallNumber s)
{
  Pointer p;

  p = getnode(smallnodesize);
  type(p) = mathnode;
  subtype(p) = s;
  width(p) = w;
  return p;
}
/*:147*/

/*151:*/
Static Pointer newspec(Pointer p)
{
  Pointer q;

  q = getnode(gluespecsize);
  mem[q - memmin] = mem[p - memmin];
  gluerefcount(q) = 0;
  width(q) = width(p);
  stretch(q) = stretch(p);
  shrink(q) = shrink(p);
  return q;
}
/*:151*/

/*152:*/
Static Pointer newparamglue(SmallNumber n)
{
  Pointer p, q;

  p = getnode(smallnodesize);
  type(p) = gluenode;
  subtype(p) = n + 1;
  leaderptr(p) = 0;
  q = gluepar(n);   /*224:*/
  /*:224*/
  glueptr(p) = q;
  (gluerefcount(q))++;
  return p;
}
/*:152*/

/*153:*/
Static Pointer newglue(Pointer q)
{
  Pointer p;

  p = getnode(smallnodesize);
  type(p) = gluenode;
  subtype(p) = normal;
  leaderptr(p) = 0;
  glueptr(p) = q;
  (gluerefcount(q))++;
  return p;
}
/*:153*/

/*154:*/
Static Pointer newskipparam(SmallNumber n)
{
  Pointer p;

  tempptr = newspec(gluepar(n));   /*224:*/
  /*:224*/
  p = newglue(tempptr);
  gluerefcount(tempptr) = 0;
  subtype(p) = n + 1;
  return p;
}
/*:154*/

/*156:*/
Static Pointer newkern(long w)
{
  Pointer p;

  p = getnode(smallnodesize);
  type(p) = kernnode;
  subtype(p) = normal;
  width(p) = w;
  return p;
}
/*:156*/

/*158:*/
Static Pointer newpenalty(long m)
{
  Pointer p;

  p = getnode(smallnodesize);
  type(p) = penaltynode;
  subtype(p) = 0;
  penalty(p) = m;
  return p;
}
/*:158*/

/// p60#167
#ifdef tt_DEBUG
Static void checkmem(Boolean printlocs) {
    Pointer p, q;
    Boolean clobbered;
    HalfWord FORLIM;

    for (p = memmin; p <= lomemmax; p++)
        P_clrbits_B(free_, p - memmin, 0, 3);
    for (p = himemmin; p <= memend; p++) /*168:*/
        P_clrbits_B(free_, p - memmin, 0, 3);
    p = avail;
    q = 0;
    clobbered = false;
    while (p != 0) {
        if (p > memend || p < himemmin)
            clobbered = true;
        else {
            if (P_getbits_UB(free_, p - memmin, 0, 3)) clobbered = true;
        }
        if (clobbered) {
            printnl(S(318));
            print_int(q);
            goto _Ldone1;
        }
        P_putbits_UB(free_, p - memmin, 1, 0, 3);
        q = p;
        p = link(q);
    }

_Ldone1: /*:168*/
    /*169:*/
    p = rover;
    q = 0;
    clobbered = false;
    do {
        if (p >= lomemmax || p < memmin)
            clobbered = true;
        else if ((rlink(p) >= lomemmax) | (rlink(p) < memmin))
            clobbered = true;
        else if ((!isempty(p)) | (nodesize(p) < 2) |
                 (p + nodesize(p) > lomemmax) | (llink(rlink(p)) != p)) {
            clobbered = true;
        }
        if (clobbered) {
            printnl(S(319));
            print_int(q);
            goto _Ldone2;
        }
        FORLIM = p + nodesize(p);
        for (q = p; q < FORLIM; q++) {
            if (P_getbits_UB(free_, q - memmin, 0, 3)) {
                printnl(S(320));
                print_int(q);
                goto _Ldone2;
            }
            P_putbits_UB(free_, q - memmin, 1, 0, 3);
        }
        q = p;
        p = rlink(p);
    } while (p != rover);

_Ldone2: /*:169*/
    /*170:*/
    p = memmin;
    while (p <= lomemmax) { /*:170*/
        if (isempty(p)) {
            printnl(S(321));
            print_int(p);
        }
        while ((p <= lomemmax) & (!P_getbits_UB(free_, p - memmin, 0, 3)))
            p++;
        while ((p <= lomemmax) &   P_getbits_UB(free_, p - memmin, 0, 3))
            p++;
    }
    if (printlocs) { /*171:*/
        printnl(S(322));
        FORLIM = lomemmax;
        for (p = memmin; p <= lomemmax; p++) {
            if ((!P_getbits_UB(free_, p - memmin, 0, 3)) &
                ((p > waslomax) | P_getbits_UB(wasfree, p - memmin, 0, 3))) {
                print_char(' ');
                print_int(p);
            }
        }
        for (p = himemmin; p <= memend; p++) {
            if ((!P_getbits_UB(free_, p - memmin, 0, 3)) &
                ((p < washimin || p > wasmemend) |
                 P_getbits_UB(wasfree, p - memmin, 0, 3))) {
                print_char(' ');
                print_int(p);
            }
        }
    }
    /*:171*/
    for (p = memmin; p <= lomemmax; p++) {
        P_clrbits_B(wasfree, p - memmin, 0, 3);
        P_putbits_UB(wasfree,
            p - memmin, P_getbits_UB(free_, p - memmin, 0, 3), 0, 3);
    }
    for (p = himemmin; p <= memend; p++) {
        P_clrbits_B(wasfree, p - memmin, 0, 3);
        P_putbits_UB(wasfree, 
            p - memmin, P_getbits_UB(free_, p - memmin, 0, 3), 0, 3);
    }
    wasmemend = memend;
    waslomax = lomemmax;
    washimin = himemmin;
} // #164: checkmem

/// p61#172
Static void searchmem(Pointer p) {
    long q;

    for (q = memmin; q <= lomemmax; q++) {
        if (link(q) == p) {
            printnl(S(323));
            print_int(q);
            print_char(')');
        }
        if (info(q) == p) {
            printnl(S(324));
            print_int(q);
            print_char(')');
        }
    }
    for (q = himemmin; q <= memend; q++) {
        if (link(q) == p) {
            printnl(S(323));
            print_int(q);
            print_char(')');
        }
        if (info(q) == p) {
            printnl(S(324));
            print_int(q);
            print_char(')');
        }
    }                                               /*255:*/
    for (q = activebase; q <= boxbase + 255; q++) { /*:255*/
        if (equiv(q) == p) {
            printnl(S(325));
            print_int(q);
            print_char(')');
        }
    }
    /*285:*/
    if (saveptr > 0) {                  /*933:*/
        for (q = 0; q < saveptr; q++) { /*:285*/
            if (equivfield(savestack[q]) == p) {
                printnl(S(326));
                print_int(q);
                print_char(')');
            }
        }
    }
    for (q = 0; q <= hyphsize; q++) { /*:933*/
        if (hyphlist[q] == p) {
            printnl(S(327));
            print_int(q);
            print_char(')');
        }
    }
} // #172: searchmem
#endif // #167,172: tt_DEBUG

/*174:*/
Static void shortdisplay(Pointer p)
{
  long n;

  while (p > memmin) {
    if (ischarnode(p)) {
      if (p <= memend) {
	if (font(p) != fontinshortdisplay) {
	  if ( /* (font(p) < 0 ) | */ (font(p) > fontmax))
	    print_char('*');
	  else  /*267:*/
		/*:267*/
		  print_esc(fontidtext(font(p)));
	  print_char(' ');
	  fontinshortdisplay = font(p);
	}
	print(character(p) - minquarterword);
      }
    } else {  /*175:*/
      switch (type(p)) {   /*:175*/

      case hlistnode:
      case vlistnode:
      case insnode:
      case whatsitnode:
      case marknode:
      case adjustnode:
      case unsetnode:
	print(S(328));
	break;

      case rulenode:
	print_char('|');
	break;

      case gluenode:
	if (glueptr(p) != zeroglue)
	  print_char(' ');
	break;

      case mathnode:
	print_char('$');
	break;

      case ligaturenode:
	shortdisplay(ligptr(p));
	break;

      case discnode:
	shortdisplay(prebreak(p));
	shortdisplay(postbreak(p));
	n = replacecount(p);
	while (n > 0) {
	  if (link(p) != 0)
	    p = link(p);
	  n--;
	}
	break;
      }
    }
    p = link(p);
  }
}
/*:174*/

/*176:*/
Static void printfontandchar(Pointer p)
{
  if (p > memend) {
    print_esc(S(308));
    return;
  }
  if ((font(p) > fontmax))
    print_char('*');
  else  /*267:*/
	/*:267*/
	  print_esc(fontidtext(font(p)));
  print_char(' ');
  print(character(p) - minquarterword);
}


Static void printmark(long p)
{
  print_char('{');
  if (p < himemmin || p > memend)
    print_esc(S(308));
  else
    showtokenlist(link(p), 0, MAX_PRINT_LINE - 10);
  print_char('}');
}


Static void printruledimen(long d)
{
  if (isrunning(d)) {
    print_char('*');
  } else
    print_scaled(d);
}
/*:176*/

/*177:*/
Static void printglue(long d, long order, StrNumber s)
{
  print_scaled(d);
  if ((unsigned long)order > filll) {
    print(S(329));
    return;
  }
  if (order <= normal) {
    if (s != 0)
      print(s);
    return;
  }
  print(S(330));
  while (order > fil) {
    print_char('l');
    order--;
  }
}
/*:177*/

/*178:*/
Static void printspec(long p, StrNumber s)
{
  if ( p >= lomemmax) {
    print_char('*');
    return;
  }
  print_scaled(width(p));
  if (s != 0)
    print(s);
  if (stretch(p) != 0) {
    print(S(331));
    printglue(stretch(p), stretchorder(p), s);
  }
  if (shrink(p) != 0) {
    print(S(332));
    printglue(shrink(p), shrinkorder(p), s);
  }
}
/*:178*/

/*691:*/
Static void printfamandchar(HalfWord p)
{
  print_esc(S(333));
  print_int(fam(p));
  print_char(' ');
  print(character(p) - minquarterword);
}


Static void printdelimiter(HalfWord p)
{
  long a;

  a = smallfam(p) * 256 + smallchar(p) - minquarterword;
  a = a * 4096 + largefam(p) * 256 + largechar(p) - minquarterword;
  if (a < 0)
    print_int(a);
  else
    print_hex(a);
}
/*:691*/

/*692:*/
Static void showinfo(void);


Static void printsubsidiarydata(HalfWord p, ASCIICode c)
{
  if (cur_length() >= depththreshold) {
    if (mathtype(p) != empty)
      print(S(334));
    return;
  }
  append_char(c);
  tempptr = p;
  switch (mathtype(p)) {

  case mathchar:
    println();
    printcurrentstring();
    printfamandchar(p);
    break;

  case subbox:
    showinfo();
    break;

  case submlist:
    if (info(p) == 0) {
      println();
      printcurrentstring();
      print(S(335));
    } else
      showinfo();
    break;
  }
  flush_char();
}
/*:692*/

/*694:*/
void printstyle(long c)
{
  switch (c / 2) {

  case 0:
    print_esc(S(336));
    break;

  case 1:
    print_esc(S(337));
    break;

  case 2:
    print_esc(S(338));
    break;

  case 3:
    print_esc(S(339));
    break;

  default:
    print(S(340));
    break;
  }
}
/*:694*/

/*225:*/
void printskipparam(long n)
{
  switch (n) {

  case lineskipcode:
    print_esc(S(341));
    break;

  case baselineskipcode:
    print_esc(S(342));
    break;

  case parskipcode:
    print_esc(S(343));
    break;

  case abovedisplayskipcode:
    print_esc(S(344));
    break;

  case belowdisplayskipcode:
    print_esc(S(345));
    break;

  case abovedisplayshortskipcode:
    print_esc(S(346));
    break;

  case belowdisplayshortskipcode:
    print_esc(S(347));
    break;

  case leftskipcode:
    print_esc(S(348));
    break;

  case rightskipcode:
    print_esc(S(349));
    break;

  case topskipcode:
    print_esc(S(350));
    break;

  case splittopskipcode:
    print_esc(S(351));
    break;

  case tabskipcode:
    print_esc(S(352));
    break;

  case spaceskipcode:
    print_esc(S(353));
    break;

  case xspaceskipcode:
    print_esc(S(354));
    break;

  case parfillskipcode:
    print_esc(S(355));
    break;

  case thinmuskipcode:
    print_esc(S(356));
    break;

  case medmuskipcode:
    print_esc(S(357));
    break;

  case thickmuskipcode:
    print_esc(S(358));
    break;

  default:
    print(S(359));
    break;
  }
}
/*:225*/

/*182:*/
Static void shownodelist(long p)
{
  long n;
  double g;

  if (cur_length() > depththreshold) {
    if (p > 0)
      print(S(334));
    goto _Lexit;
  }
  n = 0;
  while (p > memmin) {
    println();
    printcurrentstring();
    if (p > memend) {
      print(S(360));
      goto _Lexit;
    }
    n++;
    if (n > breadthmax) {
      print(S(361));
      goto _Lexit;
    }  /*183:*/
    if (ischarnode(p))
      printfontandchar(p);
    else {
      switch (type(p)) {   /*:183*/

      case hlistnode:
      case vlistnode:
      case unsetnode:   /*184:*/
	if (type(p) == hlistnode)
	  print_esc('h');
	else if (type(p) == vlistnode)
	  print_esc('v');
	else
	  print_esc(S(362));
	print(S(363));
	print_scaled(height(p));
	print_char('+');
	print_scaled(depth(p));
	print(S(364));
	print_scaled(width(p));
	if (type(p) == unsetnode) {   /*185:*/
	  if (spancount(p) != minquarterword) {
	    print(S(303));
	    print_int(spancount(p) - minquarterword + 1);
	    print(S(365));
	  }
	  if (gluestretch(p) != 0) {
	    print(S(366));
	    printglue(gluestretch(p), glueorder(p), 0);
	  }
	  if (glueshrink(p) != 0) {
	    print(S(367));
	    printglue(glueshrink(p), gluesign(p), 0);
	  }
	}  /*:185*/
	else {   /*186:*/
	  g = glueset(p);
	  if ((g != 0.0) & (gluesign(p) != normal)) {   /*:186*/
	    print(S(368));
	    if (gluesign(p) == shrinking)
	      print(S(369));
	    if (0 /*labs(mem[p + glueoffset - memmin].int_) < 1048576L */)
	      print(S(370));
	    else {
	      if (fabs(g) > 20000.0) {
		if (g > 0.0)
		  print_char('>');
		else
		  print(S(371));
		printglue(UNITY * 20000, glueorder(p), 0);
	      } else
		printglue((long)floor(UNITY * g + 0.5), glueorder(p), 0);
	    }
	  }
	  if (shiftamount(p) != 0) {
	    print(S(372));
	    print_scaled(shiftamount(p));
	  }
	}
	nodelistdisplay(listptr(p));
	break;
	/*:184*/

      case rulenode:   /*187:*/
	print_esc(S(373));
	printruledimen(height(p));
	print_char('+');
	printruledimen(depth(p));
	print(S(364));
	printruledimen(width(p));
	break;
	/*:187*/

      case insnode:   /*188:*/
	print_esc(S(374));
	print_int(subtype(p) - minquarterword);
	print(S(375));
	print_scaled(height(p));
	print(S(376));
	printspec(splittopptr(p), 0);
	print_char(',');
	print_scaled(depth(p));
	print(S(377));
	print_int(floatcost(p));
	nodelistdisplay(insptr(p));
	break;
	/*:188*/

      case whatsitnode:   /*1356:*/
	switch (subtype(p)) {   /*:1356*/

	case opennode:
	  print_write_whatsit(S(378), p);
	  print_char('=');
	  print_file_name(openname(p), openarea(p), openext(p));
	  break;

	case writenode:
	  print_write_whatsit(S(379), p);
	  printmark(writetokens(p));
	  break;

	case closenode:
	  print_write_whatsit(S(380), p);
	  break;

	case specialnode:
	  print_esc(S(381));
	  printmark(writetokens(p));
	  break;

	case languagenode:
	  print_esc(S(382));
	  print_int(whatlang(p));
	  print(S(383));
	  print_int(whatlhm(p));
	  print_char(',');
	  print_int(whatrhm(p));
	  print_char(')');
	  break;

	default:
	  print(S(384));
	  break;
	}
	break;

      case gluenode:   /*189:*/
	if (subtype(p) >= aleaders) {   /*190:*/
	  print_esc(S(385));
	  if (subtype(p) == cleaders)
	    print_char('c');
	  else if (subtype(p) == xleaders)
	    print_char('x');
	  print(S(386));
	  printspec(glueptr(p), 0);
	  nodelistdisplay(leaderptr(p));
	}  /*:190*/
	else {   /*:189*/
	  print_esc(S(387));
	  if (subtype(p) != normal) {
	    print_char('(');
	    if (subtype(p) < condmathglue)
	      printskipparam(subtype(p) - 1);
	    else {
	      if (subtype(p) == condmathglue)
		print_esc(S(388));
	      else
		print_esc(S(389));
	    }
	    print_char(')');
	  }
	  if (subtype(p) != condmathglue) {
	    print_char(' ');
	    if (subtype(p) < condmathglue)
	      printspec(glueptr(p), 0);
	    else
	      printspec(glueptr(p), S(390));
	  }
	}
	break;

      case kernnode:   /*191:*/
	if (subtype(p) != muglue) {
	  print_esc(S(391));
	  if (subtype(p) != normal)
	    print_char(' ');
	  print_scaled(width(p));
	  if (subtype(p) == acckern)
	    print(S(392));
	} else {   /*:191*/
	  print_esc(S(393));
	  print_scaled(width(p));
	  print(S(390));
	}
	break;

      case mathnode:   /*192:*/
	print_esc(S(394));
	if (subtype(p) == before)
	  print(S(395));
	else
	  print(S(396));
	if (width(p) != 0) {
	  print(S(397));
	  print_scaled(width(p));
	}
	break;
	/*:192*/

      case ligaturenode:   /*193:*/
#ifdef BIG_CHARNODE
	{
	Pointer pp=get_avail();
	type(pp) = charnodetype;
	font(pp) = font_ligchar(p);
	character(pp) = character_ligchar(p);
	printfontandchar(pp);
	FREE_AVAIL(pp);
	}
#else
	printfontandchar(ligchar(p));
#endif
	print(S(398));
	if (subtype(p) > 1)
	  print_char('|');
	fontinshortdisplay = font_ligchar(p);
	shortdisplay(ligptr(p));
	if ((subtype(p)) & 1)
	  print_char('|');
	print_char(')');
	break;
	/*:193*/

      case penaltynode:   /*194:*/
	print_esc(S(399));
	print_int(penalty(p));
	break;
	/*:194*/

      case discnode:   /*195:*/
	print_esc(S(400));
	if (replacecount(p) > 0) {
	  print(S(401));
	  print_int(replacecount(p));
	}
	nodelistdisplay(prebreak(p));
	append_char('|');
	shownodelist(postbreak(p));
	flush_char();
	break;
	/*:195*/

      case marknode:   /*196:*/
	print_esc(S(402));
	printmark(markptr(p));
	break;
	/*:196*/

      case adjustnode:   /*197:*/
	print_esc(S(403));
	nodelistdisplay(adjustptr(p));
	break;
	/*:197*/
	/*690:*/

      case stylenode:
	printstyle(subtype(p));
	break;

      case choicenode:   /*695:*/
	print_esc(S(404));
	append_char('D');
	shownodelist(displaymlist(p));
	flush_char();
	append_char('T');
	shownodelist(textmlist(p));
	flush_char();
	append_char('S');
	shownodelist(scriptmlist(p));
	flush_char();
	append_char('s');
	shownodelist(scriptscriptmlist(p));
	flush_char();
	break;
	/*:695*/

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
      case leftnoad:
      case rightnoad:   /*696:*/
	switch (type(p)) {

	case ordnoad:
	  print_esc(S(405));
	  break;

	case opnoad:
	  print_esc(S(406));
	  break;

	case binnoad:
	  print_esc(S(407));
	  break;

	case relnoad:
	  print_esc(S(408));
	  break;

	case opennoad:
	  print_esc(S(409));
	  break;

	case closenoad:
	  print_esc(S(410));
	  break;

	case punctnoad:
	  print_esc(S(411));
	  break;

	case innernoad:
	  print_esc(S(412));
	  break;

	case overnoad:
	  print_esc(S(413));
	  break;

	case undernoad:
	  print_esc(S(414));
	  break;

	case vcenternoad:
	  print_esc(S(415));
	  break;

	case radicalnoad:
	  print_esc(S(416));
	  printdelimiter(leftdelimiter(p));
	  break;

	case accentnoad:
	  print_esc(S(417));
	  printfamandchar(accentchr(p));
	  break;

	case leftnoad:
	  print_esc(S(418));
	  printdelimiter(nucleus(p));
	  break;

	case rightnoad:
	  print_esc(S(419));
	  printdelimiter(nucleus(p));
	  break;
	}
	if (subtype(p) != normal) {
	  if (subtype(p) == limits)
	    print_esc(S(420));
	  else
	    print_esc(S(421));
	}
	if (type(p) < leftnoad)
	  printsubsidiarydata(nucleus(p), '.');
	printsubsidiarydata(supscr(p), '^');
	printsubsidiarydata(subscr(p), '_');
	break;
	/*:696*/

      case fractionnoad:   /*697:*/
	print_esc(S(422));
	if (thickness(p) == defaultcode)
	  print(S(423));
	else
	  print_scaled(thickness(p));
	if ((smallfam(leftdelimiter(p)) != 0) |
	    (smallchar(leftdelimiter(p)) != minquarterword) |
	    (largefam(leftdelimiter(p)) != 0) |
	    (largechar(leftdelimiter(p)) != minquarterword)) {
	  print(S(424));
	  printdelimiter(leftdelimiter(p));
	}
	if ((smallfam(rightdelimiter(p)) != 0) |
	    (smallchar(rightdelimiter(p)) != minquarterword) |
	    (largefam(rightdelimiter(p)) != 0) |
	    (largechar(rightdelimiter(p)) != minquarterword)) {
	  print(S(425));
	  printdelimiter(rightdelimiter(p));
	}
	printsubsidiarydata(numerator(p), '\\');
	printsubsidiarydata(denominator(p), '/');
	break;
	/*:697*/
	/*:690*/

      default:
	print(S(426));
	break;
      }
    }
    p = link(p);
  }
_Lexit: ;
}
/*:182*/

/*198:*/
Static void showbox(HalfWord p)
{  /*236:*/
  depththreshold = showboxdepth;
  breadthmax = showboxbreadth;   /*:236*/
  if (breadthmax <= 0)
    breadthmax = 5;
#if 0
  if (pool_ptr + depththreshold >= POOL_SIZE)
    depththreshold = POOL_SIZE - pool_ptr - 1;
#else
	depththreshold = str_adjust_to_room(depththreshold);
#endif
  shownodelist(p);
  println();
}
/*:198*/

/*200:*/
Static void deletetokenref(HalfWord p)
{
  if (tokenrefcount(p) == 0)
    flushlist(p);
  else
    (tokenrefcount(p))--;
}
/*:200*/

/*201:*/
Static void deleteglueref(HalfWord p)
{
  karmafastdeleteglueref(p);
}
/*:201*/

/*202:*/
Static void flushnodelist(HalfWord p) {
    Pointer q;

    while (p != 0) {
        q = link(p);
        if (ischarnode(p)) {
            FREE_AVAIL(p);
        } else {
            switch (type(p)) {
            case hlistnode:
            case vlistnode:
            case unsetnode:
                flushnodelist(listptr(p));
                freenode(p, boxnodesize);
                goto _Ldone;
                break;

            case rulenode:
                freenode(p, rulenodesize);
                goto _Ldone;
                break;

            case insnode:
                flushnodelist(insptr(p));
                deleteglueref(splittopptr(p));
                freenode(p, insnodesize);
                goto _Ldone;
                break;

            case whatsitnode: /*1358:*/
                switch (subtype(p)) {

                    case opennode:
                        freenode(p, opennodesize);
                        break;

                    case writenode:
                    case specialnode:
                        deletetokenref(writetokens(p));
                        freenode(p, writenodesize);
                        goto _Ldone;
                        break;

                    case closenode:
                    case languagenode:
                        freenode(p, smallnodesize);
                        break;

                    default:
                        confusion(S(427));
                        break;
                }
                goto _Ldone;
                break;
                /*:1358*/

            case gluenode:
                karmafastdeleteglueref(glueptr(p));
                if (leaderptr(p) != 0) flushnodelist(leaderptr(p));
                break;

            case kernnode:
            case mathnode:
            case penaltynode:
                /* blank case */
                break;

            case ligaturenode:
                flushnodelist(ligptr(p));
                break;

            case marknode:
                deletetokenref(markptr(p));
                break;

            case discnode:
                flushnodelist(prebreak(p));
                flushnodelist(postbreak(p));
                break;

            case adjustnode: /*698:*/
                flushnodelist(adjustptr(p));
                break;

            case stylenode:
                freenode(p, stylenodesize);
                goto _Ldone;
                break;

            case choicenode:
                flushnodelist(displaymlist(p));
                flushnodelist(textmlist(p));
                flushnodelist(scriptmlist(p));
                flushnodelist(scriptscriptmlist(p));
                freenode(p, stylenodesize);
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
                    flushnodelist(info(nucleus(p)));
                if (mathtype(supscr(p)) >= subbox)
                    flushnodelist(info(supscr(p)));
                if (mathtype(subscr(p)) >= subbox)
                    flushnodelist(info(subscr(p)));
                if (type(p) == radicalnoad)
                    freenode(p, radicalnoadsize);
                else if (type(p) == accentnoad)
                    freenode(p, accentnoadsize);
                else
                    freenode(p, noadsize);
                goto _Ldone;
                break;

            case leftnoad:
            case rightnoad:
                freenode(p, noadsize);
                goto _Ldone;
                break;

            case fractionnoad:
                flushnodelist(info(numerator(p)));
                flushnodelist(info(denominator(p)));
                freenode(p, fractionnoadsize);
                goto _Ldone;
                break;
                /*:698*/

            default:
                confusion(S(428));
                break;
            }
            freenode(p, smallnodesize);
_Ldone:;
        }
        p = q;
    } // while (p != 0)
}
/*:202*/

/*204:*/
Static HalfWord copynodelist(HalfWord p)
{
  Pointer h, q, r=0 /* XXXX */;
  char words;

  h = get_avail();
  q = h;
  while (p != 0) {  /*205:*/
    words = 1;
    if (ischarnode(p)) {
      r = get_avail();
#ifdef BIG_CHARNODE
	words = charnodesize;
#endif
    } else {  /*206:*/
      switch (type(p)) {   /*:206*/

      case hlistnode:
      case vlistnode:
      case unsetnode:
	r = getnode(boxnodesize);
	mem[r - memmin + 6] = mem[p - memmin + 6];
	mem[r - memmin + 5] = mem[p - memmin + 5];
	listptr(r) = copynodelist(listptr(p));
	words = 5;
	break;

      case rulenode:
	r = getnode(rulenodesize);
	words = rulenodesize;
	break;

      case insnode:
	r = getnode(insnodesize);
	mem[r - memmin + 4] = mem[p - memmin + 4];
	addglueref(splittopptr(p));
	insptr(r) = copynodelist(insptr(p));
	words = insnodesize - 1;
	break;

      case whatsitnode:   /*1357:*/
	switch (subtype(p)) {   /*:1357*/

	case opennode:
	  r = getnode(opennodesize);
	  words = opennodesize;
	  break;

	case writenode:
	case specialnode:
	  r = getnode(writenodesize);
	  addtokenref(writetokens(p));
	  words = writenodesize;
	  break;

	case closenode:
	case languagenode:
	  r = getnode(smallnodesize);
	  words = smallnodesize;
	  break;

	default:
	  confusion(S(429));
	  break;
	}
	break;

      case gluenode:
	r = getnode(smallnodesize);
	addglueref(glueptr(p));
	glueptr(r) = glueptr(p);
	leaderptr(r) = copynodelist(leaderptr(p));
	break;

      case kernnode:
      case mathnode:
      case penaltynode:
	r = getnode(smallnodesize);
	words = smallnodesize;
	break;

      case ligaturenode:
	r = getnode(smallnodesize);
	mem[ligchar(r) - memmin] = mem[ligchar(p) - memmin];
	ligptr(r) = copynodelist(ligptr(p));
	break;

      case discnode:
	r = getnode(smallnodesize);
	prebreak(r) = copynodelist(prebreak(p));
	postbreak(r) = copynodelist(postbreak(p));
	break;

      case marknode:
	r = getnode(smallnodesize);
	addtokenref(markptr(p));
	words = smallnodesize;
	break;

      case adjustnode:
	r = getnode(smallnodesize);
	adjustptr(r) = copynodelist(adjustptr(p));
	break;

      default:
	confusion(S(430));
	break;
      }
    }
    while (words > 0) {   /*:205*/
      words--;
      mem[r + words - memmin] = mem[p + words - memmin];
    }
    link(q) = r;
    q = r;
    p = link(p);
  }
  link(q) = 0;
  q = link(h);
  FREE_AVAIL(h);
  return q;
}
/*:204*/

/*211:*/
Static void printmode(long m)
{
  if (m > 0) {
    switch (m / (maxcommand + 1)) {

    case 0:
      print(S(431));
      break;

    case 1:
      print(S(432));
      break;

    case 2:
      print(S(433));
      break;
    }
  } else if (m == 0)
    print(S(434));
  else {
    switch (-m / (maxcommand + 1)) {

    case 0:
      print(S(435));
      break;

    case 1:
      print(S(436));
      break;

    case 2:
      print(S(394));
      break;
    }
  }
  print(S(437));
}
/*:211*/

/*216:*/
Static void pushnest(void)
{
  if (nestptr > maxneststack) {
    maxneststack = nestptr;
    if (nestptr == nestsize)
      overflow(S(438), nestsize);
  }
  nest[nestptr] = curlist;
  nestptr++;
  head = get_avail();
  tail = head;
  prevgraf = 0;
  modeline = line;
}
/*:216*/

/*217:*/
Static void popnest(void)
{
  FREE_AVAIL(head);
  nestptr--;
  curlist = nest[nestptr];
}
/*:217*/

/*218:*/
Static void printtotals(void);


Static void showactivities(void)
{
  Pointer q, r;
  long t;
  short TEMP;

  nest[nestptr] = curlist;
  printnl(S(385));
  println();
  for (TEMP = nestptr; TEMP >= 0; TEMP--) {
    int p = TEMP;
    short m = nest[p].modefield;
    MemoryWord a = nest[p].auxfield;
    printnl(S(439));
    printmode(m);
    print(S(440));
    print_int(labs(nest[p].mlfield));
    if (m == hmode) {
      if (nest[p].pgfield != 8585216L) {
	print(S(441));
	print_int(nest[p].pgfield % 65536L);
	print(S(442));
	print_int(nest[p].pgfield / 4194304L);
	print_char(',');
	print_int((nest[p].pgfield / 65536L) & 63);
	print_char(')');
      }
    }
    if (nest[p].mlfield < 0)
      print(S(443));
    if (p == 0) {  /*986:*/
      if (pagehead != pagetail) {   /*:986*/
	printnl(S(444));
	if (outputactive)
	  print(S(445));
	showbox(link(pagehead));
	if (pagecontents > empty) {
	  printnl(S(446));
	  printtotals();
	  printnl(S(447));
	  print_scaled(pagegoal);
	  r = link(pageinshead);
	  while (r != pageinshead) {
	    println();
	    print_esc(S(374));
	    t = subtype(r) - minquarterword;
	    print_int(t);
	    print(S(448));
	    t = x_over_n(height(r), 1000) * count(t);
	    print_scaled(t);
	    if (type(r) == splitup) {
	      q = pagehead;
	      t = 0;
	      do {
		q = link(q);
		if ((type(q) == insnode) & (subtype(q) == subtype(r)))
		  t++;
	      } while (q != brokenins(r));
	      print(S(449));
	      print_int(t);
	      print(S(450));
	    }
	    r = link(r);
	  }
	}
      }
      if (link(contribhead) != 0)
	printnl(S(451));
    }
    showbox(link(nest[p].headfield));   /*219:*/
    switch (abs(m) / (maxcommand + 1)) {   /*:219*/

    case 0:
      printnl(S(452));
      if (a.sc <= ignoredepth)
	print(S(453));
      else
	print_scaled(a.sc);
      if (nest[p].pgfield != 0) {
	print(S(454));
	print_int(nest[p].pgfield);
	print(S(455));
	if (nest[p].pgfield != 1)
	  print_char('s');
      }
      break;

    case 1:
      printnl(S(456));
      print_int(a.hh.UU.lh);
      if (m > 0) {
	if (a.hh.rh > 0) {
	  print(S(457));
	  print_int(a.hh.rh);
	}
      }
      break;

    case 2:
      if (a.int_ != 0) {
	print(S(458));
	showbox(a.int_);
      }
      break;
    }
  }
}
/*:218*/

/*245:*/
Static void begindiagnostic(void) {
    diag_oldsetting = selector;
    if (tracingonline > 0 || selector != TERM_AND_LOG) return;
    selector--;
    if (history == SPOTLESS) history = WARNING_ISSUED;
}

Static void enddiagnostic(Boolean blankline) {
    printnl(S(385));
    if (blankline) println();
    selector = diag_oldsetting;
}
/*:245*/

#ifdef tt_STAT
/// #252:
Static void showeqtb(HalfWord n) {
    if (n < activebase) {
        print_char('?');
        return;
    }
    if (n < gluebase) {
        /*223:*/
        sprint_cs(n);
        print_char('=');
        printcmdchr(eqtype(n), equiv(n));
        if (eqtype(n) >= call) {
            print_char(':');
            showtokenlist(link(equiv(n)), 0, 32);
        }
        return;
    }                    /*:223*/
    if (n < localbase) { /*229:*/
        if (n < skipbase) {
            printskipparam(n - gluebase);
            print_char('=');
            if (n < gluebase + thinmuskipcode)
                printspec(equiv(n), S(459));
            else
                printspec(equiv(n), S(390));
            return;
        }
        if (n < muskipbase) {
            print_esc(S(460));
            print_int(n - skipbase);
            print_char('=');
            printspec(equiv(n), S(459));
            return;
        }
        print_esc(S(461));
        print_int(n - muskipbase);
        print_char('=');
        printspec(equiv(n), S(390));
        return;
    }
    if (n < intbase) { /*233:*/
        if (n == parshapeloc) {
            print_esc(S(462));
            print_char('=');
            if (parshapeptr == 0)
                print_char('0');
            else
                print_int(info(parshapeptr));
            return;
        }
        if (n < toksbase) {
            printcmdchr(assigntoks, n);
            print_char('=');
            if (equiv(n) != 0) showtokenlist(link(equiv(n)), 0, 32);
            return;
        }
        if (n < boxbase) {
            print_esc(S(463));
            print_int(n - toksbase);
            print_char('=');
            if (equiv(n) != 0) showtokenlist(link(equiv(n)), 0, 32);
            return;
        }
        if (n < curfontloc) {
            print_esc(S(464));
            print_int(n - boxbase);
            print_char('=');
            if (equiv(n) == 0) {
                print(S(465));
                return;
            }
            depththreshold = 0;
            breadthmax = 1;
            shownodelist(equiv(n));
            return;
        }
        if (n < catcodebase) { /*234:*/
            if (n == curfontloc)
                print(S(466));
            else if (n < mathfontbase + 16) {
                print_esc(S(266));
                print_int(n - mathfontbase);
            } else if (n < mathfontbase + 32) {
                print_esc(S(267));
                print_int(n - mathfontbase - 16);
            } else {
                print_esc(S(268));
                print_int(n - mathfontbase - 32);
            }
            print_char('=');
            print_esc(fontidtext(equiv(n)));
            return;
        }
        /*:234*/
        if (n < mathcodebase) {
            if (n < lccodebase) {
                print_esc(S(467));
                print_int(n - catcodebase);
            } else if (n < uccodebase) {
                print_esc(S(468));
                print_int(n - lccodebase);
            } else if (n < sfcodebase) {
                print_esc(S(469));
                print_int(n - uccodebase);
            } else {
                print_esc(S(470));
                print_int(n - sfcodebase);
            }
            print_char('=');
            print_int(equiv(n));
            return;
        }
        print_esc(S(471));
        print_int(n - mathcodebase);
        print_char('=');
        print_int(equiv(n));
        return;
    }
    if (n < dimenbase) { /*242:*/
        if (n < countbase)
            printparam(n - intbase);
        else if (n < delcodebase) {
            print_esc(S(472));
            print_int(n - countbase);
        } else {
            print_esc(S(473));
            print_int(n - delcodebase);
        }
        print_char('=');
        print_int(eqtb[n - activebase].int_);
        return;
    }                   /*:242*/
    if (n > eqtbsize) { /*251:*/
        print_char('?');
        return;
    }
    /*:251*/
    if (n < scaledbase)
        printlengthparam(n - dimenbase);
    else {
        print_esc(S(474));
        print_int(n - scaledbase);
    }
    print_char('=');
    print_scaled(eqtb[n - activebase].sc);
    print(S(459));

    /*:229*/
    /*235:*/
    /*:235*/
    /*:233*/
} // #252: showeqtb
#endif // #252: tt_STAT


/*
Static HalfWord idlookup(long j, long l)
{
	return idlookup_p(buffer+j,l);
}
*/

/*259:*/
HalfWord idlookup_p(unsigned char* buffp, long l, int no_new) {
    /*261:*/
    long h;
    Pointer p, k;

    h = buffp[0];
    for (k = 1; k < l; k++) {
        h += h + buffp[k];
        while (h >= hashprime)
            h -= hashprime;
    }
    p = h + hashbase;
    while (true) {
        if (text(p) > 0) {
            if (str_bcmp(buffp, l, text(p))) goto _Lfound;
        }
        if (next(p) == 0) {
            if (no_new) {
                p = undefinedcontrolsequence;
            } else {   
                if (text(p) > 0) {
                    do {
                        if (hashisfull) {
                            overflow(S(475), hashsize);
                        }
                        hashused--;
                    } while (text(hashused) != 0);
                    next(p) = hashused;
                    p = hashused;
                }
                text(p) = str_insert(buffp, l);

                #ifdef tt_STAT
                    cscount++;
                #endif // #260: tt_STAT
            }
            goto _Lfound;
        }
        p = next(p);
    }
_Lfound:
    return p;
}
/*:259*/

/// p105#264
#ifdef tt_INIT
Static void primitive(StrNumber s, QuarterWord c, HalfWord o) {
    if (s < 256) {
        curval = s + singlebase;
    } else {
        // TODO:
        // k ← str start[s]; l ← str start[s + 1] − k;
        // for j ← 0 to l − 1 do buffer[j] ← so(str pool[k + j]);
        curval = idlookup_s(s, false);
        flush_string();
        text(curval) = s;
    }
    eqlevel(curval) = levelone;
    eqtype(curval) = c;
    equiv(curval) = o;
} // #264: primitive
#endif // #264: tt_INIT

/*274:*/
Static void newsavelevel(GroupCode c)
{
  if (saveptr > maxsavestack) {
    maxsavestack = saveptr;
    if (maxsavestack > savesize - 6)
      overflow(S(476), savesize);
  }
  savetype(saveptr) = levelboundary;
  savelevel(saveptr) = curgroup;
  saveindex(saveptr) = curboundary;
  if (curlevel == maxquarterword)
    overflow(S(477), maxquarterword - minquarterword);
  curboundary = saveptr;
  curlevel++;
  saveptr++;
  curgroup = c;
}
/*:274*/

/*275:*/
Static void eqdestroy(MemoryWord w)
{
  Pointer q;

  switch (eqtypefield(w)) {

  case call:
  case longcall:
  case outercall:
  case longoutercall:
    deletetokenref(equivfield(w));
    break;

  case glueref:
    deleteglueref(equivfield(w));
    break;

  case shaperef:
    q = equivfield(w);
    if (q != 0)
      freenode(q, info(q) + info(q) + 1);
    break;

  case boxref:
    flushnodelist(equivfield(w));
    break;
  }
}
/*:275*/

/*276:*/
Static void eqsave(HalfWord p, QuarterWord l)
{
  if (saveptr > maxsavestack) {
    maxsavestack = saveptr;
    if (maxsavestack > savesize - 6)
      overflow(S(476), savesize);
  }
  if (l == levelzero)
    savetype(saveptr) = restorezero;
  else {
    savestack[saveptr] = eqtb[p - activebase];
    saveptr++;
    savetype(saveptr) = restoreoldvalue;
  }
  savelevel(saveptr) = l;
  saveindex(saveptr) = p;
  saveptr++;
}
/*:276*/

/*277:*/
Static void eqdefine(HalfWord p, QuarterWord t, HalfWord e)
{
  if (eqlevel(p) == curlevel)
    eqdestroy(eqtb[p - activebase]);
  else if (curlevel > levelone)
    eqsave(p, eqlevel(p));
  eqlevel(p) = curlevel;
  eqtype(p) = t;
  equiv(p) = e;
}
/*:277*/

/*278:*/
Static void eqworddefine(HalfWord p, long w)
{
  if (xeqlevel[p - intbase] != curlevel) {
    eqsave(p, xeqlevel[p - intbase]);
    xeqlevel[p - intbase] = curlevel;
  }
  eqtb[p - activebase].int_ = w;
}
/*:278*/

/*279:*/
Static void geqdefine(HalfWord p, QuarterWord t, HalfWord e)
{
  eqdestroy(eqtb[p - activebase]);
  eqlevel(p) = levelone;
  eqtype(p) = t;
  equiv(p) = e;
}


Static void geqworddefine(HalfWord p, long w)
{
  eqtb[p - activebase].int_ = w;
  xeqlevel[p - intbase] = levelone;
}
/*:279*/

/*280:*/
Static void saveforafter(HalfWord t)
{
  if (curlevel <= levelone)
    return;
  if (saveptr > maxsavestack) {
    maxsavestack = saveptr;
    if (maxsavestack > savesize - 6)
      overflow(S(476), savesize);
  }
  savetype(saveptr) = inserttoken;
  savelevel(saveptr) = levelzero;
  saveindex(saveptr) = t;
  saveptr++;
}
/*:280*/

/*281:*/
#ifdef tt_STAT
/// #284
Static void restoretrace(HalfWord p, StrNumber s) {
    begindiagnostic();
    print_char('{');
    print(s);
    print_char(' ');
    showeqtb(p);
    print_char('}');
    enddiagnostic(false);
}
#endif // #284: tt_STAT


Static void backinput(void);


Static void unsave(void) {
    Pointer p;
    QuarterWord l = 0 /* XXXX */;

    if (curlevel <= levelone) {
        confusion(S(478));
        return;
    }
    curlevel--; /*282:*/
    while (true) {
        saveptr--;
        if (savetype(saveptr) == levelboundary) break;
        p = saveindex(saveptr);
        if (savetype(saveptr) == inserttoken) { /*326:*/
            HalfWord t = curtok;
            curtok = p;
            backinput();
            curtok = t;
            continue;
        }                                           /*:326*/
        if (savetype(saveptr) == restoreoldvalue) { /*283:*/
            l = savelevel(saveptr);
            saveptr--;
        } else {
            savestack[saveptr] = eqtb[undefinedcontrolsequence - activebase];
        }
        if (p < intbase) {
            if (eqlevel(p) == levelone) {
                eqdestroy(savestack[saveptr]);
                #ifdef tt_STAT
                    if (tracingrestores > 0) restoretrace(p, S(479));
                #endif // #283.1: tt_STAT
            } else {
                eqdestroy(eqtb[p - activebase]);
                eqtb[p - activebase] = savestack[saveptr];
                #ifdef tt_STAT
                    if (tracingrestores > 0) restoretrace(p, S(480));
                #endif // #283.2: tt_STAT
            }
            continue;
        }
        if (xeqlevel[p - intbase] != levelone) {
            eqtb[p - activebase] = savestack[saveptr];
            xeqlevel[p - intbase] = l; 
            #ifdef tt_STAT
                if (tracingrestores > 0) restoretrace(p, S(480));
            #endif // #283.3: tt_STAT
                /*:283*/
        } else {                            
            #ifdef tt_STAT
                if (tracingrestores > 0) restoretrace(p, S(479));
            #endif // #283.4: tt_STAT
        }
    } // while (true)

    curgroup = savelevel(saveptr);
    curboundary = saveindex(saveptr); /*:282*/
}
/*:281*/

/*288:*/
Static void preparemag(void)
{
  if (magset > 0 && mag != magset) {
    printnl(S(292));
    print(S(481));
    print_int(mag);
    print(S(482));
    printnl(S(483));
    help2(S(484),S(485));
    int_error(magset);
    geqworddefine(intbase + magcode, magset);
  }
  if (mag <= 0 || mag > 32768L) {
    printnl(S(292));
    print(S(486));
    help1(S(487));
    int_error(mag);
    geqworddefine(intbase + magcode, 1000);
  }
  magset = mag;
}
/*:288*/

/*295:*/
Static void tokenshow(HalfWord p)
{
  if (p != 0)
    showtokenlist(link(p), 0, 10000000L);
}  /*:295*/


/*296:*/
Static void printmeaning(int cur_chr, int cur_cmd)
{
  printcmdchr(cur_cmd, cur_chr);
  if (cur_cmd >= call) {
    print_char(':');
    println();
    tokenshow(cur_chr);
    return;
  }
  if (cur_cmd != topbotmark)
    return;
  print_char(':');
  println();
  tokenshow(curmark[cur_chr - topmarkcode]);
}
/*:296*/

/*299:*/
Static void showcurcmdchr(void)
{
  begindiagnostic();
  printnl('{');
  if (mode != shownmode) {
    printmode(mode);
    print(S(488));
    shownmode = mode;
  }
  printcmdchr(curcmd, curchr);
  print_char('}');
  enddiagnostic(false);
}
/*:299*/

/*311:*/
Static void showcontext(void)
{   /*:315*/
  enum Selector old_setting;
  long nn;
  Boolean bottomline;   /*315:*/
  short i;
  short j;
  char l;
  long m;
  char n;
  long p, q;

  baseptr = inputptr;
  inputstack[baseptr] = curinput;
  nn = -1;
  bottomline = false;
  while (true) {
    curinput = inputstack[baseptr];
    if (state != tokenlist) {
      if (name > 17 || baseptr == 0)
	bottomline = true;
    }
    if (baseptr == inputptr || bottomline || nn < errorcontextlines) {
	  /*312:*/
	    if (baseptr == inputptr || state != tokenlist ||
		tokentype != backedup || loc != 0) {
	tally = 0;
	old_setting = selector;
	if (state != tokenlist) {  /*313:*/
	  if (name <= 17) {   /*:313*/
	    if (terminalinput) {
	      if (baseptr == 0)
		printnl(S(489));
	      else
		printnl(S(490));
	    } else {
	      printnl(S(491));
	      if (name == 17)
		print_char('*');
	      else
		print_int(name - 1);
	      print_char('>');
	    }
	  } else {
	    printnl(S(492));
	    print_int(line);
	  }
	  print_char(' ');   /*318:*/
	  beginpseudoprint();
	  if (buffer[limit] == endlinechar)
	    j = limit;
	  else
	    j = limit + 1;
	  if (j > 0) {
	    for (i = start; i < j; i++) {   /*:318*/
	      if (i == loc) {
		settrickcount();
	      }
	      print(buffer[i]);
	    }
	  }
	} else {  /*314:*/
	  switch (tokentype) {   /*:314*/

	  case parameter:
	    printnl(S(493));
	    break;

	  case utemplate:
	  case vtemplate:
	    printnl(S(494));
	    break;

	  case backedup:
	    if (loc == 0)
	      printnl(S(495));
	    else
	      printnl(S(496));
	    break;

	  case inserted:
	    printnl(S(497));
	    break;

	  case macro:
	    println();
	    print_cs(name);
	    break;

	  case outputtext:
	    printnl(S(498));
	    break;

	  case everypartext:
	    printnl(S(499));
	    break;

	  case everymathtext:
	    printnl(S(500));
	    break;

	  case everydisplaytext:
	    printnl(S(501));
	    break;

	  case everyhboxtext:
	    printnl(S(502));
	    break;

	  case everyvboxtext:
	    printnl(S(503));
	    break;

	  case everyjobtext:
	    printnl(S(504));
	    break;

	  case everycrtext:
	    printnl(S(505));
	    break;

	  case marktext:
	    printnl(S(506));
	    break;

	  case writetext:
	    printnl(S(507));
	    break;

	  default:
	    printnl('?');
	    break;
	  }
	  /*319:*/
	  beginpseudoprint();
	  if (tokentype < macro)
	    showtokenlist(start, loc, 100000L);
	  else   /*:319*/
	    showtokenlist(link(start), loc, 100000L);
	}
	selector = old_setting;   /*317:*/
	if (trickcount == 1000000L) {
	  settrickcount();
	}
	if (tally < trickcount)
	  m = tally - firstcount;
	else
	  m = trickcount - firstcount;
	if (l + firstcount <= halfERROR_LINE) {
	  p = 0;
	  n = l + firstcount;
	} else {
	  print(S(284));
	  p = l + firstcount - halfERROR_LINE + 3;
	  n = halfERROR_LINE;
	}
	for (q = p; q < firstcount; q++) {
	  print_char(trickbuf[q % ERROR_LINE]);
	}
	println();
	for (q = 1; q <= n; q++)
	  print_char(' ');
	if (m + n <= ERROR_LINE)
	  p = firstcount + m;
	else
	  p = firstcount + ERROR_LINE - n - 3;
	for (q = firstcount; q < p; q++) {
	  print_char(trickbuf[q % ERROR_LINE]);
	}
	if (m + n > ERROR_LINE)   /*:317*/
	  print(S(284));
	nn++;
      }
    }  /*:312*/
    else if (nn == errorcontextlines) {
      printnl(S(284));
      nn++;
    }
    if (bottomline)
      goto _Ldone;
    baseptr--;
  }
_Ldone:
  curinput = inputstack[inputptr];
}
/*:311*/

/*323:*/
Static void begintokenlist(HalfWord p, QuarterWord t)
{
  if (inputptr > maxinstack) {
    maxinstack = inputptr;
    if (inputptr == stacksize)
      overflow(S(508), stacksize);
  }
  inputstack[inputptr] = curinput;
  inputptr++;
  state = tokenlist;
  start = p;
  tokentype = t;
  if (t < macro) {
    loc = p;
    return;
  }
  addtokenref(p);
  if (t == macro) {
    paramstart = paramptr;
    return;
  }
  loc = link(p);
  if (tracingmacros <= 1)
    return;
  begindiagnostic();
  printnl(S(385));
  switch (t) {

  case marktext:
    print_esc(S(402));
    break;

  case writetext:
    print_esc(S(379));
    break;

  default:
    printcmdchr(assigntoks, t - outputtext + outputroutineloc);
    break;
  }
  print(S(310));
  tokenshow(p);
  enddiagnostic(false);
}
/*:323*/

/*324:*/
Static void endtokenlist(void)
{
  if (tokentype >= backedup) {
    if (tokentype <= inserted)
      flushlist(start);
    else {
      deletetokenref(start);
      if (tokentype == macro) {
	while (paramptr > paramstart) {
	  paramptr--;
	  flushlist(paramstack[paramptr]);
	}
      }
    }
  } else if (tokentype == utemplate) {
    if (alignstate > 500000L)
      alignstate = 0;
    else
      fatalerror(S(509));
  }
  popinput();
  checkinterrupt();
}
/*:324*/

/*325:*/
Static void backinput(void)
{
  Pointer p;

  while (state == tokenlist && loc == 0)
    endtokenlist();
  p = get_avail();
  info(p) = curtok;
  if (curtok < rightbracelimit) {
    if (curtok < leftbracelimit)
      alignstate--;
    else
      alignstate++;
  }
  if (inputptr > maxinstack) {
    maxinstack = inputptr;
    if (inputptr == stacksize)
      overflow(S(508), stacksize);
  }
  inputstack[inputptr] = curinput;
  inputptr++;
  state = tokenlist;
  start = p;
  tokentype = backedup;
  loc = p;
}
/*:325*/

/*327:*/
Static void backerror(void)
{
  OKtointerrupt = false;
  backinput();
  OKtointerrupt = true;
  error();
}


Static void inserror(void)
{
  OKtointerrupt = false;
  backinput();
  tokentype = inserted;
  OKtointerrupt = true;
  error();
}
/*:327*/

/*328:*/
Static void beginfilereading(void)
{
  if (inopen == maxinopen)
    overflow(S(510), maxinopen);
  if (first == bufsize)
    overflow(S(511), bufsize);
  inopen++;
  if (inputptr > maxinstack) {
    maxinstack = inputptr;
    if (inputptr == stacksize)
      overflow(S(508), stacksize);
  }
  inputstack[inputptr] = curinput;
  inputptr++;
  iindex = inopen;
  linestack[iindex - 1] = line;
  start = first;
  state = midline;
  name = 0;
}  /*:328*/


/*329:*/
Static void endfilereading(void)
{
  first = start;
  line = linestack[iindex - 1];
  if (name > 17)
    aclose(&curfile);
  popinput();
  inopen--;
}
/*:329*/

/*330:*/
Static void clearforerrorprompt(void)
{
  while (state != tokenlist && terminalinput && inputptr > 0 && loc > limit) {
    endfilereading();
  }
  println();
}
/*:330*/

/*336:*/
Static int checkoutervalidity(int local_curcs)
{
  Pointer p, q;
  if (scannerstatus == normal)
    return local_curcs;
  deletionsallowed = false;   /*337:*/
  if (local_curcs != 0) {   /*:337*/
    if (state == tokenlist || name < 1 || name > 17) {
      p = get_avail();
      info(p) = cstokenflag + local_curcs;
      backlist(p);
    }
    curcmd = spacer;
    curchr = ' ';
  }
  curcs = local_curcs;
  if (scannerstatus > skipping) {   /*338:*/
    runaway();
    if (curcs == 0) {
      printnl(S(292));
      print(S(512));
    } else {
      curcs = 0;
      printnl(S(292));
      print(S(513));
    }
    print(S(514));   /*339:*/
    p = get_avail();
    switch (scannerstatus) {

    case defining:
      print(S(313));
      info(p) = rightbracetoken + '}';
      break;

    case matching:
      print(S(515));
      info(p) = partoken;
      longstate = outercall;
      break;

    case aligning:
      print(S(315));
      info(p) = rightbracetoken + '}';
      q = p;
      p = get_avail();
      link(p) = q;
      info(p) = cstokenflag + frozencr;
      alignstate = -1000000L;
      break;

    case absorbing:
      print(S(316));
      info(p) = rightbracetoken + '}';
      break;
    }
    inslist(p);   /*:339*/
    print(S(516));
    sprint_cs(warningindex);
    help4(S(517),
          S(518),
          S(519),
          S(520));
    error();
  } else {
    printnl(S(292));
    print(S(521));
    printcmdchr(iftest, curif);
    print(S(522));
    print_int(skipline);
    help3(S(523),
          S(524),
          S(525));
    if (curcs != 0)
      curcs = 0;
    else
      helpline[2] = S(526);
    curtok = cstokenflag + frozenfi;
    inserror();
  }
  /*:338*/
  deletionsallowed = true;
  return curcs;
}
/*:336*/

/*340:*/
Static void firmuptheline(void);
/*:340*/

/*341:*/
Static void getnext_worker(Boolean no_new_control_sequence)
{
  short k;
  char cat;
  ASCIICode c, cc=0 /* XXXX */;
  char d;
  int cur_cs, cur_chr, cur_cmd;
_Lrestart:
  cur_cs = 0;
  if (state != tokenlist) {   /*343:*/
_Lswitch__:
    if (loc > limit) {
      state = newline;   /*360:*/
      if (name > 17) {   /*362:*/
	line++;
	first = start;
	if (!forceeof) {
	  if (inputln(curfile, true))
	    firmuptheline();
	  else
	    forceeof = true;
	}
	if (forceeof) {
	  print_char(')');
	  openparens--;
	  fflush(stdout);
	  forceeof = false;
	  endfilereading();
	  cur_cs=checkoutervalidity(cur_cs);
	  goto _Lrestart;
	}
	if (endlinecharinactive) {
	  limit--;
	} else
	  buffer[limit] = endlinechar;
	first = limit + 1;
	loc = start;
      }  /*:362*/
      else {   /*:360*/
	if (!terminalinput) {
	  cur_cmd = 0;
	  cur_chr = 0;
	  goto _Lexit;
	}
	if (inputptr > 0) {
	  endfilereading();
	  goto _Lrestart;
	}
	if (selector < LOG_ONLY)
	  openlogfile();
	if (interaction > nonstopmode) {
	  if (endlinecharinactive) {
	    limit++;
	  }
	  if (limit == start)
	    printnl(S(527));
	  println();
	  first = start;
	  print('*');
	  term_input();
	  limit = last;
	  if (endlinecharinactive) {
	    limit--;
	  } else
	    buffer[limit] = endlinechar;
	  first = limit + 1;
	  loc = start;
	} else
	  fatalerror(S(528));
      }
      checkinterrupt();
      goto _Lswitch__;
    }
    cur_chr = buffer[loc];
    loc++;
_Lreswitch:
    cur_cmd = catcode(cur_chr);   /*344:*/
    switch (state + cur_cmd) {   /*345:*/

    case midline + ignore:
    case skipblanks + ignore:
    case newline + ignore:
    case skipblanks + spacer:
    case newline + spacer:   /*:345*/
      goto _Lswitch__;
      break;

    case midline:
    case skipblanks:
    case newline:   /*354:*/
      if (loc > limit)
	cur_cs = nullcs;
      else {
_Lstartcs_:
	k = loc;
	cur_chr = buffer[k];
	cat = catcode(cur_chr);
	k++;
	if (cat == letter)
	  state = skipblanks;
	else if (cat == spacer)
	  state = skipblanks;
	else
	  state = midline;
	if (cat == letter && k <= limit) {   /*356:*/
	  do {
	    cur_chr = buffer[k];
	    cat = catcode(cur_chr);
	    k++;
	  } while (cat == letter && k <= limit);   /*355:*/
	  if (buffer[k] == cur_chr) {   /*:355*/
	    if (cat == supmark) {
	      if (k < limit) {
		c = buffer[k + 1];
		if (c < 128) {
		  d = 2;
		  if (ishex(c)) {
		    if (k + 2 <= limit) {
		      cc = buffer[k + 2];
		      if (ishex(cc)) {
			d++;
		      }
		    }
		  }
		  if (d > 2) {
		    buffer[k - 1] = cur_chr = hex_to_i(c,cc);
		  } else if (c < 64)
		    buffer[k - 1] = c + 64;
		  else
		    buffer[k - 1] = c - 64;
		  limit -= d;
		  first -= d;
		  while (k <= limit) {
		    buffer[k] = buffer[k + d];
		    k++;
		  }
		  goto _Lstartcs_;
		}
	      }
	    }
	  }
	  if (cat != letter)
	    k--;
	  if (k > loc + 1) {
	    cur_cs = idlookup_p(buffer+loc, k - loc, no_new_control_sequence);
	    loc = k;
	    goto _Lfound;
	  }
	} else  /*355:*/
	{   /*:355*/
	  if (buffer[k] == cur_chr) {
	    if (cat == supmark) {
	      if (k < limit) {
		c = buffer[k + 1];
		if (c < 128) {
		  d = 2;
		  if (ishex(c)) {
		    if (k + 2 <= limit) {
		      cc = buffer[k + 2];
		      if (ishex(cc)) {
			d++;
		      }
		    }
		  }
		  if (d > 2) {
		    buffer[k - 1] = cur_chr = hex_to_i(c,cc);
		  } else if (c < 64)
		    buffer[k - 1] = c + 64;
		  else
		    buffer[k - 1] = c - 64;
		  limit -= d;
		  first -= d;
		  while (k <= limit) {
		    buffer[k] = buffer[k + d];
		    k++;
		  }
		  goto _Lstartcs_;
		}
	      }
	    }
	  }
	}
	/*:356*/
	cur_cs = singlebase + buffer[loc];
	loc++;
      }
#define check_outer {\
        curchr = cur_chr;\
        curcmd=cur_cmd;\
        cur_cs=checkoutervalidity(cur_cs);\
        cur_cmd=curcmd;\
        cur_chr = curchr;\
      }

#define process_cmd \
      if (cur_cmd >= outercall) check_outer \
      break;

#define Process_cs \
      cur_cmd = eqtype(cur_cs);\
      cur_chr = equiv(cur_cs);\
      process_cmd

_Lfound:
      Process_cs
      /*:354*/

    case midline + activechar:
    case skipblanks + activechar:
    case newline + activechar:   /*353:*/
      state = midline;
      cur_cs = cur_chr + activebase;
      Process_cs
      /*:353*/

    case midline + supmark:
    case skipblanks + supmark:
    case newline + supmark:   /*352:*/
      if (cur_chr == buffer[loc]) {
	if (loc < limit) {
	  c = buffer[loc + 1];
	  if (c < 128) {
	    loc += 2;
	    if (ishex(c)) {
	      if (loc <= limit) {
		cc = buffer[loc];
		if (ishex(cc)) {
		  loc++;
		  cur_chr = hex_to_i(c,cc);
		  goto _Lreswitch;
		}
	      }
	    }
	    if (c < 64)
	      cur_chr = c + 64;
	    else
	      cur_chr = c - 64;
	    goto _Lreswitch;
	  }
	}
      }
      state = midline;
      break;
      /*:352*/

    case midline + invalidchar:
    case skipblanks + invalidchar:
    case newline + invalidchar:   /*346:*/
      printnl(S(292));
      print(S(529));
      help2(S(530),
            S(531));
      deletionsallowed = false;
      error();
      deletionsallowed = true;
      goto _Lrestart;
      break;
      /*:346*/
      /*347:*/

    case midline + spacer:   /*349:*/
      state = skipblanks;
      cur_chr = ' ';
      break;
      /*:349*/

    case midline + carret:   /*348:*/
      loc = limit + 1;
      cur_cmd = spacer;
      cur_chr = ' ';
      break;
      /*:348*/

    case skipblanks + carret:
    case midline + comment:
    case skipblanks + comment:
    case newline + comment:   /*:350*/
      /*350:*/
      loc = limit + 1;
      goto _Lswitch__;
      break;

    case newline + carret:   /*351:*/
      loc = limit + 1;
      cur_cs = parloc;
      Process_cs
      /*:351*/

    case midline + leftbrace:
      alignstate++;
      break;

    case skipblanks + leftbrace:
    case newline + leftbrace:
      state = midline;
      alignstate++;
      break;

    case midline + rightbrace:
      alignstate--;
      break;

    case skipblanks + rightbrace:
    case newline + rightbrace:
      state = midline;
      alignstate--;
      break;

    case skipblanks + mathshift:
    case skipblanks + tabmark:
    case skipblanks + macparam:
    case skipblanks + submark:
    case skipblanks + letter:
    case skipblanks + otherchar:
    case newline + mathshift:
    case newline + tabmark:
    case newline + macparam:
    case newline + submark:
    case newline + letter:
    case newline + otherchar:   /*:347*/
      state = midline;
      break;
    }
    /*:344*/
  } else {  /*357:*/
    HalfWord t;
    if (loc == 0) {
      endtokenlist();
      goto _Lrestart;
    }
    t = info(loc);
    loc = link(loc);
    if (t >= cstokenflag) {
      cur_cs = t - cstokenflag;
      cur_cmd = eqtype(cur_cs);
      cur_chr = equiv(cur_cs);
      if (cur_cmd >= outercall) {
	if (cur_cmd == dontexpand) {   /*358:*/
	  cur_cs = info(loc) - cstokenflag;
	  loc = 0;
	  cur_cmd = eqtype(cur_cs);
	  cur_chr = equiv(cur_cs);
	  if (cur_cmd > maxcommand) {
	    cur_cmd = relax;
	    cur_chr = noexpandflag;
	  }
	} else {
	    check_outer
	}
	/*:358*/
      }
    } else {
      cur_cmd = t / dwa_do_8;
      cur_chr = t & (dwa_do_8-1);
      switch (cur_cmd) {

      case leftbrace:
	alignstate++;
	break;

      case rightbrace:
	alignstate--;
	break;

      case outparam:   /*359:*/
	begintokenlist(paramstack[paramstart + cur_chr - 1], parameter);
	goto _Lrestart;
	break;
	/*:359*/
      }
    }
  }
  /*:343*/
  /*342:*/
  if (cur_cmd <= carret) {
    if (cur_cmd >= tabmark) { /*789:*/
      if (alignstate == 0) {
	if (scannerstatus == aligning)
	  fatalerror(S(509));
	cur_cmd = extrainfo(curalign);
	extrainfo(curalign) = cur_chr;
	if (cur_cmd == omit)
	  begintokenlist(omittemplate, vtemplate);
	else
	  begintokenlist(vpart(curalign), vtemplate);
	alignstate = 1000000L;
	goto _Lrestart;
      }
      /*:789*/
      /*:342*/
    }
  }
_Lexit: ;
  curcmd = cur_cmd;
  curchr = cur_chr;
  curcs = cur_cs;

  /*:357*/
}
/*:341*/

static void getnext(void)
{
    getnext_worker(true);
}

/*363:*/
Static void firmuptheline(void)
{
  short k;

  limit = last;
  if (pausing <= 0)
    return;
  if (interaction <= nonstopmode)
    return;
  println();
  if (start < limit) {
    for (k = start; k < limit; k++)
      print(buffer[k]);
  }
  first = limit;
  print(S(532));
  term_input();
  if (last <= first)
    return;
  for (k = first; k < last; k++)
    buffer[k + start - first] = buffer[k];
  limit = start + last - first;
}
/*:363*/

/*365:*/
Static void gettoken(void)
{
  getnext_worker(false);
  curtok=pack_tok(curcs,curcmd,curchr);
}
/*:365*/

/*396:*/
Static void report_argument(HalfWord unbalance, int n, Pointer * pstack)
{
    HalfWord m;
    if (longstate == call) {
        runaway();
        printnl(S(292));
        print(S(533));
        sprint_cs(warningindex);
        print(S(534));
        help3(S(535),
              S(536),
              S(537));
        backerror();
    }
    pstack[n] = link(temphead);
    alignstate -= unbalance;
    for (m = 0; m <= n; m++) {
         flushlist(pstack[m]);
    }
}
/*:396*/
/*366:*/
/*389:*/
Static void macrocall(Pointer refcount)
{
  Pointer r, p=0 /* XXXX */, s, t, u, v, rbraceptr=0 /* XXXX */, savewarningindex;
  /* SmallNumber */ int n;
  HalfWord unbalance, m=0 /* XXXX */;
  SmallNumber savescannerstatus;
  ASCIICode matchchr=0 /* XXXX */;
  Pointer pstack[9];

  savescannerstatus = scannerstatus;
  savewarningindex = warningindex;
  warningindex = curcs;
  r = link(refcount);
  n = 0;
  if (tracingmacros > 0) {   /*401:*/
    begindiagnostic();
    println();
    print_cs(warningindex);
    tokenshow(refcount);
    enddiagnostic(false);
  }
  /*:401*/
  if (info(r) != endmatchtoken) {   /*391:*/
    scannerstatus = matching;
    unbalance = 0;
    longstate = eqtype(curcs);
    if (longstate >= outercall)
      longstate -= 2;
    do {
      link(temphead) = 0;
      if ((info(r) > matchtoken + (dwa_do_8-1)) | (info(r) < matchtoken))
	    /*392:*/
	      s = 0;
      else {
	matchchr = info(r) - matchtoken;
	s = link(r);
	r = s;
	p = temphead;
	m = 0;
      }
_Llabcontinue:
      gettoken();
      if (curtok == info(r)) {   /*394:*/
	r = link(r);
	if ((info(r) >= matchtoken) & (info(r) <= endmatchtoken)) {
	  if (curtok < leftbracelimit)
	    alignstate--;
	  goto _Lfound;
	} else
	  goto _Llabcontinue;
      }
      /*:394*/
      /*397:*/
      if (s != r) {
	if (s == 0) {   /*398:*/
	  printnl(S(292));
	  print(S(538));
	  sprint_cs(warningindex);
	  print(S(539));
	  help4(S(540),
                S(541),
                S(542),
                S(543));
	  error();
	  goto _Lexit;
	}
	/*:398*/
	t = s;
	do {
	  STORE_NEW_TOKEN(p,info(t));
	  m++;
	  u = link(t);
	  v = s;
	  while (true) {
	    if (u == r) {
	      if (curtok != info(v))
		goto _Ldone;
	      else {
		r = link(v);
		goto _Llabcontinue;
	      }
	    }
	    if (info(u) != info(v))
	      goto _Ldone;
	    u = link(u);
	    v = link(v);
	  }
_Ldone:
	  t = link(t);
	} while (t != r);
	r = s;
      }
      if (curtok == partoken) {
	if (longstate != longcall) {
            report_argument(unbalance, n, pstack);
	    goto _Lexit;
	}
      }
      if (curtok < rightbracelimit) {
	if (curtok < leftbracelimit) {   /*399:*/
	  unbalance = 1;
	  while (true) {
	    FAST_STORE_NEW_TOKEN(p,curtok);
	    gettoken();
	    if (curtok == partoken) {
	      if (longstate != longcall) {
                  report_argument(unbalance, n, pstack);
		  goto _Lexit;
	      }
	    }
	    if (curtok >= rightbracelimit)
	      continue;
	    if (curtok < leftbracelimit)
	      unbalance++;
	    else {
	      unbalance--;
	      if (unbalance == 0)
		goto _Ldone1;
	    }
	  }
_Ldone1:
	  rbraceptr = p;
	  STORE_NEW_TOKEN(p,curtok);
	} else {   /*395:*/
	  backinput();
	  printnl(S(292));
	  print(S(544));
	  sprint_cs(warningindex);
	  print(S(545));
	  help6(
            S(546),
            S(547),
            S(548),
            S(549),
            S(550),
            S(551));
	  alignstate++;
	  longstate = call;
	  curtok = partoken;
	  inserror();
	}
	/*:399*/
      } else  /*393:*/
      {   /*:393*/
	if (curtok == spacetoken) {
	  if (info(r) <= endmatchtoken) {
	    if (info(r) >= matchtoken)
	      goto _Llabcontinue;
	  }
	}
	STORE_NEW_TOKEN(p,curtok);
      }
      m++;
      if (info(r) > endmatchtoken)
	goto _Llabcontinue;
      if (info(r) < matchtoken)
	goto _Llabcontinue;
_Lfound:
      if (s != 0) {   /*400:*/
	if (((m == 1) & (info(p) < rightbracelimit)) && p != temphead) {
	  link(rbraceptr) = 0;
	  FREE_AVAIL(p);
	  p = link(temphead);
	  pstack[n] = link(p);
	  FREE_AVAIL(p);
	} else
	  pstack[n] = link(temphead);
	n++;
	if (tracingmacros > 0) {
	  begindiagnostic();
	  printnl(matchchr);
	  print_int(n);
	  print(S(552));
	  showtokenlist(pstack[n - 1], 0, 1000);
	  enddiagnostic(false);
	}
      }
      /*:400*/
      /*:392*/
    } while (info(r) != endmatchtoken);
  }
  /*:391*/
  /*390:*/
  while (state == tokenlist && loc == 0)
    endtokenlist();
  begintokenlist(refcount, macro);
  name = warningindex;
  loc = link(r);
  if (n > 0) {   /*:390*/
    if (paramptr + n > maxparamstack) {
      maxparamstack = paramptr + n;
      if (maxparamstack > paramsize)
	overflow(S(553), paramsize);
    }
    for (m = 0; m < n; m++)
      paramstack[paramptr + m] = pstack[m];
    paramptr += n;
  }
_Lexit:
  scannerstatus = savescannerstatus;
  warningindex = savewarningindex;

  /*:397*/
  /*:395*/
}
/*:389*/

/*379:*/
Static void insertrelax(void)
{
  curtok = cstokenflag + curcs;
  backinput();
  curtok = cstokenflag + frozenrelax;
  backinput();
  tokentype = inserted;
}  /*:379*/


Static void passtext(void);

Static void startinput(void);

Static void conditional(void);

Static void getxtoken(void);

Static void convtoks(void);

Static void insthetoks(void);

Static void skip_spaces(void)
{  /*406:*/
    do {
        getxtoken();
    } while (curcmd == spacer); /*:406*/
}

Static void skip_spaces_or_relax(void)
{ /*404:*/
    do {
        getxtoken();
    } while (curcmd == spacer || curcmd == relax); /*:404*/
}


Static void expand(void)
{
  HalfWord t;
  Pointer p, r, backupbackup;
  short j;
  long cvbackup;
  SmallNumber cvlbackup, radixbackup, cobackup, savescannerstatus;

  cvbackup = curval;
  cvlbackup = curvallevel;
  radixbackup = radix;
  cobackup = curorder;

  backupbackup = link(backuphead);

  if (curcmd < call) {   /*367:*/
    if (tracingcommands > 1)
      showcurcmdchr();
    switch (curcmd) {

    case topbotmark:   /*386:*/
      if (curmark[curchr - topmarkcode] != 0)
	begintokenlist(curmark[curchr - topmarkcode], marktext);
      break;
      /*:386*/

    case expandafter:   /*368:*/
      gettoken();
      t = curtok;
      gettoken();
      if (curcmd > maxcommand)
	expand();
      else
	backinput();
      curtok = t;
      backinput();
      break;
      /*:368*/

    case noexpand:   /*369:*/
      savescannerstatus = scannerstatus;
      scannerstatus = normal;
      gettoken();
      scannerstatus = savescannerstatus;
      t = curtok;
      backinput();
      if (t >= cstokenflag) {
	p = get_avail();
	info(p) = cstokenflag + frozendontexpand;
	link(p) = loc;
	start = p;
	loc = p;
      }
      break;
      /*:369*/

    case csname:   /*372:*/
      r = get_avail();
      p = r;
      do {
	getxtoken();
	if (curcs == 0) {
	  STORE_NEW_TOKEN(p,curtok);
	}
      } while (curcs == 0);
      if (curcmd != endcsname) {   /*373:*/
	printnl(S(292));
	print(S(554));
	print_esc(S(263));
	print(S(555));
	help2(S(556),
              S(557));
	backerror();
      }
      /*:373*/
      /*374:*/
      j = first;
      p = link(r);
      while (p != 0) {
	if (j >= maxbufstack) {
	  maxbufstack = j + 1;
	  if (maxbufstack == bufsize)
	    overflow(S(511), bufsize);
	}
	buffer[j] = (info(p)) & (dwa_do_8-1);
	j++;
	p = link(p);
      }
      if (j > first + 1) {
	curcs = idlookup_p(buffer+first, j - first, false);
      } else if (j == first)
	curcs = nullcs;
      else
	curcs = singlebase + buffer[first];   /*:374*/
      flushlist(r);
      if (eqtype(curcs) == undefinedcs)
	eqdefine(curcs, relax, 256);
      curtok = curcs + cstokenflag;
      backinput();
      break;
      /*:372*/

    case convert:
      convtoks();
      break;

    case the:
      insthetoks();
      break;

    case iftest:
      conditional();
      break;

    case fiorelse:   /*510:*/
      if (curchr > iflimit) {
	if (iflimit == ifcode)
	  insertrelax();
	else {
	  printnl(S(292));
	  print(S(558));
	  printcmdchr(fiorelse, curchr);
	  help1(S(559));
	  error();
	}
      } else {   /*:510*/
	while (curchr != ficode)   /*496:*/
	  passtext();
	p = condptr;
	ifline = iflinefield(p);
	curif = subtype(p);
	iflimit = type(p);
	condptr = link(p);
	freenode(p, ifnodesize);   /*:496*/
      }
      break;

    case input:   /*378:*/
      if (curchr > 0)
	forceeof = true;
      else if (nameinprogress)
	insertrelax();
      else
	startinput();
      break;
      /*370:*/

    default:
      printnl(S(292));
      print(S(560));
      help5(S(561),
            S(562),
            S(563),
            S(564),
            S(565));
      error();   /*:370*/
      break;
    }
  }  /*:367*/
  else if (curcmd < endtemplate)
    macrocall(curchr);
  else {
    curtok = cstokenflag + frozenendv;
    backinput();
  }
  curval = cvbackup;
  curvallevel = cvlbackup;
  radix = radixbackup;
  curorder = cobackup;

  link(backuphead) = backupbackup;

  /*:378*/
}
/*:366*/

/*380:*/
Static void getxtoken(void) {
_Lrestart:
    getnext();
    if (curcmd <= maxcommand) goto _Ldone;
    if (curcmd >= call) {
        if (curcmd >= endtemplate) {
            curcs = frozenendv;
            curcmd = endv;
            goto _Ldone;
        }
        macrocall(curchr);
    } else
        expand();
    goto _Lrestart;
_Ldone:
    curtok = pack_tok(curcs, curcmd, curchr);
}
/*:380*/

/*381:*/
Static void xtoken(void)
{
  while (curcmd > maxcommand) {
    expand();
    getnext();
  }
  curtok = pack_tok(curcs,curcmd,curchr);
}
/*:381*/

/*403:*/
Static void scanleftbrace(void)
{
  skip_spaces_or_relax();
  if (curcmd == leftbrace)
    return;
  printnl(S(292));
  print(S(566));
  help4(S(567),
        S(568),
        S(569),
        S(570));
  backerror();
  curtok = leftbracetoken + '{';
  curcmd = leftbrace;
  curchr = '{';
  alignstate++;
}
/*:403*/

/*405:*/
Static void scanoptionalequals(void)
{
  skip_spaces();
  if (curtok != othertoken + '=')
    backinput();
}
/*:405*/

/*407:*/
Static Boolean scankeyword(StrNumber s)
{
  Boolean Result;
#if 1
  Pointer p = get_avail();
  Pointer my_backup_head = p;
#else
  Pointer my_backup_head = backuphead;
  Pointer p = backuphead;
#endif
#if 0
  PoolPtr k;
#else
  int k;
  int k_e;
#endif
  link(p) = 0;
#if 0
  k = str_start[s];
  while (k < str_end(s) ) {
    getxtoken();
    if ((curcs == 0) & ((curchr == str_pool[k]) |
			(curchr == str_pool[k] - 'a' + 'A'))) {
#else
  k=0;
  k_e=str_length(s);
  while(k<k_e) {
    int str_c = str_getc(s,k);
    getxtoken();
    if ((curcs == 0) & ((curchr == str_c) |
                        (curchr == str_c - 'a' + 'A'))) {
#endif
      STORE_NEW_TOKEN(p,curtok);
      k++;
      continue;
    } else {
      if (curcmd == spacer && p == my_backup_head)
	continue;
      backinput();
      if (p != my_backup_head) {
	backlist(link(my_backup_head));
      }
      Result = false;
      goto _Lexit;
    }
  }
  flushlist(link(my_backup_head));
  Result = true;
_Lexit:

  FREE_AVAIL(my_backup_head);

  return Result;
}
/*:407*/

/*408:*/
Static void muerror(void)
{
  printnl(S(292));
  print(S(571));
  help1(S(572));
  error();
}
/*:408*/

/*409:*/
Static void scanint(void);


/*433:*/
Static void scaneightbitint(void)
{
  scanint();
  if ((unsigned long)curval <= 255)
    return;
  printnl(S(292));
  print(S(573));
  help2(S(574),
        S(575));
  int_error(curval);
  curval = 0;
}
/*:433*/

/*434:*/
Static void scancharnum(void)
{
  scanint();
  if ((unsigned long)curval <= 255)
    return;
  printnl(S(292));
  print(S(576));
  help2(S(577),
        S(575));
  int_error(curval);
  curval = 0;
}
/*:434*/

/*435:*/
Static void scanfourbitint(void)
{
  scanint();
  if ((unsigned long)curval <= 15)
    return;
  printnl(S(292));
  print(S(578));
  help2(S(579),
        S(575));
  int_error(curval);
  curval = 0;
}
/*:435*/

/*436:*/
Static void scanfifteenbitint(void)
{
  scanint();
  if ((unsigned long)curval <= 32767)
    return;
  printnl(S(292));
  print(S(580));
  help2(S(581),
        S(575));
  int_error(curval);
  curval = 0;
}
/*:436*/

/*437:*/
Static void scantwentysevenbitint(void)
{
  scanint();
  if ((unsigned long)curval <= 134217727L)
    return;
  printnl(S(292));
  print(S(582));
  help2(S(583),
        S(575));
  int_error(curval);
  curval = 0;
}
/*:437*/

/*577:*/
Static void scanfontident(void)
{   /*406:*/
  InternalFontNumber f;
  HalfWord m;

  skip_spaces();
  if (curcmd == deffont)
    f = curfont;
  else if (curcmd == setfont)
    f = curchr;
  else if (curcmd == deffamily) {
    m = curchr;
    scanfourbitint();
    f = equiv(m + curval);
  } else {
    printnl(S(292));
    print(S(584));
    help2(S(585),
          S(586));
    backerror();
    f = nullfont;
  }
  curval = f;
}
/*:577*/

/*578:*/
Static void findfontdimen(Boolean writing) {
    InternalFontNumber f;
    long n;

    scanint();
    n = curval;
    scanfontident();
    f = curval;
    if (n <= 0) {
        curval = fmemptr;
    } else {
        if (writing && n <= spaceshrinkcode && n >= spacecode &&
            fontglue[f] != 0) {
            deleteglueref(fontglue[f]);
            fontglue[f] = 0;
        }
        if (n > fontparams[f]) {
            if (f < fontptr) {
                curval = fmemptr;
            } else { /*:580*/
                do {
                    if (fmemptr == fontmemsize) overflow(S(587), fontmemsize);
                    fontinfo[fmemptr].sc = 0;
                    fmemptr++;
                    fontparams[f]++;
                } while (n != fontparams[f]);
                curval = fmemptr - 1;
            }
        } else {
            curval = n + parambase[f];
        }
    } /*579:*/
    if (curval != fmemptr) return; /*:579*/
    printnl(S(292));
    print(S(588));
    print_esc(fontidtext(f));
    print(S(589));
    print_int(fontparams[f]);
    print(S(590));
    help2(S(591), S(592));
    error();
}
/*:578*/

/*:409*/
/*413:*/
Static void scansomethinginternal(SmallNumber level, Boolean negative)
{
  HalfWord m;
  /* char */ int p; /* INT */

  m = curchr;
  switch (curcmd) {

  case defcode:   /*414:*/
    scancharnum();
    if (m == mathcodebase) {
      curval = mathcode(curval);
      curvallevel = intval;
    } else if (m < mathcodebase) {
      curval = equiv(m + curval);
      curvallevel = intval;
    } else {
      curval = eqtb[m + curval - activebase].int_;
      curvallevel = intval;
    }
    break;
    /*:414*/

  case toksregister:
  case assigntoks:
  case deffamily:
  case setfont:
  case deffont:   /*415:*/
    if (level != tokval) {
      printnl(S(292));
      print(S(593));
      help3(S(594),
            S(595),
            S(596));
      backerror();
      curval = 0;
      curvallevel = dimenval;
    } else if (curcmd <= assigntoks) {
      if (curcmd < assigntoks) {
	scaneightbitint();
	m = toksbase + curval;
      }
      curval = equiv(m);
      curvallevel = tokval;
    } else {
      backinput();
      scanfontident();
      curval += fontidbase;
      curvallevel = identval;
    }
    break;

  case assignint:
    curval = eqtb[m - activebase].int_;
    curvallevel = intval;
    break;

  case assigndimen:
    curval = eqtb[m - activebase].sc;
    curvallevel = dimenval;
    break;

  case assignglue:
    curval = equiv(m);
    curvallevel = glueval;
    break;

  case assignmuglue:
    curval = equiv(m);
    curvallevel = muval;
    break;

  case setaux:   /*418:*/
    if (labs(mode) != m) {
      printnl(S(292));
      print(S(597));
      printcmdchr(setaux, m);
      help4(S(598),
            S(599),
            S(600),
            S(601));
      error();
      if (level != tokval) {
	curval = 0;
	curvallevel = dimenval;
      } else {
	curval = 0;
	curvallevel = intval;
      }
    } else if (m == vmode) {
      curval = prevdepth;
      curvallevel = dimenval;
    } else {
      curval = spacefactor;
      curvallevel = intval;
    }
    break;

  case setprevgraf:   /*422:*/
    if (mode == 0) {
      curval = 0;
      curvallevel = intval;
    } else {   /*:422*/
      nest[nestptr] = curlist;
      p = nestptr;
      while (abs(nest[p].modefield) != vmode)
	p--;
      curval = nest[p].pgfield;
      curvallevel = intval;
    }
    break;

  case setpageint:   /*419:*/
    if (m == 0)
      curval = deadcycles;
    else
      curval = insertpenalties;
    curvallevel = intval;
    break;
    /*:419*/

  case setpagedimen:   /*421:*/
    if (pagecontents == empty && !outputactive) {
      if (m == 0)
	curval = maxdimen;
      else
	curval = 0;
    } else
      curval = pagesofar[m];
    curvallevel = dimenval;
    break;
    /*:421*/

  case setshape:   /*423:*/
    if (parshapeptr == 0)
      curval = 0;
    else
      curval = info(parshapeptr);
    curvallevel = intval;
    break;
    /*:423*/

  case setboxdimen:   /*420:*/
    scaneightbitint();
    if (box(curval) == 0)
      curval = 0;
    else
      curval = mem[box(curval) + m - memmin].sc;
    curvallevel = dimenval;
    break;
    /*:420*/

  case chargiven:
  case mathgiven:
    curval = curchr;
    curvallevel = intval;
    break;

  case assignfontdimen:   /*425:*/
    findfontdimen(false);
    fontinfo[fmemptr].sc = 0;
    curval = fontinfo[curval].sc;
    curvallevel = dimenval;
    break;
    /*:425*/

  case assignfontint:   /*426:*/
    scanfontident();
    if (m == 0) {
      curval = get_hyphenchar(curval);
      curvallevel = intval;
    } else {
      curval = get_skewchar(curval);
      curvallevel = intval;
    }
    break;
    /*:426*/

  case register_:   /*427:*/
    scaneightbitint();
    switch (m) {

    case intval:
      curval = count(curval);
      break;

    case dimenval:
      curval = dimen(curval);
      break;

    case glueval:
      curval = skip(curval);
      break;

    case muval:
      curval = muskip(curval);
      break;
    }
    curvallevel = m;
    break;
    /*:427*/

  case lastitem:   /*424:*/
    if (curchr > glueval) {
      if (curchr == inputlinenocode)
	curval = line;
      else
	curval = lastbadness;
      curvallevel = intval;
    } else {   /*:424*/
      if (curchr == glueval)
	curval = zeroglue;
      else
	curval = 0;
      curvallevel = curchr;
      if (!ischarnode(tail) && mode != 0) {
	switch (curchr) {

	case intval:
	  if (type(tail) == penaltynode)
	    curval = penalty(tail);
	  break;

	case dimenval:
	  if (type(tail) == kernnode)
	    curval = width(tail);
	  break;

	case glueval:
	  if (type(tail) == gluenode) {
	    curval = glueptr(tail);
	    if (subtype(tail) == muglue)
	      curvallevel = muval;
	  }
	  break;
	}
      } else if (mode == vmode && tail == head) {
	switch (curchr) {

	case intval:
	  curval = lastpenalty;
	  break;

	case dimenval:
	  curval = lastkern;
	  break;

	case glueval:
	  if (lastglue != maxhalfword)
	    curval = lastglue;
	  break;
	}
      }
    }
    break;
    /*428:*/

  default:
    printnl(S(292));
    print(S(602));
    printcmdchr(curcmd, curchr);
    print(S(603));
    print_esc(S(604));
    help1(S(601));
    error();
    if (level != tokval) {   /*:428*/
      curval = 0;
      curvallevel = dimenval;
    } else {
      curval = 0;
      curvallevel = intval;
    }
    break;
  }
  while (curvallevel > level) {   /*429:*/
    if (curvallevel == glueval)
      curval = width(curval);
    else if (curvallevel == muval)
      muerror();
    curvallevel--;
  }
  /*:429*/
  /*430:*/
  if (!negative) {
    if (curvallevel >= glueval && curvallevel <= muval) {
      addglueref(curval);   /*:430*/
    }
    return;
  }
  if (curvallevel < glueval) {
    curval = -curval;
    return;
  }
  curval = newspec(curval);   /*431:*/
  width(curval) = -width(curval);
  stretch(curval) = -stretch(curval);
  shrink(curval) = -shrink(curval);   /*:431*/

  /*:415*/
  /*:418*/
}
/*:413*/

/*440:*/
Static void scanint(void)
{
  Boolean negative;
  long m;
  SmallNumber d;
  Boolean vacuous, OKsofar;

  radix = 0;
  OKsofar = true;   /*441:*/
  negative = false;
  do {
    skip_spaces();
    if (curtok == othertoken + '-') {   /*:441*/
      negative = !negative;
      curtok = othertoken + '+';
    }
  } while (curtok == othertoken + '+');
  if (curtok == alphatoken) {   /*442:*/
    gettoken();
    if (curtok < cstokenflag) {
      curval = curchr;
      if (curcmd <= rightbrace) {
	if (curcmd == rightbrace)
	  alignstate++;
	else
	  alignstate--;
      }
    } else if (curtok < cstokenflag + singlebase)
      curval = curtok - cstokenflag - activebase;
    else
      curval = curtok - cstokenflag - singlebase;
    if (curval > 255) {
      printnl(S(292));
      print(S(605));
      help2(S(606),
            S(607));
      curval = '0';
      backerror();
    } else {   /*443:*/
      getxtoken();
      if (curcmd != spacer)
	backinput();
    }
  }  /*:442*/
  else if (curcmd >= mininternal && curcmd <= maxinternal)
    scansomethinginternal(intval, false);
  else {
    radix = 10;
    m = 214748364L;
    if (curtok == octaltoken) {
      radix = 8;
      m = 268435456L;
      getxtoken();
    } else if (curtok == hextoken) {
      radix = 16;
      m = 134217728L;
      getxtoken();
    }
    vacuous = true;
    curval = 0;   /*445:*/
    while (true) {
      if (curtok < zerotoken + radix && curtok >= zerotoken &&
	  curtok <= zerotoken + 9)
	d = curtok - zerotoken;
      else if (radix == 16) {
	if (curtok <= Atoken + 5 && curtok >= Atoken)
	  d = curtok - Atoken + 10;
	else if (curtok <= otherAtoken + 5 && curtok >= otherAtoken)
	  d = curtok - otherAtoken + 10;
	else
	  goto _Ldone;
      } else
	goto _Ldone;
      vacuous = false;
      if (curval >= m && (curval > m || d > 7 || radix != 10)) {
	if (OKsofar) {
	  printnl(S(292));
	  print(S(608));
	  help2(S(609),
                S(610));
	  error();
	  curval = infinity;
	  OKsofar = false;
	}
      } else
	curval = curval * radix + d;
      getxtoken();
    }
_Ldone:   /*:445*/
    if (vacuous) {   /*446:*/
      printnl(S(292));
      print(S(593));
      help3(S(594),
            S(595),
            S(596));
      backerror();
    }  /*:446*/
    else if (curcmd != spacer)
      backinput();
  }
  if (negative)
    curval = -curval;

  /*:443*/
  /*444:*/
  /*:444*/
}
/*:440*/

/*448:*/
Static void scandimen(Boolean mu, Boolean inf, Boolean shortcut)
{
  Boolean negative;
  long f;
  /*450:*/
  long num, denom;
  /* SmallNumber */ int k, kk; /* INT */
  Pointer p, q;
  Scaled v;
  long savecurval;   /*:450*/
  char digs[23];

  f = 0;
  arith_error = false;
  curorder = normal;
  negative = false;
  if (!shortcut) {   /*441:*/
    negative = false;
    do {
      skip_spaces();
      if (curtok == othertoken + '-') {   /*:441*/
	negative = !negative;
	curtok = othertoken + '+';
      }
    } while (curtok == othertoken + '+');
    if (curcmd >= mininternal && curcmd <= maxinternal) {   /*449:*/
      if (mu) {
	scansomethinginternal(muval, false);   /*451:*/
	if (curvallevel >= glueval) {   /*:451*/
	  v = width(curval);
	  deleteglueref(curval);
	  curval = v;
	}
	if (curvallevel == muval)
	  goto _Lattachsign_;
	if (curvallevel != intval)
	  muerror();
      } else {
	scansomethinginternal(dimenval, false);
	if (curvallevel == dimenval)
	  goto _Lattachsign_;
      }  /*:449*/
    } else {
      backinput();
      if (curtok == continentalpointtoken)
	curtok = pointtoken;
      if (curtok != pointtoken)
	scanint();
      else {
	radix = 10;
	curval = 0;
      }
      if (curtok == continentalpointtoken)
	curtok = pointtoken;
      if (radix == 10 && curtok == pointtoken) {   /*452:*/
	k = 0;
	p = 0;
	gettoken();
	while (true) {
	  getxtoken();
	  if (curtok > zerotoken + 9 || curtok < zerotoken)
	    goto _Ldone1;
	  if (k >= 17)
	    continue;
	  q = get_avail();
	  link(q) = p;
	  info(q) = curtok - zerotoken;
	  p = q;
	  k++;
	}
_Ldone1:
	for (kk = k - 1; kk >= 0; kk--) {
	  digs[kk] = info(p);
	  q = p;
	  p = link(p);
	  FREE_AVAIL(q);
	}
	f = round_decimals(k,digs);
	if (curcmd != spacer)
	  backinput();
      }
      /*:452*/
    }
  }
  if (curval < 0) {
    negative = !negative;
    curval = -curval;
  }  /*453:*/
  if (inf) {   /*454:*/
    if (scankeyword(S(330))) {   /*:454*/
      curorder = fil;
      while (scankeyword('l')) {
	if (curorder != filll) {
	  curorder++;
	  continue;
	}
	printnl(S(292));
	print(S(611));
	print(S(612));
	help1(S(613));
	error();
      }
      goto _Lattachfraction_;
    }
  }
  /*455:*/
  savecurval = curval;
  skip_spaces();
  if (curcmd >= mininternal && curcmd <= maxinternal) {
    if (mu) {
      scansomethinginternal(muval, false);   /*451:*/
      if (curvallevel >= glueval) {   /*:451*/
	v = width(curval);
	deleteglueref(curval);
	curval = v;
      }
      if (curvallevel != muval)
	muerror();
    } else
      scansomethinginternal(dimenval, false);
    v = curval;
    goto _Lfound;
  }
  backinput();
  if (mu)
    goto _Lnotfound;
  if (scankeyword(S(614)))   /*443:*/
    v = quad(curfont);   /*558:*/
    /*:558*/
  else if (scankeyword(S(615)))
    v = xheight(curfont);   /*559:*/
    /*:559*/
  else
    goto _Lnotfound;
  getxtoken();
  if (curcmd != spacer)   /*:443*/
    backinput();
_Lfound:
  curval = mult_and_add(savecurval, v, xn_over_d(v, f, 65536L), 1073741823L);
  goto _Lattachsign_;
_Lnotfound:   /*:455*/
  if (mu) {   /*456:*/
    if (scankeyword(S(390)))
      goto _Lattachfraction_;
    else {   /*:456*/
      printnl(S(292));
      print(S(611));
      print(S(616));
      help4(S(617),
            S(618),
            S(619),
            S(620));
      error();
      goto _Lattachfraction_;
    }
  }
  if (scankeyword(S(621))) {   /*457:*/
    preparemag();
    if (mag != 1000) {
      curval = xn_over_d(curval, 1000, mag);
      f = (f * 1000 + tex_remainder * 65536L) / mag;
      curval += f / 65536L;
      f %= 65536L;
    }
  }
  /*:457*/
  if (scankeyword(S(459)))   /*458:*/
    goto _Lattachfraction_;
  if (scankeyword(S(622))) {
    num = 7227;
    denom = 100;
  } else if (scankeyword(S(623))) {
    num = 12;
    denom = 1;
  } else if (scankeyword(S(624))) {
    num = 7227;
    denom = 254;
  } else if (scankeyword(S(625))) {
    num = 7227;
    denom = 2540;
  } else if (scankeyword(S(626))) {
    num = 7227;
    denom = 7200;
  } else if (scankeyword(S(627))) {
    num = 1238;
    denom = 1157;
  } else if (scankeyword(S(628))) {
    num = 14856;
    denom = 1157;
  } else if (scankeyword(S(629)))
    goto _Ldone;
  else {
    printnl(S(292));
    print(S(611));
    print(S(630));
    help6(S(631),
          S(632),
          S(633),
          S(618),
          S(619),
          S(620));
    error();
    goto _Ldone2;
  }
  curval = xn_over_d(curval, num, denom);
  f = (num * f + tex_remainder * 65536L) / denom;
  curval += f / 65536L;
  f %= 65536L;
_Ldone2:   /*:458*/
_Lattachfraction_:
  if (curval >= 16384)
    arith_error = true;
  else
    curval = curval * UNITY + f;
_Ldone:   /*:453*/
  /*443:*/
  getxtoken();
  if (curcmd != spacer)   /*:443*/
    backinput();
_Lattachsign_:
  if (arith_error || labs(curval) >= 1073741824L) {   /*460:*/
    printnl(S(292));
    print(S(634));
    help2(S(635),
          S(636));
    error();
    curval = maxdimen;
    arith_error = false;
  }
  /*:460*/
  if (negative)
    curval = -curval;

  /*459:*/
  /*:459*/
}
/*:448*/

/*461:*/
Static void scanglue(SmallNumber level)
{
  Boolean negative, mu;
  Pointer q;

  mu = (level == muval);   /*441:*/
  negative = false;
  do {
    skip_spaces();
    if (curtok == othertoken + '-') {   /*:441*/
      negative = !negative;
      curtok = othertoken + '+';
    }
  } while (curtok == othertoken + '+');
  if (curcmd >= mininternal && curcmd <= maxinternal) {   /*462:*/
    scansomethinginternal(level, negative);
    if (curvallevel >= glueval) {
      if (curvallevel != level)
	muerror();
      goto _Lexit;
    }
    if (curvallevel == intval)
      scandimen(mu, false, true);
    else if (level == muval)
      muerror();
  } else {
    backinput();
    scandimen(mu, false, false);
    if (negative)
      curval = -curval;
  }
  q = newspec(zeroglue);
  width(q) = curval;
  if (scankeyword(S(637))) {
    scandimen(mu, true, false);
    stretch(q) = curval;
    stretchorder(q) = curorder;
  }
  if (scankeyword(S(638))) {
    scandimen(mu, true, false);
    shrink(q) = curval;
    shrinkorder(q) = curorder;
  }
  curval = q;   /*:462*/
_Lexit: ;
}
/*:461*/

/*463:*/
Static HalfWord scanrulespec(void)
{
  Pointer q;

  q = newrule();
  if (curcmd == vrule)
    width(q) = defaultrule;
  else {
    height(q) = defaultrule;
    depth(q) = 0;
  }
_Lreswitch:
  if (scankeyword(S(639))) {
    scannormaldimen();
    width(q) = curval;
    goto _Lreswitch;
  }
  if (scankeyword(S(640))) {
    scannormaldimen();
    height(q) = curval;
    goto _Lreswitch;
  }
  if (!scankeyword(S(641)))
    return q;
  scannormaldimen();
  depth(q) = curval;
  goto _Lreswitch;
}
/*:463*/

/*464:*/

Pointer tex_global_p;

Static void strtoks_helper(StrASCIICode t)
{
    long tt=t;
    Pointer p=tex_global_p;
    if (tt == ' ')
      tt = spacetoken;
    else
      tt += othertoken;
    FAST_STORE_NEW_TOKEN(p,tt);
    tex_global_p=p;
}

Static HalfWord strtoks(StrPoolPtr b)
{
  Pointer p;
  str_room(1);
  p = temphead;
  link(p) = 0;
  tex_global_p=p;
  str_map_from_mark(b, strtoks_helper);
  p=tex_global_p;
  return p;
}
/*:464*/

/*465:*/
Static HalfWord thetoks(void)
{
  enum Selector old_setting;
  Pointer p, r;

  getxtoken();
  scansomethinginternal(tokval, false);
  if (curvallevel >= identval) {   /*466:*/
    p = temphead;
    link(p) = 0;
    if (curvallevel == identval) {
      STORE_NEW_TOKEN(p,cstokenflag + curval);
      return p;
    }
    if (curval == 0)
      return p;
    r = link(curval);
    while (r != 0) {
      FAST_STORE_NEW_TOKEN(p,info(r));
      r = link(r);
    }
    return p;
  } else {
    StrPoolPtr b = str_mark();
    old_setting = selector;
    selector = NEW_STRING;
    switch (curvallevel) {

    case intval:
      print_int(curval);
      break;

    case dimenval:
      print_scaled(curval);
      print(S(459));
      break;

    case glueval:
      printspec(curval, S(459));
      deleteglueref(curval);
      break;

    case muval:
      printspec(curval, S(390));
      deleteglueref(curval);
      break;
    }
    selector = old_setting;
/*	fprintf(stderr,"(%d %d)",bb-b,pool_ptr-b); */
    return (strtoks(b));
  }
  /*:466*/
}
/*:465*/

/*467:*/
Static void insthetoks(void)
{
  link(garbage) = thetoks();
  inslist(link(temphead));
}  /*:467*/


/*470:*/
Static void convtoks(void)
{
  enum Selector old_setting;
  char c;
  SmallNumber savescannerstatus;
  StrPoolPtr b;

  c = curchr;   /*471:*/
  switch (c) {   /*:471*/

  case numbercode:
  case romannumeralcode:
    scanint();
    break;

  case stringcode:
  case meaningcode:
    savescannerstatus = scannerstatus;
    scannerstatus = normal;
    gettoken();
    scannerstatus = savescannerstatus;
    break;

  case fontnamecode:
    scanfontident();
    break;

  case jobnamecode:
    if (jobname == 0)
      openlogfile();
    break;
  }
  old_setting = selector;
  selector = NEW_STRING;
  b = str_mark();
  /*472:*/
  switch (c) {   /*:472*/

  case numbercode:
    print_int(curval);
    break;

  case romannumeralcode:
    print_roman_int(curval);
    break;

  case stringcode:
    if (curcs != 0)
      sprint_cs(curcs);
    else
      print_char(curchr);
    break;

  case meaningcode:
    printmeaning(curchr, curcmd);
    break;

  case fontnamecode:
    print(get_fontname(curval));
    if (get_fontsize(curval) != get_fontdsize(curval)) {
      print(S(642));
      print_scaled(get_fontsize(curval));
      print(S(459));
    }
    break;

  case jobnamecode:
    print(jobname);
    break;
  }
  selector = old_setting;
  link(garbage) = strtoks(b);
  inslist(link(temphead));
}  /*:470*/

/*473:*/
Static HalfWord scantoks(Boolean macrodef, Boolean xpand)
{
  HalfWord t, s, unbalance, hashbrace;
  Pointer p;

  if (macrodef)
    scannerstatus = defining;
  else
    scannerstatus = absorbing;
  warningindex = curcs;
  defref = get_avail();
  tokenrefcount(defref) = 0;
  p = defref;
  hashbrace = 0;
  t = zerotoken;
  if (macrodef) {   /*474:*/
    while (true) {
      gettoken();
      if (curtok < rightbracelimit)
	goto _Ldone1;
      if (curcmd == macparam) {   /*476:*/
	s = matchtoken + curchr;
	gettoken();
	if (curcmd == leftbrace) {
	  hashbrace = curtok;
	  STORE_NEW_TOKEN(p,curtok);
	  STORE_NEW_TOKEN(p,endmatchtoken);
	  goto _Ldone;
	}
	if (t == zerotoken + 9) {
	  printnl(S(292));
	  print(S(643));
	  help1(S(644));
	  error();
	} else {
	  t++;
	  if (curtok != t) {
	    printnl(S(292));
	    print(S(645));
	    help2(
              S(646),
              S(647));
	    backerror();
	  }
	  curtok = s;
	}
      }
      /*:476*/
      STORE_NEW_TOKEN(p,curtok);
    }
_Ldone1:
    STORE_NEW_TOKEN(p,endmatchtoken);
    if (curcmd == rightbrace) {   /*475:*/
      printnl(S(292));
      print(S(566));
      alignstate++;
      help2(
        S(648),
        S(649));
      error();
      goto _Lfound;
    }
    /*:475*/
_Ldone: ;
  } else
    scanleftbrace();
  /*:474*/
  /*477:*/
  unbalance = 1;
  while (true) {   /*:477*/
    if (xpand) {   /*478:*/
      while (true) {
	getnext();
	if (curcmd <= maxcommand)
	  goto _Ldone2;
	if (curcmd != the)
	  expand();
	else {
	  Pointer q = thetoks();
	  if (link(temphead) != 0) {
	    link(p) = link(temphead);
	    p = q;
	  }
	}
      }
_Ldone2:
      xtoken();
    } else
      gettoken();
    /*:478*/
    if (curtok < rightbracelimit) {
      if (curcmd < rightbrace)
	unbalance++;
      else {
	unbalance--;
	if (unbalance == 0)
	  goto _Lfound;
      }
    } else if (curcmd == macparam) {
      if (macrodef) {   /*479:*/
	s = curtok;
	if (xpand)
	  getxtoken();
	else
	  gettoken();
	if (curcmd != macparam) {
	  if (curtok <= zerotoken || curtok > t) {
	    printnl(S(292));
	    print(S(650));
	    sprint_cs(warningindex);
	    help3(
              S(651),
              S(652),
              S(653));
	    backerror();
	    curtok = s;
	  } else
	    curtok = outparamtoken - '0' + curchr;
	}
      }
      /*:479*/
    }
    STORE_NEW_TOKEN(p,curtok);
  }
_Lfound:
  scannerstatus = normal;
  if (hashbrace != 0) {
    STORE_NEW_TOKEN(p,hashbrace);
  }
  return p;
}  /*:473*/


/*482:*/
Static void readtoks(long n, HalfWord r) {
    Pointer p;
    long s;
    /* SmallNumber */ int m; /* INT */

    scannerstatus = defining;
    warningindex = r;
    defref = get_avail();
    tokenrefcount(defref) = 0;
    p = defref;
    STORE_NEW_TOKEN(p, endmatchtoken);
    if ((unsigned long)n > 15)
        m = 16;
    else
        m = n;
    s = alignstate;
    alignstate = 1000000L;
    do { /*483:*/
        beginfilereading();
        name = m + 1;
        if (readopen[m] == closed) { /*484:*/
            if (interaction > nonstopmode) {
                if (n < 0) {
                    print(S(385));
                    term_input();
                } else {
                    println();
                    sprint_cs(r);
                    print('=');
                    term_input();
                    n = -1;
                }
            } else /*:484*/
                fatalerror(S(654));
        } else if (readopen[m] == justopen) {
            if (inputln(readfile[m], false))
                readopen[m] = normal;
            else { /*:485*/
                aclose(&readfile[m]);
                readopen[m] = closed;
            }
        } else {
            if (!inputln(readfile[m], true)) {
                aclose(&readfile[m]);
                readopen[m] = closed;
                if (alignstate != 1000000L) {
                    runaway();
                    printnl(S(292));
                    print(S(655));
                    print_esc(S(656));
                    help1(S(657));
                    alignstate = 1000000L;
                    error();
                }
            }
        }
        limit = last;
        if (endlinecharinactive) {
            limit--;
        } else
            buffer[limit] = endlinechar;
        first = limit + 1;
        loc = start;
        state = newline;
        while (true) {
            gettoken();
            if (curtok == 0) goto _Ldone;
            if (alignstate < 1000000L) {
                do {
                    gettoken();
                } while (curtok != 0);
                alignstate = 1000000L;
                goto _Ldone;
            }
            STORE_NEW_TOKEN(p, curtok);
        }
_Ldone: /*:483*/    
        endfilereading();
    } while (alignstate != 1000000L);
    curval = defref;
    scannerstatus = normal;
    alignstate = s;

    /*485:*/
}
/*:482*/

/*494:*/
Static void passtext(void) {
    long l;
    SmallNumber savescannerstatus;

    savescannerstatus = scannerstatus;
    scannerstatus = skipping;
    l = 0;
    skipline = line;
    while (true) {
        getnext();
        if (curcmd == fiorelse) {
            if (l == 0) break;
            if (curchr == ficode) l--;
        } else if (curcmd == iftest) {
            l++;
        }
    }
    scannerstatus = savescannerstatus;
}
/*:494*/

/*497:*/
Static void changeiflimit(SmallNumber l, HalfWord p) {
    Pointer q;

    if (p == condptr)
        iflimit = l;
    else {
        q = condptr;
        while (true) {
            if (q == 0) confusion(S(658));
            if (link(q) == p) {
                type(q) = l;
                return;
            }
            q = link(q);
        }
    }
}

#define getxtokenoractivechar()                                                \
    (getxtoken(),                                                              \
     ((curcmd == relax) && (curchr == noexpandflag))                           \
         ? (curcmd = activechar, cur_chr = curtok - cstokenflag - activebase)  \
         : (cur_chr = curchr))

/*:497*/

/*498:*/
Static void conditional(void)
{  /*495:*/
  Boolean b=false /* XXXX */;
  long r;
  long m, n;
  Pointer p, q, savecondptr;
  SmallNumber savescannerstatus, thisif;

  p = getnode(ifnodesize);
  link(p) = condptr;
  type(p) = iflimit;
  subtype(p) = curif;
  iflinefield(p) = ifline;
  condptr = p;
  curif = curchr;
  iflimit = ifcode;
  ifline = line;   /*:495*/
  savecondptr = condptr;
  thisif = curchr;   /*501:*/
  switch (thisif) {   /*:501*/

  case ifcharcode:
  case ifcatcode:   /*506:*/
    {
    int cur_chr = curchr;
    getxtokenoractivechar();
    if (curcmd > activechar || cur_chr > 255) {
      m = relax;
      n = 256;
    } else {
      m = curcmd;
      n = cur_chr;
    }
    getxtokenoractivechar();
    if (curcmd > activechar || cur_chr > 255) {
      curcmd = relax;
      cur_chr = 256;
    }
    if (thisif == ifcharcode)
      b = (n == cur_chr);
    else
      b = (m == curcmd);
    }
    break;
    /*:506*/

  case ifintcode:
  case ifdimcode:   /*503:*/
    if (thisif == ifintcode)
      scanint();
    else {
      scannormaldimen();
    }
    n = curval;
    skip_spaces();
    if ((curtok >= othertoken + '<') & (curtok <= othertoken + '>'))
      r = curtok - othertoken;
    else {
      printnl(S(292));
      print(S(659));
      printcmdchr(iftest, thisif);
      help1(S(660));
      backerror();
      r = '=';
    }
    if (thisif == ifintcode)
      scanint();
    else {
      scannormaldimen();
    }
    switch (r) {

    case '<':
      b = (n < curval);
      break;

    case '=':
      b = (n == curval);
      break;

    case '>':
      b = (n > curval);
      break;
    }
    break;
    /*:503*/

  case ifoddcode:   /*504:*/
    scanint();
    b = curval & 1;
    break;
    /*:504*/

  case ifvmodecode:
    b = (labs(mode) == vmode);
    break;

  case ifhmodecode:
    b = (labs(mode) == hmode);
    break;

  case ifmmodecode:
    b = (labs(mode) == mmode);
    break;

  case ifinnercode:
    b = (mode < 0);
    break;

  case ifvoidcode:
  case ifhboxcode:
  case ifvboxcode:   /*505:*/
    scaneightbitint();
    p = box(curval);
    if (thisif == ifvoidcode)
      b = (p == 0);
    else if (p == 0)
      b = false;
    else if (thisif == ifhboxcode)
      b = (type(p) == hlistnode);
    else
      b = (type(p) == vlistnode);
    break;
    /*:505*/

  case ifxcode:   /*507:*/
    savescannerstatus = scannerstatus;
    scannerstatus = normal;
    getnext();
    n = curcs;
    p = curcmd;
    q = curchr;
    getnext();
    if (curcmd != p)
      b = false;
    else if (curcmd < call)
      b = (curchr == q);
    else {
      /* 508:*/
      p = link(curchr);
      q = link(equiv(n));
      if (p == q)
	b = true;
      else {
	while (p != 0 && q != 0) {
	  if (info(p) != info(q))
	    p = 0;
	  else {
	    p = link(p);
	    q = link(q);
	  }
	}
	b = (p == 0 && q == 0);
      }
    }
    scannerstatus = savescannerstatus;
    break;
    /*:507*/

  case ifeofcode:
    scanfourbitint();
    b = (readopen[curval] == closed);
    break;

  case iftruecode:
    b = true;
    break;

  case iffalsecode:
    b = false;
    break;

  case ifcasecode:   /*509:*/
    scanint();
    n = curval;
    if (tracingcommands > 1) {
      begindiagnostic();
      print(S(661));
      print_int(n);
      print_char('}');
      enddiagnostic(false);
    }
    while (n != 0) {
      passtext();
      if (condptr == savecondptr) {
	if (curchr != orcode)
	  goto _Lcommonending;
	n--;
	continue;
      }
      if (curchr != ficode)   /*496:*/
	continue;
      /*:496*/
      p = condptr;
      ifline = iflinefield(p);
      curif = subtype(p);
      iflimit = type(p);
      condptr = link(p);
      freenode(p, ifnodesize);
    }
    changeiflimit(orcode, savecondptr);
    goto _Lexit;
    break;
    /*:509*/
  }
  if (tracingcommands > 1) {   /*502:*/
    begindiagnostic();
    if (b)
      print(S(662));
    else
      print(S(663));
    enddiagnostic(false);
  }
  /*:502*/
  if (b) {
    changeiflimit(elsecode, savecondptr);
    goto _Lexit;
  }  /*500:*/
  while (true) {   /*:500*/
    passtext();
    if (condptr == savecondptr) {
      if (curchr != orcode)
	goto _Lcommonending;
      printnl(S(292));
      print(S(558));
      print_esc(S(664));
      help1(S(559));
      error();
      continue;
    }
    if (curchr != ficode)   /*496:*/
      continue;
    /*:496*/
    p = condptr;
    ifline = iflinefield(p);
    curif = subtype(p);
    iflimit = type(p);
    condptr = link(p);
    freenode(p, ifnodesize);
  }
_Lcommonending:
  if (curchr == ficode) {   /*496:*/
    p = condptr;
    ifline = iflinefield(p);
    curif = subtype(p);
    iflimit = type(p);
    condptr = link(p);
    freenode(p, ifnodesize);
  }  /*:496*/
  else
    iflimit = ficode;
_Lexit: ;

  /*:508*/
}
/*:498*/

/*515:*/
Static void beginname(void)
{
  extdelimiter = 0;
}
/*:515*/

/*516:*/
Static Boolean morename(ASCIICode c)
{
  if (c == ' ')
    return false;
  else {
    str_room(1);
    if (c == '.' && extdelimiter == 0) {
	extdelimiter = makestring();
    }
    append_char(c);
    return true;
  }
}
/*:516*/

/*517:*/
Static void endname(void)
{
  curarea = S(385);
  if (extdelimiter == 0) {
	curext = S(385);
	curname = makestring();
  } else {
	curname = extdelimiter;
	curext = makestring();
  }
}

long filename_k;
Static void appendtoname(StrASCIICode x)
{
	filename_k++;
	if (filename_k<=filenamesize) {
           nameoffile[filename_k-1]=xchr[x];
	}
}
/*:517*/

/*519:*/
void packfilename(StrNumber n, StrNumber a, StrNumber e)
{
  long k;

  k = 0;
  filename_k = 0;
  str_map(a,appendtoname);
  str_map(n,appendtoname);
  str_map(e,appendtoname);
  k = filename_k;
  if (k <= filenamesize)
    namelength = k;
  else
    namelength = filenamesize;
  for (k = namelength; k < filenamesize; k++)
    nameoffile[k] = ' ';
}  /*:519*/

/*525:*/
Static StrNumber makenamestring(void)
{
    int k;
    for (k = 0; k < namelength; k++) {
        append_char(xord[nameoffile[k]]);
    }
    return (makestring());
}


Static StrNumber amakenamestring(void)
{
  return (makenamestring());
}


Static StrNumber bmakenamestring(void)
{
  return (makenamestring());
}


Static StrNumber wmakenamestring()
{
  return (makenamestring());
}
/*:525*/

/*526:*/
Static void scanfilename(void) {
    nameinprogress = true;
    beginname();
    skip_spaces();
    while (true) {
        if (curcmd > otherchar || curchr > 255) {
            backinput();
            break;
        }
        if (!morename(curchr)) break;
        getxtoken();
    }
    endname();
    nameinprogress = false;
}
/*:526*/

/*529:*/
Static void packjobname(StrNumber s)
{
  curarea = S(385);
  curext = s;
  curname = jobname;
  packfilename(curname,curarea,curext);
}
/*:529*/

/*530:*/
Static void promptfilename(StrNumber s, StrNumber e) {
    short k;

    if (s == S(665)) {
        printnl(S(292));
        print(S(666));
    } else {
        printnl(S(292));
        print(S(667));
    }
    print_file_name(curname, curarea, curext);
    print(S(668));
    if (e == S(669)) showcontext();
    printnl(S(670));
    print(s);
    if (interaction < scrollmode) fatalerror(S(671));
    print(S(488));
    term_input(); /*531:*/
    beginname();
    k = first;
    while (buffer[k] == ' ' && k < last)
        k++;
    while (true) {
        if (k == last) break;
        if (!morename(buffer[k])) break;
        k++;
    }
    endname(); /*:531*/
    if (curext == S(385)) curext = e;
    packfilename(curname, curarea, curext);
}
/*:530*/

/*534:*/
Static void openlogfile(void) {
    enum Selector old_setting;
    short k;
    short l;
    Char months[36];
    short FORLIM;

    old_setting = selector;
    if (jobname == 0) jobname = S(672);
    packjobname(S(673));
    while (!a_open_out(&log_file)) { /*535:*/
        selector = TERM_ONLY;
        promptfilename(S(674), S(673));
    }
    /*:535*/
    logname = amakenamestring();
    selector = LOG_ONLY;
    logopened = true;
    /*536:*/
    fprintf(log_file, "%s", banner);
    slow_print(formatident);
    print(S(675));
    print_int(day);
    print_char(' ');
    memcpy(months, "JANFEBMARAPRMAYJUNJULAUGSEPOCTNOVDEC", 36);
    FORLIM = month * 3;
    for (k = month * 3 - 3; k < FORLIM; k++)
        fwrite(&months[k], 1, 1, log_file);
    print_char(' ');
    print_int(year);
    print_char(' ');
    print_two(tex_time / 60);
    print_char(':');
    print_two(tex_time % 60); /*:536*/
    inputstack[inputptr] = curinput;
    printnl(S(676));
    l = inputstack[0].limitfield;
    if (buffer[l] == endlinechar) l--;
    for (k = 1; k <= l; k++)
        print(buffer[k]);
    println();
    selector = old_setting + 2;
}
/*:534*/

/*537:*/
Static void startinput(void) {
    scanfilename();
    if (curext == S(385)) curext = S(669);
    packfilename(curname, curarea, curext);
    while (true) {
        beginfilereading();
        if (a_open_in(&curfile)) break;
        if (curarea == S(385)) {
            packfilename(curname, S(677), curext);
            if (a_open_in(&curfile)) break;
        }
        endfilereading();
        promptfilename(S(665), S(669));
    }

    name = amakenamestring();
    if (jobname == 0) {
        jobname = curname;
        openlogfile();
    }
    if (term_offset + str_length(name) > MAX_PRINT_LINE - 2) {
        println();
    } else if (term_offset > 0 || file_offset > 0)
        print_char(' ');
    print_char('(');
    openparens++;
    slow_print(name);
    fflush(stdout);
    state = newline;

#if 0
  if (name == str_ptr - 1) {   /*538:*/
    flush_string();
    name = curname;
  }
#else
    name = curname;
#endif

    line = 1;
    inputln(curfile, false);
    firmuptheline();
    if (endlinecharinactive) {
        limit--;
    } else
        buffer[limit] = endlinechar;
    first = limit + 1;
    loc = start; /*:538*/
}
/*:537*/

/*581:*/
Static void charwarning(InternalFontNumber f, EightBits c)
{
  if (tracinglostchars <= 0)
    return;
  begindiagnostic();
  printnl(S(678));
  print(c);
  print(S(679));
  slow_print(get_fontname(f));
  print_char('!');
  enddiagnostic(false);
}
/*:581*/

/*582:*/
Static HalfWord newcharacter(InternalFontNumber f, EightBits c) {
    HalfWord Result;
    Pointer p;

    if (fontbc[f] <= c) {
        if (fontec[f] >= c) {
            if (charexists(charinfo(f, c))) {
                p = get_avail();
                font(p) = f;
                character(p) = c;
                Result = p;
                goto _Lexit;
            }
        }
    }
    charwarning(f, c);
    Result = 0;
_Lexit:
    return Result;
}
/*:582*/

/*618:*/
Static void vlistout(void);
/*:618*/

/*619:*/
/*1368:*/
Static void specialout(HalfWord p)
{
  enum Selector old_setting;

  synchh();
  synchv();
  old_setting = selector;
  selector = NEW_STRING;
  showtokenlist(link(writetokens(p)), 0, POOL_SIZE /* - pool_ptr */ );
  selector = old_setting;
  str_room(1);
  { int p_len=cur_length(); /* XXXX - Assumes byte=StrASCIICode */
#define xxx1            239
#define xxx4            242

    if (p_len< 256) {
	dviout(xxx1);
	dviout(p_len);
    } else {
	dviout(xxx4);
	dvifour(p_len);
    }
    str_cur_map(dviout_helper);
  }
}
/*:1368*/

/*1370:*/
Static void writeout(HalfWord p)
{   /*1371:*/
  enum Selector old_setting;
  long oldmode;
  /* SmallNumber */ int j; /* INT */
  Pointer q, r;

  q = get_avail();
  info(q) = rightbracetoken + '}';
  r = get_avail();
  link(q) = r;
  info(r) = endwritetoken;
  inslist(q);
  begintokenlist(writetokens(p), writetext);
  q = get_avail();
  info(q) = leftbracetoken + '{';
  inslist(q);
  oldmode = mode;
  mode = 0;
  curcs = writeloc;
  q = scantoks(false, true);
  gettoken();
  if (curtok != endwritetoken) {   /*1372:*/
    printnl(S(292));
    print(S(680));
    help2(S(681),
          S(682));
    error();
    do {
      gettoken();
    } while (curtok != endwritetoken);
  }
  /*:1372*/
  mode = oldmode;   /*:1371*/
  endtokenlist();
  old_setting = selector;
  j = writestream(p);
  if (writeopen[j])
    selector = j;
  else {
    if (j == 17 && selector == TERM_AND_LOG)
      selector = LOG_ONLY;
    printnl(S(385));
  }
  tokenshow(defref);
  println();
  flushlist(defref);
  selector = old_setting;
}
/*:1370*/

/*1373:*/
Static void outwhat(HalfWord p)
{
  /* SmallNumber */ int j; /* INT */

  switch (subtype(p)) {

  case opennode:
  case writenode:
  case closenode:   /*1374:*/
    if (!doingleaders) {   /*:1374*/
      j = writestream(p);
      if (subtype(p) == writenode)
	writeout(p);
      else {
	if (writeopen[j])
	  aclose(&write_file[j]);
	if (subtype(p) == closenode)
	  writeopen[j] = false;
	else if (j < 16) {
	  curname = openname(p);
	  curarea = openarea(p);
	  curext = openext(p);
	  if (curext == S(385))
	    curext = S(669);
	packfilename(curname,curarea,curext);
	  while (!a_open_out(&write_file[j]))
	    promptfilename(S(683), S(669));
	  writeopen[j] = true;
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
    confusion(S(684));
    break;
  }
}
/*:1373*/


Static void hlistout(void)
{
  Scaled baseline, leftedge, saveh, savev, leaderwd, lx, edge;
  Pointer thisbox, p, leaderbox;
  GlueOrd gorder;
  char gsign;
  long saveloc;
  Boolean outerdoingleaders;
  double gluetemp;

  thisbox = tempptr;
  gorder = glueorder(thisbox);
  gsign = gluesign(thisbox);
  p = listptr(thisbox);
  curs++;
  if (curs > 0) {
    dvi_push();
  }
  if (curs > maxpush)
    maxpush = curs;
  saveloc = get_dvi_mark();
  baseline = curv;
  leftedge = curh;
  while (p != 0) {   /*620:*/
_Lreswitch:
    if (ischarnode(p)) {
      synchh();
      synchv();
      do {
	QuarterWord c;
	QuarterWord f = font(p);
	c = character(p);
	if (f != dvif) {   /*621:*/
	  dvi_set_font(f);
	  dvif = f;
	}
	/*:621*/
	dvi_set_char(c);
	curh += charwidth(f, charinfo(f, c));
	p = link(p);
      } while (ischarnode(p));
      dvih = curh;
      continue;
    }
    switch (type(p)) {

    case hlistnode:
    case vlistnode:   /*623:*/
      if (listptr(p) == 0)
	curh += width(p);
      else {   /*:623*/
	saveh = dvih;
	savev = dviv;
	curv = baseline + shiftamount(p);
	tempptr = p;
	edge = curh;
	if (type(p) == vlistnode)
	  vlistout();
	else
	  hlistout();
	dvih = saveh;
	dviv = savev;
	curh = edge + width(p);
	curv = baseline;
      }
      break;

    case rulenode:
      ruleht = height(p);
      ruledp = depth(p);
      rulewd = width(p);
      goto _Lfinrule_;
      break;

    case whatsitnode:   /*1367:*/
      outwhat(p);
      break;
      /*:1367*/

    case gluenode:   /*625:*/
      {Pointer g = glueptr(p);
      rulewd = width(g);
      if (gsign != normal) {
	if (gsign == stretching) {
	  if (stretchorder(g) == gorder) {
	    vetglue(glueset(thisbox) * stretch(g));
	    rulewd += (long)floor(gluetemp + 0.5);
	  }
	} else if (shrinkorder(g) == gorder) {
	  vetglue(glueset(thisbox) * shrink(g));
	  rulewd -= (long)floor(gluetemp + 0.5);
	}
      }
      }
      if (subtype(p) >= aleaders) {   /*626:*/
	leaderbox = leaderptr(p);
	if (type(leaderbox) == rulenode) {
	  ruleht = height(leaderbox);
	  ruledp = depth(leaderbox);
	  goto _Lfinrule_;
	}
	leaderwd = width(leaderbox);
	if (leaderwd > 0 && rulewd > 0) {
	  rulewd += 10;
	  edge = curh + rulewd;
	  lx = 0;   /*627:*/
	  if (subtype(p) == aleaders) {
	    saveh = curh;
	    curh = leftedge + leaderwd * ((curh - leftedge) / leaderwd);
	    if (curh < saveh)
	      curh += leaderwd;
	  } else {   /*:627*/
	    lq = rulewd / leaderwd;
	    lr = rulewd % leaderwd;
	    if (subtype(p) == cleaders)
	      curh += lr / 2;
	    else {
	      lx = (lr * 2 + lq + 1) / (lq * 2 + 2);
	      curh += (lr - (lq - 1) * lx) / 2;
	    }
	  }
	  while (curh + leaderwd <= edge) {   /*628:*/
	    curv = baseline + shiftamount(leaderbox);
	    synchv();
	    savev = dviv;
	    synchh();
	    saveh = dvih;
	    tempptr = leaderbox;
	    outerdoingleaders = doingleaders;
	    doingleaders = true;
	    if (type(leaderbox) == vlistnode)
	      vlistout();
	    else
	      hlistout();
	    doingleaders = outerdoingleaders;
	    dviv = savev;
	    dvih = saveh;
	    curv = baseline;
	    curh = saveh + leaderwd + lx;
	  }
	  /*:628*/
	  curh = edge - 10;
	  goto _Lnextp_;
	}
      }  /*:626*/
      goto _Lmovepast_;
      break;
      /*:625*/

    case kernnode:
    case mathnode:
      curh += width(p);
      break;

    case ligaturenode:   /*652:*/
	type(ligtrick) = charnodetype;
      font(ligtrick) = font_ligchar(p);
      character(ligtrick) = character_ligchar(p);
      link(ligtrick) = link(p);
      p = ligtrick;
      goto _Lreswitch;
      break;
      /*:652*/
    }
    goto _Lnextp_;
_Lfinrule_:   /*624:*/
    if (isrunning(ruleht)) {
      ruleht = height(thisbox);
    }
    if (isrunning(ruledp)) {
      ruledp = depth(thisbox);
    }
    ruleht += ruledp;
    if (ruleht > 0 && rulewd > 0) {   /*:624*/
      synchh();
      curv = baseline + ruledp;
      synchv();
      dvi_setrule(ruleht,rulewd);
      curv = baseline;
      dvih += rulewd;
    }
_Lmovepast_:
    curh += rulewd;
_Lnextp_:
    p = link(p);
  }
  prunemovements(saveloc);
  if (curs > 0)
    dvipop(saveloc);
  curs--;

  /*:620*/
}
/*:619*/

/*629:*/
Static void vlistout(void)
{
  Scaled leftedge, topedge, saveh, savev, leaderht, lx, edge;
  Pointer thisbox, p, leaderbox;
  GlueOrd gorder;
  char gsign;
  long saveloc;
  Boolean outerdoingleaders;
  double gluetemp;

  thisbox = tempptr;
  gorder = glueorder(thisbox);
  gsign = gluesign(thisbox);
  p = listptr(thisbox);
  curs++;
  if (curs > 0) {
    dvi_push();
  }
  if (curs > maxpush)
    maxpush = curs;
  saveloc = get_dvi_mark();
  leftedge = curh;
  curv -= height(thisbox);
  topedge = curv;
  while (p != 0) {   /*630:*/
    if (ischarnode(p))
      confusion(S(685));
    else  /*631:*/
    {   /*:631*/
      switch (type(p)) {

      case hlistnode:
      case vlistnode:   /*632:*/
	if (listptr(p) == 0)
	  curv += height(p) + depth(p);
	else {   /*:632*/
	  curv += height(p);
	  synchv();
	  saveh = dvih;
	  savev = dviv;
	  curh = leftedge + shiftamount(p);
	  tempptr = p;
	  if (type(p) == vlistnode)
	    vlistout();
	  else
	    hlistout();
	  dvih = saveh;
	  dviv = savev;
	  curv = savev + depth(p);
	  curh = leftedge;
	}
	break;

      case rulenode:
	ruleht = height(p);
	ruledp = depth(p);
	rulewd = width(p);
	goto _Lfinrule_;
	break;

      case whatsitnode:   /*1366:*/
	outwhat(p);
	break;
	/*:1366*/

      case gluenode:   /*634:*/
	{
	Pointer g = glueptr(p);
	ruleht = width(g);
	if (gsign != normal) {
	  if (gsign == stretching) {
	    if (stretchorder(g) == gorder) {
	      vetglue(glueset(thisbox) * stretch(g));
	      ruleht += (long)floor(gluetemp + 0.5);
	    }
	  } else if (shrinkorder(g) == gorder) {
	    vetglue(glueset(thisbox) * shrink(g));
	    ruleht -= (long)floor(gluetemp + 0.5);
	  }
	}
	}
	if (subtype(p) >= aleaders) {   /*635:*/
	  leaderbox = leaderptr(p);
	  if (type(leaderbox) == rulenode) {
	    rulewd = width(leaderbox);
	    ruledp = 0;
	    goto _Lfinrule_;
	  }
	  leaderht = height(leaderbox) + depth(leaderbox);
	  if (leaderht > 0 && ruleht > 0) {
	    ruleht += 10;
	    edge = curv + ruleht;
	    lx = 0;   /*636:*/
	    if (subtype(p) == aleaders) {
	      savev = curv;
	      curv = topedge + leaderht * ((curv - topedge) / leaderht);
	      if (curv < savev)
		curv += leaderht;
	    } else {   /*:636*/
	      lq = ruleht / leaderht;
	      lr = ruleht % leaderht;
	      if (subtype(p) == cleaders)
		curv += lr / 2;
	      else {
		lx = (lr * 2 + lq + 1) / (lq * 2 + 2);
		curv += (lr - (lq - 1) * lx) / 2;
	      }
	    }
	    while (curv + leaderht <= edge) {   /*637:*/
	      curh = leftedge + shiftamount(leaderbox);
	      synchh();
	      saveh = dvih;
	      curv += height(leaderbox);
	      synchv();
	      savev = dviv;
	      tempptr = leaderbox;
	      outerdoingleaders = doingleaders;
	      doingleaders = true;
	      if (type(leaderbox) == vlistnode)
		vlistout();
	      else
		hlistout();
	      doingleaders = outerdoingleaders;
	      dviv = savev;
	      dvih = saveh;
	      curh = leftedge;
	      curv = savev - height(leaderbox) + leaderht + lx;
	    }
	    /*:637*/
	    curv = edge - 10;
	    goto _Lnextp_;
	  }
	}
	/*:635*/
	goto _Lmovepast_;
	break;
	/*:634*/

      case kernnode:
	curv += width(p);
	break;
      }
      goto _Lnextp_;
_Lfinrule_:   /*633:*/
      if (isrunning(rulewd)) {
	rulewd = width(thisbox);
      }
      ruleht += ruledp;
      curv += ruleht;
      if (ruleht > 0 && rulewd > 0) {
	synchh();
	synchv();
	dvi_putrule( ruleht,  rulewd);
      }
      goto _Lnextp_;   /*:633*/
_Lmovepast_:
      curv += ruleht;
    }
_Lnextp_:
    p = link(p);
  }
  /*:630*/
  prunemovements(saveloc);
  if (curs > 0)
    dvipop(saveloc);
  curs--;
}
/*:629*/

/*638:*/
Static void shipout(HalfWord p)
{
  int j, k;
  enum Selector old_setting;

  if (tracingoutput > 0) {
    printnl(S(385));
    println();
    print(S(686));
  }
  if (term_offset > MAX_PRINT_LINE - 9)
    println();
  else if (term_offset > 0 || file_offset > 0)
    print_char(' ');
  print_char('[');
  j = 9;
  while (count(j) == 0 && j > 0)
    j--;
  for (k = 0; k <= j; k++) {
    print_int(count(k));
    if (k < j)
      print_char('.');
  }
  fflush(stdout);
  if (tracingoutput > 0) {   /*640:*/
    print_char(']');
    begindiagnostic();
    showbox(p);
    enddiagnostic(true);
  }
  /*641:*/
  if ((height(p) > maxdimen) | (depth(p) > maxdimen) |
      (height(p) + depth(p) + voffset > maxdimen) |
      (width(p) + hoffset > maxdimen)) {
    printnl(S(292));
    print(S(687));
    help2(S(688),
          S(689));
    error();
    if (tracingoutput <= 0) {
      begindiagnostic();
      printnl(S(690));
      showbox(p);
      enddiagnostic(true);
    }
    goto _Ldone;
  }
  if (height(p) + depth(p) + voffset > maxv)
    maxv = height(p) + depth(p) + voffset;
  if (width(p) + hoffset > maxh)
    maxh = width(p) + hoffset;   /*:641*/
  /*617:*/
  dvih = 0;
  dviv = 0;
  curh = hoffset;
  dvif = nullfont;
  if (outputfilename == 0) {
    if (jobname == 0)
      openlogfile();
    packjobname(S(691));
    while (!dvi_openout())
      promptfilename(S(692), S(691));
    outputfilename = bmakenamestring();
  }
  if (totalpages == 0) {   /*:617*/
    preparemag();
    dvi_pre(25400000L, 473628672L, mag);
    old_setting = selector;
    selector = NEW_STRING;
    print(S(693));
    print_int(year);
    print_char('.');
    print_two(month);
    print_char('.');
    print_two(day);
    print_char(':');
    print_two(tex_time / 60);
    print_two(tex_time % 60);
    selector = old_setting;
    dviout(cur_length()); /* XXXX */
    str_cur_map(dviout_helper);
  }
  {
    long cp[10];
    for (k = 0; k <= 9; k++) {
	cp[k]=count(k);
    }
    dvibop(cp);
  }
  curv = height(p) + voffset;
  tempptr = p;
  if (type(p) == vlistnode)
    vlistout();
  else
    hlistout();
  dvi_eop();
  totalpages++;
  curs = -1;

_Ldone: /*:640*/
    if (tracingoutput <= 0) print_char(']');
    deadcycles = 0;
    fflush(stdout);

/// p236#639
    #ifdef tt_STAT
        if (tracingstats > 1) {
            printnl(S(694));
            print_int(varused);
            print_char('&');
            print_int(dynused);
            print_char(';');
        }
    #endif // #639.1: tt_STAT

    flushnodelist(p); 

    #ifdef tt_STAT
        if (tracingstats > 1) {
            print(S(695));
            print_int(varused);
            print_char('&');
            print_int(dynused);
            print(S(696));
            print_int(himemmin - lomemmax - 1);
            println();
        }
    #endif // #639.2: tt_STAT
}  /*:638*/


/*645:*/
Static void scanspec(GroupCode c, Boolean threecodes)
{
  long s=0 /* XXXX */;
  char speccode;

  if (threecodes)
    s = saved(0);
  if (scankeyword(S(697)))
    speccode = exactly;
  else if (scankeyword(S(698)))
    speccode = additional;
  else {
    speccode = additional;
    curval = 0;
    goto _Lfound;
  }
  scannormaldimen();
_Lfound:
  if (threecodes) {
    saved(0) = s;
    saveptr++;
  }
  saved(0) = speccode;
  saved(1) = curval;
  saveptr += 2;
  newsavelevel(c);
  scanleftbrace();
}
/*:645*/

/*649:*/
Static HalfWord hpack(HalfWord p, long w, SmallNumber m)
{
  Pointer r, q, g;
  Scaled h, d, x, s;
  GlueOrd o;
  EightBits hd;

  lastbadness = 0;
  r = getnode(boxnodesize);
  type(r) = hlistnode;
  subtype(r) = minquarterword;
  shiftamount(r) = 0;
  q = r + listoffset;
  link(q) = p;
  h = 0;   /*650:*/
  d = 0;
  x = 0;
  totalstretch[0] = 0;
  totalshrink[0] = 0;
  totalstretch[fil - normal] = 0;
  totalshrink[fil - normal] = 0;
  totalstretch[fill - normal] = 0;
  totalshrink[fill - normal] = 0;
  totalstretch[filll - normal] = 0;
  totalshrink[filll - normal] = 0;   /*:650*/
  while (p != 0) {   /*651:*/
_Lreswitch:
    while (ischarnode(p)) {   /*654:*/
      InternalFontNumber f = font(p);
      FourQuarters i = charinfo(f, character(p));
      hd = heightdepth(i);
      x += charwidth(f, i);
      s = charheight(f, hd);
      if (s > h)
	h = s;
      s = chardepth(f, hd);
      if (s > d)
	d = s;
      p = link(p);
    }
    /*:654*/
    if (p == 0)
      break;
    switch (type(p)) {

    case hlistnode:
    case vlistnode:
    case rulenode:
    case unsetnode:   /*653:*/
      x += width(p);
      if (type(p) >= rulenode)
	s = 0;
      else
	s = shiftamount(p);
      if (height(p) - s > h)
	h = height(p) - s;
      if (depth(p) + s > d)
	d = depth(p) + s;
      break;
      /*:653*/

    case insnode:
    case marknode:
    case adjustnode:
      if (adjusttail != 0) {   /*655:*/
	while (link(q) != p)
	  q = link(q);
	if (type(p) == adjustnode) {
	  link(adjusttail) = adjustptr(p);
	  while (link(adjusttail) != 0)
	    adjusttail = link(adjusttail);
	  p = link(p);
	  freenode(link(q), smallnodesize);
	} else {
	  link(adjusttail) = p;
	  adjusttail = p;
	  p = link(p);
	}
	link(q) = p;
	p = q;
      }
      /*:655*/
      break;

    case whatsitnode:   /*1360:*/
      break;
      /*:1360*/

    case gluenode:   /*656:*/
      g = glueptr(p);
      x += width(g);
      o = stretchorder(g);
      totalstretch[o - normal] += stretch(g);
      o = shrinkorder(g);
      totalshrink[o - normal] += shrink(g);
      if (subtype(p) >= aleaders) {
	g = leaderptr(p);
	if (height(g) > h)
	  h = height(g);
	if (depth(g) > d)
	  d = depth(g);
      }
      break;
      /*:656*/

    case kernnode:
    case mathnode:
      x += width(p);
      break;

    case ligaturenode:   /*652:*/
	type(ligtrick) = charnodetype;
      font(ligtrick) = font_ligchar(p);
      character(ligtrick) = character_ligchar(p);
      link(ligtrick) = link(p);
      p = ligtrick;
      goto _Lreswitch;
      break;
      /*:652*/
    }
    p = link(p);
  }
  /*:651*/
  if (adjusttail != 0)
    link(adjusttail) = 0;
  height(r) = h;
  depth(r) = d;   /*657:*/
  if (m == additional)
    w += x;
  width(r) = w;
  x = w - x;
  if (x == 0) {
    gluesign(r) = normal;
    glueorder(r) = normal;
    glueset(r) = 0.0;
    goto _Lexit;
  } else if (x > 0) {
    if (totalstretch[filll - normal] != 0)
      o = filll;
    else if (totalstretch[fill - normal] != 0)
      o = fill;
    else if (totalstretch[fil - normal] != 0)
      o = fil;
    else {
      o = normal;
      /*:659*/
    }
    glueorder(r) = o;
    gluesign(r) = stretching;
    if (totalstretch[o - normal] != 0)
      glueset(r) = (double)x / totalstretch[o - normal];
    else {
      gluesign(r) = normal;
      glueset(r) = 0.0;
    }
    if (o == normal) {
      if (listptr(r) != 0) {   /*660:*/
	lastbadness = badness(x, totalstretch[0]);
	if (lastbadness > hbadness) {
	  println();
	  if (lastbadness > 100)
	    printnl(S(699));
	  else
	    printnl(S(700));
	  print(S(701));
	  print_int(lastbadness);
	  goto _Lcommonending;
	}
      }
      /*:660*/
    }
    goto _Lexit;
  } else {
    if (totalshrink[filll - normal] != 0)
      o = filll;
    else if (totalshrink[fill - normal] != 0)
      o = fill;
    else if (totalshrink[fil - normal] != 0)
      o = fil;
    else
      o = normal;   /*:665*/
    glueorder(r) = o;
    gluesign(r) = shrinking;
    if (totalshrink[o - normal] != 0)
      glueset(r) = (double)(-x) / totalshrink[o - normal];
    else {
      gluesign(r) = normal;
      glueset(r) = 0.0;
    }
    if ((totalshrink[o - normal] < -x && o == normal) & (listptr(r) != 0)) {
      lastbadness = 1000000L;
      glueset(r) = 1.0;   /*666:*/
      if (-x - totalshrink[0] > hfuzz || hbadness < 100) {   /*:666*/
	if (overfullrule > 0 && -x - totalshrink[0] > hfuzz) {
	  while (link(q) != 0)
	    q = link(q);
	  link(q) = newrule();
	  width(link(q)) = overfullrule;
	}
	println();
	printnl(S(702));
	print_scaled(-x - totalshrink[0]);
	print(S(703));
	goto _Lcommonending;
      }
    } else if (o == normal) {
      if (listptr(r) != 0) {   /*667:*/
	lastbadness = badness(-x, totalshrink[0]);
	if (lastbadness > hbadness) {
	  println();
	  printnl(S(704));
	  print_int(lastbadness);
	  goto _Lcommonending;
	}
      }
      /*:667*/
    }
    goto _Lexit;
  }
_Lcommonending:   /*663:*/
  if (outputactive)
    print(S(705));
  else {
    if (packbeginline != 0) {
      if (packbeginline > 0)
	print(S(706));
      else
	print(S(707));
      print_int(labs(packbeginline));
      print(S(708));
    } else
      print(S(709));
    print_int(line);
  }
  println();
  fontinshortdisplay = nullfont;
  shortdisplay(listptr(r));
  println();
  begindiagnostic();
  showbox(r);   /*:663*/
  enddiagnostic(true);
_Lexit:
  return r;

  /*:657*/
}
/*:649*/

/*668:*/
Static HalfWord vpackage(HalfWord p, long h, SmallNumber m, long l)
{
  Pointer r, g;
  Scaled w, d, x, s;
  GlueOrd o;

  lastbadness = 0;
  r = getnode(boxnodesize);
  type(r) = vlistnode;
  subtype(r) = minquarterword;
  shiftamount(r) = 0;
  listptr(r) = p;
  w = 0;   /*650:*/
  d = 0;
  x = 0;
  totalstretch[0] = 0;
  totalshrink[0] = 0;
  totalstretch[fil - normal] = 0;
  totalshrink[fil - normal] = 0;
  totalstretch[fill - normal] = 0;
  totalshrink[fill - normal] = 0;
  totalstretch[filll - normal] = 0;
  totalshrink[filll - normal] = 0;   /*:650*/
  while (p != 0) {   /*669:*/
    if (ischarnode(p))
      confusion(S(710));
    else {
      switch (type(p)) {

      case hlistnode:
      case vlistnode:
      case rulenode:
      case unsetnode:   /*670:*/
	x += d + height(p);
	d = depth(p);
	if (type(p) >= rulenode)
	  s = 0;
	else
	  s = shiftamount(p);
	if (width(p) + s > w)
	  w = width(p) + s;
	break;
	/*:670*/

      case whatsitnode:   /*1359:*/
	break;

      /*:1359*/
      case gluenode:   /*:671*/
	/*671:*/
	x += d;
	d = 0;
	g = glueptr(p);
	x += width(g);
	o = stretchorder(g);
	totalstretch[o - normal] += stretch(g);
	o = shrinkorder(g);
	totalshrink[o - normal] += shrink(g);
	if (subtype(p) >= aleaders) {
	  g = leaderptr(p);
	  if (width(g) > w)
	    w = width(g);
	}
	break;

      case kernnode:
	x += d + width(p);
	d = 0;
	break;
      }
    }
    p = link(p);
  }
  /*:669*/
  width(r) = w;
  if (d > l) {   /*672:*/
    x += d - l;
    depth(r) = l;
  } else
    depth(r) = d;
  if (m == additional)
    h += x;
  height(r) = h;
  x = h - x;
  if (x == 0) {
    gluesign(r) = normal;
    glueorder(r) = normal;
    glueset(r) = 0.0;
    goto _Lexit;
  } else if (x > 0) {
    if (totalstretch[filll - normal] != 0)
      o = filll;
    else if (totalstretch[fill - normal] != 0)
      o = fill;
    else if (totalstretch[fil - normal] != 0)
      o = fil;
    else {
      o = normal;
      /*:659*/
    }
    glueorder(r) = o;
    gluesign(r) = stretching;
    if (totalstretch[o - normal] != 0)
      glueset(r) = (double)x / totalstretch[o - normal];
    else {
      gluesign(r) = normal;
      glueset(r) = 0.0;
    }
    if (o == normal) {
      if (listptr(r) != 0) {   /*674:*/
	lastbadness = badness(x, totalstretch[0]);
	if (lastbadness > vbadness) {
	  println();
	  if (lastbadness > 100)
	    printnl(S(699));
	  else
	    printnl(S(700));
	  print(S(711));
	  print_int(lastbadness);
	  goto _Lcommonending;
	}
      }
      /*:674*/
    }
    goto _Lexit;
  } else {
    if (totalshrink[filll - normal] != 0)
      o = filll;
    else if (totalshrink[fill - normal] != 0)
      o = fill;
    else if (totalshrink[fil - normal] != 0)
      o = fil;
    else
      o = normal;   /*:665*/
    glueorder(r) = o;
    gluesign(r) = shrinking;
    if (totalshrink[o - normal] != 0)
      glueset(r) = (double)(-x) / totalshrink[o - normal];
    else {
      gluesign(r) = normal;
      glueset(r) = 0.0;
    }
    if ((totalshrink[o - normal] < -x && o == normal) & (listptr(r) != 0)) {
      lastbadness = 1000000L;
      glueset(r) = 1.0;   /*677:*/
      if (-x - totalshrink[0] > vfuzz || vbadness < 100) {   /*:677*/
	println();
	printnl(S(712));
	print_scaled(-x - totalshrink[0]);
	print(S(713));
	goto _Lcommonending;
      }
    } else if (o == normal) {
      if (listptr(r) != 0) {   /*678:*/
	lastbadness = badness(-x, totalshrink[0]);
	if (lastbadness > vbadness) {
	  println();
	  printnl(S(714));
	  print_int(lastbadness);
	  goto _Lcommonending;
	}
      }
      /*:678*/
    }
    goto _Lexit;
  }
_Lcommonending:   /*675:*/
  if (outputactive)
    print(S(705));
  else {
    if (packbeginline != 0) {
      print(S(707));
      print_int(labs(packbeginline));
      print(S(708));
    } else
      print(S(709));
    print_int(line);
    println();
  }
  begindiagnostic();
  showbox(r);   /*:675*/
  enddiagnostic(true);
_Lexit:
  return r;

  /*:672*/
}
/*:668*/

/*679:*/
Static void appendtovlist(HalfWord b)
{
  Scaled d;
  Pointer p;

  if (prevdepth > ignoredepth) {
    d = width(baselineskip) - prevdepth - height(b);
    if (d < lineskiplimit)
      p = newparamglue(lineskipcode);
    else {
      p = newskipparam(baselineskipcode);
      width(tempptr) = d;
    }
    link(tail) = p;
    tail = p;
  }
  link(tail) = b;
  tail = b;
  prevdepth = depth(b);
}
/*:679*/

/*686:*/
Static HalfWord newnoad(void)
{
  Pointer p;
  int i=0;
  p = getnode(noadsize);
  type(p) = ordnoad;
  subtype(p) = normal;
#ifdef BIG_CHARNODE
  for(i=0;i<charnodesize;i++) {
#endif
  mem[nucleus(p) + i - memmin].hh = emptyfield;
  mem[subscr(p) + i - memmin].hh = emptyfield;
  mem[supscr(p) + i - memmin].hh = emptyfield;
#ifdef BIG_CHARNODE
 }
#endif
  return p;
}
/*:686*/

/*688:*/
Static HalfWord newstyle(SmallNumber s)
{
  Pointer p;

  p = getnode(stylenodesize);
  type(p) = stylenode;
  subtype(p) = s;
  width(p) = 0;
  depth(p) = 0;
  return p;
}  /*:688*/


/*689:*/
Static HalfWord newchoice(void)
{
  Pointer p;

  p = getnode(stylenodesize);
  type(p) = choicenode;
  subtype(p) = 0;
  displaymlist(p) = 0;
  textmlist(p) = 0;
  scriptmlist(p) = 0;
  scriptscriptmlist(p) = 0;
  return p;
}
/*:689*/

/*693:*/
Static void showinfo(void)
{
  shownodelist(info(tempptr));
}
/*:693*/

/*704:*/
Static HalfWord fractionrule(long t)
{
  Pointer p;

  p = newrule();
  height(p) = t;
  depth(p) = 0;
  return p;
}
/*:704*/

/*705:*/
Static HalfWord overbar(HalfWord b, long k, long t)
{
  Pointer p, q;

  p = newkern(k);
  link(p) = b;
  q = fractionrule(t);
  link(q) = p;
  p = newkern(t);
  link(p) = q;
  return (vpack(p, 0, additional));
}
/*:705*/

/*706:*/
/*709:*/
Static HalfWord charbox(InternalFontNumber f, QuarterWord c)
{
  FourQuarters q;
  EightBits hd;
  Pointer b, p;

  q = charinfo(f, c);
  hd = heightdepth(q);
  b = newnullbox();
  width(b) = charwidth(f, q) + charitalic(f, q);
  height(b) = charheight(f, hd);
  depth(b) = chardepth(f, hd);
  p = get_avail();
  character(p) = c;
  font(p) = f;
  listptr(b) = p;
  return b;
}
/*:709*/

/*711:*/
Static void stackintobox(HalfWord b, InternalFontNumber f, QuarterWord c)
{
  Pointer p;

  p = charbox(f, c);
  link(p) = listptr(b);
  listptr(b) = p;
  height(b) = height(p);
}
/*:711*/

/*712:*/
Static long heightplusdepth(InternalFontNumber f, QuarterWord c)
{
  FourQuarters q;
  EightBits hd;

  q = charinfo(f, c);
  hd = heightdepth(q);
  return (charheight(f, hd) + chardepth(f, hd));
}  /*:712*/


Static HalfWord vardelimiter(HalfWord d, SmallNumber s, long v)
{
  Pointer b;
  InternalFontNumber f, g;
  QuarterWord c=0 /* XXXX */, x, y;
  long m, n;
  Scaled u, w;
  FourQuarters q;
  EightBits hd;
  FourQuarters r;
  SmallNumber z;
  Boolean largeattempt;

  f = nullfont;
  w = 0;
  largeattempt = false;
  z = smallfam(d);
  x = smallchar(d);
  while (true) {  /*707:*/
    if (z != 0 || x != minquarterword) {   /*:707*/
      z += s + 16;
      do {
	z -= 16;
	g = famfnt(z);
	if (g != nullfont) {   /*708:*/
	  y = x;
	  if (y - minquarterword >= fontbc[g ] &&
	      y - minquarterword <= fontec[g ]) {
_Llabcontinue:
	    q = charinfo(g, y);
	    if (charexists(q)) {
	      if (chartag(q) == exttag) {
		f = g;
		c = y;
		goto _Lfound;
	      }
	      hd = heightdepth(q);
	      u = charheight(g, hd) + chardepth(g, hd);
	      if (u > w) {
		f = g;
		c = y;
		w = u;
		if (u >= v)
		  goto _Lfound;
	      }
	      if (chartag(q) == listtag) {
		y = rembyte(q);
		goto _Llabcontinue;
	      }
	    }
	  }
	}
	/*:708*/
      } while (z >= 16);
    }
    if (largeattempt)
      goto _Lfound;
    largeattempt = true;
    z = largefam(d);
    x = largechar(d);
  }
_Lfound:
  if (f != nullfont) {   /*710:*/
    if (chartag(q) == exttag) {   /*713:*/
      b = newnullbox();
      type(b) = vlistnode;
      r = exteninfo(f,q); /* fontinfo[extenbase[f ] + rembyte(q)].qqqq; */
  /*714:*/
      c = extrep(r);
      u = heightplusdepth(f, c);
      w = 0;
      q = charinfo(f, c);
      width(b) = charwidth(f, q) + charitalic(f, q);
      c = extbot(r);
      if (c != minquarterword)
	w += heightplusdepth(f, c);
      c = extmid(r);
      if (c != minquarterword)
	w += heightplusdepth(f, c);
      c = exttop(r);
      if (c != minquarterword)
	w += heightplusdepth(f, c);
      n = 0;
      if (u > 0) {
	while (w < v) {   /*:714*/
	  w += u;
	  n++;
	  if (extmid(r) != minquarterword)
	    w += u;
	}
      }
      c = extbot(r);
      if (c != minquarterword)
	stackintobox(b, f, c);
      c = extrep(r);
      for (m = 1; m <= n; m++)
	stackintobox(b, f, c);
      c = extmid(r);
      if (c != minquarterword) {
	stackintobox(b, f, c);
	c = extrep(r);
	for (m = 1; m <= n; m++)
	  stackintobox(b, f, c);
      }
      c = exttop(r);
      if (c != minquarterword)
	stackintobox(b, f, c);
      depth(b) = w - height(b);
    } else
      b = charbox(f, c);   /*:710*/
    /*:713*/
  } else {
    b = newnullbox();
    width(b) = nulldelimiterspace;
  }
  shiftamount(b) = half(height(b) - depth(b)) - axisheight(s);
  return b;
}
/*:706*/

/*715:*/
Static HalfWord rebox(HalfWord b, long w)
{
  Pointer p;
  InternalFontNumber f;
  Scaled v;

  if ((width(b) != w) & (listptr(b) != 0)) {
    if (type(b) == vlistnode)
      b = hpack(b, 0, additional);
    p = listptr(b);
    if (ischarnode(p) & (link(p) == 0)) {
      f = font(p);
      v = charwidth(f, charinfo(f, character(p)));
      if (v != width(b))
	link(p) = newkern(width(b) - v);
    }
    freenode(b, boxnodesize);
    b = newglue(ssglue);
    link(b) = p;
    while (link(p) != 0)
      p = link(p);
    link(p) = newglue(ssglue);
    return (hpack(b, w, exactly));
  } else {
    width(b) = w;
    return b;
  }
}
/*:715*/

/*716:*/
Static HalfWord mathglue(HalfWord g, long m)
{
  Pointer p;
  long n;
  Scaled f;

  n = x_over_n(m, 65536L);
  f = tex_remainder;
  if (f < 0) {
    n--;
    f += 65536L;
  }
  p = getnode(gluespecsize);
  width(p) = mult_and_add(n, width(g), xn_over_d(width(g), f, 65536L),
			    1073741823L);
  stretchorder(p) = stretchorder(g);
  if (stretchorder(p) == normal)
    stretch(p) = mult_and_add(n, stretch(g),
	xn_over_d(stretch(g), f, 65536L), 1073741823L);
  else
    stretch(p) = stretch(g);
  shrinkorder(p) = shrinkorder(g);
  if (shrinkorder(p) == normal)
    shrink(p) = mult_and_add(n, shrink(g),
			       xn_over_d(shrink(g), f, 65536L),
			       1073741823L);
  else
    shrink(p) = shrink(g);
  return p;
}
/*:716*/

/*717:*/
Static void mathkern(HalfWord p, long m)
{
  long n;
  Scaled f;

  if (subtype(p) != muglue)
    return;
  n = x_over_n(m, 65536L);
  f = tex_remainder;
  if (f < 0) {
    n--;
    f += 65536L;
  }
  width(p) = mult_and_add(n, width(p), xn_over_d(width(p), f, 65536L),
			    1073741823L);
  subtype(p) = explicit;
}
/*:717*/

/*718:*/
Static void flushmath(void)
{
  flushnodelist(link(head));
  flushnodelist(incompleatnoad);
  link(head) = 0;
  tail = head;
  incompleatnoad = 0;
}
/*:718*/

/*720:*/
Static void mlisttohlist(void);


Static HalfWord cleanbox(HalfWord p, SmallNumber s)
{
  Pointer q, x, r;
  SmallNumber savestyle;

  switch (mathtype(p)) {

  case mathchar:
    curmlist = newnoad();
    mem[nucleus(curmlist) - memmin] = mem[p - memmin];
    break;

  case subbox:
    q = info(p);
    goto _Lfound;
    break;

  case submlist:
    curmlist = info(p);
    break;

  default:
    q = newnullbox();
    goto _Lfound;
    break;
  }
  savestyle = curstyle;
  curstyle = s;
  mlistpenalties = false;
  mlisttohlist();
  q = link(temphead);
  curstyle = savestyle;   /*703:*/
  if (curstyle < scriptstyle)
    cursize = TEXT_SIZE;
  else
    cursize = (curstyle - textstyle) / 2 * 16;
  curmu = x_over_n(mathquad(cursize), 18);   /*:703*/
_Lfound:
  if (ischarnode(q) || q == 0)   /*721:*/
    x = hpack(q, 0, additional);
  else if ((link(q) == 0) & (type(q) <= vlistnode) &
	   (shiftamount(q) == 0))
    x = q;
  else
    x = hpack(q, 0, additional);
  q = listptr(x);
  if (!ischarnode(q))   /*:721*/
    return x;
  r = link(q);
  if (r == 0)
    return x;
  if (link(r) != 0)
    return x;
  if (ischarnode(r))
    return x;
  if (type(r) == kernnode) {
    freenode(r, smallnodesize);
    link(q) = 0;
  }
  return x;
}
/*:720*/

/*722:*/
Static void fetch(HalfWord a)
{
  curc = character(a);
  curf = famfnt(fam(a) + cursize);
  if (curf == nullfont) {   /*723:*/
    printnl(S(292));
    print(S(385));
    print_size(cursize);
    print_char(' ');
    print_int(fam(a));
    print(S(715));
    print(curc - minquarterword);
    print_char(')');
    help4(S(716),
          S(717),
          S(718),
          S(719));
    error();
    curi = nullcharacter;
    mathtype(a) = empty;
    return;
  }  /*:723*/
  if (curc - minquarterword >= fontbc[curf ] &&
      curc - minquarterword <= fontec[curf ])
    curi = charinfo(curf, curc);
  else
    curi = nullcharacter;
  if (!charexists(curi)) {
    charwarning(curf, curc - minquarterword);
    mathtype(a) = empty;
  }
}
/*:722*/

/*726:*/
/*734:*/
Static void makeover(HalfWord q)
{
  info(nucleus(q)) = overbar(
      cleanbox(nucleus(q), crampedstyle(curstyle)),
      defaultrulethickness * 3, defaultrulethickness);
  mathtype(nucleus(q)) = subbox;
}
/*:734*/

/*735:*/
Static void makeunder(HalfWord q)
{
  Pointer p, x, y;
  Scaled delta;

  x = cleanbox(nucleus(q), curstyle);
  p = newkern(defaultrulethickness * 3);
  link(x) = p;
  link(p) = fractionrule(defaultrulethickness);
  y = vpack(x, 0, additional);
  delta = height(y) + depth(y) + defaultrulethickness;
  height(y) = height(x);
  depth(y) = delta - height(y);
  info(nucleus(q)) = y;
  mathtype(nucleus(q)) = subbox;
}
/*:735*/

/*736:*/
Static void makevcenter(HalfWord q)
{
  Pointer v;
  Scaled delta;

  v = info(nucleus(q));
  if (type(v) != vlistnode)
    confusion(S(415));
  delta = height(v) + depth(v);
  height(v) = axisheight(cursize) + half(delta);
  depth(v) = delta - height(v);
}
/*:736*/

/*737:*/
Static void makeradical(HalfWord q)
{
  Pointer x, y;
  Scaled delta, clr;

  x = cleanbox(nucleus(q), crampedstyle(curstyle));
  if (curstyle < textstyle)
    clr = defaultrulethickness + labs(mathxheight(cursize)) / 4;
  else {
    clr = defaultrulethickness;
    clr += labs(clr) / 4;
  }
  y = vardelimiter(leftdelimiter(q), cursize,
		   height(x) + depth(x) + clr + defaultrulethickness);
  delta = depth(y) - height(x) - depth(x) - clr;
  if (delta > 0)
    clr += half(delta);
  shiftamount(y) = -(height(x) + clr);
  link(y) = overbar(x, clr, height(y));
  info(nucleus(q)) = hpack(y, 0, additional);
  mathtype(nucleus(q)) = subbox;
}
/*:737*/

/*738:*/
Static void makemathaccent(HalfWord q)
{
  Pointer p, x, y;
  long a;
  QuarterWord c;
  InternalFontNumber f;
  FourQuarters i;
  Scaled s, h, delta, w;

  fetch(accentchr(q));
  if (!charexists(curi))
    return;
  i = curi;
  c = curc;
  f = curf;   /*741:*/
  s = 0;
  if (mathtype(nucleus(q)) == mathchar) {
    fetch(nucleus(q));
    if (chartag(curi) == ligtag) {
      a = ligkernstart(curf,curi);
      curi = fontinfo[a].qqqq;
      if (skipbyte(curi) > stopflag) {
	a = ligkernrestart(curf,curi);
	curi = fontinfo[a].qqqq;
      }
      while (true) {
	if (nextchar(curi) - minquarterword == get_skewchar(curf)) {
	  if (opbyte(curi) >= kernflag) {
	    if (skipbyte(curi) <= stopflag)
	      s = charkern(curf, curi);
	  }
	  goto _Ldone1;
	}
	if (skipbyte(curi) >= stopflag)
	  goto _Ldone1;
	a += skipbyte(curi) - minquarterword + 1;
	curi = fontinfo[a].qqqq;
      }
    }
  }
_Ldone1:   /*:741*/
  x = cleanbox(nucleus(q), crampedstyle(curstyle));
  w = width(x);
  h = height(x);   /*740:*/
  while (true) {
    if (chartag(i) != listtag) {
      goto _Ldone;
    }
    y = rembyte(i);
    i = charinfo(f, y);
    if (!charexists(i))
      goto _Ldone;
    if (charwidth(f, i) > w)
      goto _Ldone;
    c = y;
  }
_Ldone:   /*:740*/
  if (h < xheight(f))
    delta = h;
  else
    delta = xheight(f);
  if ((mathtype(supscr(q)) != empty) |
      (mathtype(subscr(q)) != empty)) {
    if (mathtype(nucleus(q)) == mathchar) {   /*742:*/
      flushnodelist(x);
      x = newnoad();
      mem[nucleus(x) - memmin] = mem[nucleus(q) - memmin];
      mem[supscr(x) - memmin] = mem[supscr(q) - memmin];
      mem[subscr(x) - memmin] = mem[subscr(q) - memmin];
      mem[supscr(q) - memmin].hh = emptyfield;
      mem[subscr(q) - memmin].hh = emptyfield;
      mathtype(nucleus(q)) = submlist;
      info(nucleus(q)) = x;
      x = cleanbox(nucleus(q), curstyle);
      delta += height(x) - h;
      h = height(x);
    }
    /*:742*/
  }
  y = charbox(f, c);
  shiftamount(y) = s + half(w - width(y));
  width(y) = 0;
  p = newkern(-delta);
  link(p) = x;
  link(y) = p;
  y = vpack(y, 0, additional);
  width(y) = width(x);
  if (height(y) < h) {   /*739:*/
    p = newkern(h - height(y));
    link(p) = listptr(y);
    listptr(y) = p;
    height(y) = h;
  }  /*:739*/
  info(nucleus(q)) = y;
  mathtype(nucleus(q)) = subbox;
}
/*:738*/

/*743:*/
Static void makefraction(HalfWord q)
{
  Pointer p, v, x, y, z;
  Scaled delta, delta1, delta2, shiftup, shiftdown, clr;

  if (thickness(q) == defaultcode)   /*744:*/
    thickness(q) = defaultrulethickness;
  x = cleanbox(numerator(q), numstyle(curstyle));
  z = cleanbox(denominator(q), denomstyle(curstyle));
  if (width(x) < width(z))
    x = rebox(x, width(z));
  else
    z = rebox(z, width(x));
  if (curstyle < textstyle) {
    shiftup = num1(cursize);
    shiftdown = denom1(cursize);
  } else {   /*:744*/
    shiftdown = denom2(cursize);
    if (thickness(q) != 0)
      shiftup = num2(cursize);
    else
      shiftup = num3(cursize);
  }
  if (thickness(q) == 0) {   /*745:*/
    if (curstyle < textstyle)
      clr = defaultrulethickness * 7;
    else
      clr = defaultrulethickness * 3;
    delta = half(clr - shiftup + depth(x) + height(z) - shiftdown);
    if (delta > 0) {
      shiftup += delta;
      shiftdown += delta;
    }
  } else {
    if (curstyle < textstyle)
      clr = thickness(q) * 3;
    else
      clr = thickness(q);
    delta = half(thickness(q));
    delta1 = clr - shiftup + depth(x) + axisheight(cursize) + delta;
    delta2 = clr - axisheight(cursize) + delta + height(z) - shiftdown;
    if (delta1 > 0)
      shiftup += delta1;
    if (delta2 > 0)
      shiftdown += delta2;
  }
  /*:745*/
  /*747:*/
  v = newnullbox();
  type(v) = vlistnode;
  height(v) = shiftup + height(x);
  depth(v) = depth(z) + shiftdown;
  width(v) = width(x);
  if (thickness(q) == 0) {
    p = newkern(shiftup - depth(x) - height(z) + shiftdown);
    link(p) = z;
  } else {
    y = fractionrule(thickness(q));
    p = newkern(axisheight(cursize) - delta - height(z) + shiftdown);
    link(y) = p;
    link(p) = z;
    p = newkern(shiftup - depth(x) - axisheight(cursize) - delta);
    link(p) = y;
  }
  link(x) = p;
  listptr(v) = x;   /*:747*/
  /*748:*/
  if (curstyle < textstyle)
    delta = delim1(cursize);
  else
    delta = delim2(cursize);
  x = vardelimiter(leftdelimiter(q), cursize, delta);
  link(x) = v;
  z = vardelimiter(rightdelimiter(q), cursize, delta);
  link(v) = z;
  newhlist(q) = hpack(x, 0, additional);   /*:748*/
}  /*:743*/


/*749:*/
Static long makeop(HalfWord q)
{
  Scaled delta, shiftup, shiftdown;
  Pointer p, v, x, y, z;
  QuarterWord c;
  FourQuarters i;

  if (subtype(q) == normal && curstyle < textstyle)
    subtype(q) = limits;
  if (mathtype(nucleus(q)) == mathchar) {
    fetch(nucleus(q));
    if ((curstyle < textstyle) & (chartag(curi) == listtag)) {
      c = rembyte(curi);
      i = charinfo(curf, c);
      if (charexists(i)) {
	curc = c;
	curi = i;
	character(nucleus(q)) = c;
      }
    }
    delta = charitalic(curf, curi);
    x = cleanbox(nucleus(q), curstyle);
    if ((mathtype(subscr(q)) != empty) & (subtype(q) != limits))
      width(x) -= delta;
    shiftamount(x) =
      half(height(x) - depth(x)) - axisheight(cursize);
    mathtype(nucleus(q)) = subbox;
    info(nucleus(q)) = x;
  } else
    delta = 0;
  if (subtype(q) != limits)   /*750:*/
    return delta;
  /*:750*/
  x = cleanbox(supscr(q), supstyle(curstyle));
  y = cleanbox(nucleus(q), curstyle);
  z = cleanbox(subscr(q), substyle(curstyle));
  v = newnullbox();
  type(v) = vlistnode;
  width(v) = width(y);
  if (width(x) > width(v))
    width(v) = width(x);
  if (width(z) > width(v))
    width(v) = width(z);
  x = rebox(x, width(v));
  y = rebox(y, width(v));
  z = rebox(z, width(v));
  shiftamount(x) = half(delta);
  shiftamount(z) = -shiftamount(x);
  height(v) = height(y);
  depth(v) = depth(y);   /*751:*/
  if (mathtype(supscr(q)) == empty) {
    freenode(x, boxnodesize);
    listptr(v) = y;
  } else {
    shiftup = bigopspacing3 - depth(x);
    if (shiftup < bigopspacing1)
      shiftup = bigopspacing1;
    p = newkern(shiftup);
    link(p) = y;
    link(x) = p;
    p = newkern(bigopspacing5);
    link(p) = x;
    listptr(v) = p;
    height(v) += bigopspacing5 + height(x) + depth(x) + shiftup;
  }
  if (mathtype(subscr(q)) == empty)
    freenode(z, boxnodesize);
  else {   /*:751*/
    shiftdown = bigopspacing4 - height(z);
    if (shiftdown < bigopspacing2)
      shiftdown = bigopspacing2;
    p = newkern(shiftdown);
    link(y) = p;
    link(p) = z;
    p = newkern(bigopspacing5);
    link(z) = p;
    depth(v) += bigopspacing5 + height(z) + depth(z) + shiftdown;
  }
  newhlist(q) = v;
  return delta;
}
/*:749*/

/*752:*/
Static void makeord(HalfWord q)
{
  long a;
  Pointer p, r;

_Lrestart:
  if (mathtype(subscr(q)) == empty) {
    if (mathtype(supscr(q)) == empty) {
      if (mathtype(nucleus(q)) == mathchar) {
	p = link(q);
	if (p != 0) {
	  if ((type(p) >= ordnoad) & (type(p) <= punctnoad)) {
	    if (mathtype(nucleus(p)) == mathchar) {
	      if (fam(nucleus(p)) == fam(nucleus(q))) {
		mathtype(nucleus(q)) = mathtextchar;
		fetch(nucleus(q));
		if (chartag(curi) == ligtag) {
		  a = ligkernstart(curf,curi);
		  curc = character(nucleus(p));
		  curi = fontinfo[a].qqqq;
		  if (skipbyte(curi) > stopflag) {
		    a = ligkernrestart(curf,curi);
		    curi = fontinfo[a].qqqq;
		  }
		  while (true) {  /*753:*/
		    if (nextchar(curi) == curc) {
		      if (skipbyte(curi) <= stopflag) {
			if (opbyte(curi) >= kernflag) {
			  p = newkern(charkern(curf, curi));
			  link(p) = link(q);
			  link(q) = p;
			  goto _Lexit;
			} else {   /*:753*/
			  checkinterrupt();
			  switch (opbyte(curi)) {

			  case minquarterword + 1:
			  case minquarterword + 5:
			    character(nucleus(q)) = rembyte(curi);
			    break;

			  case minquarterword + 2:
			  case minquarterword + 6:
			    character(nucleus(p)) = rembyte(curi);
			    break;

			  case minquarterword + 3:
			  case minquarterword + 7:
			  case minquarterword + 11:
			    r = newnoad();
			    character(nucleus(r)) = rembyte(curi);
			    fam(nucleus(r)) = fam(nucleus(q));
			    link(q) = r;
			    link(r) = p;
			    if (opbyte(curi) < minquarterword + 11)
			      mathtype(nucleus(r)) = mathchar;
			    else
			      mathtype(nucleus(r)) = mathtextchar;
			    break;

			  default:
			    link(q) = link(p);
			    character(nucleus(q)) = rembyte(curi);
			    mem[subscr(q) - memmin] =
			      mem[subscr(p) - memmin];
			    mem[supscr(q) - memmin] =
			      mem[supscr(p) - memmin];
			    freenode(p, noadsize);
			    break;
			  }
			  if (opbyte(curi) > minquarterword + 3)
			    goto _Lexit;
			  mathtype(nucleus(q)) = mathchar;
			  goto _Lrestart;
			}
		      }
		    }
		    if (skipbyte(curi) >= stopflag)
		      goto _Lexit;
		    a += skipbyte(curi) - minquarterword + 1;
		    curi = fontinfo[a].qqqq;
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }
_Lexit: ;
}
/*:752*/

/*756:*/
Static void makescripts(HalfWord q, long delta)
{
  Pointer p, x, y, z;
  Scaled shiftup, shiftdown, clr;
  SmallNumber t;

  p = newhlist(q);
  if (ischarnode(p)) {
    shiftup = 0;
    shiftdown = 0;
  } else {
    z = hpack(p, 0, additional);
    if (curstyle < scriptstyle)
      t = SCRIPT_SIZE;
    else
      t = SCRIPT_SCRIPT_SIZE;
    shiftup = height(z) - supdrop(t);
    shiftdown = depth(z) + subdrop(t);
    freenode(z, boxnodesize);
  }
  if (mathtype(supscr(q)) == empty) {   /*757:*/
    x = cleanbox(subscr(q), substyle(curstyle));
    width(x) += scriptspace;
    if (shiftdown < sub1(cursize))
      shiftdown = sub1(cursize);
    clr = height(x) - labs(mathxheight(cursize) * 4) / 5;
    if (shiftdown < clr)
      shiftdown = clr;
    shiftamount(x) = shiftdown;
  } else {  /*758:*/
    x = cleanbox(supscr(q), supstyle(curstyle));
    width(x) += scriptspace;
    if (curstyle & 1)
      clr = sup3(cursize);
    else if (curstyle < textstyle)
      clr = sup1(cursize);
    else
      clr = sup2(cursize);
    if (shiftup < clr)
      shiftup = clr;
    clr = depth(x) + labs(mathxheight(cursize)) / 4;
    if (shiftup < clr)   /*:758*/
      shiftup = clr;
    if (mathtype(subscr(q)) == empty)
      shiftamount(x) = -shiftup;
    else {   /*759:*/
      y = cleanbox(subscr(q), substyle(curstyle));
      width(y) += scriptspace;
      if (shiftdown < sub2(cursize))
	shiftdown = sub2(cursize);
      clr = defaultrulethickness * 4 - shiftup + depth(x) +
	    height(y) - shiftdown;
      if (clr > 0) {
	shiftdown += clr;
	clr = labs(mathxheight(cursize) * 4) / 5 - shiftup + depth(x);
	if (clr > 0) {
	  shiftup += clr;
	  shiftdown -= clr;
	}
      }
      shiftamount(x) = delta;
      p = newkern(shiftup - depth(x) - height(y) + shiftdown);
      link(x) = p;
      link(p) = y;
      x = vpack(x, 0, additional);
      shiftamount(x) = shiftdown;
    }
  }
  /*:757*/
  if (newhlist(q) == 0) {
    newhlist(q) = x;
    return;
  }
  p = newhlist(q);
  while (link(p) != 0)
    p = link(p);
  link(p) = x;

  /*:759*/
}  /*:756*/


/*762:*/
Static SmallNumber makeleftright(HalfWord q, SmallNumber style, long maxd,
				 long maxh)
{
  Scaled delta, delta1, delta2;

  if (style < scriptstyle)
    cursize = TEXT_SIZE;
  else
    cursize = (style - textstyle) / 2 * 16;
  delta2 = maxd + axisheight(cursize);
  delta1 = maxh + maxd - delta2;
  if (delta2 > delta1)
    delta1 = delta2;
  delta = delta1 / 500 * delimiterfactor;
  delta2 = delta1 + delta1 - delimitershortfall;
  if (delta < delta2)
    delta = delta2;
  newhlist(q) = vardelimiter(delimiter(q), cursize, delta);
  return (type(q) - leftnoad + opennoad);
}  /*:762*/


Static void mlisttohlist(void)
{
  Pointer mlist, q, r, p=0 /* XXXX */, x=0 /* XXXX */, y, z;
  Boolean penalties;
  SmallNumber style, savestyle, rtype, t;
  long pen;
  SmallNumber s;
  Scaled maxh, maxd, delta;

  mlist = curmlist;
  penalties = mlistpenalties;
  style = curstyle;
  q = mlist;
  r = 0;
  rtype = opnoad;
  maxh = 0;
  maxd = 0;   /*703:*/
  if (curstyle < scriptstyle)
    cursize = TEXT_SIZE;
  else
    cursize = (curstyle - textstyle) / 2 * 16;
  curmu = x_over_n(mathquad(cursize), 18);   /*:703*/
  while (q != 0) {   /*727:*/
_Lreswitch:
    delta = 0;
    switch (type(q)) {

    case binnoad:
      switch (rtype) {

      case binnoad:
      case opnoad:
      case relnoad:
      case opennoad:
      case punctnoad:
      case leftnoad:
	type(q) = ordnoad;
	goto _Lreswitch;
	break;
      }
      break;

    case relnoad:
    case closenoad:
    case punctnoad:
    case rightnoad:  /*729:*/
      if (rtype == binnoad)
	type(r) = ordnoad;   /*:729*/
      if (type(q) == rightnoad)
	goto _Ldonewithnoad_;
      break;
      /*733:*/

    case leftnoad:
      goto _Ldonewithnoad_;
      break;

    case fractionnoad:
      makefraction(q);
      goto _Lcheckdimensions_;
      break;

    case opnoad:
      delta = makeop(q);
      if (subtype(q) == limits)
	goto _Lcheckdimensions_;
      break;

    case ordnoad:
      makeord(q);
      break;

    case opennoad:
    case innernoad:
      /* blank case */
      break;

    case radicalnoad:
      makeradical(q);
      break;

    case overnoad:
      makeover(q);
      break;

    case undernoad:
      makeunder(q);
      break;

    case accentnoad:
      makemathaccent(q);
      break;

    case vcenternoad:   /*:733*/
      makevcenter(q);
      break;
      /*730:*/

    case stylenode:
      curstyle = subtype(q);   /*703:*/
      if (curstyle < scriptstyle)
	cursize = TEXT_SIZE;
      else
	cursize = (curstyle - textstyle) / 2 * 16;
      curmu = x_over_n(mathquad(cursize), 18);   /*:703*/
      goto _Ldonewithnode_;
      break;

    case choicenode:   /*731:*/
      switch (curstyle / 2) {

      case 0:
	p = displaymlist(q);
	displaymlist(q) = 0;
	break;

      case 1:
	p = textmlist(q);
	textmlist(q) = 0;
	break;

      case 2:
	p = scriptmlist(q);
	scriptmlist(q) = 0;
	break;

      case 3:
	p = scriptscriptmlist(q);
	scriptscriptmlist(q) = 0;
	break;
      }
      flushnodelist(displaymlist(q));
      flushnodelist(textmlist(q));
      flushnodelist(scriptmlist(q));
      flushnodelist(scriptscriptmlist(q));
      type(q) = stylenode;
      subtype(q) = curstyle;
      width(q) = 0;
      depth(q) = 0;
      if (p != 0) {
	z = link(q);
	link(q) = p;
	while (link(p) != 0)
	  p = link(p);
	link(p) = z;
      }
      goto _Ldonewithnode_;
      break;
      /*:731*/

    case insnode:
    case marknode:
    case adjustnode:
    case whatsitnode:
    case penaltynode:
    case discnode:
      goto _Ldonewithnode_;
      break;

    case rulenode:
      if (height(q) > maxh)
	maxh = height(q);
      if (depth(q) > maxd)
	maxd = depth(q);
      goto _Ldonewithnode_;
      break;

    case gluenode:  /*732:*/
      if (subtype(q) == muglue) {
	x = glueptr(q);
	y = mathglue(x, curmu);
	deleteglueref(x);
	glueptr(q) = y;
	subtype(q) = normal;
      } else if ((cursize != TEXT_SIZE) & (subtype(q) == condmathglue)) {
	p = link(q);
	if (p != 0) {
	  if ((type(p) == gluenode) | (type(p) == kernnode)) {
	    link(q) = link(p);
	    link(p) = 0;
	    flushnodelist(p);
	  }
	}
      }
      goto _Ldonewithnode_;
      break;

    case kernnode:   /*:730*/
      mathkern(q, curmu);
      goto _Ldonewithnode_;
      break;

    default:
      confusion(S(720));
      break;
    }
    /*754:*/
    switch (mathtype(nucleus(q))) {

    case mathchar:
    case mathtextchar:   /*:755*/
      /*755:*/
      fetch(nucleus(q));
      if (charexists(curi)) {
	delta = charitalic(curf, curi);
	p = newcharacter(curf, curc - minquarterword);
	if ((mathtype(nucleus(q)) == mathtextchar) & (space(curf) != 0))
	  delta = 0;
	if (mathtype(subscr(q)) == empty && delta != 0) {
	  link(p) = newkern(delta);
	  delta = 0;
	}
      } else
	p = 0;
      break;

    case empty:
      p = 0;
      break;

    case subbox:
      p = info(nucleus(q));
      break;

    case submlist:
      curmlist = info(nucleus(q));
      savestyle = curstyle;
      mlistpenalties = false;
      mlisttohlist();
      curstyle = savestyle;   /*703:*/
      if (curstyle < scriptstyle)
	cursize = TEXT_SIZE;
      else
	cursize = (curstyle - textstyle) / 2 * 16;
      curmu = x_over_n(mathquad(cursize), 18);   /*:703*/
      p = hpack(link(temphead), 0, additional);
      break;

    default:
      confusion(S(721));
      break;
    }
    newhlist(q) = p;
    if ((mathtype(subscr(q)) == empty) &
	(mathtype(supscr(q)) == empty))
	  /*:754*/
	    goto _Lcheckdimensions_;
    /*:728*/
    makescripts(q, delta);
_Lcheckdimensions_:
    z = hpack(newhlist(q), 0, additional);
    if (height(z) > maxh)
      maxh = height(z);
    if (depth(z) > maxd)
      maxd = depth(z);
    freenode(z, boxnodesize);
_Ldonewithnoad_:
    r = q;
    rtype = type(r);
_Ldonewithnode_:
    q = link(q);
  }
  /*728:*/
  /*:727*/
  /*729:*/
  if (rtype == binnoad)   /*760:*/
    type(r) = ordnoad;   /*:729*/
  p = temphead;
  link(p) = 0;
  q = mlist;
  rtype = 0;
  curstyle = style;   /*703:*/
  if (curstyle < scriptstyle)
    cursize = TEXT_SIZE;
  else
    cursize = (curstyle - textstyle) / 2 * 16;
  curmu = x_over_n(mathquad(cursize), 18);   /*:703*/
  while (q != 0) {   /*761:*/
    t = ordnoad;
    s = noadsize;
    pen = infpenalty;
    switch (type(q)) {   /*:761*/

    case opnoad:
    case opennoad:
    case closenoad:
    case punctnoad:
    case innernoad:
      t = type(q);
      break;

    case binnoad:
      t = binnoad;
      pen = binoppenalty;
      break;

    case relnoad:
      t = relnoad;
      pen = relpenalty;
      break;

    case ordnoad:
    case vcenternoad:
    case overnoad:
    case undernoad:
      /* blank case */
      break;

    case radicalnoad:
      s = radicalnoadsize;
      break;

    case accentnoad:
      s = accentnoadsize;
      break;

    case fractionnoad:
      t = innernoad;
      s = fractionnoadsize;
      break;

    case leftnoad:
    case rightnoad:
      t = makeleftright(q, style, maxd, maxh);
      break;

    case stylenode:   /*763:*/
      curstyle = subtype(q);
      s = stylenodesize;   /*703:*/
      if (curstyle < scriptstyle)
	cursize = TEXT_SIZE;
      else
	cursize = (curstyle - textstyle) / 2 * 16;
      curmu = x_over_n(mathquad(cursize), 18);   /*:703*/
      goto _Ldeleteq_;
      break;
      /*:763*/

    case whatsitnode:
    case penaltynode:
    case rulenode:
    case discnode:
    case adjustnode:
    case insnode:
    case marknode:
    case gluenode:
    case kernnode:
      link(p) = q;
      p = q;
      q = link(q);
      link(p) = 0;
      goto _Ldone;
      break;

    default:
      confusion(S(722));
      break;
    }
    /*766:*/
    if (rtype > 0) {   /*:766*/
      const char trans_table[]="0234000122*4000133**3**344*0400400*000000234000111*1111112341011";
      switch (trans_table[rtype * 8 + t - ordnoad * 9]) {
      case '0':
	x = 0;
	break;

      case '1':
	if (curstyle < scriptstyle)
	  x = thinmuskipcode;
	else
	  x = 0;
	break;

      case '2':
	x = thinmuskipcode;
	break;

      case '3':
	if (curstyle < scriptstyle)
	  x = medmuskipcode;
	else
	  x = 0;
	break;

      case '4':
	if (curstyle < scriptstyle)
	  x = thickmuskipcode;
	else
	  x = 0;
	break;

      default:
	confusion(S(723));
	break;
      }
      if (x != 0) {
	y = mathglue(gluepar(x), curmu);
	z = newglue(y);
	gluerefcount(y) = 0;
	link(p) = z;
	p = z;
	subtype(z) = x + 1;
      }
    }
    /*767:*/
    if (newhlist(q) != 0) {
      link(p) = newhlist(q);
      do {
	p = link(p);
      } while (link(p) != 0);
    }
    if (penalties) {
      if (link(q) != 0) {
	if (pen < infpenalty) {   /*:767*/
	  rtype = type(link(q));
	  if (rtype != penaltynode) {
	    if (rtype != relnoad) {
	      z = newpenalty(pen);
	      link(p) = z;
	      p = z;
	    }
	  }
	}
      }
    }
    rtype = t;
_Ldeleteq_:
    r = q;
    q = link(q);
    freenode(r, s);
_Ldone: ;
  }
  /*:760*/

  /*:732*/
}
/*:726*/

/*772:*/
Static void pushalignment(void)
{
  Pointer p;

  p = getnode(alignstacknodesize);
  link(p) = alignptr;
  info(p) = curalign;
  llink(p) = preamble;
  rlink(p) = curspan;
  mem[p - memmin + 2].int_ = curloop;
  mem[p - memmin + 3].int_ = alignstate;
  info(p + 4) = curhead;
  link(p + 4) = curtail;
  alignptr = p;
  curhead = get_avail();
}


Static void popalignment(void)
{
  Pointer p;

  FREE_AVAIL(curhead);
  p = alignptr;
  curtail = link(p + 4);
  curhead = info(p + 4);
  alignstate = mem[p - memmin + 3].int_;
  curloop = mem[p - memmin + 2].int_;
  curspan = rlink(p);
  preamble = llink(p);
  curalign = info(p);
  alignptr = link(p);
  freenode(p, alignstacknodesize);
}
/*:772*/

/*774:*/
/*782:*/
Static void getpreambletoken(void)
{
_Lrestart:
  gettoken();
  while (curchr == spancode && curcmd == tabmark) {
    gettoken();
    if (curcmd > maxcommand) {
      expand();
      gettoken();
    }
  }
  if (curcmd == endv)
    fatalerror(S(509));
  if (curcmd != assignglue || curchr != gluebase + tabskipcode)
    return;
  scanoptionalequals();
  scanglue(glueval);
  if (globaldefs > 0)
    geqdefine(gluebase + tabskipcode, glueref, curval);
  else
    eqdefine(gluebase + tabskipcode, glueref, curval);
  goto _Lrestart;
}
/*:782*/

Static void alignpeek(void);

Static void normalparagraph(void);


Static void initalign(void)
{
  Pointer savecsptr, p;

  savecsptr = curcs;
  pushalignment();
  alignstate = -1000000L;   /*776:*/
  if (mode == mmode && (tail != head || incompleatnoad != 0)) {   /*:776*/
    printnl(S(292));
    print(S(597));
    print_esc(S(724));
    print(S(725));
    help3(S(726),
          S(727),
          S(728));
    error();
    flushmath();
  }
  pushnest();   /*775:*/
  if (mode == mmode) {
    mode = -vmode;
    prevdepth = nest[nestptr - 2].auxfield.sc;
  } else if (mode > 0)
    mode = -mode;   /*:775*/
  scanspec(aligngroup, false);
  /*777:*/
  preamble = 0;
  curalign = alignhead;
  curloop = 0;
  scannerstatus = aligning;
  warningindex = savecsptr;
  alignstate = -1000000L;
  while (true) {   /*778:*/
    link(curalign) = newparamglue(tabskipcode);
    curalign = link(curalign);   /*:778*/
    if (curcmd == carret)
      goto _Ldone;
    /*779:*/
    /*783:*/
    p = holdhead;
    link(p) = 0;
    while (true) {
      getpreambletoken();
      if (curcmd == macparam)
	goto _Ldone1;
      if (curcmd <= carret && curcmd >= tabmark && alignstate == -1000000L) {
	if (p == holdhead && curloop == 0 && curcmd == tabmark) {
	  curloop = curalign;
	  continue;
	} else {
	  printnl(S(292));
	  print(S(729));
	  help3(
            S(730),
            S(731),
            S(732));
	  backerror();
	  goto _Ldone1;
	}
      } else {
	if (curcmd != spacer || p != holdhead) {
	  link(p) = get_avail();
	  p = link(p);
	  info(p) = curtok;
	}
	continue;
      }
    }
_Ldone1:   /*:783*/
    link(curalign) = newnullbox();
    curalign = link(curalign);
    info(curalign) = endspan;
    width(curalign) = nullflag;
    upart(curalign) = link(holdhead);   /*784:*/
    p = holdhead;
    link(p) = 0;
    while (true) {
_Llabcontinue:
      getpreambletoken();
      if (curcmd <= carret && curcmd >= tabmark && alignstate == -1000000L)
	goto _Ldone2;
      if (curcmd == macparam) {
	printnl(S(292));
	print(S(733));
	help3(S(730),
              S(731),
              S(734));
	error();
	goto _Llabcontinue;
      }
      link(p) = get_avail();
      p = link(p);
      info(p) = curtok;
    }
_Ldone2:
    link(p) = get_avail();
    p = link(p);
    info(p) = endtemplatetoken;   /*:784*/
    vpart(curalign) = link(holdhead);   /*:779*/
  }
_Ldone:
  scannerstatus = normal;   /*:777*/
  newsavelevel(aligngroup);
  if (everycr != 0)
    begintokenlist(everycr, everycrtext);
  alignpeek();
}
/*:774*/

/*786:*/
/*787:*/
Static void initspan(HalfWord p)
{
  pushnest();
  if (mode == -hmode)
    spacefactor = 1000;
  else {
    prevdepth = ignoredepth;
    normalparagraph();
  }
  curspan = p;
}
/*:787*/

Static void initrow(void)
{
  pushnest();
  mode = -hmode - vmode - mode;
  if (mode == -hmode)
    spacefactor = 0;
  else
    prevdepth = 0;
  tailappend(newglue(glueptr(preamble)));
  subtype(tail) = tabskipcode + 1;
  curalign = link(preamble);
  curtail = curhead;
  initspan(curalign);
}
/*:786*/

/*788:*/
Static void initcol(void)
{
  extrainfo(curalign) = curcmd;
  if (curcmd == omit)
    alignstate = 0;
  else {
    backinput();
    begintokenlist(upart(curalign), utemplate);
  }
}
/*:788*/

/*791:*/
Static Boolean fincol(void)
{
  Boolean Result;
  Pointer p, q, r, s, u;
  Scaled w;
  GlueOrd o;
  HalfWord n;

  if (curalign == 0)
    confusion(S(735));
  q = link(curalign);
  if (q == 0)
    confusion(S(735));
  if (alignstate < 500000L)
    fatalerror(S(509));
  p = link(q);
  /*792:*/
  if ((p == 0) & (extrainfo(curalign) < crcode)) {
    if (curloop != 0) {   /*793:*/
      link(q) = newnullbox();
      p = link(q);
      info(p) = endspan;
      width(p) = nullflag;
      curloop = link(curloop);   /*794:*/
      q = holdhead;
      r = upart(curloop);
      while (r != 0) {
	link(q) = get_avail();
	q = link(q);
	info(q) = info(r);
	r = link(r);
      }
      link(q) = 0;
      upart(p) = link(holdhead);
      q = holdhead;
      r = vpart(curloop);
      while (r != 0) {
	link(q) = get_avail();
	q = link(q);
	info(q) = info(r);
	r = link(r);
      }
      link(q) = 0;
      vpart(p) = link(holdhead);   /*:794*/
      curloop = link(curloop);
      link(p) = newglue(glueptr(curloop));
    } else {   /*:792*/
      printnl(S(292));
      print(S(736));
      print_esc(S(737));
      help3(S(738),
            S(739),
            S(740));
      extrainfo(curalign) = crcode;
      error();
    }
    /*:793*/
  }
  if (extrainfo(curalign) != spancode) {
    unsave();
    newsavelevel(aligngroup);   /*796:*/
    if (mode == -hmode) {
      adjusttail = curtail;
      u = hpack(link(head), 0, additional);
      w = width(u);
      curtail = adjusttail;
      adjusttail = 0;
    } else {
      u = vpackage(link(head), 0, additional, 0);
      w = height(u);
    }
    n = minquarterword;
    if (curspan != curalign) {   /*798:*/
      q = curspan;
      do {
	n++;
	q = link(link(q));
      } while (q != curalign);
      if (n > maxquarterword)
	confusion(S(741));
      q = curspan;
      while (link(info(q)) < n)
	q = info(q);
      if (link(info(q)) > n) {
	s = getnode(spannodesize);
	info(s) = info(q);
	link(s) = n;
	info(q) = s;
	width(s) = w;
      } else if (width(info(q)) < w)
	width(info(q)) = w;
    } else if (w > width(curalign))   /*:798*/
      width(curalign) = w;
    type(u) = unsetnode;
    spancount(u) = n;   /*659:*/
    if (totalstretch[filll - normal] != 0)
      o = filll;
    else if (totalstretch[fill - normal] != 0)
      o = fill;
    else if (totalstretch[fil - normal] != 0)
      o = fil;
    else {
      o = normal;
      /*:659*/
    }
    glueorder(u) = o;
    gluestretch(u) = totalstretch[o - normal];   /*665:*/
    if (totalshrink[filll - normal] != 0)
      o = filll;
    else if (totalshrink[fill - normal] != 0)
      o = fill;
    else if (totalshrink[fil - normal] != 0)
      o = fil;
    else
      o = normal;   /*:665*/
    gluesign(u) = o;
    glueshrink(u) = totalshrink[o - normal];
    popnest();
    link(tail) = u;
    tail = u;   /*:796*/
    /*795:*/
    tailappend(newglue(glueptr(link(curalign))));
    subtype(tail) = tabskipcode + 1;   /*:795*/
    if (extrainfo(curalign) >= crcode) {
      Result = true;
      goto _Lexit;
    }
    initspan(p);
  }
  alignstate = 1000000L;
  skip_spaces();
  curalign = p;
  initcol();
  Result = false;
_Lexit:
  return Result;
}
/*:791*/

/*799:*/
Static void finrow(void)
{
  Pointer p;

  if (mode == -hmode) {
    p = hpack(link(head), 0, additional);
    popnest();
    appendtovlist(p);
    if (curhead != curtail) {
      link(tail) = link(curhead);
      tail = curtail;
    }
  } else {
    p = vpack(link(head), 0, additional);
    popnest();
    link(tail) = p;
    tail = p;
    spacefactor = 1000;
  }
  type(p) = unsetnode;
  gluestretch(p) = 0;
  if (everycr != 0)
    begintokenlist(everycr, everycrtext);
  alignpeek();
}
/*:799*/

/*800:*/
Static void doassignments(void);

Static void resumeafterdisplay(void);

Static void buildpage(void);


Static void finalign(void)
{
  Pointer p, q, r, s, u, v;
  Scaled t, w, o, rulesave;
  HalfWord n;
  MemoryWord auxsave;

  if (curgroup != aligngroup)
    confusion(S(742));
  unsave();
  if (curgroup != aligngroup)
    confusion(S(743));
  unsave();
  if (nest[nestptr - 1].modefield == mmode)
    o = displayindent;
  else
    o = 0;
  /*801:*/
  q = link(preamble);
  do {   /*804:*/
    flushlist(upart(q));
    flushlist(vpart(q));
    p = link(link(q));
    if (width(q) == nullflag) {   /*802:*/
      width(q) = 0;
      r = link(q);
      s = glueptr(r);
      if (s != zeroglue) {
	addglueref(zeroglue);
	deleteglueref(s);
	glueptr(r) = zeroglue;
      }
    }
    /*:802*/
    if (info(q) != endspan) {   /*803:*/
      t = width(q) + width(glueptr(link(q)));
      r = info(q);
      s = endspan;
      info(s) = p;
      n = minquarterword + 1;
      do {
	width(r) -= t;
	u = info(r);
	while (link(r) > n) {
	  s = info(s);
	  n = link(info(s)) + 1;
	}
	if (link(r) < n) {
	  info(r) = info(s);
	  info(s) = r;
	  (link(r))--;
	  s = r;
	} else {
	  if (width(r) > width(info(s)))
	    width(info(s)) = width(r);
	  freenode(r, spannodesize);
	}
	r = u;
      } while (r != endspan);
    }
    /*:803*/
    type(q) = unsetnode;
    spancount(q) = minquarterword;
    height(q) = 0;
    depth(q) = 0;
    glueorder(q) = normal;
    gluesign(q) = normal;
    gluestretch(q) = 0;
    glueshrink(q) = 0;
    q = p;   /*:801*/
  } while (q != 0);
  saveptr -= 2;
  packbeginline = -modeline;
  if (mode == -vmode) {
    rulesave = overfullrule;
    overfullrule = 0;
    p = hpack(preamble, saved(1), saved(0));
    overfullrule = rulesave;
  } else {
    q = link(preamble);
    do {
      height(q) = width(q);
      width(q) = 0;
      q = link(link(q));
    } while (q != 0);
    p = vpack(preamble, saved(1), saved(0));
    q = link(preamble);
    do {
      width(q) = height(q);
      height(q) = 0;
      q = link(link(q));
    } while (q != 0);
  }
  packbeginline = 0;   /*:804*/
  /*805:*/
  q = link(head);
  s = head;
  while (q != 0) {   /*:805*/
    if (!ischarnode(q)) {
      if (type(q) == unsetnode) {   /*807:*/
	if (mode == -vmode) {
	  type(q) = hlistnode;
	  width(q) = width(p);
	} else {
	  type(q) = vlistnode;
	  height(q) = height(p);
	}
	glueorder(q) = glueorder(p);
	gluesign(q) = gluesign(p);
	glueset(q) = glueset(p);
	shiftamount(q) = o;
	r = link(listptr(q));
	s = link(listptr(p));
	do {   /*808:*/
	  n = spancount(r);
	  t = width(s);
	  w = t;
	  u = holdhead;
	  while (n > minquarterword) {
	    n--;
	    /*809:*/
	    s = link(s);
	    v = glueptr(s);
	    link(u) = newglue(v);
	    u = link(u);
	    subtype(u) = tabskipcode + 1;
	    t += width(v);
	    if (gluesign(p) == stretching) {
	      if (stretchorder(v) == glueorder(p))
		t += tex_round(((double)glueset(p)) * stretch(v));
	    } else if (gluesign(p) == shrinking) {
	      if (shrinkorder(v) == glueorder(p))
		t -= tex_round(((double)glueset(p)) * shrink(v));
	    }
	    s = link(s);
	    link(u) = newnullbox();
	    u = link(u);
	    t += width(s);
	    if (mode == -vmode)
	      width(u) = width(s);
	    else {   /*:809*/
	      type(u) = vlistnode;
	      height(u) = width(s);
	    }
	  }
	  if (mode == -vmode) {   /*810:*/
	    height(r) = height(q);
	    depth(r) = depth(q);
	    if (t == width(r)) {
	      gluesign(r) = normal;
	      glueorder(r) = normal;
	      glueset(r) = 0.0;
	    } else if (t > width(r)) {
	      gluesign(r) = stretching;
	      if (gluestretch(r) == 0)
		glueset(r) = 0.0;
	      else
		glueset(r) = (double)(t - width(r)) / gluestretch(r);
	    } else {
	      glueorder(r) = gluesign(r);
	      gluesign(r) = shrinking;
	      if (glueshrink(r) == 0)
		glueset(r) = 0.0;
	      else if ((glueorder(r) == normal) &
		       (width(r) - t > glueshrink(r)))
		glueset(r) = 1.0;
	      else
		glueset(r) = (double)(width(r) - t) / glueshrink(r);
	    }
	    width(r) = w;
	    type(r) = hlistnode;
	  } else  /*811:*/
	  {   /*:811*/
	    width(r) = width(q);
	    if (t == height(r)) {
	      gluesign(r) = normal;
	      glueorder(r) = normal;
	      glueset(r) = 0.0;
	    } else if (t > height(r)) {
	      gluesign(r) = stretching;
	      if (gluestretch(r) == 0)
		glueset(r) = 0.0;
	      else
		glueset(r) = (double)(t - height(r)) / gluestretch(r);
	    } else {
	      glueorder(r) = gluesign(r);
	      gluesign(r) = shrinking;
	      if (glueshrink(r) == 0)
		glueset(r) = 0.0;
	      else if ((glueorder(r) == normal) &
		       (height(r) - t > glueshrink(r)))
		glueset(r) = 1.0;
	      else
		glueset(r) = (double)(height(r) - t) / glueshrink(r);
	    }
	    height(r) = w;
	    type(r) = vlistnode;
	  }
	  /*:810*/
	  shiftamount(r) = 0;
	  if (u != holdhead) {   /*:808*/
	    link(u) = link(r);
	    link(r) = link(holdhead);
	    r = u;
	  }
	  r = link(link(r));
	  s = link(link(s));
	} while (r != 0);
      }  /*:807*/
      else if (type(q) == rulenode) {
	if (isrunning(width(q))) {
	  width(q) = width(p);
	}
	if (isrunning(height(q))) {
	  height(q) = height(p);
	}
	if (isrunning(depth(q))) {
	  depth(q) = depth(p);
	}
	if (o != 0) {
	  r = link(q);
	  link(q) = 0;
	  q = hpack(q, 0, additional);
	  shiftamount(q) = o;
	  link(q) = r;
	  link(s) = q;
	}
      }
    }
    s = q;
    q = link(q);
  }
  flushnodelist(p);
  popalignment();   /*812:*/
  auxsave = aux;
  p = link(head);
  q = tail;
  popnest();
  if (mode == mmode) {   /*1206:*/
    doassignments();
    if (curcmd != mathshift) {   /*1207:*/
      printnl(S(292));
      print(S(744));
      help2(S(726),
            S(727));
      backerror();
    } else {   /*1197:*/
      getxtoken();
      if (curcmd != mathshift) {
	printnl(S(292));
	print(S(745));
	help2(S(746),
              S(747));
	backerror();
      }
    }
    /*:1207*/
    popnest();
    tailappend(newpenalty(predisplaypenalty));
    tailappend(newparamglue(abovedisplayskipcode));
    link(tail) = p;
    if (p != 0)
      tail = q;
    tailappend(newpenalty(postdisplaypenalty));
    tailappend(newparamglue(belowdisplayskipcode));
    prevdepth = auxsave.sc;
    resumeafterdisplay();
    return;
  }
  /*:1206*/
  aux = auxsave;
  link(tail) = p;
  if (p != 0)
    tail = q;
  if (mode == vmode)
    buildpage();

  /*:1197*/
  /*:812*/
}  /*785:*/


Static void alignpeek(void)
{
_Lrestart:
  alignstate = 1000000L;
  skip_spaces();
  if (curcmd == noalign) {
    scanleftbrace();
    newsavelevel(noaligngroup);
    if (mode == -vmode)
      normalparagraph();
    return;
  }
  if (curcmd == rightbrace) {
    finalign();
    return;
  }
  if (curcmd == carret && curchr == crcrcode)
    goto _Lrestart;
  initrow();
  initcol();
}
/*:785*/
/*:800*/

/*815:*/
/*826:*/
Static HalfWord finiteshrink(HalfWord p)
{
  Pointer q;

  if (noshrinkerroryet) {
    noshrinkerroryet = false;
    printnl(S(292));
    print(S(748));
    help5(S(749),
          S(750),
          S(751),
          S(752),
          S(753));
    error();
  }
  q = newspec(p);
  shrinkorder(q) = normal;
  deleteglueref(p);
  return q;
}  /*:826*/


/// p308#829
Static void trybreak(long pi, SmallNumber breaktype) { /*831:*/
    Pointer r, prevr;
    HalfWord oldl;
    Boolean nobreakyet;
    /*830:*/
    Pointer prevprevr = 0 /* XXXXX */, s, q, v, savelink;
    long t;
    InternalFontNumber f;
    HalfWord l;
    Boolean noderstaysactive;
    Scaled linewidth = 0 /* XXXX */, shortfall; /*:830*/
    char fitclass;
    HalfWord b;
    long d;
    Boolean artificialdemerits;

    if (labs(pi) >= infpenalty) {
        if (pi > 0) goto _Lexit;
        pi = ejectpenalty;
        /*:831*/
    }

    nobreakyet = true;
    prevr = active;
    oldl = 0;
    copytocuractive(1);
    copytocuractive(2);
    copytocuractive(3);
    copytocuractive(4);
    copytocuractive(5);
    copytocuractive(6);

    while (true) {
    _Llabcontinue:
        r = link(prevr);            /*832:*/
        if (type(r) == deltanode) { /*:832*/
            updatewidth(1);
            updatewidth(2);
            updatewidth(3);
            updatewidth(4);
            updatewidth(5);
            updatewidth(6);
            prevprevr = prevr;
            prevr = r;
            goto _Llabcontinue;
        }
        /*835:*/
        l = linenumber(r);
        if (l > oldl) { /*:835*/
            if (minimumdemerits < awfulbad &&
                (oldl != easyline || r == lastactive) ) { /*836:*/
                if (nobreakyet) {                        /*837:*/
                    nobreakyet = false;
                    setbreakwidthtobackground(1);
                    setbreakwidthtobackground(2);
                    setbreakwidthtobackground(3);
                    setbreakwidthtobackground(4);
                    setbreakwidthtobackground(5);
                    setbreakwidthtobackground(6);
                    s = curp;
                    if (breaktype > unhyphenated) {
                        if (curp != 0) { /*840:*/
                            t = replacecount(curp);
                            v = curp;
                            s = postbreak(curp);
                            while (t > 0) {
                                t--;
                                v = link(v); /*841:*/
                                if (ischarnode(v)) {
                                    f = font(v);
                                    breakwidth[0] -=
                                        charwidth(f, charinfo(f, character(v)));
                                    continue;
                                }
                                switch (type(v)) { /*:841*/

                                    case ligaturenode:
                                        f = font_ligchar(v);
                                        breakwidth[0] -= charwidth(
                                            f,
                                            charinfo(f, character_ligchar(v)));
                                        break;

                                    case hlistnode:
                                    case vlistnode:
                                    case rulenode:
                                    case kernnode:
                                        breakwidth[0] -= width(v);
                                        break;

                                    default:
                                        confusion(S(754));
                                        break;
                                }
                            }
                            while (s != 0) { /*842:*/
                                if (ischarnode(s)) {
                                    f = font(s);
                                    breakwidth[0] +=
                                        charwidth(f, charinfo(f, character(s)));
                                } else {
                                    switch (type(s)) { /*:842*/

                                        case ligaturenode:
                                            f = font_ligchar(s);
                                            breakwidth[0] += charwidth(
                                                f,
                                                charinfo(f,
                                                         character_ligchar(s)));
                                            break;

                                        case hlistnode:
                                        case vlistnode:
                                        case rulenode:
                                        case kernnode:
                                            breakwidth[0] += width(s);
                                            break;

                                        default:
                                            confusion(S(755));
                                            break;
                                    }
                                }
                                s = link(s);
                            }
                            breakwidth[0] += discwidth;
                            if (postbreak(curp) == 0) s = link(v);
                        }
                        /*:840*/
                    }
                    while (s != 0) {
                        if (ischarnode(s)) goto _Ldone;
                        switch (type(s)) {

                            case gluenode: /*838:*/
                                v = glueptr(s);
                                breakwidth[0] -= width(v);
                                breakwidth[stretchorder(v) + 1] -= stretch(v);
                                breakwidth[5] -= shrink(v);
                                break;
                                /*:838*/

                            case penaltynode:
                                /* blank case */
                                break;

                            case mathnode:
                                breakwidth[0] -= width(s);
                                break;

                            case kernnode:
                                if (subtype(s) != explicit) goto _Ldone;
                                breakwidth[0] -= width(s);
                                break;

                            default:
                                goto _Ldone;
                                break;
                        }
                        s = link(s);
                    }
                _Ldone:;
                }
                /*:837*/
                /*843:*/
                if (type(prevr) == deltanode) {
                    converttobreakwidth(1);
                    converttobreakwidth(2);
                    converttobreakwidth(3);
                    converttobreakwidth(4);
                    converttobreakwidth(5);
                    converttobreakwidth(6);
                } else if (prevr == active) {
                    storebreakwidth(1);
                    storebreakwidth(2);
                    storebreakwidth(3);
                    storebreakwidth(4);
                    storebreakwidth(5);
                    storebreakwidth(6);
                } else {
                    q = getnode(deltanodesize);
                    link(q) = r;
                    type(q) = deltanode;
                    subtype(q) = 0;
                    newdeltatobreakwidth(1);
                    newdeltatobreakwidth(2);
                    newdeltatobreakwidth(3);
                    newdeltatobreakwidth(4);
                    newdeltatobreakwidth(5);
                    newdeltatobreakwidth(6);
                    link(prevr) = q;
                    prevprevr = prevr;
                    prevr = q;
                }
                if (labs(adjdemerits) >= awfulbad - minimumdemerits)
                    minimumdemerits = awfulbad - 1;
                else
                    minimumdemerits += labs(adjdemerits);
                for (fitclass = veryloosefit; fitclass <= tightfit;
                     fitclass++) {
                    if (minimaldemerits[fitclass - veryloosefit] <=
                        minimumdemerits) {
                        /// p313#845: Insert a new active node from best
                        /// place[fit class] to cur p
                        q = getnode(passivenodesize);
                        link(q) = passive;
                        passive = q;
                        curbreak(q) = curp;
                        #ifdef tt_STAT
                            passnumber++;
                            serial(q) = passnumber;
                        #endif // #845.1: tt_STAT
                        prevbreak(q) = bestplace[fitclass - veryloosefit];
                        q = getnode(activenodesize);
                        breaknode(q) = passive;
                        linenumber(q) = bestplline[fitclass - veryloosefit] + 1;
                        fitness(q) = fitclass;
                        type(q) = breaktype;
                        totaldemerits(q) =
                            minimaldemerits[fitclass - veryloosefit];
                        link(q) = r;
                        link(prevr) = q;
                        prevr = q; 
                        #ifdef tt_STAT
                            if (tracingparagraphs > 0) { /*846:*/
                                printnl(S(756));
                                print_int(serial(passive));
                                print(S(757));
                                print_int(linenumber(q) - 1);
                                print_char('.');
                                print_int(fitclass);
                                if (breaktype == hyphenated) print_char('-');
                                print(S(758));
                                print_int(totaldemerits(q));
                                print(S(759));
                                if (prevbreak(passive) == 0)
                                    print_char('0');
                                else
                                    print_int(serial(prevbreak(passive)));
                            }
                            /*:846*/
                        #endif // #845: tt_STAT
                    }
                    /*:845*/
                    minimaldemerits[fitclass - veryloosefit] = awfulbad;
                }
                minimumdemerits = awfulbad; /*844:*/
                if (r != lastactive) {      /*:844*/
                    q = getnode(deltanodesize);
                    link(q) = r;
                    type(q) = deltanode;
                    subtype(q) = 0;
                    newdeltafrombreakwidth(1);
                    newdeltafrombreakwidth(2);
                    newdeltafrombreakwidth(3);
                    newdeltafrombreakwidth(4);
                    newdeltafrombreakwidth(5);
                    newdeltafrombreakwidth(6);
                    link(prevr) = q;
                    prevprevr = prevr;
                    prevr = q;
                }
            } // if (minimumdemerits < awfulbad && (oldl != easyline || r == lastactive)
            /*:836*/
            if (r == lastactive) goto _Lexit; /*850:*/
            if (l > easyline) {
                linewidth = secondwidth;
                oldl = maxhalfword - 1;
            } else { /*:850*/
                oldl = l;
                if (l > lastspecialline)
                    linewidth = secondwidth;
                else if (parshapeptr == 0)
                    linewidth = firstwidth;
                else
                    linewidth = mem[parshapeptr + l * 2 - memmin].sc;
            } // if (l > easyline) - else
        } // if (l > oldl)

        /*851:*/
        artificialdemerits = false;
        shortfall = linewidth - curactivewidth[0];
        if (shortfall > 0) { /*852:*/
            if (curactivewidth[2] != 0 || curactivewidth[3] != 0 ||
                curactivewidth[4] != 0) {
                b = 0;
                fitclass = decentfit;
            } else { /*:852*/
                if (shortfall > 7230584L) {
                    if (curactivewidth[1] < 1663497L) {
                        b = INF_BAD;
                        fitclass = veryloosefit;
                        goto _Ldone1;
                    }
                }
                b = badness(shortfall, curactivewidth[1]);
                if (b > 12) {
                    if (b > 99)
                        fitclass = veryloosefit;
                    else
                        fitclass = loosefit;
                } else
                    fitclass = decentfit;
            _Ldone1:;
            }
        } else /*853:*/
        {      /*:853*/
            if (-shortfall > curactivewidth[5])
                b = INF_BAD + 1;
            else
                b = badness(-shortfall, curactivewidth[5]);
            if (b > 12)
                fitclass = tightfit;
            else
                fitclass = decentfit;
        }
        if (b > INF_BAD || pi == ejectpenalty) { /*854:*/
            if (((finalpass && minimumdemerits == awfulbad) &
                 (link(r) == lastactive)) &&
                prevr == active)
                artificialdemerits = true;
            else if (b > threshold)
                goto _Ldeactivate_;
            noderstaysactive = false;
        } /*:854*/
        else {
            prevr = r;
            if (b > threshold) goto _Llabcontinue;
            noderstaysactive = true;
        } /*855:*/
        if (artificialdemerits)
            d = 0;
        else { /*859:*/
            d = linepenalty + b;
            if (labs(d) >= 10000)
                d = 100000000L;
            else
                d *= d;
            if (pi != 0) {
                if (pi > 0)
                    d += pi * pi;
                else if (pi > ejectpenalty)
                    d -= pi * pi;
            }
            if ((breaktype == hyphenated) & (type(r) == hyphenated)) {
                if (curp != 0)
                    d += doublehyphendemerits;
                else
                    d += finalhyphendemerits;
            }
            if (labs(fitclass - fitness(r)) > 1) d += adjdemerits;
        }

        #ifdef tt_STAT
            if (tracingparagraphs > 0) { /*:856*/
                /*856:*/
                if (printednode != curp) { /*857:*/
                    printnl(S(385));
                    if (curp == 0)
                        shortdisplay(link(printednode));
                    else {
                        savelink = link(curp);
                        link(curp) = 0;
                        printnl(S(385));
                        shortdisplay(link(printednode));
                        link(curp) = savelink;
                    }
                    printednode = curp;
                }
                /*:857*/
                printnl('@');
                if (curp == 0)
                    print_esc(S(760));
                else if (type(curp) != gluenode) {
                    if (type(curp) == penaltynode)
                        print_esc(S(761));
                    else if (type(curp) == discnode)
                        print_esc(S(400));
                    else if (type(curp) == kernnode)
                        print_esc(S(391));
                    else
                        print_esc(S(394));
                }
                print(S(762));
                if (breaknode(r) == 0)
                    print_char('0');
                else
                    print_int(serial(breaknode(r)));
                print(S(763));
                if (b > INF_BAD)
                    print_char('*');
                else
                    print_int(b);
                print(S(764));
                print_int(pi);
                print(S(765));
                if (artificialdemerits)
                    print_char('*');
                else
                    print_int(d);
            }
        #endif // #855: tt_STAT

        d += totaldemerits(r);
        if (d <= minimaldemerits[fitclass - veryloosefit]) { /*:855*/
            minimaldemerits[fitclass - veryloosefit] = d;
            bestplace[fitclass - veryloosefit] = breaknode(r);
            bestplline[fitclass - veryloosefit] = l;
            if (d < minimumdemerits) minimumdemerits = d;
        }
        if (noderstaysactive) goto _Llabcontinue;
    _Ldeactivate_: /*860:*/
        link(prevr) = link(r);
        freenode(r, activenodesize);
        if (prevr == active) { /*861:*/
            r = link(active);
            if (type(r) != deltanode) continue;
            updateactive(1);
            updateactive(2);
            updateactive(3);
            updateactive(4);
            updateactive(5);
            updateactive(6);
            copytocuractive(1);
            copytocuractive(2);
            copytocuractive(3);
            copytocuractive(4);
            copytocuractive(5);
            copytocuractive(6);
            link(active) = link(r);
            freenode(r, deltanodesize);
            continue;
        } /*:861*/
        /*:851*/
        if (type(prevr) != deltanode) /*:860*/
            continue;
        r = link(prevr);
        if (r == lastactive) {
            downdatewidth(1);
            downdatewidth(2);
            downdatewidth(3);
            downdatewidth(4);
            downdatewidth(5);
            downdatewidth(6);
            link(prevprevr) = lastactive;
            freenode(prevr, deltanodesize);
            prevr = prevprevr;
            continue;
        }
        if (type(r) != deltanode) continue;
        updatewidth(1);
        updatewidth(2);
        updatewidth(3);
        updatewidth(4);
        updatewidth(5);
        updatewidth(6);
        combinetwodeltas(1);
        combinetwodeltas(2);
        combinetwodeltas(3);
        combinetwodeltas(4);
        combinetwodeltas(5);
        combinetwodeltas(6);
        link(prevr) = link(r);
        freenode(r, deltanodesize);
    } // while (true)

    _Lexit:
    /// p317#858: Update the value of printed node for symbolic displays
    #ifdef tt_STAT
        if (curp != printednode) return;
        if (curp == 0) return;
        if (type(curp) != discnode) return;
        t = replacecount(curp);
        while (t > 0) {
            t--;
            printednode = link(printednode);
        }
    #endif // #829: tt_STAT
       /*:843*/
       /*:859*/
    return;
} // #829: trybreak


/*877:*/
Static void postlinebreak(long finalwidowpenalty)
{   /*878:*/
  Pointer q, r, s;
  Boolean discbreak, postdiscbreak;
  Scaled curwidth, curindent;
  QuarterWord t;
  long pen;
  HalfWord curline;

  q = breaknode(bestbet);
  curp = 0;
  do {
    r = q;
    q = prevbreak(q);
    nextbreak(r) = curp;
    curp = r;   /*:878*/
  } while (q != 0);
  curline = prevgraf + 1;
  do {   /*880:*/
    q = curbreak(curp);
    discbreak = false;
    postdiscbreak = false;
    if (q != 0) {
      if (type(q) == gluenode) {
	deleteglueref(glueptr(q));
	glueptr(q) = rightskip;
	subtype(q) = rightskipcode + 1;
	addglueref(rightskip);
	goto _Ldone;
      }
      if (type(q) == discnode) {   /*882:*/
	t = replacecount(q);   /*883:*/
	if (t == 0)
	  r = link(q);
	else {   /*:883*/
	  r = q;
	  while (t > 1) {
	    r = link(r);
	    t--;
	  }
	  s = link(r);
	  r = link(s);
	  link(s) = 0;
	  flushnodelist(link(q));
	  replacecount(q) = 0;
	}
	if (postbreak(q) != 0) {   /*884:*/
	  s = postbreak(q);
	  while (link(s) != 0)
	    s = link(s);
	  link(s) = r;
	  r = postbreak(q);
	  postbreak(q) = 0;
	  postdiscbreak = true;
	}
	/*:884*/
	if (prebreak(q) != 0) {   /*885:*/
	  s = prebreak(q);
	  link(q) = s;
	  while (link(s) != 0)
	    s = link(s);
	  prebreak(q) = 0;
	  q = s;
	}  /*:885*/
	link(q) = r;
	discbreak = true;
      }  /*:882*/
      else if ((type(q) == mathnode) | (type(q) == kernnode))
	width(q) = 0;
    } else {
      q = temphead;
      while (link(q) != 0)
	q = link(q);
    }
    /*886:*/
    r = newparamglue(rightskipcode);
    link(r) = link(q);
    link(q) = r;
    q = r;   /*:886*/
_Ldone:   /*:881*/
    /*887:*/
    r = link(q);
    link(q) = 0;
    q = link(temphead);
    link(temphead) = r;
    if (leftskip != zeroglue) {   /*:887*/
      r = newparamglue(leftskipcode);
      link(r) = q;
      q = r;
    }
    /*889:*/
    if (curline > lastspecialline) {
      curwidth = secondwidth;
      curindent = secondindent;
    } else if (parshapeptr == 0) {
      curwidth = firstwidth;
      curindent = firstindent;
    } else {
      curwidth = mem[parshapeptr + curline * 2 - memmin].sc;
      curindent = mem[parshapeptr + curline * 2 - memmin - 1].sc;
    }
    adjusttail = adjusthead;
    justbox = hpack(q, curwidth, exactly);
    shiftamount(justbox) = curindent;   /*:889*/
    /*888:*/
    appendtovlist(justbox);
    if (adjusthead != adjusttail) {
      link(tail) = link(adjusthead);
      tail = adjusttail;
    }
    adjusttail = 0;   /*:888*/
    /*890:*/
    if (curline + 1 != bestline) {   /*:890*/
      pen = interlinepenalty;
      if (curline == prevgraf + 1)
	pen += clubpenalty;
      if (curline + 2 == bestline)
	pen += finalwidowpenalty;
      if (discbreak)
	pen += brokenpenalty;
      if (pen != 0) {
	r = newpenalty(pen);
	link(tail) = r;
	tail = r;
      }
    }
    /*:880*/
    curline++;
    curp = nextbreak(curp);
    if (curp != 0) {
      if (!postdiscbreak) {   /*879:*/
	r = temphead;
	while (true) {
	  q = link(r);
	  if (q == curbreak(curp))
	    goto _Ldone1;
	  if (ischarnode(q))
	    goto _Ldone1;
	  if (nondiscardable(q)) {
	    goto _Ldone1;
	  }
	  if (type(q) == kernnode) {
	    if (subtype(q) != explicit)
	      goto _Ldone1;
	  }
	  r = q;
	}
_Ldone1:
	if (r != temphead) {
	  link(r) = 0;
	  flushnodelist(link(temphead));
	  link(temphead) = q;
	}
      }
      /*:879*/
    }
  } while (curp != 0);
  /*881:*/
  if ((curline != bestline) | (link(temphead) != 0))
    confusion(S(766));
  prevgraf = bestline - 1;
}
/*:877*/

/*895:*/
/*906:*/
Static SmallNumber reconstitute(/* SmallNumber */ int j, SmallNumber n, HalfWord bchar,
				HalfWord hchar)
{
  Pointer p, t;
  FourQuarters q;
  HalfWord currh, testchar;
  Scaled w;
  FontIndex k;

  hyphenpassed = 0;
  t = holdhead;
  w = 0;
  link(holdhead) = 0;   /*908:*/
  curl = hu[j];
  curq = t;
  if (j == 0) {
    ligaturepresent = initlig;
    p = initlist;
    if (ligaturepresent)
      lfthit = initlft;
    while (p > 0) {
      appendcharnodetot(character(p));
      p = link(p);
    }
  } else if (curl < nonchar) {
    appendcharnodetot(curl);
  }
  ligstack = 0;   /*:908*/
  setcurr();
_Llabcontinue:   /*909:*/
  if (curl == nonchar) {
    k = bcharlabel[hf ];
    if (k == nonaddress)
      goto _Ldone;
    q = fontinfo[k].qqqq;
  } else {
    q = charinfo(hf, curl);
    if (chartag(q) != ligtag) {
      goto _Ldone;
    }
    k = ligkernstart(hf,q);
    q = fontinfo[k].qqqq;
    if (skipbyte(q) > stopflag) {
      k = ligkernrestart(hf,q);
      q = fontinfo[k].qqqq;
    }
  }
  if (currh < nonchar)
    testchar = currh;
  else
    testchar = curr;
  while (true) {
    if (nextchar(q) == testchar) {
      if (skipbyte(q) <= stopflag) {
	if (currh < nonchar) {
	  hyphenpassed = j;
	  hchar = nonchar;
	  currh = nonchar;
	  goto _Llabcontinue;
	} else {
	  if (hchar < nonchar) {
	    if (hyf[j] & 1) {
	      hyphenpassed = j;
	      hchar = nonchar;
	    }
	  }
	  if (opbyte(q) < kernflag) {   /*911:*/
	    if (curl == nonchar)
	      lfthit = true;
	    if (j == n) {
	      if (ligstack == 0)
		rthit = true;
	    }
	    checkinterrupt();
	    switch (opbyte(q)) {

	    case minquarterword + 1:
	    case minquarterword + 5:
	      curl = rembyte(q);
	      ligaturepresent = true;
	      break;

	    case minquarterword + 2:
	    case minquarterword + 6:
	      curr = rembyte(q);
	      if (ligstack > 0)
		character(ligstack) = curr;
	      else {
		ligstack = newligitem(curr);
		if (j == n)
		  bchar = nonchar;
		else {
		  p = get_avail();
		  ligptr(ligstack) = p;
		  character(p) = hu[j + 1];
		  font(p) = hf;
		}
	      }
	      break;

	    case minquarterword + 3:
	      curr = rembyte(q);
	      p = ligstack;
	      ligstack = newligitem(curr);
	      link(ligstack) = p;
	      break;

	    case minquarterword + 7:
	    case minquarterword + 11:
	      wraplig(false);
	      curq = t;
	      curl = rembyte(q);
	      ligaturepresent = true;
	      break;

	    default:
	      curl = rembyte(q);
	      ligaturepresent = true;
	      if (ligstack > 0) {
		popligstack();
	      } else if (j == n)
		goto _Ldone;
	      else {
		appendcharnodetot(curr);
		j++;
		setcurr();
	      }
	      break;
	    }
	    if (opbyte(q) > minquarterword + 4) {
	      if (opbyte(q) != minquarterword + 7)
		goto _Ldone;
	    }
	    goto _Llabcontinue;
	  }
	  /*:911*/
	  w = charkern(hf, q);
	  goto _Ldone;
	}
      }
    }
    if (skipbyte(q) >= stopflag) {
      if (currh == nonchar)
	goto _Ldone;
      else {
	currh = nonchar;
	goto _Llabcontinue;
      }
    }
    k += skipbyte(q) - minquarterword + 1;
    q = fontinfo[k].qqqq;
  }
_Ldone:   /*:909*/
  /*910:*/
  wraplig(rthit);
  if (w != 0) {
    link(t) = newkern(w);
    t = link(t);
    w = 0;
  }
  if (ligstack <= 0)   /*:910*/
    return j;
  curq = t;
  curl = character(ligstack);
  ligaturepresent = true;
  popligstack();
  goto _Llabcontinue;
}
/*:906*/

Static void hyphenate(void)   /*:929*/
{  /*923:*/
  /*901:*/
  /* char */ int i, j, l; /* INT */
  Pointer q, r, s;
  HalfWord bchar;
  /*:901*/
  /*912:*/
  Pointer majortail, minortail, hyfnode;
  ASCIICode c=0 /* XXXX */;
  /* char */ int cloc; /* INT */
  long rcount;
  /*:912*/
  /*922:*/
  TriePointer z;
  long v;
  /*:922*/
  /*929:*/
  HyphPointer h;
  StrNumber k;
  char FORLIM;

  for (j = 0; j <= hn; j++)   /*930:*/
    hyf[j] = 0;
  h = hc[1];
  hn++;
  hc[hn] = curlang;
  for (j = 2; j <= hn ; j++)
    h = (h + h + hc[j]) % hyphsize;
  while (true) {  /*931:*/
    k = hyphword[h];
    if (k == 0)
      goto _Lnotfound;
    if (str_length(k) < hn) {
      goto _Lnotfound;
    }
    if (str_length(k) == hn) {
	{
        int ress=str_scmp(k,hc+1);
	if(ress<0) goto _Lnotfound;
	if(ress>0) goto _Ldone;
	}
      s = hyphlist[h];
      while (s != 0) {   /*:932*/
	hyf[info(s)] = 1;
	s = link(s);
      }
      hn--;
      goto _Lfound;
    }
_Ldone:   /*:931*/
    if (h > 0)
      h--;
    else
      h = hyphsize;
  }
_Lnotfound:
  hn--;   /*:930*/
  if (triechar(curlang + 1) != curlang)
    goto _Lexit;
  hc[0] = 0;
  hc[hn + 1] = 0;
  hc[hn + 2] = 256;
  FORLIM = hn - rhyf + 1;
  for (j = 0; j <= FORLIM; j++) {
    z = trielink(curlang + 1) + hc[j];
    l = j;
    while (hc[l] == triechar(z) - minquarterword) {
      if (trieop(z) != minquarterword) {   /*924:*/
	v = trieop(z);
	do {
	  v += opstart[curlang];
	  i = l - hyfdistance[v - 1];
	  if (hyfnum[v - 1] > hyf[i])
	    hyf[i] = hyfnum[v - 1];
	  v = hyfnext[v - 1];
	} while (v != minquarterword);
      }
      /*:924*/
      l++;
      z = trielink(z) + hc[l];
    }
  }
_Lfound:
  for (j = 0; j < lhyf; j++)
    hyf[j] = 0;
  for (j = 0; j < rhyf; j++)   /*902:*/
    hyf[hn - j] = 0;   /*:923*/
  for (j = lhyf; j <= hn - rhyf; j++) {
    if (hyf[j] & 1)
      goto _Lfound1;
  }
  goto _Lexit;
_Lfound1:   /*:902*/
  /*903:*/
  q = link(hb);
  link(hb) = 0;
  r = link(ha);
  link(ha) = 0;
  bchar = hyfbchar;
  if (ischarnode(ha)) {
    if (font(ha) != hf)
      goto _Lfound2;
    initlist = ha;
    initlig = false;
    hu[0] = character(ha) - minquarterword;
  } else if (type(ha) == ligaturenode) {
    if (font_ligchar(ha) != hf) {
      goto _Lfound2;
    }
    initlist = ligptr(ha);
    initlig = true;
    initlft = (subtype(ha) > 1);
    hu[0] = character_ligchar(ha) - minquarterword;
    if (initlist == 0) {
      if (initlft) {
	hu[0] = 256;
	initlig = false;
      }
    }
    freenode(ha, smallnodesize);
  } else {
    if (!ischarnode(r)) {
      if (type(r) == ligaturenode) {
	if (subtype(r) > 1)
	  goto _Lfound2;
      }
    }
    j = 1;
    s = ha;
    initlist = 0;
    goto _Lcommonending;
  }
  s = curp;
  while (link(s) != ha)
    s = link(s);
  j = 0;
  goto _Lcommonending;
_Lfound2:
  s = ha;
  j = 0;
  hu[0] = 256;
  initlig = false;
  initlist = 0;
_Lcommonending:
  flushnodelist(r);   /*913:*/
  do {
    l = j;
    j = reconstitute(j, hn, bchar, hyfchar) + 1;
    if (hyphenpassed == 0) {
      link(s) = link(holdhead);
      while (link(s) > 0)
	s = link(s);
      if (hyf[j - 1] & 1) {
	l = j;
	hyphenpassed = j - 1;
	link(holdhead) = 0;
      }
    }
    if (hyphenpassed > 0) {   /*914:*/
      do {
	r = getnode(smallnodesize);
	link(r) = link(holdhead);
	type(r) = discnode;
	majortail = r;
	rcount = 0;
	while (link(majortail) > 0) {
	  advancemajortail();
	}
	i = hyphenpassed;
	hyf[i] = 0;   /*915:*/
	minortail = 0;
	prebreak(r) = 0;
	hyfnode = newcharacter(hf, hyfchar);
	if (hyfnode != 0) {
	  i++;
	  c = hu[i];
	  hu[i] = hyfchar;
	  FREE_AVAIL(hyfnode);
	}
	while (l <= i) {
	  l = reconstitute(l, i, fontbchar[hf ], nonchar) + 1;
	  if (link(holdhead) <= 0)
	    continue;
	  if (minortail == 0)
	    prebreak(r) = link(holdhead);
	  else
	    link(minortail) = link(holdhead);
	  minortail = link(holdhead);
	  while (link(minortail) > 0)
	    minortail = link(minortail);
	}
	if (hyfnode != 0) {   /*:915*/
	  hu[i] = c;
	  l = i;
	  i--;
	}
	/*916:*/
	minortail = 0;
	postbreak(r) = 0;
	cloc = 0;
	if (bcharlabel[hf ] != nonaddress) {
	  l--;
	  c = hu[l];
	  cloc = l;
	  hu[l] = 256;
	}
	while (l < j) {   /*:916*/
	  do {
	    l = reconstitute(l, hn, bchar, nonchar) + 1;
	    if (cloc > 0) {
	      hu[cloc] = c;
	      cloc = 0;
	    }
	    if (link(holdhead) > 0) {
	      if (minortail == 0)
		postbreak(r) = link(holdhead);
	      else
		link(minortail) = link(holdhead);
	      minortail = link(holdhead);
	      while (link(minortail) > 0)
		minortail = link(minortail);
	    }
	  } while (l < j);
	  while (l > j) {   /*917:*/
	    j = reconstitute(j, hn, bchar, nonchar) + 1;
	    link(majortail) = link(holdhead);
	    while (link(majortail) > 0) {
	      advancemajortail();
	    }
	  }
	  /*:917*/
	}
	/*918:*/
	if (rcount > 127) {
	  link(s) = link(r);
	  link(r) = 0;
	  flushnodelist(r);
	} else {
	  link(s) = r;
	  replacecount(r) = rcount;
	}
	s = majortail;   /*:918*/
	hyphenpassed = j - 1;
	link(holdhead) = 0;   /*:914*/
      } while (hyf[j - 1] & 1);
    }
  } while (j <= hn);
  link(s) = q;   /*:913*/
  /*:903*/
  flushlist(initlist);
_Lexit: ;
}
/*:895*/


/// p350#942: 944, 948, 949, 953, 957, 959, 960, and 966.
#ifdef tt_INIT

/// p351#944: Declare procedures for preprocessing hyphenation patterns
Static QuarterWord newtrieop(SmallNumber d, SmallNumber n, QuarterWord v) {
    QuarterWord Result;
    short h;
    QuarterWord u;
    short l;

    h = abs(n + d * 313 + v * 361 + curlang * 1009) %
            (trieopsize + trieopsize) - trieopsize;
    while (true) {
        l = trieophash[h + trieopsize];
        if (l == 0) {
            if (trieopptr == trieopsize) overflow(S(767), trieopsize);
            u = trieused[curlang];
            if (u == maxquarterword)
                overflow(S(768), maxquarterword - minquarterword);
            trieopptr++;
            u++;
            trieused[curlang] = u;
            hyfdistance[trieopptr - 1] = d;
            hyfnum[trieopptr - 1] = n;
            hyfnext[trieopptr - 1] = v;
            trieoplang[trieopptr - 1] = curlang;
            trieophash[h + trieopsize] = trieopptr;
            trieopval[trieopptr - 1] = u;
            Result = u;
            break;
        }
        if (hyfdistance[l - 1] == d && hyfnum[l - 1] == n &&
            hyfnext[l - 1] == v && trieoplang[l - 1] == curlang) {
            Result = trieopval[l - 1];
            break;
        }
        if (h > -trieopsize)
            h--;
        else
            h = trieopsize;
    } // while (true)

    return Result;
} // newtrieop


/// #948: Declare procedures for preprocessing hyphenation patterns
Static TriePointer trienode(TriePointer p) {
    TriePointer Result, h, q;

    h = abs(triec[p] + trieo[p] * 1009 + triel[p] * 2718 + trier[p] * 3142) %
        triesize;
    while (true) {
        q = triehash[h];
        if (q == 0) {
            triehash[h] = p;
            Result = p;
            break;
        }
        if (triec[q] == triec[p] && trieo[q] == trieo[p] &&
            triel[q] == triel[p] && trier[q] == trier[p]) {
            Result = q;
            break;
        }
        if (h > 0)
            h--;
        else
            h = triesize;
    } // while (true)

    return Result;
} // trienode


/// #949: Declare procedures for preprocessing hyphenation patterns
Static TriePointer compresstrie(TriePointer p)
{
  if (p == 0)
    return 0;
  else {
    triel[p] = compresstrie(triel[p]);
    trier[p] = compresstrie(trier[p]);
    return (trienode(p));
  }
} // compresstrie

/// #953: Declare procedures for preprocessing hyphenation patterns
Static void firstfit(TriePointer p)
{
  TriePointer h, z, q;
  ASCIICode c;
  TriePointer l, r;
  short ll;

  c = triec[p];
  z = triemin[c];
  while (true) {
    h = z - c;   /*954:*/
    if (triemax < h + 256) {   /*:954*/
      if (triesize <= h + 256)
	overflow(S(769), triesize);
      do {
	triemax++;
	P_clrbits_B(trietaken, triemax - 1, 0, 3);
	trielink(triemax) = triemax + 1;
	trieback(triemax) = triemax - 1;
      } while (triemax != h + 256);
    }
    if (P_getbits_UB(trietaken, h - 1, 0, 3))   /*955:*/
      goto _Lnotfound;
    q = trier[p];
    while (q > 0) {
      if (trielink(h + triec[q]) == 0)
	goto _Lnotfound;
      q = trier[q];
    }
    goto _Lfound;   /*:955*/
_Lnotfound:
    z = trielink(z);
  }
_Lfound:   /*956:*/
  P_putbits_UB(trietaken, h - 1, 1, 0, 3);
  triehash[p] = h;
  q = p;
  do {
    z = h + triec[q];
    l = trieback(z);
    r = trielink(z);
    trieback(r) = l;
    trielink(l) = r;
    trielink(z) = 0;
    if (l < 256) {
      if (z < 256)
	ll = z;
      else
	ll = 256;
      do {
	triemin[l] = r;
	l++;
      } while (l != ll);
    }
    q = trier[q];   /*:956*/
  } while (q != 0);
} // firstfit


/// #957: Declare procedures for preprocessing hyphenation patterns
Static void triepack(TriePointer p)
{
  TriePointer q;

  do {
    q = triel[p];
    if (q > 0 && triehash[q] == 0) {
      firstfit(q);
      triepack(q);
    }
    p = trier[p];
  } while (p != 0);
} // triepack

/// #959: Declare procedures for preprocessing hyphenation patterns
Static void triefix(TriePointer p)
{
  TriePointer q;
  ASCIICode c;
  TriePointer z;

  z = triehash[p];
  do {
    q = triel[p];
    c = triec[p];
    trielink(z + c) = triehash[q];
    triechar(z + c) = c;
    trieop(z + c) = trieo[p];
    if (q > 0)
      triefix(q);
    p = trier[p];
  } while (p != 0);
} // triefix

/// #960: Declare procedures for preprocessing hyphenation patterns
Static void newpatterns(void) {
    unsigned char k, l; /* INT */
    Boolean digitsensed;
    QuarterWord v;
    TriePointer p, q;
    Boolean firstchild;
    ASCIICode c;

    if (trie_not_ready) {
        setcurlang();
        scanleftbrace();
        /*961:*/
        k = 0;
        hyf[0] = 0;
        digitsensed = false;
        while (true) {
            getxtoken();
            switch (curcmd) {

                case letter:
                case otherchar: /*962:*/
                    if (digitsensed | (curchr < '0') | (curchr > '9')) {
                        int cur_chr;
                        if (curchr == '.')
                            cur_chr = 0;
                        else {
                            cur_chr = lccode(curchr);
                            if (cur_chr == 0) {
                                printnl(S(292));
                                print(S(770));
                                help1(S(771));
                                error();
                            }
                        }
                        if (k < 63) {
                            k++;
                            hc[k] = cur_chr;
                            hyf[k] = 0;
                            digitsensed = false;
                        }
                    } else if (k < 63) {
                        hyf[k] = curchr - '0';
                        digitsensed = true;
                    }
                    break;

                case spacer:
                case rightbrace:
                    if (k > 0) { /*963:*/
                        if (hc[1] == 0) hyf[0] = 0;
                        if (hc[k] == 0) hyf[k] = 0;
                        l = k;
                        v = minquarterword;
                        while (true) {
                            if (hyf[l] != 0) v = newtrieop(k - l, hyf[l], v);
                            if (l <= 0) goto _Ldone1;
                            l--;
                        }
_Ldone1: /*:965*/
                        q = 0;
                        hc[0] = curlang;
                        while (l <= k) {
                            c = hc[l];
                            l++;
                            p = triel[q];
                            firstchild = true;
                            while (p > 0 && c > triec[p]) {
                                q = p;
                                p = trier[q];
                                firstchild = false;
                            }
                            if (p == 0 || c < triec[p]) { /*964:*/
                                if (trieptr == triesize)
                                    overflow(S(769), triesize);
                                trieptr++;
                                trier[trieptr] = p;
                                p = trieptr;
                                triel[p] = 0;
                                if (firstchild)
                                    triel[q] = p;
                                else
                                    trier[q] = p;
                                triec[p] = c;
                                trieo[p] = minquarterword;
                            }
                            /*:964*/
                            q = p;
                        }
                        if (trieo[q] != minquarterword) {
                            printnl(S(292));
                            print(S(772));
                            help1(S(771));
                            error();
                        }
                        trieo[q] = v;
                    }
                    /*965:*/
                    /*:963*/
                    if (curcmd == rightbrace) goto _Ldone;
                    k = 0;
                    hyf[0] = 0;
                    digitsensed = false;
                    break;

                default:
                    printnl(S(292));
                    print(S(773));
                    print_esc(S(774));
                    help1(S(771));
                    error();
                    break;
            }
        }
_Ldone: /*:961*/
        return;
    }
    printnl(S(292));
    print(S(775));
    print_esc(S(774));
    help1(S(776));
    error();
    link(garbage) = scantoks(false, false);
    flushlist(defref);
    /*:962*/
} // newpatterns

/// 966: Declare procedures for preprocessing hyphenation patterns
Static void inittrie(void)
{   /*952:*/
  TriePointer p;
  long j, k, t;
  TriePointer r, s;
  TwoHalves h;

  /*945:*/
  opstart[0] = -minquarterword;
  for (j = 1; j <= 255; j++)
    opstart[j] = opstart[j - 1] + trieused[j - 1] - minquarterword;
  for (j = 1; j <= trieopptr; j++)
    trieophash[j + trieopsize] = opstart[trieoplang[j - 1]] + trieopval[j - 1];
  for (j = 1; j <= trieopptr; j++) {
    while (trieophash[j + trieopsize] > j) {   /*:945*/
      k = trieophash[j + trieopsize];
      t = hyfdistance[k - 1];
      hyfdistance[k - 1] = hyfdistance[j - 1];
      hyfdistance[j - 1] = t;
      t = hyfnum[k - 1];
      hyfnum[k - 1] = hyfnum[j - 1];
      hyfnum[j - 1] = t;
      t = hyfnext[k - 1];
      hyfnext[k - 1] = hyfnext[j - 1];
      hyfnext[j - 1] = t;
      trieophash[j + trieopsize] = trieophash[k + trieopsize];
      trieophash[k + trieopsize] = k;
    }
  }
  for (p = 0; p <= triesize; p++)
    triehash[p] = 0;
  trieroot = compresstrie(trieroot);
  for (p = 0; p <= trieptr; p++)
    triehash[p] = 0;
  for (p = 0; p <= 255; p++)
    triemin[p] = p + 1;
  trielink(0) = 1;
  triemax = 0;   /*:952*/
  if (trieroot != 0) {
    firstfit(trieroot);
    triepack(trieroot);
  }
  /*958:*/
  h.rh = 0;
  h.UU.U2.b0 = minquarterword;
  h.UU.U2.b1 = minquarterword;
  if (trieroot == 0) {
    for (r = 0; r <= 256; r++)
      trie[r] = h;
    triemax = 256;
  } else {
    triefix(trieroot);
    r = 0;
    do {
      s = trielink(r);
      trie[r] = h;
      r = s;
    } while (r <= triemax);
  }
  triechar(0) = '?';   /*:958*/
  trie_not_ready = false;
} // inittrie

#endif // #942: tt_INIT


Static void linebreak(long finalwidowpenalty) {
    /*862:*/
    Boolean autobreaking;
    Pointer prevp, q, r, s, prevs;
    InternalFontNumber f;
    /*:862*/
    /*893:*/
    /* SmallNumber */ int j; /* INT */
    UChar c;                 /*:893*/

    packbeginline = modeline; /*816:*/
    link(temphead) = link(head);
    if (ischarnode(tail)) {
        tailappend(newpenalty(infpenalty));
    } else if (type(tail) != gluenode) {
        tailappend(newpenalty(infpenalty));
    } else {
        type(tail) = penaltynode;
        deleteglueref(glueptr(tail));
        flushnodelist(leaderptr(tail));
        penalty(tail) = infpenalty;
    }
    link(tail) = newparamglue(parfillskipcode);
    initcurlang = prevgraf % 65536L;
    initlhyf = prevgraf / 4194304L;
    initrhyf = (prevgraf / 65536L) & 63;
    popnest(); /*:816*/
    /*827:*/
    noshrinkerroryet = true;
    checkshrinkage(leftskip);
    checkshrinkage(rightskip);
    q = leftskip;
    r = rightskip;
    background[0] = width(q) + width(r);
    background[1] = 0;
    background[2] = 0;
    background[3] = 0;
    background[4] = 0;
    background[stretchorder(q) + 1] = stretch(q);
    background[stretchorder(r) + 1] += stretch(r);
    background[5] = shrink(q) + shrink(r); /*:827*/
    /*834:*/
    minimumdemerits = awfulbad;
    minimaldemerits[tightfit - veryloosefit] = awfulbad;
    minimaldemerits[decentfit - veryloosefit] = awfulbad;
    minimaldemerits[loosefit - veryloosefit] = awfulbad;
    minimaldemerits[0] = awfulbad; /*:834*/
    /*848:*/
    if (parshapeptr == 0) {
        if (hangindent == 0) {
            lastspecialline = 0;
            secondwidth = hsize;
            secondindent = 0;
        } else /*849:*/
        {      /*:849*/
            lastspecialline = labs(hangafter);
            if (hangafter < 0) {
                firstwidth = hsize - labs(hangindent);
                if (hangindent >= 0)
                    firstindent = hangindent;
                else
                    firstindent = 0;
                secondwidth = hsize;
                secondindent = 0;
            } else {
                firstwidth = hsize;
                firstindent = 0;
                secondwidth = hsize - labs(hangindent);
                if (hangindent >= 0)
                    secondindent = hangindent;
                else
                    secondindent = 0;
            }
        }
    } else {
        lastspecialline = info(parshapeptr) - 1;
        secondwidth = mem[parshapeptr + (lastspecialline + 1) * 2 - memmin].sc;
        secondindent = mem[parshapeptr + lastspecialline * 2 - memmin + 1].sc;
    }
    if (looseness == 0)
        easyline = lastspecialline;
    else {
        easyline = maxhalfword;
        /*:848*/
    }
    /*863:*/
    threshold = pretolerance;
    if (threshold >= 0) {           
        #ifdef tt_STAT
            if (tracingparagraphs > 0) {
                begindiagnostic();
                printnl(S(777));
            }
        #endif // #863.1: tt_STAT
        secondpass = false;
        finalpass = false;
    } else {
        threshold = tolerance;
        secondpass = true;
        finalpass = (emergencystretch <= 0);
        #ifdef tt_STAT
            if (tracingparagraphs > 0) begindiagnostic();
        #endif // #863.2: tt_STAT
    }
    while (true) {
        if (threshold > INF_BAD) threshold = INF_BAD;
        if (secondpass) { /*891:*/
#ifdef tt_INIT
            if (trie_not_ready) inittrie();
#endif // #891: tt_INIT
            curlang = initcurlang;
            lhyf = initlhyf;
            rhyf = initrhyf;
        }
        /*:891*/
        /*864:*/
        q = getnode(activenodesize);
        type(q) = unhyphenated;
        fitness(q) = decentfit;
        link(q) = lastactive;
        breaknode(q) = 0;
        linenumber(q) = prevgraf + 1;
        totaldemerits(q) = 0;
        link(active) = q;
        storebackground(1);
        storebackground(2);
        storebackground(3);
        storebackground(4);
        storebackground(5);
        storebackground(6);
        passive = 0;
        printednode = temphead;
        passnumber = 0;
        fontinshortdisplay = nullfont; /*:864*/
        curp = link(temphead);
        autobreaking = true;
        prevp = curp;
        while ((curp != 0) & (link(active) != lastactive)) { /*866:*/
            if (ischarnode(curp)) {                          /*867:*/
                prevp = curp;
                do {
                    f = font(curp);
                    actwidth += charwidth(f, charinfo(f, character(curp)));
                    curp = link(curp);
                } while (ischarnode(curp));
            }
            /*:867*/
            switch (type(curp)) {

                case hlistnode:
                case vlistnode:
                case rulenode:
                    actwidth += width(curp);
                    break;

                case whatsitnode:  /*1362:*/
                    advpast(curp); /*:1362*/
                    break;

                case gluenode: /*868:*/
                    if (autobreaking) {
                        if (ischarnode(prevp))
                            trybreak(0, unhyphenated);
                        else if (precedesbreak(prevp)) {
                            trybreak(0, unhyphenated);
                        } else if ((type(prevp) == kernnode) &
                                   (subtype(prevp) != explicit))
                            trybreak(0, unhyphenated);
                    }
                    checkshrinkage(glueptr(curp));
                    q = glueptr(curp);
                    actwidth += width(q);
                    activewidth[stretchorder(q) + 1] += stretch(q);
                    activewidth[5] += shrink(q);      /*:868*/
                    if (secondpass && autobreaking) { /*894:*/
                        prevs = curp;
                        s = link(prevs);
                        if (s != 0) { /*896:*/
                            while (true) {
                                if (ischarnode(s)) {
                                    c = character(s) - minquarterword;
                                    hf = font(s);
                                } else if (type(s) == ligaturenode) {
                                    if (ligptr(s) == 0) goto _Llabcontinue;
                                    q = ligptr(s);
                                    c = character(q) - minquarterword;
                                    hf = font(q);
                                } else if ((type(s) == kernnode) &
                                           (subtype(s) == normal))
                                    goto _Llabcontinue;
                                else if (type(s) == whatsitnode) {
                                    advpast(s); /*:1363*/
                                    goto _Llabcontinue;
                                } else
                                    goto _Ldone1;
                                if (lccode(c) != 0) {
                                    if (lccode(c) == c || uchyph > 0)
                                        goto _Ldone2;
                                    else
                                        goto _Ldone1;
                                }
                            _Llabcontinue:
                                prevs = s;
                                s = link(prevs);
                            }
                        _Ldone2:
                            hyfchar = get_hyphenchar(hf);
                            if (hyfchar < 0) goto _Ldone1;
                            if (hyfchar > 255) goto _Ldone1;
                            ha = prevs;           /*:896*/
                            if (lhyf + rhyf > 63) /*897:*/
                                goto _Ldone1;
                            hn = 0;
                            while (true) {
                                if (ischarnode(s)) {
                                    if (font(s) != hf) goto _Ldone3;
                                    hyfbchar = character(s);
                                    c = hyfbchar - minquarterword;
                                    if (lccode(c) == 0) goto _Ldone3;
                                    if (hn == 63) goto _Ldone3;
                                    hb = s;
                                    hn++;
                                    hu[hn] = c;
                                    hc[hn] = lccode(c);
                                    hyfbchar = nonchar;
                                } else if (type(s) == ligaturenode) {
                                    if (font_ligchar(s) != hf) {
                                        goto _Ldone3;
                                    }
                                    j = hn;
                                    q = ligptr(s);
                                    if (q > 0) hyfbchar = character(q);
                                    while (q > 0) {
                                        c = character(q) - minquarterword;
                                        if (lccode(c) == 0) goto _Ldone3;
                                        if (j == 63) goto _Ldone3;
                                        j++;
                                        hu[j] = c;
                                        hc[j] = lccode(c);
                                        q = link(q);
                                    }
                                    hb = s;
                                    hn = j;
                                    if ((subtype(s)) & 1)
                                        hyfbchar = fontbchar[hf];
                                    else
                                        hyfbchar = nonchar;
                                } else if ((type(s) == kernnode) &
                                           (subtype(s) == normal)) {
                                    hb = s;
                                    hyfbchar = fontbchar[hf];
                                } else
                                    goto _Ldone3;
                                s = link(s);
                            }
                        _Ldone3: /*:897*/
                            /*899:*/
                            if (hn < lhyf + rhyf) goto _Ldone1;
                            while (true) {
                                if (!ischarnode(s)) {
                                    switch (type(s)) {

                                        case ligaturenode:
                                            /* blank case */
                                            break;

                                        case kernnode:
                                            if (subtype(s) != normal)
                                                goto _Ldone4;
                                            break;

                                        case whatsitnode:
                                        case gluenode:
                                        case penaltynode:
                                        case insnode:
                                        case adjustnode:
                                        case marknode:
                                            goto _Ldone4;
                                            break;

                                        default:
                                            goto _Ldone1;
                                            break;
                                    }
                                }
                                s = link(s);
                            }
                        _Ldone4: /*:899*/
                            hyphenate();
                        }
                    _Ldone1:;
                    }
                    /*:894*/
                    break;

                case kernnode:
                    if (subtype(curp) == explicit) {
                        kernbreak();
                    } else
                        actwidth += width(curp);
                    break;

                case ligaturenode:
                    f = font_ligchar(curp);
                    actwidth +=
                        charwidth(f, charinfo(f, character_ligchar(curp)));
                    break;

                case discnode: /*869:*/
                    s = prebreak(curp);
                    discwidth = 0;
                    if (s == 0)
                        trybreak(exhyphenpenalty, hyphenated);
                    else {
                        do { /*870:*/
                            if (ischarnode(s)) {
                                f = font(s);
                                discwidth +=
                                    charwidth(f, charinfo(f, character(s)));
                            } else {
                                switch (type(s)) { /*:870*/

                                    case ligaturenode:
                                        f = font_ligchar(s);
                                        discwidth += charwidth(
                                            f,
                                            charinfo(f, character_ligchar(s)));
                                        break;

                                    case hlistnode:
                                    case vlistnode:
                                    case rulenode:
                                    case kernnode:
                                        discwidth += width(s);
                                        break;

                                    default:
                                        confusion(S(778));
                                        break;
                                }
                            }
                            s = link(s);
                        } while (s != 0);
                        actwidth += discwidth;
                        trybreak(hyphenpenalty, hyphenated);
                        actwidth -= discwidth;
                    }
                    r = replacecount(curp);
                    s = link(curp);
                    while (r > 0) { /*871:*/
                        if (ischarnode(s)) {
                            f = font(s);
                            actwidth += charwidth(f, charinfo(f, character(s)));
                        } else {
                            switch (type(s)) { /*:871*/

                                case ligaturenode:
                                    f = font_ligchar(s);
                                    actwidth += charwidth(
                                        f, charinfo(f, character_ligchar(s)));
                                    break;

                                case hlistnode:
                                case vlistnode:
                                case rulenode:
                                case kernnode:
                                    actwidth += width(s);
                                    break;

                                default:
                                    confusion(S(779));
                                    break;
                            }
                        }
                        r--;
                        s = link(s);
                    }
                    prevp = curp;
                    curp = s;
                    goto _Ldone5;
                    break;
                    /*:869*/

                case mathnode:
                    autobreaking = (subtype(curp) == after);
                    kernbreak();
                    break;

                case penaltynode:
                    trybreak(penalty(curp), unhyphenated);
                    break;

                case marknode:
                case insnode:
                case adjustnode:
                    /* blank case */
                    break;

                default:
                    confusion(S(780));
                    break;
            }
            prevp = curp;
            curp = link(curp);
        _Ldone5:;
        }
        /*:866*/
        if (curp == 0) { /*873:*/
            trybreak(ejectpenalty, hyphenated);
            if (link(active) != lastactive) { /*874:*/
                r = link(active);
                fewestdemerits = awfulbad;
                do {
                    if (type(r) != deltanode) {
                        if (totaldemerits(r) < fewestdemerits) {
                            fewestdemerits = totaldemerits(r);
                            bestbet = r;
                        }
                    }
                    r = link(r);
                } while (r != lastactive);
                bestline = linenumber(bestbet); /*:874*/
                if (looseness == 0) goto _Ldone;
                /*875:*/
                r = link(active);
                actuallooseness = 0;
                do {
                    if (type(r) != deltanode) {
                        linediff = linenumber(r) - bestline;
                        if ((linediff < actuallooseness &&
                             looseness <= linediff) ||
                            (linediff > actuallooseness &&
                             looseness >= linediff)) {
                            bestbet = r;
                            actuallooseness = linediff;
                            fewestdemerits = totaldemerits(r);
                        } else if ((linediff == actuallooseness) &
                                   (totaldemerits(r) < fewestdemerits)) {
                            bestbet = r;
                            fewestdemerits = totaldemerits(r);
                        }
                    }
                    r = link(r);
                } while (r != lastactive);
                bestline = linenumber(bestbet); /*:875*/
                if (actuallooseness == looseness || finalpass) goto _Ldone;
            }
        }
        /*:873*/
        /*865:*/
        q = link(active);
        while (q != lastactive) {
            curp = link(q);
            if (type(q) == deltanode)
                freenode(q, deltanodesize);
            else
                freenode(q, activenodesize);
            q = curp;
        }
        q = passive;
        while (q != 0) { /*:865*/
            curp = link(q);
            freenode(q, passivenodesize);
            q = curp;
        }
        if (secondpass) {
            #ifdef tt_STAT
                if (tracingparagraphs > 0) printnl(S(781));
            #endif // #863.3: tt_STAT
            background[1] += emergencystretch;
            finalpass = true;
            continue;
        }
        #ifdef tt_STAT
            if (tracingparagraphs > 0) printnl(S(782));
        #endif // #863.4: tt_STAT

        threshold = tolerance;
        secondpass = true;
        finalpass = (emergencystretch <= 0);
    }
_Ldone:
    #ifdef tt_STAT
        if (tracingparagraphs > 0) {
            enddiagnostic(true);
            normalize_selector();
        }
    #endif // #863.5: tt_STAT
    /*:863*/
    /*876:*/
    /*:876*/
    postlinebreak(finalwidowpenalty); /*865:*/
    q = link(active);
    while (q != lastactive) {
        curp = link(q);
        if (type(q) == deltanode)
            freenode(q, deltanodesize);
        else
            freenode(q, activenodesize);
        q = curp;
    }
    q = passive;
    while (q != 0) { /*:865*/
        curp = link(q);
        freenode(q, passivenodesize);
        q = curp;
    }
    packbeginline = 0;

    /*1363:*/
    /*898:*/
    /*:898*/
}
/*:815*/

/*934:*/
Static void newhyphexceptions(void)
{
  unsigned char n; /* INT */
  unsigned char j; /* INT */
  HyphPointer h;
  StrNumber k, s, t;
  Pointer p, q;

  scanleftbrace();
  setcurlang();   /*935:*/
  n = 0;
  p = 0;
  while (true) {   /*:935*/
    int cur_chr;
    getxtoken();
    cur_chr = curchr;
_Lreswitch:
    switch (curcmd) {

    case letter:
    case otherchar:
    case chargiven:   /*937:*/
      if (cur_chr == '-') {   /*938:*/
	if (n < 63) {
	  q = get_avail();
	  link(q) = p;
	  info(q) = n;
	  p = q;
	}
      }  /*:938*/
      else {   /*:937*/
	if (lccode(cur_chr) == 0) {
	  printnl(S(292));
	  print(S(783));
	  help2(S(784),
                S(785));
	  error();
	} else if (n < 63) {
	  n++;
	  hc[n] = lccode(cur_chr);
	}
      }
      break;

    case charnum:
      scancharnum();
      cur_chr = curval;
      curcmd = chargiven;
      goto _Lreswitch;
      break;

    case spacer:
    case rightbrace:   /*936:*/
      if (n > 1) {   /*939:*/
	n++;
	hc[n] = curlang;
	h = 0;
	for (j = 1; j <= n; j++) {
	  h = (h + h + hc[j]) % hyphsize;
	}
	s = str_ins(hc+1,n);
	if (hyphcount == hyphsize)
	  overflow(S(786), hyphsize);
	hyphcount++;
	while (hyphword[h] != 0) {  /*941:*/
	  k = hyphword[h];
	  if (str_length(k) < str_length(s)) {
	    goto _Lfound;
	  }
	  if (str_length(k) > str_length(s)) {
	    goto _Lnotfound;
	  }
	{ int ress = str_cmp(k,s);
	if(ress < 0) goto _Lfound;
	if(ress > 0) goto _Lnotfound;
	}
_Lfound:
	  q = hyphlist[h];
	  hyphlist[h] = p;
	  p = q;
	  t = hyphword[h];
	  hyphword[h] = s;
	  s = t;
_Lnotfound:   /*:941*/
	  if (h > 0)
	    h--;
	  else
	    h = hyphsize;
	}
	hyphword[h] = s;
	hyphlist[h] = p;   /*:940*/
      }
      /*:939*/
      if (curcmd == rightbrace)
	        goto _Lexit;
      n = 0;
      p = 0;
      break;

    default:
      printnl(S(292));
      print(S(597));
      print_esc(S(787));
      print(S(788));
      help2(S(789),
            S(790));
      error();   /*:936*/
      break;
    }
  }
_Lexit: ;
}
/*:934*/

/*968:*/
Static HalfWord prunepagetop(HalfWord p)
{
  Pointer prevp, q;

  prevp = temphead;
  link(temphead) = p;
  while (p != 0) {
    switch (type(p)) {

    case hlistnode:
    case vlistnode:
    case rulenode:   /*969:*/
      q = newskipparam(splittopskipcode);
      link(prevp) = q;
      link(q) = p;
      if (width(tempptr) > height(p))
	width(tempptr) -= height(p);
      else
	width(tempptr) = 0;
      p = 0;
      break;
      /*:969*/

    case whatsitnode:
    case marknode:
    case insnode:
      prevp = p;
      p = link(prevp);
      break;

    case gluenode:
    case kernnode:
    case penaltynode:
      q = p;
      p = link(q);
      link(q) = 0;
      link(prevp) = p;
      flushnodelist(q);
      break;

    default:
      confusion(S(791));
      break;
    }
  }
  return (link(temphead));
}
/*:968*/

/*970:*/
Static HalfWord vertbreak(HalfWord p, long h, long d)
{
  Pointer prevp, q, r, bestplace=p /* XXXX */ ;
  long pi=0 /* XXXX */, b, leastcost;
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
    if (p == 0)   /*974:*/
      pi = ejectpenalty;
    else {  /*973:*/
      switch (type(p)) {   /*:973*/

      case hlistnode:
      case vlistnode:
      case rulenode:
	curheight += prevdp + height(p);
	prevdp = depth(p);
	goto _Lnotfound;
	break;

      case whatsitnode:   /*1365:*/
	goto _Lnotfound;   /*:1365*/
	break;

      case gluenode:
	if (!precedesbreak(prevp))
	  goto _Lupdateheights_;
	pi = 0;
	break;

      case kernnode:
	if (link(p) == 0)
	  t = penaltynode;
	else
	  t = type(link(p));
	if (t != gluenode)
	  goto _Lupdateheights_;
	pi = 0;
	break;

      case penaltynode:
	pi = penalty(p);
	break;

      case marknode:
      case insnode:
	goto _Lnotfound;
	break;

      default:
	confusion(S(792));
	break;
      }
    }
    if (pi < infpenalty)   /*:974*/
    {  /*975:*/
      if (curheight < h) {
	if (activeheight[2] != 0 || activeheight[3] != 0 ||
	    activeheight[4] != 0)
	  b = 0;
	else
	  b = badness(h - curheight, activeheight[1]);
      } else if (curheight - h > activeheight[5])
	b = awfulbad;
      else
	b = badness(curheight - h, activeheight[5]);   /*:975*/
      if (b < awfulbad) {
	if (pi <= ejectpenalty)
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
      if (b == awfulbad || pi <= ejectpenalty)
	goto _Ldone;
    }
    if ((type(p) < gluenode) | (type(p) > kernnode))
      goto _Lnotfound;
_Lupdateheights_:   /*976:*/
    if (type(p) == kernnode)
      q = p;
    else {
      q = glueptr(p);
      activeheight[stretchorder(q) + 1] += stretch(q);
      activeheight[5] += shrink(q);
      if ((shrinkorder(q) != normal) & (shrink(q) != 0)) {
	printnl(S(292));
	print(S(793));
	help4(S(794),
              S(795),
              S(796),
              S(753));
	error();
	r = newspec(q);
	shrinkorder(r) = normal;
	deleteglueref(q);
	glueptr(p) = r;
	q = r;
      }
    }
    curheight += prevdp + width(q);
    prevdp = 0;   /*:976*/
_Lnotfound:
    if (prevdp > d) {   /*:972*/
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
Static HalfWord vsplit(EightBits n, long h) {
    HalfWord Result;
    Pointer v, p, q;

    v = box(n);
    if (splitfirstmark != 0) {
        deletetokenref(splitfirstmark);
        splitfirstmark = 0;
        deletetokenref(splitbotmark);
        splitbotmark = 0;
    }
    /*978:*/
    if (v == 0) {
        Result = 0;
        goto _Lexit;
    }
    if (type(v) != vlistnode) { /*:978*/
        printnl(S(292));
        print(S(385));
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
            if (type(p) == marknode) {
                if (splitfirstmark == 0) {
                    splitfirstmark = markptr(p);
                    splitbotmark = splitfirstmark;
                    tokenrefcount(splitfirstmark) += 2;
                } else {
                    deletetokenref(splitbotmark);
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
    freenode(v, boxnodesize);
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

/*985:*/
Static void printtotals(void)
{
  print_scaled(pagetotal);
  if (pagesofar[2] != 0) {
    print(S(331));
    print_scaled(pagesofar[2]);
    print(S(385));
  }
  if (pagesofar[3] != 0) {
    print(S(331));
    print_scaled(pagesofar[3]);
    print(S(330));
  }
  if (pagesofar[4] != 0) {
    print(S(331));
    print_scaled(pagesofar[4]);
    print(S(802));
  }
  if (pagesofar[5] != 0) {
    print(S(331));
    print_scaled(pagesofar[5]);
    print(S(803));
  }
  if (pageshrink != 0) {
    print(S(332));
    print_scaled(pageshrink);
  }
}
/*:985*/

/*987:*/
Static void freezepagespecs(SmallNumber s) {
    pagecontents = s;
    pagegoal = vsize;
    pagemaxdepth = maxdepth;
    pagedepth = 0;
    setpagesofarzero(1);
    setpagesofarzero(2);
    setpagesofarzero(3);
    setpagesofarzero(4);
    setpagesofarzero(5);
    setpagesofarzero(6);
    leastpagecost = awfulbad;
    #ifdef tt_STAT
        if (tracingpages <= 0) return;
    #endif // #987: tt_STAT
    begindiagnostic();
    printnl(S(804));
    print_scaled(pagegoal);
    print(S(805));
    print_scaled(pagemaxdepth);
    enddiagnostic(false);
} /*:987*/


/*992:*/
Static void boxerror(EightBits n)
{
  error();
  begindiagnostic();
  printnl(S(690));
  showbox(box(n));
  enddiagnostic(true);
  flushnodelist(box(n));
  box(n) = 0;
}
/*:992*/

/*993:*/
Static void ensurevbox(EightBits n)
{
  Pointer p;

  p = box(n);
  if (p == 0)
    return;
  if (type(p) != hlistnode)
    return;
  printnl(S(292));
  print(S(806));
  help3(S(807),
        S(808),
        S(809));
  boxerror(n);
}
/*:993*/

/*994:*/
/*1012:*/
Static void fireup(HalfWord c)
{  /*1013:*/
  Pointer p, q, r, s, prevp, savesplittopskip;
  UChar n;
  Boolean wait;
  long savevbadness;
  Scaled savevfuzz;

  if (type(bestpagebreak) == penaltynode) {
    geqworddefine(intbase + outputpenaltycode, penalty(bestpagebreak));
    penalty(bestpagebreak) = infpenalty;
  } else   /*:1013*/
    geqworddefine(intbase + outputpenaltycode, infpenalty);
  if (botmark != 0) {   /*1014:*/
    if (topmark != 0)
      deletetokenref(topmark);
    topmark = botmark;
    addtokenref(topmark);
    deletetokenref(firstmark);
    firstmark = 0;
  }
  if (c == bestpagebreak)   /*1015:*/
    bestpagebreak = 0;
  if (box(255) != 0) {   /*:1015*/
    printnl(S(292));
    print(S(385));
    print_esc(S(464));
    print(S(810));
    help2(S(811),
          S(809));
    boxerror(255);
  }
  insertpenalties = 0;
  savesplittopskip = splittopskip;
  if (holdinginserts <= 0) {   /*1018:*/
    r = link(pageinshead);
    while (r != pageinshead) {
      if (bestinsptr(r) != 0) {
	n = subtype(r) - minquarterword;
	ensurevbox(n);
	if (box(n) == 0)
	  box(n) = newnullbox();
	p = box(n) + listoffset;
	while (link(p) != 0)
	  p = link(p);
	lastinsptr(r) = p;
      }
      r = link(r);
    }
  }
  /*:1018*/
  q = holdhead;
  link(q) = 0;
  prevp = pagehead;
  p = link(prevp);
  while (p != bestpagebreak) {
    if (type(p) == insnode) {
      if (holdinginserts <= 0) {   /*1020:*/
	r = link(pageinshead);
	while (subtype(r) != subtype(p))
	  r = link(r);
	if (bestinsptr(r) == 0)   /*1022:*/
	  wait = true;
	else {
	  wait = false;
	  s = lastinsptr(r);
	  link(s) = insptr(p);
	  if (bestinsptr(r) == p) {   /*1021:*/
	    if (type(r) == splitup) {
	      if ((brokenins(r) == p) & (brokenptr(r) != 0)) {
		while (link(s) != brokenptr(r))
		  s = link(s);
		link(s) = 0;
		splittopskip = splittopptr(p);
		insptr(p) = prunepagetop(brokenptr(r));
		if (insptr(p) != 0) {
		  tempptr = vpack(insptr(p), 0, additional);
		  height(p) = height(tempptr) + depth(tempptr);
		  freenode(tempptr, boxnodesize);
		  wait = true;
		}
	      }
	    }
	    bestinsptr(r) = 0;
	    n = subtype(r) - minquarterword;
	    tempptr = listptr(box(n));
	    freenode(box(n), boxnodesize);
	    box(n) = vpack(tempptr, 0, additional);
	  }  /*:1021*/
	  else {
	    while (link(s) != 0)
	      s = link(s);
	    lastinsptr(r) = s;
	  }
	}
	link(prevp) = link(p);
	link(p) = 0;
	if (wait) {
	  link(q) = p;
	  q = p;
	  insertpenalties++;
	} else {
	  deleteglueref(splittopptr(p));
	  freenode(p, insnodesize);
	}
	p = prevp;   /*:1022*/
      }
      /*:1020*/
    } else if (type(p) == marknode) {
      if (firstmark == 0) {
	firstmark = markptr(p);
	addtokenref(firstmark);
      }
      if (botmark != 0)
	deletetokenref(botmark);
      botmark = markptr(p);
      addtokenref(botmark);
    }
    prevp = p;
    p = link(prevp);
  }
  splittopskip = savesplittopskip;   /*1017:*/
  if (p != 0) {
    if (link(contribhead) == 0) {
      if (nestptr == 0)
	tail = pagetail;
      else
	contribtail = pagetail;
    }
    link(pagetail) = link(contribhead);
    link(contribhead) = p;
    link(prevp) = 0;
  }
  savevbadness = vbadness;
  vbadness = INF_BAD;
  savevfuzz = vfuzz;
  vfuzz = maxdimen;
  box(255) = vpackage(link(pagehead), bestsize, exactly, pagemaxdepth);
  vbadness = savevbadness;
  vfuzz = savevfuzz;
  if (lastglue != maxhalfword)   /*991:*/
    deleteglueref(lastglue);
  pagecontents = empty;
  pagetail = pagehead;
  link(pagehead) = 0;
  lastglue = maxhalfword;
  lastpenalty = 0;
  lastkern = 0;
  pagedepth = 0;
  pagemaxdepth = 0;   /*:991*/
  if (q != holdhead) {   /*:1017*/
    link(pagehead) = link(holdhead);
    pagetail = q;
  }
  /*1019:*/
  r = link(pageinshead);
  while (r != pageinshead) {
    q = link(r);
    freenode(r, pageinsnodesize);
    r = q;
  }
  link(pageinshead) = pageinshead;   /*:1019*/
  /*:1014*/
  if (topmark != 0 && firstmark == 0) {
    firstmark = topmark;
    addtokenref(topmark);
  }
  if (outputroutine != 0) {   /*1023:*/
    if (deadcycles < maxdeadcycles) {   /*1024:*/
      outputactive = true;
      deadcycles++;
      pushnest();
      mode = -vmode;
      prevdepth = ignoredepth;
      modeline = -line;
      begintokenlist(outputroutine, outputtext);
      newsavelevel(outputgroup);
      normalparagraph();
      scanleftbrace();
      goto _Lexit;
    }
    /*:1024*/
    printnl(S(292));
    print(S(812));
    print_int(deadcycles);
    print(S(813));
    help3(S(814),
          S(815),
          S(816));
    error();
  }
  if (link(pagehead) != 0) {
    if (link(contribhead) == 0) {
      if (nestptr == 0)
	tail = pagetail;
      else
	contribtail = pagetail;
    } else
      link(pagetail) = link(contribhead);
    link(contribhead) = link(pagehead);
    link(pagehead) = 0;
    pagetail = pagehead;
  }
  shipout(box(255));
  box(255) = 0;   /*:1023*/
_Lexit: ;
}
/*:1012*/

Static void buildpage(void) {
    Pointer p, q, r;
    long b, c, pi = 0 /* XXXX */;
    UChar n;
    Scaled delta, h, w;

    if (link(contribhead) == 0 || outputactive) goto _Lexit;
    do {
    _Llabcontinue:
        p = link(contribhead); /*996:*/
        if (lastglue != maxhalfword) deleteglueref(lastglue);
        lastpenalty = 0;
        lastkern = 0;
        if (type(p) == gluenode) { /*997:*/
            lastglue = glueptr(p);
            addglueref(lastglue);
        } else { /*:996*/
            lastglue = maxhalfword;
            if (type(p) == penaltynode)
                lastpenalty = penalty(p);
            else if (type(p) == kernnode)
                lastkern = width(p);
        }
        /*1000:*/
        switch (type(p)) { /*:1000*/

            case hlistnode:
            case vlistnode:
            case rulenode:
                if (pagecontents < boxthere) { /*1001:*/
                    if (pagecontents == empty)
                        freezepagespecs(boxthere);
                    else
                        pagecontents = boxthere;
                    q = newskipparam(topskipcode);
                    if (width(tempptr) > height(p))
                        width(tempptr) -= height(p);
                    else
                        width(tempptr) = 0;
                    link(q) = p;
                    link(contribhead) = q;
                    goto _Llabcontinue;
                } else /*1002:*/
                {      /*:1002*/
                    pagetotal += pagedepth + height(p);
                    pagedepth = depth(p);
                    goto _Lcontribute_;
                }
                /*:1001*/
                break;

            case whatsitnode:       /*1364:*/
                goto _Lcontribute_; /*:1364*/
                break;

            case gluenode:
                if (pagecontents < boxthere) goto _Ldone1;
                if (!precedesbreak(pagetail)) goto _Lupdateheights_;
                pi = 0;
                break;

            case kernnode:
                if (pagecontents < boxthere) goto _Ldone1;
                if (link(p) == 0) goto _Lexit;
                if (type(link(p)) != gluenode) goto _Lupdateheights_;
                pi = 0;
                break;

            case penaltynode:
                if (pagecontents < boxthere) goto _Ldone1;
                pi = penalty(p);
                break;

            case marknode:
                goto _Lcontribute_;
                break;

            case insnode: /*1008:*/
                if (pagecontents == empty) freezepagespecs(insertsonly);
                n = subtype(p);
                r = pageinshead;
                while (n >= subtype(link(r)))
                    r = link(r);
                n -= minquarterword;
                if (subtype(r) != n) { /*1009:*/
                    q = getnode(pageinsnodesize);
                    link(q) = link(r);
                    link(r) = q;
                    r = q;
                    subtype(r) = n;
                    type(r) = inserting;
                    ensurevbox(n);
                    if (box(n) == 0)
                        height(r) = 0;
                    else
                        height(r) = height(box(n)) + depth(box(n));
                    bestinsptr(r) = 0;
                    q = skip(n);
                    if (count(n) == 1000)
                        h = height(r);
                    else
                        h = x_over_n(height(r), 1000) * count(n);
                    pagegoal += -h - width(q);
                    pagesofar[stretchorder(q) + 2] += stretch(q);
                    pageshrink += shrink(q);
                    if ((shrinkorder(q) != normal) & (shrink(q) != 0)) {
                        printnl(S(292));
                        print(S(817));
                        print_esc(S(460));
                        print_int(n);
                        help3(S(818), S(819), S(753));
                        error();
                    }
                }
                /*:1009*/
                if (type(r) == splitup)
                    insertpenalties += floatcost(p);
                else {
                    lastinsptr(r) = p;
                    delta = pagegoal - pagetotal - pagedepth + pageshrink;
                    if (count(n) == 1000)
                        h = height(p);
                    else
                        h = x_over_n(height(p), 1000) * count(n);
                    if ((h <= 0 || h <= delta) &
                        (height(p) + height(r) <= dimen(n))) {
                        pagegoal -= h;
                        height(r) += height(p);
                    } else /*1010:*/
                    {      /*:1010*/
                        if (count(n) <= 0)
                            w = maxdimen;
                        else {
                            w = pagegoal - pagetotal - pagedepth;
                            if (count(n) != 1000)
                                w = x_over_n(w, count(n)) * 1000;
                        }
                        if (w > dimen(n) - height(r)) w = dimen(n) - height(r);
                        q = vertbreak(insptr(p), w, depth(p));
                        height(r) += bestheightplusdepth;

                        #ifdef tt_STAT
                            if (tracingpages > 0) {
                                /*1011:*/
                                begindiagnostic();
                                printnl(S(820));
                                print_int(n);
                                print(S(821));
                                print_scaled(w);
                                print_char(',');
                                print_scaled(bestheightplusdepth);
                                print(S(764));
                                if (q == 0)
                                    print_int(ejectpenalty);
                                else if (type(q) == penaltynode)
                                    print_int(penalty(q));
                                else
                                    print_char('0');
                                enddiagnostic(false);
                            } /*:1011*/
                        #endif // #1010: tt_STAT

                        if (count(n) != 1000)
                            bestheightplusdepth =
                                x_over_n(bestheightplusdepth, 1000) * count(n);
                        pagegoal -= bestheightplusdepth;
                        type(r) = splitup;
                        brokenptr(r) = q;
                        brokenins(r) = p;
                        if (q == 0)
                            insertpenalties += ejectpenalty;
                        else if (type(q) == penaltynode)
                            insertpenalties += penalty(q);
                    }
                }
                goto _Lcontribute_;
                break;
                /*:1008*/

            default:
                confusion(S(822));
                break;
        }
        /*1005:*/
        if (pi < infpenalty) /*:1005*/
        {                    /*1007:*/
            if (pagetotal < pagegoal) {
                if (pagesofar[3] != 0 || pagesofar[4] != 0 || pagesofar[5] != 0)
                    b = 0;
                else
                    b = badness(pagegoal - pagetotal, pagesofar[2]);
            } else if (pagetotal - pagegoal > pageshrink)
                b = awfulbad;
            else
                b = badness(pagetotal - pagegoal, pageshrink); /*:1007*/
            if (b < awfulbad) {
                if (pi <= ejectpenalty)
                    c = pi;
                else if (b < INF_BAD)
                    c = b + pi + insertpenalties;
                else
                    c = deplorable;
            } else
                c = b;
            if (insertpenalties >= 10000)
                c = awfulbad;

            #ifdef tt_STAT
                if (tracingpages > 0) { /*1006:*/
                    begindiagnostic();
                    printnl('%');
                    print(S(758));
                    printtotals();
                    print(S(823));
                    print_scaled(pagegoal);
                    print(S(763));
                    if (b == awfulbad)
                        print_char('*');
                    else
                        print_int(b);
                    print(S(764));
                    print_int(pi);
                    print(S(824));
                    if (c == awfulbad)
                        print_char('*');
                    else
                        print_int(c);
                    if (c <= leastpagecost) print_char('#');
                    enddiagnostic(false);
                }
            #endif // #1005: tt_STAT
            /*:1006*/

            if (c <= leastpagecost) {
                bestpagebreak = p;
                bestsize = pagegoal;
                leastpagecost = c;
                r = link(pageinshead);
                while (r != pageinshead) {
                    bestinsptr(r) = lastinsptr(r);
                    r = link(r);
                }
            }
            if (c == awfulbad || pi <= ejectpenalty) {
                fireup(p);
                if (outputactive) goto _Lexit;
                goto _Ldone;
            }
        }
        if ((type(p) < gluenode) | (type(p) > kernnode)) goto _Lcontribute_;
    _Lupdateheights_: /*1004:*/
        if (type(p) == kernnode)
            q = p;
        else {
            q = glueptr(p);
            pagesofar[stretchorder(q) + 2] += stretch(q);
            pageshrink += shrink(q);
            if ((shrinkorder(q) != normal) & (shrink(q) != 0)) {
                printnl(S(292));
                print(S(825));
                help4(S(826), S(795), S(796), S(753));
                error();
                r = newspec(q);
                shrinkorder(r) = normal;
                deleteglueref(q);
                glueptr(p) = r;
                q = r;
            }
        }
        pagetotal += pagedepth + width(q);
        pagedepth = 0;                  /*:1004*/
    _Lcontribute_:                      /*1003:*/
        if (pagedepth > pagemaxdepth) { /*:1003*/
            pagetotal += pagedepth - pagemaxdepth;
            pagedepth = pagemaxdepth;
        }
        /*998:*/
        link(pagetail) = p;
        pagetail = p;
        link(contribhead) = link(p);
        link(p) = 0;
        goto _Ldone; /*:998*/
    _Ldone1:         /*999:*/
        link(contribhead) = link(p);
        link(p) = 0; /*:999*/
        flushnodelist(p);
    _Ldone:;                          /*:997*/
    } while (link(contribhead) != 0); /*995:*/
    if (nestptr == 0)
        tail = contribhead;
    else
        contribtail = contribhead; /*:995*/
_Lexit:;
}
/*:994*/

/*1030:*/
/*1043:*/
Static void appspace(void)
{
  Pointer q;

  if (spacefactor >= 2000 && xspaceskip != zeroglue)
    q = newparamglue(xspaceskipcode);
  else {
    if (spaceskip != zeroglue)
      mainp = spaceskip;
    else {   /*1042:*/
      mainp = fontglue[curfont ];
      if (mainp == 0) {
	FontIndex mmaink;
	mainp = newspec(zeroglue);
	mmaink = parambase[curfont ] + spacecode;
#if 1
	maink = mmaink;
#endif
	width(mainp) = fontinfo[maink].sc;
	stretch(mainp) = fontinfo[mmaink + 1].sc;
	shrink(mainp) = fontinfo[mmaink + 2].sc;
	fontglue[curfont ] = mainp;
      }
    }
    mainp = newspec(mainp);   /*1044:*/
    if (spacefactor >= 2000)
      width(mainp) += extraspace(curfont);
    stretch(mainp) = xn_over_d(stretch(mainp), spacefactor, 1000);
    shrink(mainp) = xn_over_d(shrink(mainp), 1000, spacefactor);
	/*:1044*/
    q = newglue(mainp);
    gluerefcount(mainp) = 0;
  }
  link(tail) = q;
  tail = q;

  /*:1042*/
}
/*:1043*/

/*1047:*/
Static void insertdollarsign(void)
{
  backinput();
  curtok = mathshifttoken + '$';
  printnl(S(292));
  print(S(827));
  help2(S(828),
        S(829));
  inserror();
}
/*:1047*/

/*1049:*/
Static void youcant(void)
{
  printnl(S(292));
  print(S(602));
  printcmdchr(curcmd, curchr);
  print(S(830));
  printmode(mode);
}
/*:1049*/

/*1050:*/
Static void reportillegalcase(void)
{
  youcant();
  help4(S(831),
        S(832),
        S(833),
        S(834));
  error();
}
/*:1050*/

/*1051:*/
Static Boolean privileged(void)
{
  if (mode > 0)
    return true;
  else {
    reportillegalcase();
    return false;
  }
}
/*:1051*/

/*1054:*/
Static Boolean itsallover(void) {
    if (privileged()) {
        if (pagehead == pagetail && head == tail && deadcycles == 0) {
            return true;
        }
        backinput();
        tailappend(newnullbox());
        width(tail) = hsize;
        tailappend(newglue(fillglue));
        tailappend(newpenalty(-1073741824L));
        buildpage();
    }

    return false;
}
/*:1054*/

/*1060:*/
Static void appendglue(void)
{
  SmallNumber s;

  s = curchr;
  switch (s) {

  case filcode:
    curval = filglue;
    break;

  case fillcode:
    curval = fillglue;
    break;

  case sscode:
    curval = ssglue;
    break;

  case filnegcode:
    curval = filnegglue;
    break;

  case skipcode:
    scanglue(glueval);
    break;

  case mskipcode:
    scanglue(muval);
    break;
  }
  tailappend(newglue(curval));
  if (s < skipcode)
    return;
  (gluerefcount(curval))--;
  if (s > skipcode)
    subtype(tail) = muglue;
}  /*:1060*/


/*1061:*/
Static void appendkern(void)
{
  QuarterWord s;

  s = curchr;
  scandimen(s == muglue, false, false);
  tailappend(newkern(curval));
  subtype(tail) = s;
}
/*:1061*/

/*1064:*/
Static void offsave(void)
{
  Pointer p;

  if (curgroup == bottomlevel) {   /*1066:*/
    printnl(S(292));
    print(S(558));
    printcmdchr(curcmd, curchr);
    help1(S(835));
    error();
    return;
  }  /*:1066*/
  backinput();
  p = get_avail();
  link(temphead) = p;
  printnl(S(292));
  print(S(554));   /*1065:*/
  switch (curgroup) {   /*:1065*/

  case semisimplegroup:
    info(p) = cstokenflag + frozenendgroup;
    print_esc(S(836));
    break;

  case mathshiftgroup:
    info(p) = mathshifttoken + '$';
    print_char('$');
    break;

  case mathleftgroup:
    info(p) = cstokenflag + frozenright;
    link(p) = get_avail();
    p = link(p);
    info(p) = othertoken + '.';
    print_esc(S(837));
    break;

  default:
    info(p) = rightbracetoken + '}';
    print_char('}');
    break;
  }
  print(S(555));
  inslist(link(temphead));
  help5(S(838),
        S(839),
        S(840),
        S(841),
        S(842));
  error();
}
/*:1064*/

/*1069:*/
Static void extrarightbrace(void)
{
  printnl(S(292));
  print(S(843));
  switch (curgroup) {

  case semisimplegroup:
    print_esc(S(836));
    break;

  case mathshiftgroup:
    print_char('$');
    break;

  case mathleftgroup:
    print_esc(S(419));
    break;
  }
  help5(S(844),
        S(845),
        S(846),
        S(847),
        S(848));
  error();
  alignstate++;
}
/*:1069*/

/*1070:*/
Static void normalparagraph(void)
{
  if (looseness != 0)
    eqworddefine(intbase + loosenesscode, 0);
  if (hangindent != 0)
    eqworddefine(dimenbase + hangindentcode, 0);
  if (hangafter != 1)
    eqworddefine(intbase + hangaftercode, 1);
  if (parshapeptr != 0)
    eqdefine(parshapeloc, shaperef, 0);
}  /*:1070*/


/*1075:*/
Static void boxend(long boxcontext)
{
  Pointer p;

  if (boxcontext < boxflag) {   /*1076:*/
    if (curbox == 0)
      return;
    shiftamount(curbox) = boxcontext;
    if (labs(mode) == vmode) {
      appendtovlist(curbox);
      if (adjusttail != 0) {
	if (adjusthead != adjusttail) {
	  link(tail) = link(adjusthead);
	  tail = adjusttail;
	}
	adjusttail = 0;
      }
      if (mode > 0)
	buildpage();
      return;
    }
    if (labs(mode) == hmode)
      spacefactor = 1000;
    else {
      p = newnoad();
      mathtype(nucleus(p)) = subbox;
      info(nucleus(p)) = curbox;
      curbox = p;
    }
    link(tail) = curbox;
    tail = curbox;
    return;
  }  /*:1076*/
  if (boxcontext < shipoutflag) {   /*1077:*/
    if (boxcontext < boxflag + 256)
      eqdefine(boxbase - boxflag + boxcontext, boxref, curbox);
    else   /*:1077*/
      geqdefine(boxbase - boxflag + boxcontext - 256, boxref, curbox);
    return;
  }
  if (curbox == 0)
    return;
  if (boxcontext <= shipoutflag)   /*1078:*/
  {
    shipout(curbox);
    return;
  }
  /*:1078*/
  skip_spaces_or_relax();
  if ( (curcmd == hskip && labs(mode) != vmode) ||
      (curcmd == vskip && labs(mode) == vmode) ||
      (curcmd == mskip && labs(mode) == mmode) ) {
    appendglue();
    subtype(tail) = boxcontext - leaderflag + aleaders;
    leaderptr(tail) = curbox;
    return;
  }
  printnl(S(292));
  print(S(849));
  help3(S(850),
        S(851),
        S(852));
  backerror();
  flushnodelist(curbox);
}
/*:1075*/

/*1079:*/
Static void beginbox(long boxcontext) {
    Pointer p, q;
    QuarterWord m;
    HalfWord k;
    EightBits n;

    switch (curchr) {

    case boxcode:
        scaneightbitint();
        curbox = box(curval);
        box(curval) = 0;
        break;

    case copycode:
        scaneightbitint();
        curbox = copynodelist(box(curval));
        break;

    case lastboxcode: /*1080:*/
        curbox = 0;
        if (labs(mode) == mmode) {
            youcant();
            help1(S(853));
            error();
        } else if (mode == vmode && head == tail) {
            youcant();
            help2(S(854), S(855));
            error();
        } else {
            if (!ischarnode(tail)) {
                if ((type(tail) == hlistnode) |
                    (type(tail) == vlistnode)) { /*1081:*/
                    q = head;
                    do {
                        p = q;
                        if (!ischarnode(q)) {
                            if (type(q) == discnode) {
                                QuarterWord FORLIM = replacecount(q);
                                for (m = 1; m <= FORLIM; m++)
                                    p = link(p);
                                if (p == tail) goto _Ldone;
                            }
                        }
                        q = link(p);
                    } while (q != tail);
                    curbox = tail;
                    shiftamount(curbox) = 0;
                    tail = p;
                    link(p) = 0;
_Ldone:;
                }
                /*:1081*/
            }
        }
        break;
        /*:1080*/

    case vsplitcode: /*1082:*/
        scaneightbitint();
        n = curval;
        if (!scankeyword(S(697))) {
            printnl(S(292));
            print(S(856));
            help2(S(857), S(858));
            error();
        }
        scannormaldimen();
        curbox = vsplit(n, curval);
        break;
        /*:1082*/
        /*1083:*/

    default:
        k = curchr - vtopcode;
        saved(0) = boxcontext;
        if (k == hmode) {
            if (boxcontext < boxflag && labs(mode) == vmode)
                scanspec(adjustedhboxgroup, true);
            else
                scanspec(hboxgroup, true);
        } else {
            if (k == vmode)
                scanspec(vboxgroup, true);
            else {
                scanspec(vtopgroup, true);
                k = vmode;
            }
            normalparagraph();
        }
        pushnest();
        mode = -k;
        if (k == vmode) {
            prevdepth = ignoredepth;
            if (everyvbox != 0) begintokenlist(everyvbox, everyvboxtext);
        } else {
            spacefactor = 1000;
            if (everyhbox != 0) begintokenlist(everyhbox, everyhboxtext);
        }
        goto _Lexit; 

        /*:1083*/
        break;
    }
    boxend(boxcontext);
_Lexit:;
}
/*:1079*/

/*1084:*/
Static void scanbox(long boxcontext)
{
  skip_spaces_or_relax();
  if (curcmd == makebox) {
    beginbox(boxcontext);
    return;
  }
  if (boxcontext >= leaderflag && (curcmd == hrule || curcmd == vrule)) {
    curbox = scanrulespec();
    boxend(boxcontext);
    return;
  }
  printnl(S(292));
  print(S(859));
  help3(S(860),
        S(861),
        S(862));
  backerror();
}
/*:1084*/

/*1086:*/
Static void package(SmallNumber c)
{
  Scaled h, d;
  Pointer p;

  d = boxmaxdepth;
  unsave();
  saveptr -= 3;
  if (mode == -hmode)
    curbox = hpack(link(head), saved(2), saved(1));
  else {
    curbox = vpackage(link(head), saved(2), saved(1), d);
    if (c == vtopcode) {   /*1087:*/
      h = 0;
      p = listptr(curbox);
      if (p != 0) {
	if (type(p) <= rulenode)
	  h = height(p);
      }
      depth(curbox) += height(curbox) - h;
      height(curbox) = h;
    }
    /*:1087*/
  }
  popnest();
  boxend(saved(0));
}
/*:1086*/

/*1091:*/
Static SmallNumber normmin(long h)
{
  if (h <= 0)
    return 1;
  else if (h >= 63)
    return 63;
  else
    return h;
}


Static void newgraf(Boolean indented)
{
  prevgraf = 0;
  if (mode == vmode || head != tail) {
    tailappend(newparamglue(parskipcode));
  }
  pushnest();
  mode = hmode;
  spacefactor = 1000;
  setcurlang();
  clang = curlang;
  prevgraf = (normmin(lefthyphenmin) * 64 + normmin(righthyphenmin)) * 65536L +
	     curlang;
  if (indented) {
    tail = newnullbox();
    link(head) = tail;
    width(tail) = parindent;
  }
  if (everypar != 0)
    begintokenlist(everypar, everypartext);
  if (nestptr == 1)
    buildpage();
}
/*:1091*/

/*1093:*/
Static void indentinhmode(void)
{
  Pointer p, q;

  if (curchr <= 0)
    return;
  p = newnullbox();
  width(p) = parindent;
  if (labs(mode) == hmode)
    spacefactor = 1000;
  else {
    q = newnoad();
    mathtype(nucleus(q)) = subbox;
    info(nucleus(q)) = p;
    p = q;
  }
  tailappend(p);
}
/*:1093*/

/*1095:*/
Static void headforvmode(void)
{
  if (mode < 0) {
    if (curcmd != hrule) {
      offsave();
      return;
    }
    printnl(S(292));
    print(S(602));
    print_esc(S(863));
    print(S(864));
    help2(S(865),
          S(866));
    error();
    return;
  }
  backinput();
  curtok = partoken;
  backinput();
  tokentype = inserted;
}
/*:1095*/

/*1096:*/
Static void endgraf(void)
{
  if (mode != hmode)
    return;
  if (head == tail)
    popnest();
  else
    linebreak(widowpenalty);
  normalparagraph();
  errorcount = 0;
}
/*:1096*/

/*1099:*/
Static void begininsertoradjust(void)
{
  if (curcmd == vadjust)
    curval = 255;
  else {
    scaneightbitint();
    if (curval == 255) {
      printnl(S(292));
      print(S(867));
      print_esc(S(374));
      print_int(255);
      help1(S(868));
      error();
      curval = 0;
    }
  }
  saved(0) = curval;
  saveptr++;
  newsavelevel(insertgroup);
  scanleftbrace();
  normalparagraph();
  pushnest();
  mode = -vmode;
  prevdepth = ignoredepth;
}
/*:1099*/


/*1101:*/
Static void makemark(void)
{
  Pointer p;

  p = scantoks(false, true);
  p = getnode(smallnodesize);
  type(p) = marknode;
  subtype(p) = 0;
  markptr(p) = defref;
  link(tail) = p;
  tail = p;
}
/*:1101*/

/*1103:*/
Static void appendpenalty(void)
{
  scanint();
  tailappend(newpenalty(curval));
  if (mode == vmode)
    buildpage();
}
/*:1103*/

/*1105:*/
Static void deletelast(void) {
    Pointer p, q;

    if (mode == vmode && tail == head) { /*1106:*/
    if (curchr != gluenode || lastglue != maxhalfword) {
        youcant();
        help2(S(854), S(869));
        if (curchr == kernnode)
            helpline[0] = S(870);
        else if (curchr != gluenode)
            helpline[0] = S(871);
        error();
    }
    } /*:1106*/
    else {
        if (!ischarnode(tail)) {
        if (type(tail) == curchr) {
            q = head;
            do {
                p = q;
                if (!ischarnode(q)) {
                if (type(q) == discnode) {
                    QuarterWord FORLIM = replacecount(q);
                    QuarterWord m;
                    for (m = 1; m <= FORLIM; m++)
                        p = link(p);
                    if (p == tail) return;
                }
                }
                q = link(p);
            } while (q != tail);
            link(p) = 0;
            flushnodelist(tail);
            tail = p;
        }
        }
    }
}
/*:1105*/

/*1110:*/
Static void unpackage(void) {
    Pointer p;
    char c;

    c = curchr;
    scaneightbitint();
    p = box(curval);
    if (p == 0) return;
    if ( (labs(mode) == mmode) |
        ((labs(mode) == vmode) & (type(p) != vlistnode)) |
        ((labs(mode) == hmode) & (type(p) != hlistnode))) {
        printnl(S(292));
        print(S(872));
        help3(S(873), S(874), S(875));
        error();
        return;
    }

    if (c == copycode)
        link(tail) = copynodelist(listptr(p));
    else {
        link(tail) = listptr(p);
        box(curval) = 0;
        freenode(p, boxnodesize);
    }
    while (link(tail) != 0)
        tail = link(tail);
}
/*:1110*/

/*1113:*/
Static void appenditaliccorrection(void) {
    Pointer p;
    InternalFontNumber f;
    int c;

    if (tail != head) {
        if (ischarnode(tail)) {
            p = tail;
            f = font(p);
            c = character(p);
        } else if (type(tail) == ligaturenode) {
            p = ligchar(tail);
            f = font_ligchar(tail);
            c = character_ligchar(tail);
        } else {
            return;
        }
        /*   
            f = font(p);
            tailappend(newkern(charitalic(f, charinfo(f, character(p)))));
        */
        tailappend(newkern(charitalic(f, charinfo(f, c))));
        subtype(tail) = explicit;
    }
}
/*:1113*/

/*1117:*/
Static void appenddiscretionary(void)
{
  long c;

  tailappend(newdisc());
  if (curchr == 1) {
    c = get_hyphenchar(curfont);
    if (c >= 0) {
      if (c < 256)
	prebreak(tail) = newcharacter(curfont, c);
    }
    return;
  }
  saveptr++;
  saved(-1) = 0;
  newsavelevel(discgroup);
  scanleftbrace();
  pushnest();
  mode = -hmode;
  spacefactor = 1000;
}
/*:1117*/

/*1119:*/
Static void builddiscretionary(void) {
    Pointer p, q;
    long n;

    unsave(); /*1121:*/
    q = head;
    p = link(q);
    n = 0;

    while (p != 0) {
        if (!ischarnode(p)) {
            if (type(p) > rulenode) {
                if (type(p) != kernnode) {
                    if (type(p) != ligaturenode) {
                        printnl(S(292));
                        print(S(876));
                        help1(S(877));
                        error();
                        begindiagnostic();
                        printnl(S(878));
                        showbox(p);
                        enddiagnostic(true);
                        flushnodelist(p);
                        link(q) = 0;
                        break;
                    }
                }
            }
        }
        q = p;
        p = link(q);
        n++;
    }
/*:1121*/

    p = link(head);
    popnest();
    switch (saved(-1)) {

        case 0:
            prebreak(tail) = p;
            break;

        case 1:
            postbreak(tail) = p;
            break;

        case 2: /*1120:*/
            if (n > 0 && labs(mode) == mmode) {
                printnl(S(292));
                print(S(879));
                print_esc(S(400));
                help2(S(880), S(881));
                flushnodelist(p);
                n = 0;
                error();
            } else
                link(tail) = p;
            if (n <= maxquarterword)
                replacecount(tail) = n;
            else {
                printnl(S(292));
                print(S(882));
                help2(S(883), S(884));
                error();
            }
            if (n > 0) tail = q;
            saveptr--;
            goto _Lexit;
            break;
            /*:1120*/
    }
    (saved(-1))++;
    newsavelevel(discgroup);
    scanleftbrace();
    pushnest();
    mode = -hmode;
    spacefactor = 1000;
_Lexit:;
}
/*:1119*/

/*1123:*/
Static void makeaccent(void)
{
  double s, t;
  Pointer p, q, r;
  InternalFontNumber f;
  Scaled a, h, x, w, delta;
  FourQuarters i;

  scancharnum();
  f = curfont;
  p = newcharacter(f, curval);
  if (p == 0)
    return;
  x = xheight(f);
  s = slant(f) / 65536.0;
  a = charwidth(f, charinfo(f, character(p)));
  doassignments();   /*1124:*/
  q = 0;
  f = curfont;
  if (curcmd == letter || curcmd == otherchar || curcmd == chargiven)
    q = newcharacter(f, curchr);
  else if (curcmd == charnum) {
    scancharnum();
    q = newcharacter(f, curval);
  } else
    backinput();
  if (q != 0) {   /*1125:*/
    t = slant(f) / 65536.0;
    i = charinfo(f, character(q));
    w = charwidth(f, i);
    h = charheight(f, heightdepth(i));
    if (h != x) {
      p = hpack(p, 0, additional);
      shiftamount(p) = x - h;
    }
    delta = (long)floor((w - a) / 2.0 + h * t - x * s + 0.5);
    r = newkern(delta);
    subtype(r) = acckern;
    link(tail) = r;
    link(r) = p;
    tail = newkern(-a - delta);
    subtype(tail) = acckern;
    link(p) = tail;
    p = q;
  }
  /*:1125*/
  link(tail) = p;
  tail = p;
  spacefactor = 1000;

  /*:1124*/
}
/*:1123*/

/*1127:*/
Static void alignerror(void)
{
  if (labs(alignstate) > 2) {   /*1128:*/
    printnl(S(292));
    print(S(885));
    printcmdchr(curcmd, curchr);
    if (curtok == tabtoken + '&') {
      help6(S(886),
            S(887),
            S(888),
            S(889),
            S(890),
            S(891));
    } else {
      help5(S(886),
            S(892),
            S(889),
            S(890),
            S(891));
    }
    error();
    return;
  }
  /*:1128*/
  backinput();
  if (alignstate < 0) {
    printnl(S(292));
    print(S(566));
    alignstate++;
    curtok = leftbracetoken + '{';
  } else {
    printnl(S(292));
    print(S(893));
    alignstate--;
    curtok = rightbracetoken + '}';
  }
  help3(S(894),
        S(895),
        S(896));
  inserror();
}
/*:1127*/

/*1129:*/
Static void noalignerror(void)
{
  printnl(S(292));
  print(S(885));
  print_esc(S(897));
  help2(S(898),
        S(899));
  error();
}


Static void omiterror(void)
{
  printnl(S(292));
  print(S(885));
  print_esc(S(900));
  help2(S(901),
        S(899));
  error();
}
/*:1129*/

/*1131:*/
Static void doendv(void)
{
  if (curgroup != aligngroup) {
    offsave();
    return;
  }
  endgraf();
  if (fincol())
    finrow();
}
/*:1131*/

/*1135:*/
Static void cserror(void)
{
  printnl(S(292));
  print(S(558));
  print_esc(S(263));
  help1(S(902));
  error();
}  /*:1135*/


/*1136:*/
Static void pushmath(GroupCode c)
{
  pushnest();
  mode = -mmode;
  incompleatnoad = 0;
  newsavelevel(c);
}
/*:1136*/

/*1138:*/
Static void initmath(void)
{
  Scaled w, l, s, v, d;
  Pointer p, q;
  InternalFontNumber f;
  long n;

  gettoken();
  if (curcmd == mathshift && mode > 0) {   /*1145:*/
    if (head == tail) {
      popnest();
      w = -maxdimen;
    } else {
      linebreak(displaywidowpenalty);   /*1146:*/
      v = shiftamount(justbox) + quad(curfont) * 2;
      w = -maxdimen;
      p = listptr(justbox);
      while (p != 0) {  /*1147:*/
_Lreswitch:
	if (ischarnode(p)) {
	  f = font(p);
	  d = charwidth(f, charinfo(f, character(p)));
	  goto _Lfound;
	}
	switch (type(p)) {   /*:1147*/

	case hlistnode:
	case vlistnode:
	case rulenode:
	  d = width(p);
	  goto _Lfound;
	  break;

	case ligaturenode:   /*652:*/
      type(ligtrick) = charnodetype ;
      font(ligtrick) = font_ligchar(p);
      character(ligtrick) = character_ligchar(p);
	  link(ligtrick) = link(p);
	  p = ligtrick;
	  goto _Lreswitch;
	  break;
	  /*:652*/

	case kernnode:
	case mathnode:
	  d = width(p);
	  break;

	case gluenode:   /*1148:*/
	  q = glueptr(p);
	  d = width(q);
	  if (gluesign(justbox) == stretching) {
	    if ((glueorder(justbox) == stretchorder(q)) &
		(stretch(q) != 0))
	      v = maxdimen;
	  } else if (gluesign(justbox) == shrinking) {
	    if ((glueorder(justbox) == shrinkorder(q)) &
		(shrink(q) != 0))
	      v = maxdimen;
	  }
	  if (subtype(p) >= aleaders)
	    goto _Lfound;
	  break;
	  /*:1148*/

	case whatsitnode:   /*1361:*/
	  d = 0;   /*:1361*/
	  break;

	default:
	  d = 0;
	  break;
	}
	if (v < maxdimen)
	  v += d;
	goto _Lnotfound;
_Lfound:
	if (v >= maxdimen) {
	  w = maxdimen;
	  goto _Ldone;
	}
	v += d;
	w = v;
_Lnotfound:
	p = link(p);
      }
_Ldone: ;   /*:1146*/
    }  /*1149:*/
    if (parshapeptr == 0) {
      if (hangindent != 0 &&
	  ( (hangafter >= 0 && prevgraf + 2 > hangafter) ||
	   prevgraf + 1 < -hangafter) ) {
	l = hsize - labs(hangindent);
	if (hangindent > 0)
	  s = hangindent;
	else
	  s = 0;
      } else {
	l = hsize;
	s = 0;
      }
    } else {   /*:1149*/
      n = info(parshapeptr);
      if (prevgraf + 2 >= n)
	p = parshapeptr + n * 2;
      else
	p = parshapeptr + (prevgraf + 2) * 2;
      s = mem[p - memmin - 1].sc;
      l = mem[p - memmin].sc;
    }
    pushmath(mathshiftgroup);
    mode = mmode;
    eqworddefine(intbase + curfamcode, -1);
    eqworddefine(dimenbase + predisplaysizecode, w);
    eqworddefine(dimenbase + displaywidthcode, l);
    eqworddefine(dimenbase + displayindentcode, s);
    if (everydisplay != 0)
      begintokenlist(everydisplay, everydisplaytext);
    if (nestptr == 1)
      buildpage();
    return;
  }
  /*:1145*/
  backinput();   /*1139:*/
  pushmath(mathshiftgroup);
  eqworddefine(intbase + curfamcode, -1);
  if (everymath != 0)   /*:1139*/
    begintokenlist(everymath, everymathtext);
}
/*:1138*/

/*1142:*/
Static void starteqno(void) {
    saved(0) = curchr;
    saveptr++; /*1139:*/
    pushmath(mathshiftgroup);
    eqworddefine(intbase + curfamcode, -1);
    if (everymath != 0) /*:1139*/
        begintokenlist(everymath, everymathtext);
}
/*:1142*/

/*1151:*/
Static void scanmath(HalfWord p) {
    long c;

_Lrestart:
    skip_spaces_or_relax();
_Lreswitch:
    switch (curcmd) {

        case letter:
        case otherchar:
        case chargiven:
            c = mathcode(curchr);
            if (c == 32768L) { /*1152:*/
                curcs = curchr + activebase;
                curcmd = eqtype(curcs);
                curchr = equiv(curcs);
                xtoken();
                backinput(); /*:1152*/
                goto _Lrestart;
            }
            break;

        case charnum:
            scancharnum();
            curchr = curval;
            curcmd = chargiven;
            goto _Lreswitch;
            break;

        case mathcharnum:
            scanfifteenbitint();
            c = curval;
            break;

        case mathgiven:
            c = curchr;
            break;

        case delimnum: /*1153:*/
            scantwentysevenbitint();
            c = curval / 4096;
            break;

        default:
            backinput();
            scanleftbrace();
            saved(0) = p;
            saveptr++;
            pushmath(mathgroup);
            goto _Lexit;
            /*:1153*/
            break;
    }
    mathtype(p) = mathchar;
    character(p) = c & 255;
    if (c >= varcode && faminrange) {
        fam(p) = curfam;
    } else {
        fam(p) = (c / 256) & 15;
    }
_Lexit:;
}
/*:1151*/

/*1155:*/
Static void setmathchar(long c)
{
  Pointer p;

  if (c >= 32768L) {   /*1152:*/
    curcs = curchr + activebase;
    curcmd = eqtype(curcs);
    curchr = equiv(curcs);
    xtoken();
    backinput();
    return;
  }  /*:1152*/
  p = newnoad();
  mathtype(nucleus(p)) = mathchar;
  character(nucleus(p)) = c & 255;
  fam(nucleus(p)) = (c / 256) & 15;
  if (c >= varcode) {
    if (faminrange) {
      fam(nucleus(p)) = curfam;
    }
    type(p) = ordnoad;
  } else
    type(p) = ordnoad + c / 4096;
  link(tail) = p;
  tail = p;
}
/*:1155*/

/*1159:*/
Static void mathlimitswitch(void) {
    if (head != tail) {
    if (type(tail) == opnoad) {
        subtype(tail) = curchr;
        return;
    }
    }
    printnl(S(292));
    print(S(903));
    help1(S(904));
    error();
}
/*:1159*/

/*1160:*/
Static void scandelimiter(HalfWord p, Boolean r)
{
  if (r)
    scantwentysevenbitint();
  else {
    skip_spaces_or_relax();
    switch (curcmd) {

    case letter:
    case otherchar:
      curval = delcode(curchr);
      break;

    case delimnum:
      scantwentysevenbitint();
      break;

    default:
      curval = -1;
      break;
    }
  }
  if (curval < 0) {   /*1161:*/
    printnl(S(292));
    print(S(905));
    help6(S(906),
          S(907),
          S(908),
          S(909),
          S(910),
          S(911));
    backerror();
    curval = 0;
  }
  /*:1161*/
  smallfam(p) = (curval / 1048576L) & 15;
  smallchar(p) = (curval / 4096) & 255;
  largefam(p) = (curval / 256) & 15;
  largechar(p) = curval & 255;
}
/*:1160*/

/*1163:*/
Static void mathradical(void)
{
  int i=0;
  tailappend(getnode(radicalnoadsize));
  type(tail) = radicalnoad;
  subtype(tail) = normal;
#ifdef BIG_CHARNODE
  for(i=0;i<charnodesize;i++) {
#endif
  mem[nucleus(tail) + i - memmin].hh = emptyfield;
  mem[subscr(tail) + i - memmin].hh = emptyfield;
  mem[supscr(tail) + i - memmin].hh = emptyfield;
#ifdef BIG_CHARNODE
  }
#endif
  scandelimiter(leftdelimiter(tail), true);
  scanmath(nucleus(tail));
}
/*:1163*/

/*1165:*/
Static void mathac(void)
{
  if (curcmd == accent) {   /*1166:*/
    printnl(S(292));
    print(S(912));
    print_esc(S(913));
    print(S(914));
    help2(S(915),
          S(916));
    error();
  }
  /*:1166*/
  tailappend(getnode(accentnoadsize));
  type(tail) = accentnoad;
  subtype(tail) = normal;
  mem[nucleus(tail) - memmin].hh = emptyfield;
  mem[subscr(tail) - memmin].hh = emptyfield;
  mem[supscr(tail) - memmin].hh = emptyfield;
  mathtype(accentchr(tail)) = mathchar;
  scanfifteenbitint();
  character(accentchr(tail)) = curval & 255;
  if (curval >= varcode && faminrange) {
    fam(accentchr(tail)) = curfam;
  } else
    fam(accentchr(tail)) = (curval / 256) & 15;
  scanmath(nucleus(tail));
}
/*:1165*/

/*1172:*/
Static void appendchoices(void)
{
  tailappend(newchoice());
  saveptr++;
  saved(-1) = 0;
  pushmath(mathchoicegroup);
  scanleftbrace();
}
/*:1172*/

/*1174:*/
/*1184:*/
Static HalfWord finmlist(HalfWord p)
{
  Pointer q;

  if (incompleatnoad != 0) {   /*1185:*/
    mathtype(denominator(incompleatnoad)) = submlist;
    info(denominator(incompleatnoad)) = link(head);
    if (p == 0)
      q = incompleatnoad;
    else {
      q = info(numerator(incompleatnoad));
      if (type(q) != leftnoad)
	confusion(S(419));
      info(numerator(incompleatnoad)) = link(q);
      link(q) = incompleatnoad;
      link(incompleatnoad) = p;
    }
  }  /*:1185*/
  else {
    link(tail) = p;
    q = link(head);
  }
  popnest();
  return q;
}
/*:1184*/

Static void buildchoices(void) {
    Pointer p;

    unsave();
    p = finmlist(0);
    switch (saved(-1)) {
        case 0:
            displaymlist(tail) = p;
            break;

        case 1:
            textmlist(tail) = p;
            break;

        case 2:
            scriptmlist(tail) = p;
            break;

        case 3:
            scriptscriptmlist(tail) = p;
            saveptr--;
            return;
            break;
    }
    (saved(-1))++;
    pushmath(mathchoicegroup);
    scanleftbrace();
}
/*:1174*/

/*1176:*/
Static void subsup(void)
{
  SmallNumber t;
  Pointer p;

  t = empty;
  p = 0;
  if (tail != head) {
    if (scriptsallowed(tail)) {
	if(curcmd == supmark) {
		p = supscr(tail);
	} else {
		p = subscr(tail);
	}
      t = mathtype(p);
    }
  }
  if (p == 0 || t != empty) {   /*1177:*/
    tailappend(newnoad());
    if(curcmd == supmark) {
	p = supscr(tail);
    } else {
	p = subscr(tail);
    }
    if (t != empty) {
      if (curcmd == supmark) {
	printnl(S(292));
	print(S(917));
	help1(S(918));
      } else {
	printnl(S(292));
	print(S(919));
	help1(S(920));
      }
      error();
    }
  }
  /*:1177*/
  scanmath(p);
}
/*:1176*/

/*1181:*/
Static void mathfraction(void)
{
  SmallNumber c;

  c = curchr;
  if (incompleatnoad != 0) {   /*1183:*/
    if (c >= delimitedcode) {
      scandelimiter(garbage, false);
      scandelimiter(garbage, false);
    }
    if (c % delimitedcode == abovecode) {
      scannormaldimen();
    }
    printnl(S(292));
    print(S(921));
    help3(S(922),
          S(923),
          S(924));
    error();
    return;
  }  /*:1183*/
  incompleatnoad = getnode(fractionnoadsize);
  type(incompleatnoad) = fractionnoad;
  subtype(incompleatnoad) = normal;
  mathtype(numerator(incompleatnoad)) = submlist;
  info(numerator(incompleatnoad)) = link(head);
  mem[denominator(incompleatnoad) - memmin].hh = emptyfield;
  mem[leftdelimiter(incompleatnoad) - memmin].qqqq = nulldelimiter;
  mem[rightdelimiter(incompleatnoad) - memmin].qqqq = nulldelimiter;
  link(head) = 0;
  tail = head;   /*1182:*/
  if (c >= delimitedcode) {
    scandelimiter(leftdelimiter(incompleatnoad), false);
    scandelimiter(rightdelimiter(incompleatnoad), false);
  }
  switch (c % delimitedcode) {   /*:1182*/

  case abovecode:
    scannormaldimen();
    thickness(incompleatnoad) = curval;
    break;

  case overcode:
    thickness(incompleatnoad) = defaultcode;
    break;

  case atopcode:
    thickness(incompleatnoad) = 0;
    break;
  }
}
/*:1181*/

/*1191:*/
Static void mathleftright(void)
{
  SmallNumber t;
  Pointer p;

  t = curchr;
  if (t == rightnoad && curgroup != mathleftgroup) {   /*1192:*/
    if (curgroup != mathshiftgroup) {
      offsave();
      return;
    }
    scandelimiter(garbage, false);
    printnl(S(292));
    print(S(558));
    print_esc(S(419));
    help1(S(925));
    error();
    return;
  }
  /*:1192*/
  p = newnoad();
  type(p) = t;
  scandelimiter(delimiter(p), false);
  if (t == leftnoad) {
    pushmath(mathleftgroup);
    link(head) = p;
    tail = p;
    return;
  }
  p = finmlist(p);
  unsave();
  tailappend(newnoad());
  type(tail) = innernoad;
  mathtype(nucleus(tail)) = submlist;
  info(nucleus(tail)) = p;
}
/*:1191*/

/*1194:*/
Static void aftermath(void)
{
  Boolean l, danger;
  long m;
  Pointer p, a;
  /*1198:*/
  Pointer b, r, t;   /*:1198*/
  Scaled w, z, e, q, d, s;
  SmallNumber g1, g2;

  danger = false;   /*1195:*/
  if ((fontparams[famfnt(TEXT_SIZE + 2) ] < totalmathsyparams) |
      (fontparams[famfnt(SCRIPT_SIZE + 2) ] < totalmathsyparams) |
      (fontparams[famfnt(SCRIPT_SCRIPT_SIZE + 2) ] <
       totalmathsyparams)) {
    printnl(S(292));
    print(S(926));
    help3(S(927),
          S(928),
          S(929));
    error();
    flushmath();
    danger = true;
  } else if ((fontparams[famfnt(TEXT_SIZE + 3) ] <
	      totalmathexparams) |
	     (fontparams[famfnt(SCRIPT_SIZE + 3) ] <
	      totalmathexparams) |
	     (fontparams[famfnt(SCRIPT_SCRIPT_SIZE + 3) ] <
	      totalmathexparams)) {
    printnl(S(292));
    print(S(930));
    help3(S(931),
          S(932),
          S(933));
    error();
    flushmath();
    danger = true;
  }
  m = mode;
  l = false;
  p = finmlist(0);
  if (mode == -m) {   /*1197:*/
    getxtoken();
    if (curcmd != mathshift) {   /*:1197*/
      printnl(S(292));
      print(S(745));
      help2(S(746),
            S(747));
      backerror();
    }
    curmlist = p;
    curstyle = textstyle;
    mlistpenalties = false;
    mlisttohlist();
    a = hpack(link(temphead), 0, additional);
    unsave();
    saveptr--;
    if (saved(0) == 1)
      l = true;
    danger = false;   /*1195:*/
    if ((fontparams[famfnt(TEXT_SIZE + 2) ] < totalmathsyparams) |
	(fontparams[famfnt(SCRIPT_SIZE + 2) ] < totalmathsyparams) |
	(fontparams[famfnt(SCRIPT_SCRIPT_SIZE + 2) ] <
	 totalmathsyparams)) {
      printnl(S(292));
      print(S(926));
      help3(S(927),
            S(928),
            S(929));
      error();
      flushmath();
      danger = true;
    } else if ((fontparams[famfnt(TEXT_SIZE + 3) ] <
		totalmathexparams) |
	       (fontparams[famfnt(SCRIPT_SIZE + 3) ] <
		totalmathexparams) |
	       (fontparams[famfnt(SCRIPT_SCRIPT_SIZE + 3) ] <
		totalmathexparams)) {
      printnl(S(292));
      print(S(930));
      help3(S(931),
            S(932),
            S(933));
      error();
      flushmath();
      danger = true;
    }
    m = mode;
    p = finmlist(0);
  } else
    a = 0;
  if (m < 0) {   /*1196:*/
    tailappend(newmath(mathsurround, before));
    curmlist = p;
    curstyle = textstyle;
    mlistpenalties = (mode > 0);
    mlisttohlist();
    link(tail) = link(temphead);
    while (link(tail) != 0)
      tail = link(tail);
    tailappend(newmath(mathsurround, after));
    spacefactor = 1000;
    unsave();
    return;
  }
  /*:1196*/
  if (a == 0) {   /*1197:*/
    getxtoken();
    if (curcmd != mathshift) {
      printnl(S(292));
      print(S(745));
      help2(S(746),
            S(747));
      backerror();
    }
  }
  /*:1197*/
  /*1199:*/
  curmlist = p;
  curstyle = displaystyle;
  mlistpenalties = false;
  mlisttohlist();
  p = link(temphead);
  adjusttail = adjusthead;
  b = hpack(p, 0, additional);
  p = listptr(b);
  t = adjusttail;
  adjusttail = 0;
  w = width(b);
  z = displaywidth;
  s = displayindent;
  if (a == 0 || danger) {
    e = 0;
    q = 0;
  } else {
    e = width(a);
    q = e + mathquad(TEXT_SIZE);
  }
  if (w + q > z) {   /*1201:*/
    if (e != 0 && (w - totalshrink[0] + q <= z ||
		   totalshrink[fil - normal] != 0 ||
		   totalshrink[fill - normal] != 0 ||
		   totalshrink[filll - normal] != 0)) {
      freenode(b, boxnodesize);
      b = hpack(p, z - q, exactly);
    } else {
      e = 0;
      if (w > z) {
	freenode(b, boxnodesize);
	b = hpack(p, z, exactly);
      }
    }
    w = width(b);
  }
  /*:1201*/
  /*1202:*/
  d = half(z - w);
  if (e > 0 && d < e * 2) {   /*:1202*/
    d = half(z - w - e);
    if (p != 0) {
      if (!ischarnode(p)) {
	if (type(p) == gluenode)
	  d = 0;
      }
    }
  }
  /*1203:*/
  tailappend(newpenalty(predisplaypenalty));
  if (d + s <= predisplaysize || l) {
    g1 = abovedisplayskipcode;
    g2 = belowdisplayskipcode;
  } else {
    g1 = abovedisplayshortskipcode;
    g2 = belowdisplayshortskipcode;
  }
  if (l && e == 0) {   /*1204:*/
    shiftamount(a) = s;
    appendtovlist(a);
    tailappend(newpenalty(infpenalty));
  } else {
    tailappend(newparamglue(g1));   /*:1203*/
  }
  if (e != 0) {
    r = newkern(z - w - e - d);
    if (l) {
      link(a) = r;
      link(r) = b;
      b = a;
      d = 0;
    } else {
      link(b) = r;
      link(r) = a;
    }
    b = hpack(b, 0, additional);
  }
  shiftamount(b) = s + d;   /*:1204*/
  appendtovlist(b);   /*1205:*/
  if (a != 0 && e == 0 && !l) {
    tailappend(newpenalty(infpenalty));
    shiftamount(a) = s + z - width(a);
    appendtovlist(a);
    g2 = 0;
  }
  if (t != adjusthead) {
    link(tail) = link(adjusthead);
    tail = t;
  }
  tailappend(newpenalty(postdisplaypenalty));
  if (g2 > 0) {   /*:1199*/
    tailappend(newparamglue(g2));   /*:1205*/
  }
  resumeafterdisplay();

  /*:1195*/
  /*:1195*/
}
/*:1194*/

/*1200:*/
Static void resumeafterdisplay(void)
{
  if (curgroup != mathshiftgroup)
    confusion(S(934));
  unsave();
  prevgraf += 3;
  pushnest();
  mode = hmode;
  spacefactor = 1000;
  setcurlang();
  clang = curlang;
  prevgraf = (normmin(lefthyphenmin) * 64 + normmin(righthyphenmin)) * 65536L +
	     curlang;
      /*443:*/
  getxtoken();
  if (curcmd != spacer)   /*:443*/
    backinput();
  if (nestptr == 1)
    buildpage();
}
/*:1200*/

/*1211:*/
/*1215:*/
Static void getrtoken(void)
{
_Lrestart:
  do {
    gettoken();
  } while (curtok == spacetoken);
  if (curcs != 0 && curcs <= frozencontrolsequence)
    return;
  printnl(S(292));
  print(S(935));
  help5(S(936),
        S(937),
        S(938),
        S(939),
        S(940));
  if (curcs == 0)
    backinput();
  curtok = cstokenflag + frozenprotection;
  inserror();
  goto _Lrestart;
}
/*:1215*/

/*1229:*/
Static void trapzeroglue(void) {
    if (!((width(curval) == 0) & (stretch(curval) == 0) &
          (shrink(curval) == 0)))
        return;
    addglueref(zeroglue);
    deleteglueref(curval);
    curval = zeroglue;
}
/*:1229*/

/*1236:*/
Static void doregistercommand(SmallNumber a) {
    Pointer l = 0 /* XXXX */, q, r, s;
    char p;

    q = curcmd; /*1237:*/
    if (q != register_) {
        getxtoken();
        if (curcmd >= assignint && curcmd <= assignmuglue) {
            l = curchr;
            p = curcmd - assignint;
            goto _Lfound;
        }
        if (curcmd != register_) {
            printnl(S(292));
            print(S(602));
            printcmdchr(curcmd, curchr);
            print(S(603));
            printcmdchr(q, 0);
            help1(S(941));
            error();
            goto _Lexit;
        }
    }
    p = curchr;
    scaneightbitint();
    switch (p) {

        case intval:
            l = curval + countbase;
            break;

        case dimenval:
            l = curval + scaledbase;
            break;

        case glueval:
            l = curval + skipbase;
            break;

        case muval:
            l = curval + muskipbase;
            break;
    }
_Lfound: /*:1237*/
    if (q == register_)
        scanoptionalequals();
    else
        scankeyword(S(942));
    arith_error = false;
    if (q < multiply) { /*1238:*/
        if (p < glueval) {
            if (p == intval)
                scanint();
            else {
                scannormaldimen();
            }
            if (q == advance) curval += eqtb[l - activebase].int_;
        } else { /*:1238*/
            scanglue(p);
            if (q == advance) { /*1239:*/
                q = newspec(curval);
                r = equiv(l);
                deleteglueref(curval);
                width(q) += width(r);
                if (stretch(q) == 0) stretchorder(q) = normal;
                if (stretchorder(q) == stretchorder(r))
                    stretch(q) += stretch(r);
                else if ((stretchorder(q) < stretchorder(r)) &
                         (stretch(r) != 0)) {
                    stretch(q) = stretch(r);
                    stretchorder(q) = stretchorder(r);
                }
                if (shrink(q) == 0) shrinkorder(q) = normal;
                if (shrinkorder(q) == shrinkorder(r))
                    shrink(q) += shrink(r);
                else if ((shrinkorder(q) < shrinkorder(r)) & (shrink(r) != 0)) {
                    shrink(q) = shrink(r);
                    shrinkorder(q) = shrinkorder(r);
                }
                curval = q;
            }
            /*:1239*/
        }
    } else { /*1240:*/
        scanint();
        if (p < glueval) {
            if (q == multiply) {
                if (p == intval)
                    curval = mult_and_add(
                        eqtb[l - activebase].int_, curval, 0, 2147483647L);
                else
                    curval = mult_and_add(
                        eqtb[l - activebase].int_, curval, 0, 1073741823L);
            } else
                curval = x_over_n(eqtb[l - activebase].int_, curval);
        } else {
            s = equiv(l);
            r = newspec(s);
            if (q == multiply) {
                width(r) = mult_and_add(width(s), curval, 0, 1073741823L);
                stretch(r) = mult_and_add(stretch(s), curval, 0, 1073741823L);
                shrink(r) = mult_and_add(shrink(s), curval, 0, 1073741823L);
            } else {
                width(r) = x_over_n(width(s), curval);
                stretch(r) = x_over_n(stretch(s), curval);
                shrink(r) = x_over_n(shrink(s), curval);
            }
            curval = r;
        }
    } /*:1240*/
    if (arith_error) {
        printnl(S(292));
        print(S(943));
        help2(S(944), S(945));
        error();
        goto _Lexit;
    }
    if (p < glueval) {
        worddefine(l, curval);
    } else {
        trapzeroglue();
        define(l, glueref, curval);
    }
_Lexit:;
}
/*:1236*/

/*1243:*/
Static void alteraux(void) {
    HalfWord c;

    if (curchr != labs(mode)) {
        reportillegalcase();
        return;
    }
    c = curchr;
    scanoptionalequals();
    if (c == vmode) {
        scannormaldimen();
        prevdepth = curval;
        return;
    }
    scanint();
    if (curval > 0 && curval <= 32767) {
        spacefactor = curval;
        return;
    }
    printnl(S(292));
    print(S(946));
    help1(S(947));
    int_error(curval);
}
/*:1243*/

/*1244:*/
Static void alterprevgraf(void) {
    int p;

    nest[nestptr] = curlist;
    p = nestptr;
    while (abs(nest[p].modefield) != vmode)
        p--;
    scanoptionalequals();
    scanint();
    if (curval >= 0) {
        nest[p].pgfield = curval;
        curlist = nest[nestptr];
        return;
    }
    printnl(S(292));
    print(S(773));
    print_esc(S(948));
    help1(S(949));
    int_error(curval);
}
/*:1244*/

/*1245:*/
Static void alterpagesofar(void) {
    int c;

    c = curchr;
    scanoptionalequals();
    scannormaldimen();
    pagesofar[c] = curval;
} /*:1245*/


/*1246:*/
Static void alterinteger(void) {
    char c;

    c = curchr;
    scanoptionalequals();
    scanint();
    if (c == 0)
        deadcycles = curval;
    else
        insertpenalties = curval;
} /*:1246*/


/*1247:*/
Static void alterboxdimen(void) {
    SmallNumber c;
    EightBits b;

    c = curchr;
    scaneightbitint();
    b = curval;
    scanoptionalequals();
    scannormaldimen();
    if (box(b) != 0) mem[box(b) + c - memmin].sc = curval;
}
/*:1247*/

/*1257:*/
Static void newfont(SmallNumber a) {
    Pointer u;
    Scaled s;
    InternalFontNumber f;
    StrNumber t;
    enum Selector old_setting;
    /* XXXX  StrNumber flushablestring; */

    if (jobname == 0) openlogfile();
    getrtoken();
    u = curcs;
    if (u >= hashbase) {
        t = text(u);
    } else if (u >= singlebase) {
        if (u == nullcs)
            t = S(950);
        else
            t = u - singlebase;
    } else {
        old_setting = selector;
        selector = NEW_STRING;
        print(S(950));
        print(u - activebase);
        selector = old_setting;
        str_room(1);
        t = makestring();
    }
    define(u, setfont, nullfont);
    scanoptionalequals();
    scanfilename(); /*1258:*/
    nameinprogress = true;
    if (scankeyword(S(951))) { /*1259:*/
        scannormaldimen();
        s = curval;
        if (s <= 0 || s >= 134217728L) {
            printnl(S(292));
            print(S(952));
            print_scaled(s);
            print(S(953));
            help2(S(954), S(955));
            error();
            s = UNITY * 10;
        }
        /*:1259*/
    } else if (scankeyword(S(956))) {
        scanint();
        s = -curval;
        if (curval <= 0 || curval > 32768L) {
            printnl(S(292));
            print(S(486));
            help1(S(487));
            int_error(curval);
            s = -1000;
        }
    } else {
        s = -1000;
    }
    nameinprogress = false;
    /*:1258*/

    /*1260:*/
    #if 0
        flushablestring = str_ptr - 1;
    #endif
    for (f = 1; f <= fontptr; f++) {
        if (str_eq_str(get_fontname(f), curname) /* &
	        str_eq_str(fontarea[f ], curarea) */ ) {   
            /*:1260*/
            #if 0
                if (curname == flushablestring) {
                    flush_string();
                    curname = fontname[f ];
                }
            #endif
            if (s > 0) {
                if (s == get_fontsize(f)) goto _Lcommonending;
            } else if (get_fontsize(f) == xn_over_d(get_fontdsize(f), -s, 1000)) {
                goto _Lcommonending;
            }
        } // if(str_eq_str(...))
    } // for (f = 1; f <= fontptr; f++)
    f = readfontinfo(u, curname, curarea, s);

_Lcommonending:
    equiv(u) = f;
    eqtb[fontidbase + f - activebase] = eqtb[u - activebase];
    set_fontidtext(f, t);
}
/*:1257*/

/*1265:*/
Static void newinteraction(void) {
    println();
    interaction = curchr; /*75:*/
    if (interaction == batchmode)
        selector = NO_PRINT;
    else {
        selector = TERM_ONLY;
        /*:75*/
    }
    if (logopened) selector += 2;
} /*:1265*/


Static void prefixedcommand(void)
{
  SmallNumber a;
  InternalFontNumber f;
  HalfWord j;
  FontIndex k;
  Pointer p, q;
  long n;
  Boolean e;

  a = 0;
  while (curcmd == prefix) {
    if (!((a / curchr) & 1))
      a += curchr;
    skip_spaces_or_relax();
    if (curcmd > maxnonprefixedcommand)   /*1212:*/
      continue;
    /*:1212*/
    printnl(S(292));
    print(S(957));
    printcmdchr(curcmd, curchr);
    print_char('\'');
    help1(S(958));
    backerror();
    goto _Lexit;
  }  /*1213:*/
  if (curcmd != def && (a & 3) != 0) {   /*:1213*/
    printnl(S(292));
    print(S(602));
    print_esc(S(959));
    print(S(960));
    print_esc(S(961));
    print(S(962));
    printcmdchr(curcmd, curchr);
    print_char('\'');
    help1(S(963));
    error();
  }
  /*1214:*/
  if (globaldefs != 0) {
    if (globaldefs < 0) {
      if (global) {
	a -= 4;
      }
    } else {   /*:1214*/
      if (~global) {
	a += 4;
      }
    }
  }
  switch (curcmd) {   /*1217:*/

  case setfont:   /*:1217*/
    define(curfontloc, data, curchr);
    break;

  /*1218:*/
  case def:   /*:1218*/
    if ((curchr & 1) && ~global && globaldefs >= 0) {
      a += 4;
    }
    e = (curchr >= 2);
    getrtoken();
    p = curcs;
    q = scantoks(true, e);
    define(p, call + (a & 3), defref);
    break;
    /*1221:*/

  case let:
    n = curchr;
    getrtoken();
    p = curcs;
    if (n == normal) {
      do {
	gettoken();
      } while (curcmd == spacer);
      if (curtok == othertoken + '=') {
	gettoken();
	if (curcmd == spacer)
	  gettoken();
      }
    } else {
      gettoken();
      q = curtok;
      gettoken();
      backinput();
      curtok = q;
      backinput();
    }
    if (curcmd >= call) {
      addtokenref(curchr);
    }
    define(p, curcmd, curchr);
    break;

  /*:1221*/
  /*1224:*/
  case shorthanddef:
    n = curchr;
    getrtoken();
    p = curcs;
    define(p, relax, 256);
    scanoptionalequals();
    switch (n) {

    case chardefcode:
      scancharnum();
      define(p, chargiven, curval);
      break;

    case mathchardefcode:
      scanfifteenbitint();
      define(p, mathgiven, curval);
      break;

    default:
      scaneightbitint();
      switch (n) {

      case countdefcode:
	define(p, assignint, countbase + curval);
	break;

      case dimendefcode:
	define(p, assigndimen, scaledbase + curval);
	break;

      case skipdefcode:
	define(p, assignglue, skipbase + curval);
	break;

      case muskipdefcode:
	define(p, assignmuglue, muskipbase + curval);
	break;

      case toksdefcode:
	define(p, assigntoks, toksbase + curval);
	break;
      }
      break;
    }
    break;
    /*:1224*/

  /*1225:*/
  case readtocs:   /*:1225*/
    scanint();
    n = curval;
    if (!scankeyword(S(697))) {
      printnl(S(292));
      print(S(856));
      help2(S(964),
            S(965));
      error();
    }
    getrtoken();
    p = curcs;
    readtoks(n, p);
    define(p, call, curval);
    break;
    /*1226:*/

  case toksregister:
  case assigntoks:   /*:1226*/
    q = curcs;
    if (curcmd == toksregister) {
      scaneightbitint();
      p = toksbase + curval;
    } else
      p = curchr;
    scanoptionalequals();
    skip_spaces_or_relax();
    if (curcmd != leftbrace) {   /*1227:*/
      int cur_chr = curchr;
      if (curcmd == toksregister) {
	scaneightbitint();
	curcmd = assigntoks;
	cur_chr = toksbase + curval;
      }
      if (curcmd == assigntoks) {
	q = equiv(cur_chr);
	if (q == 0) {
	  define(p, undefinedcs, 0);
	} else {
	  addtokenref(q);
	  define(p, call, q);
	}
	goto _Ldone;
      }
    }
    /*:1227*/
    backinput();
    curcs = q;
    q = scantoks(false, false);
    if (link(defref) == 0) {
      define(p, undefinedcs, 0);
      FREE_AVAIL(defref);
    } else {
      if (p == outputroutineloc) {
	link(q) = get_avail();
	q = link(q);
	info(q) = rightbracetoken + '}';
	q = get_avail();
	info(q) = leftbracetoken + '{';
	link(q) = link(defref);
	link(defref) = q;
      }
      define(p, call, defref);
    }
    break;
    /*1228:*/

  case assignint:
    p = curchr;
    scanoptionalequals();
    scanint();
    worddefine(p, curval);
    break;

  case assigndimen:
    p = curchr;
    scanoptionalequals();
    scannormaldimen();
    worddefine(p, curval);
    break;

  case assignglue:
  case assignmuglue:   /*:1228*/
    p = curchr;
    n = curcmd;
    scanoptionalequals();
    if (n == assignmuglue)
      scanglue(muval);
    else
      scanglue(glueval);
    trapzeroglue();
    define(p, glueref, curval);
    break;
    /*1232:*/

  case defcode:   /*:1232*/
    /*1233:*/
    if (curchr == catcodebase)
      n = maxcharcode;
    else if (curchr == mathcodebase)
      n = 32768L;
    else if (curchr == sfcodebase)
      n = 32767;
    else if (curchr == delcodebase)
      n = 16777215L;
    else
      n = 255;   /*:1233*/
    p = curchr;
    scancharnum();
    p += curval;
    scanoptionalequals();
    scanint();
    if ( (curval < 0 && p < delcodebase) || curval > n) {
      printnl(S(292));
      print(S(966));
      print_int(curval);
      if (p < delcodebase)
	print(S(967));
      else
	print(S(968));
      print_int(n);
      help1(S(969));
      error();
      curval = 0;
    }
    if (p < mathcodebase) {
      define(p, data, curval);
    } else if (p < delcodebase) {
      define(p, data, curval);
    } else {
      worddefine(p, curval);
    }
    break;
    /*1234:*/

  case deffamily:   /*:1234*/
    p = curchr;
    scanfourbitint();
    p += curval;
    scanoptionalequals();
    scanfontident();
    define(p, data, curval);
    break;
    /*1235:*/

  case register_:
  case advance:
  case multiply:
  case divide:   /*:1235*/
    doregistercommand(a);
    break;
    /*1241:*/

  case setbox:   /*:1241*/
    scaneightbitint();
    if (global) {
      n = curval + 256;
    } else
      n = curval;
    scanoptionalequals();
    if (setboxallowed)
      scanbox(boxflag + n);
    else {
      printnl(S(292));
      print(S(597));
      print_esc(S(970));
      help2(S(971),
            S(972));
      error();
    }
    break;
    /*1242:*/

  case setaux:
    alteraux();
    break;

  case setprevgraf:
    alterprevgraf();
    break;

  case setpagedimen:
    alterpagesofar();
    break;

  case setpageint:
    alterinteger();
    break;

  case setboxdimen:   /*:1242*/
    alterboxdimen();
    break;
    /*1248:*/

  case setshape:   /*:1248*/
    scanoptionalequals();
    scanint();
    n = curval;
    if (n <= 0)
      p = 0;
    else {
      p = getnode(n * 2 + 1);
      info(p) = n;
      for (j = 1; j <= n; j++) {
	scannormaldimen();
	mem[p + j * 2 - memmin - 1].sc = curval;
	scannormaldimen();
	mem[p + j * 2 - memmin].sc = curval;
      }
    }
    define(parshapeloc, shaperef, p);
    break;

  case hyphdata:
    if (curchr == 1) {
#ifdef tt_INIT
        /// #1252
        newpatterns();
        goto _Ldone;
#endif // #1252: tt_INIT
    } else {
      newhyphexceptions();
      goto _Ldone;
    }
    break;
    /*1253:*/

  case assignfontdimen:
    findfontdimen(true);
    k = curval;
    scanoptionalequals();
    scannormaldimen();
    fontinfo[k].sc = curval;
    break;

  case assignfontint:
    n = curchr;
    scanfontident();
    f = curval;
    scanoptionalequals();
    scanint();
    if (n == 0)
      set_hyphenchar(f, curval);
    else
      set_skewchar(f, curval);
    break;
    /*:1253*/

  /*1256:*/
  case deffont:
    newfont(a);
    break;

  /*:1256*/
  /*1264:*/
  case setinteraction:
    newinteraction();
    break;

  /*:1264*/
  default:
    confusion(S(973));
    break;
  }
_Ldone:
  /*1269:*/
  if (aftertoken != 0) {   /*:1269*/
    curtok = aftertoken;
    backinput();
    aftertoken = 0;
  }
_Lexit: ;
}
/*:1211*/

/*1270:*/
Static void doassignments(void) {
    while (true) {
        skip_spaces_or_relax();
        if (curcmd <= maxnonprefixedcommand) break;
        setboxallowed = false;
        prefixedcommand();
        setboxallowed = true;
    }
}
/*:1270*/

/*1275:*/
Static void openorclosein(void) {
    int c;
    int n;

    c = curchr;
    scanfourbitint();
    n = curval;
    if (readopen[n] != closed) {
        aclose(&readfile[n]);
        readopen[n] = closed;
    }
    if (c == 0) return;
    scanoptionalequals();
    scanfilename();
    if (curext == S(385)) curext = S(669);
    packfilename(curname, S(677), curext);
    if (a_open_in(&readfile[n])) readopen[n] = justopen;
}
/*:1275*/

/*1279:*/
Static void issuemessage(void) {
    enum Selector old_setting;
    char c;
    StrNumber s;

    c = curchr;
    link(garbage) = scantoks(false, true);
    old_setting = selector;
    selector = NEW_STRING;
    tokenshow(defref);
    selector = old_setting;
    flushlist(defref);
    str_room(1);
    s = makestring();
    if (c == 0) { /*1280:*/
        if (term_offset + str_length(s) > MAX_PRINT_LINE - 2) {
            println();
        } else if (term_offset > 0 || file_offset > 0)
            print_char(' ');
        slow_print(s);
        fflush(stdout);
        /*1283:*/
    } else {      /*:1283*/
        printnl(S(292));
        print(S(385));
        slow_print(s);
        if (errhelp != 0)
            useerrhelp = true;
        else if (longhelpseen) {
            help1(S(974));
        } else {
            if (interaction < errorstopmode) longhelpseen = true;
            help4(S(975), S(976), S(977), S(978));
        }
        error();
        useerrhelp = false;
    }
    /*:1280*/
    flush_string();
}
/*:1279*/

/*1288:*/
Static void shiftcase(void) {
    Pointer b, p;
    EightBits c;

    b = curchr;
    p = scantoks(false, false);
    p = link(defref);
    while (p != 0) { /*1289:*/
        HalfWord t = info(p);
        if (t < cstokenflag + singlebase) { /*:1289*/
            c = t & (dwa_do_8 - 1);
            if (equiv(b + c) != 0) info(p) = t - c + equiv(b + c);
        }
        p = link(p);
    }
    backlist(link(defref));
    FREE_AVAIL(defref);
}
/*:1288*/

/*1293:*/
Static void showwhatever(void) {
    switch (curchr) {
        case showlists:
            begindiagnostic();
            showactivities();
            break;

        case showboxcode: /*1296:*/
            scaneightbitint();
            begindiagnostic();
            printnl(S(979));
            print_int(curval);
            print_char('=');
            if (box(curval) == 0)
                print(S(465));
            else
                showbox(box(curval));
            break;
            /*:1296*/

        case showcode: /*1294:*/
            gettoken();
            printnl(S(980));
            if (curcs != 0) {
                sprint_cs(curcs);
                print_char('=');
            }
            printmeaning(curchr, curcmd);
            goto _Lcommonending;
            break;
            /*:1294*/
            /*1297:*/

        default:
            (void)thetoks();
            printnl(S(980));
            tokenshow(temphead);
            flushlist(link(temphead));
            goto _Lcommonending; /*:1297*/
            break;
    } /*1298:*/
    enddiagnostic(true);
    printnl(S(292));
    print(S(981));
    if (selector == TERM_AND_LOG) {
        if (tracingonline <= 0) { /*:1298*/
            selector = TERM_ONLY;
            print(S(982));
            selector = TERM_AND_LOG;
        }
    }
_Lcommonending:
    if (interaction < errorstopmode) {
        helpptr = 0;
        errorcount--;
    } else if (tracingonline > 0) {
        help3(S(983), S(984), S(985));
    } else {
        help5(S(983), S(984), S(985), S(986), S(987));
    }
    error();
}
/*:1293*/


#ifdef tt_INIT
/// 455#1302: Declare action procedures for use by main control
Static void storefmtfile(void) { /*1304:*/
    long j, k, l, x;
    Pointer p, q;
    MemoryWord pppfmtfile;
    if (saveptr != 0) {
        printnl(S(292));
        print(S(988));
        help1(S(989));
        succumb();
    }
    /*:1304*/
    /*1328:*/
    selector = NEW_STRING;
    print(S(990));
    print(jobname);
    print_char(' ');
    print_int(year % 100);
    print_char('.');
    print_int(month);
    print_char('.');
    print_int(day);
    print_char(')');
    if (interaction == batchmode)
        selector = LOG_ONLY;
    else
        selector = TERM_AND_LOG;
    str_room(1);
    formatident = makestring();
    packjobname(formatextension);
    while (!wopenout(&fmtfile))
        promptfilename(S(991), formatextension);
    printnl(S(992));
    slow_print(wmakenamestring());
    flush_string();
    printnl(S(385));        /*:1328*/
    slow_print(formatident); /*1307:*/
    pppfmtfile.int_ = 371982687L;
    pput(pppfmtfile);
    pppfmtfile.int_ = membot;
    pput(pppfmtfile);
    pppfmtfile.int_ = memtop;
    pput(pppfmtfile);
    pppfmtfile.int_ = eqtbsize;
    pput(pppfmtfile);
    pppfmtfile.int_ = hashprime;
    pput(pppfmtfile);
    pppfmtfile.int_ = hyphsize;
    pput(pppfmtfile); /*:1307*/
    str_dump(fmtfile);

    sort_avail(); // #131

    varused = 0;
    pppfmtfile.int_ = lomemmax;
    pput(pppfmtfile);
    pppfmtfile.int_ = rover;
    pput(pppfmtfile);
    p = membot;
    q = rover;
    x = 0;
    do {
        for (k = p; k <= q + 1; k++) {
            pppfmtfile = mem[k - memmin];
            pput(pppfmtfile);
        }
        x += q - p + 2;
        varused += q - p;
        p = q + nodesize(q);
        q = rlink(q);
    } while (q != rover);
    varused += lomemmax - p;
    dynused = memend - himemmin + 1;
    for (k = p; k <= lomemmax; k++) {
        pppfmtfile = mem[k - memmin];
        pput(pppfmtfile);
    }
    x += lomemmax - p + 1;
    pppfmtfile.int_ = himemmin;
    pput(pppfmtfile);
    pppfmtfile.int_ = avail;
    pput(pppfmtfile);
    for (k = himemmin; k <= memend; k++) {
        pppfmtfile = mem[k - memmin];
        pput(pppfmtfile);
    }
    x += memend - himemmin + 1;
    p = avail;
    while (p != 0) {
        dynused -= charnodesize;
        p = link(p);
    }
    pppfmtfile.int_ = varused;
    pput(pppfmtfile);
    pppfmtfile.int_ = dynused;
    pput(pppfmtfile);
    println();
    print_int(x);
    print(S(993));
    print_int(varused);
    print_char('&');    /*:1311*/
    print_int(dynused); /*1313:*/
    /*1315:*/
    k = activebase;

    do { /*1316:*/
        j = k;
        while (j < intbase - 1) {
            if ((equiv(j) == equiv(j + 1)) & (eqtype(j) == eqtype(j + 1)) &
                (eqlevel(j) == eqlevel(j + 1)))
                goto _Lfound1;
            j++;
        }
        l = intbase;
        goto _Ldone1;
_Lfound1:
        j++;
        l = j;
        while (j < intbase - 1) {
            if ((equiv(j) != equiv(j + 1)) | (eqtype(j) != eqtype(j + 1)) |
                (eqlevel(j) != eqlevel(j + 1)))
                goto _Ldone1;
            j++;
        }
_Ldone1:
        pppfmtfile.int_ = l - k;
        pput(pppfmtfile);
        while (k < l) {
            pppfmtfile = eqtb[k - activebase];
            pput(pppfmtfile);
            k++;
        }
        k = j + 1;
        pppfmtfile.int_ = k - l;
        pput(pppfmtfile); /*:1315*/
    } while (k != intbase);
    do {
        j = k;
        while (j < eqtbsize) {
            if (eqtb[j - activebase].int_ == eqtb[j - activebase + 1].int_)
                goto _Lfound2;
            j++;
        }
        l = eqtbsize + 1;
        goto _Ldone2;
_Lfound2:
        j++;
        l = j;
        while (j < eqtbsize) {
            if (eqtb[j - activebase].int_ != eqtb[j - activebase + 1].int_)
                goto _Ldone2;
            j++;
        }
_Ldone2:
        pppfmtfile.int_ = l - k;
        pput(pppfmtfile);
        while (k < l) {
            pppfmtfile = eqtb[k - activebase];
            pput(pppfmtfile);
            k++;
        }
        k = j + 1;
        pppfmtfile.int_ = k - l;
        pput(pppfmtfile); /*:1316*/
    } while (k <= eqtbsize);

    pppfmtfile.int_ = parloc;
    pput(pppfmtfile);
    pppfmtfile.int_ = writeloc;
    pput(pppfmtfile); /*1318:*/
    pppfmtfile.int_ = hashused;
    pput(pppfmtfile);
    cscount = frozencontrolsequence - hashused - 1;
    for (p = hashbase; p <= hashused; p++) {
        if (text(p) != 0) {
            pppfmtfile.int_ = p;
            pput(pppfmtfile);
            pppfmtfile.hh = hash[p - hashbase];
            pput(pppfmtfile);
            cscount++;
        }
    }
    for (p = hashused + 1; p < undefinedcontrolsequence; p++) {
        pppfmtfile.hh = hash[p - hashbase];
        pput(pppfmtfile);
    }
    pppfmtfile.int_ = cscount;
    pput(pppfmtfile);
    println();
    print_int(cscount); /*:1318*/
    /*:1313*/
    print(S(994));
    fonts_dump(fmtfile);
    /*1324:*/
    pppfmtfile.int_ = hyphcount;
    pput(pppfmtfile);
    for (k = 0; k <= hyphsize; k++) {
        if (hyphword[k] != 0) {
            pppfmtfile.int_ = k;
            pput(pppfmtfile);
            pppfmtfile.int_ = hyphword[k];
            pput(pppfmtfile);
            pppfmtfile.int_ = hyphlist[k];
            pput(pppfmtfile);
        }
    }
    println();
    print_int(hyphcount);
    print(S(995));
    if (hyphcount != 1) print_char('s');
    if (trie_not_ready) inittrie();
    pppfmtfile.int_ = triemax;
    pput(pppfmtfile);
    for (k = 0; k <= triemax; k++) {
        pppfmtfile.hh = trie[k];
        pput(pppfmtfile);
    }
    pppfmtfile.int_ = trieopptr;
    pput(pppfmtfile);
    for (k = 0; k < trieopptr; k++) {
        pppfmtfile.int_ = hyfdistance[k];
        pput(pppfmtfile);
        pppfmtfile.int_ = hyfnum[k];
        pput(pppfmtfile);
        pppfmtfile.int_ = hyfnext[k];
        pput(pppfmtfile);
    }
    printnl(S(996));
    print_int(triemax);
    print(S(997));
    print_int(trieopptr);
    print(S(998));
    if (trieopptr != 1) print_char('s');
    print(S(999));
    print_int(trieopsize);
    for (k = 255; k >= 0; k--) {            /*1326:*/
        if (trieused[k] > minquarterword) { /*:1324*/
            printnl(S(675));
            print_int(trieused[k] - minquarterword);
            print(S(1000));
            print_int(k);
            pppfmtfile.int_ = k;
            pput(pppfmtfile);
            pppfmtfile.int_ = trieused[k] - minquarterword;
            pput(pppfmtfile);
        }
    }
    pppfmtfile.int_ = interaction;
    pput(pppfmtfile);
    pppfmtfile.int_ = formatident;
    pput(pppfmtfile);
    pppfmtfile.int_ = 69069L;
    pput(pppfmtfile);
    tracingstats = 0; /*:1326*/
    /*1329:*/
    /*:1329*/
    wclose(&fmtfile);
} // storefmtfile
#endif // #1302: tt_INIT


/*1348:*/
/*1349:*/
Static void newwhatsit(SmallNumber s, SmallNumber w) {
    Pointer p;

    p = getnode(w);
    type(p) = whatsitnode;
    subtype(p) = s;
    link(tail) = p;
    tail = p;
}
/*:1349*/

/*1350:*/
Static void newwritewhatsit(SmallNumber w) {
    newwhatsit(curchr, w);
    if (w != writenodesize)
        scanfourbitint();
    else {
        scanint();
        if (curval < 0)
            curval = 17;
        else if (curval > 15)
            curval = 16;
    }
    writestream(tail) = curval;
}
/*:1350*/

Static void doextension(void)
{
  long k;
  Pointer p;

  switch (curchr) {

  case opennode:   /*1351:*/
    newwritewhatsit(opennodesize);
    scanoptionalequals();
    scanfilename();
    openname(tail) = curname;
    openarea(tail) = curarea;
    openext(tail) = curext;
    break;
    /*:1351*/

  case writenode:   /*1352:*/
    k = curcs;
    newwritewhatsit(writenodesize);
    curcs = k;
    p = scantoks(false, false);
    writetokens(tail) = defref;
    break;
    /*:1352*/

  case closenode:   /*1353:*/
    newwritewhatsit(writenodesize);
    writetokens(tail) = 0;
    break;
    /*:1353*/

  case specialnode:   /*1354:*/
    newwhatsit(specialnode, writenodesize);
    writestream(tail) = 0;
    p = scantoks(false, true);
    writetokens(tail) = defref;
    break;
    /*:1354*/

  case immediatecode:   /*1375:*/
    getxtoken();
    if (curcmd == extension && curchr <= closenode) {
      p = tail;
      doextension();
      outwhat(tail);
      flushnodelist(tail);
      tail = p;
      link(p) = 0;
    } else
      backinput();
    break;
    /*:1375*/

  case setlanguagecode:   /*1377:*/
    if (labs(mode) != hmode)
      reportillegalcase();
    else {   /*:1377*/
      newwhatsit(languagenode, smallnodesize);
      scanint();
      if (curval <= 0)
	clang = 0;
      else if (curval > 255)
	clang = 0;
      else
	clang = curval;
      whatlang(tail) = clang;
      whatlhm(tail) = normmin(lefthyphenmin);
      whatrhm(tail) = normmin(righthyphenmin);
    }
    break;

  default:
    confusion(S(1001));
    break;
  }
}
/*:1348*/

/*1376:*/
Static void fixlanguage(void) {
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

/*1068:*/
Static void handlerightbrace(void) {
    Pointer p, q;
    Scaled d;
    long f;

    switch (curgroup) {

        case simplegroup:
            unsave();
            break;

        case bottomlevel:
            printnl(S(292));
            print(S(1002));
            help2(S(1003), S(1004));
            error();
            break;

        case semisimplegroup:
        case mathshiftgroup:
        case mathleftgroup:
            extrarightbrace();
            break;

        /*1085:*/
        case hboxgroup:
            package(0);
            break;

        case adjustedhboxgroup:
            adjusttail = adjusthead;
            package(0);
            break;

        case vboxgroup:
            endgraf();
            package(0);
            break;

        case vtopgroup: /*:1085*/
            endgraf();
            package(vtopcode);
            break;
            /*1100:*/

        case insertgroup:
            endgraf();
            q = splittopskip;
            addglueref(q);
            d = splitmaxdepth;
            f = floatingpenalty;
            unsave();
            saveptr--;
            p = vpack(link(head), 0, additional);
            popnest();
            if (saved(0) < 255) {
                tailappend(getnode(insnodesize));
                type(tail) = insnode;
                subtype(tail) = saved(0);
                height(tail) = height(p) + depth(p);
                insptr(tail) = listptr(p);
                splittopptr(tail) = q;
                depth(tail) = d;
                floatcost(tail) = f;
            } else {
                tailappend(getnode(smallnodesize));
                type(tail) = adjustnode;
                subtype(tail) = 0;
                adjustptr(tail) = listptr(p);
                deleteglueref(q);
            }
            freenode(p, boxnodesize);
            if (nestptr == 0) buildpage();
            break;

        case outputgroup: /*1026:*/
            if (loc != 0 ||
                (tokentype != outputtext && tokentype != backedup)) { /*:1027*/
                printnl(S(292));
                print(S(1005));
                help2(S(1006), S(682));
                error();
                do {
                    gettoken();
                } while (loc != 0);
            }
            endtokenlist();
            endgraf();
            unsave();
            outputactive = false;
            insertpenalties = 0; /*1028:*/
            if (box(255) != 0) { /*:1028*/
                printnl(S(292));
                print(S(1007));
                print_esc(S(464));
                print_int(255);
                help3(S(1008), S(1009), S(1010));
                boxerror(255);
            }
            if (tail != head) {
                link(pagetail) = link(head);
                pagetail = tail;
            }
            if (link(pagehead) != 0) {
                if (link(contribhead) == 0) contribtail = pagetail;
                link(pagetail) = link(contribhead);
                link(contribhead) = link(pagehead);
                link(pagehead) = 0;
                pagetail = pagehead;
            }
            popnest();
            buildpage();
            break;
            /*:1026*/
            /*:1100*/
            /*1118:*/

        case discgroup: /*:1118*/
            builddiscretionary();
            break;
            /*1132:*/

        case aligngroup: /*:1132*/
            backinput();
            curtok = cstokenflag + frozencr;
            printnl(S(292));
            print(S(554));
            print_esc(S(737));
            print(S(555));
            help1(S(1011));
            inserror();
            break;
            /*1133:*/

        case noaligngroup: /*:1133*/
            endgraf();
            unsave();
            alignpeek();
            break;
            /*1168:*/

        case vcentergroup: /*:1168*/
            endgraf();
            unsave();
            saveptr -= 2;
            p = vpack(link(head), saved(1), saved(0));
            popnest();
            tailappend(newnoad());
            type(tail) = vcenternoad;
            mathtype(nucleus(tail)) = subbox;
            info(nucleus(tail)) = p;
            break;
            /*1173:*/

        case mathchoicegroup: /*:1173*/
            buildchoices();
            break;
            /*1186:*/

        case mathgroup:
            unsave();
            saveptr--;
            mathtype(saved(0)) = submlist;
            p = finmlist(0);
            info(saved(0)) = p;
            if (p != 0) {
                if (link(p) == 0) {
                    if (type(p) == ordnoad) {
                        if (mathtype(subscr(p)) == empty) {
                            if (mathtype(supscr(p)) == empty) {
                                mem[saved(0) - memmin].hh =
                                    mem[nucleus(p) - memmin].hh;
                                freenode(p, noadsize);
                            }
                        }
                    } else if (type(p) == accentnoad) {
                        if (saved(0) == nucleus(tail)) {
                            if (type(tail) == ordnoad) { /*1187:*/
                                q = head;
                                while (link(q) != tail)
                                    q = link(q);
                                link(q) = p;
                                freenode(tail, noadsize);
                                tail = p;
                            }
                            /*:1187*/
                        }
                    }
                }
            }
            break;
            /*:1186*/

        default:
            confusion(S(1012));
            break;
    }

    /*1027:*/
} /*:1068*/


Static void maincontrol(void) {
    long t;

    if (everyjob != 0) begintokenlist(everyjob, everyjobtext);
_Lbigswitch_:
    getxtoken();
_Lreswitch: /*1031:*/
    if (interrupt != 0) {
        if (OKtointerrupt) {
            backinput();
            checkinterrupt();
            goto _Lbigswitch_;
        }
    }
#ifdef tt_DEBUG
    if (panicking) checkmem(false);
#endif // #1031: tt_DEBUG
    if (tracingcommands > 0) /*:1031*/
        showcurcmdchr();
    switch (labs(mode) + curcmd) {
        case hmode + letter:
        case hmode + otherchar:
        case hmode + chargiven:
            goto _Lmainloop;
            break;

        case hmode + charnum:
            scancharnum();
            curchr = curval;
            goto _Lmainloop;
            break;

        case hmode + noboundary:
            getxtoken();
            if (curcmd == letter || curcmd == otherchar ||
                curcmd == chargiven || curcmd == charnum)
                cancelboundary = true;
            goto _Lreswitch;
            break;

        case hmode + spacer:
            if (spacefactor == 1000) goto _Lappendnormalspace_;
            appspace();
            break;

        case hmode + exspace:
        case mmode + exspace: /*1045:*/
            goto _Lappendnormalspace_;
            break;

        case vmode:
        case hmode:
        case mmode:
        case vmode + spacer:
        case mmode + spacer:
        case mmode + noboundary:
            /* blank case */
            break;

        case vmode + ignorespaces:
        case hmode + ignorespaces:
        case mmode + ignorespaces:
            skip_spaces();
            goto _Lreswitch;
            break;

        case vmode + stop: /*1048:*/
            if (itsallover()) goto _Lexit;
            break;

        case vmode + vmove:
        case hmode + hmove:
        case mmode + hmove:
        case vmode + lastitem:
        case hmode + lastitem:
        case mmode + lastitem:
        case vmode + vadjust:
        case vmode + italcorr:
        case vmode + eqno:
        case hmode + eqno:
        case vmode + macparam:
        case hmode + macparam:
        case mmode + macparam: /*:1048*/
            reportillegalcase();
            break;
            /*1046:*/

        case vmode + supmark:
        case hmode + supmark:
        case vmode + submark:
        case hmode + submark:
        case vmode + mathcharnum:
        case hmode + mathcharnum:
        case vmode + mathgiven:
        case hmode + mathgiven:
        case vmode + mathcomp:
        case hmode + mathcomp:
        case vmode + delimnum:
        case hmode + delimnum:
        case vmode + leftright:
        case hmode + leftright:
        case vmode + above:
        case hmode + above:
        case vmode + radical:
        case hmode + radical:
        case vmode + mathstyle:
        case hmode + mathstyle:
        case vmode + mathchoice:
        case hmode + mathchoice:
        case vmode + vcenter:
        case hmode + vcenter:
        case vmode + nonscript:
        case hmode + nonscript:
        case vmode + mkern:
        case hmode + mkern:
        case vmode + limitswitch:
        case hmode + limitswitch:
        case vmode + mskip:
        case hmode + mskip:
        case vmode + mathaccent:
        case hmode + mathaccent:
        case mmode + endv:
        case mmode + parend:
        case mmode + stop:
        case mmode + vskip:
        case mmode + unvbox:
        case mmode + valign:
        case mmode + hrule: /*:1046*/
            insertdollarsign();
            break;

        /*1056:*/
        case vmode + hrule:
        case hmode + vrule:
        case mmode + vrule: /*:1056*/
            tailappend(scanrulespec());
            if (labs(mode) == vmode)
                prevdepth = ignoredepth;
            else if (labs(mode) == hmode)
                spacefactor = 1000;
            break;
            /*1057:*/

        case vmode + vskip:
        case hmode + hskip:
        case mmode + hskip:
        case mmode + mskip:
            appendglue();
            break;

        case vmode + kern:
        case hmode + kern:
        case mmode + kern:
        case mmode + mkern: /*:1057*/
            appendkern();
            break;
            /*1063:*/

        case vmode + leftbrace:
        case hmode + leftbrace:
            newsavelevel(simplegroup);
            break;

        case vmode + begingroup:
        case hmode + begingroup:
        case mmode + begingroup:
            newsavelevel(semisimplegroup);
            break;

        case vmode + endgroup:
        case hmode + endgroup:
        case mmode + endgroup: /*:1063*/
            if (curgroup == semisimplegroup)
                unsave();
            else
                offsave();
            break;
            /*1067:*/

        case vmode + rightbrace:
        case hmode + rightbrace:
        case mmode + rightbrace:
            handlerightbrace();
            break;

        /*:1067*/
        /*1073:*/
        case vmode + hmove:
        case hmode + vmove:
        case mmode + vmove:
            t = curchr;
            scannormaldimen();
            if (t == 0)
                scanbox(curval);
            else
                scanbox(-curval);
            break;

        case vmode + leadership:
        case hmode + leadership:
        case mmode + leadership:
            scanbox(leaderflag - aleaders + curchr);
            break;

        case vmode + makebox:
        case hmode + makebox:
        case mmode + makebox:
            beginbox(0);
            break;

        /*:1073*/
        /*1090:*/
        case vmode + startpar:
            newgraf(curchr > 0);
            break;

        case vmode + letter:
        case vmode + otherchar:
        case vmode + charnum:
        case vmode + chargiven:
        case vmode + mathshift:
        case vmode + unhbox:
        case vmode + vrule:
        case vmode + accent:
        case vmode + discretionary:
        case vmode + hskip:
        case vmode + valign:
        case vmode + exspace:
        case vmode + noboundary: /*:1090*/
            backinput();
            newgraf(true);
            break;
            /*1092:*/

        case hmode + startpar:
        case mmode + startpar: /*:1092*/
            indentinhmode();
            break;
            /*1094:*/

        case vmode + parend:
            normalparagraph();
            if (mode > 0) buildpage();
            break;

        case hmode + parend:
            if (alignstate < 0) offsave();
            endgraf();
            if (mode == vmode) buildpage();
            break;

        case hmode + stop:
        case hmode + vskip:
        case hmode + hrule:
        case hmode + unvbox:
        case hmode + halign: /*:1094*/
            headforvmode();
            break;
            /*1097:*/

        case vmode + insert_:
        case hmode + insert_:
        case mmode + insert_:
        case hmode + vadjust:
        case mmode + vadjust:
            begininsertoradjust();
            break;

        case vmode + mark_:
        case hmode + mark_:
        case mmode + mark_: /*:1097*/
            makemark();
            break;

        /*1102:*/
        case vmode + breakpenalty:
        case hmode + breakpenalty:
        case mmode + breakpenalty:
            appendpenalty();
            break;

        /*:1102*/
        /*1104:*/
        case vmode + removeitem:
        case hmode + removeitem:
        case mmode + removeitem: /*:1104*/
            deletelast();
            break;

        /*1109:*/
        case vmode + unvbox:
        case hmode + unhbox:
        case mmode + unhbox:
            unpackage();
            break;

        /*:1109*/
        /*1112:*/
        case hmode + italcorr:
            appenditaliccorrection();
            break;

        case mmode + italcorr: /*:1112*/
            tailappend(newkern(0));
            break;
            /*1116:*/

        case hmode + discretionary:
        case mmode + discretionary: /*:1116*/
            appenddiscretionary();
            break;

        /*1122:*/
        case hmode + accent:
            makeaccent();
            break;

        /*:1122*/
        /*1126:*/
        case vmode + carret:
        case hmode + carret:
        case mmode + carret:
        case vmode + tabmark:
        case hmode + tabmark:
        case mmode + tabmark:
            alignerror();
            break;

        case vmode + noalign:
        case hmode + noalign:
        case mmode + noalign:
            noalignerror();
            break;

        case vmode + omit:
        case hmode + omit:
        case mmode + omit: /*:1126*/
            omiterror();
            break;
            /*1130:*/

        case vmode + halign:
        case hmode + valign:
            initalign();
            break;

        case mmode + halign:
            if (privileged()) {
                if (curgroup == mathshiftgroup)
                    initalign();
                else
                    offsave();
            }
            break;

        case vmode + endv:
        case hmode + endv: /*:1130*/
            doendv();
            break;
            /*1134:*/

        case vmode + endcsname:
        case hmode + endcsname:
        case mmode + endcsname: /*:1134*/
            cserror();
            break;
            /*1137:*/

        case hmode + mathshift: /*:1137*/
            initmath();
            break;
            /*1140:*/

        case mmode + eqno: /*:1140*/
            if (privileged()) {
                if (curgroup == mathshiftgroup)
                    starteqno();
                else
                    offsave();
            }
            break;
            /*1150:*/

        case mmode + leftbrace: /*:1150*/
            tailappend(newnoad());
            backinput();
            scanmath(nucleus(tail));
            break;
            /*1154:*/

        case mmode + letter:
        case mmode + otherchar:
        case mmode + chargiven:
            setmathchar(mathcode(curchr));
            break;

        case mmode + charnum:
            scancharnum();
            curchr = curval;
            setmathchar(mathcode(curchr));
            break;

        case mmode + mathcharnum:
            scanfifteenbitint();
            setmathchar(curval);
            break;

        case mmode + mathgiven:
            setmathchar(curchr);
            break;

        case mmode + delimnum: /*:1154*/
            scantwentysevenbitint();
            setmathchar(curval / 4096);
            break;
            /*1158:*/

        case mmode + mathcomp:
            tailappend(newnoad());
            type(tail) = curchr;
            scanmath(nucleus(tail));
            break;

        case mmode + limitswitch: /*:1158*/
            mathlimitswitch();
            break;
            /*1162:*/

        case mmode + radical: /*:1162*/
            mathradical();
            break;
            /*1164:*/

        case mmode + accent:
        case mmode + mathaccent: /*:1164*/
            mathac();
            break;
            /*1167:*/

        case mmode + vcenter:
            scanspec(vcentergroup, false);
            normalparagraph();
            pushnest();
            mode = -vmode;
            prevdepth = ignoredepth;
            if (everyvbox != 0) begintokenlist(everyvbox, everyvboxtext);
            break;
            /*:1167*/

        /*1171:*/
        case mmode + mathstyle:
            tailappend(newstyle(curchr));
            break;

        case mmode + nonscript:
            tailappend(newglue(zeroglue));
            subtype(tail) = condmathglue;
            break;

        case mmode + mathchoice:
            appendchoices();
            break;

        /*:1171*/
        /*1175:*/
        case mmode + submark:
        case mmode + supmark:
            subsup();
            break;

        /*:1175*/
        /*1180:*/
        case mmode + above: /*:1180*/
            mathfraction();
            break;
            /*1190:*/

        case mmode + leftright:
            mathleftright();
            break;

        /*:1190*/
        /*1193:*/
        case mmode + mathshift:
            if (curgroup == mathshiftgroup)
                aftermath();
            else
                offsave();
            break;

        /*:1193*/
        /*1210:*/
        case vmode + toksregister:
        case hmode + toksregister:
        case mmode + toksregister:
        case vmode + assigntoks:
        case hmode + assigntoks:
        case mmode + assigntoks:
        case vmode + assignint:
        case hmode + assignint:
        case mmode + assignint:
        case vmode + assigndimen:
        case hmode + assigndimen:
        case mmode + assigndimen:
        case vmode + assignglue:
        case hmode + assignglue:
        case mmode + assignglue:
        case vmode + assignmuglue:
        case hmode + assignmuglue:
        case mmode + assignmuglue:
        case vmode + assignfontdimen:
        case hmode + assignfontdimen:
        case mmode + assignfontdimen:
        case vmode + assignfontint:
        case hmode + assignfontint:
        case mmode + assignfontint:
        case vmode + setaux:
        case hmode + setaux:
        case mmode + setaux:
        case vmode + setprevgraf:
        case hmode + setprevgraf:
        case mmode + setprevgraf:
        case vmode + setpagedimen:
        case hmode + setpagedimen:
        case mmode + setpagedimen:
        case vmode + setpageint:
        case hmode + setpageint:
        case mmode + setpageint:
        case vmode + setboxdimen:
        case hmode + setboxdimen:
        case mmode + setboxdimen:
        case vmode + setshape:
        case hmode + setshape:
        case mmode + setshape:
        case vmode + defcode:
        case hmode + defcode:
        case mmode + defcode:
        case vmode + deffamily:
        case hmode + deffamily:
        case mmode + deffamily:
        case vmode + setfont:
        case hmode + setfont:
        case mmode + setfont:
        case vmode + deffont:
        case hmode + deffont:
        case mmode + deffont:
        case vmode + register_:
        case hmode + register_:
        case mmode + register_:
        case vmode + advance:
        case hmode + advance:
        case mmode + advance:
        case vmode + multiply:
        case hmode + multiply:
        case mmode + multiply:
        case vmode + divide:
        case hmode + divide:
        case mmode + divide:
        case vmode + prefix:
        case hmode + prefix:
        case mmode + prefix:
        case vmode + let:
        case hmode + let:
        case mmode + let:
        case vmode + shorthanddef:
        case hmode + shorthanddef:
        case mmode + shorthanddef:
        case vmode + readtocs:
        case hmode + readtocs:
        case mmode + readtocs:
        case vmode + def:
        case hmode + def:
        case mmode + def:
        case vmode + setbox:
        case hmode + setbox:
        case mmode + setbox:
        case vmode + hyphdata:
        case hmode + hyphdata:
        case mmode + hyphdata:
        case vmode + setinteraction:
        case hmode + setinteraction:
        case mmode + setinteraction: /*:1210*/
            prefixedcommand();
            break;
            /*1268:*/

        case vmode + afterassignment:
        case hmode + afterassignment:
        case mmode + afterassignment: /*:1268*/
            gettoken();
            aftertoken = curtok;
            break;
            /*1271:*/

        case vmode + aftergroup:
        case hmode + aftergroup:
        case mmode + aftergroup: /*:1271*/
            gettoken();
            saveforafter(curtok);
            break;
            /*1274:*/

        case vmode + instream:
        case hmode + instream:
        case mmode + instream: /*:1274*/
            openorclosein();
            break;

        /*1276:*/
        case vmode + message:
        case hmode + message:
        case mmode + message:
            issuemessage();
            break;

        /*:1276*/
        /*1285:*/
        case vmode + caseshift:
        case hmode + caseshift:
        case mmode + caseshift:
            shiftcase();
            break;

        /*:1285*/
        /*1290:*/
        case vmode + xray:
        case hmode + xray:
        case mmode + xray:
            showwhatever();
            break;

        /*:1290*/
        /*1347:*/
        case vmode + extension:
        case hmode + extension:
        case mmode + extension: /*:1347*/
            doextension();
            break;
            /*:1045*/
    }
    goto _Lbigswitch_;
_Lmainloop:
    /*1034:*/
    adjustspacefactor();
    mainf = curfont;
    bchar = fontbchar[mainf];
    falsebchar = fontfalsebchar[mainf];
    if (mode > 0) {
        if (language != clang) fixlanguage();
    }
    FAST_GET_AVAIL(ligstack);
    font(ligstack) = mainf;
    curl = curchr;
    character(ligstack) = curl;
    curq = tail;
    if (cancelboundary) {
        cancelboundary = false;
        maink = nonaddress;
    } else
        maink = bcharlabel[mainf];
    if (maink == nonaddress) goto _Lmainloopmove2;
    curr = curl;
    curl = nonchar;
    goto _Lmainligloop1;
_Lmainloopwrapup:  /*1035:*/
    wrapup(rthit); /*:1035*/
_Lmainloopmove:    /*1036:*/
    if (ligstack == 0) goto _Lreswitch;
    curq = tail;
    curl = character(ligstack);
_Lmainloopmove1:
    if (!ischarnode(ligstack)) goto _Lmainloopmovelig;
_Lmainloopmove2:
    if (curchr < fontbc[mainf] || curchr > fontec[mainf]) {
        charwarning(mainf, curchr);
        FREE_AVAIL(ligstack);
        goto _Lbigswitch_;
    }
    maini = charinfo(mainf, curl);
    if (!charexists(maini)) {
        charwarning(mainf, curchr);
        FREE_AVAIL(ligstack);
        goto _Lbigswitch_;
    }
    tailappend(ligstack); /*:1036*/
_Lmainlooplookahead:      /*1038:*/
    getnext();
    if (curcmd == letter) goto _Lmainlooplookahead1;
    if (curcmd == otherchar) goto _Lmainlooplookahead1;
    if (curcmd == chargiven) goto _Lmainlooplookahead1;
    xtoken();
    if (curcmd == letter) goto _Lmainlooplookahead1;
    if (curcmd == otherchar) goto _Lmainlooplookahead1;
    if (curcmd == chargiven) goto _Lmainlooplookahead1;
    if (curcmd == charnum) {
        scancharnum();
        curchr = curval;
        goto _Lmainlooplookahead1;
    }
    if (curcmd == noboundary) bchar = nonchar;
    curr = bchar;
    ligstack = 0;
    goto _Lmainligloop;
_Lmainlooplookahead1:
    adjustspacefactor();
    FAST_GET_AVAIL(ligstack);
    font(ligstack) = mainf;
    curr = curchr;
    character(ligstack) = curr;
    if (curr == falsebchar) curr = nonchar; /*:1038*/
_Lmainligloop:                              /*1039:*/
    if (chartag(maini) != ligtag) {
        goto _Lmainloopwrapup;
    }
    maink = ligkernstart(mainf, maini);
    mainj = fontinfo[maink].qqqq;
    if (skipbyte(mainj) <= stopflag) goto _Lmainligloop2;
    maink = ligkernrestart(mainf, mainj);
_Lmainligloop1:
    mainj = fontinfo[maink].qqqq;
_Lmainligloop2:
    if (nextchar(mainj) == curr) {
        if (skipbyte(mainj) <= stopflag) { /*1040:*/
            if (opbyte(mainj) >= kernflag) {
                wrapup(rthit);
                tailappend(newkern(charkern(mainf, mainj)));
                goto _Lmainloopmove;
            }
            if (curl == nonchar)
                lfthit = true;
            else if (ligstack == 0)
                rthit = true;
            checkinterrupt();
            switch (opbyte(mainj)) {

                case minquarterword + 1:
                case minquarterword + 5:
                    curl = rembyte(mainj);
                    maini = charinfo(mainf, curl);
                    ligaturepresent = true;
                    break;

                case minquarterword + 2:
                case minquarterword + 6:
                    curr = rembyte(mainj);
                    if (ligstack == 0) {
                        ligstack = newligitem(curr);
                        bchar = nonchar;
                    } else if (ischarnode(ligstack)) {
                        mainp = ligstack;
                        ligstack = newligitem(curr);
                        ligptr(ligstack) = mainp;
                    } else
                        character(ligstack) = curr;
                    break;

                case minquarterword + 3:
                    curr = rembyte(mainj);
                    mainp = ligstack;
                    ligstack = newligitem(curr);
                    link(ligstack) = mainp;
                    break;

                case minquarterword + 7:
                case minquarterword + 11:
                    wrapup(false);
                    curq = tail;
                    curl = rembyte(mainj);
                    maini = charinfo(mainf, curl);
                    ligaturepresent = true;
                    break;

                default:
                    curl = rembyte(mainj);
                    ligaturepresent = true;
                    if (ligstack == 0)
                        goto _Lmainloopwrapup;
                    else
                        goto _Lmainloopmove1;
                    break;
            }
            if (opbyte(mainj) > minquarterword + 4) {
                if (opbyte(mainj) != minquarterword + 7) goto _Lmainloopwrapup;
            }
            if (curl < nonchar) goto _Lmainligloop;
            maink = bcharlabel[mainf];
            goto _Lmainligloop1;
        }
        /*:1040*/
    }
    if (skipbyte(mainj) == minquarterword)
        maink++;
    else {
        if (skipbyte(mainj) >= stopflag) goto _Lmainloopwrapup;
        maink += skipbyte(mainj) - minquarterword + 1;
    }
    goto _Lmainligloop1; /*:1039*/
_Lmainloopmovelig:       /*1037:*/
    mainp = ligptr(ligstack);
    if (mainp > 0) {
        tailappend(mainp);
    }
    tempptr = ligstack;
    ligstack = link(tempptr);
    freenode(tempptr, smallnodesize);
    maini = charinfo(mainf, curl);
    ligaturepresent = true;
    if (ligstack == 0) {
        if (mainp > 0) goto _Lmainlooplookahead;
        curr = bchar;
    } else
        curr = character(ligstack);
    goto _Lmainligloop;          /*:1037*/
                                 /*:1034*/
_Lappendnormalspace_:            /*1041:*/
    if (spaceskip == zeroglue) { /*1042:*/
        mainp = fontglue[curfont];
        if (mainp == 0) { /*:1042*/
            FontIndex mmaink;
            mainp = newspec(zeroglue);
            mmaink = parambase[curfont] + spacecode;
#if 1
            maink = mmaink;
#endif
            width(mainp) = fontinfo[mmaink].sc;
            stretch(mainp) = fontinfo[mmaink + 1].sc;
            shrink(mainp) = fontinfo[mmaink + 2].sc;
            fontglue[curfont] = mainp;
        }
        tempptr = newglue(mainp);
    } else
        tempptr = newparamglue(spaceskipcode);
    link(tail) = tempptr;
    tail = tempptr;
    goto _Lbigswitch_; /*:1041*/
_Lexit:;
}
/*:1030*/

/*1284:*/
Static void giveerrhelp(void) { tokenshow(errhelp); }
/*:1284*/

/*1303:*/
Static Boolean openfmtfile(void) { return open_fmt(&fmtfile, stdout); }
/*:524*/

Static Boolean loadfmtfile(void) { /*1308:*/
    Boolean Result;
    long j, k, x;
    Pointer p, q;
    /* FourQuarters w; */
    MemoryWord pppfmtfile;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x != 371982687L) goto _Lbadfmt_;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x != membot) goto _Lbadfmt_;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x != memtop) goto _Lbadfmt_;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x != eqtbsize) goto _Lbadfmt_;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x != hashprime) goto _Lbadfmt_;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x != hyphsize) /*1310:*/
        goto _Lbadfmt_;
    if (!str_undump(fmtfile, stdout)) goto _Lbadfmt_;
    /*1312:*/
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x < lomemstatmax + 1000 || x >= himemstatmin) goto _Lbadfmt_;
    lomemmax = x;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x <= lomemstatmax || x > lomemmax) goto _Lbadfmt_;
    rover = x;
    p = membot;
    q = rover;
    do {
        for (k = p; k <= q + 1; k++) {
            pget(pppfmtfile);
            mem[k - memmin] = pppfmtfile;
        }
        p = q + nodesize(q);
        if ((p > lomemmax) | ((q >= rlink(q)) & (rlink(q) != rover)))
            goto _Lbadfmt_;
        q = rlink(q);
    } while (q != rover);
    for (k = p; k <= lomemmax; k++) {
        pget(pppfmtfile);
        mem[k - memmin] = pppfmtfile;
    }
    if (memmin < membot - 2) {
        p = llink(rover);
        q = memmin + 1;
        link(memmin) = 0;
        info(memmin) = 0;
        rlink(p) = q;
        llink(rover) = q;
        rlink(q) = rover;
        llink(q) = p;
        link(q) = emptyflag;
        nodesize(q) = membot - q;
    }
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x <= lomemmax || x > himemstatmin) goto _Lbadfmt_;
    himemmin = x;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if ((unsigned long)x > memtop) goto _Lbadfmt_;
    avail = x;
    memend = memtop;
    for (k = himemmin; k <= memend; k++) {
        pget(pppfmtfile);
        mem[k - memmin] = pppfmtfile;
    }
    pget(pppfmtfile);
    varused = pppfmtfile.int_;
    pget(pppfmtfile);
    dynused = pppfmtfile.int_; /*:1312*/
    /*1314:*/
    /*1317:*/
    k = activebase;
    do {
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if (x < 1 || k + x > eqtbsize + 1) goto _Lbadfmt_;
        for (j = k; j < k + x; j++) {
            pget(pppfmtfile);
            eqtb[j - activebase] = pppfmtfile;
        }
        k += x;
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if (x < 0 || k + x > eqtbsize + 1) goto _Lbadfmt_;
        for (j = k; j < k + x; j++)
            eqtb[j - activebase] = eqtb[k - activebase - 1];
        k += x; /*:1317*/
    } while (k <= eqtbsize);
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x < hashbase || x > frozencontrolsequence) goto _Lbadfmt_;
    parloc = x;
    partoken = cstokenflag + parloc;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x < hashbase || x > frozencontrolsequence) /*1319:*/
        goto _Lbadfmt_;
    writeloc = x;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x < hashbase || x > frozencontrolsequence) goto _Lbadfmt_;
    hashused = x;
    p = hashbase - 1;
    do {
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if (x <= p || x > hashused) goto _Lbadfmt_;
        p = x;
        pget(pppfmtfile);
        hash[p - hashbase] = pppfmtfile.hh;
    } while (p != hashused);
    for (p = hashused + 1; p < undefinedcontrolsequence; p++) {
        pget(pppfmtfile);
        hash[p - hashbase] = pppfmtfile.hh;
    }
    pget(pppfmtfile);
    cscount = pppfmtfile.int_; /*:1319*/
    /*:1314*/
    if (!fonts_undump(fmtfile, stdout)) goto _Lbadfmt_;
    /*1325:*/
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if ((unsigned long)x > hyphsize) goto _Lbadfmt_;
    hyphcount = x;
    for (k = 1; k <= hyphcount; k++) {
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if ((unsigned long)x > hyphsize) goto _Lbadfmt_;
        j = x;
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if (!str_valid(x)) goto _Lbadfmt_;
        hyphword[j] = x;
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if ((unsigned long)x > maxhalfword) goto _Lbadfmt_;
        hyphlist[j] = x;
    }
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x < 0) goto _Lbadfmt_;
    if (x > triesize) {
        fprintf(stdout, "---! Must increase the trie size\n");
        goto _Lbadfmt_;
    }
    j = x;
#ifdef tt_INIT
    triemax = j;
#endif // #1325.1: tt_INIT
    for (k = 0; k <= j; k++) {
        pget(pppfmtfile);
        trie[k] = pppfmtfile.hh;
    }
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x < 0) goto _Lbadfmt_;
    if (x > trieopsize) {
        fprintf(stdout, "---! Must increase the trie op size\n");
        goto _Lbadfmt_;
    }
    j = x;
#ifdef tt_INIT
    trieopptr = j;
#endif // #1325.2: tt_INIT
    for (k = 1; k <= j; k++) {
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if ((unsigned long)x > 63) goto _Lbadfmt_;
        hyfdistance[k - 1] = x;
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if ((unsigned long)x > 63) goto _Lbadfmt_;
        hyfnum[k - 1] = x;
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if ((unsigned long)x > maxquarterword) goto _Lbadfmt_;
        hyfnext[k - 1] = x;
    }
#ifdef tt_INIT
    for (k = 0; k <= 255; k++)
        trieused[k] = minquarterword;
#endif // #1325.3: tt_INIT
    k = 256;
    while (j > 0) {
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if (x >= k) goto _Lbadfmt_;
        k = x;
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if (x < 1 || x > j) goto _Lbadfmt_;
#ifdef tt_INIT
        trieused[k] = x;
#endif // #1325.4: tt_INIT
        j -= x;
        opstart[k] = j - minquarterword;
    }
#ifdef tt_INIT
    trie_not_ready = false;
#endif // #1325.5: tt_INIT
       /*:1325*/

    /*1327:*/
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if ((unsigned long)x > errorstopmode) goto _Lbadfmt_;
    interaction = x;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (!str_valid(x)) goto _Lbadfmt_;
    formatident = x;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if ((x != 69069L) | feof(fmtfile)) goto _Lbadfmt_; /*:1327*/
    Result = true;
    goto _Lexit;
_Lbadfmt_:
    fprintf(stdout, "(Fatal format file error; I'm stymied)\n");
    Result = false;
_Lexit:
    return Result;
}
/*:1303*/

/*1330:*/
/*1333:*/
Static void closefilesandterminate(void) { /*1378:*/
    long k;
    for (k = 0; k <= 15; k++) {
        if (writeopen[k]) /*:1378*/
            aclose(&write_file[k]);
    }

    #ifdef tt_STAT
        if (tracingstats > 0) {
            /*1334:*/
            if (logopened) { /*:1334*/
                fprintf(log_file, " \n");
                fprintf(log_file, "Here is how much of TeX's memory you used:\n");
                str_print_stats(log_file);
                fprintf(log_file,
                        " %ld words of memory out of %ld\n",
                        lomemmax - memmin + memend - himemmin + 2L,
                        memend - memmin + 1L);
                fprintf(log_file,
                        " %ld multiletter control sequences out of %ld\n",
                        cscount,
                        (long)hashsize);
                fprintf(log_file,
                        " %d words of font info for %d font",
                        fmemptr,
                        fontptr);
                if (fontptr != 1) {
                    fprintf(log_file, "s");
                }
                fprintf(log_file,
                        ", out of %ld for %ld\n",
                        (long)fontmemsize,
                        (long)(fontmax));
                fprintf(log_file, " %d hyphenation exception", hyphcount);
                if (hyphcount != 1) {
                    fprintf(log_file, "s");
                }
                fprintf(log_file, " out of %ld\n", (long)hyphsize);
                fprintf(log_file,
                        " %di,%dn,%ldp,%db,%ds stack positions out of "
                        "%ldi,%ldn,%ldp,%ldb,%lds\n",
                        maxinstack,
                        maxneststack,
                        maxparamstack,
                        maxbufstack + 1,
                        maxsavestack + 6,
                        (long)stacksize,
                        (long)nestsize,
                        (long)paramsize,
                        (long)bufsize,
                        (long)savesize);
            }
        }  // if (tracingstats > 0)
#endif // #1333: tt_STAT

    /*642:*/
    while (curs > -1) {
        if (curs > 0) {
            dvi_pop();
        } else {
            dvi_eop();
            totalpages++;
        }
        curs--;
    }
    if (totalpages == 0) {
        printnl(S(1013));
    } else { /*:642*/
        long total_dvi_bytes;
        preparemag();
        dvipost(25400000L,
                473628672L,
                mag,
                maxv,
                maxh,
                maxpush,
                totalpages,
                fontptr);
        total_dvi_bytes = dviflush();
        printnl(S(1014));
        slow_print(outputfilename);
        print(S(303));
        print_int(totalpages);
        print(S(1015));
        if (totalpages != 1) print_char('s');
        print(S(1016));
        print_int(total_dvi_bytes);
        print(S(1017));
    }
    if (!logopened) return;
    putc('\n', log_file);
    aclose(&log_file);
    selector -= 2;
    if (selector != TERM_ONLY) return;
    printnl(S(1018));
    slow_print(logname);
    print_char('.');
    println();
} // void closefilesandterminate(void)
/*:1333*/

/// p468#1335: Last-minute procedures
Static void finalcleanup(void) {
    SmallNumber c;

    c = curchr;
    if (jobname == 0) openlogfile();
    while (inputptr > 0) {
        if (state == tokenlist)
            endtokenlist();
        else
            endfilereading();
    }
    while (openparens > 0) {
        print(S(1019));
        openparens--;
    }
    if (curlevel > levelone) {
        printnl('(');
        print_esc(S(1020));
        print(S(1021));
        print_int(curlevel - levelone);
        print_char(')');
    }
    while (condptr != 0) {
        printnl('(');
        print_esc(S(1020));
        print(S(1022));
        printcmdchr(iftest, curif);
        if (ifline != 0) {
            print(S(1023));
            print_int(ifline);
        }
        print(S(1024));
        ifline = iflinefield(condptr);
        curif = subtype(condptr);
        tempptr = condptr;
        condptr = link(condptr);
        freenode(tempptr, ifnodesize);
    }
    if (history != SPOTLESS) {
        if (history == WARNING_ISSUED || interaction < errorstopmode) {
            if (selector == TERM_AND_LOG) {
                selector = TERM_ONLY;
                printnl(S(1025));
                selector = TERM_AND_LOG;
            }
        }
    }
    if (c == 1) {
    #ifdef tt_INIT
            for (int i = topmarkcode; i <= splitbotmarkcode; i++) {
                if (curmark[i] != 0) deletetokenref(curmark[i]);
            }
            storefmtfile();
    #endif // #1335: tt_INIT
    }
} // #1335: finalcleanup

#ifdef tt_INIT
// TeX 原语定义
/// p468#1336: initialize all the primitives
Static void initprim(void) {
    /*226:*/
    primitive(S(341), assignglue, gluebase);
    primitive(S(342), assignglue, gluebase + baselineskipcode);
    primitive(S(343), assignglue, gluebase + parskipcode);
    primitive(S(344), assignglue, gluebase + abovedisplayskipcode);
    primitive(S(345), assignglue, gluebase + belowdisplayskipcode);
    primitive(S(346), assignglue, gluebase + abovedisplayshortskipcode);
    primitive(S(347), assignglue, gluebase + belowdisplayshortskipcode);
    primitive(S(348), assignglue, gluebase + leftskipcode);
    primitive(S(349), assignglue, gluebase + rightskipcode);
    primitive(S(350), assignglue, gluebase + topskipcode);
    primitive(S(351), assignglue, gluebase + splittopskipcode);
    primitive(S(352), assignglue, gluebase + tabskipcode);
    primitive(S(353), assignglue, gluebase + spaceskipcode);
    primitive(S(354), assignglue, gluebase + xspaceskipcode);
    primitive(S(355), assignglue, gluebase + parfillskipcode);
    primitive(S(356), assignmuglue, gluebase + thinmuskipcode);
    primitive(S(357), assignmuglue, gluebase + medmuskipcode);
    primitive(S(358), assignmuglue, gluebase + thickmuskipcode);
    /*:226*/
    /*230:*/
    primitive(S(1026), assigntoks, outputroutineloc);
    primitive(S(1027), assigntoks, everyparloc);
    primitive(S(1028), assigntoks, everymathloc);
    primitive(S(1029), assigntoks, everydisplayloc);
    primitive(S(1030), assigntoks, everyhboxloc);
    primitive(S(1031), assigntoks, everyvboxloc);
    primitive(S(1032), assigntoks, everyjobloc);
    primitive(S(1033), assigntoks, everycrloc);
    primitive(S(1034), assigntoks, errhelploc); /*:230*/
    /*238:*/
    primitive(S(1035), assignint, intbase);
    primitive(S(1036), assignint, intbase + tolerancecode);
    primitive(S(1037), assignint, intbase + linepenaltycode);
    primitive(S(1038), assignint, intbase + hyphenpenaltycode);
    primitive(S(1039), assignint, intbase + exhyphenpenaltycode);
    primitive(S(1040), assignint, intbase + clubpenaltycode);
    primitive(S(1041), assignint, intbase + widowpenaltycode);
    primitive(S(1042), assignint, intbase + displaywidowpenaltycode);
    primitive(S(1043), assignint, intbase + brokenpenaltycode);
    primitive(S(1044), assignint, intbase + binoppenaltycode);
    primitive(S(1045), assignint, intbase + relpenaltycode);
    primitive(S(1046), assignint, intbase + predisplaypenaltycode);
    primitive(S(1047), assignint, intbase + postdisplaypenaltycode);
    primitive(S(1048), assignint, intbase + interlinepenaltycode);
    primitive(S(1049), assignint, intbase + doublehyphendemeritscode);
    primitive(S(1050), assignint, intbase + finalhyphendemeritscode);
    primitive(S(1051), assignint, intbase + adjdemeritscode);
    primitive(S(1052), assignint, intbase + magcode);
    primitive(S(1053), assignint, intbase + delimiterfactorcode);
    primitive(S(1054), assignint, intbase + loosenesscode);
    primitive(S(1055), assignint, intbase + timecode);
    primitive(S(1056), assignint, intbase + daycode);
    primitive(S(1057), assignint, intbase + monthcode);
    primitive(S(1058), assignint, intbase + yearcode);
    primitive(S(1059), assignint, intbase + showboxbreadthcode);
    primitive(S(1060), assignint, intbase + showboxdepthcode);
    primitive(S(1061), assignint, intbase + hbadnesscode);
    primitive(S(1062), assignint, intbase + vbadnesscode);
    primitive(S(1063), assignint, intbase + pausingcode);
    primitive(S(1064), assignint, intbase + tracingonlinecode);
    primitive(S(1065), assignint, intbase + tracingmacroscode);
    primitive(S(1066), assignint, intbase + tracingstatscode);
    primitive(S(1067), assignint, intbase + tracingparagraphscode);
    primitive(S(1068), assignint, intbase + tracingpagescode);
    primitive(S(1069), assignint, intbase + tracingoutputcode);
    primitive(S(1070), assignint, intbase + tracinglostcharscode);
    primitive(S(1071), assignint, intbase + tracingcommandscode);
    primitive(S(1072), assignint, intbase + tracingrestorescode);
    primitive(S(1073), assignint, intbase + uchyphcode);
    primitive(S(1074), assignint, intbase + outputpenaltycode);
    primitive(S(1075), assignint, intbase + maxdeadcyclescode);
    primitive(S(1076), assignint, intbase + hangaftercode);
    primitive(S(1077), assignint, intbase + floatingpenaltycode);
    primitive(S(1078), assignint, intbase + globaldefscode);
    primitive(S(333), assignint, intbase + curfamcode);
    primitive(S(1079), assignint, intbase + ESCAPE_CHARcode);
    primitive(S(1080), assignint, intbase + defaulthyphencharcode);
    primitive(S(1081), assignint, intbase + defaultskewcharcode);
    primitive(S(1082), assignint, intbase + endlinecharcode);
    primitive(S(1083), assignint, intbase + newlinecharcode);
    primitive(S(1084), assignint, intbase + languagecode);
    primitive(S(1085), assignint, intbase + lefthyphenmincode);
    primitive(S(1086), assignint, intbase + righthyphenmincode);
    primitive(S(1087), assignint, intbase + holdinginsertscode);
    primitive(S(1088), assignint, intbase + errorcontextlinescode);
    /*:238*/
    /*248:*/
    primitive(S(1089), assigndimen, dimenbase);
    primitive(S(1090), assigndimen, dimenbase + mathsurroundcode);
    primitive(S(1091), assigndimen, dimenbase + lineskiplimitcode);
    primitive(S(1092), assigndimen, dimenbase + hsizecode);
    primitive(S(1093), assigndimen, dimenbase + vsizecode);
    primitive(S(1094), assigndimen, dimenbase + maxdepthcode);
    primitive(S(1095), assigndimen, dimenbase + splitmaxdepthcode);
    primitive(S(1096), assigndimen, dimenbase + boxmaxdepthcode);
    primitive(S(1097), assigndimen, dimenbase + hfuzzcode);
    primitive(S(1098), assigndimen, dimenbase + vfuzzcode);
    primitive(S(1099), assigndimen, dimenbase + delimitershortfallcode);
    primitive(S(1100), assigndimen, dimenbase + nulldelimiterspacecode);
    primitive(S(1101), assigndimen, dimenbase + scriptspacecode);
    primitive(S(1102), assigndimen, dimenbase + predisplaysizecode);
    primitive(S(1103), assigndimen, dimenbase + displaywidthcode);
    primitive(S(1104), assigndimen, dimenbase + displayindentcode);
    primitive(S(1105), assigndimen, dimenbase + overfullrulecode);
    primitive(S(1106), assigndimen, dimenbase + hangindentcode);
    primitive(S(1107), assigndimen, dimenbase + hoffsetcode);
    primitive(S(1108), assigndimen, dimenbase + voffsetcode);
    primitive(S(1109), assigndimen, dimenbase + emergencystretchcode);
    /*:248*/
    /*265:*/
    primitive(' ', exspace, 0);
    primitive('/', italcorr, 0);
    primitive(S(417), accent, 0);
    primitive(S(1110), advance, 0);
    primitive(S(1111), afterassignment, 0);
    primitive(S(1112), aftergroup, 0);
    primitive(S(1113), begingroup, 0);
    primitive(S(1114), charnum, 0);
    primitive(S(262), csname, 0);
    primitive(S(1115), delimnum, 0);
    primitive(S(1116), divide, 0);
    primitive(S(263), endcsname, 0);
    primitive(S(836), endgroup, 0);
    text(frozenendgroup) = S(836);
    eqtb[frozenendgroup - activebase] = eqtb[curval - activebase];
    primitive(S(1117), expandafter, 0);
    primitive(S(1118), deffont, 0);
    primitive(S(1119), assignfontdimen, 0);
    primitive(S(724), halign, 0);
    primitive(S(863), hrule, 0);
    primitive(S(1120), ignorespaces, 0);
    primitive(S(374), insert_, 0);
    primitive(S(402), mark_, 0);
    primitive(S(913), mathaccent, 0);
    primitive(S(1121), mathcharnum, 0);
    primitive(S(404), mathchoice, 0);
    primitive(S(1122), multiply, 0);
    primitive(S(897), noalign, 0);
    primitive(S(1123), noboundary, 0);
    primitive(S(1124), noexpand, 0);
    primitive(S(388), nonscript, 0);
    primitive(S(900), omit, 0);
    primitive(S(462), setshape, 0);
    primitive(S(761), breakpenalty, 0);
    primitive(S(948), setprevgraf, 0);
    primitive(S(416), radical, 0);
    primitive(S(656), readtocs, 0);
    primitive(S(1125), relax, 256);
    text(frozenrelax) = S(1125);
    eqtb[frozenrelax - activebase] = eqtb[curval - activebase];
    primitive(S(970), setbox, 0);
    primitive(S(604), the, 0);
    primitive(S(463), toksregister, 0);
    primitive(S(403), vadjust, 0);
    primitive(S(1126), valign, 0);
    primitive(S(415), vcenter, 0);
    primitive(S(1127), vrule, 0); /*:265*/
    /*334:*/
    primitive(S(760), parend, 256);
    parloc = curval;
    partoken = cstokenflag + parloc; /*:334*/
    /*376:*/
    primitive(S(1128), input, 0);
    primitive(S(1129), input, 1); /*:376*/
    /*384:*/
    primitive(S(1130), topbotmark, topmarkcode);
    primitive(S(1131), topbotmark, firstmarkcode);
    primitive(S(1132), topbotmark, botmarkcode);
    primitive(S(1133), topbotmark, splitfirstmarkcode);
    primitive(S(1134), topbotmark, splitbotmarkcode); /*:384*/
    /*411:*/
    primitive(S(472), register_, intval);
    primitive(S(474), register_, dimenval);
    primitive(S(460), register_, glueval);
    primitive(S(461), register_, muval); /*:411*/
    /*416:*/
    primitive(S(1135), setaux, hmode);
    primitive(S(1136), setaux, vmode);
    primitive(S(1137), setpageint, 0);
    primitive(S(1138), setpageint, 1);
    primitive(S(1139), setboxdimen, widthoffset);
    primitive(S(1140), setboxdimen, heightoffset);
    primitive(S(1141), setboxdimen, depthoffset);
    primitive(S(1142), lastitem, intval);
    primitive(S(1143), lastitem, dimenval);
    primitive(S(1144), lastitem, glueval);
    primitive(S(1145), lastitem, inputlinenocode);
    primitive(S(1146), lastitem, badnesscode); /*:416*/
    /*468:*/
    primitive(S(1147), convert, numbercode);
    primitive(S(1148), convert, romannumeralcode);
    primitive(S(1149), convert, stringcode);
    primitive(S(1150), convert, meaningcode);
    primitive(S(1151), convert, fontnamecode);
    primitive(S(1152), convert, jobnamecode); /*:468*/
    /*487:*/
    primitive(S(658), iftest, ifcharcode);
    primitive(S(1153), iftest, ifcatcode);
    primitive(S(1154), iftest, ifintcode);
    primitive(S(1155), iftest, ifdimcode);
    primitive(S(1156), iftest, ifoddcode);
    primitive(S(1157), iftest, ifvmodecode);
    primitive(S(1158), iftest, ifhmodecode);
    primitive(S(1159), iftest, ifmmodecode);
    primitive(S(1160), iftest, ifinnercode);
    primitive(S(1161), iftest, ifvoidcode);
    primitive(S(1162), iftest, ifhboxcode);
    primitive(S(1163), iftest, ifvboxcode);
    primitive(S(1164), iftest, ifxcode);
    primitive(S(1165), iftest, ifeofcode);
    primitive(S(1166), iftest, iftruecode);
    primitive(S(1167), iftest, iffalsecode);
    primitive(S(1168), iftest, ifcasecode); /*:487*/
    /*491:*/
    primitive(S(1169), fiorelse, ficode);
    text(frozenfi) = S(1169);
    eqtb[frozenfi - activebase] = eqtb[curval - activebase];
    primitive(S(664), fiorelse, orcode);
    primitive(S(1170), fiorelse, elsecode); /*:491*/
    /*553:*/
    primitive(S(1171), setfont, nullfont);
    text(frozennullfont) = S(1171);
    eqtb[frozennullfont - activebase] = eqtb[curval - activebase]; /*:553*/
    /*780:*/
    primitive(S(1172), tabmark, spancode);
    primitive(S(737), carret, crcode);
    text(frozencr) = S(737);
    eqtb[frozencr - activebase] = eqtb[curval - activebase];
    primitive(S(1173), carret, crcrcode);
    text(frozenendtemplate) = S(1174);
    text(frozenendv) = S(1174);
    eqtype(frozenendv) = endv;
    equiv(frozenendv) = nulllist;
    eqlevel(frozenendv) = levelone;
    eqtb[frozenendtemplate - activebase] = eqtb[frozenendv - activebase];
    eqtype(frozenendtemplate) = endtemplate; /*:780*/
    /*983:*/
    primitive(S(1175), setpagedimen, 0);
    primitive(S(1176), setpagedimen, 1);
    primitive(S(1177), setpagedimen, 2);
    primitive(S(1178), setpagedimen, 3);
    primitive(S(1179), setpagedimen, 4);
    primitive(S(1180), setpagedimen, 5);
    primitive(S(1181), setpagedimen, 6);
    primitive(S(1182), setpagedimen, 7); /*:983*/
    /*1052:*/
    primitive(S(1183), stop, 0);
    primitive(S(1184), stop, 1); /*:1052*/
    /*1058:*/
    primitive(S(1185), hskip, skipcode);
    primitive(S(1186), hskip, filcode);
    primitive(S(1187), hskip, fillcode);
    primitive(S(1188), hskip, sscode);
    primitive(S(1189), hskip, filnegcode);
    primitive(S(1190), vskip, skipcode);
    primitive(S(1191), vskip, filcode);
    primitive(S(1192), vskip, fillcode);
    primitive(S(1193), vskip, sscode);
    primitive(S(1194), vskip, filnegcode);
    primitive(S(389), mskip, mskipcode);
    primitive(S(391), kern, explicit);
    primitive(S(393), mkern, muglue); /*:1058*/
    /*1071:*/
    primitive(S(1195), hmove, 1);
    primitive(S(1196), hmove, 0);
    primitive(S(1197), vmove, 1);
    primitive(S(1198), vmove, 0);
    primitive(S(464), makebox, boxcode);
    primitive(S(1199), makebox, copycode);
    primitive(S(1200), makebox, lastboxcode);
    primitive(S(797), makebox, vsplitcode);
    primitive(S(1201), makebox, vtopcode);
    primitive(S(799), makebox, vtopcode + vmode);
    primitive(S(1202), makebox, vtopcode + hmode);
    primitive(S(1203), leadership, aleaders - 1);
    primitive(S(1204), leadership, aleaders);
    primitive(S(1205), leadership, cleaders);
    primitive(S(1206), leadership, xleaders); /*:1071*/
    /*1088:*/
    primitive(S(1207), startpar, 1);
    primitive(S(1208), startpar, 0); /*:1088*/
    /*1107:*/
    primitive(S(1209), removeitem, penaltynode);
    primitive(S(1210), removeitem, kernnode);
    primitive(S(1211), removeitem, gluenode);
    primitive(S(1212), unhbox, boxcode);
    primitive(S(1213), unhbox, copycode);
    primitive(S(1214), unvbox, boxcode);
    primitive(S(1215), unvbox, copycode); /*:1107*/
    /*1114:*/
    primitive('-', discretionary, 1);
    primitive(S(400), discretionary, 0); /*:1114*/
    /*1141:*/
    primitive(S(1216), eqno, 0);
    primitive(S(1217), eqno, 1); /*:1141*/
    /*1156:*/
    primitive(S(405), mathcomp, ordnoad);
    primitive(S(406), mathcomp, opnoad);
    primitive(S(407), mathcomp, binnoad);
    primitive(S(408), mathcomp, relnoad);
    primitive(S(409), mathcomp, opennoad);
    primitive(S(410), mathcomp, closenoad);
    primitive(S(411), mathcomp, punctnoad);
    primitive(S(412), mathcomp, innernoad);
    primitive(S(414), mathcomp, undernoad);
    primitive(S(413), mathcomp, overnoad);
    primitive(S(1218), limitswitch, normal);
    primitive(S(420), limitswitch, limits);
    primitive(S(421), limitswitch, nolimits); /*:1156*/
    /*1169:*/
    primitive(S(336), mathstyle, displaystyle);
    primitive(S(337), mathstyle, textstyle);
    primitive(S(338), mathstyle, scriptstyle);
    primitive(S(339), mathstyle, scriptscriptstyle); /*:1169*/
    /*1178:*/
    primitive(S(1219), above, abovecode);
    primitive(S(1220), above, overcode);
    primitive(S(1221), above, atopcode);
    primitive(S(1222), above, delimitedcode);
    primitive(S(1223), above, delimitedcode + overcode);
    primitive(S(1224), above, delimitedcode + atopcode); /*:1178*/
    /*1188:*/
    primitive(S(418), leftright, leftnoad);
    primitive(S(419), leftright, rightnoad);
    text(frozenright) = S(419);
    eqtb[frozenright - activebase] = eqtb[curval - activebase]; /*:1188*/
    /*1208:*/
    primitive(S(959), prefix, 1);
    primitive(S(961), prefix, 2);
    primitive(S(1225), prefix, 4);
    primitive(S(1226), def, 0);
    primitive(S(1227), def, 1);
    primitive(S(1228), def, 2);
    primitive(S(1229), def, 3);
    /*:1208*/
    /*1219:*/
    primitive(S(1230), let, normal);
    primitive(S(1231), let, normal + 1); /*:1219*/
    /*1222:*/
    primitive(S(1232), shorthanddef, chardefcode);
    primitive(S(1233), shorthanddef, mathchardefcode);
    primitive(S(1234), shorthanddef, countdefcode);
    primitive(S(1235), shorthanddef, dimendefcode);
    primitive(S(1236), shorthanddef, skipdefcode);
    primitive(S(1237), shorthanddef, muskipdefcode);
    primitive(S(1238), shorthanddef, toksdefcode); /*:1222*/
    /*1230:*/
    primitive(S(467), defcode, catcodebase);
    primitive(S(471), defcode, mathcodebase);
    primitive(S(468), defcode, lccodebase);
    primitive(S(469), defcode, uccodebase);
    primitive(S(470), defcode, sfcodebase);
    primitive(S(473), defcode, delcodebase);
    primitive(S(266), deffamily, mathfontbase);
    primitive(S(267), deffamily, mathfontbase + SCRIPT_SIZE);
    primitive(S(268), deffamily, mathfontbase + SCRIPT_SCRIPT_SIZE);
    /*:1230*/
    /*1250:*/
    primitive(S(787), hyphdata, 0);
    primitive(S(774), hyphdata, 1); /*:1250*/
    /*1254:*/
    primitive(S(1239), assignfontint, 0);
    primitive(S(1240), assignfontint, 1); /*:1254*/
    /*1262:*/
    primitive(S(281), setinteraction, batchmode);
    primitive(S(282), setinteraction, nonstopmode);
    primitive(S(283), setinteraction, scrollmode);
    primitive(S(1241), setinteraction, errorstopmode); /*:1262*/
    /*1272:*/
    primitive(S(1242), instream, 1);
    primitive(S(1243), instream, 0); /*:1272*/
    /*1277:*/
    primitive(S(1244), message, 0);
    primitive(S(1245), message, 1); /*:1277*/
    /*1286:*/
    primitive(S(1246), caseshift, lccodebase);
    primitive(S(1247), caseshift, uccodebase); /*:1286*/
    /*1291:*/
    primitive(S(1248), xray, showcode);
    primitive(S(1249), xray, showboxcode);
    primitive(S(1250), xray, showthecode);
    primitive(S(1251), xray, showlists); /*:1291*/
    /*1344:*/
    primitive(S(378), extension, opennode);
    primitive(S(379), extension, writenode);
    writeloc = curval;
    primitive(S(380), extension, closenode);
    primitive(S(381), extension, specialnode);
    primitive(S(1252), extension, immediatecode);
    primitive(S(382), extension, setlanguagecode); /*:1344*/
} // #1336: initprim
#endif // #1336: tt_INIT

#ifdef tt_DEBUG
// 交互式 debug 环境
/// p470#1338
Static void debughelp(void) {
    long k, l, m, n;

    while (true) {
        printnl(S(1253)); // "debug # (−1 to exit):"
        fflush(stdout);

        fscanf(stdin, " %ld", &m);
        if (m < 0) {
            return;
        }
        if (m == 0) {
            goto _Lbreakpoint_;
        _Lbreakpoint_:
            m = 0;
            continue;
        }

        fscanf(stdin, " %ld", &n);
        switch (m) {
            /// #1339
            // display mem[n] in all forms
            case 1: printword(mem[n - memmin]); break;
            case 2: print_int(info(n)); break;
            case 3: print_int(link(n)); break;
            case 4: printword(eqtb[n - activebase]); break;
            case 5: printword(fontinfo[n]); break;
            case 6: printword(savestack[n]); break;
            // show a box, abbreviated by show box depth and show box breadth
            case 7: showbox(n); break;
            case 8: {
                breadthmax = 10000;
                depththreshold = str_adjust_to_room(POOL_SIZE) - 10;
                shownodelist(n);
                break;
            }
            case 9: showtokenlist(n, 0, 1000); break;
            case 10: slow_print(n); break;
            // check wellformedness; print new busy locations if n > 0
            case 11: checkmem(n > 0); break;
            // look for pointers to n
            case 12: searchmem(n); break;
            case 13: {
                fscanf(stdin, " %ld", &l);
                printcmdchr(n, l);
                break;
            }
            case 14: {
                for (k = 0; k <= n; k++)
                    print(buffer[k]);
                break;
            }
            case 15: {
                fontinshortdisplay = nullfont;
                shortdisplay(n);
                break;
            }
            case 16: panicking = !panicking; break;

            default:
                print('?');
                break;
        } // switch (m)
    } // while (true)
} // #1338: debughelp
#endif // #1338: tt_DEBUG

// 检查常量范围是否正确。
// 有误则返回错误代码
// Check the “constant” values for consistency
// 14, 111, 290, 522, 1249, used in 1332
int check_constant(void) {
    /// bad: 13, 14, 111, 290, 522, 1249, 1332
    int bad = 0;

    /// #14
    if (
        halfERROR_LINE < 30 ||
        halfERROR_LINE > (ERROR_LINE - 15)
    ) bad = 1;
    if (MAX_PRINT_LINE < 60) bad = 2;
    if ((dvibufsize & 8) != 0) bad = 3;
    if ((membot + 1100) > memtop) bad = 4;
    if (hashprime > hashsize) bad = 5;
    if (maxinopen >= 128) bad = 6;
    if (memtop < (256 + 11)) bad = 7;

    /// #111
    #ifdef tt_INIT
        if (memmin != membot || memmax != memtop) bad = 10;
    #endif // #111: tt_INIT
    if (memmin > membot || memmax < memtop) bad = 10;
    if (minquarterword > 0 || maxquarterword < 127) bad = 11;
    if (maxhalfword < 32767) bad = 12;
    if (minquarterword < 0 || maxquarterword > maxhalfword) bad = 13;
    if (
        memmin < 0 ||
        memmax >= maxhalfword ||
        (membot - memmin) > (maxhalfword + 1)
    ) bad = 14;
    if (0 < minquarterword || fontmax > maxquarterword) bad = 15;
    if (fontmax > 256+0) bad = 16;
    if (savesize > maxhalfword || MAX_STRINGS > maxhalfword) bad = 17;
    if (bufsize > maxhalfword) bad = 18;
    if (maxquarterword - minquarterword < 255) bad = 19;

    /// #290
    if ((cstokenflag + undefinedcontrolsequence) > maxhalfword) bad = 21;
    /// #522
    if (formatdefaultlength > filenamesize) bad = 31;
    /// 1249
    if ((maxhalfword * 2) < (memtop - memmin)) bad = 41;

    return bad;
}

/// 入口函数
/// p466#1332: TeX starts and ends here.
int main(int argc, char* argv[]) {
    long ready_already = 0; /* 1331, 1332 */
    int bad = 0;

    PASCAL_MAIN(argc, argv);
    if (setjmp(_JLendofTEX)) goto _L_end_of_TEX;
    if (setjmp(_JLfinalend)) goto _L_final_end;

    history = FATAL_ERROR_STOP;
    if (ready_already == 314159L) goto _L_start_of_TEX;

    // 常量范围检查
    bad = check_constant();
    if (bad > 0) {
        fprintf(
            stdout,
            "Ouch---my internal constants have been clobbered!---case %d\n",
            bad);
        goto _L_final_end;
    }

    initialize();
#ifdef tt_INIT
    if (!get_strings_started()) goto _L_final_end;
    initprim();
    str_set_init_ptrs();
    fixdateandtime(&tex_time, &day, &month, &year);
#endif // #1332: tt_INIT
    ready_already = 314159L;

_L_start_of_TEX: /*55:*/
    selector = TERM_ONLY;
    tally = 0;
    term_offset = 0;
    file_offset = 0;
    /*:55*/
    /*61:*/
    fprintf(stdout, "%s", banner);
    if (formatident == 0)
        fprintf(stdout, " (no format preloaded)\n");
    else {
        slow_print(formatident);
        println();
    }
    fflush(stdout); /*:61*/
    /*528:*/
    jobname = 0;
    nameinprogress = false;
    logopened = false; /*:528*/
    /*533:*/
    outputfilename = 0; /*:533*/
    /*1337:*/
    /*331:*/
    inputptr = 0;
    maxinstack = 0;
    inopen = 0;
    openparens = 0;
    maxbufstack = 0;
    paramptr = 0;
    maxparamstack = 0;
    first = bufsize;
    do {
        buffer[first] = 0;
        first--;
    } while (first != 0);
    scannerstatus = normal;
    warningindex = 0;
    first = 1;
    state = newline;
    start = 1;
    iindex = 0;
    line = 0;
    name = 0;
    forceeof = false;
    alignstate = 1000000L;
    if (!initterminal()) goto _L_final_end;
    limit = last;
    first = last + 1; /*:331*/
    if (need_to_load_format /* (formatident == 0) | (buffer[loc] == '&') */) {
        if (formatident != 0) initialize();
        if (!openfmtfile()) goto _L_final_end;
        if (!loadfmtfile()) {
            wclose(&fmtfile);
            goto _L_final_end;
        }
        wclose(&fmtfile);
        while ((loc < limit) & (buffer[loc] == ' '))
            loc++;
    }
    if (endlinecharinactive) {
        limit--;
    } else {
        buffer[limit] = endlinechar;
    }
    fixdateandtime(&tex_time, &day, &month, &year); /*765:*/
    /*75:*/
    if (interaction == batchmode)
        selector = NO_PRINT;
    else {
        selector = TERM_ONLY; /*:75*/
    }
    if ((loc < limit) & (catcode(buffer[loc]) != escape)) /*:1337*/
        startinput();
    history = SPOTLESS;
    maincontrol();
    finalcleanup();

_L_end_of_TEX:
    closefilesandterminate();
_L_final_end:
    ready_already = 0;
    exit(EXIT_SUCCESS);
} // main

/* End. */