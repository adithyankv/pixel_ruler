#include <gtk/gtk.h>
#include "window.h"

void activate(GtkApplication *app) {
    GtkWidget *window = create_main_window(app);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {

    GtkApplication *app = gtk_application_new("com.github.adithyan-kv.pixelruler",
                          G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);

    int status = g_application_run(G_APPLICATION (app), argc, argv);
    g_object_unref(app);

    return status;
}
