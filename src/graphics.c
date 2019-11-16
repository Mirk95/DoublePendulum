#include <allegro.h>
#include <stdio.h>

#include "ptask.h"
#include "graphics.h"
#include "mylib.h"

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
    sprintf(text, "Press SPACE to create a new pendulum");
    textout_centre_ex(screen, font, text, XWIN/2, YWIN/2, WHITE, 0);
    sprintf(text, "ESC to exit");
    textout_centre_ex(screen, font, text, XWIN/2, YWIN/2 + 30, WHITE, 0);
}

/* Graphic task */
ptask graphic()
{
    int i = 0;
    int end = 0;

    while(!end) {
        check_end();
        clear_to_color(screen, BKG);
        rect(screen, PAD, PAD, XWIN - PAD, YWIN - PAD, WHITE);
        circlefill(screen, 150+i, 150+i, 10, 14);
        i++;
        ptask_wait_for_period();
    }
}