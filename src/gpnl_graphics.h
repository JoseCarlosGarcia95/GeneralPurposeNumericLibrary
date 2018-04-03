#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "gpnl_core.h"

#include <GL/gl.h>
#include <GL/freeglut.h>

typedef struct {
    char * plot_name;
    float rulemm;
    float function_precision;
    float rule_size;
    float movement_size;
    unsigned int functions_size;
    gpnl_function * functions;
    float ** colors;
    
} graphics_plot2d;

void graphics_create_plot2d(const char *,  graphics_plot2d *);
void graphics_add_plot2d(gpnl_function, int, int, int, graphics_plot2d * gpl2d);
void graphics_launch_plot2d(graphics_plot2d);
#endif

