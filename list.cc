#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

List* list_new(){
  List* my_list=(List*)malloc(sizeof(List));
  my_list->length=0;
  my_list->head=NULL;
  my_list->tail=NULL;
  return my_list;
}

void list_destroy(List* list){
  NODE* current=NULL;
  while(list->length!=0){
    current=list->head->next;
    free(list->head->data); //free data
    free(list->head); //free node struct
    list->head=current;
    list->length--;
  }
  free(list); //free list struct
}

void list_insert(List* list, void* elt, int pos){
  NODE* current=NULL; //helper
  NODE* new_node=(NODE*)malloc(sizeof(NODE)); //new node
  new_node->data=elt;

  if(list->length==0){//empty list
    assert(list->head==NULL && list->tail==NULL);
    list->head=new_node;
    list->tail=new_node;
    new_node->next=NULL;
    new_node->prev=NULL;
  }else if(pos==(list->length-1)){//insert at tail
     current=list->tail;
     list->tail=new_node;
     new_node->prev=current;
     current->next=new_node;
     list->tail->next=NULL;
   }
   else{
     current=list->head;
     for(int i=0;i<pos;i++){//insert behind list[pos]
       current=current->next;
     }
     current->next->prev=new_node;
     new_node->next=current->next;
     new_node->prev=current;
     current->next=new_node;
   }
   list->length++;
}

void list_delete(List* list, int pos){
  NODE* current=list->head;
  int i=0;
  if(list->length==0){
    return; // called upon empty list
  }else if(list->length==1){ //when there's only one elt
    free(current->data);
    free(current);
    list->head=NULL;
    list->tail=NULL;
    list->length--;
  }else if(pos==0){
    list->head=current->next;
    list->head->prev=NULL;
    free(current->data);
    free(current);
    list->length--;
  }else if(list->length-1==pos){ //when deleting the last elt
    current=list->tail;
    list->tail=current->prev;
    list->tail->next=NULL;
    free(current->data);
    free(current);
    list->length--;
  }else{   //when deleting elt in the middle
    while(i<pos){
      current=current->next;
      i++;
    }
    current->prev->next=current->next;
    current->next->prev=current->prev;
    free(current->data);
    free(current);
    list->length--;
  }
}

void* list_get(List* list, int pos){
  if(list->length==0){
    return NULL;
  }

  NODE* current=list->head;
  for(int i=0;i<pos;i++){
    current=current->next;
  }
  return current->data;
}

int list_size(List* list){
  return list->length;
}
