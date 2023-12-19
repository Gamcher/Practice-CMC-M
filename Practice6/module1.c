//здесь происходит создание модуля_1 с подключением семафоров detailA, detailB
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <errno.h>
#define COUNT_PRODUCT 10

int main(){
    //ключи ко всему
    key_t keyM = ftok("./detail.txt", 'M');
    key_t keyA = ftok("./detail.txt", 'A');
    key_t keyB = ftok("./detail.txt", 'B');
    //подключение всех семафоров
    int sem_idM = semget(keyM, 1, 0666);
    int sem_idA = semget(keyA, 1, 0666);
    int sem_idB = semget(keyB, 1, 0666);
    //создание модулей
    struct sembuf semops;
    semops.sem_flg = 0;
    semops.sem_num = 0;
    //инициализация используемых семафоров
    semctl(sem_idA, 0, SETVAL, (int) 0);
    semctl(sem_idB, 0, SETVAL, (int) 0);
    printf("Запускаем конвейерную ленту для модулей\n");
    for(int i=0; i<COUNT_PRODUCT; i++){
        //ожидание создания детали A и B
        semops.sem_op = -1;
        semop(sem_idA, &semops, 1);
        semop(sem_idB, &semops, 1);
        semops.sem_op = 1;
        printf("Создано уже %d модулей\n", i+1);
        semop(sem_idM, &semops, 1);
    }
    printf("Конвейерная лента для модулей остановлена\n");
    return 0; 
}