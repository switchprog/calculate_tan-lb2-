#include "dd_trig.h"
#include "dd_arith.h"
#include "dd_mul.h"
#include "dd_div.h"
#include "dd_type.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif

static double get_time(void) {
#ifdef _WIN32
    static int initialized = 0;
    static double freq = 0.0;
    LARGE_INTEGER now;
    
    if (!initialized) {
        LARGE_INTEGER freq_li;
        QueryPerformanceFrequency(&freq_li);
        freq = (double)freq_li.QuadPart;
        initialized = 1;
    }
    
    QueryPerformanceCounter(&now);
    return (double)now.QuadPart / freq;
#else
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec + now.tv_nsec / 1e9;
#endif
}

// Ряд Тейлора для синуса
static DoubleDouble sin_series(DoubleDouble x) {
    if (x.hi == 0.0 && x.lo == 0.0) return x;
    
    DoubleDouble x2 = dd_mul(x, x);
    DoubleDouble term = x;
    DoubleDouble result = x;
    int sign = 0;
    double start_time = get_time();
    
    for (int i = 0; i < 50; i++) {
        term = dd_mul(term, x2);
        
        if (i >= 20) break;
        
        DoubleDouble coeff = FACTORIAL_ODD[i];
        DoubleDouble add = dd_mul(coeff, term);
        
        if (sign == 0) {
            result = dd_sub(result, add);
        } else {
            result = dd_add(result, add);
        }
        
        if (get_time() - start_time > 5.0) {
            printf("Time is over\n");
            exit(0);
        }
        
        if (fabs(add.hi) < fabs(result.hi) * 1e-38) break;
        
        sign = 1 - sign;
    }
    
    return result;
}

// Ряд Тейлора для косинуса
static DoubleDouble cos_series(DoubleDouble x) {
    if (x.hi == 0.0 && x.lo == 0.0) return (DoubleDouble){1.0, 0.0};
    
    DoubleDouble x2 = dd_mul(x, x);
    DoubleDouble power = {1.0, 0.0};
    DoubleDouble result = {1.0, 0.0};
    int sign = 1;  // 1 для вычитания, 0 для сложения
    double start_time = get_time();
    
    for (int i = 0; i < 50; i++) {
        power = dd_mul(power, x2);
        
        if (i >= 15) break; 
        
        DoubleDouble coeff = FACTORIAL_EVEN[i];
        DoubleDouble term = dd_mul(power, coeff);
        
        if (sign == 1) {
            result = dd_sub(result, term);
        } else {
            result = dd_add(result, term);
        }
        
        if (get_time() - start_time > 5.0) {
            printf("Time is over\n");
            exit(0);
        }
        
        if (fabs(term.hi) < fabs(result.hi) * 1e-38) break;
        
        sign = 1 - sign;
    }
    
    return result;
}

// Приведение угла к диапазону [-пи/4, пи/4] с определением квадранта
static DoubleDouble reduce_angle_precise(DoubleDouble x, int *quadrant) {
    if (fabs(x.hi) < DD_PI_2.hi) {
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
// Синус
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
        case 0:
            result = sin_series(r);
            break;
        case 1:
            result = cos_series(r);
            break;
        case 2:
            result = dd_neg(sin_series(r));
            break;
        case 3:
            result = dd_neg(cos_series(r));
            break;
        default:
            result = sin_series(r);
            break;
    }
    
    return negate ? dd_neg(result) : result;
}

// Косинус
DoubleDouble dd_cos(DoubleDouble x) {
    if (x.hi < 0.0 || (x.hi == 0.0 && x.lo < 0.0)) {
        x = dd_neg(x);
    }
    
    int quadrant;
    DoubleDouble r = reduce_angle_precise(x, &quadrant);
    DoubleDouble result;
    
    switch (quadrant) {
        case 0:
            result = cos_series(r);
            break;
        case 1:
            result = dd_neg(sin_series(r));
            break;
        case 2:
            result = dd_neg(cos_series(r));
            break;
        case 3:
            result = sin_series(r);
            break;
        default:
            result = cos_series(r);
            break;
    }
    
    return result;
}

// Тангенс
DoubleDouble dd_tan(DoubleDouble x) {
    // Для особых случаев
    if (x.hi == 0.0 && x.lo == 0.0) {
        return (DoubleDouble){0.0, 0.0};
    }
    
    DoubleDouble sin_x = dd_sin(x);
    DoubleDouble cos_x = dd_cos(x);
    
    // Проверка на полюс
    if (fabs(cos_x.hi) < 1e-300) {
        if (cos_x.hi > 0.0) {
            return (DoubleDouble){1e300, 0.0};
        } else {
            return (DoubleDouble){-1e300, 0.0};
        }
    }
    
    return dd_div(sin_x, cos_x);
}
