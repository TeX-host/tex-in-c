#include "texmath.h"
#include "texfunc.h" // [func] print_char, print_int

/** @addtogroup S99x109_P38x41
 * @{
 */

/// [#108]: infinitely bad value
#define INF_BAD 10000

/// [#104] has arithmetic overflow occurred recently?
Boolean arith_error = false;
/// [#104] amount subtracted to get an exact division
Scaled tex_remainder = 0;


/// [#100] 整除 2
Integer half(Integer x) {
    if (x % 2) { // if odd(x)
        return (x + 1) / 2;
    } else {
        return x / 2;
    }
} // #100: half

/// [#102]:  create a scaled integer from a given decimal fraction.
///  { dig[i], | i = k-1, 0 ≤ k ≤ 17 }
Scaled round_decimals(SmallNumber k, char digs[]) {
    Integer a = 0; // the accumulator

    while (k > 0) {
        k--;
        a = (a + digs[k] * TWO) / 10;
    }
    return (a + 1) / 2;
} // #102: round_decimals

/// [#103]: prints scaled real, rounded to five digits
void print_scaled(Scaled s) {
    Scaled delta; // amount of allowable inaccuracy

    if (s < 0) { // print the sign, if negative
        print_char('-');
        s = -s;
    }

    // assert(s >= 0)
    print_int(s / UNITY); // print the integer part
    print_char('.');

    s = 10 * (s % UNITY) + 5;
    delta = 10;
    do { // s−δ ≤ 10*(2^16)*f < s
        // round the last digit
        if (delta > UNITY) s += (1 << 15) - 50000;
        print_char('0' + (s / UNITY));
        s = 10 * (s % UNITY);
        delta *= 10;
    } while (s > delta);
} // #103: print_scaled

/// [#105]: return `nx + y`
Scaled mult_and_add(Integer n, Scaled x, Scaled y, Scaled max_ans) {
    if (n < 0) {
        x = -x;
        n = -n;
    }

    // assert(n >= 0)
    if (n == 0) {
        return y;
    } else { // n > 0
        if (    x <= ((max_ans - y) / n)
            && -x <= ((max_ans + y) / n) ) {
            return (n * x) + y;
        } else {
            arith_error = true;
            return 0;
        }
    } // if (n <> 0)
} // #105: mult_and_add

/// [#106]: `x / n`
Scaled x_over_n(Scaled x, Integer n) {
    Scaled quotient;
    // should remainder be negated?
    Boolean negative = false;

    if (n == 0) {
        // 除 0 错误
        arith_error = true;
        quotient = 0;
        tex_remainder = x;
    } else { // n != 0
        if (n < 0) {
            x = -x;
            n = -n;
            negative = true;
        } // if (n < 0)

        // assert(n > 0)
        if (x >= 0) {
            quotient = x / n;
            tex_remainder = x % n;
        } else { // x < 0
            quotient = -(-x / n);
            tex_remainder = -(-x % n);
        } // if (x <> 0)
    } // if (n <> 0)

    if (negative) tex_remainder = -tex_remainder;
    return quotient;
} // #106: x_over_n

/// [#107]: `x * (n / d)`
Scaled xn_over_d(Scaled x, Integer n, Integer d) {
    Scaled quotient;
    // Note: 0100000L == 32768L == UNITY / 2
    const Scaled HALF = 0100000L; 
    Boolean positive; // was x >= 0?
    NonNegativeInteger t, u, v;

    if (x >= 0) {
        positive = true;
    } else {
        x = -x;
        positive = false;
    } // if (x <> 0)

    // assert(x >= 0)
    t = (x % HALF) * n;
    u = (x / HALF) * n + (t / HALF);
    v = (u % d) * HALF + (t % HALF);
    if ((u / d) >= HALF) {
        arith_error = true;
    } else {
        u = (u / d) * HALF + (v / d);
    }

    if (positive) {
        quotient = u;
        tex_remainder = (v % d);
    } else {
        quotient = -u;
        tex_remainder = -(v % d);
    }
    return quotient;
} // #107: xn_over_d

/// [#108]: compute the “badness” of glue.
/// given t >= 0
/// when a total t is supposed to be made from amounts that sum to s.
/// 
/// + badness = 100 * (t/s)^3
/// + badness(t + 1,s) ≥ badness(t,s) ≥ badness(t,s + 1)
HalfWord badness(Scaled t, Scaled s) {
    Scaled r; // approximation to α*t/s, where α^3 ≈ 100 * 2^18

    if (t == 0) {
        return 0;
    } else if (s <= 0) {
        return INF_BAD;
    } else {
        if (t <= 7230584L) {
            r = t * 297 / s; // 297^3 = 99.94 * 2^18
        } else if (s >= 1663497L) {
            r = t / (s / 297);
        } else {
            r = t;
        }
        if (r > 1290) { // 1290^3 < 2^31 < 1291^3
            return INF_BAD;
        } else {
            return ((r * r * r + 131072L) / 262144L);
        }
    }
} // #108:badness

/** @}*/ // end group S99x109_P38x41