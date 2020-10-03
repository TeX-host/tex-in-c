#include "main_ctrl.h"

// build box and list
// build-math
void offsave(void);
// indep
void scanbox(long boxcontext);

// build math
// build box
HalfWord finmlist(HalfWord p);
void buildchoices(void);


#include "build_box.c"
#include "build_math.c"
#include "indep_process.c"


Static void appspace(void);
Static void insertdollarsign(void);
Static Boolean privileged(void);
Static Boolean itsallover(void);

/** @addtogroup S1029x1054_P383x394
 * @{
 */
/*1032:*/
InternalFontNumber mainf;
FourQuarters maini, mainj;
FontIndex maink;
Pointer mainp;
Integer mains;
HalfWord falsebchar;
Boolean cancelboundary, insdisc;
/*:1032*/

/// [p386#1034]
#define adjustspacefactor()              \
    {                                    \
        mains = sf_code(curchr);         \
        if (mains == 1000) {             \
            spacefactor = 1000;          \
        } else if (mains < 1000) {       \
            if (mains > 0) {             \
                spacefactor = mains;     \
            }                            \
        } else if (spacefactor < 1000) { \
            spacefactor = 1000;          \
        } else                           \
            spacefactor = mains;         \
    }

/// [p387#1035] the PARAMETER is either |rthit| or |false|
#define packlig(x)                                    \
    {                                                 \
        mainp = newligature(mainf, curl, link(curq)); \
        if (lfthit) {                                 \
            subtype(mainp) = 2;                       \
            lfthit = false;                           \
        }                                             \
        if ((x) && (ligstack == null)) {              \
            subtype(mainp)++;                         \
            rthit = false;                            \
        }                                             \
        link(curq) = mainp;                           \
        tail = mainp;                                 \
        ligaturepresent = false;                      \
    }

/// [p387#1035]
#define wrapup(x)                                       \
    if (curl < NON_CHAR) {                              \
        if (character(tail) == get_hyphenchar(mainf)) { \
            if (link(curq) > 0) insdisc = true;         \
        }                                               \
        if (ligaturepresent) {                          \
            packlig(x);                                 \
        }                                               \
        if (insdisc) {                                  \
            insdisc = false;                            \
            if (mode > 0) {                             \
                tailappend(newdisc());                  \
            }                                           \
        }                                               \
    }



/// [#1033]
void main_ctrl_init() {
    ligaturepresent = false;
    cancelboundary = false;
    lfthit = false;
    rthit = false;
    insdisc = false;
} /* main_ctrl_init */

