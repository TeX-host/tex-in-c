#include <stdio.h> // FILE
#include "str.h"    // [type] StrNumber
#include "error.h"  // [func] error, print_err,
#include "lexer.h"  // curcmd
#include "fonts.h"  // [export]
    // [func] print_*,
    //  pack_file_name, error,
#include "texmath.h"
    // [macro] UNITY
    // [func] print_scaled, xn_over_d,
#include "hash.h" // [func] sprint_cs, fontidtext
#include "mem.h"  // [func] get_lo_mem_max, get_avail
#include "eqtb.h" // [func] get_defaultskewchar, get_defaulthyphenchar,
#include "print.h"
#include "io.h"  // a_open_in
#include "scan.h" // [func] skip_spaces
#include "box.h" // font, character


/** @addtogroup S539x582_P196x213
 * @{
 */
// p201#549

MemoryWord fontinfo[FONT_MEM_SIZE + 1]; ///< the big collection of font data
FontIndex fmemptr;                      ///< first unused word of font info
InternalFontNumber fontptr; ///< largest internal font number in use
Static FourQuarters fontcheck[FONT_MAX + 1]; ///< check sum
Static Scaled fontsize[FONT_MAX + 1];        ///< "at" size
Static Scaled fontdsize[FONT_MAX + 1];       ///< "design" size
FontIndex
    fontparams[FONT_MAX + 1]; ///< how many font parameters are present
Static StrNumber fontname[FONT_MAX + 1]; ///< name of the font
Static StrNumber fontarea[FONT_MAX + 1]; ///< area of the font
EightBits fontbc[FONT_MAX + 1]; ///< beginning (smallest) character code
EightBits fontec[FONT_MAX + 1]; ///< ending (largest) character code.
/// glue specification for interword space, null if not allocated.
Pointer fontglue[FONT_MAX + 1];
/// has a character from this font actually appeared in the output?
Boolean fontused[FONT_MAX + 1];
Static Integer hyphenchar[FONT_MAX + 1]; ///< current `\hyphenchar` values
Static Integer skewchar[FONT_MAX + 1];   ///< current `\skewchar` values.
/// start of lig kern program for left boundary character,
/// non address if there is none.
FontIndex bcharlabel[FONT_MAX + 1];
/// right boundary character, non char if there is none.
Integer fontbchar[FONT_MAX + 1];
/// font bchar if it doesn’t exist in the font, otherwise non char.
Integer fontfalsebchar[FONT_MAX + 1];

// #550

Static Integer charbase[FONT_MAX + 1];   ///< base addresses for char info.
Static Integer widthbase[FONT_MAX + 1];  ///< base addresses for widths.
Static Integer heightbase[FONT_MAX + 1]; ///< base addresses for heights.
Static Integer depthbase[FONT_MAX + 1];  ///< base addresses for depths.
/// base addresses for italic corrections.
Static Integer italicbase[FONT_MAX + 1];
/// base addresses for ligature/kerning programs.
Integer ligkernbase[FONT_MAX + 1];
/// base addresses for kerns.
Static Integer kernbase[FONT_MAX + 1];
/// base addresses for extensible recipes.
Integer extenbase[FONT_MAX + 1];
/// base addresses for font parameters.
Integer parambase[FONT_MAX + 1];

/*555:*/
FourQuarters nullcharacter;
/*:555*/


Integer get_skewchar(InternalFontNumber x) { return skewchar[x]; }
void set_skewchar(InternalFontNumber x, Integer c) { skewchar[x] = c; }
Integer get_hyphenchar(InternalFontNumber x) { return hyphenchar[x]; }
void set_hyphenchar(InternalFontNumber x, Integer c) { hyphenchar[x] = c; }
Scaled get_fontsize(InternalFontNumber x) { return fontsize[x]; }
Scaled get_fontdsize(InternalFontNumber x) { return fontdsize[x]; }
StrNumber get_fontname(InternalFontNumber x) { return fontname[x]; }
FourQuarters get_fontcheck(InternalFontNumber x) { return fontcheck[x]; }

FourQuarters charinfo(InternalFontNumber f, EightBits p) {
        return fontinfo[charbase[f] + (p)].qqqq;
}

Scaled charwidth(InternalFontNumber x, FourQuarters y) {
    return (fontinfo[widthbase[x] + (y).b0].sc);
}

