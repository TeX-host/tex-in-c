#include <math.h> // floor, 
#include "global_const.h" // tt_*
#include "shipout.h"    // [export]
#include "tex.h"
#include "texmac.h"     // [macro] tracingoutput
#include "dviout.h"     // [func] dviout_helper, synchh
#include "macros.h"     // [macro] help2

#define charnodetype 0xfff
#undef BIG_CHARNODE
#define BIG_CHARNODE

#ifdef BIG_CHARNODE
#define charnodesize 2
#define font_ligchar(x) type(ligchar(x))
#define character_ligchar(x) subtype(ligchar(x))
#else
#define charnodesize 1
#define font_ligchar(x) font(ligchar(x))
#define character_ligchar(x) character(ligchar(x))
#endif // #ifdef BIG_CHARNODE

#define vlistout() vlist_out( tempptr, dvih, dviv, curh, curv, maxh, maxv, ruleht, ruledp, rulewd, curs, maxpush, lq, lr, dvif, doingleaders, mem, himemmin )
#define hlistout() hlist_out( tempptr, dvih, dviv, curh, curv, maxh, maxv, ruleht, ruledp, rulewd, curs, maxpush, lq, lr, dvif, doingleaders, mem, himemmin )

void vlist_out(
    Pointer tempptr,
    Scaled dvih,
    Scaled dviv,
    Scaled curh,
    Scaled curv,
    Scaled maxh,
    Scaled maxv,
    Scaled ruleht,
    Scaled ruledp,
    Scaled rulewd,
    Integer curs,
    Integer maxpush,
    Integer lq,
    Integer lr,
    InternalFontNumber dvif,
    Boolean doingleaders,
    MemoryWord mem[],
    Pointer himemmin
);


