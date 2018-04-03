#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "gpnl_core.h"

void matrix_sum(number **, number **, number **, unsigned int, unsigned int);
void matrix_multiply(number **, number **, number **, unsigned int, unsigned int);
number matrix_determinant(number **, unsigned int);
#endif
