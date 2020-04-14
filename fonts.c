#include <stdio.h> // FILE
#include "tex.h"
    // [type] MemoryWord, EightBits, Pointer, Boolean, 
    //  Scaled FourQuarters, HalfWord
    // [macro] FONT_MEM_SIZE, FONT_MAX, nullfont, nonaddress, kernbaseoffset
#include "macros.h" // [macro] nonchar
#include "str.h"    // [type] StrNumber
#include "funcs.h"  // [func] a_open_in
#include "fonts.h"  // [export], [func] fontidtext
#include "texfunc.h"
    // [func] print_*,
    //  packfilename, sprint_cs, error, xn_over_d,
    //  get_defaultskewchar, get_defaulthyphenchar,
    //  get_lo_mem_max

// p201#549
FontIndex fmemptr;
InternalFontNumber fontptr;
MemoryWord fontinfo[FONT_MEM_SIZE + 1];
FontIndex fontparams[FONT_MAX + 1];
EightBits fontbc[FONT_MAX + 1]; // beginning (smallest) character code
EightBits fontec[FONT_MAX + 1]; // ending (largest) character code
// glue specification for interword space, null if not allocated
Pointer fontglue[FONT_MAX + 1];
// has a character from this font actually appeared in the output?
Boolean fontused[FONT_MAX + 1];
// start of lig kern program for left boundary character, 
// non address if there is none
FontIndex bcharlabel[FONT_MAX + 1];
// right boundary character, non char if there is none
Integer fontbchar[FONT_MAX + 1];
// font bchar if it doesn’t exist in the font, otherwise non char
Integer fontfalsebchar[FONT_MAX + 1];

// #550
// base addresses for ligature/kerning programs
Integer ligkernbase[FONT_MAX + 1];
// base addresses for extensible recipes
Integer extenbase[FONT_MAX + 1];
// base addresses for font parameters
Integer parambase[FONT_MAX + 1];

Static Integer skewchar[FONT_MAX + 1];   // current \skewchar values
Static Integer hyphenchar[FONT_MAX + 1]; // current \hyphenchar values
Static StrNumber fontname[FONT_MAX + 1]; // name of the font
Static StrNumber fontarea[FONT_MAX + 1]; // area of the font
Static Scaled fontsize[FONT_MAX + 1];    // "at" size
Static Scaled fontdsize[FONT_MAX + 1];   // "design" size
Static FourQuarters fontcheck[FONT_MAX + 1]; // check sum

Static long charbase[FONT_MAX + 1];   // base addresses for char info
Static long widthbase[FONT_MAX + 1];  // base addresses for widths
Static long heightbase[FONT_MAX + 1]; // base addresses for heights
Static long depthbase[FONT_MAX + 1];  // base addresses for depths
Static long italicbase[FONT_MAX + 1]; // base addresses for italic corrections
Static long kernbase[FONT_MAX + 1];   // base addresses for kerns


long        get_skewchar(InternalFontNumber x) { return skewchar[x]; }
extern void set_skewchar(InternalFontNumber x, long c) { skewchar[x] = c; }

long        get_hyphenchar(InternalFontNumber x) { return hyphenchar[x]; }
extern void set_hyphenchar(InternalFontNumber x, long c) { hyphenchar[x] = c; }

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

Scaled charheight(InternalFontNumber x, int y) {
    return (fontinfo[heightbase[x] + ((y) / 16)].sc);
}

Scaled chardepth(InternalFontNumber x, int y) {
    return (fontinfo[depthbase[x] + ((y) % 16)].sc);
}

Scaled charkern(InternalFontNumber x, FourQuarters y) {
    return (fontinfo[kernbase[x] + 256 * opbyte(y) + rembyte(y)].sc);
}

