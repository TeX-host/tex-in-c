#include "tex_header.h"

/*
 * main 一次调用部分
 *
 *  + initialize
 *  + init_prim
 *  + S1337_Get_the_first_line_of_input_and_prepare_to_start
 *      + open_fmt_file
 *      + load_fmt_file
 *      ++ start_input
 *  + main_control
 *  + final_cleanup
 *  + close_files_and_terminate
 *
 */

/// [#4]: this procedure gets things started properly.
Static void initialize(void) {
    /** [p11#21] Set initial values of key variables
     *
     * ## xref
     *  [21, 23, 24], [74, 77, 80, 97], [166], [215],
     *  [254], [257], [272, 287], [383], [439],
     *  [481], [490], [521], [551, 556], [593, 596, 606],
     *  [648, 662], [685], [771], [928], [990],
     *  [1033], [1267, 1282], [1300], [1343].
     */
    charset_init();
    error_init();
    mem_var_init();
    lexer_semantic_init();

    eqtb_init();
    hash_var_init();
    eqtb_save_init();
    expand_init();
    scan_init();

    build_token_init();
    cond_process_init();
    fname_init();
    font_init();
    dviout_init();

    pack_init();
    mmode_init();
    align_init();
    hyphen_init();
    page_builder_init();

    main_ctrl_init();
    mode_indep_init();
    dump_init();
    extension_init();
    /** end block [p11#21] */

/** [p59#164]: Initialize table entries (done by INITEX only).
 *
 * ## xref
 *  [164], [222, 228, 232, 240, 250], [258], [552],
 *  [946, 951], [1216], [1301], [1369].
 */
#ifdef tt_INIT
    mem_init_once();
    eqtb_init_once();
    hash_init_once();
    fonts_init_once();

    hyphen_init_once();
    mode_indep_init_once();
    dump_init_once();
    extension_init_once();
#endif // #164: tt_INIT
} /* [#4]: initialize */

// #1303: call open_fmt@func.c
Static Boolean open_fmt_file(void) { return open_fmt(&fmt_file); }


/** @addtogroup S1330x1337_P465x469
 * @{
 */
/// [#1333]
Static void close_files_and_terminate(void) {
    Integer k; // all-purpose index

    // #1378: Finish the extensions
    for (k = 0; k <= 15; k++) {
        if (write_open[k]) aclose(&write_file[k]);
    }

    #ifdef tt_STAT
        if (tracingstats > 0 && log_opened) {
            // #1334: Output statistics about this job
            fprintf(log_file, " \n");
            fprintf(log_file, "Here is how much of TeX's memory you used:\n");
            str_print_stats(log_file);
            fprintf(log_file,
                    " %ld words of memory out of %ld\n",
                    lo_mem_max - MEM_MIN + mem_end - hi_mem_min + 2L,
                    mem_end - MEM_MIN + 1L);
            fprintf(log_file,
                    " %lld multiletter control sequences out of %ld\n",
                    cs_count,
                    (long)HASH_SIZE);
            fprintf(log_file,
                    " %ld words of font info for %d font",
                    fmemptr,
                    fontptr);
            if (fontptr != 1) {
                fprintf(log_file, "s");
            }
            fprintf(log_file,
                    ", out of %ld for %ld\n",
                    (long)FONT_MEM_SIZE,
                    (long)FONT_MAX);
            fprintf(log_file, " %ld hyphenation exception", hyphcount);
            if (hyphcount != 1) {
                fprintf(log_file, "s");
            }
            fprintf(log_file, " out of %ld\n", (long)HYPH_SIZE);
            fprintf(log_file,
                    " %di,%dn,%lldp,%db,%ds stack positions out of "
                    "%ldi,%ldn,%ldp,%ldb,%lds\n",
                    get_maxinstack(),
                    max_nest_stack,
                    maxparamstack,
                    max_buf_stack + 1,
                    max_save_stack + 6,
                    (long)STACK_SIZE,
                    (long)NEST_SIZE,
                    (long)PARAM_SIZE,
                    (long)BUF_SIZE,
                    (long)SAVE_SIZE);
        }  // if (tracingstats > 0 && log_opened)
    #endif // #1333: tt_STAT

    // #642: Finish the DVI file
    while (curs > -1) {
        if (curs > 0) {
            dviout_POP();
        } else {
            dviout_EOP();
            totalpages++;
        }
        curs--;
    }

    if (totalpages == 0) {
        printnl(S(1013)); // "No pages of output."
    } else { /*:642*/
        long total_dvi_bytes;
        dviout_POST(); // beginning of the postamble
        _dvi_lastbop(); // post location
        // conversion ratio for sp
        dvi_four(25400000L);
        dvi_four(473628672L);
        // magnification factor
        prepare_mag();
        dvi_four(mag);
        dvi_four(maxv);
        dvi_four(maxh);
        dviout(maxpush / 256);
        dviout(maxpush % 256);
        dviout((totalpages / 256) % 256);
        dviout(totalpages % 256);
        while (fontptr > 0) {
            if (fontused[fontptr]) dvi_font_def(fontptr);
            fontptr--;
        }

        total_dvi_bytes = dviflush();
        printnl(S(1014)); // "Output written on "
        slow_print(output_file_name);
        print(S(303));  // " ("
        print_int(totalpages);
        print(S(1015)); // " page"
        if (totalpages != 1) print_char('s');
        print(S(1016)); // ", "
        print_int(total_dvi_bytes);
        print(S(1017)); // " bytes)."
    } // if (totalpages == 0) - else

    if (!log_opened) return;
    putc('\n', log_file);
    aclose(&log_file);
    selector -= 2;
    if (selector != TERM_ONLY) return;
    printnl(S(1018));
    slow_print(log_name);
    print_char('.');
    println();
} // #1333: void close_files_and_terminate(void)

