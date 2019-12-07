#ifndef MAIN_H
#define MAIN_H

#include "mylib.h"

struct pendulum_t inborder_p[MAX_P];    // Struct MAX_P correct double pendulums


//------------------------------------------------------------------------------
// FUNCTIONS
//------------------------------------------------------------------------------

/* Initialization pendulums array inborder_p */
void init_pendulums();

/* Transformation from degrees to radians of pendulum i */
void degree_to_radians(int i);

/* Reading parameter's file */
int read_file(FILE *f);

/* Checking number pendulums in file */
int check_read(int return_value);

/* Checking pedulum lengths and borders gui */
void check_border();

/* Set exit variable to 1 and waits for join */
void check_join();

#endif