void hlist_out(
    Pointer tempptr,
    Scaled dvih,
    Scaled dviv,
    Scaled curh,
    Scaled curv,
    Scaled maxh,
    Scaled maxv,
    Scaled ruleht,
    Scaled ruledp,
    Scaled rulewd,
    Integer curs,
    Integer maxpush,
    Integer lq,
    Integer lr,
    InternalFontNumber dvif,
    Boolean doingleaders,
    MemoryWord mem[],
    Pointer himemmin
) {
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
        dvi_out_push();
    }
    if (curs > maxpush) maxpush = curs;
    saveloc = get_dvi_mark();
    baseline = curv;
    leftedge = curh;
    while (p != 0) { /*620:*/
    _Lreswitch:
        if (ischarnode(p)) {
            synchh();
            synchv();
            do {
                QuarterWord c;
                QuarterWord f = font(p);
                c = character(p);
                if (f != dvif) { /*621:*/
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
            case vlistnode: /*623:*/
                if (listptr(p) == 0)
                    curh += width(p);
                else { /*:623*/
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

            case whatsitnode: /*1367:*/
                outwhat(p);
                break;
                /*:1367*/

            case gluenode: /*625:*/
            {
                Pointer g = glueptr(p);
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
                if (subtype(p) >= aleaders) { /*626:*/
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
                        lx = 0; /*627:*/
                        if (subtype(p) == aleaders) {
                            saveh = curh;
                            curh =
                                leftedge +
                                leaderwd * ((curh - leftedge) / leaderwd);
                            if (curh < saveh) curh += leaderwd;
                        } else { /*:627*/
                            lq = rulewd / leaderwd;
                            lr = rulewd % leaderwd;
                            if (subtype(p) == cleaders)
                                curh += lr / 2;
                            else {
                                lx = (lr * 2 + lq + 1) / (lq * 2 + 2);
                                curh += (lr - (lq - 1) * lx) / 2;
                            }
                        }
                        while (curh + leaderwd <= edge) { /*628:*/
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
                } /*:626*/
                goto _Lmovepast_;
                break;
                /*:625*/

            case kernnode:
            case mathnode:
                curh += width(p);
                break;

            case ligaturenode: /*652:*/
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
    _Lfinrule_: /*624:*/
        if (isrunning(ruleht)) {
            ruleht = height(thisbox);
        }
        if (isrunning(ruledp)) {
            ruledp = depth(thisbox);
        }
        ruleht += ruledp;
        if (ruleht > 0 && rulewd > 0) { /*:624*/
            synchh();
            curv = baseline + ruledp;
            synchv();
            dvi_setrule(ruleht, rulewd);
            curv = baseline;
            dvih += rulewd;
        }
    _Lmovepast_:
        curh += rulewd;
    _Lnextp_:
        p = link(p);
    }
    prune_movements(saveloc);
    if (curs > 0) dvi_pop(saveloc);
    curs--;

    /*:620*/
}
/*:619*/

/*629:*/
void vlist_out(
    Pointer tempptr,
    Scaled dvih,
    Scaled dviv,
    Scaled curh,
    Scaled curv,
    Scaled maxh,
    Scaled maxv,
    Scaled ruleht,
    Scaled ruledp,
    Scaled rulewd,
    Integer curs,
    Integer maxpush,
    Integer lq,
    Integer lr,
    InternalFontNumber dvif,
    Boolean doingleaders,
    MemoryWord mem[],
    Pointer himemmin
) {
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
        dvi_out_push();
    }
    if (curs > maxpush) maxpush = curs;
    saveloc = get_dvi_mark();
    leftedge = curh;
    curv -= height(thisbox);
    topedge = curv;
    while (p != 0) { /*630:*/
        if (ischarnode(p))
            confusion(S(685));
        else /*631:*/
        {    /*:631*/
            switch (type(p)) {

                case hlistnode:
                case vlistnode: /*632:*/
                    if (listptr(p) == 0)
                        curv += height(p) + depth(p);
                    else { /*:632*/
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

                case whatsitnode: /*1366:*/
                    outwhat(p);
                    break;
                    /*:1366*/

                case gluenode: /*634:*/
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
                    if (subtype(p) >= aleaders) { /*635:*/
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
                            lx = 0; /*636:*/
                            if (subtype(p) == aleaders) {
                                savev = curv;
                                curv = topedge +
                                       leaderht * ((curv - topedge) / leaderht);
                                if (curv < savev) curv += leaderht;
                            } else { /*:636*/
                                lq = ruleht / leaderht;
                                lr = ruleht % leaderht;
                                if (subtype(p) == cleaders)
                                    curv += lr / 2;
                                else {
                                    lx = (lr * 2 + lq + 1) / (lq * 2 + 2);
                                    curv += (lr - (lq - 1) * lx) / 2;
                                }
                            }
                            while (curv + leaderht <= edge) { /*637:*/
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
                                curv =
                                    savev - height(leaderbox) + leaderht + lx;
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
        _Lfinrule_: /*633:*/
            if (isrunning(rulewd)) {
                rulewd = width(thisbox);
            }
            ruleht += ruledp;
            curv += ruleht;
            if (ruleht > 0 && rulewd > 0) {
                synchh();
                synchv();
                dvi_putrule(ruleht, rulewd);
            }
            goto _Lnextp_; /*:633*/
        _Lmovepast_:
            curv += ruleht;
        }
    _Lnextp_:
        p = link(p);
    }
    /*:630*/
    prune_movements(saveloc);
    if (curs > 0) dvi_pop(saveloc);
    curs--;
}
/*:629*/

/*638:*/
void ship_out(
    HalfWord p,
    Scaled dvih,
    Scaled dviv,
    Scaled curh,
    Scaled curv,
    Scaled maxh,
    Scaled maxv,
    Scaled ruleht,
    Scaled ruledp,
    Scaled rulewd,
    Integer lq,
    Integer lr,
    Integer curs,
    Integer varused,
    Integer dynused,
    Integer maxpush,
    Integer totalpages,
    Integer deadcycles,
    Pointer tempptr,
    Pointer himemmin,
    Pointer lomemmax,
    MemoryWord mem[],
    MemoryWord eqtb[],
    char term_offset,
    char file_offset,
    StrNumber jobname,
    StrNumber outputfilename,
    InternalFontNumber dvif,
    Boolean doingleaders,
    enum Selector selector
) {
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
        if (k < j) print_char('.');
    }
    fflush(stdout);
    if (tracingoutput > 0) { /*640:*/
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
        help2(S(688), S(689));
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
    if (width(p) + hoffset > maxh) maxh = width(p) + hoffset; /*:641*/
    /*617:*/
    dvih = 0;
    dviv = 0;
    curh = hoffset;
    dvif = nullfont;
    if (outputfilename == 0) {
        if (jobname == 0) openlogfile();
        packjobname(S(691));
        while (!dvi_openout())
            promptfilename(S(692), S(691));
        outputfilename = bmakenamestring();
    }
    if (totalpages == 0) { /*:617*/
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
            cp[k] = count(k);
        }
        dvibop(cp);
    }
    curv = height(p) + voffset;
    tempptr = p;
    if (type(p) == vlistnode)
        vlistout();
    else
        hlistout();
    dvi_out_eop();
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
} /*:638*/