/// [p468#1335]: Last-minute procedures
Static void final_cleanup(void) {
    SmallNumber c;

    c = curchr;
    if (job_name == 0) open_log_file();
    while (inputptr > 0) {
        if (STATE == TOKEN_LIST)
            endtokenlist();
        else
            endfilereading();
    }
    while (openparens > 0) {
        print(S(1019));
        openparens--;
    }
    if (cur_level > LEVEL_ONE) {
        printnl('(');
    #ifndef USE_REAL_STR
        print_esc(S(1020)); // "end occurred "
        print(S(1021));     // "inside a group at level "
    #else
        print_esc_str("end occurred ");
        print_str("inside a group at level ");
    #endif // USE_REAL_STR
        print_int(cur_level - LEVEL_ONE);
        print_char(')');
    }
    while (condptr != 0) {
        printnl('(');
        print_esc(S(1020));
        print(S(1022));
        printcmdchr(IF_TEST, curif);
        if (ifline != 0) {
            print(S(1023));
            print_int(ifline);
        }
        print(S(1024));
        ifline = iflinefield(condptr);
        curif = subtype(condptr);
        temp_ptr = condptr;
        condptr = link(condptr);
        free_node(temp_ptr, ifnodesize);
    }
    if (history != SPOTLESS) {
        if (history == WARNING_ISSUED || interaction < ERROR_STOP_MODE) {
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
                if (curmark[i] != 0) delete_token_ref(curmark[i]);
            }
            store_fmt_file();
    #endif // #1335: tt_INIT
    }
} // #1335: final_cleanup

