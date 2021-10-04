#include <gtk/gtk.h>
#include "overlay.h"

void create_overlay_window() {
    /* creating a fullscreen always on top, transparent window to draw on.
    A more elegant solution would be to draw on the X Overlay window, but
    that implementation doesn't seem to work with some distros/compositors */

    /* This implementation may however be susceptible to whims of window managers */
    GtkWidget *overlay_window;
    overlay_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_app_paintable(overlay_window, TRUE);
    gtk_window_fullscreen(GTK_WINDOW (overlay_window));
    gtk_window_set_keep_above(GTK_WINDOW (overlay_window), TRUE);

    GdkScreen *screen;
    GdkVisual *visual;
    screen = gdk_screen_get_default();
    visual = gdk_screen_get_rgba_visual(screen);

    if (visual != NULL && gdk_screen_is_composited(screen)) {
        gtk_widget_set_visual(overlay_window, visual);
        gtk_widget_show_all(overlay_window);
    }

}
