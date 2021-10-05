#include <cairo.h>
#include <gdk/gdk.h>
#include "ruler.h"

static void draw_subruler(SubRuler ruler, cairo_t *cairo_context);
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

    GdkCursor *cursor;
    GdkDisplay *display;
    display = gdk_display_get_default();
    cursor = gdk_cursor_new_from_name(display, "crosshair");
    gdk_window_set_cursor(window, cursor);

    SubRuler horizontal_ruler;
    horizontal_ruler.orientation = RULER_ORIENTATION_HORIZONTAL;
    horizontal_ruler.position = y;
    horizontal_ruler.start_coord = 0;
    horizontal_ruler.end_coord = 1920;
    SubRuler vertical_ruler;
    vertical_ruler.orientation = RULER_ORIENTATION_VERTICAL;
    vertical_ruler.position = x;
    vertical_ruler.start_coord = 0;
    vertical_ruler.end_coord = 1080;

    clear_drawing_area(cairo_context);
    draw_subruler(horizontal_ruler, cairo_context);
    draw_subruler(vertical_ruler, cairo_context);

    gdk_window_end_draw_frame(window, surface);
}

static void draw_subruler(SubRuler ruler, cairo_t *cairo_context) {
    cairo_set_operator(cairo_context, CAIRO_OPERATOR_SOURCE);
    cairo_set_source_rgb(cairo_context, 1, 0, 0);
    /* draw main line */
    if (ruler.orientation == RULER_ORIENTATION_VERTICAL) {
        cairo_move_to(cairo_context, ruler.position, ruler.start_coord);
        cairo_line_to(cairo_context, ruler.position, ruler.end_coord);
    } else {
        cairo_move_to(cairo_context, ruler.start_coord, ruler.position);
        cairo_line_to(cairo_context, ruler.end_coord, ruler.position);
    }
    cairo_stroke(cairo_context);

    /* draw end ticks */
    if (ruler.orientation == RULER_ORIENTATION_VERTICAL) {
        cairo_move_to(cairo_context, ruler.position - 10, ruler.start_coord);
        cairo_line_to(cairo_context, ruler.position + 10, ruler.start_coord);
        cairo_move_to(cairo_context, ruler.position - 10, ruler.end_coord);
        cairo_line_to(cairo_context, ruler.position + 10, ruler.end_coord);
    } else {
        cairo_move_to(cairo_context, ruler.start_coord, ruler.position - 10);
        cairo_line_to(cairo_context, ruler.start_coord, ruler.position + 10);
        cairo_move_to(cairo_context, ruler.end_coord, ruler.position - 10);
        cairo_line_to(cairo_context, ruler.end_coord, ruler.position + 10);
    }
    cairo_stroke(cairo_context);
}

static void clear_drawing_area(cairo_t *cairo_context) {
    cairo_set_operator(cairo_context, CAIRO_OPERATOR_CLEAR);
    cairo_paint(cairo_context);
}
