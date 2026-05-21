#include "dd_div.h"
#include "dd_mul.h"
#include "dd_arith.h"

// Деление через итерации Ньютона для обратного
DoubleDouble dd_div(DoubleDouble a, DoubleDouble b) {
    double inv_hi = 1.0 / b.hi;
    DoubleDouble inv = {inv_hi, 0.0};
    DoubleDouble two = {2.0, 0.0};
    
    // Три итерации
    for (int i = 0; i < 3; i++) {
        DoubleDouble b_inv = dd_mul(b, inv);
        DoubleDouble two_minus = dd_sub(two, b_inv);
        inv = dd_mul(inv, two_minus);
    }
    
    return dd_mul(a, inv);
}