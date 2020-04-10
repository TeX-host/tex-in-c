#include <stdio.h>  // FILE
#include <string.h> // memcmp
#include "global_const.h"
#include "tex.h"
#include "macros.h" // [macro] pget, pput
#include "texfunc.h"
#include "str.h"    // [export]

#define maxstrings 300000
#define POOLPOINTER_IS_POINTER 1
#if POOLPOINTER_IS_POINTER
typedef StrASCIICode* PoolPtr;
#define pool_top (strpool + poolsize)
#define pool_elem(x, y) ((x)[(y)])
#else
typedef int PoolPtr;
#define pool_top poolsize
#define pool_elem(x, y) (strpool[(x) + (y)])
#endif

static StrASCIICode strpool[poolsize + 1];
static PoolPtr strstart[maxstrings + 1];
static PoolPtr poolptr;
static StrNumber strptr;
static PoolPtr initpoolptr;
static StrNumber initstrptr;

#define str_end(x) (strstart[(x) + 1])


#ifdef tt_INIT // #47
int getstringsstarted(void) {
#if POOLPOINTER_IS_POINTER
    poolptr = strpool;
#else
    poolptr = 0;
#endif
    strptr = 0;
    strstart[0] = poolptr;
    return str_pool_init();
}
#endif // #47: tt_INIT

int str_getc(StrNumber s, int k) { return pool_elem(strstart[s], k); }

void str_set_init_ptrs(void) {
    initstrptr = strptr;
    initpoolptr = poolptr;
}

StrPoolPtr str_mark(void) {
    StrPoolPtr res;
    res.val = (long)poolptr;
    return res;
}

void str_map_from_mark(StrPoolPtr b, void (*f)(StrASCIICode)) {
    PoolPtr k = (PoolPtr)b.val;
    while (k < poolptr) {
        f(pool_elem(k, 0));
        k++;
    }
    poolptr = (PoolPtr)b.val;
}

void str_print_stats(FILE* f_log_file) {
    fprintf(f_log_file, " %d string", strptr - initstrptr);
    if (strptr != initstrptr + 1) {
        fprintf(f_log_file, "s");
    }
    fprintf(f_log_file, " out of %ld\n", (long)(maxstrings - initstrptr));
    fprintf(f_log_file,
            " %ld string characters out of %ld\n",
            (long)(poolptr - initpoolptr),
            (long)(pool_top - initpoolptr));
}

int str_undump(FILE* fmtfile, FILE* _not_use_) {
    MemoryWord pppfmtfile;
    long x;
    /*:1308*/
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x < 0) goto _Lbadfmt_;
    if (x > poolsize) {
        fprintf(stdout, "---! Must increase the string pool size\n");
        goto _Lbadfmt_;
    }
    poolptr = (PoolPtr)x;
    pget(pppfmtfile);
    x = pppfmtfile.int_;
    if (x < 0) goto _Lbadfmt_;
    if (x > maxstrings) {
        fprintf(stdout, "---! Must increase the max strings\n");
        goto _Lbadfmt_;
    }
    strptr = x;
    fread(strstart, 1, sizeof(strstart[0]) * (strptr + 1), fmtfile);
    fread(strpool, 1, sizeof(strpool[0]) * (long)poolptr, fmtfile);
    initstrptr = strptr;
#if POOLPOINTER_IS_POINTER
    for (x = 0; x <= strptr; x++) {
        strstart[x] = strpool + (long)strstart[x];
    }
    poolptr = strpool + (long)poolptr;
#endif
    initpoolptr = poolptr; /*:1310*/
    return 1;
_Lbadfmt_:
    return 0;
}

void str_dump(FILE* fmtfile) {
    MemoryWord pppfmtfile;
    long poolused;
#if POOLPOINTER_IS_POINTER
    /* Convert for dumping */
    long x;
    for (x = 0; x <= strptr; x++) {
        strstart[x] = (PoolPtr)(strstart[x] - strpool);
    }
    poolused = poolptr - strpool;
#else
    poolused = poolptr;
#endif

    pppfmtfile.int_ = poolused;
    pput(pppfmtfile);
    pppfmtfile.int_ = strptr;
    pput(pppfmtfile);
    fwrite(strstart, 1, sizeof(strstart[0]) * (strptr + 1), fmtfile);
    fwrite(strpool, 1, sizeof(strpool[0]) * poolused, fmtfile);
#if POOLPOINTER_IS_POINTER
    /* Pool is still in use, need to convert back */
    for (x = 0; x <= strptr; x++) {
        strstart[x] = (PoolPtr)(strpool + (long)strstart[x]);
    }
#endif
    println();
    printint(strptr);
    print(S(1275));
    printint(poolused);
    /*:1309*/
}

void str_room(long l) {
    if (poolptr + l > pool_top) overflow(S(1276), pool_top - initpoolptr);
}

