#include "dd_trig.h"
#include "dd_arith.h"
#include "dd_mul.h"
#include "dd_div.h"
#include "dd_type.h"
#include <math.h>
#include <stdlib.h>

static DoubleDouble sin_series(DoubleDouble x) {
    if (x.hi == 0.0 && x.lo == 0.0) return x;

    DoubleDouble x2 = dd_mul(x, x);
    DoubleDouble term = x;
    DoubleDouble result = x;

    for (int i = 1; i < 200; i++) {
        term = dd_mul(term, x2);
        DoubleDouble add = dd_mul(FACTORIAL_ODD[i], term);
        result = dd_add(result, add);

        double norm_add = fabs(add.hi) + fabs(add.lo);
        double norm_res = fabs(result.hi) + fabs(result.lo);
        if (norm_add < norm_res * 1e-40) break;
    }
    return result;
}

static DoubleDouble cos_series(DoubleDouble x) {
    if (x.hi == 0.0 && x.lo == 0.0) return (DoubleDouble){1.0, 0.0};

    DoubleDouble x2 = dd_mul(x, x);
    DoubleDouble power = {1.0, 0.0};
    DoubleDouble result = {1.0, 0.0};

    for (int i = 1; i < 201; i++) {
        power = dd_mul(power, x2);
        DoubleDouble term = dd_mul(FACTORIAL_EVEN[i], power);
        result = dd_add(result, term);

        double norm_term = fabs(term.hi) + fabs(term.lo);
        double norm_res = fabs(result.hi) + fabs(result.lo);
        if (norm_term < norm_res * 1e-40) break;
    }
    return result;
}

static DoubleDouble reduce_angle_precise(DoubleDouble x, int *quadrant) {
    if (fabs(x.hi) < DD_PI_4.hi) {
        *quadrant = 0;
        return x;
    }
    double k_val = round((x.hi + x.lo) / DD_PI_2.hi);
    DoubleDouble k_dd = {k_val, 0.0};
    DoubleDouble k_halfpi = dd_mul(k_dd, DD_PI_2);
    DoubleDouble r = dd_sub(x, k_halfpi);
    *quadrant = ((int)fmod(k_val, 4.0) + 4) % 4;
    return r;
}

DoubleDouble dd_sin(DoubleDouble x) {
    int negate = 0;
    if (x.hi < 0.0 || (x.hi == 0.0 && x.lo < 0.0)) {
        x = dd_neg(x);
        negate = 1;
    }
    int quadrant;
    DoubleDouble r = reduce_angle_precise(x, &quadrant);
    DoubleDouble result;
    switch (quadrant) {
        case 0: result = sin_series(r); break;
        case 1: result = cos_series(r); break;
        case 2: result = dd_neg(sin_series(r)); break;
        case 3: result = dd_neg(cos_series(r)); break;
        default: result = sin_series(r); break;
    }
    return negate ? dd_neg(result) : result;
}

DoubleDouble dd_cos(DoubleDouble x) {
    if (x.hi < 0.0 || (x.hi == 0.0 && x.lo < 0.0)) {
        x = dd_neg(x);
    }
    int quadrant;
    DoubleDouble r = reduce_angle_precise(x, &quadrant);
    DoubleDouble result;
    switch (quadrant) {
        case 0: result = cos_series(r); break;
        case 1: result = dd_neg(sin_series(r)); break;
        case 2: result = dd_neg(cos_series(r)); break;
        case 3: result = sin_series(r); break;
        default: result = cos_series(r); break;
    }
    return result;
}

DoubleDouble dd_tan(DoubleDouble x) {
    if (x.hi == 0.0 && x.lo == 0.0) {
        return (DoubleDouble){0.0, 0.0};
    }
    int negate = 0;
    if (x.hi < 0.0 || (x.hi == 0.0 && x.lo < 0.0)) {
        x = dd_neg(x);
        negate = 1;
    }
    int quadrant;
    DoubleDouble r = reduce_angle_precise(x, &quadrant);
    DoubleDouble sin_r, cos_r;
    switch (quadrant) {
        case 0: sin_r = sin_series(r); cos_r = cos_series(r); break;
        case 1: sin_r = cos_series(r); cos_r = dd_neg(sin_series(r)); break;
        case 2: sin_r = dd_neg(sin_series(r)); cos_r = dd_neg(cos_series(r)); break;
        case 3: sin_r = dd_neg(cos_series(r)); cos_r = sin_series(r); break;
        default: sin_r = sin_series(r); cos_r = cos_series(r); break;
    }
    DoubleDouble result = dd_div(sin_r, cos_r);
    return negate ? dd_neg(result) : result;
}
