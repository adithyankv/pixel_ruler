#include <cairo.h>
#include <gdk/gdk.h>
#include <unistd.h>
#include "ruler.h"

static void draw_subruler(SubRuler *ruler, cairo_t *cairo_context);
static void clear_drawing_area(cairo_t *cairo_context);
static void set_ruler_extrema(Ruler *ruler);
static gboolean pixels_similar_within_threshold (guchar *pixel_1, guchar *pixel_2);
static guchar *get_pixel_in_pixbuf (GdkPixbuf *pixbuf, int x, int y);

Ruler *create_new_ruler() {
    Ruler *ruler = malloc(sizeof(Ruler));

    SubRuler *horizontal_ruler = create_new_subruler (RULER_ORIENTATION_HORIZONTAL);
    SubRuler *vertical_ruler = create_new_subruler (RULER_ORIENTATION_VERTICAL);
    ruler->horizontal_ruler = horizontal_ruler;
    ruler->vertical_ruler = vertical_ruler;

    return ruler;
}

SubRuler *create_new_subruler(RulerOrientation orientation) {
    SubRuler *ruler = malloc(sizeof(SubRuler));
    ruler->orientation = orientation;
    return ruler;
}

void draw_ruler(Ruler *ruler, GtkWidget *drawing_area, int x, int y) {

    GdkWindow *window = gtk_widget_get_window(drawing_area);
    cairo_region_t *region = gdk_window_get_visible_region(window);
    GdkDrawingContext *surface = gdk_window_begin_draw_frame(window, region);
    cairo_t *cairo_context = gdk_drawing_context_get_cairo_context(surface);

    GdkDisplay *display = gdk_display_get_default();
    GdkCursor *cursor = gdk_cursor_new_from_name(display, "crosshair");
    gdk_window_set_cursor(window, cursor);


    SubRuler *horizontal_ruler;
    horizontal_ruler = ruler->horizontal_ruler;
    horizontal_ruler->y = y;
    horizontal_ruler->x = x;
    horizontal_ruler->start_coord = 0;
    horizontal_ruler->end_coord = 1920;

    SubRuler *vertical_ruler;
    vertical_ruler = ruler->vertical_ruler;
    vertical_ruler->x = x;
    vertical_ruler->y = y;
    vertical_ruler->start_coord = 0;
    vertical_ruler->end_coord = 1080;

    clear_drawing_area(cairo_context);
    set_ruler_extrema(ruler);
    draw_subruler(horizontal_ruler, cairo_context);
    draw_subruler(vertical_ruler, cairo_context);

    gdk_window_end_draw_frame(window, surface);
}

static void draw_subruler(SubRuler *ruler, cairo_t *cairo_context) {
    cairo_set_operator(cairo_context, CAIRO_OPERATOR_SOURCE);
    cairo_set_source_rgb(cairo_context, 1, 0, 0);

    int tick_width = 5;
    int line_width = 2;
    cairo_set_line_width(cairo_context, line_width);

    /* draw main line */
    if (ruler->orientation == RULER_ORIENTATION_VERTICAL) {
        cairo_move_to(cairo_context, ruler->x, ruler->start_coord);
        cairo_line_to(cairo_context, ruler->x, ruler->end_coord);
    } else {
        cairo_move_to(cairo_context, ruler->start_coord, ruler->y);
        cairo_line_to(cairo_context, ruler->end_coord, ruler->y);
    }

    /* draw end ticks */
    if (ruler->orientation == RULER_ORIENTATION_VERTICAL) {
        cairo_move_to(cairo_context, ruler->x - tick_width, ruler->start_coord + line_width / 2);
        cairo_line_to(cairo_context, ruler->x + tick_width, ruler->start_coord + line_width / 2);
        cairo_move_to(cairo_context, ruler->x - tick_width, ruler->end_coord - line_width / 2);
        cairo_line_to(cairo_context, ruler->x + tick_width, ruler->end_coord - line_width / 2);
    } else {
        cairo_move_to(cairo_context, ruler->start_coord + line_width / 2, ruler->y - tick_width);
        cairo_line_to(cairo_context, ruler->start_coord + line_width / 2, ruler->y + tick_width);
        cairo_move_to(cairo_context, ruler->end_coord - line_width / 2, ruler->y - tick_width);
        cairo_line_to(cairo_context, ruler->end_coord - line_width / 2, ruler->y + tick_width);
    }
    cairo_stroke(cairo_context);
}

static void clear_drawing_area(cairo_t *cairo_context) {
    cairo_set_operator(cairo_context, CAIRO_OPERATOR_CLEAR);
    cairo_paint(cairo_context);
}

static void set_ruler_extrema(Ruler *ruler) {
    GdkWindow *root_window = gdk_get_default_root_window();

    GdkPixbuf *pixbuf;
    int height = gdk_window_get_height(root_window);
    int width = gdk_window_get_width(root_window);
    pixbuf = gdk_pixbuf_get_from_window(root_window, 0, 0, width, height);

    SubRuler *vruler, *hruler;
    vruler = ruler->vertical_ruler;
    hruler = ruler->horizontal_ruler;

    if (pixbuf != NULL) {
        guchar *root_pixel, *current_pixel;
        root_pixel = get_pixel_in_pixbuf(pixbuf, vruler->x, vruler->y);

        for (int i = hruler->x; i > 0; i--) {
            current_pixel = get_pixel_in_pixbuf(pixbuf, i, hruler->y);
            if (! pixels_similar_within_threshold(current_pixel, root_pixel)) {
                hruler->start_coord = i;
                break;
            }
        }
        for (int i = hruler->x; i < width; i++) {
            current_pixel = get_pixel_in_pixbuf(pixbuf, i, hruler->y);
            if (! pixels_similar_within_threshold(current_pixel, root_pixel)) {
                hruler->end_coord = i;
                break;
            }
        }
        for (int i = vruler->y; i < height; i++) {
            current_pixel = get_pixel_in_pixbuf(pixbuf, vruler->x, i);
            if (! pixels_similar_within_threshold(current_pixel, root_pixel)) {
                vruler->end_coord = i;
                break;
            }
        }
        for (int i = vruler->y; i > 0; i--) {
            current_pixel = get_pixel_in_pixbuf(pixbuf, vruler->x, i);
            if (! pixels_similar_within_threshold(current_pixel, root_pixel)) {
                vruler->start_coord = i;
                break;
            }
        }
    }
    g_object_unref(pixbuf);
}

static guchar *get_pixel_in_pixbuf (GdkPixbuf *pixbuf, int x, int y) {
    guchar *start_index = gdk_pixbuf_get_pixels(pixbuf);
    int row_stride = gdk_pixbuf_get_rowstride(pixbuf);
    int num_channels = gdk_pixbuf_get_n_channels(pixbuf);

    guchar *index = start_index + x * num_channels + y * row_stride;
    return index;
}

static gboolean pixels_similar_within_threshold (guchar * pixel_1, guchar * pixel_2) {
    int threshold = 15;
    int r_diff = abs(pixel_1[0] - pixel_2[0]);
    int g_diff = abs(pixel_1[1] - pixel_2[1]);
    int b_diff = abs(pixel_1[2] - pixel_2[2]);
    if (r_diff < threshold && g_diff < threshold && b_diff < threshold) {
        return TRUE;
    } else {
        return FALSE;
    }
}
