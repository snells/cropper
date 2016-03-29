#ifndef RENDER_H
#define RENDER_H

#include <allegro5/allegro.h>

void render_screen(ALLEGRO_DISPLAY *dsp);
int render_init();
void load_img(char*);
void keydown(int keycode);
#endif
