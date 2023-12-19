//здесь идет создание детали A
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <errno.h>
#define COUNT_PRODUCT 10

int main(){
    //ключи ко всему
    key_t keyA = ftok("./detail.txt", 'A');
    //подключение всех семафоров
    int sem_id = semget(keyA, 1, 0666);
    //создание детали
    struct sembuf semops = {0, 1, 0};
    printf("Запускаем конвейерную ленту для деталей A\n");
    for(int i=0; i<COUNT_PRODUCT; i++){
        sleep(2);
        printf("Создано уже %d деталей A\n", i+1);
        semop(sem_id, &semops, 1);
    }
    printf("Конвейерная лента для деталей A остановлена\n");
    return 0; 
}