#ifdef tt_INIT
// TeX 原语定义
/// p468#1336: initialize all the primitives
Static void init_prim(void) {
    /*226:*/
    primitive(S(341), ASSIGN_GLUE, GLUE_BASE);
    primitive(S(342), ASSIGN_GLUE, GLUE_BASE + BASELINE_SKIP_CODE);
    primitive(S(343), ASSIGN_GLUE, GLUE_BASE + PAR_SKIP_CODE);
    primitive(S(344), ASSIGN_GLUE, GLUE_BASE + ABOVE_DISPLAY_SKIP_CODE);
    primitive(S(345), ASSIGN_GLUE, GLUE_BASE + BELOW_DISPLAY_SKIP_CODE);
    primitive(S(346), ASSIGN_GLUE, GLUE_BASE + ABOVE_DISPLAY_SHORT_SKIP_CODE);
    primitive(S(347), ASSIGN_GLUE, GLUE_BASE + BELOW_DISPLAY_SHORT_SKIP_CODE);
    primitive(S(348), ASSIGN_GLUE, GLUE_BASE + LEFT_SKIP_CODE);
    primitive(S(349), ASSIGN_GLUE, GLUE_BASE + RIGHT_SKIP_CODE);
    primitive(S(350), ASSIGN_GLUE, GLUE_BASE + TOP_SKIP_CODE);
    primitive(S(351), ASSIGN_GLUE, GLUE_BASE + SPLIT_TOP_SKIP_CODE);
    primitive(S(352), ASSIGN_GLUE, GLUE_BASE + TAB_SKIP_CODE);
    primitive(S(353), ASSIGN_GLUE, GLUE_BASE + SPACE_SKIP_CODE);
    primitive(S(354), ASSIGN_GLUE, GLUE_BASE + XSPACE_SKIP_CODE);
    primitive(S(355), ASSIGN_GLUE, GLUE_BASE + PAR_FILL_SKIP_CODE);
    primitive(S(356), ASSIGN_MU_GLUE, GLUE_BASE + THIN_MU_SKIP_CODE);
    primitive(S(357), ASSIGN_MU_GLUE, GLUE_BASE + MED_MU_SKIP_CODE);
    primitive(S(358), ASSIGN_MU_GLUE, GLUE_BASE + THICK_MU_SKIP_CODE);
    /*:226*/
    /*230:*/
    primitive(S(1026), ASSIGN_TOKS, OUTPUT_ROUTINE_LOC);
    primitive(S(1027), ASSIGN_TOKS, EVERY_PAR_LOC);
    primitive(S(1028), ASSIGN_TOKS, EVERY_MATH_LOC);
    primitive(S(1029), ASSIGN_TOKS, EVERY_DISPLAY_LOC);
    primitive(S(1030), ASSIGN_TOKS, EVERY_HBOX_LOC);
    primitive(S(1031), ASSIGN_TOKS, EVERY_VBOX_LOC);
    primitive(S(1032), ASSIGN_TOKS, EVERY_JOB_LOC);
    primitive(S(1033), ASSIGN_TOKS, EVERY_CR_LOC);
    primitive(S(1034), ASSIGN_TOKS, ERR_HELP_LOC); /*:230*/
    /*238:*/
    primitive(S(1035), ASSIGN_INT, INT_BASE);
    primitive(S(1036), ASSIGN_INT, INT_BASE + tolerancecode);
    primitive(S(1037), ASSIGN_INT, INT_BASE + linepenaltycode);
    primitive(S(1038), ASSIGN_INT, INT_BASE + hyphenpenaltycode);
    primitive(S(1039), ASSIGN_INT, INT_BASE + exhyphenpenaltycode);
    primitive(S(1040), ASSIGN_INT, INT_BASE + clubpenaltycode);
    primitive(S(1041), ASSIGN_INT, INT_BASE + widowpenaltycode);
    primitive(S(1042), ASSIGN_INT, INT_BASE + displaywidowpenaltycode);
    primitive(S(1043), ASSIGN_INT, INT_BASE + brokenpenaltycode);
    primitive(S(1044), ASSIGN_INT, INT_BASE + binoppenaltycode);
    primitive(S(1045), ASSIGN_INT, INT_BASE + relpenaltycode);
    primitive(S(1046), ASSIGN_INT, INT_BASE + predisplaypenaltycode);
    primitive(S(1047), ASSIGN_INT, INT_BASE + postdisplaypenaltycode);
    primitive(S(1048), ASSIGN_INT, INT_BASE + interlinepenaltycode);
    primitive(S(1049), ASSIGN_INT, INT_BASE + doublehyphendemeritscode);
    primitive(S(1050), ASSIGN_INT, INT_BASE + finalhyphendemeritscode);
    primitive(S(1051), ASSIGN_INT, INT_BASE + adjdemeritscode);
    primitive(S(1052), ASSIGN_INT, INT_BASE + magcode);
    primitive(S(1053), ASSIGN_INT, INT_BASE + delimiterfactorcode);
    primitive(S(1054), ASSIGN_INT, INT_BASE + loosenesscode);
    primitive(S(1055), ASSIGN_INT, INT_BASE + timecode);
    primitive(S(1056), ASSIGN_INT, INT_BASE + daycode);
    primitive(S(1057), ASSIGN_INT, INT_BASE + monthcode);
    primitive(S(1058), ASSIGN_INT, INT_BASE + yearcode);
    primitive(S(1059), ASSIGN_INT, INT_BASE + showboxbreadthcode);
    primitive(S(1060), ASSIGN_INT, INT_BASE + showboxdepthcode);
    primitive(S(1061), ASSIGN_INT, INT_BASE + hbadnesscode);
    primitive(S(1062), ASSIGN_INT, INT_BASE + vbadnesscode);
    primitive(S(1063), ASSIGN_INT, INT_BASE + pausingcode);
    primitive(S(1064), ASSIGN_INT, INT_BASE + tracingonlinecode);
    primitive(S(1065), ASSIGN_INT, INT_BASE + tracingmacroscode);
    primitive(S(1066), ASSIGN_INT, INT_BASE + tracingstatscode);
    primitive(S(1067), ASSIGN_INT, INT_BASE + tracingparagraphscode);
    primitive(S(1068), ASSIGN_INT, INT_BASE + tracingpagescode);
    primitive(S(1069), ASSIGN_INT, INT_BASE + tracingoutputcode);
    primitive(S(1070), ASSIGN_INT, INT_BASE + tracinglostcharscode);
    primitive(S(1071), ASSIGN_INT, INT_BASE + tracingcommandscode);
    primitive(S(1072), ASSIGN_INT, INT_BASE + tracingrestorescode);
    primitive(S(1073), ASSIGN_INT, INT_BASE + uchyphcode);
    primitive(S(1074), ASSIGN_INT, INT_BASE + outputpenaltycode);
    primitive(S(1075), ASSIGN_INT, INT_BASE + maxdeadcyclescode);
    primitive(S(1076), ASSIGN_INT, INT_BASE + hangaftercode);
    primitive(S(1077), ASSIGN_INT, INT_BASE + floatingpenaltycode);
    primitive(S(1078), ASSIGN_INT, INT_BASE + globaldefscode);
    primitive(S(333), ASSIGN_INT, INT_BASE + curfamcode);
    primitive(S(1079), ASSIGN_INT, INT_BASE + ESCAPE_CHARcode);
    primitive(S(1080), ASSIGN_INT, INT_BASE + defaulthyphencharcode);
    primitive(S(1081), ASSIGN_INT, INT_BASE + defaultskewcharcode);
    primitive(S(1082), ASSIGN_INT, INT_BASE + endlinecharcode);
    primitive(S(1083), ASSIGN_INT, INT_BASE + newlinecharcode);
    primitive(S(1084), ASSIGN_INT, INT_BASE + languagecode);
    primitive(S(1085), ASSIGN_INT, INT_BASE + lefthyphenmincode);
    primitive(S(1086), ASSIGN_INT, INT_BASE + righthyphenmincode);
    primitive(S(1087), ASSIGN_INT, INT_BASE + holdinginsertscode);
    primitive(S(1088), ASSIGN_INT, INT_BASE + errorcontextlinescode);
    /*:238*/
    /*248:*/
    primitive(S(1089), ASSIGN_DIMEN, DIMEN_BASE);
    primitive(S(1090), ASSIGN_DIMEN, DIMEN_BASE + mathsurroundcode);
    primitive(S(1091), ASSIGN_DIMEN, DIMEN_BASE + lineskiplimitcode);
    primitive(S(1092), ASSIGN_DIMEN, DIMEN_BASE + hsizecode);
    primitive(S(1093), ASSIGN_DIMEN, DIMEN_BASE + vsizecode);
    primitive(S(1094), ASSIGN_DIMEN, DIMEN_BASE + maxdepthcode);
    primitive(S(1095), ASSIGN_DIMEN, DIMEN_BASE + splitmaxdepthcode);
    primitive(S(1096), ASSIGN_DIMEN, DIMEN_BASE + boxmaxdepthcode);
    primitive(S(1097), ASSIGN_DIMEN, DIMEN_BASE + hfuzzcode);
    primitive(S(1098), ASSIGN_DIMEN, DIMEN_BASE + vfuzzcode);
    primitive(S(1099), ASSIGN_DIMEN, DIMEN_BASE + delimitershortfallcode);
    primitive(S(1100), ASSIGN_DIMEN, DIMEN_BASE + nulldelimiterspacecode);
    primitive(S(1101), ASSIGN_DIMEN, DIMEN_BASE + scriptspacecode);
    primitive(S(1102), ASSIGN_DIMEN, DIMEN_BASE + predisplaysizecode);
    primitive(S(1103), ASSIGN_DIMEN, DIMEN_BASE + displaywidthcode);
    primitive(S(1104), ASSIGN_DIMEN, DIMEN_BASE + displayindentcode);
    primitive(S(1105), ASSIGN_DIMEN, DIMEN_BASE + overfullrulecode);
    primitive(S(1106), ASSIGN_DIMEN, DIMEN_BASE + hangindentcode);
    primitive(S(1107), ASSIGN_DIMEN, DIMEN_BASE + hoffsetcode);
    primitive(S(1108), ASSIGN_DIMEN, DIMEN_BASE + voffsetcode);
    primitive(S(1109), ASSIGN_DIMEN, DIMEN_BASE + emergencystretchcode);
    /*:248*/
    /*265:*/
    primitive(' ', EX_SPACE, 0);
    primitive('/', ITAL_CORR, 0);
    primitive(S(417), ACCENT, 0);
    primitive(S(1110), ADVANCE, 0);
    primitive(S(1111), AFTER_ASSIGNMENT, 0);
    primitive(S(1112), AFTER_GROUP, 0);
    primitive(S(1113), BEGIN_GROUP, 0);
    primitive(S(1114), CHAR_NUM, 0);
    primitive(S(262), CS_NAME, 0);
    primitive(S(1115), DELIM_NUM, 0);
    primitive(S(1116), DIVIDE, 0);
    primitive(S(263), END_CS_NAME, 0);
    primitive(S(836), END_GROUP, 0);
    set_text(FROZEN_END_GROUP, S(836));
    eqtb[FROZEN_END_GROUP] = eqtb[cur_val];
    primitive(S(1117), EXPAND_AFTER, 0);
    primitive(S(1118), DEF_FONT, 0);
    primitive(S(1119), ASSIGN_FONT_DIMEN, 0);
    primitive(S(724), HALIGN, 0);
    primitive(S(863), HRULE, 0);
    primitive(S(1120), IGNORE_SPACES, 0);
    primitive(S(374), INSERT, 0);
    primitive(S(402), MARK, 0);
    primitive(S(913), MATH_ACCENT, 0);
    primitive(S(1121), MATH_CHAR_NUM, 0);
    primitive(S(404), MATH_CHOICE, 0);
    primitive(S(1122), MULTIPLY, 0);
    primitive(S(897), NO_ALIGN, 0);
    primitive(S(1123), NO_BOUNDARY, 0);
    primitive(S(1124), NO_EXPAND, 0);
    primitive(S(388), NON_SCRIPT, 0);
    primitive(S(900), OMIT, 0);
    primitive(S(462), SET_SHAPE, 0);
    primitive(S(761), BREAK_PENALTY, 0);
    primitive(S(948), SET_PREV_GRAF, 0);
    primitive(S(416), RADICAL, 0);
    primitive(S(656), READ_TO_CS, 0);
    primitive(S(1125), RELAX, 256);
    set_text(FROZEN_RELAX, S(1125));
    eqtb[FROZEN_RELAX] = eqtb[cur_val];
    primitive(S(970), SET_BOX, 0);
    primitive(S(604), THE, 0);
    primitive(S(463), TOKS_REGISTER, 0);
    primitive(S(403), VADJUST, 0);
    primitive(S(1126), VALIGN, 0);
    primitive(S(415), VCENTER, 0);
    primitive(S(1127), VRULE, 0); /*:265*/
    /*334:*/
    primitive(S(760), PAR_END, 256);
    parloc = cur_val;
    partoken = CS_TOKEN_FLAG + parloc; /*:334*/
    /*376:*/
    primitive(S(1128), INPUT, 0);
    primitive(S(1129), INPUT, 1); /*:376*/
    /*384:*/
    primitive(S(1130), TOP_BOT_MARK, topmarkcode);
    primitive(S(1131), TOP_BOT_MARK, firstmarkcode);
    primitive(S(1132), TOP_BOT_MARK, botmarkcode);
    primitive(S(1133), TOP_BOT_MARK, splitfirstmarkcode);
    primitive(S(1134), TOP_BOT_MARK, splitbotmarkcode); /*:384*/
    /*411:*/
    primitive(S(472), REGISTER, INT_VAL);
    primitive(S(474), REGISTER, DIMEN_VAL);
    primitive(S(460), REGISTER, GLUE_VAL);
    primitive(S(461), REGISTER, MU_VAL); /*:411*/
    /*416:*/
    primitive(S(1135), SET_AUX, H_MODE);
    primitive(S(1136), SET_AUX, V_MODE);
    primitive(S(1137), SET_PAGE_INT, 0);
    primitive(S(1138), SET_PAGE_INT, 1);
    primitive(S(1139), SET_BOX_DIMEN, widthoffset);
    primitive(S(1140), SET_BOX_DIMEN, heightoffset);
    primitive(S(1141), SET_BOX_DIMEN, depthoffset);
    primitive(S(1142), LAST_ITEM, INT_VAL);
    primitive(S(1143), LAST_ITEM, DIMEN_VAL);
    primitive(S(1144), LAST_ITEM, GLUE_VAL);
    primitive(S(1145), LAST_ITEM, INPUT_LINE_NO_CODE);
    primitive(S(1146), LAST_ITEM, BADNESS_CODE); /*:416*/
    /*468:*/
    primitive(S(1147), CONVERT, numbercode);
    primitive(S(1148), CONVERT, romannumeralcode);
    primitive(S(1149), CONVERT, stringcode);
    primitive(S(1150), CONVERT, meaningcode);
    primitive(S(1151), CONVERT, fontnamecode);
    primitive(S(1152), CONVERT, jobnamecode); /*:468*/
    /*487:*/
    primitive(S(658), IF_TEST, IF_CHAR_CODE);
    primitive(S(1153), IF_TEST, IF_CAT_CODE);
    primitive(S(1154), IF_TEST, IF_INT_CODE);
    primitive(S(1155), IF_TEST, IF_DIM_CODE);
    primitive(S(1156), IF_TEST, IF_ODD_CODE);
    primitive(S(1157), IF_TEST, IF_VMODE_CODE);
    primitive(S(1158), IF_TEST, IF_HMODE_CODE);
    primitive(S(1159), IF_TEST, IF_MMODE_CODE);
    primitive(S(1160), IF_TEST, IF_INNER_CODE);
    primitive(S(1161), IF_TEST, IF_VOID_CODE);
    primitive(S(1162), IF_TEST, IF_HBOX_CODE);
    primitive(S(1163), IF_TEST, IF_VBOX_CODE);
    primitive(S(1164), IF_TEST, IF_X_CODE);
    primitive(S(1165), IF_TEST, IF_EOF_CODE);
    primitive(S(1166), IF_TEST, IF_TRUE_CODE);
    primitive(S(1167), IF_TEST, IF_FALSE_CODE);
    primitive(S(1168), IF_TEST, IF_CASE_CODE); /*:487*/
    /*491:*/
    primitive(S(1169), FI_OR_ELSE, ficode);
    set_text(FROZEN_FI, S(1169));
    eqtb[FROZEN_FI] = eqtb[cur_val];
    primitive(S(664), FI_OR_ELSE, orcode);
    primitive(S(1170), FI_OR_ELSE, elsecode); /*:491*/
    /*553:*/
    primitive(S(1171), SET_FONT, NULL_FONT);
    set_text(FROZEN_NULL_FONT, S(1171));
    eqtb[FROZEN_NULL_FONT] = eqtb[cur_val]; /*:553*/
    /*780:*/
    primitive(S(1172), TAB_MARK, spancode);
    primitive(S(737), CAR_RET, crcode);
    set_text(FROZEN_CR, S(737));
    eqtb[FROZEN_CR] = eqtb[cur_val];
    primitive(S(1173), CAR_RET, crcrcode);
    set_text(FROZEN_END_TEMPLATE, S(1174));
    set_text(FROZEN_ENDV, S(1174));
    eq_type(FROZEN_ENDV) = ENDV;
    equiv(FROZEN_ENDV) = nulllist;
    eq_level(FROZEN_ENDV) = LEVEL_ONE;
    eqtb[FROZEN_END_TEMPLATE] = eqtb[FROZEN_ENDV];
    eq_type(FROZEN_END_TEMPLATE) = END_TEMPLATE; /*:780*/
    /*983:*/
    primitive(S(1175), SET_PAGE_DIMEN, 0);
    primitive(S(1176), SET_PAGE_DIMEN, 1);
    primitive(S(1177), SET_PAGE_DIMEN, 2);
    primitive(S(1178), SET_PAGE_DIMEN, 3);
    primitive(S(1179), SET_PAGE_DIMEN, 4);
    primitive(S(1180), SET_PAGE_DIMEN, 5);
    primitive(S(1181), SET_PAGE_DIMEN, 6);
    primitive(S(1182), SET_PAGE_DIMEN, 7); /*:983*/
    /*1052:*/
    primitive(S(1183), STOP, 0);
    primitive(S(1184), STOP, 1); /*:1052*/
    /*1058:*/
    primitive(S(1185), HSKIP, SKIP_CODE);
    primitive(S(1186), HSKIP, FIL_CODE);
    primitive(S(1187), HSKIP, FILL_CODE);
    primitive(S(1188), HSKIP, SS_CODE);
    primitive(S(1189), HSKIP, FIL_NEG_CODE);
    primitive(S(1190), VSKIP, SKIP_CODE);
    primitive(S(1191), VSKIP, FIL_CODE);
    primitive(S(1192), VSKIP, FILL_CODE);
    primitive(S(1193), VSKIP, SS_CODE);
    primitive(S(1194), VSKIP, FIL_NEG_CODE);
    primitive(S(389), MSKIP, MSKIP_CODE);
    primitive(S(391), KERN, explicit);
    primitive(S(393), MKERN, muglue); /*:1058*/
    /*1071:*/
    primitive(S(1195), HMOVE, 1);
    primitive(S(1196), HMOVE, 0);
    primitive(S(1197), VMOVE, 1);
    primitive(S(1198), VMOVE, 0);
    primitive(S(464), MAKE_BOX, boxcode);
    primitive(S(1199), MAKE_BOX, copycode);
    primitive(S(1200), MAKE_BOX, lastboxcode);
    primitive(S(797), MAKE_BOX, vsplitcode);
    primitive(S(1201), MAKE_BOX, vtopcode);
    primitive(S(799), MAKE_BOX, vtopcode + V_MODE);
    primitive(S(1202), MAKE_BOX, vtopcode + H_MODE);
    primitive(S(1203), LEADER_SHIP, aleaders - 1);
    primitive(S(1204), LEADER_SHIP, aleaders);
    primitive(S(1205), LEADER_SHIP, cleaders);
    primitive(S(1206), LEADER_SHIP, xleaders); /*:1071*/
    /*1088:*/
    primitive(S(1207), START_PAR, 1);
    primitive(S(1208), START_PAR, 0); /*:1088*/
    /*1107:*/
    primitive(S(1209), REMOVE_ITEM, PENALTY_NODE);
    primitive(S(1210), REMOVE_ITEM, KERN_NODE);
    primitive(S(1211), REMOVE_ITEM, GLUE_NODE);
    primitive(S(1212), UN_HBOX, boxcode);
    primitive(S(1213), UN_HBOX, copycode);
    primitive(S(1214), UN_VBOX, boxcode);
    primitive(S(1215), UN_VBOX, copycode); /*:1107*/
    /*1114:*/
    primitive('-', DISCRETIONARY, 1);
    primitive(S(400), DISCRETIONARY, 0); /*:1114*/
    /*1141:*/
    primitive(S(1216), EQ_NO, 0);
    primitive(S(1217), EQ_NO, 1); /*:1141*/
    /*1156:*/
    primitive(S(405), MATH_COMP, ordnoad);
    primitive(S(406), MATH_COMP, opnoad);
    primitive(S(407), MATH_COMP, binnoad);
    primitive(S(408), MATH_COMP, relnoad);
    primitive(S(409), MATH_COMP, opennoad);
    primitive(S(410), MATH_COMP, closenoad);
    primitive(S(411), MATH_COMP, punctnoad);
    primitive(S(412), MATH_COMP, innernoad);
    primitive(S(414), MATH_COMP, undernoad);
    primitive(S(413), MATH_COMP, overnoad);
    primitive(S(1218), LIMIT_SWITCH, NORMAL);
    primitive(S(420), LIMIT_SWITCH, limits);
    primitive(S(421), LIMIT_SWITCH, nolimits); /*:1156*/
    /*1169:*/
    primitive(S(336), MATH_STYLE, displaystyle);
    primitive(S(337), MATH_STYLE, textstyle);
    primitive(S(338), MATH_STYLE, scriptstyle);
    primitive(S(339), MATH_STYLE, scriptscriptstyle); /*:1169*/
    /*1178:*/
    primitive(S(1219), ABOVE, abovecode);
    primitive(S(1220), ABOVE, overcode);
    primitive(S(1221), ABOVE, atopcode);
    primitive(S(1222), ABOVE, delimitedcode);
    primitive(S(1223), ABOVE, delimitedcode + overcode);
    primitive(S(1224), ABOVE, delimitedcode + atopcode); /*:1178*/
    /*1188:*/
    primitive(S(418), LEFT_RIGHT, leftnoad);
    primitive(S(419), LEFT_RIGHT, rightnoad);
    set_text(FROZEN_RIGHT, S(419));
    eqtb[FROZEN_RIGHT] = eqtb[cur_val]; /*:1188*/
    /*1208:*/
    primitive(S(959), PREFIX, 1);
    primitive(S(961), PREFIX, 2);
    primitive(S(1225), PREFIX, 4);
    primitive(S(1226), DEF, 0);
    primitive(S(1227), DEF, 1);
    primitive(S(1228), DEF, 2);
    primitive(S(1229), DEF, 3);
    /*:1208*/
    /*1219:*/
    primitive(S(1230), LET, NORMAL);
    primitive(S(1231), LET, NORMAL + 1); /*:1219*/
    /*1222:*/
    primitive(S(1232), SHORTHAND_DEF, chardefcode);
    primitive(S(1233), SHORTHAND_DEF, mathchardefcode);
    primitive(S(1234), SHORTHAND_DEF, countdefcode);
    primitive(S(1235), SHORTHAND_DEF, dimendefcode);
    primitive(S(1236), SHORTHAND_DEF, skipdefcode);
    primitive(S(1237), SHORTHAND_DEF, muskipdefcode);
    primitive(S(1238), SHORTHAND_DEF, toksdefcode); /*:1222*/
    /*1230:*/
    primitive(S(467), DEF_CODE, CAT_CODE_BASE);
    primitive(S(471), DEF_CODE, MATH_CODE_BASE);
    primitive(S(468), DEF_CODE, LC_CODE_BASE);
    primitive(S(469), DEF_CODE, UC_CODE_BASE);
    primitive(S(470), DEF_CODE, SF_CODE_BASE);
    primitive(S(473), DEF_CODE, DEL_CODE_BASE);
    primitive(S(266), DEF_FAMILY, MATH_FONT_BASE);
    primitive(S(267), DEF_FAMILY, MATH_FONT_BASE + SCRIPT_SIZE);
    primitive(S(268), DEF_FAMILY, MATH_FONT_BASE + SCRIPT_SCRIPT_SIZE);
    /*:1230*/
    /*1250:*/
    primitive(S(787), HYPH_DATA, 0);
    primitive(S(774), HYPH_DATA, 1); /*:1250*/
    /*1254:*/
    primitive(S(1239), ASSIGN_FONT_INT, 0);
    primitive(S(1240), ASSIGN_FONT_INT, 1); /*:1254*/
    /*1262:*/
    primitive(S(281), SET_INTERACTION, BATCH_MODE);
    primitive(S(282), SET_INTERACTION, NON_STOP_MODE);
    primitive(S(283), SET_INTERACTION, SCROLL_MODE);
    primitive(S(1241), SET_INTERACTION, ERROR_STOP_MODE); /*:1262*/
    /*1272:*/
    primitive(S(1242), IN_STREAM, 1);
    primitive(S(1243), IN_STREAM, 0); /*:1272*/
    /*1277:*/
    primitive(S(1244), MESSAGE, 0);
    primitive(S(1245), MESSAGE, 1); /*:1277*/
    /*1286:*/
    primitive(S(1246), CASE_SHIFT, LC_CODE_BASE);
    primitive(S(1247), CASE_SHIFT, UC_CODE_BASE); /*:1286*/
    /*1291:*/
    primitive(S(1248), XRAY, showcode);
    primitive(S(1249), XRAY, showboxcode);
    primitive(S(1250), XRAY, showthecode);
    primitive(S(1251), XRAY, showlists); /*:1291*/
    /*1344:*/
    primitive(S(378), EXTENSION, opennode);
    primitive(S(379), EXTENSION, writenode);
    writeloc = cur_val;
    primitive(S(380), EXTENSION, closenode);
    primitive(S(381), EXTENSION, specialnode);
    primitive(S(1252), EXTENSION, immediatecode);
    primitive(S(382), EXTENSION, setlanguagecode); /*:1344*/
} // #1336: init_prim
#endif // #1336: tt_INIT
/** @}*/ // end group S1330x1337_P465x469


