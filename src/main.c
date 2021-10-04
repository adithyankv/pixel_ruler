#include <gtk/gtk.h>

void activate(GtkApplication *app) {
    GtkWidget *window;
    window = gtk_application_window_new(app);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {

    GtkApplication *app;
    app = gtk_application_new("com.github.adithyan-kv.pixelruler",
                              G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);

    int status;
    status = g_application_run(G_APPLICATION (app), argc, argv);
    g_object_unref(app);

    return status;
}
