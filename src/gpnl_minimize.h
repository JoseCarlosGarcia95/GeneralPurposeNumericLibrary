#ifndef _MINIMIZE_H_
#define _MINIMIZE_H_

#include "gpnl_core.h"
#include "gpnl_vector.h"
#include "gpnl_calculus.h"

typedef enum {
    SECANT,
    GRADIENT
} minimize_method;

typedef struct {
    unsigned int variables;
    minimize_method method;

    number tol;

    unsigned int max_iterations;

    gpnl_function objective_function;

    unsigned int restrictions;
    gpnl_function * restrictions_functions;

    number * initial_points;
    number zero;
    number initial_gradient_alpha;
} minimize_problem;

typedef struct {
    unsigned int iterations;
    unsigned int error_code;

    number * sols;
} minimize_problem_result;

void minimize_create(gpnl_function, number, unsigned int, unsigned int, minimize_problem*);
void minimize_change_method(const char *, minimize_problem *);
void minimize_change_gradient_alpha(number, minimize_problem * );
void minimize_add_restriction(gpnl_function, minimize_problem *);
void minimize_change_initial_points(number *, minimize_problem *);

void minimize_solve(minimize_problem, minimize_problem_result*);
#endif
