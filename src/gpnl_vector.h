#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "gpnl_core.h"

number vector_norm2(number *, int);
void vector_diff(number *, number *, number *, int);
void vector_sum(number *, number *, number *, int);
void vector_product_constant(number *, number, number *, int);
number vector_scalar_product(number *, number *, int);
number vector_ang(number *, number *, int);

#endif
