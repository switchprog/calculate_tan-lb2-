#ifndef DD_ARITH_H
#define DD_ARITH_H

#include "dd_type.h"

// Быстрое точное сложение двух чисел
void fast_two_sum(double a, double b, double *s, double *t);

// Точное сложение двух чисел
void two_sum(double a, double b, double *s, double *t);

// Сложение двух double-double чисел
DoubleDouble dd_add(DoubleDouble a, DoubleDouble b);

// Вычитание double-double чисел
DoubleDouble dd_sub(DoubleDouble a, DoubleDouble b);

// Отрицание double-double числа
DoubleDouble dd_neg(DoubleDouble a);

#endif