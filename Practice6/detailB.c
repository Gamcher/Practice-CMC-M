//здесь происходит создание детали B
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <errno.h>
#define COUNT_PRODUCT 10

int main(){
    //ключи ко всему
    key_t keyB = ftok("./detail.txt", 'B');
    //подключение всех семафоров
    int sem_id = semget(keyB, 1, 0666);
    //создание детали
    struct sembuf semops = {0, 1, 0};
    printf("Запускаем конвейерную ленту для деталей B\n");
    for(int i=0; i<COUNT_PRODUCT; i++){
        sleep(3);
        printf("Создано уже %d деталей B\n", i+1);
        semop(sem_id, &semops, 1);
    }
    printf("Конвейерная лента для деталей B остановлена\n");
    return 0; 
}