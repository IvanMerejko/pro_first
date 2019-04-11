//
// Created by ivan on 29.03.19.
//
#include "queue.h"
#include "threads.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
void* P1_thread_function(void* parameters){
    queue_type* queue = (queue_type*)(parameters);
    P1_and_P4_function(queue , "P1");
    printf("Thread P1 stopped !!!\n");
}
void* P2_thread_function(void* parameters){
    queue_type* queue = (queue_type*)(parameters);
    P2_and_P5_function(queue , "P2");
    pthread_cancel(P1);
    pthread_cancel(P3);
    pthread_cancel(P4);
    pthread_cancel(P5);
    pthread_cancel(P6);
    printf("Thread P2 stopped !!!\n");
    return NULL;
}
void* P3_thread_function(void* parameters){
    queue_type* queue = (queue_type*) (parameters);

    int sem_value = -1;
    node* current_element = NULL;

    while(true){

        if(is_end()){
            break;
        }
        sem_wait(&semaphore);

            pthread_mutex_trylock(&mutex);
            current_element = (node*)pop_front(queue);
            sem_getvalue(&semaphore , &sem_value);

            printf("Thread P3: semaphore=%d; element %d TAKEN; \n",
               sem_value,current_element->value);
            //print

            pthread_mutex_unlock(&mutex);

            free(current_element);
    }
    printf("Thread P3 stopped !!!\n");

}
void* P4_thread_function(void* parameters){
    queue_type* queue = (queue_type*)(parameters);
    P1_and_P4_function(queue , "P4");
    printf("Thread P4 stopped !!!\n");
}
void* P5_thread_function(void* parameters){
    queue_type* queue = (queue_type*)(parameters);
    P2_and_P5_function(queue , "P5");
    pthread_cancel(P1);
    pthread_cancel(P2);
    pthread_cancel(P3);
    pthread_cancel(P4);
    pthread_cancel(P6);
    printf("Thread P5 stopped !!!\n");
    return NULL;
}
void* P6_thread_function(void* parameters){
    queue_type* queue = (queue_type*) (parameters);

    while (true){
        if(!push_value_to_buffer(queue , rand()%8 , "P6")){
            break;
        }
    }
    pthread_cancel(P1);
    pthread_cancel(P2);
    pthread_cancel(P3);
    pthread_cancel(P4);
    pthread_cancel(P5);

    printf("Thread P6 stopped !!!\n");


    return NULL;
}
void P1_and_P4_function(queue_type* queue , const char* thread_name){
    while(true){
        if(is_end()){
            break;
        }
        modifier_atomic_values(thread_name , !strcmp(thread_name , "P1") ? 1 : 4 );
        print_atomic_values();
        pthread_mutex_trylock(&mutex);
        printf("%s waits at the barrier\n" ,thread_name);
        pthread_mutex_unlock(&mutex);

        pthread_barrier_wait(&barrier);

        pthread_mutex_trylock(&mutex);
        printf("%s works after barrier\n" , thread_name);
        printf("%s send signal \n" , thread_name);
        pthread_cond_signal(&conditional_variable);
        pthread_mutex_unlock(&mutex);
        use_and_modifier_atomic_values(thread_name , !strcmp(thread_name , "P1") ? 2 : 5 );

    }
}
void P2_and_P5_function(queue_type* queue, const char* thread_name){
    srand(time(NULL));
    while(true){
        printf("%s waits for signals \n" , thread_name);
        pthread_cond_wait(&conditional_variable , &mutex);
        pthread_cond_wait(&conditional_variable , &mutex);
        printf("%s start works \n" , thread_name);
        if(!push_value_to_buffer(queue , rand()%8 , thread_name)){
            break;
        }
    }
}
bool push_value_to_buffer(queue_type* queue, int index , const char* thread_name){
    pthread_mutex_trylock(&mutex);

    if(is_end()){
        pthread_mutex_unlock(&mutex);
        return false;
    }

    int sem_value = -1;
    sem_getvalue(&semaphore , &sem_value);
    printf("Number of empty queue %d\nNumber of full queue: %d\n" , numberOfEmptyQueue , numberOfFullQueue);
    printf("Number of elements in queue: %d\n", sem_value);
    if(sem_value < MAX_QUEUE_LENGHT){
        isTakeWhenQueueIsFull = true;
        push_back(queue , get_atomic_values_by_index(index));
        sem_getvalue(&semaphore , &sem_value);
        printf("Thread %s: semaphore=%d; element %d (atomic index %d) CREATED; \n",
               thread_name, sem_value,queue->end->value , index);

        sem_post(&semaphore);
    } else {
        if(isTakeWhenQueueIsFull){
            ++numberOfFullQueue;
            isTakeWhenQueueIsFull = false;
        }

    }
    pthread_mutex_unlock(&mutex);
    return true;
}
void init_atomic_value(){
    atomicValues.first_int = -1;
    atomicValues.second_int = -2;

    atomicValues.first_unsigned = 1;
    atomicValues.second_unsigned = 2;

    atomicValues.first_long = -100;
    atomicValues.second_long = -200;

    atomicValues.first_long_unsigned = 100;
    atomicValues.second_long_unsigned = 200;
}
void modifier_atomic_values(const char* thread , int value ){
    pthread_mutex_trylock(&mutex);
    printf("%s thread modifier atomic values\n" , thread);
    pthread_mutex_unlock(&mutex);

    __sync_sub_and_fetch(&atomicValues.first_int , value);
    __sync_sub_and_fetch(&atomicValues.second_int , value);

    __sync_or_and_fetch(&atomicValues.first_unsigned , value);
    __sync_or_and_fetch(&atomicValues.second_unsigned , value);

    __sync_xor_and_fetch(&atomicValues.first_long , value);
    __sync_xor_and_fetch(&atomicValues.second_long , value);

    __sync_or_and_fetch(&atomicValues.first_long_unsigned , value);
    __sync_or_and_fetch(&atomicValues.second_long_unsigned , value);

   /* __sync_bool_compare_and_swap(&atomicValues.first_int ,1, value);
    __sync_val_compare_and_swap(&atomicValues.first_int ,1, value);*/
}
void use_and_modifier_atomic_values(const char* thread, int value){
    printf("%s thread use atomic value:%d  and modifier it\n" , thread , __sync_fetch_and_add(&atomicValues.first_int , value));
    thread_careful_print(atomicValues.first_int);

    printf("%s thread use atomic value:%d  and modifier it\n" , thread , __sync_fetch_and_and(&atomicValues.second_int , value));
    thread_careful_print(atomicValues.second_int);

    printf("%s thread use atomic value:%d  and modifier it\n" , thread , __sync_fetch_and_nand(&atomicValues.first_unsigned , value));
    thread_careful_print(atomicValues.first_unsigned);

    printf("%s thread use atomic value:%d  and modifier it\n" , thread , __sync_fetch_and_add(&atomicValues.second_unsigned , value));
    thread_careful_print(atomicValues.second_unsigned);

    printf("%s thread use atomic value:%ld  and modifier it\n" , thread , __sync_fetch_and_and(&atomicValues.first_long , value));
    thread_careful_print((int)atomicValues.first_long);

    printf("%s thread use atomic value:%ld  and modifier it\n" , thread , __sync_fetch_and_nand(&atomicValues.second_long , value));
    thread_careful_print((int)atomicValues.second_long);

    printf("%s thread use atomic value:%lu  and modifier it\n" , thread , __sync_fetch_and_add(&atomicValues.first_long_unsigned , value));
    thread_careful_print((int)atomicValues.first_long_unsigned);

    printf("%s thread use atomic value:%lu  and modifier it\n" , thread , __sync_fetch_and_and(&atomicValues.second_long_unsigned , value));
    thread_careful_print((int)atomicValues.second_long_unsigned);
}
void thread_careful_print(int value){
    pthread_mutex_trylock(&mutex);
    printf("New value: %d\n" , value);
    pthread_mutex_unlock(&mutex);
}
void print_atomic_values(){
    pthread_mutex_trylock(&mutex);
    printf("atomic fist int = %d\n" , atomicValues.first_int);
    printf("atomic second int = %d\n" , atomicValues.second_int);

    printf("atomic fist unsigned int = %d\n" , atomicValues.first_unsigned);
    printf("atomic second unsigned int = %d\n" , atomicValues.second_unsigned);

    printf("atomic fist long = %ld\n" , atomicValues.first_long);
    printf("atomic second long = %ld\n" , atomicValues.second_long);

    printf("atomic fist unsigned long = %lu\n" , atomicValues.first_long_unsigned);
    printf("atomic second unsigned long = %lu\n" , atomicValues.second_long_unsigned);

    pthread_mutex_unlock(&mutex);
}
int get_atomic_values_by_index(int index){
    switch (index){
        case 0:
            return atomicValues.first_int;
        case 1:
            return atomicValues.second_int;
        case 2:
            return atomicValues.first_unsigned;
        case 3:
            return atomicValues.second_unsigned;
        case 4:
            return (int)atomicValues.first_long;
        case 5:
            return (int)atomicValues.second_long;
        case 6:
            return (int)atomicValues.first_long_unsigned;
        case 7:
            return (int)atomicValues.second_long_unsigned;
    }
}
bool is_end(){
    return (numberOfEmptyQueue == 2 && numberOfFullQueue == 2) ||
            numberOfEmptyQueue > 10 || numberOfFullQueue > 10;
}