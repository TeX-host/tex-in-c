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
    ASCIICode c = '\0';

    if (history < ERROR_MESSAGE_ISSUED) history = ERROR_MESSAGE_ISSUED;
    print_char('.');
    showcontext();

    if (interaction == ERROR_STOP_MODE) {
        // [#83] Get user’s advice and return
        while (true) {
            clearforerrorprompt();
            #ifndef USE_REAL_STR
                print(S(269)); // "? "
            #else
                print_str("? ");
            #endif
            term_input();
            if (last == first) return;

            c = buffer[first];
            if (c >= 'a') c += 'A' - 'a';
            switch (c) {
                case '0': case '1':
                case '2': case '3':
                case '4': case '5':
                case '6': case '7':
                case '8': case '9': {
                    if (!deletions_allowed) break;

                    // [#88] Delete c - "0" tokens and goto continue.
                    HalfWord s1 = curtok;
                    EightBits s2 = curcmd;
                    HalfWord s3 = curchr;
                    Integer s4 = align_state;
                    align_state = 1000000L;
                    OK_to_interrupt = false;

                    if (    (last > (first + 1)) 
                        &&  (buffer[first + 1] >= '0') 
                        &&  (buffer[first + 1] <= '9')) {
                        // c = c * 10 + buffer[first + 1] - '0' * 11;
                        c = (c - '0') * 10 + (buffer[first + 1] - '0');
                    } else {
                        c = c - '0';
                    }
                    while (c > 0) {
                        gettoken();
                        c--;
                    }

                    curtok = s1;
                    curcmd = s2;
                    curchr = s3;
                    align_state = s4;
                    OK_to_interrupt = true;
                    // "I have just deleted some text as you asked."
                    // "You can now delete more or insert or whatever." 
                    help2(S(270), S(271));
                    showcontext();
                    continue; // [#88]
                } // case: [0-9]

                case 'D':
                    #ifdef tt_DEBUG
                        debughelp();
                        continue;
                    #endif // #84: tt_DEBUG
                    break; // case: [D]

                case 'E': {
                    if (baseptr > 0) break;

                    #ifndef USE_REAL_STR
                        printnl(S(272));
                        slow_print(inputstack[baseptr].namefield);
                        print(S(273));
                        print_int(line);
                    #else
                        printnl_str("You want to edit file ");
                        slow_print(inputstack[baseptr].namefield);
                        printnl_str(" at line ");
                        print_int(line);
                    #endif // USE_REAL_STR

                    interaction = SCROLL_MODE;
                    jumpout();
                } // case: [E]

                case 'H': {
                    // [#89] Print the help information and goto continue.
                    if (use_err_help) {
                        giveerrhelp();
                        use_err_help = false;
                    } else {
                        if (help_ptr == 0) {
                            // "Sorry I don't know how to help in this situation."
                            // "Maybe you should try asking a human?"
                            help2(S(274), S(275));
                        }
                        do {
                            help_ptr--;
                            print(help_line[help_ptr]);
                            println();
                        } while (help_ptr != 0);
                    } // if (<> use_err_help)

                    /* 
                     * "Sorry I already gave what help I could..."
                     * "Maybe you should try asking a human?"
                     * "An error might have occurred before I noticed any problems."
                     * "``If all else fails read the instructions.''"
                     */
                    help4(S(276), S(275), S(277), S(278));
                    continue;
                } // case: [H]

                case 'I': {
                    // [#87] Introduce new material from the terminal and return.

                    // enter a new syntactic level for terminal input
                    // now `state = mid_line`, 
                    // so an initial blank space will count as a blank
                    beginfilereading();
                    if (last > first + 1) {
                        LOC = first + 1;
                        buffer[first] = ' ';
                    } else {
                        #ifndef USE_REAL_STR
                            print(S(279));
                        #else
                            print_str("insert>");
                        #endif // USE_REAL_STR
                        term_input();
                        LOC = first;
                    }
                    first = last;
                    // no end_line_char ends this line
                    cur_input.limitfield = last - 1; 
                    return;
                } // case: [I]

                case 'Q':
                case 'R':
                case 'S': {
                    // [#86] Change the interaction level and return.
                    errorcount = 0;
                    interaction = BATCH_MODE + (c - 'Q');

                    #ifndef USE_REAL_STR
                        print(S(280)); // "OK entering "
                        switch (c) {
                            // "batchmode"
                            case 'Q': print_esc(S(281)); selector--; break; 
                            case 'R': print_esc(S(282)); break; // "nonstopmode"
                            case 'S': print_esc(S(283)); break; // "scrollmode"
                        }
                        print(S(284)); // "..."
                    #else
                        print_str("OK entering ");
                        switch (c) {
                            case 'Q': print_esc_str("batchmode");
                                selector--; break;
                            case 'R': print_esc_str("nonstopmode"); break;
                            case 'S': print_esc_str("scrollmode"); break;
                        }
                        print_str("..."); 
                    #endif // USE_REAL_STR
            
                    println();
                    fflush(stdout);
                    return;
                } // [#86] case: [QRS]

                case 'X':
                    interaction = SCROLL_MODE;
                    jumpout();

            } // [#84] switch (c)

            // [#85] Print the menu of available options.
            #ifndef USE_REAL_STR
                /*
                * "Type <return> to proceed S to scroll future error messages"
                * "R to run without stopping Q to run quietly"
                * "I to insert something "
                */
                print(S(285));
                printnl(S(286));
                printnl(S(287));
                // "E to edit your file"
                if (baseptr > 0) print(S(288));
                //"1 or ... or 9 to ignore the next 1 to 9 tokens of input"
                if (deletions_allowed) printnl(S(289));
                printnl(S(290)); // "H for help X to quit."
            #else
                print_str("Type <return> to proceed S to scroll future error messages");
                printnl_str("R to run without stopping Q to run quietly");
                printnl_str("I to insert something ");
                if (baseptr > 0) print_str("E to edit your file");
                if (deletions_allowed) 
                    printnl_str("1 or ... or 9 to ignore the next 1 to 9 tokens of input");
                printnl_str("H for help X to quit.");
            #endif // USE_REAL_STR
        } // while (true)
    } // if (interaction == ERROR_STOP_MODE)

    errorcount++;
    if (errorcount == 100) {
        #ifndef USE_REAL_STR
            printnl(S(291));
        #else
            printnl_str("(That makes 100 errors; please try again.)");
        #endif // USE_REAL_STR
        history = FATAL_ERROR_STOP;
        jumpout();
    }

    // [#90] Put help message on the transcript file
    if (interaction > BATCH_MODE) selector--; // avoid terminal output
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

    // re-enable terminal output
    if (interaction > BATCH_MODE) selector++;
    println();
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