#include "types.h"

int return_points(Point* data, double* out_x, double* out_y, double* out_z) {
    if (!data) return -1;

    if (out_x && data->x) {
        *out_x = *data->x;
    }else {
        return -1;
    }
    if (out_y && data->y) {
        *out_y = *data->y;
    }else {
        return -1;
    }
    if (out_z && data->z) {
        *out_z = *data->z;
    }else {
        return -1;
    }
    return 0;
}