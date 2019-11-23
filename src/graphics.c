#include <allegro.h>
#include <stdio.h>

#include "ptask.h"
#include "graphics.h"
#include "mylib.h"
#include "main.h"

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
    char text[MSG_LEN];      // Temp buffer for text message

    rect(screen, PAD, PAD, XWIN - PAD, YWIN - PAD, WHITE);
    rect(screen, 2 * PAD, 2 * PAD, XWIN - 2 * PAD, YWIN - 2 * PAD, WHITE);
    sprintf(text, "DOUBLE PENDULUMS VIEWER");
    textout_centre_ex(screen, font, text, XWIN/2, YWIN/2 - 30, WHITE, BKG);
    sprintf(text, "Press ENTER to show pendulums");
    textout_centre_ex(screen, font, text, XWIN/2, YWIN/2, WHITE, BKG);
    sprintf(text, "ESC to exit");
    textout_centre_ex(screen, font, text, XWIN/2, YWIN/2 + 30, WHITE, BKG);
}

/* Initialendulums graphic interface after ENTER pressing */
void init_pendulums_gui()
{
    clear_to_color(screen, BKG);
    rect(screen, PAD, PAD, XWIN - PAD, YWIN - PAD, WHITE);
}

/* Graphic task */
ptask graphic()
{
    int i;
    int end = 0;

    while (!end) {
        end = check_end();
        init_pendulums_gui();

        for (i = 0; i < MAX_P; ++i) {
            if (check_pendulum[i] != 0) {
                start_reader();
                circlefill(screen, shared_mem.x0y0[i].x, shared_mem.x0y0[i].y, 5, i+1);
                line(screen, shared_mem.x1y1[i].x, shared_mem.x1y1[i].y, 
                            shared_mem.x0y0[i].x, shared_mem.x0y0[i].y, i+1);
                circlefill(screen, shared_mem.x1y1[i].x, shared_mem.x1y1[i].y, 5, i+1);
                line(screen, shared_mem.x2y2[i].x, shared_mem.x2y2[i].y, 
                            shared_mem.x1y1[i].x, shared_mem.x1y1[i].y, i+1);
                circlefill(screen, shared_mem.x2y2[i].x, shared_mem.x2y2[i].y, 5, i+1);
                end_reader();
            }
        }

        ptask_wait_for_period();
    }
}
