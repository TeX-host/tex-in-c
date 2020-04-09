/* Do NOT edit - this is generated file */
#line 1 "./ppc/fonts.ppc"
#include <stdio.h>
#include "tex.h"
#include "macros.h"
#include "str.h"
#include "printout.h"
#include "funcs.h"
#include "fonts.h"
#include "texfunc.h"

/*549:*/
#undef Static
#define Static 
Static fontindex fmemptr;
Static internalfontnumber fontptr;
Static memoryword fontinfo[fontmemsize + 1];
Static fontindex fontparams[fontmax  + 1];
Static eightbits fontbc[fontmax  + 1];
Static eightbits fontec[fontmax  + 1];
Static pointer fontglue[fontmax  + 1];
Static boolean fontused[fontmax  + 1];
Static fontindex bcharlabel[fontmax  + 1];
Static long fontbchar[fontmax  + 1];
Static long fontfalsebchar[fontmax  + 1];
/*:549*/
/*550:*/
Static long ligkernbase[fontmax  + 1];
Static long extenbase[fontmax  + 1];
Static long parambase[fontmax  + 1];
/*555:*/
#undef Static
#define Static static

Static long skewchar[fontmax  + 1];
Static long hyphenchar[fontmax  + 1];
Static strnumber fontname[fontmax  + 1];
Static strnumber fontarea[fontmax  + 1];
Static scaled fontsize[fontmax  + 1];
Static scaled fontdsize[fontmax  + 1];
Static fourquarters fontcheck[fontmax  + 1];


Static long charbase[fontmax + 1];
Static long widthbase[fontmax + 1];
Static long heightbase[fontmax + 1];
Static long depthbase[fontmax + 1];
Static long italicbase[fontmax + 1];
Static long kernbase[fontmax + 1];

long get_skewchar(internalfontnumber x)
{
        return skewchar[x];
}

extern void set_skewchar(internalfontnumber x, long c)
{
	skewchar[x] = c;
}


long get_hyphenchar(internalfontnumber x)
{
	return hyphenchar[x];
}

extern void set_hyphenchar(internalfontnumber x, long c)
{
	hyphenchar[x] = c;
}

scaled get_fontsize(internalfontnumber x)
{
	return fontsize[x];
}

scaled get_fontdsize(internalfontnumber x)
{
        return fontdsize[x];
}

strnumber get_fontname(internalfontnumber x)
{
	return fontname[x];
}

fourquarters get_fontcheck(internalfontnumber x)
{
	return fontcheck[x];	
}

fourquarters charinfo(internalfontnumber f, eightbits p)
{
	return fontinfo[charbase[f]+(p)].qqqq;
}

scaled charwidth(internalfontnumber x, fourquarters y) 
{
	return (fontinfo[widthbase[x]+(y).b0].sc);
}

scaled charitalic(internalfontnumber x, fourquarters y)
{
        return (fontinfo[italicbase[x]+((y).b2/4)].sc);
}

scaled charheight(internalfontnumber x, int y)
{
        return (fontinfo[heightbase[x]+((y)/16)].sc);
}

scaled chardepth(internalfontnumber x, int y)
{
        return (fontinfo[depthbase[x]+((y)%16)].sc);
}

scaled charkern(internalfontnumber x, fourquarters y)
{
        return (fontinfo[kernbase[x]+256*opbyte(y)+rembyte(y)].sc);
}

void fonts_init(void)
{
    long k;
  /*551:*/
  for (k = 0; k <= fontmax; k++)   /*:551*/
    fontused[k ] = false;
  /*552:*/
  fontptr = nullfont;
  fmemptr = 7;
  fontname[nullfont ] = S(1171);
  fontarea[nullfont ] = S(385);
  hyphenchar[nullfont ] = '-';
  skewchar[nullfont ] = -1;
  bcharlabel[nullfont ] = nonaddress;
  fontbchar[nullfont ] = nonchar;
  fontfalsebchar[nullfont ] = nonchar;
  fontbc[nullfont ] = 1;
  fontec[nullfont ] = 0;
  fontsize[nullfont ] = 0;
  fontdsize[nullfont ] = 0;
  charbase[nullfont ] = 0;
  widthbase[nullfont ] = 0;
  heightbase[nullfont ] = 0;
  depthbase[nullfont ] = 0;
  italicbase[nullfont ] = 0;
  ligkernbase[nullfont ] = 0;
  kernbase[nullfont ] = 0;
  extenbase[nullfont ] = 0;
  fontglue[nullfont ] = 0;
  fontparams[nullfont ] = 7;
  parambase[nullfont ] = -1;
  for (k = 0; k <= 6; k++)  
    fontinfo[k].sc = 0;
   /*:552*/
}

