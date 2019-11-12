#ifndef MYLIB_H
#define MYLIB_H

//------------------------------------------------------------------------------
// GLOBAL CONSTANTS
//------------------------------------------------------------------------------

#define NAMEFILE    "parameters.txt"    // name file parameters pendulums
#define XWIN        1280                // window x resolution
#define YWIN        800                 // window y resolution
#define GRAVITY     9.81                // gravity acceleration

//------------------------------------------------------------------------------
// STRUCTURE DEFINITIONS
//------------------------------------------------------------------------------

// Structure that represent a double pendulum
struct pendulum_t {
    int     id;             // ID pendulum
    float   l1;             // length first pendulum
    float   l2;             // length second pendulum
    float   m1;             // mass first weigth
    float   m2;             // mass second weight
    float   teta1;          // angle between vertical and first branch
    float   teta2;          // angle between vertical and second branch
}; 

#endif