#ifndef MEASUREMENT_H_INCLUDED
#define MEASUREMENT_H_INCLUDED

#ifndef GTK_H_INCLUDED
#define GTK_H_INCLUDED
#include <gtk/gtk.h>
#endif

#ifndef RULER_H_INCLUDED
#include "ruler.h"
#endif

typedef struct Measurement {
    int x, y;
    int x_start, y_start, x_end, y_end;
} Measurement;

typedef struct MeasurementNode {
    Measurement *measurement;
    struct MeasurementNode *next;
} MeasurementNode;

typedef struct MeasurementList {
    MeasurementNode *head;
    int length;
} MeasurementList;

Measurement *create_new_measurement(Ruler *ruler);
MeasurementList *create_new_measurement_list(void);
void destroy_measurement_list(MeasurementList *list);
void add_measurement_to_list(Measurement *measurement, MeasurementList *list);
void draw_measurement(Measurement *measurement, GtkWidget *drawing_area);
void draw_all_measurements(MeasurementList *list, GtkWidget *drawing_area);

#endif /* MEASUREMENT_H_INCLUDED */