Scaled charitalic(InternalFontNumber x, FourQuarters y) {
    return (fontinfo[italicbase[x] + ((y).b2 / 4)].sc);
}

Scaled charheight(InternalFontNumber x, Integer y) {
    return (fontinfo[heightbase[x] + ((y) / 16)].sc);
}

Scaled chardepth(InternalFontNumber x, Integer y) {
    return (fontinfo[depthbase[x] + ((y) % 16)].sc);
}

Scaled charkern(InternalFontNumber x, FourQuarters y) {
    return (fontinfo[kernbase[x] + 256 * opbyte(y) + rembyte(y)].sc);
}

/// [#551, #556]
void font_init() {
    /// [#551]
    for (int k = FONT_BASE; k <= FONT_MAX; k++) {
        fontused[k - FONT_BASE] = false;
    }

    /// [#556]
    nullcharacter.b0 = MIN_QUARTER_WORD;
    nullcharacter.b1 = MIN_QUARTER_WORD;
    nullcharacter.b2 = MIN_QUARTER_WORD;
    nullcharacter.b3 = MIN_QUARTER_WORD;
} /* font_init */


/// [#552]
/// TeX always knows at least one font, namely the null font. 
/// 
/// It has no characters, and its seven parameters are all equal to zero.
void fonts_init_once(void) {
    fontptr = NULL_FONT;
    fmemptr = 7;

    fontname[NULL_FONT] = S(1171); // "nullfont"
    fontarea[NULL_FONT] = S(385);  // ""
    hyphenchar[NULL_FONT] = '-';

    skewchar[NULL_FONT] = -1;

    bcharlabel[NULL_FONT] = nonaddress;
    fontbchar[NULL_FONT] = NON_CHAR;
    fontfalsebchar[NULL_FONT] = NON_CHAR;

    fontbc[NULL_FONT] = 1;
    fontec[NULL_FONT] = 0;
    fontsize[NULL_FONT] = 0;
    fontdsize[NULL_FONT] = 0;
    charbase[NULL_FONT] = 0;
    widthbase[NULL_FONT] = 0;
    heightbase[NULL_FONT] = 0;
    depthbase[NULL_FONT] = 0;
    italicbase[NULL_FONT] = 0;
    ligkernbase[NULL_FONT] = 0;
    kernbase[NULL_FONT] = 0;
    extenbase[NULL_FONT] = 0;
    fontglue[NULL_FONT] = 0;

    fontparams[NULL_FONT] = 7;
    parambase[NULL_FONT] = -1;
    for (int k = 0; k <= 6; k++) {
        fontinfo[k].sc = 0;
    }
} /* #552: fonts_init_once */

/// [#1320] Dump the font information.
void fonts_dump() {
    Integer k;

    dump_int(fmemptr);
    for (k = 0; k < fmemptr; k++) {
        dump_wd(fontinfo[k]);
    }
    dump_int(fontptr);

    for (k = NULL_FONT; k <= fontptr; k++) {
        /** [#1322] Dump the array info for internal font number k. */
        dump_qqqq(fontcheck[k]);
        dump_int(fontsize[k]);
        dump_int(fontdsize[k]);
    
        dump_int(fontparams[k]);
        dump_int(hyphenchar[k]);
        dump_int(skewchar[k]);

        dump_int(fontname[k]);
        dump_int(fontarea[k]);
        dump_int(fontbc[k]);
        dump_int(fontec[k]);

        dump_int(charbase[k]);
        dump_int(widthbase[k]);
        dump_int(heightbase[k]);
        dump_int(depthbase[k]);
        dump_int(italicbase[k]);
        dump_int(ligkernbase[k]);
        dump_int(kernbase[k]);
        dump_int(extenbase[k]);

        dump_int(parambase[k]);
        dump_int(fontglue[k]);
        dump_int(bcharlabel[k]);
        dump_int(fontbchar[k]);
        dump_int(fontfalsebchar[k]);

    #ifndef USE_REAL_STR
        printnl(S(1278)); // "\\font"
    #else
        printnl_str("\\font");
    #endif // USE_REAL_STR

        print_esc(fontidtext(k));
        print_char('=');
        print_file_name(fontname[k], fontarea[k], S(385));
        if (fontsize[k] != fontdsize[k]) {
            print(S(642));
            print_scaled(fontsize[k]);
            print(S(459));
        }
    }

    println();
    print_int(fmemptr - 7);
    print(S(1279));
    print_int(fontptr);
    print(S(1280));
    if (fontptr != 1) {
        print_char('s');
    }
} /* [#1320] fonts_dump */

