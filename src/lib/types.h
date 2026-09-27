#ifndef TYPES_H
#define TYPES_H

typedef struct {
    double* x;
    double* y;
    double* z;
} Point;

int return_points(Point* data, double* out_x, double* out_y, double* out_z);

#endif