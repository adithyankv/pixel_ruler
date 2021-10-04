#ifndef GTK_H_DEFINED
#define GTK_H_DEFINED
#include <gtk/gtk.h>
#endif /* GTK_H_DEFINED */

GtkWidget *create_main_window(GtkApplication *app);
void init_window_layout(GtkApplicationWindow *window);