/** [#1321] Undump the font information.
 *
 */
int fonts_undump() {
    Integer k, x;

    x = undump_int();
    if (x < 7) goto _LN_badfmt;
    if (x > FONT_MEM_SIZE) {
        fprintf(TERM_OUT, "---! Must increase the font mem size\n");
        goto _LN_badfmt;
    }
    fmemptr = x;

    /// undump fontinfo[]
    for (k = 0; k < fmemptr; k++) {
        fontinfo[k] = undump_wd();
    }

    x = undump_int();
    if (x < 0) goto _LN_badfmt;
    if (x > FONT_MAX) {
        fprintf(TERM_OUT, "---! Must increase the font max\n");
        goto _LN_badfmt;
    }
    fontptr = x;

    for (k = NULL_FONT; k <= fontptr; k++) {
        /** [#1323] Undump the array info for internal font number k. */
        fontcheck[k] = undump_qqqq();
        fontsize[k] = undump_int();
        fontdsize[k] = undump_int();
    
        x = undump_int();
        if ((unsigned long)x > MAX_HALF_WORD) goto _LN_badfmt;
        fontparams[k] = x;
        hyphenchar[k] = undump_int();
        skewchar[k] = undump_int();

        x = undump_int();
        if (!str_valid(x)) goto _LN_badfmt;
        fontname[k] = x;
        x = undump_int();
        if (!str_valid(x)) goto _LN_badfmt;
        fontarea[k] = x;
        x = undump_int();
        if ((unsigned long)x > 255) goto _LN_badfmt;
        fontbc[k] = x;
        x = undump_int();
        if ((unsigned long)x > 255) goto _LN_badfmt;
        fontec[k] = x;

        charbase[k] = undump_int();
        widthbase[k] = undump_int();
        heightbase[k] = undump_int();
        depthbase[k] = undump_int();
        italicbase[k] = undump_int();
        ligkernbase[k] = undump_int();
        kernbase[k] = undump_int();
        extenbase[k] = undump_int();

        parambase[k] = undump_int();
        x = undump_int();
        if (x > (Integer)get_lo_mem_max()) goto _LN_badfmt;
        fontglue[k] = x;
        x = undump_int();
        if (x >= (Integer)fmemptr) goto _LN_badfmt;
        bcharlabel[k] = x;
        x = undump_int();
        if (x > (Integer)NON_CHAR) goto _LN_badfmt;
        fontbchar[k] = x;
        x = undump_int();
        if (x > (Integer)NON_CHAR) goto _LN_badfmt;
        fontfalsebchar[k] = x;
    }
    return true;

_LN_badfmt:
    return false;
} /* [#1321] fonts_undump */

