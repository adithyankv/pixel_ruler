#ifndef OVERLAY_H_INCLUDED
#define OVERLAY_H_INCLUDED

#ifndef GTK_H_INCLUDED
#define GTK_H_INCLUDED
#include <gtk/gtk.h>
#endif /* GTK_H_INCLUDED */

#ifndef RULER_H_INCLUDED
#include "ruler.h"
#endif /* RULER_H_INCLUDED */

#ifndef MEASUREMENT_H_INCLUDED
#include "measurement.h"
#endif

typedef struct Overlay {
    GtkWidget *overlay_window;
    Ruler *ruler;
    MeasurementList *measurement_list;
} Overlay;

Overlay *create_overlay(void);
void draw_overlay_window(Overlay *overlay);

#endif /* OVERLAY_H_INCLUDED */
