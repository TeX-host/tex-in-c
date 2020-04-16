#include <stdio.h>
#include "pure_func.h"
// 放置纯函数 / 无全局变量依赖的函数

int hex_to_i(int c, int cc) {
    int res = (c <= '9') ? (c - '0') : (c - 'a' + 10);
    res *= 16;
    res += (cc <= '9') ? (cc - '0') : (cc - 'a' + 10);
    return res;
}

/*28:*/
void aclose(FILE** f) {
    if (*f != NULL) fclose(*f);
    *f = NULL;
}

void w_close(FILE** f) {
    if (*f != NULL) fclose(*f);
    *f = NULL;
}
/*:28*/