/// [p205#560]: input a TFM file.
InternalFontNumber
readfontinfo(Pointer u, StrNumber nom, StrNumber aire, Scaled s) {
    FontIndex k; ///< index into font info
    Boolean fileopened; ///< was tfm file successfully opened?

    // sizes of subfiles
    int lf; // length of the entire file, in words
    int lh; // length of the header data, in words
    int bc; // smallest character code in the font
    int ec; // largest character code in the font
    int nw; // number of words in the width table
    int nh; // number of words in the height table
    int nd; // number of words in the depth table
    int ni; // number of words in the italic correction table
    int nl; // number of words in the lig/kern table
    int nk; // number of words in the kern table
    int ne; // number of words in the extensible character table
    int np; // number of font parameter words

    InternalFontNumber f; // the new font’s number
    InternalFontNumber g; // the number to return
    int a, b, c, d; // byte variables

    // accumulators
    FourQuarters qw; 
    Scaled sw;

    Scaled z;      // the design size or the “at” size
    Integer bchlabel; // left boundary start location, or infinity
    UInt16 bchar;    // right boundary character, or 256
    // auxiliary quantities used in fixed-point multiplication
    Integer alpha;
    UChar beta; // [1, 16]

    FontIndex FORLIM; // for 循环上限
    FILE* tfmfile = NULL;

    g = NULL_FONT;

    /*
        [ #562 ]
        Read and check the font data;
            `abort` if the TFM file is malformed;
        if there’s no room for this font,
            say so and goto done;
        otherwise
            incr(font ptr) and goto done

        [#563] Open tfm file for input 
        [#565] Read the TFM size fields 
        [#566] Use size fields to allocate font information 
        [#568] Read the TFM header 
        [#569] Read character data 
        [#571] Read box dimensions 
        [#573] Read ligature/kern program 
        [#574] Read extensible character recipes 
        [#575] Read font parameters 
        [#576] Make final adjustments and goto done 

    */

    /// #563: Open tfm file for input
    fileopened = false;
    if (aire == S(385)) { // ""
        //                "TeXfonts:"
        pack_file_name(nom, S(1281), S(1282)); // ".tfm"
    } else {
        pack_file_name(nom, aire, S(1282)); // ".tfm"
    }
    if (!a_open_in(&tfmfile)) goto _L_bad_TFM;
    fileopened = true;

    #define READ_SIXTEEN(var)                \
        do {                                 \
            var = getc(tfmfile);             \
            if (var > 127) goto _L_bad_TFM;  \
            var = var * 256 + getc(tfmfile); \
        } while (0)

    // #565: Read the TFM size fields
    #ifndef READ_SIXTEEN
        lf = getc(tfmfile);
        if (lf > 127) goto _L_bad_TFM;
        lf = lf * 256 + getc(tfmfile);
        lh = getc(tfmfile);
        if (lh > 127) goto _L_bad_TFM;
        lh = lh * 256 + getc(tfmfile);
        bc = getc(tfmfile);
        if (bc > 127) goto _L_bad_TFM;
        bc = bc * 256 + getc(tfmfile);
        ec = getc(tfmfile);
        if (ec > 127) goto _L_bad_TFM;
        ec = ec * 256 + getc(tfmfile);
    #else
        READ_SIXTEEN(lf);
        READ_SIXTEEN(lh);
        READ_SIXTEEN(bc);
        READ_SIXTEEN(ec);
    #endif // READ_SIXTEEN
    
    if (bc > (ec + 1) || ec > 255) goto _L_bad_TFM;
    if (bc > 255) { // bc = 256 and ec = 255
        bc = 1;
        ec = 0;
    }
    #ifndef READ_SIXTEEN
        nw = getc(tfmfile);
        if (nw > 127) goto _L_bad_TFM;
        nw = nw * 256 + getc(tfmfile);
        nh = getc(tfmfile);
        if (nh > 127) goto _L_bad_TFM;
        nh = nh * 256 + getc(tfmfile);
        nd = getc(tfmfile);
        if (nd > 127) goto _L_bad_TFM;
        nd = nd * 256 + getc(tfmfile);
        ni = getc(tfmfile);
        if (ni > 127) goto _L_bad_TFM;
        ni = ni * 256 + getc(tfmfile);
        nl = getc(tfmfile);
        if (nl > 127) goto _L_bad_TFM;
        nl = nl * 256 + getc(tfmfile);
        nk = getc(tfmfile);
        if (nk > 127) goto _L_bad_TFM;
        nk = nk * 256 + getc(tfmfile);
        ne = getc(tfmfile);
        if (ne > 127) goto _L_bad_TFM;
        ne = ne * 256 + getc(tfmfile);
        np = getc(tfmfile);
        if (np > 127) goto _L_bad_TFM;
        np = np * 256 + getc(tfmfile);
    #else
        READ_SIXTEEN(nw);
        READ_SIXTEEN(nh);
        READ_SIXTEEN(nd);
        READ_SIXTEEN(ni);

        READ_SIXTEEN(nl);
        READ_SIXTEEN(nk);
        READ_SIXTEEN(ne);
        READ_SIXTEEN(np);
    #endif // READ_SIXTEEN
    
    if (
        lf != (
            6 + lh 
            + (ec - bc + 1)
            + nw + nh + nd + ni 
            + nl + nk + ne + np 
        )
    ) goto _L_bad_TFM;
    if (nw == 0 || nh == 0 || nd == 0 || ni == 0) goto _L_bad_TFM;

    /// #566: Use size fields to allocate font information.
    // lf words should be loaded into font info
    lf += -lh - 6; 
    // at least seven parameters will appear
    if (np < 7) lf += 7 - np;
    if (fontptr == FONT_MAX || (fmemptr + lf) > FONT_MEM_SIZE) {
        // #567: Apologize for not loading the font, goto done

        // #561: `start font error message`
        // Report that the font won’t be loaded
    #ifndef USE_REAL_STR
        print_err(S(588)); // "Font "
    #else
        print_err_str("Font ");
    #endif // USE_REAL_STR

        sprint_cs(u);
        print_char('=');
        print_file_name(nom, aire, S(385)); // ""
    
    #ifndef USE_REAL_STR
        if (s >= 0) {
            print(S(642));  // " at "
            print_scaled(s);
            print(S(459));  // "pt"
        } else if (s != -1000) {
            print(S(1283)); // " scaled "
            print_int(-s);
        }
        print(S(1284)); // " not loaded: Not enough room left"
    #else
        if (s >= 0) {
            print_str(" at ");
            print_scaled(s);
            print_str("pt");
        } else if (s != -1000) {
            print_str(" scaled ");
            print_int(-s);
        }
        print_str(" not loaded: Not enough room left");
    #endif // USE_REAL_STR
        /*
         * (1285) "I´m afraid I won´t be able to make use of this font,"
         * (1286) "because my memory for character−size data is too small."
         * (1287) "If you´re really stuck, ask a wizard to enlarge me."
         * (1288) "Or maybe try `I\font<same font id>=<name of loaded font>´."
         */
        help4(S(1285), S(1286), S(1287), S(1288));
        error();
        goto _Ldone;
    } // if (fontptr == FONT_MAX || (fmemptr + lf) > FONT_MEM_SIZE)

    f = fontptr + 1;
    charbase[f] = fmemptr - bc;
    widthbase[f] = charbase[f] + ec + 1;
    heightbase[f] = widthbase[f] + nw;
    depthbase[f] = heightbase[f] + nh;
    italicbase[f] = depthbase[f] + nd;
    ligkernbase[f] = italicbase[f] + ni;
    kernbase[f] = ligkernbase[f] + nl - kernbaseoffset;
    extenbase[f] = kernbase[f] + kernbaseoffset + nk;
    parambase[f] = extenbase[f] + ne;

    /// #568: Read the TFM header
    if (lh < 2) goto _L_bad_TFM;

    // #564
    #define STORE_FOUR_QUARTERS(_qw)    \
        do {                            \
            a = _qw.b0 = getc(tfmfile); \
            b = _qw.b1 = getc(tfmfile); \
            c = _qw.b2 = getc(tfmfile); \
            d = _qw.b3 = getc(tfmfile); \
        } while (0)

    #ifndef STORE_FOUR_QUARTERS
        a = getc(tfmfile);
        qw.b0 = a;
        b = getc(tfmfile);
        qw.b1 = b;
        c = getc(tfmfile);
        qw.b2 = c;
        d = getc(tfmfile);
        qw.b3 = d;
    #else
        STORE_FOUR_QUARTERS(qw);
    #endif // STORE_FOUR_QUARTERS
    fontcheck[f] = qw;

    // this rejects a negative design size
    #ifndef READ_SIXTEEN
        z = getc(tfmfile);
        if (z > 127) goto _L_bad_TFM;
        z = z * 256 + getc(tfmfile);
    #else
        READ_SIXTEEN(z);
    #endif
    z = z * 256 + getc(tfmfile);
    z = z * 16 + (getc(tfmfile) / 16);
    if (z < UNITY) goto _L_bad_TFM;

    // ignore the rest of the header
    while (lh > 2) {
        getc(tfmfile);
        getc(tfmfile);
        getc(tfmfile);
        getc(tfmfile);
        lh--;
    }
    fontdsize[f] = z;
    if (s != -1000) {
        if (s >= 0) {
            z = s;
        } else {
            z = xn_over_d(z, -s, 1000);
        }
    }
    fontsize[f] = z;

    /// #569: Read character data
    FORLIM = widthbase[f];
    for (k = fmemptr; k < FORLIM; k++) {
        #ifndef STORE_FOUR_QUARTERS
            a = getc(tfmfile);
            qw.b0 = a;
            b = getc(tfmfile);
            qw.b1 = b;
            c = getc(tfmfile);
            qw.b2 = c;
            d = getc(tfmfile);
            qw.b3 = d;
        #else
            STORE_FOUR_QUARTERS(qw);
        #endif // STORE_FOUR_QUARTERS
        fontinfo[k].qqqq = qw;
        if (a >= nw 
            || (b / 16) >= nh 
            || (b % 16) >= nd 
            || (c / 4)  >= ni
        ) goto _L_bad_TFM;

        switch (c % 4) {
            case LIG_TAG:
                if (d >= nl) goto _L_bad_TFM;
                break;

            case EXT_TAG:
                if (d >= ne) goto _L_bad_TFM;
                break;

            case LIST_TAG: 
                // #570: Check for charlist cycle
                if (d < bc || d > ec) goto _L_bad_TFM;
                while (d < (int)(k + bc - fmemptr)) {
                    // N.B.: not qi(d), 
                    // since char base[f] hasn't been adjusted yet
                    qw = charinfo(f, d);
                    if (chartag(qw) != LIST_TAG) goto _Lnotfound;
                    // next character on the list
                    d = rembyte(qw) - MIN_QUARTER_WORD;
                }
                if (d == (k + bc - fmemptr)) goto _L_bad_TFM;
            
            case NO_TAG:
            default:
            _Lnotfound:;
                break;
        } // switch (c % 4)
    } // #569: for (k = fmemptr; k < FORLIM; k++)


    /// #571: Read box dimensions
    /// #572: Replace z by z 0 and compute α,β
    alpha = 16;
    while (z >= 8388608L) {
        z /= 2;
        alpha += alpha;
    }
    beta = 256 / alpha;
    alpha *= z;

    FORLIM = ligkernbase[f];
    for (k = widthbase[f]; k < FORLIM; k++) {
        // store scaled
        a = getc(tfmfile);
        b = getc(tfmfile);
        c = getc(tfmfile);
        d = getc(tfmfile);
        sw = (((d * z / 256) + (c * z)) / 256 + (b * z)) / beta;
        if (a == 0) {
            fontinfo[k].sc = sw;
        } else if (a == 255) {
            fontinfo[k].sc = sw - alpha;
        } else {
            goto _L_bad_TFM;
        }
    } // for (k = widthbase[f]; k < FORLIM; k++)
    if (fontinfo[widthbase[f]].sc  != 0) goto _L_bad_TFM;
    if (fontinfo[heightbase[f]].sc != 0) goto _L_bad_TFM;
    if (fontinfo[depthbase[f]].sc  != 0) goto _L_bad_TFM;
    if (fontinfo[italicbase[f]].sc != 0) goto _L_bad_TFM;

    /// #573: Read ligature/kern program
    bchlabel = 32767; // 077777
    bchar = 256;
    if (nl > 0) {
        FORLIM = kernbase[f] + kernbaseoffset;
        for (k = ligkernbase[f]; k < FORLIM; k++) {
            #ifndef STORE_FOUR_QUARTERS
                a = getc(tfmfile);
                qw.b0 = a;
                b = getc(tfmfile);
                qw.b1 = b;
                c = getc(tfmfile);
                qw.b2 = c;
                d = getc(tfmfile);
                qw.b3 = d;
            #else
                STORE_FOUR_QUARTERS(qw);
            #endif // STORE_FOUR_QUARTERS
            fontinfo[k].qqqq = qw;
        
            if (a > 128) {
                if ((c * 256 + d) >= nl) goto _L_bad_TFM;
                if (a == 255 && k == ligkernbase[f]) bchar = b;
            } else {
                if (b != bchar) {
                    // check_existence(b)
                    if (b < bc || b > ec) goto _L_bad_TFM;
                    qw = charinfo(f, b);
                    if (!charexists(qw)) goto _L_bad_TFM;
                }
                if (c < 128) {
                    // check_existence(d)
                    if (d < bc || d > ec) goto _L_bad_TFM;
                    qw = charinfo(f, d);
                    if (!charexists(qw)) goto _L_bad_TFM;
                } else if ((256 * (c - 128) + d) >= nk) {
                    goto _L_bad_TFM;
                } // if (c < 128)
                if (a < 128) {
                    if ((k - ligkernbase[f] + a + 1) >= nl) goto _L_bad_TFM;
                }
            } // if (a > 128) - else
        } // for (k = ligkernbase[f]; k < FORLIM; k++)
        if (a == 255) bchlabel = c * 256 + d;
    } // if (nl > 0)

    FORLIM = extenbase[f];
    for (k = kernbase[f] + kernbaseoffset; k < FORLIM; k++) {
        // store scaled
        a = getc(tfmfile);
        b = getc(tfmfile);
        c = getc(tfmfile);
        d = getc(tfmfile);
        sw = (((d * z / 256) + (c * z)) / 256 + (b * z)) / beta;
        if (a == 0) {
            fontinfo[k].sc = sw;
        } else if (a == 255) {
            fontinfo[k].sc = sw - alpha;
        } else {
            goto _L_bad_TFM;
        }
    }


    /// #574: Read extensible character recipes
    FORLIM = parambase[f];
    for (k = extenbase[f]; k < FORLIM; k++) { /*:574*/
        #ifndef STORE_FOUR_QUARTERS
            a = getc(tfmfile);
            qw.b0 = a;
            b = getc(tfmfile);
            qw.b1 = b;
            c = getc(tfmfile);
            qw.b2 = c;
            d = getc(tfmfile);
            qw.b3 = d;
        #else
            STORE_FOUR_QUARTERS(qw);
        #endif // STORE_FOUR_QUARTERS
        fontinfo[k].qqqq = qw;
    
        if (a != 0) {
            if (a < bc || a > ec) goto _L_bad_TFM;
            qw = charinfo(f, a);
            if (!charexists(qw)) goto _L_bad_TFM;
        }
        if (b != 0) {
            if (b < bc || b > ec) goto _L_bad_TFM;
            qw = charinfo(f, b);
            if (!charexists(qw)) goto _L_bad_TFM;
        }
        if (c != 0) {
            if (c < bc || c > ec) goto _L_bad_TFM;
            qw = charinfo(f, c);
            if (!charexists(qw)) goto _L_bad_TFM;
        }
        if (d < bc || d > ec) goto _L_bad_TFM;
        qw = charinfo(f, d);
        if (!charexists(qw)) goto _L_bad_TFM;
    } // for (k = extenbase[f]; k < FORLIM; k++)


    /// #575: Read font parameters
    for (k = 1; (int)k <= np; k++) {
        if (k == 1) {
            sw = getc(tfmfile);
            if (sw > 127) sw -= 256;
            sw = sw * 256 + getc(tfmfile);
            sw = sw * 256 + getc(tfmfile);
            fontinfo[parambase[f]].sc = sw * 16 + getc(tfmfile) / 16;
        } else {
            // store scaled
            a = getc(tfmfile);
            b = getc(tfmfile);
            c = getc(tfmfile);
            d = getc(tfmfile);
            sw = ((d * z / 256 + c * z) / 256 + b * z) / beta;
            if (a == 0) {
                fontinfo[parambase[f] + k - 1].sc = sw;
            } else if (a == 255) {
                fontinfo[parambase[f] + k - 1].sc = sw - alpha;
            } else {
                goto _L_bad_TFM;
            }
        } // if (k == 1) - else
    } // for (k = 1; k <= np; k++)
    if (feof(tfmfile)) goto _L_bad_TFM;
    for (k = np; k <= 6; k++) fontinfo[parambase[f] + k].sc = 0;


    // #576: Make final adjustments and goto done
    if (np >= 7) {
        fontparams[f] = np;
    } else {
        fontparams[f] = 7;
    }
    hyphenchar[f] = get_defaulthyphenchar();
    skewchar[f] = get_defaultskewchar();
    if (bchlabel < nl) {
        bcharlabel[f] = bchlabel + ligkernbase[f];
    } else {
        bcharlabel[f] = nonaddress;
    }
    fontbchar[f] = bchar;
    fontfalsebchar[f] = bchar;
    if (bchar >= bc && bchar <= ec) {
        qw = charinfo(f, bchar);
        if (charexists(qw)) fontfalsebchar[f] = NON_CHAR;
    }
    fontname[f] = nom;
    fontarea[f] = aire;
    fontbc[f] = bc;
    fontec[f] = ec;
    fontglue[f] = 0;
    charbase[f] -= MIN_QUARTER_WORD;
    widthbase[f] -= MIN_QUARTER_WORD;
    ligkernbase[f] -= MIN_QUARTER_WORD;
    kernbase[f] -= MIN_QUARTER_WORD;
    extenbase[f] -= MIN_QUARTER_WORD;
    parambase[f]--;
    fmemptr += lf;
    fontptr = f;
    g = f;
    goto _Ldone;

_L_bad_TFM:
    // #561: `start font error message`
    // Report that the font won’t be loaded
#ifndef USE_REAL_STR
    print_err(S(588)); // "Font "
    sprint_cs(u);
    print_char('=');
    print_file_name(nom, aire, S(385)); // ""
    if (s >= 0) {
        print(S(642));  // " at "
        print_scaled(s);
        print(S(459));  // "pt"
    } else if (s != -1000) {
        print(S(1283)); // " scaled "
        print_int(-s);
    }
    if (fileopened) {
        print(S(1289)); // " not loadable: Bad metric (TFM) file"
    } else {
        print(S(1290)); // " not loadable: Metric (TFM) file not found"
    }
#else
    print_err_str("Font ");
    sprint_cs(u);
    print_char('=');
    print_file_name(nom, aire, S(385)); // ""
    if (s >= 0) {
        print_str(" at ");
        print_scaled(s);
        print_str("pt");
    } else if (s != -1000) {
        print_str(" scaled ");
        print_int(-s);
    }
    if (fileopened) {
        print_str(" not loadable: Bad metric (TFM) file");
    } else {
        print_str(" not loadable: Metric (TFM) file not found");
    }
#endif // USE_REAL_STR

    /*
     * (1291) "I wasn´t able to read the size data for this font,"
     * (1292) "so I will ignore the font specification."
     * (1293) "[Wizards can fix TFM files using TFtoPL/PLtoTF.]"
     * (1294) "You might try inserting a different font spec;"
     * (1295) "e.g., type `I\font<same font id>=<substitute font name>´."
     */
    help5(S(1291), S(1292), S(1293), S(1294), S(1295));
    error();

_Ldone:
    if (fileopened) fclose(tfmfile);
    return g;
} // p205#560: readfontinfo

