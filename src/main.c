#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <allegro.h>

#include "ptask.h"
#include "main.h"
#include "graphics.h"
#include "mylib.h"

static int      k = 0;      // Character for keyboard input
struct pendulum_t p;        // Struct double pendulum
struct pendulum_t np[N];    // Struct N double pendulums


/* Transformation from degrees to radians of pendulum i */
void degree_to_radians(int i)
{
    np[i].th1 = (np[i].th1 * PI) / 180;
    np[i].th2 = (np[i].th2 * PI) / 180;
}


/* Reading parameters' file */
int read_file(FILE *f)
{
    int i;
    int ret;

    i = 0;
    while(ret != EOF && i < N) {
        ret = fscanf(f, "%i %lf %lf %lf %lf %lf %lf", &np[i].id, &np[i].l1, &np[i].l2, 
                                &np[i].m1, &np[i].m2, &np[i].th1, &np[i].th2);
        if (ret != EOF) {
            degree_to_radians(i);
            i++;
        }
    }

    return i;
}



int main(void)
{
    int n_pend;     // Return value for read_file  

    init_allegro();
    init_gui();

    FILE *fp = fopen(NAMEFILE, "r");
    if (fp == NULL) {
        // Error during opening file
        return EXIT_FAILURE;
    }

    n_pend = read_file(fp);
    fclose(fp);

    for (int i = 0; i < n_pend; ++i) {
        printf("ID: %i\n", np[i].id);
        printf("l1: %lf\n", np[i].l1);
        printf("l2: %lf\n", np[i].l2);
        printf("m1: %lf\n", np[i].m1);
        printf("m2: %lf\n", np[i].m2);
        printf("th1: %lf\n", np[i].th1);
        printf("th2: %lf\n", np[i].th2);
    }

    do {
        if (keypressed()) {
            k = readkey() >> 8;
            // If press ENTER, new pendulum starts.
            if (k == KEY_ENTER) {
                printf("PRESS ENTER\n");
            }
        }
    } while (k != KEY_ESC);

    allegro_exit();
    return EXIT_SUCCESS;
}