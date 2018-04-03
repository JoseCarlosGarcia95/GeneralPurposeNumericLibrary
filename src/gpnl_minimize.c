// test

#include "gpnl_minimize.h"

void minimize_create(gpnl_function objective_function, number tol, unsigned int max_iterations, unsigned int variables, minimize_problem* new_problem) {    
    new_problem->objective_function = objective_function;
    new_problem->tol = tol;
    new_problem->max_iterations = max_iterations;
    new_problem->variables = variables;

    if(new_problem->variables == 1) {
        new_problem->method = SECANT;
    } else {
        new_problem->method = GRADIENT;
    }

    new_problem->restrictions = 0;
    new_problem->restrictions_functions = malloc(0);
    new_problem->initial_gradient_alpha = 0;
    new_problem->initial_points = calloc(sizeof(number), new_problem->variables);
    new_problem->zero = .000001;
}
void minimize_change_method(const char * method, minimize_problem * problem) {
    if(strcmp(method, "secant")) {
        problem->method = SECANT;
    } else if (strcmp(method, "gradient")) {
        problem->method = GRADIENT;
    }
}

void minimize_change_gradient_alpha(number initial_alpha, minimize_problem * problem) {
    problem->initial_gradient_alpha = initial_alpha;
}


void minimize_add_restriction(gpnl_function restriction, minimize_problem * problem) {
    problem->restrictions++;
    problem->restrictions_functions = realloc(problem->restrictions_functions, sizeof(gpnl_function)*problem->restrictions);

    problem->restrictions_functions[problem->restrictions - 1] = restriction;

    if(problem->method == SECANT) {
        problem->method = GRADIENT;
    }
}

void minimize_change_initial_points(number * x0, minimize_problem * problem) {
    int i;

    for(i = 0; i < problem->variables;i++) {
        problem->initial_points[i] = x0[i];
    }

}

void minimize_findminimum_secant(minimize_problem problem, minimize_problem_result* result) { 
    number candidate;
    int result_code;
    
    number derivative(number x) {
        number onevariable_function(number z) {
            number * x = malloc(sizeof(number));
            x[0] = z;
            return problem.objective_function(x);
        }

        return calculus_numeric_derivative(onevariable_function, x, problem.zero);
    }

    candidate = calculus_findroot_newton(derivative, problem.initial_points[0], problem.tol, problem.max_iterations, &result_code, problem.zero);

    result->iterations = result_code;
    result->sols = malloc(sizeof(number));

    if(result_code < 0) {
        result->error_code = result_code;
    } else {
        if(calculus_numeric_derivative(derivative, candidate, problem.zero) > 0) {
            result->error_code = 0;
            result->sols[0] = candidate;
        } else {
            result->error_code = -3;
        }
    }
}

number minimize_kkt(minimize_problem problem, number * x) {
    number f = 0;

    f = problem.objective_function(x);

    return f;    
}

void minimize_findminimum_gradient_alpha(minimize_problem problem, minimize_problem_result* alpha_result, number * gradient, number * x0p) {
    int i;
    number * x0;

    x0 = (number*)malloc(sizeof(number)*problem.variables);

    for(i = 0;i < problem.variables;i++) {
        x0[i] = x0p[i];
    }
    
    number alpha_function(number* alpha) {
        for ( i = 0; i < problem.variables; i++ ) {
            x0[i] = x0[i] - alpha[0]*gradient[i];
        }
        
        return minimize_kkt(problem, x0);
    }

    number alpha_initial[] = {problem.initial_gradient_alpha};

    minimize_problem alpha_problem;
    minimize_create(alpha_function, problem.tol, problem.max_iterations, 1, &alpha_problem);
    minimize_change_initial_points(alpha_initial, &alpha_problem);
    minimize_solve(alpha_problem, alpha_result);    
}

void minimize_findminimum_gradient(minimize_problem problem, minimize_problem_result* result) {
    int i, j;
    minimize_problem_result alpha_problem;
    number * gradient, *x0, *test;

    gradient = malloc(sizeof(number)*problem.variables);
    x0 = malloc(sizeof(number)*problem.variables);
    result->sols = malloc(sizeof(number)*problem.variables);
    test = malloc(sizeof(number)*problem.variables);

    for(i = 0; i < problem.variables; i++) {
        x0[i] = problem.initial_points[i];
    }
    
    for ( result->iterations = 0; result->iterations < problem.max_iterations; ) {
        calculus_multivar_gradient(problem.objective_function, problem.variables, x0, gradient, problem.zero);
        minimize_findminimum_gradient_alpha(problem, &alpha_problem, gradient, x0);

        if(alpha_problem.error_code < 0) {
            result->error_code = alpha_problem.error_code;
            return;
        }

        for( j = 0; j < problem.variables; j++) {
            result->sols[j] = x0[j]-alpha_problem.sols[0]*gradient[j];
        }

        vector_diff(x0, result->sols, test, problem.variables);

        if(vector_norm2(test, problem.variables) < problem.tol) {
            result->error_code = 0;
            return;
        }

        for( j = 0; j < problem.variables; j++) {
            x0[j] = result->sols[j];
        }

        result->iterations = result->iterations + alpha_problem.iterations;
    }
    result->error_code = -3;
}

void minimize_solve(minimize_problem problem, minimize_problem_result* result) {
    if(problem.method == SECANT && problem.variables == 1) {
        minimize_findminimum_secant(problem, result);
    } else {
        minimize_findminimum_gradient(problem, result);
    }
}
