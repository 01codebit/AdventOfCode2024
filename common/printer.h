#include <stdio.h>

void print_array(char* title, int* array, int len);

void print_matrix(char** data, int rows, int cols);

void print_matrix_marker(char** data, int rows, int cols, char marker, int marker_x, int marker_y);

int print_matrix_markers(char** data, int rows, int cols, char marker, int* marker_x, int* marker_y, int markers_count);