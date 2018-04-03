#include "gpnl_vector.h"

number vector_norm2(number * v, int n) {
    int i;

    number val;

    for( i = 0, val = 0; i < n; i++ ) {
        val = val + v[i]*v[i];
    }

    return sqrt(val);
}

void vector_diff(number * v1, number* v2, number *result, int n) {
    int i;

#if GPU
#pragma acc parallel loop
#else
#pragma omp parallel for
#endif
    for( i = 0; i < n; i++ ) {
        result[i] = v1[i] - v2[i];
    }
}

void vector_sum(number * v1, number* v2, number *result, int n) {
    int i;

#if GPU
#pragma acc parallel loop
#else
#pragma omp parallel for private(i)
#endif
    for( i = 0; i < n; i++ ) {
        result[i] = v1[i] + v2[i];
    }
}


void vector_product_constant(number * v1, number lambda, number * result, int n) {
    int i;

#if GPU
#pragma acc parallel loop
#else
#pragma omp parallel for private(i)
#endif
    for( i = 0; i < n; i++ ) {
        result[i] = lambda * v1[i];
    }
}

number vector_scalar_product(number * v1, number * v2, int n) {
    int i;
    number scalar;
    
    for ( i = 0, scalar = 0; i < n; i++ ) {
        scalar = scalar + v1[i] * v2[i]; // Not in parallel due to race conditions.
    }
    
    return scalar;
}

number vector_ang(number * v1, number * v2, int n) {
    number ang;

    ang = vector_scalar_product(v1, v2, n);
    ang = ang/(vector_norm2(v1, n) * vector_norm2(v2, n));

    return acos(ang);
}
