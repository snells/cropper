#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "selection.h"
#include "render.h"
#include "files.h"

#define RED al_map_rgb(255, 0, 0)
#define GREEN al_map_rgb(0, 255, 0)
#define BLUE al_map_rgb(0, 0, 255)
#define GRAY al_map_rgb(128, 128, 128)
#define DARK_GRAY al_map_rgb(60, 60, 60)
#define WHITE al_map_rgb(255, 255, 255)
#define BLACK al_map_rgb(0, 0, 0)
#define BOX_COLOR al_map_rgb(60, 60, 60)


typedef enum render_state {
  file_selection,
  image_view
} render_state;


selection *sel;
ALLEGRO_BITMAP *image = 0;
char current_file[1024];
int image_w;
int image_h;
render_state state = file_selection;
ALLEGRO_FONT *default_font;
dir_cnt *dir;

int render_init()
{
    sel = sel_new();
    default_font = al_load_ttf_font("default.ttf", 16, 0);
    if(!default_font) {
      fputs("Couldn't load default font", stderr);
      return -1;
    }
    puts("render init");
    char arr[1024];
    if(!getcwd(arr, 1024))
      return -1;
    dir = dir_cnt_new(arr);
    if(!dir)
      return -1;
    return 0;
}


void load_img(char *name)
{
  if(image)
    al_destroy_bitmap(image);

  image = al_load_bitmap(name);
  if(image) {
    image_w = al_get_bitmap_width(image);
    image_h = al_get_bitmap_height(image);
    }
}

void draw_box_text(const char *str, float x, float y, float w, float h, ALLEGRO_COLOR color)
{
  int len = strlen(str);
  al_draw_filled_rectangle(x, y, x + w, y + h, color);
  al_draw_rectangle(x, y, x + w, y + h, color, 0);
  int tw = al_get_text_width(default_font, str);
  int th = al_get_font_line_height(default_font);
  al_draw_text(default_font, WHITE, x + w / 2, y + h / 2 - th / 2, ALLEGRO_ALIGN_CENTRE, str);
}


ALLEGRO_COLOR selection_color()
{
  return al_map_rgb(60, 60, 160);
}



int selected = 0;
int file_count = 0;
int cols = 0;
int rows = 0;
int box_w = 200;
int box_h = 100;

void row_down()
{
  int ns = selected + cols;
  if(ns > file_count -1) {
    selected = file_count - 1;
    return;
  }
  selected = ns;
}

void row_up()
{
  int ns = selected - cols;
  if(ns < 0) {
    selected = 0;
    return;
  }
  selected = ns;
}

void move_left()
{
  int ns = selected - 1;
  selected = (ns < 0) ? 0 : ns;
}

void move_right()
{
  int ns = selected + 1;
  selected = (ns > file_count -1) ? file_count - 1 : ns;
}

void open_file(char *fp)
{
  if(is_dir(fp)) {
    if(dir_cnt_to(dir, fp))
      selected = ;
    return;
  }
  load_img(fp);
  if(image) {
    state = image_view;
  }
}

void keydown(int k)
{
  switch(k) {
  case ALLEGRO_KEY_Q:
    if(state == image_view)
      state = file_selection;
    else {
      dir_cnt_up(dir);
      selected = 0;
    }
    break;
  case ALLEGRO_KEY_W:
    if(state == file_selection)
      row_up();
    break;
  case ALLEGRO_KEY_R:
    if(state == file_selection)
      row_down();
    break;
  case ALLEGRO_KEY_A:
    if(state == file_selection)
      move_left();
    break;
  case ALLEGRO_KEY_S:
    if(state == file_selection)
      move_right();
    break;
  case ALLEGRO_KEY_F:
    if(state == file_selection) {
      if(selected < 0 || selected >= dir->clen)
        break;
      char fp[1024];
      strcpy(fp, dir->path);
      strcat(fp, dir->cnt[selected]);
      puts(fp);
      open_file(fp);
    //dir_cnt_to(dir, selected);
    }
    break;
  }
}

int clamp(int val, int min, int max)
{
  if(val < min)
    return min;
  if(val > max)
    return max;
  return val;
}
int min(int x, int y)
{
  return (x < y) ? x : y;
}

void render_screen(ALLEGRO_DISPLAY *dsp)
{
  int sw = al_get_display_width(dsp);
  int sh = al_get_display_height(dsp);
  int swm = sw / 2;
  int shm = sh / 2;
  file_count = dir->clen;
  if(state == file_selection) {
    int bvc = sh / box_h;
    rows = bvc;
    int bhc = sw / box_w;
    cols = bhc;
    int btot = bhc * bvc;
    int pad_w = (sw - (box_w * bhc)) / 2;
    int pad_h = (sh - (box_h * bvc)) / 2;
    int bstart = btot * (selected / btot);
    int n = 0;
    int nh = -1;
    for(; bstart < dir->clen; bstart++) {
      if(!(n % bhc))
        nh++;
      draw_box_text(dir->cnt[bstart],
                    pad_w + box_w * (n % bhc),
                    pad_h + box_h * nh,
                    box_w, box_h,
                    n == (selected % btot) ? selection_color() : BOX_COLOR);
      n++;
    }
    return;
    }


  if(state == image_view) {
    if(image) {
      al_draw_scaled_bitmap(image, 0, 0, image_w, image_h, 0, 0, al_get_display_width(dsp),
                            al_get_display_height(dsp), 0);
    }
  }
}




void draw_menu(ALLEGRO_DISPLAY *dsp)
{
  
}
