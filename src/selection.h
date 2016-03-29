#ifndef SELECTION_H
#define SELECTION_H

typedef struct rect {
    float x0;
    float y0;
    float x1;
    float y1;
} rect;

typedef enum selection_state {
    empty,
    started,
    ready
} selection_state;

typedef struct selection {
    rect sel;
    selection_state state;
      
} selection;


selection *sel_new();
void sel_reset(selection*);
void sel_click(selection*, float, float);
rect *sel_box(selection*);



#endif
