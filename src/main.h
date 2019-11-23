#ifndef MAIN_H
#define MAIN_H

#include "mylib.h"

//------------------------------------------------------------------------------
// FUNCTIONS
//------------------------------------------------------------------------------

int             check_pendulum[MAX_P];  // Array for checking presence pendulums
struct pendulum_t np[MAX_P];            // Struct MAX_P double pendulums

/* 
 *  Initialization array check_pendulum:
 *  check_pendulum[i] = 0;  -> Pendulum i is not present
 *  check_pendulum[i] = 1;  -> Pendulum i is present
 */
void init_check_pendulum();

/* Transformation from degrees to radians */
void degree_to_radians();

/* Reading parameter's file */
int read_file();

/* Checking pedulum lengths and borders gui */
void check_border();

#endif