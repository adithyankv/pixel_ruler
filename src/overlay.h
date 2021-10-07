#ifndef GTK_H_INCLUDED
#define GTK_H_INCLUDED

#include <gtk/gtk.h>

#endif /* GTK_H_INCLUDED */

typedef struct Overlay {
    GtkWidget *overlay_window;
} Overlay;

Overlay *create_overlay(void);
void draw_overlay_window(Overlay *overlay);
