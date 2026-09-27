#include "utlis.h"

double get_elapsed_seconds(struct timespec start, struct timespec end) {
    double start_sec = (double)start.tv_sec 
                   + ((double)start.tv_nsec / 1000000000.0);

    double end_sec = (double)end.tv_sec
                   + ((double)end.tv_nsec / 1000000000.0);
                   
    return end_sec - start_sec;
}