void fonts_init(void) {
    /*552:*/
    fontptr = nullfont;
    fmemptr = 7;
    fontname[nullfont] = S(1171);
    fontarea[nullfont] = S(385);
    hyphenchar[nullfont] = '-';
    skewchar[nullfont] = -1;
    bcharlabel[nullfont] = nonaddress;
    fontbchar[nullfont] = nonchar;
    fontfalsebchar[nullfont] = nonchar;
    fontbc[nullfont] = 1;
    fontec[nullfont] = 0;
    fontsize[nullfont] = 0;
    fontdsize[nullfont] = 0;
    charbase[nullfont] = 0;
    widthbase[nullfont] = 0;
    heightbase[nullfont] = 0;
    depthbase[nullfont] = 0;
    italicbase[nullfont] = 0;
    ligkernbase[nullfont] = 0;
    kernbase[nullfont] = 0;
    extenbase[nullfont] = 0;
    fontglue[nullfont] = 0;
    fontparams[nullfont] = 7;
    parambase[nullfont] = -1;
    for (int k = 0; k <= 6; k++)
        fontinfo[k].sc = 0;
    /*:552*/
}

void fonts_dump(FILE* fmtfile) {
    MemoryWord pppfmtfile;
    long k;
    /*1320:*/
    pppfmtfile.int_ = fmemptr;
    pput(pppfmtfile);
    for (k = 0; k < fmemptr; k++) {
        pppfmtfile = fontinfo[k];
        pput(pppfmtfile);
    }
    pppfmtfile.int_ = fontptr;
    pput(pppfmtfile);
    for (k = nullfont; k <= fontptr; k++) { /*1322:*/
        pppfmtfile.qqqq = fontcheck[k];
        pput(pppfmtfile);
        pppfmtfile.int_ = fontsize[k];
        pput(pppfmtfile);
        pppfmtfile.int_ = fontdsize[k];
        pput(pppfmtfile);
        pppfmtfile.int_ = fontparams[k];
        pput(pppfmtfile);
        pppfmtfile.int_ = hyphenchar[k];
        pput(pppfmtfile);
        pppfmtfile.int_ = skewchar[k];
        pput(pppfmtfile);
        pppfmtfile.int_ = fontname[k];
        pput(pppfmtfile);
        pppfmtfile.int_ = fontarea[k];
        pput(pppfmtfile);
        pppfmtfile.int_ = fontbc[k];
        pput(pppfmtfile);
        pppfmtfile.int_ = fontec[k];
        pput(pppfmtfile);
        pppfmtfile.int_ = charbase[k];
        pput(pppfmtfile);
        pppfmtfile.int_ = widthbase[k];
        pput(pppfmtfile);
        pppfmtfile.int_ = heightbase[k];
        pput(pppfmtfile);
        pppfmtfile.int_ = depthbase[k];
        pput(pppfmtfile);
        pppfmtfile.int_ = italicbase[k];
        pput(pppfmtfile);
        pppfmtfile.int_ = ligkernbase[k];
        pput(pppfmtfile);
        pppfmtfile.int_ = kernbase[k];
        pput(pppfmtfile);
        pppfmtfile.int_ = extenbase[k];
        pput(pppfmtfile);
        pppfmtfile.int_ = parambase[k];
        pput(pppfmtfile);
        pppfmtfile.int_ = fontglue[k];
        pput(pppfmtfile);
        pppfmtfile.int_ = bcharlabel[k];
        pput(pppfmtfile);
        pppfmtfile.int_ = fontbchar[k];
        pput(pppfmtfile);
        pppfmtfile.int_ = fontfalsebchar[k];
        pput(pppfmtfile);
        printnl(S(1278));
        print_esc(fontidtext(k));
        print_char('=');
        print_file_name(fontname[k], fontarea[k], S(385));
        if (fontsize[k] != fontdsize[k]) {
            print(S(642));
            print_scaled(fontsize[k]);
            print(S(459));
        }
    }
    /*:1322*/
    println();
    print_int(fmemptr - 7);
    print(S(1279));
    print_int(fontptr);
    print(S(1280));
    if (fontptr != 1) /*:1320*/
        print_char('s');
}

