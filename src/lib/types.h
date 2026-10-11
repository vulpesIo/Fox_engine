#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>

typedef struct {
    double* x;
    double* y;
    double* z;
} Point;

typedef struct Node{
  size_t size;
  void* value;
  struct Node* next;
  struct Node* prev;
} Node;

typedef struct LinkedList{ 
  int length;
  Node* head;
  Node* tail;
} LinkedList;

int return_points(Point* data, double* out_x, double* out_y, double* out_z);

void add_node_to_list(LinkedList* list, void* value, size_t size);

Node* pop_list(LinkedList* list);

// void free_list(LinkedList* list);

#endif
