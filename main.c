#include <stdio.h>
#include <stdlib.h>
#include "dd_type.h"
#include "dd_trig.h"

int main(int argc, char *argv[]) {
    DoubleDouble x;
    
    if (argc < 2) {
        return 1;
    }
    
    x.hi = atof(argv[1]);
    
    if (argc > 2) {
        x.lo = atof(argv[2]);
    } else {
        x.lo = 0.0;
    }
    
    DoubleDouble result = dd_tan(x);
    
    printf("%.17e %.17e\n", result.hi, result.lo);
    
    return 0;
}
