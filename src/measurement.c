#include <stdlib.h>
#include <gtk/gtk.h>
#include "measurement.h"
#include "overlay.h"
#include "ruler.h"

Measurement *create_new_measurement(Ruler *ruler) {
    Measurement *measurement = malloc(sizeof(measurement));
    measurement->x = ruler->vertical_ruler->x;
    measurement->y = ruler->vertical_ruler->y;
    measurement->x_start = ruler->horizontal_ruler->start_coord;
    measurement->x_end = ruler->horizontal_ruler->end_coord;
    measurement->y_start = ruler->vertical_ruler->start_coord;
    measurement->y_end = ruler->vertical_ruler->end_coord;
    return measurement;
}

MeasurementList *create_new_measurement_list(void) {
    MeasurementList *measurement_list = malloc(sizeof(MeasurementList));
    measurement_list->head = NULL;
    measurement_list->length = 0;
    return measurement_list;
}

void destroy_measurement_list (MeasurementList *list) {
    MeasurementNode *current_node = list->head;
    while (current_node != NULL) {
        list->head = current_node->next;
        free(current_node->measurement);
        free(current_node);
        current_node = list->head;
    }
    free(list);
}

/* adds measurement to beginning of linked list */
void add_measurement_to_list(Measurement *measurement, MeasurementList *list) {
    MeasurementNode *node = malloc(sizeof(MeasurementNode));
    node->measurement = measurement;
    node->next = list->head;
    list->head = node;
    list->length += 1;
}

void draw_measurement(Measurement *measurement, GtkWidget *drawing_area) {
    GdkWindow *window = gtk_widget_get_window(drawing_area);
    cairo_region_t *region = gdk_window_get_visible_region(window);
    GdkDrawingContext *surface = gdk_window_begin_draw_frame(window, region);
    cairo_t *cairo_context = gdk_drawing_context_get_cairo_context(surface);

    cairo_set_source_rgb(cairo_context, 1, 0, 0);

    cairo_move_to(cairo_context, measurement->x_start, measurement->y);
    cairo_line_to(cairo_context, measurement->x_end, measurement->y);
    cairo_move_to(cairo_context, measurement->x, measurement->y_start);
    cairo_line_to(cairo_context, measurement->x, measurement->y_end);
    cairo_stroke(cairo_context);

    gdk_window_end_draw_frame(window, surface);
}

void draw_all_measurements(MeasurementList *list, GtkWidget *drawing_area) {
    MeasurementNode *current_node = list->head;
    while (current_node != NULL) {
        Measurement *measurement = current_node->measurement;
        draw_measurement(measurement, drawing_area);
        current_node = current_node->next;
    }
}
