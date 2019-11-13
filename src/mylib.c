#include <semaphore.h>

#include "mylib.h"

/* Initialization shared memory */
void init_shared_mem(struct mem_t *mem)
{
    int i;

    for (i = 0; i < N; ++i) {
        // Array initialized to 0 -> No pendulum at the beginning
        mem->check_pendulum[i] = 0;
        mem->ptrack.x[i] = -1;
        mem->ptrack.y[i] = -1;
    }


    sem_init(&mem->mutex, 0, 1);
    sem_init(&mem->sem_read, 0, 0);
    sem_init(&mem->sem_write, 0, 0);
}