/* ----------------------------------------------------------------------------
 * main 函数及其辅助函数(从 main 中拆分的函数)
 *
 *  + int main(int argc, char* argv[])
 *      + Integer S14_Check_the_constant_values_for_consistency(void)
 *      + void S55_Initialize_the_output_routines(void)
 *      + Boolean S1337_Get_the_first_line_of_input_and_prepare_to_start(void)
 */

// [p8#14] 检查常量范围是否正确。
// 有误则返回错误代码 bad
// Check the "constant" values for consistency
// xref: [14], 111, 290, 522, 1249, used in 1332@main
static Integer S14_Check_the_constant_values_for_consistency(void) {
    // bad: is some “constant” wrong?
    // xref: [13], 14, 111, 290, 522, 1249, 1332
    Integer bad = 0;

    /// [#14]
    if (    HALF_ERROR_LINE < 30 
        ||  HALF_ERROR_LINE > (ERROR_LINE - 15)
    ) bad = 1;
    if (MAX_PRINT_LINE < 60) bad = 2;
    if ((DVI_BUF_SIZE % 8) != 0) bad = 3;
    if ((MEM_BOT + 1100) > MEM_TOP) bad = 4;
    if (HASH_PRIME > HASH_SIZE) bad = 5;
    if (MAX_IN_OPEN >= 128) bad = 6;
    if (MEM_TOP < (256 + 11)) bad = 7; // we will want null list > 255

    /// [#111]
    #ifdef tt_INIT
        if (MEM_MIN != MEM_BOT || MEM_MAX != MEM_TOP) bad = 10;
    #endif // #111: tt_INIT
    if (MEM_MIN > MEM_BOT || MEM_MAX < MEM_TOP) bad = 10;
    if (MIN_QUARTER_WORD > 0 || MAX_QUARTER_WORD < 127) bad = 11;
    if (MIN_HALF_WORD > 0 || MAX_HALF_WORD < 32767) bad = 12;
    if (    MIN_QUARTER_WORD < MIN_HALF_WORD 
        ||  MAX_QUARTER_WORD > MAX_HALF_WORD
    ) bad = 13;
    if (    MEM_MIN <  MIN_HALF_WORD
        ||  MEM_MAX >= MAX_HALF_WORD 
        || (MEM_BOT - MEM_MIN) > (MAX_HALF_WORD + 1)
    ) bad = 14;
    if (FONT_BASE < MIN_QUARTER_WORD || FONT_MAX > MAX_QUARTER_WORD) bad = 15;
    if (FONT_MAX > FONT_BASE + 256) bad = 16;
    if (SAVE_SIZE > MAX_HALF_WORD || MAX_STRINGS > MAX_HALF_WORD) bad = 17;
    if (BUF_SIZE > MAX_HALF_WORD) bad = 18;
    if ((MAX_QUARTER_WORD - MIN_QUARTER_WORD) < 255) bad = 19;

    /// [#290]
    if ((CS_TOKEN_FLAG + UNDEFINED_CONTROL_SEQUENCE) > MAX_HALF_WORD) bad = 21;
    /// [#522]
    if (FORMAT_DEFAULT_LENGTH > FILE_NAME_SIZE) bad = 31;
    /// [#1249]
    if ((2 * MAX_HALF_WORD) < (MEM_TOP - MEM_MIN)) bad = 41;

    return bad;
} // S14_Check_the_constant_values_for_consistency

