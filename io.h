#pragma once
#ifndef INC_IO_H
/** [p13#25] Input and output.
 *
 */
#define INC_IO_H
#include "tex_constant.h"
#include "charset.h" // [type] ASCIICode
#include "str.h"

/** @addtogroup S25x37_P13x18
 * @{
 */
/// [p14#27]  open a file for output.
#define wopenout a_open_out
// [p17#33] 无需打开输出流，直接使用 stdout
// #define topenin() (termin = stdin) // 33, 37
// #define topenout() (termout = stdout) // 33, 1332

/// [#32] the terminal as an input file.
#define TERM_IN     stdin
/// [#32] the terminal as an output file.
#define TERM_OUT    stdout
/// 错误输出到 stderr
#define TERM_ERR    stderr

/// [#34] empty the terminal output buffer.
#define update_terminal() fflush(TERM_OUT)
/// [#34] clear the terminal input buffer.
/// xref: 330(#clearforerrorprompt), 530(#promptfilename).
#define clear_terminal()
/** @}*/ // end group S25x37_P13x18

/** @addtogroup S511x538_P188x195
 * @{
 */
/// [p190#520] `.fmt`, the extension, as a constant.
#define FORMAT_DEFAULT_LENGTH   20
#define formatarealength        11
#define formatextlength         4
#define formatextension         S(256)
/** @}*/ // end group S511x538_P188x195


extern Char name_of_file[FILE_NAME_SIZE + 1];
extern UInt16 namelength;

extern ASCIICode buffer[BUF_SIZE + 1];
extern UInt16 first;
extern UInt16 last;
extern UInt16 max_buf_stack;

// [inputln], tex
extern Boolean inputln(FILE* f, Boolean bypass_eoln);
extern Boolean initterminal(void);


// io_unix
// [funcs], tex
extern int PASCAL_MAIN(int argc, char** argv);
extern Boolean initinc(int _not_use_);
extern Boolean open_fmt(FILE** fmt);
extern void fix_date_and_time(long* tex_time_p, long* day_p, long* month_p,
                              long* year_p);
extern Boolean need_to_load_format;

// [funcs], fonts, tex
extern Boolean a_open_in(FILE** f);

// [funcs], dviout, tex
extern Boolean a_open_out(FILE** f);

extern void aclose(FILE** f);
extern void w_close(FILE** f);


// io_fname
extern StrNumber curname, curarea, curext;
extern StrNumber extdelimiter;
extern Char TEXformatdefault[FORMAT_DEFAULT_LENGTH];
extern Boolean name_in_progress;
extern StrNumber job_name;
extern Boolean log_opened;
extern StrNumber output_file_name, logname;

extern void fname_init();
extern StrNumber makenamestring(void);
extern StrNumber bmakenamestring(void);
extern StrNumber wmakenamestring(void);
extern void scanfilename(void);
extern void print_file_name(StrNumber n, StrNumber a, StrNumber e);
extern void packfilename(StrNumber fname, StrNumber prefix, StrNumber ext);
extern void packjobname(StrNumber s);
extern void promptfilename(StrNumber s, StrNumber e);
extern void openlogfile(void);
extern void startinput(void);

#endif /* INC_IO_H */