// [#1030]: governs T E X’s activities
// xref[17]:
//      [1030], 1032, 1040, 1041,
//  1052, 1054, 1055, 1056, 1057,
//  1126, 1134, 1208, 1290, 1332,
//  1337, 1344, 1347
void main_control(void) {
    Integer t;
    HalfWord bchar;

    if (everyjob != 0) begintokenlist(everyjob, EVERY_JOB_TEXT);

_LN_main_control__big_switch:
    get_x_token();

_LN_main_control__reswitch:
    // #1031: Give diagnostic information, if requested
    if (interrupt != 0) {
        if (OK_to_interrupt) {
            backinput();
            checkinterrupt();
            goto _LN_main_control__big_switch;
        }
    }
    #ifdef tt_DEBUG
        if (panicking) check_mem(false);
    #endif // #1031: tt_DEBUG
    if (tracingcommands > 0) /*:1031*/
        showcurcmdchr();
    switch (labs(mode) + curcmd) {
        case H_MODE + LETTER:
        case H_MODE + OTHER_CHAR:
        case H_MODE + CHAR_GIVEN:
            goto _Lmainloop;
            break;

        case H_MODE + CHAR_NUM:
            scan_char_num();
            curchr = cur_val;
            goto _Lmainloop;
            break;

        case H_MODE + NO_BOUNDARY:
            get_x_token();
            if (curcmd == LETTER || curcmd == OTHER_CHAR ||
                curcmd == CHAR_GIVEN || curcmd == CHAR_NUM)
                cancelboundary = true;
            goto _LN_main_control__reswitch;
            break;

        case H_MODE + SPACER:
            if (spacefactor == 1000) goto _Lappendnormalspace_;
            appspace();
            break;

        case H_MODE + EX_SPACE:
        case M_MODE + EX_SPACE: /*1045:*/
            goto _Lappendnormalspace_;
            break;

        case V_MODE:
        case H_MODE:
        case M_MODE:
        case V_MODE + SPACER:
        case M_MODE + SPACER:
        case M_MODE + NO_BOUNDARY:
            /* blank case */
            break;

        case V_MODE + IGNORE_SPACES:
        case H_MODE + IGNORE_SPACES:
        case M_MODE + IGNORE_SPACES:
            skip_spaces();
            goto _LN_main_control__reswitch;
            break;

        case V_MODE + STOP: /*1048:*/
            if (itsallover()) goto _Lexit;
            break;

        case V_MODE + VMOVE:
        case H_MODE + HMOVE:
        case M_MODE + HMOVE:
        case V_MODE + LAST_ITEM:
        case H_MODE + LAST_ITEM:
        case M_MODE + LAST_ITEM:
        case V_MODE + VADJUST:
        case V_MODE + ITAL_CORR:
        case V_MODE + EQ_NO:
        case H_MODE + EQ_NO:
        case V_MODE + MAC_PARAM:
        case H_MODE + MAC_PARAM:
        case M_MODE + MAC_PARAM: /*:1048*/
            reportillegalcase();
            break;
            /*1046:*/

        case V_MODE + SUP_MARK:
        case H_MODE + SUP_MARK:
        case V_MODE + SUB_MARK:
        case H_MODE + SUB_MARK:
        case V_MODE + MATH_CHAR_NUM:
        case H_MODE + MATH_CHAR_NUM:
        case V_MODE + MATH_GIVEN:
        case H_MODE + MATH_GIVEN:
        case V_MODE + MATH_COMP:
        case H_MODE + MATH_COMP:
        case V_MODE + DELIM_NUM:
        case H_MODE + DELIM_NUM:
        case V_MODE + LEFT_RIGHT:
        case H_MODE + LEFT_RIGHT:
        case V_MODE + ABOVE:
        case H_MODE + ABOVE:
        case V_MODE + RADICAL:
        case H_MODE + RADICAL:
        case V_MODE + MATH_STYLE:
        case H_MODE + MATH_STYLE:
        case V_MODE + MATH_CHOICE:
        case H_MODE + MATH_CHOICE:
        case V_MODE + VCENTER:
        case H_MODE + VCENTER:
        case V_MODE + NON_SCRIPT:
        case H_MODE + NON_SCRIPT:
        case V_MODE + MKERN:
        case H_MODE + MKERN:
        case V_MODE + LIMIT_SWITCH:
        case H_MODE + LIMIT_SWITCH:
        case V_MODE + MSKIP:
        case H_MODE + MSKIP:
        case V_MODE + MATH_ACCENT:
        case H_MODE + MATH_ACCENT:
        case M_MODE + ENDV:
        case M_MODE + PAR_END:
        case M_MODE + STOP:
        case M_MODE + VSKIP:
        case M_MODE + UN_VBOX:
        case M_MODE + VALIGN:
        case M_MODE + HRULE: /*:1046*/
            insertdollarsign();
            break;

        /*1056:*/
        case V_MODE + HRULE:
        case H_MODE + VRULE:
        case M_MODE + VRULE: /*:1056*/
            tailappend(scan_rule_spec());
            if (labs(mode) == V_MODE)
                prevdepth = ignoredepth;
            else if (labs(mode) == H_MODE)
                spacefactor = 1000;
            break;
            /*1057:*/

        case V_MODE + VSKIP:
        case H_MODE + HSKIP:
        case M_MODE + HSKIP:
        case M_MODE + MSKIP:
            appendglue();
            break;

        case V_MODE + KERN:
        case H_MODE + KERN:
        case M_MODE + KERN:
        case M_MODE + MKERN: /*:1057*/
            appendkern();
            break;
            /*1063:*/

        case V_MODE + LEFT_BRACE:
        case H_MODE + LEFT_BRACE:
            newsavelevel(simplegroup);
            break;

        case V_MODE + BEGIN_GROUP:
        case H_MODE + BEGIN_GROUP:
        case M_MODE + BEGIN_GROUP:
            newsavelevel(semisimplegroup);
            break;

        case V_MODE + END_GROUP:
        case H_MODE + END_GROUP:
        case M_MODE + END_GROUP: /*:1063*/
            if (curgroup == semisimplegroup)
                unsave();
            else
                offsave();
            break;
            /*1067:*/

        case V_MODE + RIGHT_BRACE:
        case H_MODE + RIGHT_BRACE:
        case M_MODE + RIGHT_BRACE:
            handlerightbrace();
            break;

        /*:1067*/
        /*1073:*/
        case V_MODE + HMOVE:
        case H_MODE + VMOVE:
        case M_MODE + VMOVE:
            t = curchr;
            SCAN_NORMAL_DIMEN();
            if (t == 0)
                scanbox(cur_val);
            else
                scanbox(-cur_val);
            break;

        case V_MODE + LEADER_SHIP:
        case H_MODE + LEADER_SHIP:
        case M_MODE + LEADER_SHIP:
            scanbox(leaderflag - aleaders + curchr);
            break;

        case V_MODE + MAKE_BOX:
        case H_MODE + MAKE_BOX:
        case M_MODE + MAKE_BOX:
            beginbox(0);
            break;

        /*:1073*/
        /*1090:*/
        case V_MODE + START_PAR:
            newgraf(curchr > 0);
            break;

        case V_MODE + LETTER:
        case V_MODE + OTHER_CHAR:
        case V_MODE + CHAR_NUM:
        case V_MODE + CHAR_GIVEN:
        case V_MODE + MATH_SHIFT:
        case V_MODE + UN_HBOX:
        case V_MODE + VRULE:
        case V_MODE + ACCENT:
        case V_MODE + DISCRETIONARY:
        case V_MODE + HSKIP:
        case V_MODE + VALIGN:
        case V_MODE + EX_SPACE:
        case V_MODE + NO_BOUNDARY: /*:1090*/
            backinput();
            newgraf(true);
            break;
            /*1092:*/

        case H_MODE + START_PAR:
        case M_MODE + START_PAR: /*:1092*/
            indentinhmode();
            break;
            /*1094:*/

        case V_MODE + PAR_END:
            normalparagraph();
            if (mode > 0) buildpage();
            break;

        case H_MODE + PAR_END:
            if (align_state < 0) offsave();
            endgraf();
            if (mode == V_MODE) buildpage();
            break;

        case H_MODE + STOP:
        case H_MODE + VSKIP:
        case H_MODE + HRULE:
        case H_MODE + UN_VBOX:
        case H_MODE + HALIGN: /*:1094*/
            headforvmode();
            break;
            /*1097:*/

        case V_MODE + INSERT:
        case H_MODE + INSERT:
        case M_MODE + INSERT:
        case H_MODE + VADJUST:
        case M_MODE + VADJUST:
            begininsertoradjust();
            break;

        case V_MODE + MARK:
        case H_MODE + MARK:
        case M_MODE + MARK: /*:1097*/
            makemark();
            break;

        /*1102:*/
        case V_MODE + BREAK_PENALTY:
        case H_MODE + BREAK_PENALTY:
        case M_MODE + BREAK_PENALTY:
            appendpenalty();
            break;

        /*:1102*/
        /*1104:*/
        case V_MODE + REMOVE_ITEM:
        case H_MODE + REMOVE_ITEM:
        case M_MODE + REMOVE_ITEM: /*:1104*/
            deletelast();
            break;

        /*1109:*/
        case V_MODE + UN_VBOX:
        case H_MODE + UN_HBOX:
        case M_MODE + UN_HBOX:
            unpackage();
            break;

        /*:1109*/
        /*1112:*/
        case H_MODE + ITAL_CORR:
            appenditaliccorrection();
            break;

        case M_MODE + ITAL_CORR: /*:1112*/
            tailappend(newkern(0));
            break;
            /*1116:*/

        case H_MODE + DISCRETIONARY:
        case M_MODE + DISCRETIONARY: /*:1116*/
            appenddiscretionary();
            break;

        /*1122:*/
        case H_MODE + ACCENT:
            makeaccent();
            break;

        /*:1122*/
        /*1126:*/
        case V_MODE + CAR_RET:
        case H_MODE + CAR_RET:
        case M_MODE + CAR_RET:
        case V_MODE + TAB_MARK:
        case H_MODE + TAB_MARK:
        case M_MODE + TAB_MARK:
            alignerror();
            break;

        case V_MODE + NO_ALIGN:
        case H_MODE + NO_ALIGN:
        case M_MODE + NO_ALIGN:
            noalignerror();
            break;

        case V_MODE + OMIT:
        case H_MODE + OMIT:
        case M_MODE + OMIT: /*:1126*/
            omiterror();
            break;
            /*1130:*/

        case V_MODE + HALIGN:
        case H_MODE + VALIGN:
            initalign();
            break;

        case M_MODE + HALIGN:
            if (privileged()) {
                if (curgroup == mathshiftgroup)
                    initalign();
                else
                    offsave();
            }
            break;

        case V_MODE + ENDV:
        case H_MODE + ENDV: /*:1130*/
            doendv();
            break;
            /*1134:*/

        case V_MODE + END_CS_NAME:
        case H_MODE + END_CS_NAME:
        case M_MODE + END_CS_NAME: /*:1134*/
            cserror();
            break;
            /*1137:*/

        case H_MODE + MATH_SHIFT: /*:1137*/
            initmath();
            break;
            /*1140:*/

        case M_MODE + EQ_NO: /*:1140*/
            if (privileged()) {
                if (curgroup == mathshiftgroup)
                    starteqno();
                else
                    offsave();
            }
            break;
            /*1150:*/

        case M_MODE + LEFT_BRACE: /*:1150*/
            tailappend(newnoad());
            backinput();
            scanmath(nucleus(tail));
            break;
            /*1154:*/

        case M_MODE + LETTER:
        case M_MODE + OTHER_CHAR:
        case M_MODE + CHAR_GIVEN:
            setmathchar(math_code(curchr));
            break;

        case M_MODE + CHAR_NUM:
            scan_char_num();
            curchr = cur_val;
            setmathchar(math_code(curchr));
            break;

        case M_MODE + MATH_CHAR_NUM:
            scan_fifteen_bit_int();
            setmathchar(cur_val);
            break;

        case M_MODE + MATH_GIVEN:
            setmathchar(curchr);
            break;

        case M_MODE + DELIM_NUM: /*:1154*/
            scan_twenty_seven_bit_int();
            setmathchar(cur_val / 4096);
            break;
            /*1158:*/

        case M_MODE + MATH_COMP:
            tailappend(newnoad());
            type(tail) = curchr;
            scanmath(nucleus(tail));
            break;

        case M_MODE + LIMIT_SWITCH: /*:1158*/
            mathlimitswitch();
            break;
            /*1162:*/

        case M_MODE + RADICAL: /*:1162*/
            mathradical();
            break;
            /*1164:*/

        case M_MODE + ACCENT:
        case M_MODE + MATH_ACCENT: /*:1164*/
            mathac();
            break;
            /*1167:*/

        case M_MODE + VCENTER:
            scanspec(vcentergroup, false);
            normalparagraph();
            pushnest();
            mode = -V_MODE;
            prevdepth = ignoredepth;
            if (everyvbox != 0) begintokenlist(everyvbox, EVERY_VBOX_TEXT);
            break;
            /*:1167*/

        /*1171:*/
        case M_MODE + MATH_STYLE:
            tailappend(newstyle(curchr));
            break;

        case M_MODE + NON_SCRIPT:
            tailappend(newglue(zeroglue));
            subtype(tail) = condmathglue;
            break;

        case M_MODE + MATH_CHOICE:
            appendchoices();
            break;

        /*:1171*/
        /*1175:*/
        case M_MODE + SUB_MARK:
        case M_MODE + SUP_MARK:
            subsup();
            break;

        /*:1175*/
        /*1180:*/
        case M_MODE + ABOVE: /*:1180*/
            mathfraction();
            break;
            /*1190:*/

        case M_MODE + LEFT_RIGHT:
            mathleftright();
            break;

        /*:1190*/
        /*1193:*/
        case M_MODE + MATH_SHIFT:
            if (curgroup == mathshiftgroup)
                aftermath();
            else
                offsave();
            break;

        /*:1193*/
        /*1210:*/
        case V_MODE + TOKS_REGISTER:
        case H_MODE + TOKS_REGISTER:
        case M_MODE + TOKS_REGISTER:
        case V_MODE + ASSIGN_TOKS:
        case H_MODE + ASSIGN_TOKS:
        case M_MODE + ASSIGN_TOKS:
        case V_MODE + ASSIGN_INT:
        case H_MODE + ASSIGN_INT:
        case M_MODE + ASSIGN_INT:
        case V_MODE + ASSIGN_DIMEN:
        case H_MODE + ASSIGN_DIMEN:
        case M_MODE + ASSIGN_DIMEN:
        case V_MODE + ASSIGN_GLUE:
        case H_MODE + ASSIGN_GLUE:
        case M_MODE + ASSIGN_GLUE:
        case V_MODE + ASSIGN_MU_GLUE:
        case H_MODE + ASSIGN_MU_GLUE:
        case M_MODE + ASSIGN_MU_GLUE:
        case V_MODE + ASSIGN_FONT_DIMEN:
        case H_MODE + ASSIGN_FONT_DIMEN:
        case M_MODE + ASSIGN_FONT_DIMEN:
        case V_MODE + ASSIGN_FONT_INT:
        case H_MODE + ASSIGN_FONT_INT:
        case M_MODE + ASSIGN_FONT_INT:
        case V_MODE + SET_AUX:
        case H_MODE + SET_AUX:
        case M_MODE + SET_AUX:
        case V_MODE + SET_PREV_GRAF:
        case H_MODE + SET_PREV_GRAF:
        case M_MODE + SET_PREV_GRAF:
        case V_MODE + SET_PAGE_DIMEN:
        case H_MODE + SET_PAGE_DIMEN:
        case M_MODE + SET_PAGE_DIMEN:
        case V_MODE + SET_PAGE_INT:
        case H_MODE + SET_PAGE_INT:
        case M_MODE + SET_PAGE_INT:
        case V_MODE + SET_BOX_DIMEN:
        case H_MODE + SET_BOX_DIMEN:
        case M_MODE + SET_BOX_DIMEN:
        case V_MODE + SET_SHAPE:
        case H_MODE + SET_SHAPE:
        case M_MODE + SET_SHAPE:
        case V_MODE + DEF_CODE:
        case H_MODE + DEF_CODE:
        case M_MODE + DEF_CODE:
        case V_MODE + DEF_FAMILY:
        case H_MODE + DEF_FAMILY:
        case M_MODE + DEF_FAMILY:
        case V_MODE + SET_FONT:
        case H_MODE + SET_FONT:
        case M_MODE + SET_FONT:
        case V_MODE + DEF_FONT:
        case H_MODE + DEF_FONT:
        case M_MODE + DEF_FONT:
        case V_MODE + REGISTER:
        case H_MODE + REGISTER:
        case M_MODE + REGISTER:
        case V_MODE + ADVANCE:
        case H_MODE + ADVANCE:
        case M_MODE + ADVANCE:
        case V_MODE + MULTIPLY:
        case H_MODE + MULTIPLY:
        case M_MODE + MULTIPLY:
        case V_MODE + DIVIDE:
        case H_MODE + DIVIDE:
        case M_MODE + DIVIDE:
        case V_MODE + PREFIX:
        case H_MODE + PREFIX:
        case M_MODE + PREFIX:
        case V_MODE + LET:
        case H_MODE + LET:
        case M_MODE + LET:
        case V_MODE + SHORTHAND_DEF:
        case H_MODE + SHORTHAND_DEF:
        case M_MODE + SHORTHAND_DEF:
        case V_MODE + READ_TO_CS:
        case H_MODE + READ_TO_CS:
        case M_MODE + READ_TO_CS:
        case V_MODE + DEF:
        case H_MODE + DEF:
        case M_MODE + DEF:
        case V_MODE + SET_BOX:
        case H_MODE + SET_BOX:
        case M_MODE + SET_BOX:
        case V_MODE + HYPH_DATA:
        case H_MODE + HYPH_DATA:
        case M_MODE + HYPH_DATA:
        case V_MODE + SET_INTERACTION:
        case H_MODE + SET_INTERACTION:
        case M_MODE + SET_INTERACTION: /*:1210*/
            prefixedcommand();
            break;
            /*1268:*/

        case V_MODE + AFTER_ASSIGNMENT:
        case H_MODE + AFTER_ASSIGNMENT:
        case M_MODE + AFTER_ASSIGNMENT: /*:1268*/
            gettoken();
            aftertoken = curtok;
            break;
            /*1271:*/

        case V_MODE + AFTER_GROUP:
        case H_MODE + AFTER_GROUP:
        case M_MODE + AFTER_GROUP: /*:1271*/
            gettoken();
            saveforafter(curtok);
            break;
            /*1274:*/

        case V_MODE + IN_STREAM:
        case H_MODE + IN_STREAM:
        case M_MODE + IN_STREAM: /*:1274*/
            openorclosein();
            break;

        /*1276:*/
        case V_MODE + MESSAGE:
        case H_MODE + MESSAGE:
        case M_MODE + MESSAGE:
            issuemessage();
            break;

        /*:1276*/
        /*1285:*/
        case V_MODE + CASE_SHIFT:
        case H_MODE + CASE_SHIFT:
        case M_MODE + CASE_SHIFT:
            shiftcase();
            break;

        /*:1285*/
        /*1290:*/
        case V_MODE + XRAY:
        case H_MODE + XRAY:
        case M_MODE + XRAY:
            showwhatever();
            break;

        /*:1290*/
        /*1347:*/
        case V_MODE + EXTENSION:
        case H_MODE + EXTENSION:
        case M_MODE + EXTENSION: /*:1347*/
            doextension();
            break;
            /*:1045*/
    }
    goto _LN_main_control__big_switch;

/// [#1030] 均从上方跳入
_Lmainloop:
    // [#1034]: Append character `cur_chr` and the following characters (if any)
    // to the current hlist in the current font; 
    // goto `_reswitch`
    //      when a non-character has been fetched
    adjustspacefactor();
    mainf = cur_font;
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
    curl = NON_CHAR;
    goto _Lmainligloop1;

_Lmainloopwrapup:  /*1035:*/
    wrapup(rthit); /*:1035*/

_Lmainloopmove:    /*1036:*/
    if (ligstack == 0) goto _LN_main_control__reswitch;
    curq = tail;
    curl = character(ligstack);

_Lmainloopmove1:
    if (!ischarnode(ligstack)) goto _Lmainloopmovelig;

_Lmainloopmove2:
    if (curchr < fontbc[mainf] || curchr > fontec[mainf]) {
        charwarning(mainf, curchr);
        FREE_AVAIL(ligstack);
        goto _LN_main_control__big_switch;
    }
    maini = charinfo(mainf, curl);
    if (!charexists(maini)) {
        charwarning(mainf, curchr);
        FREE_AVAIL(ligstack);
        goto _LN_main_control__big_switch;
    }
    tailappend(ligstack); /*:1036*/

_Lmainlooplookahead:      /*1038:*/
    getnext();
    if (curcmd == LETTER) goto _Lmainlooplookahead1;
    if (curcmd == OTHER_CHAR) goto _Lmainlooplookahead1;
    if (curcmd == CHAR_GIVEN) goto _Lmainlooplookahead1;
    xtoken();
    if (curcmd == LETTER) goto _Lmainlooplookahead1;
    if (curcmd == OTHER_CHAR) goto _Lmainlooplookahead1;
    if (curcmd == CHAR_GIVEN) goto _Lmainlooplookahead1;
    if (curcmd == CHAR_NUM) {
        scan_char_num();
        curchr = cur_val;
        goto _Lmainlooplookahead1;
    }
    if (curcmd == NO_BOUNDARY) bchar = NON_CHAR;
    curr = bchar;
    ligstack = 0;
    goto _Lmainligloop;

_Lmainlooplookahead1:
    adjustspacefactor();
    FAST_GET_AVAIL(ligstack);
    font(ligstack) = mainf;
    curr = curchr;
    character(ligstack) = curr;
    if (curr == falsebchar) curr = NON_CHAR; /*:1038*/

_Lmainligloop:                              /*1039:*/
    if (chartag(maini) != LIG_TAG) {
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
            if (curl == NON_CHAR)
                lfthit = true;
            else if (ligstack == 0)
                rthit = true;
            checkinterrupt();
            switch (opbyte(mainj)) {

                case MIN_QUARTER_WORD + 1:
                case MIN_QUARTER_WORD + 5:
                    curl = rembyte(mainj);
                    maini = charinfo(mainf, curl);
                    ligaturepresent = true;
                    break;

                case MIN_QUARTER_WORD + 2:
                case MIN_QUARTER_WORD + 6:
                    curr = rembyte(mainj);
                    if (ligstack == 0) {
                        ligstack = newligitem(curr);
                        bchar = NON_CHAR;
                    } else if (ischarnode(ligstack)) {
                        mainp = ligstack;
                        ligstack = newligitem(curr);
                        ligptr(ligstack) = mainp;
                    } else
                        character(ligstack) = curr;
                    break;

                case MIN_QUARTER_WORD + 3:
                    curr = rembyte(mainj);
                    mainp = ligstack;
                    ligstack = newligitem(curr);
                    link(ligstack) = mainp;
                    break;

                case MIN_QUARTER_WORD + 7:
                case MIN_QUARTER_WORD + 11:
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
            if (opbyte(mainj) > MIN_QUARTER_WORD + 4) {
                if (opbyte(mainj) != MIN_QUARTER_WORD + 7) goto _Lmainloopwrapup;
            }
            if (curl < NON_CHAR) goto _Lmainligloop;
            maink = bcharlabel[mainf];
            goto _Lmainligloop1;
        }
        /*:1040*/
    }
    if (skipbyte(mainj) == MIN_QUARTER_WORD)
        maink++;
    else {
        if (skipbyte(mainj) >= stopflag) goto _Lmainloopwrapup;
        maink += skipbyte(mainj) - MIN_QUARTER_WORD + 1;
    }
    goto _Lmainligloop1; /*:1039*/

_Lmainloopmovelig:       /*1037:*/
    mainp = ligptr(ligstack);
    if (mainp > 0) {
        tailappend(mainp);
    }
    temp_ptr = ligstack;
    ligstack = link(temp_ptr);
    free_node(temp_ptr, smallnodesize);
    maini = charinfo(mainf, curl);
    ligaturepresent = true;
    if (ligstack == 0) {
        if (mainp > 0) goto _Lmainlooplookahead;
        curr = bchar;
    } else
        curr = character(ligstack);
    goto _Lmainligloop;          /*:1037*/
                                 /*:1034*/

/// [#1030]
_Lappendnormalspace_:
    // #1041: Append a normal inter-word space to the current list, 
    // then goto big switch
    if (spaceskip == zeroglue) { /*1042:*/
        mainp = fontglue[cur_font];
        if (mainp == 0) { /*:1042*/
            FontIndex mmaink;
            mainp = newspec(zeroglue);
            mmaink = parambase[cur_font] + SPACE_CODE;
    #if 1
            maink = mmaink;
    #endif
            width(mainp) = fontinfo[mmaink].sc;
            stretch(mainp) = fontinfo[mmaink + 1].sc;
            shrink(mainp) = fontinfo[mmaink + 2].sc;
            fontglue[cur_font] = mainp;
        }
        temp_ptr = newglue(mainp);
    } else
        temp_ptr = newparamglue(SPACE_SKIP_CODE);
    link(tail) = temp_ptr;
    tail = temp_ptr;
    goto _LN_main_control__big_switch;

_Lexit:;
} // #1030: main_control


