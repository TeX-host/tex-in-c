#include <stdarg.h>    // va_start, va_arg, va_end,
#include "tex_types.h" // [types] StrNumber, Integer
#include "global.h"
#include "texfunc.h"
#include "macros.h" // [macro] help4,
#include "print.h"  // term_input
#include "io.h"     // [func] update_terminal
#include "debug.h"
#include "error.h"


/** @addtogroup S72x98_P30x37
 * @{
 */
/// [p30#73] current level of #interaction.
/// #interaction = [BATCH_MODE=0, ERROR_STOP_MODE=3]
///
/// [REPORTING ERRORS]
UChar interaction;

/// [#76] is it safe for #error to call #gettoken?
Boolean deletions_allowed;
/// [#76] is it safe to do a `\setbox` assignment?
Boolean set_box_allowed;
/// [#76] has the source input been clean so far?
/// [SPOTLESS, FATAL_ERROR_STOP]
ErrorLevel history;
/// [#76] the number of scrolled errors since the last paragraph ended.
/// errorcount = [-1, 100]
SChar errorcount;

/// [#79] helps for the next #error.
StrNumber help_line[6];
/// [#79] the number of help lines present.
UChar help_ptr;
/// [#79] should the #errhelp list be shown?
Boolean use_err_help;

/// [#96] should TeX pause for instructions?
Integer interrupt;
/// [#96] should interrupts be observed?
Boolean OK_to_interrupt;


/// [#74, 77, 80, 97] Set initial values of key variables.
void error_init() {
    /*74:*/
    interaction = ERROR_STOP_MODE;
    /*77:*/
    deletions_allowed = true;
    set_box_allowed = true;
    errorcount = 0;
    /*80:*/
    help_ptr = 0;
    use_err_help = false;
    /*97:*/
    interrupt = 0;
    OK_to_interrupt = true;
} /* error_init */

/// [p30#75]: Initialize the print #selector based on #interaction.
void error_selector_init() {
    if (interaction == BATCH_MODE) {
        selector = NO_PRINT;
    } else {
        selector = TERM_ONLY;
    }
} /* error_selector_init */

// [#73] 打印错误信息，以 `! ` 开头.
inline void print_err(StrNumber s) {
    printnl(S(292)); // "! "
    print(s);
} // [#73]

// [#73] 打印错误信息，以 `! ` 开头.
inline void print_err_str(Str s) {
    printnl_str("! ");
    print_str(s);
} // [#73]

// [#79]
void set_help(SChar k, ...) {
    va_list ap;
    va_start(ap, k);
    help_ptr = k;
    k--;
    while (k >= 0) {
        help_line[k] = va_arg(ap, uint_fast32_t);
        k--;
    }
    va_end(ap);
}

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
                     * (276) "Sorry I already gave what help I could..."
                     * (275) "Maybe you should try asking a human?"
                     * (277) "An error might have occurred before I noticed any problems."
                     * (278) "``If all else fails read the instructions.''"
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
                                // NO terminal output
                                //  TERM_ONLY => NO_PRINT
                                //  TERM_AND_LOG => LOG_ONLY
                                selector--; break;
                            case 'R': print_esc_str("nonstopmode"); break;
                            case 'S': print_esc_str("scrollmode"); break;
                        } 
                        print_str("..."); 
                    #endif // USE_REAL_STR
            
                    println();
                    update_terminal();
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

    /* [#90] Put help message on the transcript file */
    // avoid terminal output
    //  TERM_ONLY => NO_PRINT
    //  TERM_AND_LOG => LOG_ONLY
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

    // re-enable terminal output
    //  NO_PRINT => TERM_ONLY
    //  LOG_ONLY => TERM_AND_LOG
    if (interaction > BATCH_MODE) selector++;
    println();
} // #82: error

/// [#91] error messages end with a parenthesized integer.
void int_error(Integer n) {
#ifndef USE_REAL_STR
    print(S(303)); // " ("
    print_int(n);
    print_char(')');
#else
    print_str(" (");
    print_int(n);
    print_char(')');
#endif // USE_REAL_STR
    error();
} // [#91] int_error

/// [#92] fix selector state just enough to keep running a bit longer.
///
/// used in anomalous cases.
///     + fatalerror
///     + overflow
///     + confusion
void normalize_selector(void) {
    if (log_opened) {
        selector = TERM_AND_LOG;
    } else {
        selector = TERM_ONLY;
    }

    // not open log file
    if (job_name == 0) openlogfile();

    // NO terminal output
    //  TERM_ONLY => NO_PRINT
    //  TERM_AND_LOG => LOG_ONLY
    if (interaction == BATCH_MODE) selector--;
} /* [#92] normalize_selector */

/// [#93]: prints TEX’s last words before dying.
void succumb(void) {
    // no more interaction
    if (interaction == ERROR_STOP_MODE) interaction = SCROLL_MODE;
    if (log_opened) error();
#ifdef tt_DEBUG
    if (interaction > BATCH_MODE) debughelp();
#endif // #93: tt_DEBUG
    history = FATAL_ERROR_STOP;
    jumpout(); // irrecoverable error
} // #93: succumb

/// [#93] prints `s`, and that’s it.
void fatalerror(StrNumber s) {
    normalize_selector();
#ifndef USE_REAL_STR
    print_err(S(293)); // "Emergency stop"
#else
    print_err_str("Emergency stop");
#endif // USE_REAL_STR
    help_ptr = 1;
    help_line[0] = s;
    succumb();
} // #93: fatalerror

/// [#94] stop due to finiteness.
void overflow(StrNumber s, Integer n) {
    normalize_selector();

#ifndef USE_REAL_STR
    print_err(S(294)); // "TeX capacity exceeded, sorry ["
#else
    print_err_str("TeX capacity exceeded, sorry [");
#endif // USE_REAL_STR

    print(s);
    print_char('=');
    print_int(n);
    print_char(']');
    // "If you really absolutely need more capacity,"
    // "you can ask a wizard to enlarge me."
    help2(S(295), S(296));
    succumb();
} // #94: overflow

/// [#95] consistency check violated; s tells where.
void confusion(StrNumber s) {
    normalize_selector();
    if (history < ERROR_MESSAGE_ISSUED) {
    #ifndef USE_REAL_STR
        print_err(S(297)); // "This can´t happen ("
    #else
        print_err_str("This can´t happen (");
    #endif // USE_REAL_STR
        print(s);
        print_char(')');
        // "I´m broken. Please show this to someone who can fix can fix"
        help1(S(298));
    } else {
    #ifndef USE_REAL_STR
        print_err(S(299)); // "I can´t go on meeting you like this"
    #else
        print_err_str("I can´t go on meeting you like this");
    #endif // USE_REAL_STR
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

    // Open terminal output
    //  NO_PRINT => TERM_ONLY
    //  LOG_ONLY => TERM_AND_LOG
    if (selector == LOG_ONLY || selector == NO_PRINT) selector++;

#ifndef USE_REAL_STR
    print_err(S(304)); // "Interruption"
#else
    print_err_str("Interruption");
#endif // USE_REAL_STR

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