long str_adjust_to_room(long l) {
    if (poolptr + l > pool_top) {
        return pool_top - poolptr;
    } else {
        return l;
    }
}


int flength(StrNumber x) { return strstart[x + 1] - strstart[x]; }

void flushstring(void) {
    strptr--;
    poolptr = strstart[strptr];
}

void flushchar(void) { poolptr--; }

int get_cur_length() { return poolptr - strstart[strptr]; }

void str_cur_map(void (*f)(StrASCIICode)) {
    PoolPtr s;
    for (s = strstart[strptr]; s < poolptr; s++) {
        f(pool_elem(s, 0));
    }
    poolptr = strstart[strptr];
}

void str_map(StrNumber k, void (*f)(StrASCIICode)) {
    PoolPtr s;
    PoolPtr s_end = strstart[k] + flength(k);
    for (s = strstart[k]; s < s_end; s++) {
        f(pool_elem(s, 0));
    }
}

void printchar_helper(StrASCIICode c) { printchar(c); }

void str_print(StrNumber k) {
    if (k >= strptr) k = S(261);
    str_map(k, printchar);
}

static void f_pool_helper(ASCIICode c) { fputc(c, stderr); }

void f_pool(StrNumber s) {
    if (s >= strptr || s < 0) s = 274;
    str_map(s, f_pool_helper);
    fputc('\n', stderr);
    fflush(stderr);
}

StrNumber idlookup_s(StrNumber s, int nonew) {
    PoolPtr k = strstart[s];
    int l = flength(s);
    return idlookup_p(&pool_elem(k, 0), l, nonew);
}

/*70:*/
void printcurrentstring(void) {
    PoolPtr j;

    j = strstart[strptr];
    while (j < poolptr) {
        printchar(pool_elem(j, 0));
        j++;
    }
}
/*:70*/


/*43:*/
StrNumber makestring(void) {
    if (strptr == maxstrings) overflow(S(1277), maxstrings - initstrptr);
    strptr++;
    strstart[strptr] = poolptr;
    return (strptr - 1);
}
/*:43*/

/*46:*/
int str_cmp(StrNumber s, StrNumber t) {
    PoolPtr j = strstart[s];
    PoolPtr k = strstart[t];
    int l = 0;
    while (l < flength(s) && l < flength(t)) {
        int dif = pool_elem(j, l) - pool_elem(k, l);
        if (dif) {
            return dif;
        }
        l++;
    }
    return flength(s) - flength(t);
}

Boolean streqstr(StrNumber s, StrNumber t) {
    if (flength(s) != flength(t)) {
        return false;
    }
    return !str_cmp(s, t);
}
/*:46*/

void str_appendchar(StrASCIICode s) {
    if (poolptr < pool_top) {
        pool_elem(poolptr, 0) = s;
        poolptr++;
    }
}

void appendchar(StrASCIICode s) {
    if (poolptr < pool_top) {
        str_appendchar(s);
    } else {
        overflow(S(1276), pool_top - initpoolptr);
    }
}

int str_valid(StrNumber s) { return ((s >= 0) && (s < strptr)); }

/*60:*/
void slowprint(StrNumber s) {
    PoolPtr j;

    if (s >= strptr || s < 256) {
        print(s);
        return;
    }
    j = strstart[s];
    while (j < str_end(s)) {
        print(pool_elem(j, 0));
        j++;
    }
} /*:60*/

int str_scmp(StrNumber s, short* buffp) {
    PoolPtr u = strstart[s];
    int hn = flength(s);
    int j = 0;
    do {
        if (pool_elem(u, j) < buffp[j]) return -1;
        if (pool_elem(u, j) > buffp[j]) return 1;
        j++;
    } while (j < hn); /*932:*/
    return 0;
}

int str_bcmp(unsigned char* buffp, long l, StrNumber s) {
    if (flength(s) == l) {
        if (!memcmp(&pool_elem(strstart[s], 0), buffp, l)) return 1;
    }
    return 0;
}

StrNumber str_ins(short* buffp, long l) {
    int k;
    if (poolptr + l > pool_top) overflow(S(1276), pool_top - initpoolptr);
    for (k = 0; k < l; k++) {
        appendchar(buffp[k]);
    }
    return makestring();
}

StrNumber str_insert(unsigned char* buffp, long l) {
    int d, k;
    StrNumber result;
    if (poolptr + l > pool_top) overflow(S(1276), pool_top - initpoolptr);
    d = get_cur_length();
    while (poolptr > strstart[strptr]) {
        poolptr--;
        pool_elem(poolptr, l) = pool_elem(poolptr, 0);
    }
    for (k = 0; k < l; k++) {
        appendchar(buffp[k]);
    }
    result = makestring();
    poolptr += d;
    return result;
}
