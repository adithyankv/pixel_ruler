#ifndef GTK_H_INCLUDED
#define GTK_H_INCLUDED
#include <gtk/gtk.h>
#endif /* GTK_H_INCLUDED */

GtkWidget *create_main_window(GtkApplication *app);
void init_window_layout(GtkApplicationWindow *window);
