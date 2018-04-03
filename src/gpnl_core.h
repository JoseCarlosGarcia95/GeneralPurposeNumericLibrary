#ifndef _CORE_H_
#define _CORE_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#define VERBOSE 1
#define GPU 0

typedef long double number;
typedef number (*gpnl_function)(number*);

#endif
