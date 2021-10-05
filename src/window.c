#include <gtk/gtk.h>
#include "window.h"
#include "overlay.h"

GtkWidget *create_main_window(GtkApplication *app) {
    GtkWidget *window;
    window = gtk_application_window_new(app);
    init_window_layout(GTK_APPLICATION_WINDOW (window));
    gtk_widget_set_size_request(window, 200, 200);
    return window;
}

void init_window_layout(GtkApplicationWindow *window) {
    GtkWidget *layout_box;
    GtkWidget *button;
    layout_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    button = gtk_button_new_with_label("start");
    g_signal_connect(G_OBJECT (button), "clicked", G_CALLBACK (create_overlay_window), NULL);

    gtk_widget_set_margin_start(layout_box, 10);
    gtk_widget_set_margin_end(layout_box, 10);

    gtk_box_pack_start(GTK_BOX (layout_box), button, FALSE, FALSE, 10);
    gtk_container_add(GTK_CONTAINER (window), layout_box);
}
