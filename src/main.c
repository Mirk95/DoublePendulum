#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <allegro.h>

#include "ptask.h"
#include "main.h"
#include "graphics.h"
#include "mylib.h"

int             k = 0;                  // Character for keyboard input
int             num_pends = 0;          // Number of pendulums in file
int             check_pendulum[MAX_P];  // Array for checking presence pendulums

struct pendulum_t np[MAX_P];            // Struct MAX_P double pendulums


/* Initialization array check_pendulum */
void init_check_pendulum()
{
    int i = 0;

    for (i = 0; i < MAX_P; ++i) {
        check_pendulum[i] = 0;
    }
}


/* Transformation from degrees to radians of pendulum i */
void degree_to_radians(int i)
{
    np[i].th1 = (np[i].th1 * PI) / 180;
    np[i].th2 = (np[i].th2 * PI) / 180;
}


/* Reading parameters' file */
int read_file(FILE *f)
{
    int i = 0;
    int val = 0;

    fscanf(f, "N = %d", &num_pends);
    printf("Valore di N: %d\n", num_pends);
    if (num_pends > MAX_P) {
        return -1;
    }

    while(val != EOF) {
        val = fscanf(f, 
            "%d x0=%lf y0=%lf l1=%lf l2=%lf m1=%lf m2=%lf th1=%lf th2=%lf", 
            &np[i].id, &np[i].x0y0.x, &np[i].x0y0.y, &np[i].l1, &np[i].l2, 
            &np[i].m1, &np[i].m2, &np[i].th1, &np[i].th2);

        if (val != EOF) {
            check_pendulum[i] = 1;
            degree_to_radians(i);
            i++;
        }
    }

    return i;
}


/* Checking number pendulums in file */
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
    int radius = 0;

    for (int i = 0; i < MAX_P; ++i) {
        if (check_pendulum[i] != 0) {
            radius = np[i].l1 + np[i].l2;
            if ((np[i].x0y0.x - radius) < PAD || 
                (np[i].x0y0.y - radius) < PAD) {
                // Pendulum exits from left border or from top border
                check_pendulum[i] = 0;
                printf("Pendulum %d not shown because out of borders!\n", i+1);
            }

            if ((np[i].x0y0.x + radius) > (XWIN - PAD) || 
                (np[i].x0y0.y + radius) > (YWIN - PAD)) {
                // Pendulum exits from right border or from bottom border
                check_pendulum[i] = 0;
                printf("Pendulum %d not shown because out of borders!\n", i+1);
            }
        }
    }
}

int main(void)
{
    int r_value = 0;        // Return value for read_file function
    int i = 0;              // Variable for cycle  

    init_allegro();
    init_gui();
    init_check_pendulum();
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

    for (i = 0; i < MAX_P; ++i) {
        if (check_pendulum[i] != 0) {
            printf("ID: %i\n", np[i].id);
            printf("x0: %lf\n", np[i].x0y0.x);
            printf("y0: %lf\n", np[i].x0y0.y);
            printf("l1: %lf\n", np[i].l1);
            printf("l2: %lf\n", np[i].l2);
            printf("m1: %lf\n", np[i].m1);
            printf("m2: %lf\n", np[i].m2);
            printf("th1: %lf\n", np[i].th1);
            printf("th2: %lf\n", np[i].th2);
        }
    }

    do {
        if (keypressed()) {
            k = readkey() >> 8;
            // If press ENTER, pendulums start.
            if (k == KEY_ENTER) {
                manager();     // Shared memory initialization
            }
        }
    } while (k != KEY_ESC);

    allegro_exit();
    return EXIT_SUCCESS;
}