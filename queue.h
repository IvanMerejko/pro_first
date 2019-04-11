//
// Created by ivan on 29.03.19.
//

#ifndef PRO_QUEUE_H
#define PRO_QUEUE_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_LENGHT 5

struct node{
    struct node *next;
    int value;
};
struct values{
    struct node* begin;
    struct node* end;
};
typedef struct values queue_type;
typedef struct node node;

void construct_empty_queue(queue_type*);
void push_back(queue_type* , int);
void* pop_front(queue_type*);
void delete_queue(queue_type* );
void print_queue(const queue_type*);
void recursion_free(node** current_node);






#endif //PRO_QUEUE_H
