#ifndef GTK_H_INCLUDED
#define GTK_H_INCLUDED
#include <gtk/gtk.h>
#endif /* GTK_H_INCLUDED */

typedef enum RulerOrientation {
    RULER_ORIENTATION_VERTICAL,
    RULER_ORIENTATION_HORIZONTAL
} RulerOrientation;

typedef struct SubRuler {
    RulerOrientation orientation;
    int start_coord, end_coord, x, y;
} SubRuler;

typedef struct Ruler {
    SubRuler *horizontal_ruler;
    SubRuler *vertical_ruler;
} Ruler;

void draw_ruler(Ruler *ruler, GtkWidget *drawing_area, int x, int y);
SubRuler *create_new_subruler(RulerOrientation orientation);
Ruler *create_new_ruler();