/*
 * [#1043]: Declare action procedures for use by `main_control
 * `
 * xref[57]:
 *  1043, 1047, 1049, 1050, 1051,
 *  1054, 1060, 1061, 1064, 1069,
 *  1070, 1075, 1079, 1084, 1086,
 *  1091, 1093, 1095, 1096, 1099,
 *  1101, 1103, 1105, 1110, 1113,
 *  1117, 1119, 1123, 1127, 1129,
 *  1131, 1135, 1136, 1138, 1142,
 *  1151, 1155, 1159, 1160, 1163,
 *  1165, 1172, 1174, 1176, 1181,
 *  1191, 1194, 1200, 1211, 1270,
 *  1275, 1279, 1288, 1293, 1302,
 *  1348, 1376.
 */

// #1043
void appspace(void) {
    Pointer q;

    if (spacefactor >= 2000 && xspaceskip != zeroglue) {
        q = newparamglue(XSPACE_SKIP_CODE);
    } else {
        if (spaceskip != zeroglue) {
            mainp = spaceskip;
        } else { /*1042:*/
            mainp = fontglue[cur_font];
            if (mainp == 0) {
                FontIndex mmaink;
                mainp = newspec(zeroglue);
                mmaink = parambase[cur_font] + SPACE_CODE;
#if 1
                maink = mmaink;
#endif
                width(mainp) = fontinfo[maink].sc;
                stretch(mainp) = fontinfo[mmaink + 1].sc;
                shrink(mainp) = fontinfo[mmaink + 2].sc;
                fontglue[cur_font] = mainp;
            }
        }
        mainp = newspec(mainp); /*1044:*/
        if (spacefactor >= 2000) width(mainp) += extraspace(cur_font);
        stretch(mainp) = xn_over_d(stretch(mainp), spacefactor, 1000);
        shrink(mainp) = xn_over_d(shrink(mainp), 1000, spacefactor);
        /*:1044*/
        q = newglue(mainp);
        gluerefcount(mainp) = 0;
    }
    link(tail) = q;
    tail = q;

    /*:1042*/
} // #1043: appspace

