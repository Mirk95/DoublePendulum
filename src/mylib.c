#include <semaphore.h>
#include <allegro.h>
#include <math.h>

#include "ptask.h"
#include "mylib.h"
#include "graphics.h"
#include "main.h"

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
        mem->x0y0[i].x = -1;
        mem->x0y0[i].y = -1;
        mem->x1y1[i].x = -1;
        mem->x1y1[i].y = -1;
        mem->x2y2[i].x = -1;
        mem->x2y2[i].y = -1;
    }

    for (i = 0; i < N + 1; ++i) {
        // No task at the beginning
        mem->pid[i] = -1;
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

/* Computation acceleration first pendulum */
double compute_acceleration1(struct pendulum_t p, double v1, double v2)
{
    double num1 = 0;            // Formula first part numerator
    double num2 = 0;            // Formula second part numerator
    double num3 = 0;            // Formula third part numerator
    double num4 = 0;            // Formula fouth part numerator
    double den = 0;             // Formula denominator
    double res = 0;             // Formula result

    num1 = - GRAVITY * (2 * p.m1 + p.m2) * sin(p.th1);
    num2 = - p.m2 * GRAVITY * sin(p.th1 - 2 * p.th2);
    num3 = -2 * sin(p.th1 - p.th2) * p.m2;
    num4 = v2 * v2 * p.l2 + v1 * v1 * p.l1 * cos(p.th1 - p.th2);
    den = p.l1 * (2 * p.m1 + p.m2 - p.m2 * cos(2 *p.th1 - 2 * p.th2));
    res = (num1 + num2 + num3 * num4) / den;

    return res;
}

/* Computation acceleration second pendulum */
double compute_acceleration2(struct pendulum_t p, double v1, double v2)
{
    double num1 = 0;            // Formula first part numerator
    double num2 = 0;            // Formula second part numerator
    double num3 = 0;            // Formula third part numerator
    double num4 = 0;            // Formula fouth part numerator
    double den = 0;             // Formula denominator
    double res = 0;             // Formula result

    num1 = 2 * sin(p.th1 - p.th2);
    num2 = (v1 * v1 * p.l1 * (p.m1 + p.m2));
    num3 = GRAVITY * (p.m1 + p.m2) * cos(p.th1);
    num4 = v2 * v2 * p.l2 * p.m2 * cos(p.th1 - p.th2);
    den = p.l2 * (2 * p.m1 + p.m2 - p.m2 * cos(2 * p.th1 - 2 * p.th2));
    res = (num1 * (num2 + num3 + num4)) / den;

    return res;
}

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

/* Pendulum task */
ptask pend()
{
    int end = 0;
    int index = 0;
    int count = 0;
    double vel_1 = 0;
    double vel_2 = 0;
    double acc_1 = 0;
    double acc_2 = 0;
    struct pendulum_t p;

    index = ptask_get_index();
    printf("Index: %d\n", index);
    p.id = np[index - 1].id;
    p.l1 = np[index - 1].l1;
    p.l2 = np[index - 1].l2;
    p.m1 = np[index - 1].m1;
    p.m2 = np[index - 1].m2;
    p.th1 = np[index - 1].th1;
    p.th2 = np[index - 1].th2;
    p.x0y0.x = np[index - 1].x0y0.x;
    p.x0y0.y = np[index - 1].x0y0.y;

    start_writer();
    shared_mem.x0y0[index - 1].x = p.x0y0.x;
    shared_mem.x0y0[index - 1].y = p.x0y0.y;
    end_writer();

    while (!end) {
        check_end();

        acc_1 = compute_acceleration1(p, vel_1, vel_2);
        acc_2 = compute_acceleration2(p, vel_1, vel_2);
        printf("acc_1 for pend %d: %lf\n", index - 1, acc_1);
        printf("acc_2 for pend %d: %lf\n", index -1, acc_2);
        count++;

        start_writer();
        shared_mem.x1y1[index - 1].x = shared_mem.x0y0[index - 1].x + p.l1 * sin(p.th1);
        shared_mem.x1y1[index - 1].y = shared_mem.x0y0[index - 1].y + p.l1 * cos(p.th1);
        shared_mem.x2y2[index - 1].x = shared_mem.x1y1[index - 1].x + p.l2 * sin(p.th2);
        shared_mem.x2y2[index - 1].y = shared_mem.x1y1[index - 1].y + p.l2 * cos(p.th2);
        end_writer();

        if (abs(vel_1) < 30 || abs(vel_2) < 30) {
            printf("Ciao\n");
            vel_1 += acc_1;
            vel_2 += acc_2;
        }


        printf("vel_1 for pend %d: %lf\n", index - 1, vel_1);
        printf("vel_2 for pend %d: %lf\n", index - 1, vel_2);
        printf("angle1 before for pend %d: %lf\n", index - 1, p.th1);
        printf("angle2 before for pend %d: %lf\n", index - 1, p.th2);
        p.th1 += vel_1;
        p.th2 += vel_2;
        p.th1 = (fmod(p.th1, 2*PI));
        p.th2 = (fmod(p.th2, 2*PI));


        if (abs(p.th1) > 2*PI) {
            printf("ESCO DALL'ANGOLO\n");
            p.th1 = p.th1 - ((p.th1 >= 0 ) ? 1 : -1) * 2*PI;
        } 

        if (abs(p.th2) > 2*PI) {
            printf("ESCO DALL'ANGOLO\n");
            p.th2 = p.th2 - ((p.th2 >= 0 ) ? 1 : -1 )* 2*PI;
        } 

        printf("angle1 after for pend %d: %lf\n", index - 1, p.th1);
        printf("angle2 after for pend %d: %lf\n", index - 1, p.th2);


        printf("Count: %d\n", count);

        ptask_wait_for_period();
    }
}


/* Manager for the program */
void manager()
{
    int pid_g, pid_p;               // Graphic task index
    static tpars params;            // Parameters for graphic task
    int i;

    // Initialization of the shared memory
    init_shared_mem(&shared_mem);
    // Create graphic task
    params = init_param(PRIO_G, PER_G);
    pid_g = ptask_create_param(graphic, &params);
    start_writer();
    shared_mem.pid[0] = pid_g;
    end_writer();
    // Create pendulum task
    params = init_param(PRIO_P, PER_P);
    for (i = 0; i < N; ++i) {
        pid_p = ptask_create_param(pend, &params);
        start_writer();
        shared_mem.pid[i + 1] = pid_p;
        end_writer();
    }

}



