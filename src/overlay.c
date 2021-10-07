#include <gtk/gtk.h>
#include "overlay.h"
#include "ruler.h"

static void on_mouse_moved(GtkWidget *drawing_area, GdkEvent *event, Ruler *ruler);
static void on_mouse_clicked(GtkWidget *overlay_window, GdkEvent *event, gpointer *data);
static void on_key_pressed(GtkWidget *overlay_window, GdkEvent *event, gpointer *data);
static void on_destroy(GtkWidget *overlay_window, Overlay *overlay);

Overlay *create_overlay(void) {
    Overlay *overlay = malloc(sizeof(Overlay));
    GtkWidget *overlay_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    overlay->overlay_window = overlay_window;

    Ruler *ruler = create_new_ruler();
    overlay->ruler = ruler;

    return overlay;
}

void draw_overlay_window(Overlay *overlay) {
    /* creating a fullscreen always on top, transparent window to draw on.
    *  A more elegant solution would be to draw on the X Overlay window, but
    *  that implementation doesn't seem to work with some distros/compositors.
    *  This implementation may however be susceptible to whims of window managers */
    GtkWidget *overlay_window = overlay->overlay_window;
    Ruler *ruler = overlay->ruler;
    gtk_widget_set_app_paintable(overlay_window, TRUE);
    gtk_window_fullscreen(GTK_WINDOW (overlay_window));
    gtk_window_set_keep_above(GTK_WINDOW (overlay_window), TRUE);
    g_signal_connect(G_OBJECT (overlay_window), "button-press-event",
                     G_CALLBACK (on_mouse_clicked), NULL);
    g_signal_connect(G_OBJECT (overlay_window), "key-press-event",
                     G_CALLBACK (on_key_pressed), NULL);

    GdkScreen *screen = gdk_screen_get_default();
    GdkVisual *visual = gdk_screen_get_rgba_visual(screen);

    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_widget_set_events(drawing_area, GDK_POINTER_MOTION_MASK | GDK_BUTTON_PRESS_MASK);
    gtk_widget_set_events(overlay_window, GDK_BUTTON_PRESS_MASK | GDK_KEY_PRESS_MASK);
    gtk_container_add(GTK_CONTAINER (overlay_window), drawing_area);

    if (visual != NULL && gdk_screen_is_composited(screen)) {
        gtk_widget_set_visual(overlay_window, visual);
        gtk_widget_show_all(overlay_window);
    }
    g_signal_connect (G_OBJECT (overlay_window), "destroy",
                      G_CALLBACK (on_destroy), (gpointer) ruler);
    g_signal_connect(G_OBJECT (drawing_area), "motion-notify-event",
                     G_CALLBACK (on_mouse_moved), (gpointer) ruler);

}

static void on_mouse_moved(GtkWidget *drawing_area, GdkEvent *event, Ruler *ruler) {
    if (event->type == GDK_MOTION_NOTIFY) {
        int mouse_x = event->motion.x;
        int mouse_y = event->motion.y;
        draw_ruler(ruler, drawing_area, mouse_x, mouse_y);
    }
}

static void on_key_pressed(GtkWidget *overlay_window, GdkEvent *event, gpointer *data) {
    if (event->type == GDK_KEY_PRESS) {
        switch (event->key.keyval) {
        /* close window on escape key */
        case GDK_KEY_Escape:
            gtk_widget_destroy (overlay_window);
            break;
        default:
            break;
        }
    }
}

static void on_mouse_clicked(GtkWidget *overlay_window, GdkEvent *event, gpointer *data) {
    if (event->type == GDK_BUTTON_PRESS) {
        switch (event->button.button) {
        case 1:
            g_print("left click\n");
            break;
        case 3:
            g_print("right click\n");
            break;
        default:
            g_print("button press");
            break;
        }
    }
}

static void on_destroy(GtkWidget *overlay_window, Overlay *overlay) {
    free(overlay->ruler->horizontal_ruler);
    free(overlay->ruler->vertical_ruler);
    free(overlay->ruler);
    free(overlay);
}