// [p24#55]: <Initialize the output routines> 
// [55], 61, 528, 533 Used in section 1332@main.
static void S55_Initialize_the_output_routines(void) {
    // [#55. #61]
    print_mod_init();

    // #528
    job_name = 0;
    name_in_progress = false;
    log_opened = false;

    // #533
    output_file_name = 0;
} // S55_Initialize_the_output_routines


/** @addtogroup S1330x1337_P465x469
 * @{
 */
// [p469#1337]: Get the first line of input and prepare to START
// return has_error?
static Boolean S1337_Get_the_first_line_of_input_and_prepare_to_start(void) {
    const Boolean HAS_ERROR = true, NO_ERROR = false;

    // [#331]: Initialize the input routines
    if (init_lexer()==HAS_ERROR) return HAS_ERROR;

    /// [#1337]
    // TODO: check if
    if (    need_to_load_format 
        || (format_ident == 0) 
        || (buffer[LOC] == '&')
        ) {
        if (format_ident != 0) initialize(); // erase preloaded format
        if (!open_fmt_file()) return HAS_ERROR;
        if (!load_fmt_file()) {
            w_close(&fmt_file);
            return HAS_ERROR;
        }
        w_close(&fmt_file);
        while ((LOC < LIMIT) && (buffer[LOC] == ' '))
            LOC++;
    }
    if (end_line_char_inactive) {
        LIMIT--;
    } else {
        buffer[LIMIT] = end_line_char;
    }
    fix_date_and_time(&tex_time, &day, &month, &year);

    /// [#765]: Compute the magic offset
    // _NOT_USE_
    // ??? magic offset ← str start[math spacing] − 9 ∗ ord noad

    error_selector_init();

    /// [#1337]
    if ((LOC < LIMIT) && (cat_code(buffer[LOC]) != ESCAPE)) {
        start_input(); // \input assumed
    }

    return NO_ERROR;
} // S1337_Get_the_first_line_of_input_and_prepare_to_start

