#pragma once
#ifndef INC_PURE_FUNC
// 放置纯函数 / 无全局变量依赖的函数
#define INC_PURE_FUNC

extern int hex_to_i(int c, int cc);
void aclose(FILE** f);
void w_close(FILE** f);


#endif // INC_PURE_FUNC