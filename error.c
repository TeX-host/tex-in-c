#include "tex_types.h"
#include "global.h"
#include "texfunc.h"
#include "macros.h"
#include "print.h" // 
#include "error.h"


/** @addtogroup S72x98_P30x37
 * @{
 */

/*
 * Error handling procedures
 *
 * xref[4]: 81, 93, 94, 95, 1304
 */

/// [#82]: completes the job of error reporting.
void error(void) {
    ASCIICode c;
    if (history < ERROR_MESSAGE_ISSUED) history = ERROR_MESSAGE_ISSUED;
    print_char('.');
    showcontext();
    if (interaction == ERROR_STOP_MODE) { /*83:*/
        while (true) {                    /*:83*/
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
                    if (deletions_allowed) { /*88:*/
                        long s1 = curtok;
                        long s2 = curcmd;
                        long s3 = curchr;
                        long s4 = align_state;
                        align_state = 1000000L;
                        OK_to_interrupt = false;
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
                        align_state = s4;
                        OK_to_interrupt = true;
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
                        interaction = SCROLL_MODE;
                        jumpout();
                    }
                    break;

                case 'H': /*89:*/
                    if (use_err_help) {
                        giveerrhelp();
                        use_err_help = false;
                    } else {
                        if (help_ptr == 0) {
                            help2(S(274), S(275));
                        }
                        do {
                            help_ptr--;
                            print(help_line[help_ptr]);
                            println();
                        } while (help_ptr != 0);
                    }
                    help4(S(276), S(275), S(277), S(278));
                    goto _Llabcontinue;
                    break;
                    /*:89*/

                case 'I': /*87:*/
                    beginfilereading();
                    if (last > first + 1) {
                        LOC = first + 1;
                        buffer[first] = ' ';
                    } else {
                        print(S(279));
                        term_input();
                        LOC = first;
                    }
                    first = last;
                    cur_input.limitfield = last - 1;
                    goto _Lexit;
                    break;
                    /*:87*/

                case 'Q':
                case 'R':
                case 'S': /*86:*/
                    errorcount = 0;
                    interaction = BATCH_MODE + c - 'Q';
                    print(S(280));
                    switch (c) {
                        case 'Q':
                            print_esc(S(281));
                            selector--;
                            break;

                        case 'R': print_esc(S(282)); break;

                        case 'S': print_esc(S(283)); break;
                    }
                    print(S(284));
                    println();
                    fflush(stdout);
                    goto _Lexit;
                    break;
                    /*:86*/

                case 'X':
                    interaction = SCROLL_MODE;
                    jumpout();
                    break;
            } // switch (c)

            print(S(285));
            printnl(S(286));
            printnl(S(287));
            if (baseptr > 0) print(S(288));
            if (deletions_allowed) printnl(S(289));
            printnl(S(290)); /*:85*/
            /*:84*/
        } // while (true)
    }     // if (interaction == ERROR_STOP_MODE)

    errorcount++;
    if (errorcount == 100) {
        printnl(S(291));
        history = FATAL_ERROR_STOP;
        jumpout();
    } /*90:*/
    if (interaction > BATCH_MODE) selector--;
    if (use_err_help) {
        println();
        giveerrhelp();
    } else {
        while (help_ptr > 0) {
            help_ptr--;
            printnl(help_line[help_ptr]);
        }
    }
    println();
    if (interaction > BATCH_MODE) /*:90*/
        selector++;
    println();

_Lexit:;
} // #82: error

/// [#91] error messages end with a parenthesized integer.
void int_error(long n) {
    print(S(303)); // " ("
    print_int(n);
    print_char(')');
    error();
} // [#91] int_error

/// [#92]
void normalize_selector(void) {
    if (log_opened) {
        selector = TERM_AND_LOG;
    } else {
        selector = TERM_ONLY;
    }
    if (job_name == 0) openlogfile();
    if (interaction == BATCH_MODE) selector--;
} // [#92] normalize_selector

/// [#93]: prints TEX’s last words before dying.
void succumb(void) {
    if (interaction == ERROR_STOP_MODE) interaction = SCROLL_MODE;
    if (log_opened) error();
#ifdef tt_DEBUG
    if (interaction > BATCH_MODE) debughelp();
#endif // #93: tt_DEBUG
    history = FATAL_ERROR_STOP;
    jumpout();
} // #93: succumb

/// [#93] prints `s`, and that’s it.
void fatalerror(StrNumber s) {
    normalize_selector();
    printnl(S(292));
    print(S(293)); // "Emergency stop"
    help_ptr = 1;
    help_line[0] = s;
    succumb();
} // #93: fatalerror

/// [#94] stop due to finiteness.
void overflow(StrNumber s, Integer n) {
    normalize_selector();
    printnl(S(292));
    print(S(294)); // "TeX capacity exceeded, sorry ["
    print(s);
    print_char('=');
    print_int(n);
    print_char(']');
    // "If you really absolutely need more capacity,"
    // "you can ask a wizard to enlarge me."
    help2(S(295), S(296));
    succumb();
} // #94: overflow

/// [#95]  consistency check violated; s tells where.
void confusion(StrNumber s) {
    normalize_selector();
    if (history < ERROR_MESSAGE_ISSUED) {
        printnl(S(292));
        print(S(297)); // "This can´t happen ("
        print(s);
        print_char(')');
        // "I´m broken. Please show this to someone who can fix can fix"
        help1(S(298));
    } else {
        printnl(S(292));
        print(S(299)); // "I can´t go on meeting you like this"
        // "One of your faux pas seems to have wounded me deeply..."
        // "in fact, I´m barely conscious. Please fix it and try again."
        help2(S(300), S(301));
    }
    succumb();
} // #95: confusion

/// [#98]
void pause_for_instructions(void) {
    if (!OK_to_interrupt) return;

    interaction = ERROR_STOP_MODE;
    if (selector == LOG_ONLY || selector == NO_PRINT) selector++;

    printnl(S(292));
    print(S(304)); // "Interruption"
    // "You rang?"
    // "Try to insert some instructions for me (e.g.,`I\showlists´),"
    // "unless you just want to quit by typing `X´."
    help3(S(305), S(306), S(307));
    deletions_allowed = false;
    error();
    deletions_allowed = true;
    interrupt = 0;
} // [#98] pause_for_instructions

/** @}*/ // end group S72x98_P30x37