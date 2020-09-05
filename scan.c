#include <stdlib.h> // [func] labs
#include "texmac.h" // [macro] link, 
#include "texfunc.h"
#include "tex.h"
#include "tex_inc.h"
#include "global.h"
#include "macros.h" // [macro] help4,
#include "expand.h"
#include "texmac.h" // [macro] ischarnode

#include "texmath.h"
#include "fonts.h"
#include "printout.h" // [func] printcmdchr
#include "error.h"    // [func] error, int_error, print_err,
#include "scan.h"

/** @addtogroup S402x463_P155x173
 *
 * + #scan_left_brace
 * + #scan_optional_equals
 * + mu_error
 * @{
 */

/// [#438] apostrophe, indicates an octal constant.
#define OCTAL_TOKEN (othertoken + '\'')
/// [#438] double quote, indicates a hex constant.
#define HEX_TOKEN   (othertoken + '"')
/// [#438] reverse apostrophe, precedes alpha constants.
#define ALPHA_TOKEN (othertoken + '`')
/// [#438] decimal point.
#define POINT_TOKEN (othertoken + '.')
/// [#438] decimal point, Eurostyle.
#define CONTINENTAL_POINT_TOKEN (othertoken + ',')

/// [#445] the largest positive value that @f$ \rm\TeX\ @f$ knows.
#define INFINITY 2147483647L
/// [#445] the smallest special hex digit.
#define A_TOKEN (lettertoken + 'A')
/// [#445] special hex digit of type otherchar.
#define OTHER_A_TOKEN (othertoken + 'A')

/// [#463] 0.4 pt.
#define DEFAULT_RULE 26214


/// [#410] value returned by numeric scanners.
Integer cur_val;
/// [#410] the "level" of this value.
/// [INT_VAL=0, TOK_VAL=5]
SmallNumber cur_val_level;
/// [#438] #scan_int sets this to 8, 10, 16, or zero.
SmallNumber radix;
/// [#447] order of INFINITY found by #scan_dimen.
GlueOrd cur_order;


/// [#406] Get the next non-blank non-call token.
/// used in sections 405, 441, 455, 503, 526, 577, 785, 791, and 1045.
void skip_spaces(void) {
    do {
        get_x_token();
    } while (curcmd == SPACER);
} // [#406]

/// [#404] Get the next non-blank non-relax non-call token.
/// @f$ \rm\TeX\ @f$ allows `\relax` to appear before the left brace.
/// used in sections 403, 1078, 1084, 1151, 1160, 1211, 1226, and 1270.
void skip_spaces_or_relax(void) {
    do {
        get_x_token();
    } while (curcmd == SPACER || curcmd == RELAX);
} // [#404]

/** [#403] when a left brace is supposed to be the next non-blank token.
 *
 * xref[12]:
 *       473,  645,  785,  934,  960,
 *      1025, 1099, 1117, 1119, 1153,
 *      1172, 1174
 */
void scan_left_brace(void) {
    skip_spaces_or_relax();
    if (curcmd == LEFT_BRACE) return;

    print_err(S(566)); // "Missing { inserted"
    /*
     * (567) "A left brace was mandatory here, so I´ve put one in."
     * (568) "You might want to delete and/or insert some corrections"
     * (569) "so that I will find a matching right brace soon."
     * (570) "(If you´re confused by all this, try typing `I}´ now.)"
     */
    help4(S(567), S(568), S(569), S(570));
    backerror();

    curtok = leftbracetoken + '{';
    curcmd = LEFT_BRACE;
    curchr = '{';
    align_state++;
} // [#403] scan_left_brace

/** [#405] looks for an optional `=` sign preceded by optional spaces.
 * `\relax` is not ignored here.
 * 
 * xerf[19]:
 *       782, 1224, 1226, 1228, 1232,
 *      1234, 1236, 1241, 1243, 1244,
 *      1245, 1246, 1247, 1248, 1253,
 *      1257, 1275, 1351
 */   
void scan_optional_equals(void) {
    skip_spaces();
    if (curtok != (othertoken + '=')) backinput();
} // [#405] scan_optional_equals

