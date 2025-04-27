#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* thread_sum(void* args) {
    int* sum = (int*) args;
    
    pthread_mutex_lock(&mutex);
    printf("%lu - %d\n", pthread_self(), *sum);
    *sum += pthread_self() % 100;
    printf("~%lu - %d\n", pthread_self(), *sum);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(void) {
    int sum = 0;
    pthread_t tid[10];

    for (int i = 0; i < 10; i++) {
        pthread_create(&tid[i], NULL, thread_sum, (void*) &sum);
    }

    for (int i = 0; i < 10; i++) {
        pthread_join(tid[i], NULL);
    }

    return 0;
}