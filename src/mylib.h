#ifndef MYLIB_H
#define MYLIB_H

#include "ptask.h"

//------------------------------------------------------------------------------
// GLOBAL CONSTANTS
//------------------------------------------------------------------------------

#define NAMEFILE    "params.txt"    // Name file parameters pendulums
#define XWIN        1280            // Window X resolution
#define YWIN        800             // Window Y resolution
#define PAD         30              // Padding for initial gui
#define MSG_LEN     50              // Max message length 
#define N           5               // Max number of pendulums
#define GRAVITY     9.81            // Gravitational constant
#define PI          3.14159         // Pi greco
#define PER_G       35              // Period graphic task
#define PRIO_G      20              // Priority graphic task
#define PER_P       90              // Period pendulum task
#define PRIO_P      10              // Priority pendulum task

//------------------------------------------------------------------------------
// STRUCTURE DEFINITIONS
//------------------------------------------------------------------------------

/* Structure that represents a 2D point */
struct point_t {
    double      x;              // X position
    double      y;              // Y position
};

/* Structure that represents N double pendulums */
struct pendulum_t {
    int             id;             // ID pendulum
    double          l1;             // Length first rod
    double          l2;             // Length second rod
    double          m1;             // Mass first pendulum
    double          m2;             // Mass second pendulum
    double          th1;            // Angle between vertical and first rod
    double          th2;            // Angle between vertical and second rod
    struct point_t  x0y0;           // Initial point pendulum
};

// struct pendulum_t  pendulums[N];

/* Shared memory structure definitions */
struct mem_t {
    int         check_pendulum[N];  // Checking presence of pendulums
    int         count_pendulums;    // Counter active pendulums
    int         end;                // Exit variable: no more tasks
    int         pid[N + 1];         // Array with tasks' indexes
    
    struct point_t x0y0[N];         // Position point (x0, y0) -> initial point
    struct point_t x1y1[N];         // Position point (x1,y1) -> first mass
    struct point_t x2y2[N];         // Position point (x2,y2) -> second mass

    int     nr;                     // Number of active readers
    int     nbr;                    // Number of blocked readers
    int     nw;                     // Number of active writers
    int     nbw;                    // Number of blocked writers

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

/* Check end to terminate task */
int check_end();

/* Preamble phase writer tasks */
void start_writer();

/* Postamble phase writer tasks */
void end_writer();

/* Preamble phase reader tasks */
void start_reader();

/* Postamble phase reader tasks */
void end_reader();

/* Computation acceleration first pendulum */
double compute_acceleration1(struct pendulum_t p, double v1, double v2);

/* Computation acceleration second pendulum */
double compute_acceleration2(struct pendulum_t p, double v1, double v2);

/* Initialization of task's parameters */
tpars init_param(int priority, int period);

/* Pendulum task */
ptask pend();

/* Manager for the program */
void manager();


#endif