#include <semaphore.h>

#include "ptask.h"
#include "mylib.h"
#include "graphics.h"

/* Initialization shared memory */
void init_shared_mem(struct mem_t *mem)
{
    int i;
    mem->count_pendulums = 0;
    mem->end = 0;

    mem->nr = 0;
    mem->nbr = 0;
    mem->nw = 0;
    mem->nbw = 0;

    for (i = 0; i < N; ++i) {
        // No pendulum at the beginning
        mem->check_pendulum[i] = 0;
    }

    for (i = 0; i < N + 1; ++i) {
        // No task at the beginning
        mem->pid[i] = -1;
    }

    for (i = 0; i < 2; ++i) {
        // Current point and previous one initialized to -1
        mem->xy[i].x = -1;
        mem->xy[i].y = -1; 
    }

    sem_init(&mem->mutex, 0, 1);
    sem_init(&mem->sem_read, 0, 0);
    sem_init(&mem->sem_write, 0, 0);
}

/* Check end to terminate task */
int check_end()
{
    int end;

    start_reader();
    end = shared_mem.end;
    end_reader();

    return end;
}

/* Preamble phase writer tasks */
void start_writer()
{
    sem_wait(&shared_mem.mutex);
    if (shared_mem.nr > 0 || shared_mem.nw > 0) {
        // If someone is reading or is writing, I must to block!
        shared_mem.nbw++;
    }
    else {
        // If nobody is reading or writing, I can write
        shared_mem.nw++;
        sem_post(&shared_mem.sem_write);
    }
    // Always release the mutex, before blocking!
    sem_post(&shared_mem.mutex);
    sem_wait(&shared_mem.sem_write);
}

/* Postamble phase writer tasks */
void end_writer()
{
    sem_wait(&shared_mem.mutex);
    shared_mem.nw--;
    if (shared_mem.nbr > 0) {
        // If there are blocked readers, I wake up all them
        // because they can read simultaneously.
        while (shared_mem.nbr > 0) {
            shared_mem.nbr--;
            shared_mem.nr++;
            sem_post(&shared_mem.sem_read);
        }
    }
    else if (shared_mem.nbw > 0) {
        // If there are blocked writers, I wake up one writer
        shared_mem.nbw--;
        shared_mem.nw++;
        sem_post(&shared_mem.sem_write);
    }
    sem_post(&shared_mem.mutex);
}

/* Preamble phase reader tasks */
void start_reader()
{
    sem_wait(&shared_mem.mutex);
    if (shared_mem.nw > 0 || shared_mem.nbw > 0) {
        // If someone is reading or writing, I must to block!
        shared_mem.nbr++;
    }
    else {
        // If nobody is reading or writing, I can read
        shared_mem.nr++;
        sem_post(&shared_mem.sem_read);
    }
    // Always release the mutex, before blocking!
    sem_post(&shared_mem.mutex);
    sem_wait(&shared_mem.sem_read);
}

/* Postamble phase reader tasks */
void end_reader()
{
    sem_wait(&shared_mem.mutex);
    shared_mem.nr--;
    if (shared_mem.nbw > 0 && shared_mem.nr == 0) {
        // If there are blocked writers and no readers, I can wake up one writer
        shared_mem.nbw--;
        shared_mem.nw++;
        sem_post(&shared_mem.sem_write);
    }
    sem_post(&shared_mem.mutex);
}

/*
void fill_array(int id)
{
    int i;
    start_writer();
    shared_mem.check_pendulum[id] = 1;
    for (i = 0; i < N; ++i) {
        printf("check_pendulum[%i] = %i\n", i, shared_mem.check_pendulum[i]);
    }
    end_writer();
}

void read_array()
{
    int i;
    start_reader();
    for (i = 0; i < N; ++i) {
        if (shared_mem.check_pendulum[i] == 1) {
            shared_mem.count_pendulums++;
        }
    }
    //printf("Counter of ones: %i\n", shared_mem.count_pendulums);
    shared_mem.count_pendulums = 0;
    end_reader();
}
*/

/* Initialization of task's parameters */
tpars init_param(int priority, int period)
{
    tpars params = TASK_SPEC_DFL;

    params.period = tspec_from(period, MILLI);
    params.rdline = tspec_from(period, MILLI);
    params.priority = priority;
    params.measure_flag = 1;
    params.act_flag = NOW;
    params.arg = NULL;
    return params;
}

/* Manager for the program */
void manager()
{
    int pid_g;                      // Graphic task index
    static tpars params;            // Parameters for graphic task

    // Initialization of the shared memory
    init_shared_mem(&shared_mem);
    // Create graphic task
    params = init_param(PRIO_G, PER_G);
    pid_g = ptask_create_param(graphic, &params);
    
    start_writer();
    shared_mem.pid[0] = pid_g;
    end_writer();
}



