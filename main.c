#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dd_type.h"
#include "dd_trig.h"

int main(int argc, char *argv[]) {
    DoubleDouble x;
    
    // Чтение старшей части
    x.hi = atof(argv[1]);
    
    // Чтение младшей части
    if (argc > 2) {
        x.lo = atof(argv[2]);
    } else {
        x.lo = 0.0;
    }
    
    // Вычисление тангенса
    DoubleDouble result = dd_tan(x);
    
    // Вывод результата
    printf("%.17e %.17e\n", result.hi, result.lo);
    
    return 0;
}