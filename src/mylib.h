#ifndef MYLIB_H
#define MYLIB_H

#include "ptask.h"

//------------------------------------------------------------------------------
// GLOBAL CONSTANTS
//------------------------------------------------------------------------------

#define NAMEFILE    "params.txt"    // Name file parameters pendulums
#define N           50              // Max number of pendulums
#define XWIN        1280            // Window X resolution
#define YWIN        800             // Window Y resolution
#define GRAVITY     9.81            // Gravitational constant
#define PI          3.14159         // Pi greco


//------------------------------------------------------------------------------
// STRUCTURE DEFINITIONS
//------------------------------------------------------------------------------

/* Structure that represents a 2D point */
struct point_t {
    int     x;              // X position
    int     y;              // Y position
};

/* Structure that represents all X,Y trajectry points*/
struct trajectory_t {
    int     x[N];           // X trajectory points
    int     y[N];           // Y trajectory points
};

/* Structure that represents a double pendulum */
struct pendulum_t {
    int     id;             // ID pendulum
    double  l1;             // Length first rod
    double  l2;             // Length second rod
    double  m1;             // Mass first pendulum
    double  m2;             // Mass second pendulum
    double  th1;            // Angle between vertical and first rod
    double  th2;            // Angle between vertical and second rod
}; 

/* Double pendulums buffer */
struct pendulum_t pendulums[N];

/* Shared memory structure definitions */
struct mem_t {
    int     check_pendulum[N];      // Checking presence of pendulums
    
    struct trajectory_t ptrack;     // Pendulum trajectory

    sem_t sem_read, sem_write;      // Semaphores for reader and writer tasks
    sem_t mutex;
};

/* Shared memory structure */
struct mem_t shared_mem;

//------------------------------------------------------------------------------
// FUNCTIONS
//------------------------------------------------------------------------------

/* Initialization shared memory */
void init_shared_mem(struct mem_t *mem);


#endif