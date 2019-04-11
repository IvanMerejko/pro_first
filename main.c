#include <stdio.h>
#include <pthread.h>
#include "queue.h"
#include "threads.h"
int main() {

    queue_type queue;
    construct_empty_queue(&queue);

    push_back(&queue , 10);
    sem_post(&semaphore);
    push_back(&queue , 9);
    sem_post(&semaphore);

    init_atomic_value();
    pthread_barrier_init(&barrier , NULL , 2);
    pthread_mutex_init(&mutex , NULL);
    pthread_cond_init(&conditional_variable , NULL);

    printf("atomic values: \n" );
    print_atomic_values();

    pthread_create(&P1 , NULL , &P1_thread_function , &queue);
    pthread_create(&P2 , NULL , &P2_thread_function , &queue);
    pthread_create(&P3 , NULL , &P3_thread_function , &queue);
    pthread_create(&P4 , NULL , &P4_thread_function , &queue);
    pthread_create(&P5 , NULL , &P5_thread_function , &queue);
    pthread_create(&P6 , NULL , &P6_thread_function , &queue);




    pthread_join(P6 , NULL);
    delete_queue(&queue);

    return 0;
}