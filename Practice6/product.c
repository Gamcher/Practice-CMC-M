//здесь делаем продукт, создаем его, подключаем семафоры из детали C и модуля_1
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
    key_t keyC = ftok("./detail.txt", 'C');
    //создание всех семафоров
    int sem_idM = semget(keyM, 1, 0666 | IPC_CREAT);
    int sem_idA = semget(keyA, 1, 0666 | IPC_CREAT);
    int sem_idB = semget(keyB, 1, 0666 | IPC_CREAT);
    int sem_idC = semget(keyC, 1, 0666 | IPC_CREAT);
    //создание продуктов
    struct sembuf semops;
    semops.sem_flg = 0;
    semops.sem_num = 0;
    //инициализация используемых семафоров
    semctl(sem_idM, 0, SETVAL, (int) 0);
    semctl(sem_idC, 0, SETVAL, (int) 0);
    printf("Запускаем конвейерную ленту для продуктов\n");
    for(int i=0; i<COUNT_PRODUCT; i++){
        //ожидание создания детали C и модуля_1
        semops.sem_op = -1;
        semop(sem_idM, &semops, 1);
        semop(sem_idC, &semops, 1);
        semops.sem_op = 1;
        printf("Создано уже %d продуктов\n", i+1);
    }
    printf("Конвейерная лента для продуктов остановлена\n");
    //закрытие всех семафоров
    semctl(sem_idM, 0, IPC_RMID, (int) 0);
    semctl(sem_idA, 0, IPC_RMID, (int) 0);
    semctl(sem_idB, 0, IPC_RMID, (int) 0);
    semctl(sem_idC, 0, IPC_RMID, (int) 0);
    return 0; 
}