/// 入口函数
/// [p466#1332]: TeX starts and ends here.
int main(int argc, char* argv[]) {
    // #13: a sacrifice of purity for economy
    // TODO: remove this
    Integer ready_already = 0;
    Integer bad = 0;
    Boolean has_error = false;

    // 附加的初始化
    PASCAL_MAIN(argc, argv);
    if (setjmp(_JMP_global__end_of_TEX)) goto _LN_main__end_of_TEX;
    if (setjmp(_JMP_global__final_end)) goto _LN_main__final_end;

    /// [#1332]: TeX start here
    history = FATAL_ERROR_STOP; // in case we quit during initialization
    // t_open_out; // 无需打开输出流，直接使用 stdout
    if (ready_already == 314159L) goto _LN_main__start_of_TEX;
    // 常量范围检查
    bad = S14_Check_the_constant_values_for_consistency();
    if (bad > 0) {
        fprintf(
            TERM_OUT,
            "Ouch---my internal constants have been clobbered!---case %lld\n",
            bad);
        goto _LN_main__final_end;
    }

    initialize(); // set global variables to their starting values
    #ifdef tt_INIT
        if (!get_strings_started()) goto _LN_main__final_end;
        init_prim(); // call primitive for each primitive
        str_set_init_ptrs(); // @str.c
        fix_date_and_time(&tex_time, &day, &month, &year); // @func.c
    #endif // #1332: tt_INIT
    ready_already = 314159L;

_LN_main__start_of_TEX:
    S55_Initialize_the_output_routines();
    has_error = S1337_Get_the_first_line_of_input_and_prepare_to_start();
    if (has_error) goto _LN_main__final_end;
    // #1332
    history = SPOTLESS; // ready to go!
    main_control();     // come to life
    final_cleanup();    // prepare for death

_LN_main__end_of_TEX:
    close_files_and_terminate();

_LN_main__final_end:
    ready_already = 0;
    exit(EXIT_SUCCESS);
} /* main */
/** @}*/ // end group S1330x1337_P465x469

/* End. */