#ifndef MYLIB_H
#define MYLIB_H

#include "ptask.h"

//------------------------------------------------------------------------------
// GLOBAL CONSTANTS
//------------------------------------------------------------------------------

#define NAMEFILE    "params.txt"    // Name file parameters pendulums
#define XWIN        1280            // Window X resolution
#define YWIN        800             // Window Y resolution
#define PAD         20              // Padding for initial gui
#define MSG_LEN     50              // Max message length 
#define MAX_P       7               // Max number of pendulums
#define TSCALE      0.1             // Time scale factor
#define PI          3.14159         // Pi greco
#define TLEN        50              // Trajectory length
#define PER_G       50              // Period graphic task
#define PRIO_G      20              // Priority graphic task
#define PER_P       20              // Period pendulum task
#define PRIO_P      10              // Priority pendulum task


//------------------------------------------------------------------------------
// STRUCTURE DEFINITIONS
//------------------------------------------------------------------------------

/* Structure that represents a 2D point */
struct point_t {
    double      x;              // X position
    double      y;              // Y position
};

/* Structure that represents circular buffer for trajectory */
struct cbuf_t {
    int         top;            // Index of the current element
    double      x[TLEN];        // Array of x coordinates
    double      y[TLEN];        // Array of y coordinates
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

/* Shared memory structure definitions */
struct mem_t {
    int         end;                // Exit variable: no more tasks
    int         pid[MAX_P + 1];     // Array with tasks' indexes
    
    struct point_t x0y0[MAX_P];     // Position point (x0,y0) -> zero point
    struct point_t x1y1[MAX_P];     // Position point (x1,y1) -> first mass
    struct point_t x2y2[MAX_P];     // Position point (x2,y2) -> second mass
    struct cbuf_t trail[MAX_P];     // Array for trajectory pendulums

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

/* Retrieve parameters pendulum i */
void retrieve_p(struct pendulum_t *p, int i);

/* Update positions points pendulum i*/
void update_positions(struct pendulum_t p, int i);

/* Initialization of task's parameters */
tpars init_param(int priority, int period);

/* Check Deadline Miss */
void check_deadline_miss();

/* Pendulum task */
ptask pend();

/* Manager for the program */
void manager();


#endif