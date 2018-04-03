#include "gpnl_matrix.h"

void matrix_sum(number ** m1, number ** m2, number ** result, unsigned int w,unsigned int h) {
    unsigned int i, j;
#if GPU
#pragma acc parallel loop collapse(2)
#else
#pragma omp parallel for collapse(2)
#endif
    for( i = 0; i < w; i++) {
        for( j = 0; j < h; j++ ) {
            result[i][j] = m1[i][j] + m2[i][j];
        }
    }
}


void matrix_multiply(number ** m1, number ** m2, number ** result, unsigned int w, unsigned int h) {
    unsigned int i, j, k;
#if GPU
#pragma acc parallel loop collapse(3)
#else
#pragma omp parallel for collapse(3)
#endif
    for( i = 0; i < w; i++) {
        for( j = 0; j < h; j++ ) {
            for ( k = 0; k < w; k++) {
                if( k = 0 ) result[i][j] = 0;
                result[i][j] += m1[k][j]* m2[i][j];
            }
        }
    }
}

number matrix_determinant_swap(unsigned long *x, unsigned long *y) {
    unsigned long tmp;
    tmp = *x;
    *x = *y;
    *y = tmp;
}

void matrix_determinant_calculate_permutations(unsigned long **permutations, unsigned long * a, unsigned long l, unsigned long r, unsigned long * k, unsigned long n) {
    unsigned long i;

    if ( l == r ) {
        for(i = 0; i < n; i++) {
            permutations[*k][i] = a[i];
        }
        (*k)++;
    } else {
        for( i = l; i <= r; i++ ) {
            matrix_determinant_swap((a+l), (a+i));
            matrix_determinant_calculate_permutations(permutations, a, l+1, r, k, n);
            matrix_determinant_swap((a+l), (a+i));

        }
    }
}

int matrix_determinante_calculate_permutation_inversion_size(unsigned long * permutation, unsigned long n) {
    long unsigned i, j;
    int count;
    
    for(i=0, count=0;i<n-1;i++)  {  
        for(j=i+1;j<n;j++)  {
            if(permutation[i] > permutation[j]) 
                count++;
        }
    }
    
    return count;
}

void matrix_lu_decomposition(number ** m, number ** l, number ** u, unsigned long n, unsigned long * s) {
    
}

number matrix_determinant(number ** m, unsigned int n) {
    // http://www.staff.science.uu.nl/~bisse101/Book/PSC/psc2_3.pdf
    unsigned long i, j, ** permutations, * permutations_basis;
    
    unsigned long permutation_size = 1;

    for( i = 2;i <= n; i++) {
        permutation_size *= i;
    }

    number * determinant_part, determinant, product;

    permutations = malloc(sizeof(long*) * permutation_size);
    permutations_basis = malloc(sizeof(long) * n);
    determinant_part = malloc(sizeof(number) * permutation_size);

    for( i = 0; i < n; i++ ) {
        permutations_basis[i] = i + 1;
    }
    for( i = 0; i < permutation_size; i++) {
        permutations[i] = malloc(sizeof(long*) * n);
    }

    j = 0;
    matrix_determinant_calculate_permutations(permutations, permutations_basis, 0, n-1, &j, n);

    for( i = 0, determinant = 0;i < permutation_size; i++ ) {

        if(matrix_determinante_calculate_permutation_inversion_size(permutations[i], n) % 2) {
            product = -1;
        } else {
            product = 1;
        }
        for( j = 0; j < n; j++) {
            product *= m[j][permutations[i][j]-1];
        }

        determinant += product;
    }

    free(permutations);
    free(permutations_basis);
    free(determinant_part);
    
    return determinant;
}
