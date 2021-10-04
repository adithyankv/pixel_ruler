#include <gtk/gtk.h>
#include "window.h"
#include "overlay_window.h"

GtkWidget *create_main_window(GtkApplication *app) {
    GtkWidget *window;
    window = gtk_application_window_new(app);
    init_window_layout(GTK_APPLICATION_WINDOW (window));
    gtk_widget_set_size_request(window, 200, 200);
    return window;
}

void init_window_layout(GtkApplicationWindow *window) {
    GtkWidget *layout_box;
    GtkAdjustment *threshold_adjustment;
    GtkWidget *button, *threshold_scale;

    layout_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_margin_start(layout_box, 10);
    gtk_widget_set_margin_end(layout_box, 10);

    threshold_adjustment = gtk_adjustment_new(50, 0, 100, 50, 50, 0);
    threshold_scale = gtk_scale_new(GTK_ORIENTATION_HORIZONTAL, threshold_adjustment);
    gtk_scale_set_draw_value(GTK_SCALE (threshold_scale), FALSE);
    gtk_scale_add_mark(GTK_SCALE (threshold_scale), 0, GTK_POS_BOTTOM, "Low");
    gtk_scale_add_mark(GTK_SCALE (threshold_scale), 50, GTK_POS_BOTTOM, "Medium");
    gtk_scale_add_mark(GTK_SCALE (threshold_scale), 100, GTK_POS_BOTTOM, "High");

    button = gtk_button_new_with_label("start");
    g_signal_connect(button, "clicked", G_CALLBACK (create_overlay_window), NULL);

    gtk_box_pack_start(GTK_BOX (layout_box), threshold_scale, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX (layout_box), button, FALSE, FALSE, 10);
    gtk_container_add(GTK_CONTAINER (window), layout_box);
}
