#include "linebreak.h"
#include "error.h"
#include "hyphen.h"


/** @addtogroup S900x918_P334x343
 * @{
 */
/*900:*/
char hyf[65];
Boolean initlig, initlft;
/*:900*/
/*905:*/
SmallNumber hyphenpassed;
/*:905*/
/*907:*/
HalfWord curl, curr;
Boolean ligaturepresent, lfthit, rthit;
/*:907*/


/*895:*/
/*906:*/
SmallNumber reconstitute(/* SmallNumber */ int j,
                                SmallNumber n,
                                HalfWord bchar,
                                HalfWord hchar) {
    Pointer p, t;
    FourQuarters q;
    HalfWord currh, testchar;
    Scaled w;
    FontIndex k;

    hyphenpassed = 0;
    t = holdhead;
    w = 0;
    link(holdhead) = 0; /*908:*/
    curl = hu[j];
    curq = t;
    if (j == 0) {
        ligaturepresent = initlig;
        p = initlist;
        if (ligaturepresent) lfthit = initlft;
        while (p > 0) {
            appendcharnodetot(character(p));
            p = link(p);
        }
    } else if (curl < NON_CHAR) {
        appendcharnodetot(curl);
    }
    ligstack = 0; /*:908*/
    setcurr();
_Llabcontinue: /*909:*/
    if (curl == NON_CHAR) {
        k = bcharlabel[hf];
        if (k == nonaddress) goto _Ldone;
        q = fontinfo[k].qqqq;
    } else {
        q = charinfo(hf, curl);
        if (chartag(q) != LIG_TAG) {
            goto _Ldone;
        }
        k = ligkernstart(hf, q);
        q = fontinfo[k].qqqq;
        if (skipbyte(q) > stopflag) {
            k = ligkernrestart(hf, q);
            q = fontinfo[k].qqqq;
        }
    }
    if (currh < NON_CHAR)
        testchar = currh;
    else
        testchar = curr;
    while (true) {
        if (nextchar(q) == testchar) {
            if (skipbyte(q) <= stopflag) {
                if (currh < NON_CHAR) {
                    hyphenpassed = j;
                    hchar = NON_CHAR;
                    currh = NON_CHAR;
                    goto _Llabcontinue;
                } else {
                    if (hchar < NON_CHAR) {
                        if (hyf[j] & 1) {
                            hyphenpassed = j;
                            hchar = NON_CHAR;
                        }
                    }
                    if (opbyte(q) < kernflag) { /*911:*/
                        if (curl == NON_CHAR) lfthit = true;
                        if (j == n) {
                            if (ligstack == 0) rthit = true;
                        }
                        checkinterrupt();
                        switch (opbyte(q)) {

                            case MIN_QUARTER_WORD + 1:
                            case MIN_QUARTER_WORD + 5:
                                curl = rembyte(q);
                                ligaturepresent = true;
                                break;

                            case MIN_QUARTER_WORD + 2:
                            case MIN_QUARTER_WORD + 6:
                                curr = rembyte(q);
                                if (ligstack > 0)
                                    character(ligstack) = curr;
                                else {
                                    ligstack = newligitem(curr);
                                    if (j == n)
                                        bchar = NON_CHAR;
                                    else {
                                        p = get_avail();
                                        ligptr(ligstack) = p;
                                        character(p) = hu[j + 1];
                                        font(p) = hf;
                                    }
                                }
                                break;

                            case MIN_QUARTER_WORD + 3:
                                curr = rembyte(q);
                                p = ligstack;
                                ligstack = newligitem(curr);
                                link(ligstack) = p;
                                break;

                            case MIN_QUARTER_WORD + 7:
                            case MIN_QUARTER_WORD + 11:
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
                        if (opbyte(q) > MIN_QUARTER_WORD + 4) {
                            if (opbyte(q) != MIN_QUARTER_WORD + 7) goto _Ldone;
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
            if (currh == NON_CHAR)
                goto _Ldone;
            else {
                currh = NON_CHAR;
                goto _Llabcontinue;
            }
        }
        k += skipbyte(q) - MIN_QUARTER_WORD + 1;
        q = fontinfo[k].qqqq;
    }
_Ldone: /*:909*/
    /*910:*/
    wraplig(rthit);
    if (w != 0) {
        link(t) = newkern(w);
        t = link(t);
        w = 0;
    }
    if (ligstack <= 0) /*:910*/
        return j;
    curq = t;
    curl = character(ligstack);
    ligaturepresent = true;
    popligstack();
    goto _Llabcontinue;
}
/*:906*/
/** @}*/ // end group S900x918_P334x343