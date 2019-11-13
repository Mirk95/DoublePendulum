#include <allegro.h>

#include "graphics.h"
#include "mylib.h"

/* Initialization of Allegro libray */
void init_allegro()
{
    allegro_init();
    install_keyboard();
    set_color_depth(8);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, XWIN, YWIN, 0, 0);
    clear_to_color(screen, BKG);
}

/* Initialization of graphic interface */
void init_gui()
{
    
}