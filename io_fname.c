#include <string.h> // [func] memcpy
#include "tex_constant.h" // [const] TEX_BANNER
#include "charset.h" // [var] xord, xchr
#include "str.h"
#include "print.h"  // [type] Selector [var] selector
#include "eqtb.h"   // [var] day, month, year, tex_time
#include "expand.h" // [func] get_x_token
#include "error.h"  // [func] print_err [var] interaction
#include "lexer.h"  // [macro] end_line_char_inactive
#include "dump.h"   // [var] format_ident
#include "scan.h"   // [func] skip_spaces
#include "io.h"


/** @addtogroup S511x538_P188x195
 * @{
 */
/*512:*/
StrNumber curname, curarea, curext;
/*:512*/
/*513:*/
StrNumber extdelimiter;
/*:513*/

/// [#520] _not_use_
Char TEXformatdefault[FORMAT_DEFAULT_LENGTH];

/// [#527] is a file name being scanned?
/// xref: scanfilename, newfont, expand
Boolean name_in_progress;
/// [#527] principal file name.
/// contains the file name that was first `\\input` by the user.
/// This name is extended by `.log` and `.dvi` and `.fmt`
///     in the names of TEX’s output files.
StrNumber job_name;
/// [#527] has the transcript file been opened?
Boolean log_opened;

/*532:*/
StrNumber output_file_name, logname;
/*:532*/


/// [ #511. File names. ] 

/// [#521]
void fname_init() {
    memcpy(TEXformatdefault, "TeXformats:plain.fmt", FORMAT_DEFAULT_LENGTH);
}

/*515:*/
void beginname(void) { extdelimiter = 0; }
/*:515*/

/*516:*/
Boolean morename(ASCIICode c) {
    if (c == ' ') {
        return false;
    } else {
        str_room(1);
        if (c == '.' && extdelimiter == 0) {
            extdelimiter = makestring();
        }
        append_char(c);
        return true;
    }
}
/*:516*/

/*517:*/
void endname(void) {
    curarea = S(385);
    if (extdelimiter == 0) {
        curext = S(385);
        curname = makestring();
    } else {
        curname = extdelimiter;
        curext = makestring();
    }
}

Integer _tmp_fname_len;
void appendtoname(ASCIICode x) {
    _tmp_fname_len++;
    if (_tmp_fname_len <= FILE_NAME_SIZE) {
        name_of_file[_tmp_fname_len - 1] = xchr[x];
    }
}
/*:517*/

/// #518
void print_file_name(StrNumber n, StrNumber a, StrNumber e) {
    slow_print(a);
    slow_print(n);
    slow_print(e);
} // #518: print_file_name

/// [#519] 打包文件名.
/// @param[in] fname    文件名
/// @param[in] prefix   前缀
/// @param[in] ext      后缀
void packfilename(StrNumber fname, StrNumber prefix, StrNumber ext) {
    Integer k;

    k = 0;
    _tmp_fname_len = 0;
    /// TODO: 让 str_map 返回打印的字符个数。消除 _tmp_fname_len
    str_map(prefix, appendtoname);
    str_map(fname, appendtoname);
    str_map(ext, appendtoname);

    k = _tmp_fname_len;
    if (k <= FILE_NAME_SIZE) {
        namelength = k;
    } else {
        namelength = FILE_NAME_SIZE;
    }

    /// MOD: 直接中止字符串
    name_of_file[k] = '\0';
    // for (k = namelength; k < FILE_NAME_SIZE; k++) {
    //     name_of_file[k] = ' ';
    // }
} /* [#519] packfilename */

/*525:*/
StrNumber makenamestring(void) {
    int k;
    for (k = 0; k < namelength; k++) {
        append_char(xord[name_of_file[k]]);
    }
    return (makestring());
}

StrNumber amakenamestring(void) { return (makenamestring()); }
StrNumber bmakenamestring(void) { return (makenamestring()); }
StrNumber wmakenamestring(void) { return (makenamestring()); }
/*:525*/

/*526:*/
void scanfilename(void) {
    name_in_progress = true;
    beginname();
    skip_spaces();
    while (true) {
        if (curcmd > OTHER_CHAR || curchr > 255) {
            backinput();
            break;
        }
        if (!morename(curchr)) break;
        get_x_token();
    }
    endname();
    name_in_progress = false;
}
/*:526*/