// #1047
void insertdollarsign(void) {
    backinput();
    curtok = mathshifttoken + '$';
    print_err(S(827)); // "Missing $ inserted"
    // "I've inserted a begin-math/end-math symbol since I think"
    // "you left one out. Proceed with fingers crossed."
    help2(S(828), S(829));
    inserror();
} // #1047: insertdollarsign

// #1049
void youcant(void) {
    print_err(S(602)); // "You can't use `"
    printcmdchr(curcmd, curchr);
    print(S(830)); // "' in "
    print_mode(mode);
} // #1049: youcant

// #1050
void reportillegalcase(void) {
    youcant();
    /*
     * (831) "Sorry but I'm not programmed to handle this case;"
     * (832) "I'll just pretend that you didn't ask for it."
     * (833) "If you're in the wrong mode you might be able to"
     * (834) "return to the right one by typing `I}' or `I$' or `I\\par'."
     */
    help4(S(831), S(832), S(833), S(834));
    error();
} // #1050: reportillegalcase

// #1051
Boolean privileged(void) {
    if (mode > 0) {
        return true;
    } else {
        reportillegalcase();
        return false;
    }
} // #1051: privileged

// #1054
Boolean itsallover(void) {
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
} // #1054: itsallover

/** @}*/ // end group S1029x1054_P383x394