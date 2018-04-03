#include "gpnl_edo.h"

void edo_create(gpnl_function function, number tol, number x0, number x1, number y0, unsigned int max_iterations, edo_problem* problem) {
    problem->function = function;
    problem->tol = tol;
    problem->x0 = x0;
    problem->x1 = x1;
    problem->y0 = y0;
    problem->max_iterations = max_iterations;
    problem->method = RK4;
}

number edo_solve_rk4(edo_problem problem, edo_solution * solution) {
    unsigned int i, n = 1 + (problem.x1 - problem.x0)/problem.tol;
    number p[2], x, f1, f2, f3, f4;
    
    solution->y_values = malloc(sizeof(number) * n);
    solution->x_values = malloc(sizeof(number) * n);

    for(solution->x_values[0] = problem.x0, solution->y_values[0] = problem.y0, i = 1; i < n; i++) {
        solution->x_values[i] = problem.x0 + problem.tol*(i-1);
        
        p[0] = solution->x_values[i];
        p[1] = solution->y_values[i-1];
        
        f1 = problem.function(p);
        
        p[0] = solution->x_values[i]+problem.tol/2;
        p[1] = solution->y_values[i-1] + (f1*problem.tol)/2;
        
        f2 = problem.function(p);
        
        p[0] = solution->x_values[i]+problem.tol/2;
        p[1] = solution->y_values[i-1] + problem.tol*f2/2;
        
        f3 = problem.function(p);
        
        p[0] = solution->x_values[i]+problem.tol;
        p[1] = solution->y_values[i-1] + problem.tol*f3;
        
        f4 = problem.function(p);
                                 
        solution->y_values[i] = solution->y_values[i-1] + 1.0/6*(f1 + 2*f2 + 2*f3 + f4)*problem.tol;
    }

    solution->points = n;
}

number edo_evaluate(number x, edo_solution solution) {
    unsigned int i;

    for(i = 0; i < solution.points -1;i++) {
        if(solution.x_values[i] <= x && solution.x_values[i+1] >= x) {
            return solution.y_values[i];
        }
    }

    return NAN;
}
void edo_solve(edo_problem problem, edo_solution * solution) {
    edo_solve_rk4(problem, solution);
}
