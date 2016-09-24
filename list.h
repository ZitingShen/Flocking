#ifndef LIST_H
#define LIST_H

typedef struct _node {
  void* data;
  struct _node *prev;
  struct _node *next;
} NODE;

typedef struct _list{
  int length;
  NODE* head;
  NODE* tail;
} List;

List* list_new();
void list_destroy(List* list);

void list_insert(List* list, void* elt, int pos);
void list_delete(List* list, int pos);

void* list_get(List* list, int pos); //return a pointer to the data
int list_size(List* list);

#endif
