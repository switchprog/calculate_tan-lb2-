#include "dd_mul.h"
#include "dd_arith.h"
#include <math.h>

// Разбиение числа на две части с заданной константой
void veltkamp_split(double x, double *x_high, double *x_low) {
    double C = 134217729.0;
    double gamma = C * x;
    double delta = x - gamma;
    *x_high = gamma + delta;
    *x_low = x - *x_high;
}

// Точное произведение через разбиение множителей
void dekker_prod(double x, double y, double *p_high, double *p_low) {
    double x_high, x_low, y_high, y_low;
    double t1, t2, t3;
    
    veltkamp_split(x, &x_high, &x_low);
    veltkamp_split(y, &y_high, &y_low);
    
    *p_high = x * y;
    // Компенсация ошибок округления
    t1 = -*p_high + x_high * y_high;
    t2 = t1 + x_high * y_low;
    t3 = t2 + x_low * y_high;
    *p_low = t3 + x_low * y_low;
}

// Умножение double-double чисел
DoubleDouble dd_mul(DoubleDouble a, DoubleDouble b) {
    double ph, pl;
    
    dekker_prod(a.hi, b.hi, &ph, &pl);
    pl += a.hi * b.lo + a.lo * b.hi;
    
    double zh, zl;
    fast_two_sum(ph, pl, &zh, &zl);
    
    return (DoubleDouble){zh, zl};
}
