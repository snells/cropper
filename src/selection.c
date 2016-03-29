#include "selection.h"
#include <stdlib.h>

selection *sel_new()
{
    selection *s = malloc(sizeof(selection));
    return s;
}

void sel_reset(selection *s)
{
    s->state = empty;
    s->sel.x0 = 0;
    s->sel.x1 = 0;
    s->sel.y0 = 0;
    s->sel.y1 = 0;
}

void sel_click(selection *s, float x, float y)
{
    if(s->state == empty) {
	s->state = started;
	s->sel.x0 = x;
	s->sel.y0 = y;
    }
    else if(s->state == started) {
	s->state = ready;
	s->sel.x1 = x;
	s->sel.y1 = y;
    }
    else {
	s->state = started;
	s->sel.x0 = x;
	s->sel.y0 = y;
    }
}

rect *sel_box(selection *s)
{
    return &(s->sel);
}
