#include "dd_arith.h"
#include <math.h>

// Быстрое точное сложение с перестановкой операндов
void fast_two_sum(double a, double b, double *s, double *t) {
    double dum, z;
    
    if (fabs(b) > fabs(a)) {
        dum = a;
        a = b;
        b = dum;
    }
    
    *s = a + b;
    z = *s - a;
    *t = b - z;
}

// Точное сложение
void two_sum(double a, double b, double *s, double *t) {
    double a1, b1, da, db;
    
    *s = a + b;
    a1 = *s - b;
    b1 = *s - a1;
    da = a - a1;
    db = b - b1;
    *t = da + db;
}

// Сложение double-double
DoubleDouble dd_add(DoubleDouble a, DoubleDouble b) {
    double sh, sl, th, tl, c, vh, vl, wh, zh, zl;
    
    two_sum(a.hi, b.hi, &sh, &sl);   // сумма старших частей
    two_sum(a.lo, b.lo, &th, &tl);   // сумма младших частей
    
    c = sl + th;
    fast_two_sum(sh, c, &vh, &vl);
    
    wh = tl + vl;
    fast_two_sum(vh, wh, &zh, &zl);
    
    return (DoubleDouble){zh, zl};
}

// Вычитание через сложение с отрицанием
DoubleDouble dd_sub(DoubleDouble a, DoubleDouble b) {
    return dd_add(a, (DoubleDouble){-b.hi, -b.lo});
}

// Отрицание
DoubleDouble dd_neg(DoubleDouble a) {
    return (DoubleDouble){-a.hi, -a.lo};
}