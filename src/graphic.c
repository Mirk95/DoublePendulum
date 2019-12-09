#include <allegro.h>
#include <stdio.h>

#include "ptask.h"
#include "graphic.h"
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

/* Initialization graphic interface */
void init_gui()
{
    int i;

    for (i = 1; i < 8; i++) {
        rect(screen, i * PAD, i * PAD, XWIN - i * PAD, YWIN - i * PAD, WHITE);
    }

    sprintf(text, "DOUBLE PENDULUMS VIEWER");
    textout_centre_ex(screen, font, text, XWIN/2, YWIN/2 - 30, WHITE, BKG);
    sprintf(text, "Press ENTER to show double pendulums");
    textout_centre_ex(screen, font, text, XWIN/2, YWIN/2, WHITE, BKG);
    sprintf(text, "ESC to exit");
    textout_centre_ex(screen, font, text, XWIN/2, YWIN/2 + 30, WHITE, BKG);
    sprintf(text, "Developed by Mirko Cavecchia");
    textout_centre_ex(screen, font, text, XWIN/2, YWIN/2 + 60, WHITE, BKG);
}

/* Initial pendulums graphic interface after ENTER pressing */
void init_pendulums_gui()
{
    clear_to_color(screen, BKG);
    rect(screen, PAD, PAD, XWIN - PAD, YWIN - PAD, WHITE);
    rect(screen, 2 * PAD, 2 * PAD, XWIN - 2 * PAD, YWIN - 2 * PAD, WHITE);
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

/* Draw past values of trajectory pendulum */
void draw_trail(int color, struct cbuf_t t)
{
    int j, k = 0;
    double x, y = 0;

    for (j = 0; j < TLEN; j++) {
        if (t.top != -1) {
            k = (t.top + TLEN - j) % TLEN;
            x = t.x[k];
            y = t.y[k];
            if (x == -1 && y == -1) {
                circlefill(screen, x, y, 1, BKG);
            }
            else {
                circlefill(screen, x, y, 1, color + 9);
            }
        }
    }
}

/* Check deadline miss graphic task */
void check_deadline_miss_g()
{
    if (ptask_deadline_miss()) {
            printf("Graphic task has missed deadline!\n");
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
                circlefill(screen, 0, 0, 1, BKG);
            }
        }

        check_deadline_miss_g();
        ptask_wait_for_period();
    }
}
