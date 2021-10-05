#ifndef GTK_H_INCLUDED
#define GTK_H_INCLUDED
#include <gtk/gtk.h>
#endif /* GTK_H_INCLUDED */

typedef struct VerticalRuler {
    int top_y, bottom_y, x;
} VerticalRuler;

typedef struct HorizontalRuler {
    int top_x, bottom_x, y;
} HorizontalRuler;

typedef struct Ruler {
    HorizontalRuler *hruler;
    VerticalRuler *vruler;
} Ruler;

void draw_ruler(GtkWidget *drawing_area, int x, int y);

