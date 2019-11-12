#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <allegro.h>

#include "ptask.h"
#include "main.h"
#include "graphics.h"
#include "mylib.h"



// Reading parameter's file
int read_file()
{
    FILE *fp = fopen(NAMEFILE, "r");
    if (fp == NULL) {
        // Error during opening file
        return -1;
    }

    int a;
    float b;
    fscanf(fp, "%i %f", &a, &b);
    printf("a: %i, b: %f", a, b);

    fclose(fp);
    return 0;
}

int main(void)
{
    init_gui();
    
    if (read_file() != 0) {
        printf("Error during opening file\n");
        return EXIT_FAILURE;
    }

    allegro_exit();
    sleep(5);
    return EXIT_SUCCESS;
}