void fonts_dump(FILE * fmtfile)
{
   memoryword pppfmtfile;
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
  for (k = nullfont; k <= fontptr; k++) {   /*1322:*/
    pppfmtfile.qqqq = fontcheck[k ];
    pput(pppfmtfile);
    pppfmtfile.int_ = fontsize[k ];
    pput(pppfmtfile);
    pppfmtfile.int_ = fontdsize[k ];
    pput(pppfmtfile);
    pppfmtfile.int_ = fontparams[k ];
    pput(pppfmtfile);
    pppfmtfile.int_ = hyphenchar[k ];
    pput(pppfmtfile);
    pppfmtfile.int_ = skewchar[k ];
    pput(pppfmtfile);
    pppfmtfile.int_ = fontname[k ];
    pput(pppfmtfile);
    pppfmtfile.int_ = fontarea[k ];
    pput(pppfmtfile);
    pppfmtfile.int_ = fontbc[k ];
    pput(pppfmtfile);
    pppfmtfile.int_ = fontec[k ];
    pput(pppfmtfile);
    pppfmtfile.int_ = charbase[k ];
    pput(pppfmtfile);
    pppfmtfile.int_ = widthbase[k ];
    pput(pppfmtfile);
    pppfmtfile.int_ = heightbase[k ];
    pput(pppfmtfile);
    pppfmtfile.int_ = depthbase[k ];
    pput(pppfmtfile);
    pppfmtfile.int_ = italicbase[k ];
    pput(pppfmtfile);
    pppfmtfile.int_ = ligkernbase[k ];
    pput(pppfmtfile);
    pppfmtfile.int_ = kernbase[k ];
    pput(pppfmtfile);
    pppfmtfile.int_ = extenbase[k ];
    pput(pppfmtfile);
    pppfmtfile.int_ = parambase[k ];
    pput(pppfmtfile);
    pppfmtfile.int_ = fontglue[k ];
    pput(pppfmtfile);
    pppfmtfile.int_ = bcharlabel[k ];
    pput(pppfmtfile);
    pppfmtfile.int_ = fontbchar[k ];
    pput(pppfmtfile);
    pppfmtfile.int_ = fontfalsebchar[k ];
    pput(pppfmtfile);
    printnl(S(1278));
    printesc(fontidtext(k));
    printchar('=');
    printfilename(fontname[k ], fontarea[k ],
		  S(385));
    if (fontsize[k ] != fontdsize[k ]) {
      print(S(642));
      printscaled(fontsize[k ]);
      print(S(459));
    }
  }
  /*:1322*/
  println();
  printint(fmemptr - 7);
  print(S(1279));
  printint(fontptr );
  print(S(1280));
  if (fontptr != 1)   /*:1320*/
    printchar('s');

}

int fonts_undump(FILE * fmtfile, FILE * termout)
{
  memoryword pppfmtfile;
  long k, x;
  
  /*1321:*/
  pget(pppfmtfile);
  x = pppfmtfile.int_;
  if (x < 7)
    goto _Lbadfmt_;
  if (x > fontmemsize) {
    fprintf(termout, "---! Must increase the font mem size\n");
    goto _Lbadfmt_;
  }
  fmemptr = x;
  for (k = 0; k < fmemptr; k++) {
    pget(pppfmtfile);
    fontinfo[k] = pppfmtfile;
  }
  pget(pppfmtfile);
  x = pppfmtfile.int_;
  if (x < 0 )
    goto _Lbadfmt_;
  if (x > fontmax) {
    fprintf(termout, "---! Must increase the font max\n");
    goto _Lbadfmt_;
  }
  fontptr = x;
  for (k = nullfont; k <= fontptr; k++) {   /*1323:*/
    pget(pppfmtfile);
    fontcheck[k ] = pppfmtfile.qqqq;
    pget(pppfmtfile);
    fontsize[k ] = pppfmtfile.int_;
    pget(pppfmtfile);
    fontdsize[k ] = pppfmtfile.int_;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if ((unsigned long)x > maxhalfword)
      goto _Lbadfmt_;
    fontparams[k ] = x;
    pget(pppfmtfile);
    hyphenchar[k ] = pppfmtfile.int_;
    pget(pppfmtfile);
    skewchar[k ] = pppfmtfile.int_;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (!str_valid(x))
      goto _Lbadfmt_;
    fontname[k ] = x;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (!str_valid(x))
      goto _Lbadfmt_;
    fontarea[k ] = x;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if ((unsigned long)x > 255)
      goto _Lbadfmt_;
    fontbc[k ] = x;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if ((unsigned long)x > 255)
      goto _Lbadfmt_;
    fontec[k ] = x;
    pget(pppfmtfile);
    charbase[k ] = pppfmtfile.int_;
    pget(pppfmtfile);
    widthbase[k ] = pppfmtfile.int_;
    pget(pppfmtfile);
    heightbase[k ] = pppfmtfile.int_;
    pget(pppfmtfile);
    depthbase[k ] = pppfmtfile.int_;
    pget(pppfmtfile);
    italicbase[k ] = pppfmtfile.int_;
    pget(pppfmtfile);
    ligkernbase[k ] = pppfmtfile.int_;
    pget(pppfmtfile);
    kernbase[k ] = pppfmtfile.int_;
    pget(pppfmtfile);
    extenbase[k ] = pppfmtfile.int_;
    pget(pppfmtfile);
    parambase[k ] = pppfmtfile.int_;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if ((long)x > get_lomemmax())
      goto _Lbadfmt_;
    fontglue[k ] = x;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if ((long)x >= fmemptr)
      goto _Lbadfmt_;
    bcharlabel[k ] = x;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if ((unsigned long)x > nonchar)
      goto _Lbadfmt_;
    fontbchar[k ] = x;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if ((unsigned long)x > nonchar)
      goto _Lbadfmt_;
    fontfalsebchar[k ] = x;
  }
  /*:1323*/
  return 1;
 _Lbadfmt_:
	return 0;
}