/// [#577] Declare procedures that scan font-related stuff.
void scanfontident(void) {
    InternalFontNumber f;
    HalfWord m;

    skip_spaces(); // [#406]

    if (curcmd == DEF_FONT) {
        f = cur_font;
    } else if (curcmd == SET_FONT) {
        f = curchr;
    } else if (curcmd == DEF_FAMILY) {
        m = curchr;
        scan_four_bit_int();
        f = equiv(m + cur_val);
    } else {
        print_err(S(584)); // "Missing font identifier"
        // "I was looking for a control sequence whose"
        // "current meaning has been defined by \\font."
        help2(S(585), S(586));
        backerror();
        f = NULL_FONT;
    }

    cur_val = f;
} /* [#577] scanfontident */

/// [#578] sets `cur_val` to `font_info` location.
/// implement `\\fontdimen n f`.
/// The boolean parameter writing is set true
///     if the calling program intends to change the parameter value.
void findfontdimen(Boolean writing) {
    InternalFontNumber f;
    Integer n;

    scan_int();
    n = cur_val;
    scanfontident();
    f = cur_val;
    if (n <= 0) {
        cur_val = fmemptr;
    } else {
        if (writing 
            && n <= SPACE_SHRINK_CODE 
            && n >= SPACE_CODE 
            && fontglue[f] != 0) {
            delete_glue_ref(fontglue[f]);
            fontglue[f] = 0;
        }

        if (n > (long)fontparams[f]) {
            if (f < fontptr) {
                cur_val = fmemptr;
            } else { 
                // [#580] Increase the number of parameters in the last font.
                do {
                    if (fmemptr == FONT_MEM_SIZE) {
                        // "font memory"
                        overflow(S(587), FONT_MEM_SIZE);
                    }
                    fontinfo[fmemptr].sc = 0;
                    fmemptr++;
                    fontparams[f]++;
                } while (n != fontparams[f]);
                // this equals `param base[f] + font params[f]`
                cur_val = fmemptr - 1;
            }
        } else {
            cur_val = n + parambase[f];
        }
    }

    // [#579] Issue an error message if `cur_val = fmem ptr`.
    if (cur_val != fmemptr) return;

    print_err(S(588)); // "Font "
    print_esc(fontidtext(f));
    print(S(589)); // " has only "
    print_int(fontparams[f]);
    print(S(590)); // " fontdimen parameters"
    // "To increase the number of font parameters you must"
    // "use \\fontdimen immediately after the \\font is loaded."
    help2(S(591), S(592));
    error();
} /* [#578] findfontdimen */

/// [#581]
void charwarning(InternalFontNumber f, EightBits c) {
    if (tracinglostchars <= 0) return;
    begin_diagnostic();
    printnl(S(678));
    print(c);
    print(S(679));
    slow_print(get_fontname(f));
    print_char('!');
    end_diagnostic(false);
} // [#581] charwarning

/// [#582]
HalfWord newcharacter(InternalFontNumber f, EightBits c) {
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
} // [#582] newcharacter

/** @}*/ // end group S539x582_P196x213