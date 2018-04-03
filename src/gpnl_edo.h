#ifndef _EDO_H_
#define _EDO_H_

#include "gpnl_core.h"


typedef enum {
    RK4
} edo_method;

typedef struct {
    edo_method method;

    number tol;
    number x0;
    number x1;
    number y0;
    unsigned int max_iterations;
    gpnl_function function;
} edo_problem;

typedef struct {
    number * y_values;
    number * x_values;

    unsigned int points;
} edo_solution;

void edo_create(gpnl_function, number, number, number, number,  unsigned int, edo_problem*);
void edo_change_method(const char *, edo_problem *);

void edo_solve(edo_problem, edo_solution*);

number edo_evaluate(number x, edo_solution);
#endif
