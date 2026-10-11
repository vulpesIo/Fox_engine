#include "types.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

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

void add_node_to_list(LinkedList* list, void* value, size_t size) {
  Node *new_node = malloc(sizeof(Node));

  if (!new_node) {
    fprintf(stderr, "Failed to allocate memory");
    perror("Dentails");
    fflush(stderr);
    exit(1);
  }
  
  new_node->size = size;
  new_node->value = value;

  list->length++;

  if (!list->head) {
    list->head = new_node;
  }

  if (!list->tail) {
    list->tail = new_node;
  }else {
    list->tail->next = new_node;
    list->tail = new_node;
  }
}

Node* pop_list(LinkedList* list) {
    if (!list || !list->tail) {
        return NULL;
    }

    Node* tmp = list->tail;

    if (!tmp->prev) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        list->tail = tmp->prev;
        list->tail->next = NULL;
    }

    tmp->prev = NULL;
    tmp->next = NULL;
    list->length--;

    return tmp;
}

// void free_list(LinkedList* list) {
//     if (!list) {
//         return;
//     }

//     Node* removed;

//     while ((removed = pop_list(list)) != NULL) {
//         free(removed);
//     }
// }