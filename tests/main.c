#include "../src/gpnl_calculus.h"
#include "../src/gpnl_minimize.h"
#include "../src/gpnl_edo.h"
#include "../src/gpnl_graphics.h"
#include "../src/gpnl_vector.h"
#include "../src/gpnl_matrix.h"

int main(int argc, char ** args) {
  
    ///////////////////////////////////////////////////////
    /// Plotting
    ///////////////////////////////////////////////////////
    number test_function3(number * x) {
        return x[0]*x[0] * sin(1/x[0]);
    }

    graphics_plot2d gplot2d;
    
    graphics_create_plot2d("Test Plot2d", &gplot2d);
    graphics_add_plot2d(test_function3, 255, 0, 0,  &gplot2d);
    graphics_launch_plot2d(gplot2d);
}
