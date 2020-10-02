#include <stdio.h>  // FILE
#include <string.h> // memcpy, strlen, strcpy, strchr, strcmp, strcat, strncmp 
#include <stdlib.h> // malloc, exit, getenv,
#include "tex.h"    // [macro] BUF_SIZE, FILE_NAME_SIZE
#include "str.h"    // [type] StrNumber
#include "global.h" // [var] buffer, last, first, name_of_file
#include "global_const.h" // [macro] update_terminal
#include "eqtb.h"         // [var] use_independence_date
#include "io.h"           // [macro] TERM_ERR


static int pos = 0; 
static Boolean in_c_flag = false;
Boolean need_to_load_format = false;

#define MY_BUFSIZE 512
static char mybuff[MY_BUFSIZE];

static char* format_name;

static char** font_path;
static char* font_path_buf;
static char** input_path;
static char* input_path_buf;
static char** format_path;
static char* format_path_buf;

static char* font_path_default[] = {
    "/usr/share/texmf/fonts/tfm/public/cm/",
    "/usr/share/texmf/fonts/tfm/public/latex/",
    "/usr/share/texmf/fonts/tfm/public/ae/",
    "/usr/share/texmf/fonts/tfm/public/cmextra/",
    "/usr/share/texmf/fonts/tfm/public/mflogo",
    "/usr/share/texmf/fonts/tfm/public/misc/",
    "/usr/share/texmf/fonts/tfm/ams/cmextra/",
    "/usr/share/texmf/fonts/tfm/ams/cyrillic/",
    "/usr/share/texmf/fonts/tfm/ams/euler/",
    "/usr/share/texmf/fonts/tfm/ams/symbols/",
    "/var/lib/texmf/tfm/public/plfonts/",
    "/var/lib/texmf/tfm/jknappen/ec/",
    "",
    0};

char* p_path[] = {"", 0};


Boolean initinc(int _not_use_) {
    if (in_c_flag) {
        last = first;
        memcpy(&(buffer[first]), mybuff, pos);
        last += pos;
        return true;
    } else {
        return false;
    }
}

static void env_to_path(char*** path, char** buf, char* env_var) {
    char** my_path;
    char* my_path_buf;
    char* pp;
    int k;
    int i = strlen(env_var);
    my_path_buf = (char*)malloc(i + 1);
    my_path = (char**)malloc(sizeof(char*) * (i + 2));
    if (!my_path_buf || !my_path) {
        fprintf(TERM_ERR, "Out of memory\n");
        exit(11);
    }
    strcpy(my_path_buf, env_var);
    my_path[0] = pp = my_path_buf;
    k = 1;
    while ((pp = strchr(pp, ':'))) {
        *pp = 0;
        pp++;
        my_path[k++] = pp;
    }
    my_path[k] = 0;
    *path = my_path;
    *buf = my_path_buf;
}

int PASCAL_MAIN(int argc, char** argv) {
    char* pom;
    // printf("%s\n", argv[0]); // 打印可执行程序路径
    if ((pom = getenv("TEXINPUTS"))) {
        env_to_path(&input_path, &input_path_buf, pom);
    } else {
        input_path = p_path;
    }
    if ((pom = getenv("TEXFONTS"))) {
        env_to_path(&font_path, &font_path_buf, pom);
    } else {
        font_path = font_path_default;
    }
    if ((pom = getenv("TEXFORMATS"))) {
        env_to_path(&format_path, &format_path_buf, pom);
    } else {
        format_path = p_path;
    }
    if (argc > 1 && !strcmp(argv[1], "--indep")) {
        use_independence_date = true;
        argv++;
        argc--;
    }
    if (argc > 1) {
        if (argv[1][0] == '&') {
            need_to_load_format = true;
            format_name = &(argv[1][1]);
            argv++;
            argc--;
        }
    }
    if (argc <= 1) {
        in_c_flag = false;
    } else {
        int j = 1;
        pos = 0;
        for (; j < argc; j++) {
            if (strlen(argv[j]) + pos + 3 < BUF_SIZE) {
                strcpy(&(mybuff[pos]), argv[j]);
                pos += strlen(argv[j]);
                mybuff[pos++] = ' ';
            }
        }
        pos--;
        if (pos > 0) {
            in_c_flag = true;
        }
    }
    return 0;
}

static Boolean a_open_in1(char* name, char** path_lst, FILE** f);

/// [#524]
Boolean open_fmt(FILE** fmt) {
    char* fmt_name;
    char name_buf[256] = "plain.fmt";
    fmt_name = format_name;
    fprintf(TERM_OUT, "%s\n", format_name);
    if (!strchr(format_name, '.')) {
        if (strlen(format_name) + strlen(".fmt") < 255) {
            strcpy(name_buf, format_name);
            strcat(name_buf, ".fmt");
            fmt_name = name_buf;
        }
    }
    if (a_open_in1(fmt_name, format_path, fmt)) {
        return true;
    } else {
        fprintf(TERM_OUT, "Sorry, I can't find that format; will try PLAIN.\n");
        update_terminal();
        if (a_open_in1("plain.fmt", format_path, fmt)) {
            return true;
        } else {
            fprintf(TERM_OUT, "I can't find the PLAIN format file!\n");
            return false;
        }
    }
}


static Boolean a_open_in1(char* name, char** path_lst, FILE** f) {
    /* XXXX Fixed buffer size */
    char path_buff[512];
    char* path_dir;
    int ln = strlen(name);
    for (path_dir = *path_lst++; path_dir; path_dir = *path_lst++) {
        int lp = strlen(path_dir);
        if (lp > 512 - 2 - ln) break;
        strcpy(path_buff, path_dir);
        if (lp > 0 && path_buff[lp - 1] != '/') {
            path_buff[lp] = '/';
            path_buff[lp + 1] = 0;
        }
        strcat(path_buff, name);
        if (!*f)
            *f = fopen(path_buff, "rb");
        else
            *f = freopen(path_buff, "rb", *f);
        if (*f) return 1;
    }
    return *f != 0;
}

Boolean a_open_in(FILE** f) {
    char** path_lst;
    char* name;
    char* fname = (char*)name_of_file;

    if (!strncmp(fname, "TeXfonts:", strlen("TeXfonts:"))) {
        name = fname + strlen("TeXfonts:");
        path_lst = font_path;
    } else if (!strncmp(fname, "TeXinputs:", strlen("TeXinputs:"))) {
        name = fname + strlen("TeXinputs:");
        path_lst = input_path;
    } else if (!strncmp(fname, "TeXformats:", strlen("TeXformats:"))) {
        name = fname + strlen("TeXformats:");
        path_lst = format_path;
    } else {
        name = fname;
        path_lst = p_path;
    }

    return a_open_in1(name, path_lst, f);
}

Boolean a_open_out(FILE** f) {
    char* fname = (char*)name_of_file;

    if ((*f) == NULL) {
        *f = fopen(fname, "wb");
    } else {
        *f = freopen(fname, "wb", *f);
    }

    return (*f) != NULL;
}
