#ifndef _MY_QUEUE
#define _MY_QUEUE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
  struct node *next;
  char *data;
} node;

typedef struct my_queue {
  node *head;
  node *tail;
  int size;
} my_queue;

int queue_init(my_queue *queue);
int queue_push(my_queue *queue, const char *string);
int queue_pop(my_queue *queue, char **string);
int queue_clear(my_queue *queue);
int queue_getsize(const my_queue *queue);

#endif