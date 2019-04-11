//
// Created by ivan on 29.03.19.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <limits.h>
#include "queue.h"
#include "threads.h"

void construct_empty_queue(queue_type* queue){
    queue->begin = NULL;
    queue->end = NULL;
}
void push_back(queue_type* queue , int value){
    node* new_node = (node *) malloc(sizeof(node));
    new_node->next = NULL;
    new_node->value = value;
    if(queue->begin == NULL){
        ++numberOfEmptyQueue;
        queue->begin = new_node;
    } else {
        queue->end->next = new_node;
    }
    queue->end = new_node;
}
void* pop_front(queue_type* queue ){
    void* node = NULL;
    node = (void *)queue->begin;
    if(queue->begin == queue->end){
        queue->end = queue->end->next;
    }
    queue->begin = queue->begin->next;
    return node;
}
void print_queue(const queue_type* queue){
    node *tmp = queue->begin;
    for(;tmp != queue->end ; tmp = tmp->next){
        printf("%d " , tmp->value);
    }
    printf("\n");

}
void delete_queue(queue_type* queue){
    recursion_free(&queue->begin);
}
void recursion_free(node** current_node){
    if(*current_node == NULL){
        return;
    }
    if((*current_node)->next == NULL){
        free(*current_node);
    } else {
        recursion_free(&(*current_node)->next);
        free(*current_node);
    }
}