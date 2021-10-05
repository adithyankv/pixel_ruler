#include <cairo.h>
#include <gdk/gdk.h>
#include "ruler.h"

static void draw_horizontal_ruler(HorizontalRuler ruler, cairo_t *cairo_context);
static void draw_vertical_ruler(VerticalRuler ruler, cairo_t *cairo_context);
static void clear_drawing_area(cairo_t *cairo_context);

void draw_ruler(GtkWidget *drawing_area, int x, int y) {

    GdkWindow *window;
    window = gtk_widget_get_window(drawing_area);
    cairo_region_t *region;
    region = gdk_window_get_visible_region(window);
    GdkDrawingContext *surface;
    surface = gdk_window_begin_draw_frame(window, region);
    cairo_t *cairo_context;
    cairo_context = gdk_drawing_context_get_cairo_context(surface);

    HorizontalRuler h_ruler;
    h_ruler.y = y;
    VerticalRuler v_ruler;
    v_ruler.x = x;

    clear_drawing_area(cairo_context);
    draw_horizontal_ruler(h_ruler, cairo_context);
    draw_vertical_ruler(v_ruler, cairo_context);

    gdk_window_end_draw_frame(window, surface);
}

static void draw_horizontal_ruler(HorizontalRuler ruler, cairo_t *cairo_context) {
    cairo_set_operator(cairo_context, CAIRO_OPERATOR_SOURCE);
    cairo_set_source_rgb(cairo_context, 1, 0, 0);
    cairo_move_to(cairo_context, 0, ruler.y);
    cairo_line_to(cairo_context, 1920, ruler.y);
    cairo_stroke(cairo_context);
}

static void draw_vertical_ruler(VerticalRuler ruler, cairo_t *cairo_context) {
    cairo_set_operator(cairo_context, CAIRO_OPERATOR_SOURCE);
    cairo_set_source_rgb(cairo_context, 1, 0, 0);
    cairo_move_to(cairo_context, ruler.x, 0);
    cairo_line_to(cairo_context, ruler.x, 1080);
    cairo_stroke(cairo_context);
}

static void clear_drawing_area(cairo_t *cairo_context) {
    cairo_set_operator(cairo_context, CAIRO_OPERATOR_CLEAR);
    cairo_paint(cairo_context);
}
