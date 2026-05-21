#ifndef DD_TYPE_H
#define DD_TYPE_H

// Структура для числа двойной точности
typedef struct {
    double hi;  // старшая часть
    double lo;  // младшая часть
} DoubleDouble;

// Внешние константы
extern const DoubleDouble DD_PI;
extern const DoubleDouble DD_PI_2;
extern const DoubleDouble DD_PI_4;
extern const DoubleDouble DD_2PI;
extern const DoubleDouble FACTORIAL_EVEN[];
extern const DoubleDouble FACTORIAL_ODD[];
extern const double INV_PI[];
extern const double P_COEFF[];

#endif