/**
   GPNL - General Purpose Numeric Library
   Copyright (C) 2017 freesoftmath.org

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
   
   @author José Carlos García - freesoftmath.org
 */
#include "gpnl_calculus.h"


/**
   Calculate the derivative of a function in a given point.
   @param f a C function with one variable.
   @param x0 a point where you want to calculate the derivative.
 */
number calculus_numeric_derivative( number (*f)(number), number x0 , number h) {
    return (-f(x0+2*h)+8*f(x0+h)-8*f(x0-h)+f(x0-2*h))/(12*h);
}

/**
   Search a root of a funcion near a point with a given tolerance.
   @param f a C function with one variable.
   @param x0 a point near the root.
   @param tol tolerance.
   @param max_iterations the max amount of iterations.
   @param result_code a pointer for retrieving the result code.
*/
number calculus_findroot_newton(number (*f)(number), number x0, number tol, unsigned int max_iterations, int * result_code, number h) {
    int k;
    number x1, derivative, value;

    for(k = 0; k < max_iterations;k++) {
        derivative = calculus_numeric_derivative(f, x0, h);
        if((derivative > 0 && derivative < tol) || (derivative <= 0 && -derivative < tol)) {
            *result_code = -1; // f' have to be different of zero.
            return x1;
        }
        x1 = x0 - f(x0)/derivative;
        if(x1-x0 < tol) {
            *result_code = k+1;
            return x1;
        }
        
        x0 = x1;

    }
    *result_code = -2; // Not convergence after max_iterations.

    return x1;
}


void calculus_multivar_gradient(gpnl_function f, unsigned int variables, number* x0, number * gradient, number h) {
    int i;

#if GPU
#pragma acc parallel loop
#else
#pragma omp parallel for
#endif
    for(i = 0; i < variables; i++) {

        number function_onevariable(number x) {
            int j;            
            number *t, value;
            
            t = (number*)malloc(sizeof(number)*variables);

            for(j = 0; j < variables; j++) {
                if(i != j) {
                    t[j] = x0[j];
                }
                else {
                    t[j] = x;
                }
            }
            value = f(t);
            free(t);

            return value;
        }

        gradient[i] = calculus_numeric_derivative(function_onevariable, x0[i], h);
    }
}
