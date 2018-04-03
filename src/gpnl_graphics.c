#include "gpnl_graphics.h"



void graphics_create_plot2d(const char * plot_name, graphics_plot2d * gplot2d) {
    
    gplot2d->plot_name = malloc(strlen(plot_name));
    gplot2d->functions_size = 0;
    gplot2d->rulemm = 50;
    gplot2d->movement_size = 5;
    gplot2d->rule_size = 2;
    gplot2d->functions = malloc(0);
    gplot2d->colors = malloc(0);
    gplot2d->function_precision = 0.0001;
    strcpy(gplot2d->plot_name, plot_name);
}

void graphics_launch_plot2d(graphics_plot2d gplot2d) {
	// Initialize glut and make some tricks.
    int argc = 1, offset_y, offset_x;
    char ** argv;
    float  pixel_conversor;
            
    offset_y = offset_x = 0;
    
    argv = (char**)malloc(sizeof(char*));
    argv[0] = malloc(strlen(gplot2d.plot_name) + 1);

    strcpy(argv[0], gplot2d.plot_name);
    
    glutInit(&argc, argv);

    // Define window.
    pixel_conversor = (1.0* glutGet( GLUT_SCREEN_WIDTH_MM )) / glutGet( GLUT_SCREEN_WIDTH );
    glutInitWindowSize(720, 720);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow(gplot2d.plot_name);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable (GL_BLEND);
 
	glEnable (GL_LINE_SMOOTH);
	glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
    
    glClearColor( 1, 1, 1, 1);

    
    void renderScene(void) {
        int i, j;
        float p0, p1, window_width, window_height;
        gpnl_function func;
        number x, *t, x1, x2, dx;

        t = malloc(sizeof(number));

        dx = gplot2d.function_precision;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw edges.
        window_width = pixel_conversor*glutGet( GLUT_WINDOW_WIDTH );
        window_height = pixel_conversor*glutGet( GLUT_WINDOW_HEIGHT );

        glLineWidth(1);
        glColor3f(29.0/255, 102.0/255, 173.0/255);
        
        glBegin(GL_LINES);
        glVertex2f(-1, offset_y/window_height*gplot2d.movement_size);
        glVertex2f(1, offset_y/window_height*gplot2d.movement_size);
        glEnd();
        
        glBegin(GL_LINES);
        glVertex2f(offset_x/window_width*gplot2d.movement_size, -1);
        glVertex2f(offset_x/window_width*gplot2d.movement_size, 1);
        glEnd();

        
        for(i = offset_y; i < (window_height+offset_y*gplot2d.movement_size)/gplot2d.rulemm || i-offset_y < (window_height+offset_y*gplot2d.movement_size)/gplot2d.rulemm; i++) {
            // hacky trick but works.
            if( i-offset_y < (window_height+offset_y*gplot2d.movement_size)/gplot2d.rulemm)
                p0 = (1.0*(i-offset_y))/window_height*gplot2d.rulemm;
            else
                p0 = i/window_height*gplot2d.rulemm;
            
            p1 = -p0;
            
            glBegin(GL_LINES);
            glVertex2f(offset_x/window_width*gplot2d.movement_size-1.0/window_width*gplot2d.rule_size, p0+offset_y/window_height*gplot2d.movement_size);
            glVertex2f(offset_x/window_width*gplot2d.movement_size+1.0/window_width*gplot2d.rule_size, p0+offset_y/window_height*gplot2d.movement_size);
            glEnd();
            
            glBegin(GL_LINES);
            glVertex2f(offset_x/window_width*gplot2d.movement_size-1.0/window_width*gplot2d.rule_size, p1+offset_y/window_height*gplot2d.movement_size);
            glVertex2f(offset_x/window_width*gplot2d.movement_size+1.0/window_width*gplot2d.rule_size, p1+offset_y/window_height*gplot2d.movement_size);
            
            glEnd();
        }

        for(i = offset_x; i < (window_width+offset_x*gplot2d.movement_size)/gplot2d.rulemm || i - offset_x < (window_width+offset_x*gplot2d.movement_size)/gplot2d.rulemm; i++) {
            if(i - offset_x < (window_width+offset_x*gplot2d.movement_size)/gplot2d.rulemm)
                p0 = (1.0*(i-offset_x))/window_width*gplot2d.rulemm;
            else
                p0 = (1.0*i)/window_width*gplot2d.rulemm;

            p1 = -p0;
            
            glBegin(GL_LINES);
            glVertex2f(offset_x/window_width*gplot2d.movement_size+p0, offset_y/window_height*gplot2d.movement_size-1.0/window_height*gplot2d.rule_size);
            glVertex2f(offset_x/window_width*gplot2d.movement_size+p0, offset_y/window_height*gplot2d.movement_size+1.0/window_height*gplot2d.rule_size);
            glEnd();
            
            glBegin(GL_LINES);
            glVertex2f(offset_x/window_width*gplot2d.movement_size+p1, offset_y/window_height*gplot2d.movement_size-1.0/window_height*gplot2d.rule_size);
            glVertex2f(offset_x/window_width*gplot2d.movement_size+p1, offset_y/window_height*gplot2d.movement_size+1.0/window_height*gplot2d.rule_size);
            
            glEnd();
        }


        // Print graphics.
        glLineWidth(2.5);

        for(j = 0; j < gplot2d.functions_size;j++) {
            func = gplot2d.functions[j];
            x1 = (-window_width-offset_x*gplot2d.movement_size)/gplot2d.rulemm;
            x2 = (window_width-offset_x*gplot2d.movement_size)/gplot2d.rulemm;

            glColor3f(gplot2d.colors[j][0], gplot2d.colors[j][1], gplot2d.colors[j][2]);

            glBegin(GL_LINE_STRIP);

            
            for(x = x1; x < x2; x += dx) {
                t[0] = x;
                glVertex2f(+offset_x/window_width*gplot2d.movement_size+x/window_width*gplot2d.rulemm, +offset_y/window_height*gplot2d.movement_size+func(t)/window_height*gplot2d.rulemm);
            }
            
            glEnd();
            
        }

        glutSwapBuffers();
    }

    void keySpecial (int key, int x, int y) {  
        switch(key) {
        case 100: // left
            offset_x++;
            break;
        case 101: // up
            offset_y--;
            break;
        case 102: // right
            offset_x--;
            break;
        case 103: // down
            offset_y++;
            break;
        }

        glutPostRedisplay(); 
    }

    void keyPressed (unsigned char key, int x, int y) {
        switch(key) {
        case 43:
            //if(gplot2d.rulemm <= 270)
            gplot2d.rulemm *= 1.5;
            break;
        case 45:
            if(gplot2d.rulemm >= 25)
                gplot2d.rulemm /= 1.5;
            break;
        case 48:
            offset_y = offset_x = 0;
        }
        
        glutPostRedisplay(); 

    }

    void eventMouse(int button, int state, int x, int y)
    {
        float window_width, window_height, graph_x, graph_y;
        
        window_width = glutGet( GLUT_WINDOW_WIDTH );
        window_height = glutGet( GLUT_WINDOW_HEIGHT );

        if(button == 4) {
            offset_y--;
        } else if(button == 3) {
            offset_y++;
        } else if(button == 0 && state == 0) {
            // TODO
        }
        glutPostRedisplay(); 
    }

	glutDisplayFunc(renderScene);
    glutSpecialFunc(keySpecial);
    glutKeyboardFunc(keyPressed);
    glutMouseFunc(eventMouse);
    
    glutMainLoop();

    
}

void graphics_add_plot2d(gpnl_function function, int r, int g, int b, graphics_plot2d * gpl2d) {
    gpl2d->functions_size++;

    gpl2d->functions = realloc(gpl2d->functions, sizeof(gpnl_function)*gpl2d->functions_size);
    gpl2d->colors = realloc(gpl2d->colors, sizeof(float*) * gpl2d->functions_size);
    gpl2d->functions[gpl2d->functions_size - 1] = function;

    gpl2d->colors[gpl2d->functions_size - 1] = malloc(sizeof(float) * 3);
    gpl2d->colors[gpl2d->functions_size - 1][0] = 1.0*r/255;
    gpl2d->colors[gpl2d->functions_size - 1][1] = 1.0*g/255;    
    gpl2d->colors[gpl2d->functions_size - 1][2] = 1.0*b/255;

    
}