/** [#407] look for a given string.
 * checks to see whether the next tokens of input match this string.
 * 
 * If a match is found, 
 *  the characters are effectively removed from the input
 *  and `true` is returned.
 * Otherwise false is returned, 
 *  and the input is left essentially unchanged
 *  (except for the fact that some macros may have been expanded, etc.).
 */  
Boolean scankeyword(StrNumber s) {
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
        get_x_token();
        if ((curcs == 0) & ((curchr == str_pool[k]) |
                (curchr == str_pool[k] - 'a' + 'A'))) {
#else
    k = 0;
    k_e = str_length(s);
    while (k < k_e) {
        int str_c = str_getc(s, k);
        get_x_token();
        if ((curcs == 0) &
            ((curchr == str_c) | (curchr == str_c - 'a' + 'A'))) {
#endif
            STORE_NEW_TOKEN(p, curtok);
            k++;
            continue;
        } else {
            if (curcmd == SPACER && p == my_backup_head) continue;
            backinput();
            if (p != my_backup_head) {
                backlist(link(my_backup_head));
            }
            Result = false;
            goto _Lexit;
        }
    } // while (k < k_e)
    flushlist(link(my_backup_head));
    Result = true;

_Lexit :
    FREE_AVAIL(my_backup_head);
    return Result;
} // [#407] scankeyword

/// [#408] sounds an alarm when mu and non-mu units are being switched.
static void mu_error(void) {
    print_err(S(571)); // "Incompatible glue units"
    help1(S(572)); // "I´m going to assume that 1mu=1pt when they´re mixed."
    error();
} // [#408] mu_error

/** [#413] fetch an internal parameter.
 * 
 * xref[9]:
 *      409, 410, 432, 440, 449,
 *      451, 455, 461, 465
 */
void scan_something_internal(SmallNumber level, Boolean negative) {
    HalfWord m;
    /* char */ int p; /* INT */

    m = curchr;
    switch (curcmd) {
        case DEF_CODE: /*414:*/
            scan_char_num();
            if (m == mathcodebase) {
                cur_val = mathcode(cur_val);
                cur_val_level = INT_VAL;
            } else if (m < mathcodebase) {
                cur_val = equiv(m + cur_val);
                cur_val_level = INT_VAL;
            } else {
                cur_val = eqtb[m + cur_val - activebase].int_;
                cur_val_level = INT_VAL;
            }
            break;
            /*:414*/

        case TOKS_REGISTER:
        case ASSIGN_TOKS:
        case DEF_FAMILY:
        case SET_FONT:
        case DEF_FONT: /*415:*/
            if (level != TOK_VAL) {
                print_err(S(593)); // "Missing number treated as zero"
                /*
                 * (594) "A number should have been here; I inserted `0'."
                 * (595) "(If you can't figure out why I needed to see a number"
                 * (596) "look up `weird error' in the index to The TeXbook.)"
                 */
                help3(S(594), S(595), S(596));
                backerror();
                cur_val = 0;
                cur_val_level = DIMEN_VAL;
            } else if (curcmd <= ASSIGN_TOKS) {
                if (curcmd < ASSIGN_TOKS) {
                    scan_eight_bit_int();
                    m = toksbase + cur_val;
                }
                cur_val = equiv(m);
                cur_val_level = TOK_VAL;
            } else {
                backinput();
                scanfontident();
                cur_val += fontidbase;
                cur_val_level = IDENT_VAL;
            }
            break;

        case ASSIGN_INT:
            cur_val = eqtb[m - activebase].int_;
            cur_val_level = INT_VAL;
            break;

        case ASSIGN_DIMEN:
            cur_val = eqtb[m - activebase].sc;
            cur_val_level = DIMEN_VAL;
            break;

        case ASSIGN_GLUE:
            cur_val = equiv(m);
            cur_val_level = GLUE_VAL;
            break;

        case ASSIGN_MU_GLUE:
            cur_val = equiv(m);
            cur_val_level = MU_VAL;
            break;

        case SET_AUX: /*418:*/
            if (labs(mode) != m) {
                print_err(S(597)); // "Improper "
                printcmdchr(SET_AUX, m);
                /*
                 * (598) "You can refer to \\spacefactor only in horizontal mode;"
                 * (599) "you can refer to \\prevdepth only in vertical mode; and"
                 * (600) "neither of these is meaningful inside \\write. So"
                 * (601) "I'm forgetting what you said and using zero instead."
                 */
                help4(S(598), S(599), S(600), S(601));
                error();
                if (level != TOK_VAL) {
                    cur_val = 0;
                    cur_val_level = DIMEN_VAL;
                } else {
                    cur_val = 0;
                    cur_val_level = INT_VAL;
                }
            } else if (m == V_MODE) {
                cur_val = prevdepth;
                cur_val_level = DIMEN_VAL;
            } else {
                cur_val = spacefactor;
                cur_val_level = INT_VAL;
            }
            break;

        case SET_PREV_GRAF: /*422:*/
            if (mode == 0) {
                cur_val = 0;
                cur_val_level = INT_VAL;
            } else { /*:422*/
                nest[nest_ptr] = cur_list;
                p = nest_ptr;
                while (abs(nest[p].modefield) != V_MODE)
                    p--;
                cur_val = nest[p].pgfield;
                cur_val_level = INT_VAL;
            }
            break;

        case SET_PAGE_INT: /*419:*/
            if (m == 0)
                cur_val = deadcycles;
            else
                cur_val = insertpenalties;
            cur_val_level = INT_VAL;
            break;
            /*:419*/

        case SET_PAGE_DIMEN: /*421:*/
            if (pagecontents == EMPTY && !outputactive) {
                if (m == 0)
                    cur_val = MAX_DIMEN;
                else
                    cur_val = 0;
            } else
                cur_val = pagesofar[m];
            cur_val_level = DIMEN_VAL;
            break;
            /*:421*/

        case SET_SHAPE: /*423:*/
            if (parshapeptr == 0)
                cur_val = 0;
            else
                cur_val = info(parshapeptr);
            cur_val_level = INT_VAL;
            break;
            /*:423*/

        case SET_BOX_DIMEN: /*420:*/
            scan_eight_bit_int();
            if (box(cur_val) == 0)
                cur_val = 0;
            else
                cur_val = mem[box(cur_val) + m - MEM_MIN].sc;
            cur_val_level = DIMEN_VAL;
            break;
            /*:420*/

        case CHAR_GIVEN:
        case MATH_GIVEN:
            cur_val = curchr;
            cur_val_level = INT_VAL;
            break;

        case ASSIGN_FONT_DIMEN: /*425:*/
            findfontdimen(false);
            fontinfo[fmemptr].sc = 0;
            cur_val = fontinfo[cur_val].sc;
            cur_val_level = DIMEN_VAL;
            break;
            /*:425*/

        case ASSIGN_FONT_INT: /*426:*/
            scanfontident();
            if (m == 0) {
                cur_val = get_hyphenchar(cur_val);
                cur_val_level = INT_VAL;
            } else {
                cur_val = get_skewchar(cur_val);
                cur_val_level = INT_VAL;
            }
            break;
            /*:426*/

        case REGISTER: /*427:*/
            scan_eight_bit_int();
            switch (m) {
                case INT_VAL: cur_val = count(cur_val); break;
                case DIMEN_VAL: cur_val = dimen(cur_val); break;
                case GLUE_VAL: cur_val = skip(cur_val); break;
                case MU_VAL: cur_val = muskip(cur_val); break;
            }
            cur_val_level = m;
            break;
            /*:427*/

        case LAST_ITEM: /*424:*/
            if (curchr > GLUE_VAL) {
                if (curchr == INPUT_LINE_NO_CODE)
                    cur_val = line;
                else
                    cur_val = lastbadness;
                cur_val_level = INT_VAL;
            } else { /*:424*/
                if (curchr == GLUE_VAL)
                    cur_val = zeroglue;
                else
                    cur_val = 0;
                cur_val_level = curchr;
                if (!ischarnode(tail) && mode != 0) {
                    switch (curchr) {

                        case INT_VAL:
                            if (type(tail) == PENALTY_NODE)
                                cur_val = penalty(tail);
                            break;

                        case DIMEN_VAL:
                            if (type(tail) == KERN_NODE) cur_val = width(tail);
                            break;

                        case GLUE_VAL:
                            if (type(tail) == GLUE_NODE) {
                                cur_val = glueptr(tail);
                                if (subtype(tail) == muglue)
                                    cur_val_level = MU_VAL;
                            }
                            break;
                    }
                } else if (mode == V_MODE && tail == head) {
                    switch (curchr) {

                        case INT_VAL: cur_val = lastpenalty; break;

                        case DIMEN_VAL: cur_val = lastkern; break;

                        case GLUE_VAL:
                            if (lastglue != MAX_HALF_WORD) cur_val = lastglue;
                            break;
                    }
                }
            }
            break;
            /*428:*/

        default:
            print_err(S(602)); // "You can't use `"
            printcmdchr(curcmd, curchr);
            print(S(603)); // "' after "
            print_esc(S(604)); // "the"
            // "I'm forgetting what you said and using zero instead."
            help1(S(601));
            error();
            if (level != TOK_VAL) { /*:428*/
                cur_val = 0;
                cur_val_level = DIMEN_VAL;
            } else {
                cur_val = 0;
                cur_val_level = INT_VAL;
            }
            break;
    }
    while (cur_val_level > level) { /*429:*/
        if (cur_val_level == GLUE_VAL)
            cur_val = width(cur_val);
        else if (cur_val_level == MU_VAL)
            mu_error();
        cur_val_level--;
    }
    /*:429*/
    /*430:*/
    if (!negative) {
        if (cur_val_level >= GLUE_VAL && cur_val_level <= MU_VAL) {
            addglueref(cur_val); /*:430*/
        }
        return;
    }
    if (cur_val_level < GLUE_VAL) {
        cur_val = -cur_val;
        return;
    }
    cur_val = newspec(cur_val); /*431:*/
    width(cur_val) = -width(cur_val);
    stretch(cur_val) = -stretch(cur_val);
    shrink(cur_val) = -shrink(cur_val); /*:431*/

    /*:415*/
    /*:418*/
} // [#413] scan_something_internal

/** [#433] scan `register code = [0, 255]`, use #scan_int.
 *
 * xref[15]:
 *       415,  420,  427,  505, 1079,
 *      1082, 1099, 1110, 1224, 1226,
 *      1227, 1237, 1241, 1247, 1296
 */
void scan_eight_bit_int(void) {
    scan_int();
    if ((unsigned long)cur_val <= 255) return;
    print_err(S(573)); // "Bad register code"
    // "A register number must be between 0 and 255."
    // "I changed this one to zero."
    help2(S(574), S(575));
    int_error(cur_val);
    cur_val = 0;
} // [#433] scan_eight_bit_int

/** [#434] scan `character code = [0, 255]`, use #scan_int.
 *
 * xref[10]:
 *       414,  935, 1030, 1038, 1123,
 *      1124, 1151, 1154, 1224, 1232
 */
void scan_char_num(void) {
    scan_int();
    if ((unsigned long)cur_val <= 255) return;
    print_err(S(576)); // "Bad character code"
    // "A character number must be between 0 and 255."
    // "I changed this one to zero."
    help2(S(577), S(575));
    int_error(cur_val);
    cur_val = 0;
} // [#434] scan_char_num

/** [#435] scan `number = [0, 15]`, use #scan_int.
 *
 * xref[5]: 501, 577, 1234, 1275, 1350
 */
void scan_four_bit_int(void) {
    scan_int();
    if ((unsigned long)cur_val <= 15) return;
    print_err(S(578)); // "Bad number"
    // "Since I expected to read a number between 0 and 15,"
    // "I changed this one to zero."
    help2(S(579), S(575));
    int_error(cur_val);
    cur_val = 0;
} // [#435] scan_four_bit_int

/** [#436] scan `mathchar = [0, 23767]`, use #scan_int.
 *
 * xref[4]: 1151, 1154, 1165, 1224
 */
void scan_fifteen_bit_int(void) {
    scan_int();
    if ((unsigned long)cur_val <= 32767) return;
    print_err(S(580)); // "Bad mathchar"
    // "A mathchar number must be between 0 and 32767."
    // "I changed this one to zero."
    help2(S(581), S(575));
    int_error(cur_val);
    cur_val = 0;
} // [#436] scan_fifteen_bit_int

/** [#437] scan `mathchar` = @f$ [0, 2^{27}−1] @f$, use #scan_int.
 *
 * xref[3]: 1151, 1154, 1160
 */
void scan_twenty_seven_bit_int(void) {
    scan_int();
    if ((unsigned long)cur_val <= 134217727L) return;
    print_err(S(582)); // "Bad delimiter code"
    // "A numeric delimiter code must be between 0 and 2^{27}−1."
    // "I changed this one to zero."
    help2(S(583), S(575));
    int_error(cur_val);
    cur_val = 0;
} // [#437] scan_twenty_seven_bit_int

/** [#440] sets #cur_val to an integer.
 *
 * xref[31]:
 *      409, 410, 432, 433, 434, 
 *      435, 436, 437, 438, 447, 
 *      448, 461, 471, 503, 504, 
 *      509, 578, 1103, 1225, 1228, 
 *      1232, 1238, 1240, 1243, 1244, 
 *      1246, 1248, 1253, 1258, 1350, 
 *      1377
 */
void scan_int(void) {
    Boolean negative;
    long m;
    SmallNumber d;
    Boolean vacuous, OKsofar;

    radix = 0;
    OKsofar = true; /*441:*/
    negative = false;

    // [#441] Get the next non-blank non-sign token; 
    // set #negative appropriately
    do {
        skip_spaces();
        if (curtok == othertoken + '-') { /*:441*/
            negative = !negative;
            curtok = othertoken + '+';
        }
    } while (curtok == othertoken + '+');

    // [#442] Scan an alphabetic character code into #cur_val.
    if (curtok == ALPHA_TOKEN) { /*442:*/
        gettoken();
        if (curtok < CS_TOKEN_FLAG) {
            cur_val = curchr;
            if (curcmd <= RIGHT_BRACE) {
                if (curcmd == RIGHT_BRACE)
                    align_state++;
                else
                    align_state--;
            }
        } else if (curtok < CS_TOKEN_FLAG + singlebase)
            cur_val = curtok - CS_TOKEN_FLAG - activebase;
        else
            cur_val = curtok - CS_TOKEN_FLAG - singlebase;

        if (cur_val > 255) {
            print_err(S(605)); // "Improper alphabetic constant"
            // "A one−character control sequence belongs after a ` mark."
            // "So I´m essentially inserting \0 here."
            help2(S(606), S(607));
            cur_val = '0';
            backerror();
        } else { /*443:*/
            get_x_token();
            if (curcmd != SPACER) backinput();
        }
    } /*:442*/
    else if (curcmd >= MIN_INTERNAL && curcmd <= MAX_INTERNAL)
        scan_something_internal(INT_VAL, false);
    else {
        radix = 10;
        m = 214748364L;
        if (curtok == OCTAL_TOKEN) {
            radix = 8;
            m = 268435456L;
            get_x_token();
        } else if (curtok == HEX_TOKEN) {
            radix = 16;
            m = 134217728L;
            get_x_token();
        }
        vacuous = true;
        cur_val = 0; 
        
        // [#445] Accumulate the constant until cur tok is not a suitable digit.
        while (true) {
            if (curtok < ZERO_TOKEN + radix && curtok >= ZERO_TOKEN &&
                curtok <= ZERO_TOKEN + 9)
                d = curtok - ZERO_TOKEN;
            else if (radix == 16) {
                if (curtok <= A_TOKEN + 5 && curtok >= A_TOKEN)
                    d = curtok - A_TOKEN + 10;
                else if (curtok <= OTHER_A_TOKEN + 5 && curtok >= OTHER_A_TOKEN)
                    d = curtok - OTHER_A_TOKEN + 10;
                else
                    goto _Ldone;
            } else
                goto _Ldone;
            vacuous = false;
            if (cur_val >= m && (cur_val > m || d > 7 || radix != 10)) {
                if (OKsofar) {
                    print_err(S(608)); // "So I´m essentially inserting \0 here."
                    // "I can only go up to 2147483647=´17777777777=\" \" 7FFFFFFF,"
                    // " so I´m using that number instead of yours."
                    help2(S(609), S(610));
                    error();
                    cur_val = INFINITY;
                    OKsofar = false;
                }
            } else
                cur_val = cur_val * radix + d;
            get_x_token();
        }

    _Ldone:            /*:445*/
        if (vacuous) { /*446:*/
            // [#446] Express astonishment that no number was here.
            print_err(S(593)); // "Missing number, treated as zero"
            // "A number should have been here; I inserted `0´."
            // "(If you can´t figure out why I needed to see a number,"
            // "look up `weird error´ in the index to The TeXbook.)"
            help3(S(594), S(595), S(596));
            backerror();
        } /*:446*/
        else if (curcmd != SPACER)
            backinput();
    }

    if (negative) cur_val = -cur_val;
    /*:443*/
} // [#440] scan_int

/** [#448] sets #cur_val to a dimension.
 *
 * xref[6]: 410, 440, 447, 461, 462, 1061
 */
void scan_dimen(Boolean mu, Boolean inf, Boolean shortcut) {
    Boolean negative; ///< should the answer be negated?
    Integer f;        ///<  numerator of a fraction whose denominator is 2^16.

    /// [#450] Local variables for dimension calculations
    long num, denom; // conversion ratio for the scanned units
    /* SmallNumber */ int k, kk; // number of digits in a decimal fraction
    Pointer p, q; // top of decimal digit stack
    Scaled v; // an internal dimension
    long savecurval; // temporary storage of cur_val

    char digs[23];


    f = 0;
    arith_error = false;
    cur_order = NORMAL;
    negative = false;
    if (!shortcut) {
        /// [#441] Get the next non-blank non-sign token; 
        /// set negative appropriately
        negative = false;
        do {
            skip_spaces();
            if (curtok == othertoken + '-') {
                negative = !negative;
                curtok = othertoken + '+';
            }
        } while (curtok == othertoken + '+');

        if (curcmd >= MIN_INTERNAL && curcmd <= MAX_INTERNAL) {
            /// [#449] Fetch an internal dimension and goto attach sign,
            /// or fetch an internal integer
            if (mu) {
                scan_something_internal(MU_VAL, false);
                /// [#451] Coerce glue to a dimension.
                if (cur_val_level >= GLUE_VAL) {
                    v = width(cur_val);
                    delete_glue_ref(cur_val);
                    cur_val = v;
                }
                if (cur_val_level == MU_VAL) goto _Lattachsign_;
                if (cur_val_level != INT_VAL) mu_error();
            } else {
                scan_something_internal(DIMEN_VAL, false);
                if (cur_val_level == DIMEN_VAL) goto _Lattachsign_;
            } // if (<=>mu)
        } else {
            backinput();
            if (curtok == CONTINENTAL_POINT_TOKEN) curtok = POINT_TOKEN;
            if (curtok != POINT_TOKEN) {
                scan_int();
            } else {
                radix = 10;
                cur_val = 0;
            }
            if (curtok == CONTINENTAL_POINT_TOKEN) curtok = POINT_TOKEN;
            if (radix == 10 && curtok == POINT_TOKEN) { 
                /// [#452] 
                k = 0;
                p = 0;
                gettoken();
                while (true) {
                    get_x_token();
                    if (curtok > ZERO_TOKEN + 9 || curtok < ZERO_TOKEN) {
                        goto _Ldone1;
                    }
                    if (k >= 17) continue;
                    q = get_avail();
                    link(q) = p;
                    info(q) = curtok - ZERO_TOKEN;
                    p = q;
                    k++;
                } // while (true)

            _Ldone1:
                for (kk = k - 1; kk >= 0; kk--) {
                    digs[kk] = info(p);
                    q = p;
                    p = link(p);
                    FREE_AVAIL(q);
                }
                f = round_decimals(k, digs);
                if (curcmd != SPACER) backinput();
            } // if (radix == 10 && curtok == POINT_TOKEN)
        }
    } // if (!shortcut)

    /// [#451]
    if (cur_val < 0) {
        negative = !negative;
        cur_val = -cur_val;
    }

    /// [#453] Scan units and set cur_val to
    ///     x · (cur val + f/2^16)
    /// where there are x sp per unit;
    /// goto attach sign if the units are internal
    if (inf) {
        /// [#454] Scan for fil units; goto attach_fraction if found
        if (scankeyword(S(330))) {
            cur_order = FIL;
            while (scankeyword('l')) {
                if (cur_order != FILLL) {
                    cur_order++;
                    continue;
                }
                print_err(S(611)); // "Illegal unit of measure (\"
                print(S(612)); // "replaced by filll)"
                help1(S(613)); // "I dddon´t go any higher than filll."
                error();
            }
            goto _Lattachfraction_;
        }
    } // if (inf)

    /// [#455] Scan for units that are internal dimensions;
    /// goto attach sign with cur val set if found.
    savecurval = cur_val;
    skip_spaces();
    if (curcmd >= MIN_INTERNAL && curcmd <= MAX_INTERNAL) {
        if (mu) {
            scan_something_internal(MU_VAL, false);
            /// [#451] Coerce glue to a dimension.
            if (cur_val_level >= GLUE_VAL) {
                v = width(cur_val);
                delete_glue_ref(cur_val);
                cur_val = v;
            }
            if (cur_val_level != MU_VAL) mu_error();
        } else {
            scan_something_internal(DIMEN_VAL, false);
        }
        v = cur_val;
        goto _Lfound;
    }
    backinput();
    if (mu) goto _Lnotfound;
    
    if (scankeyword(S(614))) { // "em"
        v = quad(curfont); /// [#558] The em width for cur_font.
    } else if (scankeyword(S(615))) { // "ex"
        v = xheight(curfont); /// [#559] The x-height for cur_font.
    } else {
        goto _Lnotfound;
    }

    get_x_token();
    if (curcmd != SPACER) {
        backinput();
    }

_Lfound:
    cur_val = nx_plus_y(savecurval, v, xn_over_d(v, f, 65536L));
    goto _Lattachsign_;

_Lnotfound:
    /// [#456] Scan for mu units and goto attach fraction.
    if (mu) {
        if (scankeyword(S(390))) { // "mu"
            goto _Lattachfraction_;
        } else {
            print_err(S(611)); // "Illegal unit of measure ("
            print(S(616)); // "mu inserted)"
            /*
             * (617) "The unit of measurement in math glue must be mu."
             * (618) "To recover gracefully from this error, it´s best to"
             * (619) "delete the erroneous units; e.g., type `2´ to delete"
             * (620) "two letters. (See Chapter 27 of The TeXbook.)"
             */
            help4(S(617), S(618), S(619), S(620));
            error();
            goto _Lattachfraction_;
        }
    } // if (mu)

    if (scankeyword(S(621))) { //  true"
        // [#457] Adjust for the magnification ratio.
        preparemag();
        if (mag != 1000) {
            cur_val = xn_over_d(cur_val, 1000, mag);
            f = (f * 1000 + tex_remainder * 65536L) / mag;
            cur_val += f / 65536L;
            f %= 65536L;
        }
    }

    if (scankeyword(S(459))) { // "pt"
        goto _Lattachfraction_;
    }

    // [#458] Scan for all other units and adjust cur val and f accordingly;
    // goto done in the case of scaled points
    if (scankeyword(S(622))) { // "in"
        num = 7227;
        denom = 100;
    } else if (scankeyword(S(623))) { // "pc"
        num = 12;
        denom = 1;
    } else if (scankeyword(S(624))) { // "cm"
        num = 7227;
        denom = 254;
    } else if (scankeyword(S(625))) { // "mm"
        num = 7227;
        denom = 2540;
    } else if (scankeyword(S(626))) { // "bp"
        num = 7227;
        denom = 7200;
    } else if (scankeyword(S(627))) { // "dd"
        num = 1238;
        denom = 1157;
    } else if (scankeyword(S(628))) { // "cc"
        num = 14856;
        denom = 1157;
    } else if (scankeyword(S(629))) { // "sp"
        goto _Ldone;
    } else {
        // [#459] Complain about unknown unit and goto done2.
        print_err(S(611)); // "Illegal unit of measure ("
        print(S(630)); // "pt inserted)"
        /*
         * [631] " Dimensions can be in units of em, ex, in, pt, pc,"
         * [632] "cm, mm, dd, cc, bp, or sp; but yours is a new one!"
         * [633] "I´ll assume that you meant to say pt, for printer´s points."
         * [618] "To recover gracefully from this error, it´s best to"
         * [619] "delete the erroneous units; e.g., type `2´ to delete"
         * [620] "two letters. (See Chapter 27 of The TeXbook.)"
         */
        help6(S(631), S(632), S(633), S(618), S(619), S(620));
        error();
        goto _Ldone2;
    }

    cur_val = xn_over_d(cur_val, num, denom);
    f = (num * f + tex_remainder * 65536L) / denom;
    cur_val += f / 65536L;
    f %= 65536L;

_Ldone2:
_Lattachfraction_:
    if (cur_val >= 16384) {
        arith_error = true;
    } else {
        cur_val = cur_val * UNITY + f;
    }

_Ldone:
    /// [#443] Scan an optional space.
    get_x_token();
    if (curcmd != SPACER) {
        backinput();
    }

_Lattachsign_:
    if (arith_error || labs(cur_val) >= 1073741824L) {
        // [#460] Report that this dimension is out of range.
        print_err(S(634)); // " Dimension too large"
        // "I can´t work with sizes bigger than about 19 feet."
        // "Continue and I´ll use the largest value I can."
        help2(S(635), S(636));
        error();
        cur_val = MAX_DIMEN;
        arith_error = false;
    }

    if (negative) cur_val = -cur_val;
} /* [#448] scan_dimen */

/** [#461] sets #cur_val to a glue spec pointer.
 *
 * xref[5]: 410, 782, 1060, 1228, 1238
 */
void scan_glue(SmallNumber level) {
    Boolean negative, mu;
    Pointer q;

    mu = (level == MU_VAL); /*441:*/
    negative = false;
    do {
        skip_spaces();
        if (curtok == othertoken + '-') { /*:441*/
            negative = !negative;
            curtok = othertoken + '+';
        }
    } while (curtok == othertoken + '+');

    if (curcmd >= MIN_INTERNAL && curcmd <= MAX_INTERNAL) { /*462:*/
        scan_something_internal(level, negative);
        if (cur_val_level >= GLUE_VAL) {
            if (cur_val_level != level) mu_error();
            goto _Lexit;
        }
        if (cur_val_level == INT_VAL)
            scan_dimen(mu, false, true);
        else if (level == MU_VAL)
            mu_error();
    } else {
        backinput();
        scan_dimen(mu, false, false);
        if (negative) cur_val = -cur_val;
    }

    q = newspec(zeroglue);
    width(q) = cur_val;

    if (scankeyword(S(637))) { // "plus"
        scan_dimen(mu, true, false);
        stretch(q) = cur_val;
        stretchorder(q) = cur_order;
    }
    if (scankeyword(S(638))) { // "minus"
        scan_dimen(mu, true, false);
        shrink(q) = cur_val;
        shrinkorder(q) = cur_order;
    }
    cur_val = q; /*:462*/

_Lexit:;
} // [#461] scan_glue

/** [#463] returns a pointer to a rule node.
 * 
 * This routine is called just after @f$ \rm\TeX\ @f$ has seen `\hrule` or
 *  `\vrule`; therefore cur cmd will be either hrule or vrule.
 * The idea is to store the default rule dimensions in the node,
 *  then to override them if `height` or `width` or `depth` specifications
 *  are found (in any order).
 * 
 * xref[2]: 1056, 1084
 */
HalfWord scan_rule_spec(void) {
    Pointer q;

    q = newrule();
    if (curcmd == VRULE)
        width(q) = DEFAULT_RULE;
    else {
        height(q) = DEFAULT_RULE;
        depth(q) = 0;
    }

_LN_scanrulespec__reswitch:
    if (scankeyword(S(639))) { // "width"
        SCAN_NORMAL_DIMEN();
        width(q) = cur_val;
        goto _LN_scanrulespec__reswitch;
    }

    if (scankeyword(S(640))) { // "height"
        SCAN_NORMAL_DIMEN();
        height(q) = cur_val;
        goto _LN_scanrulespec__reswitch;
    }

    // "depth"
    if (!scankeyword(S(641))) return q;
    SCAN_NORMAL_DIMEN();
    depth(q) = cur_val;
    goto _LN_scanrulespec__reswitch;
} // [#463] scan_rule_spec

/** @}*/ // end group S402x463_P155x173
