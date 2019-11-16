#ifndef MYLIB_H
#define MYLIB_H

#include "ptask.h"

//------------------------------------------------------------------------------
// GLOBAL CONSTANTS
//------------------------------------------------------------------------------

#define NAMEFILE    "params.txt"    // Name file parameters pendulums
#define N           5               // Max number of pendulums
#define XWIN        1280            // Window X resolution
#define YWIN        800             // Window Y resolution
#define PAD         30              // Padding for initial gui
#define MSG_LEN     50              // Max message length 
#define GRAVITY     9.81            // Gravitational constant
#define PI          3.14159         // Pi greco
#define PER_G       25              // Period graphic task
#define PRIO_G      10              // Priority graphic task

//------------------------------------------------------------------------------
// STRUCTURE DEFINITIONS
//------------------------------------------------------------------------------

/* Structure that represents a 2D point */
struct point_t {
    int     x;              // X position
    int     y;              // Y position
};

/* Structure that represents N double pendulums */
struct pendulum_t {
    int     id;             // ID pendulum
    int     x0;             // X initial point
    int     y0;             // Y initial point
    double  l1;             // Length first rod
    double  l2;             // Length second rod
    double  m1;             // Mass first pendulum
    double  m2;             // Mass second pendulum
    double  th1;            // Angle between vertical and first rod
    double  th2;            // Angle between vertical and second rod
} pendulums[N]; 


/* Shared memory structure definitions */
struct mem_t {
    int     check_pendulum[N];      // Checking presence of pendulums
    int     count_pendulums;        // Counter active pendulums
    int     end;                    // Exit variable: no more tasks
    int     pid[N + 1];             // Array with tasks' indexes
    
    struct point_t xy[2];           // Positions current point and previous one 

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

/* Initialization of task's parameters */
tpars init_param(int priority, int period);

/* Manager for the program */
void manager();


#endif