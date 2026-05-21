#ifndef DD_MUL_H
#define DD_MUL_H

#include "dd_type.h"

// Разбиение числа Велткампом для точного умножения
void veltkamp_split(double x, double *x_high, double *x_low);

// Точное произведение двух чисел методом Деккера
void dekker_prod(double x, double y, double *p_high, double *p_low);

// Умножение double-double чисел
DoubleDouble dd_mul(DoubleDouble a, DoubleDouble b);

#endif