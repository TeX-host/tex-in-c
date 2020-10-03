#pragma once
#ifndef INC_TEX_HEADER
/// 存放 tex.c 的头部。
/// 导入库、其他模块，声明 tex.c 内部的变量
#define INC_TEX_HEADER
#include <stdlib.h> // exit, EXIT_SUCCESS

#include "global_const.h"
#include "tex.h"
#include "tex_types.h"
#include "tex_constant.h"

#include "charset.h"
#include "error.h"
    // [func] normalize_selector, succumb, fatalerror,
    // overflow, confusion, print_err,
    // [macro] checkinterrupt,
#include "mem.h"
#include "lexer.h"      // lexer

#include "eqtb.h"
#include "hash.h"       // [func] sprint_cs
#include "expand.h"     // [macro] STORE_NEW_TOKEN
#include "scan.h"       // scan mod;

#include "io.h" // [func] inputln
#include "fonts.h"
#include "dviout.h"

#include "pack.h"
#include "mmode.h"
#include "align.h"
#include "hyphen.h"
#include "page.h"

#include "main_ctrl.h"
#include "dump.h"
#include "extension.h"

#include "str.h" // [func] get_strings_started
#include "box.h" // [macro] subtype, widthoffset; [func] delete_token_ref


/// [#1332] long jmp label
jmp_buf _JMP_global__end_of_TEX;
/// [#1332] long jmp label
jmp_buf _JMP_global__final_end;

#endif // INC_TEX_HEADER