#ifndef _CALCULUS_H_
#define _CALCULUS_H_

#include "gpnl_core.h"

number calculus_numeric_derivative(number (*f)(number), number, number);
number calculus_findroot_newton(number (*f)(number), number x0, number tol, unsigned int, int *, number);
void calculus_multivar_gradient(gpnl_function, unsigned int, number* , number *, number);

#endif