/*560:*/
internalfontnumber readfontinfo(halfword u, strnumber nom,
				       strnumber aire, long s)
{
  fontindex k;
  boolean fileopened;
  int lf, lh, bc, ec, nw, nh, nd, ni, nl, nk, ne, np;
  internalfontnumber f, g;
  int a, b, c, d;
  fourquarters qw;
  scaled sw, z;
  long bchlabel;
  short bchar;
  long alpha;
  char beta;
  fontindex FORLIM;
  FILE * tfmfile=0;
  g = nullfont;   /*562:*/
  /*563:*/
  fileopened = false;
  if (aire == S(385))
    packfilename(nom, S(1281), S(1282));
  else
    packfilename(nom, aire, S(1282));
  if (!aopenin(&tfmfile))
    goto _Lbadtfm_;
  fileopened = true;   /*:563*/
  /*565:*/
  lf = getc(tfmfile);
  if (lf > 127)
    goto _Lbadtfm_;
  lf = lf * 256 + getc(tfmfile);
  lh = getc(tfmfile);
  if (lh > 127)
    goto _Lbadtfm_;
  lh = lh * 256 + getc(tfmfile);
  bc = getc(tfmfile);
  if (bc > 127)
    goto _Lbadtfm_;
  bc = bc * 256 + getc(tfmfile);
  ec = getc(tfmfile);
  if (ec > 127)
    goto _Lbadtfm_;
  ec = ec * 256 + getc(tfmfile);
  if (bc > ec + 1 || ec > 255)
    goto _Lbadtfm_;
  if (bc > 255) {
    bc = 1;
    ec = 0;
  }
  nw = getc(tfmfile);
  if (nw > 127)
    goto _Lbadtfm_;
  nw = nw * 256 + getc(tfmfile);
  nh = getc(tfmfile);
  if (nh > 127)
    goto _Lbadtfm_;
  nh = nh * 256 + getc(tfmfile);
  nd = getc(tfmfile);
  if (nd > 127)
    goto _Lbadtfm_;
  nd = nd * 256 + getc(tfmfile);
  ni = getc(tfmfile);
  if (ni > 127)
    goto _Lbadtfm_;
  ni = ni * 256 + getc(tfmfile);
  nl = getc(tfmfile);
  if (nl > 127)
    goto _Lbadtfm_;
  nl = nl * 256 + getc(tfmfile);
  nk = getc(tfmfile);
  if (nk > 127)
    goto _Lbadtfm_;
  nk = nk * 256 + getc(tfmfile);
  ne = getc(tfmfile);
  if (ne > 127)
    goto _Lbadtfm_;
  ne = ne * 256 + getc(tfmfile);
  np = getc(tfmfile);
  if (np > 127)
    goto _Lbadtfm_;
  np = np * 256 + getc(tfmfile);
  if (lf != lh + ec - bc + nw + nh + nd + ni + nl + nk + ne + np + 7)
	/*:565*/
	  goto _Lbadtfm_;
  /*566:*/
  lf += -lh - 6;
  if (np < 7)
    lf += 7 - np;
  if (fontptr == fontmax || fmemptr + lf > fontmemsize) {   /*567:*/
    printnl(S(292));
    print(S(588));
    sprintcs(u);
    printchar('=');
    printfilename(nom, aire, S(385));
    if (s >= 0) {
      print(S(642));
      printscaled(s);
      print(S(459));
    } else if (s != -1000) {
      print(S(1283));
      printint(-s);
    }
    print(S(1284));
    help4(S(1285),S(1286),S(1287),S(1288));
    error();
    goto _Ldone;
  }
  /*:567*/
  f = fontptr + 1;
  charbase[f ] = fmemptr - bc;
  widthbase[f ] = charbase[f ] + ec + 1;
  heightbase[f ] = widthbase[f ] + nw;
  depthbase[f ] = heightbase[f ] + nh;
  italicbase[f ] = depthbase[f ] + nd;
  ligkernbase[f ] = italicbase[f ] + ni;
  kernbase[f ] = ligkernbase[f ] + nl - kernbaseoffset;
  extenbase[f ] = kernbase[f ] + kernbaseoffset + nk;
  parambase[f ] = extenbase[f ] + ne;   /*:566*/
  /*568:*/
  if (lh < 2)
    goto _Lbadtfm_;
  a = getc(tfmfile);
  qw.b0 = a;
  b = getc(tfmfile);
  qw.b1 = b;
  c = getc(tfmfile);
  qw.b2 = c;
  d = getc(tfmfile);
  qw.b3 = d;
  fontcheck[f ] = qw;
  z = getc(tfmfile);
  if (z > 127)
    goto _Lbadtfm_;
  z = z * 256 + getc(tfmfile);
  z = z * 256 + getc(tfmfile);
  z = z * 16 + getc(tfmfile) / 16;
  if (z < unity)
    goto _Lbadtfm_;
  while (lh > 2) {
    getc(tfmfile);
    getc(tfmfile);
    getc(tfmfile);
    getc(tfmfile);
    lh--;
  }
  fontdsize[f ] = z;
  if (s != -1000) {
    if (s >= 0)
      z = s;
    else
      z = xnoverd(z, -s, 1000);
  }
  fontsize[f ] = z;   /*:568*/
  /*569:*/
  FORLIM = widthbase[f ];
  for (k = fmemptr; k < FORLIM; k++) {   /*:569*/
    a = getc(tfmfile);
    qw.b0 = a;
    b = getc(tfmfile);
    qw.b1 = b;
    c = getc(tfmfile);
    qw.b2 = c;
    d = getc(tfmfile);
    qw.b3 = d;
    fontinfo[k].qqqq = qw;
    if (a >= nw || 
	b / 16 >= nh || 
	(b & 15) >= nd || 
	c / 4 >= ni)
      goto _Lbadtfm_;
    switch (c & 3) {

    case ligtag:
      if (d >= nl)
	goto _Lbadtfm_;
      break;

    case exttag:
      if (d >= ne)
	goto _Lbadtfm_;
      break;

    case listtag:   /*570:*/
      if (d < bc || d > ec)
	goto _Lbadtfm_;
      while (d < k + bc - fmemptr) {
	qw = charinfo(f, d);
	if (chartag(qw) != listtag) {
	  goto _Lnotfound;
	}
	d = rembyte(qw) - minquarterword;
      }
      if (d == k + bc - fmemptr)
	goto _Lbadtfm_;
_Lnotfound: ;
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
  alpha *= z;   /*:572*/
  FORLIM = ligkernbase[f ];
  for (k = widthbase[f ]; k < FORLIM; k++) {
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
  if (fontinfo[widthbase[f ]].sc != 0)
    goto _Lbadtfm_;
  if (fontinfo[heightbase[f ]].sc != 0)
    goto _Lbadtfm_;
  if (fontinfo[depthbase[f ]].sc != 0)
    goto _Lbadtfm_;
  if (fontinfo[italicbase[f ]].sc != 0)   /*:571*/
    goto _Lbadtfm_;
  /*573:*/
  bchlabel = 32767;
  bchar = 256;
  if (nl > 0) {
    FORLIM = kernbase[f ] + kernbaseoffset;
    for (k = ligkernbase[f ]; k < FORLIM; k++) {
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
	if (c * 256 + d >= nl)
	  goto _Lbadtfm_;
	if (a == 255) {
	  if (k == ligkernbase[f ])
	    bchar = b;
	}
      } else {
	if (b != bchar) {
	  if (b < bc || b > ec)
	    goto _Lbadtfm_;
	  qw = charinfo(f, b);
	  if (!charexists(qw))
	    goto _Lbadtfm_;
	}
	if (c < 128) {
	  if (d < bc || d > ec)
	    goto _Lbadtfm_;
	  qw = charinfo(f, d);
	  if (!charexists(qw))
	    goto _Lbadtfm_;
	} else if ((c - 128) * 256 + d >= nk)
	  goto _Lbadtfm_;
	if (a < 128) {
	  if (k - ligkernbase[f ] + a + 1 >= nl)
	    goto _Lbadtfm_;
	}
      }
    }
    if (a == 255)
      bchlabel = c * 256 + d;
  }
  FORLIM = extenbase[f ];
  for (k = kernbase[f ] + kernbaseoffset; k < FORLIM; k++) {
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
  FORLIM = parambase[f ];
  for (k = extenbase[f ]; k < FORLIM; k++) {   /*:574*/
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
      if (a < bc || a > ec)
	goto _Lbadtfm_;
      qw = charinfo(f, a);
      if (!charexists(qw))
	goto _Lbadtfm_;
    }
    if (b != 0) {
      if (b < bc || b > ec)
	goto _Lbadtfm_;
      qw = charinfo(f, b);
      if (!charexists(qw))
	goto _Lbadtfm_;
    }
    if (c != 0) {
      if (c < bc || c > ec)
	goto _Lbadtfm_;
      qw = charinfo(f, c);
      if (!charexists(qw))
	goto _Lbadtfm_;
    }
    if (d < bc || d > ec)
      goto _Lbadtfm_;
    qw = charinfo(f, d);
    if (!charexists(qw))
      goto _Lbadtfm_;
  }
  /*575:*/
  for (k = 1; k <= np; k++) {
    if (k == 1) {
      sw = getc(tfmfile);
      if (sw > 127)
	sw -= 256;
      sw = sw * 256 + getc(tfmfile);
      sw = sw * 256 + getc(tfmfile);
      fontinfo[parambase[f ]].sc = sw * 16 + getc(tfmfile) / 16;
    } else {
      a = getc(tfmfile);
      b = getc(tfmfile);
      c = getc(tfmfile);
      d = getc(tfmfile);
      sw = ((d * z / 256 + c * z) / 256 + b * z) / beta;
      if (a == 0)
	fontinfo[parambase[f ] + k - 1].sc = sw;
      else if (a == 255)
	fontinfo[parambase[f ] + k - 1].sc = sw - alpha;
      else
	goto _Lbadtfm_;
    }
  }
  if (feof(tfmfile))
    goto _Lbadtfm_;
  for (k = np; k <= 6; k++)   /*:575*/
    fontinfo[parambase[f ] + k].sc = 0;
  /*576:*/
  if (np >= 7)
    fontparams[f ] = np;
  else
    fontparams[f ] = 7;
  hyphenchar[f ] = get_defaulthyphenchar();
  skewchar[f ] = get_defaultskewchar();
  if (bchlabel < nl)
    bcharlabel[f ] = bchlabel + ligkernbase[f ];
  else
    bcharlabel[f ] = nonaddress;
  fontbchar[f ] = bchar;
  fontfalsebchar[f ] = bchar;
  if (bchar <= ec) {
    if (bchar >= bc) {
      qw = charinfo(f, bchar);
      if (charexists(qw))
	fontfalsebchar[f ] = nonchar;
    }
  }
  fontname[f ] = nom;
  fontarea[f ] = aire;
  fontbc[f ] = bc;
  fontec[f ] = ec;
  fontglue[f ] = 0;
  charbase[f ] -= minquarterword;
  widthbase[f ] -= minquarterword;
  ligkernbase[f ] -= minquarterword;
  kernbase[f ] -= minquarterword;
  extenbase[f ] -= minquarterword;
  parambase[f ]--;
  fmemptr += lf;
  fontptr = f;
  g = f;
  goto _Ldone;   /*:576*/
  /*:562*/
_Lbadtfm_:   /*561:*/
  printnl(S(292));
  print(S(588));
  sprintcs(u);
  printchar('=');
  printfilename(nom, aire, S(385));
  if (s >= 0) {
    print(S(642));
    printscaled(s);
    print(S(459));
  } else if (s != -1000) {
    print(S(1283));
    printint(-s);
  }
  if (fileopened)
    print(S(1289));
  else
    print(S(1290));
  help5(S(1291),S(1292),S(1293),S(1294),S(1295));
/*:561*/
  error();
_Ldone:
  if (fileopened)
    fclose(tfmfile);
  return g;
}