int fonts_undump(FILE* fmtfile, FILE* _not_use_) {
    MemoryWord pppfmtfile;
    long k, x;

    /*1321:*/
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x < 7) goto _Lbadfmt_;
    if (x > FONT_MEM_SIZE) {
        fprintf(stdout, "---! Must increase the font mem size\n");
        goto _Lbadfmt_;
    }
    fmemptr = x;
    for (k = 0; k < fmemptr; k++) {
        pget(pppfmtfile);
        fontinfo[k] = pppfmtfile;
    }
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x < 0) goto _Lbadfmt_;
    if (x > FONT_MAX) {
        fprintf(stdout, "---! Must increase the font max\n");
        goto _Lbadfmt_;
    }
    fontptr = x;
    for (k = nullfont; k <= fontptr; k++) { /*1323:*/
        pget(pppfmtfile);
        fontcheck[k] = pppfmtfile.qqqq;
        pget(pppfmtfile);
        fontsize[k] = pppfmtfile.int_;
        pget(pppfmtfile);
        fontdsize[k] = pppfmtfile.int_;
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if ((unsigned long)x > MAX_HALF_WORD) goto _Lbadfmt_;
        fontparams[k] = x;
        pget(pppfmtfile);
        hyphenchar[k] = pppfmtfile.int_;
        pget(pppfmtfile);
        skewchar[k] = pppfmtfile.int_;
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if (!str_valid(x)) goto _Lbadfmt_;
        fontname[k] = x;
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if (!str_valid(x)) goto _Lbadfmt_;
        fontarea[k] = x;
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if ((unsigned long)x > 255) goto _Lbadfmt_;
        fontbc[k] = x;
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if ((unsigned long)x > 255) goto _Lbadfmt_;
        fontec[k] = x;
        pget(pppfmtfile);
        charbase[k] = pppfmtfile.int_;
        pget(pppfmtfile);
        widthbase[k] = pppfmtfile.int_;
        pget(pppfmtfile);
        heightbase[k] = pppfmtfile.int_;
        pget(pppfmtfile);
        depthbase[k] = pppfmtfile.int_;
        pget(pppfmtfile);
        italicbase[k] = pppfmtfile.int_;
        pget(pppfmtfile);
        ligkernbase[k] = pppfmtfile.int_;
        pget(pppfmtfile);
        kernbase[k] = pppfmtfile.int_;
        pget(pppfmtfile);
        extenbase[k] = pppfmtfile.int_;
        pget(pppfmtfile);
        parambase[k] = pppfmtfile.int_;
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if ((long)x > get_lo_mem_max()) goto _Lbadfmt_;
        fontglue[k] = x;
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if ((long)x >= fmemptr) goto _Lbadfmt_;
        bcharlabel[k] = x;
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if ((unsigned long)x > nonchar) goto _Lbadfmt_;
        fontbchar[k] = x;
        pget(pppfmtfile);
        x = pppfmtfile.int_;
        if ((unsigned long)x > nonchar) goto _Lbadfmt_;
        fontfalsebchar[k] = x;
    }
    /*:1323*/
    return 1;
_Lbadfmt_:
    return 0;
}

