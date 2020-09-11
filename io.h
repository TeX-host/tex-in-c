#pragma once
#ifndef INC_IO_H
/** [p13#25] Input and output.
 *
 */
#define INC_IO_H

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


extern Char name_of_file[FILE_NAME_SIZE + 1];
extern UInt16 namelength;

extern ASCIICode buffer[BUF_SIZE + 1];
extern UInt16 first;
extern UInt16 last;
extern UInt16 max_buf_stack;


// [inputln], tex
extern Boolean inputln(FILE* f, Boolean bypass_eoln);
extern Boolean initterminal(void);

#endif /* INC_IO_H */