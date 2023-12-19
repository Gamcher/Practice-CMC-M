//здесь происходит создание детали C
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <errno.h>
#define COUNT_PRODUCT 10

int main(){
    //ключи ко всему
    key_t keyC = ftok("./detail.txt", 'C');
    //подключение всех семафоров
    int sem_id = semget(keyC, 1, 0666);
    //создание детали
    struct sembuf semops = {0, 1, 0};
    printf("Запускаем конвейерную ленту для деталей C\n");
    for(int i=0; i<COUNT_PRODUCT; i++){
        sleep(4);
        printf("Создано уже %d деталей C\n", i+1);
        semop(sem_id, &semops, 1);
    }
    printf("Конвейерная лента для деталей С остановлена\n");
    return 0; 
}