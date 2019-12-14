#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <allegro.h>

#include "ptask.h"
#include "main.h"
#include "graphic.h"

int             k;                      // Character for keyboard input
int             num_pends;              // Number of pendulums in file

struct pendulum_t read_p[MAX_P];        // Struct MAX_P double pendulums in file
struct pendulum_t inborder_p[MAX_P];    // Struct MAX_P correct double pendulums


/* Initialization pendulums array inborder_p */
void init_pendulums()
{
    int i = 0;

    for (i = 0; i < MAX_P; i++) {
        inborder_p[i].id = -1;
        inborder_p[i].l1 = -1;
        inborder_p[i].l2 = -1;
        inborder_p[i].m1 = -1;
        inborder_p[i].m2 = -1;
        inborder_p[i].th1 = -1;
        inborder_p[i].th2 = -1;
        inborder_p[i].x0y0.x = -1;
        inborder_p[i].x0y0.y = -1;
    }
}


/* Transformation from degrees to radians of pendulums' angles */
void degree_to_radians(int index)
{
    read_p[index].th1 = (read_p[index].th1 * PI) / 180;
    read_p[index].th2 = (read_p[index].th2 * PI) / 180;
}


/* Reading parameters' file */
int read_file(FILE *f)
{
    int num_p = 0;          // Number of pendulums read
    int val = 0;            // Return value fscanf

    fscanf(f, "N = %d", &num_pends);
    if (num_pends > MAX_P) {
        return -1;
    }

    while(val != EOF) {
        val = fscanf(f, 
            "%d x0=%lf y0=%lf l1=%lf l2=%lf m1=%lf m2=%lf th1=%lf th2=%lf", 
            &read_p[num_p].id, &read_p[num_p].x0y0.x, &read_p[num_p].x0y0.y, 
            &read_p[num_p].l1, &read_p[num_p].l2, &read_p[num_p].m1, 
            &read_p[num_p].m2, &read_p[num_p].th1, &read_p[num_p].th2);

        if (val != EOF) {
            degree_to_radians(num_p);
            num_p++;
        }
    }

    return num_p;
}


/* Checking number of pendulums in file */
int check_read(int return_value)
{
    if (return_value == -1) {
        printf("Error! N = %d in the file must be lower then MAX_PENDS = %d!\n", 
                num_pends, MAX_P);
        return -1;
    }

    if (return_value != num_pends) {
        printf("Error! N = %d and rows' number in the file must match!\n", 
                num_pends);
        return -1;
    }

    return 0;
}


/* Checking pedulum lengths and borders gui */
void check_border()
{
    int radius = 0;         // length first rod + second rod
    int i, j = 0;           // Variables for structs double pendulums

    for (i = 0; i < MAX_P; i++) {
        radius = read_p[i].l1 + read_p[i].l2;

        // Check conditions inside borders gui:
        if ((read_p[i].x0y0.x - radius) >= 2 * PAD && 
            (read_p[i].x0y0.y - radius) >= 2 * PAD && 
            (read_p[i].x0y0.x + radius) <= (XWIN - 2 * PAD) && 
            (read_p[i].x0y0.y + radius) <= (YWIN - 2 * PAD))
        {
            inborder_p[j] = read_p[i];
            j++;
        }
        else {
            if (i < num_pends) {
                printf("Pendulum %d not shown because out of borders!\n", i);
            }
        }
    }
}

/* Setting exit variable to 1 and waiting for join */
void check_join()
{
    int i;
    int local_pid[num_pends + 1];

    start_writer();
    shared_mem.end = 1;
    end_writer();

    // Retrieve all pid from shared memory:
    for (i = 0; i < num_pends + 1; i++) {
        start_reader();
        local_pid[i] = shared_mem.pid[i];
        end_reader();
    }

    // Wait all the tasks to end:
    for (i = 0; i < num_pends + 1; i++) {
        pthread_join(ptask_get_threadid(local_pid[i]), 0);
    }
}

int main(void)
{
    int r_value = 0;        // Return value for read_file function
    int press = 0;          // Variable for avoid pressing ENTER more times 

    init_allegro();
    init_gui();
    init_pendulums();
    
    ptask_init(SCHED_FIFO, GLOBAL, NO_PROTOCOL);

    FILE *fp = fopen(NAMEFILE, "r");
    if (fp == NULL) {
        printf("Error during opening file!\n");
        return EXIT_FAILURE;
    }

    r_value = read_file(fp);
    if (check_read(r_value) == -1) {
        return EXIT_FAILURE;
    }

    check_border();
    fclose(fp);

    do {
        if (keypressed()) {
            k = readkey() >> 8;
            // If press ENTER, pendulums start:
            if (k == KEY_ENTER && press == 0) {
                manager();
                press = 1;
            }
        }
    } while (k != KEY_ESC);

    if (press != 0) { check_join(); }
    allegro_exit();
    return EXIT_SUCCESS;
}