/*560:*/
InternalFontNumber
readfontinfo(HalfWord u, StrNumber nom, StrNumber aire, long s) {
    FontIndex k;
    Boolean fileopened;
    int lf, lh, bc, ec, nw, nh, nd, ni, nl, nk, ne, np;
    InternalFontNumber f, g;
    int a, b, c, d;
    FourQuarters qw;
    Scaled sw, z;
    long bchlabel;
    short bchar;
    long alpha;
    char beta;
    FontIndex FORLIM;
    FILE* tfmfile = 0;

    g = nullfont; /*562:*/
    /*563:*/
    fileopened = false;
    if (aire == S(385))
        packfilename(nom, S(1281), S(1282));
    else
        packfilename(nom, aire, S(1282));
    if (!a_open_in(&tfmfile)) goto _Lbadtfm_;
    fileopened = true; /*:563*/
    /*565:*/
    lf = getc(tfmfile);
    if (lf > 127) goto _Lbadtfm_;
    lf = lf * 256 + getc(tfmfile);
    lh = getc(tfmfile);
    if (lh > 127) goto _Lbadtfm_;
    lh = lh * 256 + getc(tfmfile);
    bc = getc(tfmfile);
    if (bc > 127) goto _Lbadtfm_;
    bc = bc * 256 + getc(tfmfile);
    ec = getc(tfmfile);
    if (ec > 127) goto _Lbadtfm_;
    ec = ec * 256 + getc(tfmfile);
    if (bc > ec + 1 || ec > 255) goto _Lbadtfm_;
    if (bc > 255) {
        bc = 1;
        ec = 0;
    }
    nw = getc(tfmfile);
    if (nw > 127) goto _Lbadtfm_;
    nw = nw * 256 + getc(tfmfile);
    nh = getc(tfmfile);
    if (nh > 127) goto _Lbadtfm_;
    nh = nh * 256 + getc(tfmfile);
    nd = getc(tfmfile);
    if (nd > 127) goto _Lbadtfm_;
    nd = nd * 256 + getc(tfmfile);
    ni = getc(tfmfile);
    if (ni > 127) goto _Lbadtfm_;
    ni = ni * 256 + getc(tfmfile);
    nl = getc(tfmfile);
    if (nl > 127) goto _Lbadtfm_;
    nl = nl * 256 + getc(tfmfile);
    nk = getc(tfmfile);
    if (nk > 127) goto _Lbadtfm_;
    nk = nk * 256 + getc(tfmfile);
    ne = getc(tfmfile);
    if (ne > 127) goto _Lbadtfm_;
    ne = ne * 256 + getc(tfmfile);
    np = getc(tfmfile);
    if (np > 127) goto _Lbadtfm_;
    np = np * 256 + getc(tfmfile);
    if (lf != lh + ec - bc + nw + nh + nd + ni + nl + nk + ne + np + 7) /*:565*/
        goto _Lbadtfm_;
    /*566:*/
    lf += -lh - 6;
    if (np < 7) lf += 7 - np;
    if (fontptr == FONT_MAX || fmemptr + lf > FONT_MEM_SIZE) { /*567:*/
        printnl(S(292));
        print(S(588));
        sprint_cs(u);
        print_char('=');
        print_file_name(nom, aire, S(385));
        if (s >= 0) {
            print(S(642));
            print_scaled(s);
            print(S(459));
        } else if (s != -1000) {
            print(S(1283));
            print_int(-s);
        }
        print(S(1284));
        help4(S(1285), S(1286), S(1287), S(1288));
        error();
        goto _Ldone;
    }
    /*:567*/
    f = fontptr + 1;
    charbase[f] = fmemptr - bc;
    widthbase[f] = charbase[f] + ec + 1;
    heightbase[f] = widthbase[f] + nw;
    depthbase[f] = heightbase[f] + nh;
    italicbase[f] = depthbase[f] + nd;
    ligkernbase[f] = italicbase[f] + ni;
    kernbase[f] = ligkernbase[f] + nl - kernbaseoffset;
    extenbase[f] = kernbase[f] + kernbaseoffset + nk;
    parambase[f] = extenbase[f] + ne; /*:566*/
    /*568:*/
    if (lh < 2) goto _Lbadtfm_;
    a = getc(tfmfile);
    qw.b0 = a;
    b = getc(tfmfile);
    qw.b1 = b;
    c = getc(tfmfile);
    qw.b2 = c;
    d = getc(tfmfile);
    qw.b3 = d;
    fontcheck[f] = qw;
    z = getc(tfmfile);
    if (z > 127) goto _Lbadtfm_;
    z = z * 256 + getc(tfmfile);
    z = z * 256 + getc(tfmfile);
    z = z * 16 + getc(tfmfile) / 16;
    if (z < UNITY) goto _Lbadtfm_;
    while (lh > 2) {
        getc(tfmfile);
        getc(tfmfile);
        getc(tfmfile);
        getc(tfmfile);
        lh--;
    }
    fontdsize[f] = z;
    if (s != -1000) {
        if (s >= 0)
            z = s;
        else
            z = xn_over_d(z, -s, 1000);
    }
    fontsize[f] = z; /*:568*/
    /*569:*/
    FORLIM = widthbase[f];
    for (k = fmemptr; k < FORLIM; k++) { /*:569*/
        a = getc(tfmfile);
        qw.b0 = a;
        b = getc(tfmfile);
        qw.b1 = b;
        c = getc(tfmfile);
        qw.b2 = c;
        d = getc(tfmfile);
        qw.b3 = d;
        fontinfo[k].qqqq = qw;
        if (a >= nw || b / 16 >= nh || (b & 15) >= nd || c / 4 >= ni)
            goto _Lbadtfm_;
        switch (c & 3) {

            case LIG_TAG:
                if (d >= nl) goto _Lbadtfm_;
                break;

            case EXT_TAG:
                if (d >= ne) goto _Lbadtfm_;
                break;

            case LIST_TAG: /*570:*/
                if (d < bc || d > ec) goto _Lbadtfm_;
                while (d < k + bc - fmemptr) {
                    qw = charinfo(f, d);
                    if (chartag(qw) != LIST_TAG) {
                        goto _Lnotfound;
                    }
                    d = rembyte(qw) - MIN_QUARTER_WORD;
                }
                if (d == k + bc - fmemptr) goto _Lbadtfm_;
            _Lnotfound:;
                break;
                /*:570*/
        }
    }
    /*571:*/
    /*572:*/
    alpha = 16;
    while (z >= 8388608L) {
        z /= 2;
        alpha += alpha;
    }
    beta = 256 / alpha;
    alpha *= z; /*:572*/
    FORLIM = ligkernbase[f];
    for (k = widthbase[f]; k < FORLIM; k++) {
        a = getc(tfmfile);
        b = getc(tfmfile);
        c = getc(tfmfile);
        d = getc(tfmfile);
        sw = ((d * z / 256 + c * z) / 256 + b * z) / beta;
        if (a == 0)
            fontinfo[k].sc = sw;
        else if (a == 255)
            fontinfo[k].sc = sw - alpha;
        else
            goto _Lbadtfm_;
    }
    if (fontinfo[widthbase[f]].sc != 0) goto _Lbadtfm_;
    if (fontinfo[heightbase[f]].sc != 0) goto _Lbadtfm_;
    if (fontinfo[depthbase[f]].sc != 0) goto _Lbadtfm_;
    if (fontinfo[italicbase[f]].sc != 0) /*:571*/
        goto _Lbadtfm_;
    /*573:*/
    bchlabel = 32767;
    bchar = 256;
    if (nl > 0) {
        FORLIM = kernbase[f] + kernbaseoffset;
        for (k = ligkernbase[f]; k < FORLIM; k++) {
            a = getc(tfmfile);
            qw.b0 = a;
            b = getc(tfmfile);
            qw.b1 = b;
            c = getc(tfmfile);
            qw.b2 = c;
            d = getc(tfmfile);
            qw.b3 = d;
            fontinfo[k].qqqq = qw;
            if (a > 128) {
                if (c * 256 + d >= nl) goto _Lbadtfm_;
                if (a == 255) {
                    if (k == ligkernbase[f]) bchar = b;
                }
            } else {
                if (b != bchar) {
                    if (b < bc || b > ec) goto _Lbadtfm_;
                    qw = charinfo(f, b);
                    if (!charexists(qw)) goto _Lbadtfm_;
                }
                if (c < 128) {
                    if (d < bc || d > ec) goto _Lbadtfm_;
                    qw = charinfo(f, d);
                    if (!charexists(qw)) goto _Lbadtfm_;
                } else if ((c - 128) * 256 + d >= nk)
                    goto _Lbadtfm_;
                if (a < 128) {
                    if (k - ligkernbase[f] + a + 1 >= nl) goto _Lbadtfm_;
                }
            }
        }
        if (a == 255) bchlabel = c * 256 + d;
    }
    FORLIM = extenbase[f];
    for (k = kernbase[f] + kernbaseoffset; k < FORLIM; k++) {
        /*:573*/
        a = getc(tfmfile);
        b = getc(tfmfile);
        c = getc(tfmfile);
        d = getc(tfmfile);
        sw = ((d * z / 256 + c * z) / 256 + b * z) / beta;
        if (a == 0)
            fontinfo[k].sc = sw;
        else if (a == 255)
            fontinfo[k].sc = sw - alpha;
        else
            goto _Lbadtfm_;
    }
    /*574:*/
    FORLIM = parambase[f];
    for (k = extenbase[f]; k < FORLIM; k++) { /*:574*/
        a = getc(tfmfile);
        qw.b0 = a;
        b = getc(tfmfile);
        qw.b1 = b;
        c = getc(tfmfile);
        qw.b2 = c;
        d = getc(tfmfile);
        qw.b3 = d;
        fontinfo[k].qqqq = qw;
        if (a != 0) {
            if (a < bc || a > ec) goto _Lbadtfm_;
            qw = charinfo(f, a);
            if (!charexists(qw)) goto _Lbadtfm_;
        }
        if (b != 0) {
            if (b < bc || b > ec) goto _Lbadtfm_;
            qw = charinfo(f, b);
            if (!charexists(qw)) goto _Lbadtfm_;
        }
        if (c != 0) {
            if (c < bc || c > ec) goto _Lbadtfm_;
            qw = charinfo(f, c);
            if (!charexists(qw)) goto _Lbadtfm_;
        }
        if (d < bc || d > ec) goto _Lbadtfm_;
        qw = charinfo(f, d);
        if (!charexists(qw)) goto _Lbadtfm_;
    }
    /*575:*/
    for (k = 1; k <= np; k++) {
        if (k == 1) {
            sw = getc(tfmfile);
            if (sw > 127) sw -= 256;
            sw = sw * 256 + getc(tfmfile);
            sw = sw * 256 + getc(tfmfile);
            fontinfo[parambase[f]].sc = sw * 16 + getc(tfmfile) / 16;
        } else {
            a = getc(tfmfile);
            b = getc(tfmfile);
            c = getc(tfmfile);
            d = getc(tfmfile);
            sw = ((d * z / 256 + c * z) / 256 + b * z) / beta;
            if (a == 0)
                fontinfo[parambase[f] + k - 1].sc = sw;
            else if (a == 255)
                fontinfo[parambase[f] + k - 1].sc = sw - alpha;
            else
                goto _Lbadtfm_;
        }
    }
    if (feof(tfmfile)) goto _Lbadtfm_;
    for (k = np; k <= 6; k++) /*:575*/
        fontinfo[parambase[f] + k].sc = 0;
    /*576:*/
    if (np >= 7)
        fontparams[f] = np;
    else
        fontparams[f] = 7;
    hyphenchar[f] = get_defaulthyphenchar();
    skewchar[f] = get_defaultskewchar();
    if (bchlabel < nl)
        bcharlabel[f] = bchlabel + ligkernbase[f];
    else
        bcharlabel[f] = nonaddress;
    fontbchar[f] = bchar;
    fontfalsebchar[f] = bchar;
    if (bchar <= ec) {
        if (bchar >= bc) {
            qw = charinfo(f, bchar);
            if (charexists(qw)) fontfalsebchar[f] = nonchar;
        }
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
    goto _Ldone; /*:576*/
                 /*:562*/
_Lbadtfm_:       /*561:*/
    printnl(S(292));
    print(S(588));
    sprint_cs(u);
    print_char('=');
    print_file_name(nom, aire, S(385));
    if (s >= 0) {
        print(S(642));
        print_scaled(s);
        print(S(459));
    } else if (s != -1000) {
        print(S(1283));
        print_int(-s);
    }
    if (fileopened)
        print(S(1289));
    else
        print(S(1290));
    help5(S(1291), S(1292), S(1293), S(1294), S(1295));
    /*:561*/
    error();
_Ldone:
    if (fileopened) fclose(tfmfile);
    return g;
}
