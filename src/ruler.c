#include <cairo.h>
#include <gdk/gdk.h>
#include <unistd.h>
#include "ruler.h"

static void draw_subruler(SubRuler *ruler, cairo_t *cairo_context);
static void clear_drawing_area(cairo_t *cairo_context);
static int find_ruler_extrema(SubRuler *ruler);
/* static void get_rgb_values_at_pixel(guchar *pixel, int *r, int *g, int *b); */
static gboolean pixels_within_threshold (guchar *pixel_1, guchar *pixel_2);

Ruler *create_new_ruler() {
    Ruler *ruler;
    ruler = malloc(sizeof(Ruler));
    return ruler;
}

SubRuler *create_new_subruler(RulerOrientation orientation) {
    SubRuler *ruler;
    ruler = malloc(sizeof(SubRuler));
    ruler->orientation = orientation;
    return ruler;
}

void draw_ruler(Ruler *ruler, GtkWidget *drawing_area, int x, int y) {

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
    find_ruler_extrema(vertical_ruler);
    find_ruler_extrema(horizontal_ruler);
    draw_subruler(horizontal_ruler, cairo_context);
    draw_subruler(vertical_ruler, cairo_context);

    gdk_window_end_draw_frame(window, surface);
}

static void draw_subruler(SubRuler *ruler, cairo_t *cairo_context) {
    cairo_set_operator(cairo_context, CAIRO_OPERATOR_SOURCE);
    cairo_set_source_rgb(cairo_context, 1, 0, 0);

    /* leaving a gap of a few pixels under the mouse pointer. This is because some
    times the root window pixbuf seems to be yielding the actual lines being
    drawn by cairo as the pixel data instead of whatever is underneath. Doing
    this seems to fix the problem. But does cause the line to flicker a little.
    will look into a better implementation later */
    int pixel_gap = 3;

    /* draw main line */
    if (ruler->orientation == RULER_ORIENTATION_VERTICAL) {
        cairo_move_to(cairo_context, ruler->x, ruler->start_coord);
        cairo_line_to(cairo_context, ruler->x, ruler->y - pixel_gap);
        cairo_move_to(cairo_context, ruler->x, ruler->y + pixel_gap);
        cairo_line_to(cairo_context, ruler->x, ruler->end_coord);
    } else {
        cairo_move_to(cairo_context, ruler->start_coord, ruler->y);
        cairo_line_to(cairo_context, ruler->x - pixel_gap, ruler->y);
        cairo_move_to(cairo_context, ruler->x + pixel_gap, ruler->y);
        cairo_line_to(cairo_context, ruler->end_coord, ruler->y);
    }
    cairo_stroke(cairo_context);

    /* draw end ticks */
    if (ruler->orientation == RULER_ORIENTATION_VERTICAL) {
        cairo_move_to(cairo_context, ruler->x - 10, ruler->start_coord);
        cairo_line_to(cairo_context, ruler->x + 10, ruler->start_coord);
        cairo_move_to(cairo_context, ruler->x - 10, ruler->end_coord);
        cairo_line_to(cairo_context, ruler->x + 10, ruler->end_coord);
    } else {
        cairo_move_to(cairo_context, ruler->start_coord, ruler->y - 10);
        cairo_line_to(cairo_context, ruler->start_coord, ruler->y + 10);
        cairo_move_to(cairo_context, ruler->end_coord, ruler->y - 10);
        cairo_line_to(cairo_context, ruler->end_coord, ruler->y + 10);
    }
    cairo_stroke(cairo_context);
}

static void clear_drawing_area(cairo_t *cairo_context) {
    cairo_set_operator(cairo_context, CAIRO_OPERATOR_CLEAR);
    cairo_paint(cairo_context);
}

static int find_ruler_extrema(SubRuler *ruler) {
    GdkWindow *root_window;
    root_window = gdk_get_default_root_window();

    GdkPixbuf *pixbuf;
    int height, width;
    height = gdk_window_get_height(root_window);
    width = gdk_window_get_width(root_window);

    if (ruler->orientation == RULER_ORIENTATION_VERTICAL) {
        pixbuf = gdk_pixbuf_get_from_window(root_window, ruler->x, 0, 1, height);

        /* values necessary for iterating pixbuf */
        int row_stride = gdk_pixbuf_get_rowstride(pixbuf);

        if (pixbuf != NULL) {
            guchar *pixel_data = gdk_pixbuf_get_pixels(pixbuf);
            guchar *current_pixel = pixel_data + ruler->y * row_stride;
            int distance_to_top = ruler->y;
            for (int i = distance_to_top; i > 0; i--) {
                guchar *pixel_at_i = pixel_data + i * row_stride;
                if (! pixels_within_threshold(current_pixel, pixel_at_i)) {
                    ruler->start_coord =  i;
                    break;
                }
            }
            for (int i = ruler->y; i < height; i++) {
                guchar *pixel_at_i = pixel_data + i * row_stride;
                if (! pixels_within_threshold(current_pixel, pixel_at_i)) {
                    ruler->end_coord = i;
                    break;
                }
            }
        }
        g_object_unref(pixbuf);
    } else {
        pixbuf = gdk_pixbuf_get_from_window(root_window, 0, ruler->y, width, 1);

        int num_channels = gdk_pixbuf_get_n_channels(pixbuf);

        if (pixbuf != NULL) {
            guchar *pixel_data = gdk_pixbuf_get_pixels(pixbuf);
            guchar *current_pixel = pixel_data + ruler->x * num_channels;
            int distance_to_left_edge = ruler->x;
            for (int i = distance_to_left_edge; i > 0; i--) {
                guchar *pixel_at_i = pixel_data + i * num_channels;
                if (! pixels_within_threshold(current_pixel, pixel_at_i)) {
                    ruler->start_coord =  i;
                    break;
                }
            }
            for (int i = ruler->x; i < width; i++) {
                guchar *pixel_at_i = pixel_data + i * num_channels;
                if (! pixels_within_threshold(current_pixel, pixel_at_i)) {
                    ruler->end_coord =  i;
                    break;
                }
            }
        }
        g_object_unref(pixbuf);
    }
    return 0;
}

/* static void get_rgb_values_at_pixel(guchar * pixel, int *r, int *g, int *b) { */
/*     *r = pixel[0]; */
/*     *g = pixel[1]; */
/*     *b = pixel[2]; */
/*     g_print("%d, %d, %d\n", *r, *g, *b); */
/* } */

static gboolean pixels_within_threshold (guchar * pixel_1, guchar * pixel_2) {
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