/*529:*/
void packjobname(StrNumber s) {
    curarea = S(385); // ""
    curext = s;
    curname = job_name;
    packfilename(curname, curarea, curext);
}
/*:529*/

/// [#530]
void promptfilename(StrNumber s, StrNumber e) {
    short k; ///< index into buffer.

    if (s == S(665)) { // "input file name"
        print_err(S(666)); // "I can't find file `"
    } else {
        print_err(S(667)); // "I can't write on file `"
    }

    print_file_name(curname, curarea, curext);
    print(S(668)); // "'."

    // ".tex"
    if (e == S(669)) showcontext();
    printnl(S(670)); // "Please type another "
    print(s);

    // "*** (job aborted file error in nonstop mode)"
    if (interaction < SCROLL_MODE) fatalerror(S(671));
    clear_terminal();
    print(S(488)); // ": "
    term_input();

    /// [#531] Scan file name in the buffer.
    beginname();
    k = first;
    while (buffer[k] == ' ' && k < last) k++;

    while (true) {
        if (k == last) break;
        if (!morename(buffer[k])) break;
        k++;
    }
    endname();

    // ""
    if (curext == S(385)) curext = e;
    packfilename(curname, curarea, curext);
} /* [#530] promptfilename */

/*534:*/
void openlogfile(void) {
    Selector old_setting;
    short k;
    short l;
    Char months[36];
    short FORLIM;

    old_setting = selector;
    if (job_name == 0) job_name = S(672); // "texput"
    packjobname(S(673)); // ".log"
    while (!a_open_out(&log_file)) { /*535:*/
        selector = TERM_ONLY;
        // "transcript file name" ".log"
        promptfilename(S(674), S(673));
    }
    /*:535*/
    logname = amakenamestring();
    selector = LOG_ONLY;
    log_opened = true;

    /*536:*/
    fprintf(log_file, "%s", TEX_BANNER);
    slow_print(format_ident);
    print(S(675)); // "  "
    print_int(day);
    print_char(' ');
    memcpy(months, "JANFEBMARAPRMAYJUNJULAUGSEPOCTNOVDEC", 36);
    FORLIM = month * 3;
    for (k = month * 3 - 3; k < FORLIM; k++) {
        fwrite(&months[k], 1, 1, log_file);
    }
    print_char(' ');
    print_int(year);
    print_char(' ');
    print_two(tex_time / 60);
    print_char(':');
    print_two(tex_time % 60); /*:536*/
    inputstack[inputptr] = cur_input;

    printnl(S(676)); // "**"
    l = inputstack[0].limitfield;
    if (buffer[l] == end_line_char) l--;
    for (k = 1; k <= l; k++) {
        print(buffer[k]);
    }
    println();

    selector = old_setting + 2;
}
/*:534*/

/*537:*/
void start_input(void) {
    scanfilename();
    if (curext == S(385)) curext = S(669);
    packfilename(curname, curarea, curext);
    while (true) {
        beginfilereading();
        if (a_open_in(&curfile)) break;
        if (curarea == S(385)) {
            packfilename(curname, S(677), curext);
            if (a_open_in(&curfile)) break;
        }
        endfilereading();
        promptfilename(S(665), S(669));
    }

    NAME = amakenamestring();
    if (job_name == 0) {
        job_name = curname;
        openlogfile();
    }

    newline_or_space(str_length(NAME));

    print_char('(');
    openparens++;
    slow_print(NAME);
    update_terminal();
    STATE = NEW_LINE;

#if 0
  if (NAME == str_ptr - 1) {   /*538:*/
    flush_string();
    NAME = curname;
  }
#else
    NAME = curname;
#endif

    line = 1;
    inputln(curfile, false);
    firm_up_the_line();
    if (end_line_char_inactive) {
        LIMIT--;
    } else {
        buffer[LIMIT] = end_line_char;
    }
    first = LIMIT + 1;
    LOC = START; /*:538*/
}
/*:537*/
/** @}*/ // end group S511x538_P188x195