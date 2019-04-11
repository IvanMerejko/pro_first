//
// Created by ivan on 29.03.19.
//

#ifndef PRO_THREADS_H
#define PRO_THREADS_H

#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

struct many_values{
    int first_int;
    int second_int;

    unsigned first_unsigned;
    unsigned second_unsigned;

    long first_long;
    long second_long;

    long unsigned first_long_unsigned;
    long unsigned second_long_unsigned;
};
bool isTakeWhenQueueIsFull;
typedef struct many_values atomic_values;
static int numberOfEmptyQueue = 0;
static int numberOfFullQueue = 0;
atomic_values atomicValues;

pthread_mutex_t mutex;
sem_t semaphore;
pthread_barrier_t   barrier;
pthread_cond_t conditional_variable ;

pthread_t P1;
pthread_t P2;
pthread_t P3;
pthread_t P4;
pthread_t P5;
pthread_t P6;

void init_atomic_value();

void * P1_thread_function(void *);
void * P2_thread_function(void *);
void * P3_thread_function(void *);
void * P4_thread_function(void *);
void * P5_thread_function(void *);
void * P6_thread_function(void *);


void P1_and_P4_function(queue_type* , const char*);
void P2_and_P5_function(queue_type* , const char*);
bool push_value_to_buffer(queue_type* ,int, const char *);
void modifier_atomic_values(const char* , int );
void use_and_modifier_atomic_values(const char* , int);
void thread_careful_print(int );

void print_atomic_values();
bool is_end();
int get_atomic_values_by_index(int);
#endif //PRO_THREADS_H
