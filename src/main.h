#ifndef MAIN_H
#define MAIN_H

#include "mylib.h"

//------------------------------------------------------------------------------
// FUNCTIONS
//------------------------------------------------------------------------------

struct pendulum_t np[N];

// Reading parameter's file
int read_file();

// Transformation from degrees to radians
void degree_to_radians();

#endif