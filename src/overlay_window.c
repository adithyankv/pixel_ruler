#include <gtk/gtk.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xcomposite.h>
#include <cairo.h>
#include <cairo/cairo-xlib.h>
#include <unistd.h>

void create_overlay_window() {
    Window overlay_window, root_window;
    Display *display;

    display = XOpenDisplay(NULL);
    int screen_id = DefaultScreen(display);
    int screen_height, screen_width;
    Visual *default_visual;
    screen_height = DisplayHeight(display, screen_id);
    screen_width = DisplayWidth(display, screen_id);
    default_visual = DefaultVisual(display, screen_id);
    root_window = RootWindow(display, screen_id);
    overlay_window = XCompositeGetOverlayWindow(display, root_window);

    cairo_t *cr_context;
    cairo_surface_t *cr_surface;
    cr_surface = cairo_xlib_surface_create(display, overlay_window,
                                           default_visual,
                                           screen_width, screen_height);
    cr_context = cairo_create(cr_surface);

    cairo_set_source_rgb(cr_context, 1, 0, 0);
    cairo_move_to(cr_context, 0, 0);
    cairo_line_to(cr_context, 1920, 1080);
    cairo_stroke(cr_context);

    sleep(4);
}
