#ifndef GRAPHICS_H
#define GRAPHICS_H

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

/* Graphic task */
ptask graphic();

#endif