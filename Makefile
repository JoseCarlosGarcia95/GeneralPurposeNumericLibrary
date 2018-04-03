all: vector matrix calculus minimize edo graphics test

libs=-lm -fopenmp # -fopenacc
vector:
	gcc -o build/gpnl_vector.o -c src/gpnl_vector.c $(libs)
matrix:
	gcc -o build/gpnl_matrix.o -c src/gpnl_matrix.c $(libs)
calculus: 
	gcc -o build/gpnl_calculus.o -c src/gpnl_calculus.c	$(libs)
minimize: 
	gcc -o build/gpnl_minimize.o -c src/gpnl_minimize.c $(libs)
edo: 
	gcc -o build/gpnl_edo.o -c src/gpnl_edo.c $(libs)
graphics:
	gcc -o build/gpnl_graphics.o -c src/gpnl_graphics.c $(libs) -lGL -lGLU -lglut
test:
	gcc tests/main.c build/gpnl_vector.o build/gpnl_matrix.o build/gpnl_calculus.o build/gpnl_minimize.o build/gpnl_edo.o build/gpnl_graphics.o -o build/test $(libs) -lGL -lGLU -lglut
