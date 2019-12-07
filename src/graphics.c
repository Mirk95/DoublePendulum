#include <allegro.h>
#include <stdio.h>

#include "ptask.h"
#include "graphics.h"
#include "mylib.h"
#include "main.h"

char text[MSG_LEN];      // Temp buffer for text message


/* Initialization of Allegro libray */
void init_allegro()
{
    allegro_init();
    set_color_depth(8);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, XWIN, YWIN, 0, 0);
    clear_to_color(screen, BKG);
    install_keyboard();
}

/* Initial graphic interface */
void init_gui()
{
    rect(screen, PAD, PAD, XWIN - PAD, YWIN - PAD, WHITE);
    rect(screen, 2 * PAD, 2 * PAD, XWIN - 2 * PAD, YWIN - 2 * PAD, WHITE);
    sprintf(text, "DOUBLE PENDULUMS VIEWER");
    textout_centre_ex(screen, font, text, XWIN/2, YWIN/2 - 30, WHITE, BKG);
    sprintf(text, "Press ENTER to show pendulums");
    textout_centre_ex(screen, font, text, XWIN/2, YWIN/2, WHITE, BKG);
    sprintf(text, "ESC to exit");
    textout_centre_ex(screen, font, text, XWIN/2, YWIN/2 + 30, WHITE, BKG);
}

/* Initial pendulums graphic interface after ENTER pressing */
void init_pendulums_gui()
{
    clear_to_color(screen, BKG);
    rect(screen, PAD, PAD, XWIN - PAD, YWIN - PAD, WHITE);
    //rect(screen, 2 * PAD, 2 * PAD, XWIN - 2 * PAD, YWIN - 2 * PAD, WHITE);
}

/* Retrieve Shared Memory parameters */
void retrieve_sm(struct point_t *g_x0y0, struct point_t *g_x1y1, 
                struct point_t *g_x2y2, struct cbuf_t *t)
{
    int i, j;
    for (i = 0; i < MAX_P; i++) {
        if (inborder_p[i].id != -1) {
            start_reader();
            g_x0y0[i].x = shared_mem.x0y0[i].x;
            g_x0y0[i].y = shared_mem.x0y0[i].y;
            g_x1y1[i].x = shared_mem.x1y1[i].x;
            g_x1y1[i].y = shared_mem.x1y1[i].y;
            g_x2y2[i].x = shared_mem.x2y2[i].x;
            g_x2y2[i].y = shared_mem.x2y2[i].y;
            t[i].top = shared_mem.trail[i].top;
            for (j = 0; j < TLEN; j++) {
                t[i].x[j] = shared_mem.trail[i].x[j];
                t[i].y[j] = shared_mem.trail[i].y[j];
            }
            end_reader();
        }
    }
}

/* Draw past values of trajectory pendulum i */
void draw_trail(int color, struct cbuf_t t)
{
    int j, k = 0;
    double x, y = 0;

    for (j = 0; j < TLEN; j++) {
        if (t.top != -1) {
            // printf("t.top = %d\n", t.top);
            k = (t.top + TLEN - j) % TLEN;
            x = t.x[k];
            y = t.y[k];
            if (x == -1 && y == -1) {
                circlefill(screen, x, y, 1, BKG);
            }
            else {
                // printf("t.x[%d] = %lf\n", k, x);
                // printf("t.y[%d] = %lf\n", k, y);
                circlefill(screen, x, y, 1, color + 9);
            }
            // circlefill(screen, x, y, 2, i + 1);
            // circlefill(screen, x + 10, y + 10, 2, 15);

            // }
            // else {
            //     x_prev = t.x[k - 1];
            //     y_prev = t.y[k - 1];
            //     putpixel(screen, x_prev, y_prev, i + 1);
            //     x = t.x[k];
            //     y = t.y[k];
            //     putpixel(screen, x, y, i + 1);
            //     line(screen, x, y, x_prev, y_prev, i + 1);
            // }
        }
    }
}

/* Graphic task */
ptask graphic()
{
    int i;
    int end = 0;
    struct point_t graph_x0y0[MAX_P];
    struct point_t graph_x1y1[MAX_P];
    struct point_t graph_x2y2[MAX_P];
    struct cbuf_t trajectory[MAX_P];

    while (!end) {
        end = check_end();
        init_pendulums_gui();

        retrieve_sm(graph_x0y0, graph_x1y1, graph_x2y2, trajectory);

        for (i = 0; i < MAX_P; i++) {
            if (inborder_p[i].id != -1) {
                // printf("check_pendulum[%d] = %d\n", i, check_pendulum[i]);
                // printf("graph_x0y0 x[%d]: %lf\n", i, graph_x0y0[i].x);
                // printf("graph_x0y0 y[%d]: %lf\n", i, graph_x0y0[i].y);
                // printf("graph_x1y1 x[%d]: %lf\n", i, graph_x1y1[i].x);
                // printf("graph_x1y1 y[%d]: %lf\n", i, graph_x1y1[i].y);
                // printf("graph_x2y2 x[%d]: %lf\n", i, graph_x2y2[i].x);
                // printf("graph_x2y2 y[%d]: %lf\n", i, graph_x2y2[i].y);
                circlefill(screen, graph_x0y0[i].x, graph_x0y0[i].y, 5, i+1);
                sprintf(text, "%d", inborder_p[i].id);
                textout_centre_ex(screen, font, text, graph_x0y0[i].x, 
                                graph_x0y0[i].y - 20, WHITE, BKG);
                line(screen, graph_x1y1[i].x, graph_x1y1[i].y, 
                            graph_x0y0[i].x, graph_x0y0[i].y, i+9);
                circlefill(screen, graph_x1y1[i].x, graph_x1y1[i].y, 5, i+1);
                line(screen, graph_x2y2[i].x, graph_x2y2[i].y, 
                            graph_x1y1[i].x, graph_x1y1[i].y, i+9);
                circlefill(screen, graph_x2y2[i].x, graph_x2y2[i].y, 5, i+1);
                draw_trail(i, trajectory[i]);
            }
        }

        if (ptask_deadline_miss()) {
            printf("Graphic task has missed deadline!\n");
        }
        ptask_wait_for_period();
    }
}
