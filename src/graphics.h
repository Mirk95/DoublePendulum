#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "mylib.h"

//------------------------------------------------------------------------------
// GLOBAL CONSTANTS
//------------------------------------------------------------------------------

#define BKG     0       // VGA code backgroud color -> Black
#define WHITE   15      // VGA code white color 

//------------------------------------------------------------------------------
// FUNCTIONS
//------------------------------------------------------------------------------

/* Initialization of Allegro libray */
void init_allegro();

/* Initial graphic interface */
void init_gui();

/* Initial pendulums graphic interface after ENTER pressing */
void init_pendulums_gui();

/* Retrieve Shared Memory parameters */
void retrieve_sm(struct point_t *g_x0y0, struct point_t *g_x1y1, 
                struct point_t *g_x2y2, struct cbuf_t *t);

/* Draw past values of trajectory pendulum i */
void draw_trail(int color, struct cbuf_t t);

/* Graphic task */
ptask graphic();


#endif