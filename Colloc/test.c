#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>

// 1 задача 2021 года, 10 задача 2022 года
struct segment{
    unsigned int size;
    unsigned int base;
};

unsigned int VirtIntoPhys(struct segment *SegTable, unsigned int VirtAdr){
    unsigned short nseg = VirtAdr >> 29; //количество сегментов - 8, значит 3 бита в вирт. адресе на nseg
    unsigned int offset = VirtAdr & 03777777777; //представление бинарной маски 29 битов (XOR первых 29 битов)
    if(offset >= (SegTable[nseg]).size) exit(25);
    else return offset + (SegTable[nseg]).base;
}

int main(){
    struct segment SegTable[8]; //8 сегментов
}

//9 задача 2022 года
unsigned int sizeBlock(unsigned * FAT, unsigned num){
    unsigned sizeB = 0;
    while(num != 0){
        sizeB++;
        num = FAT[num]; 
    }
    return sizeB;
}
// Написать программу, моделирующую работу Raid 0, построенного из 4-х дисков (вместо реальных дисков используются файлы с именами
// «Disk0», «Disk1», … «Disk3»). Размер блока – BlockSize (определенная в программе константа, измеряется в байтах). Нумерация блоков – от
// нуля. В программе должны быть: функция void Ini_Raid0(void) – инициализация (открытие файлов «Disk0», «Disk1», … «Disk3» – считаем, что
// они существуют, доступны и нужного размера), а также функция void Read_Raid0(int num, char *buf) – запрос на чтение блока с номером num в
// буфер по указателю *buf. Функцию записи и функцию main реализовывать не нужно.
#define BlockSize 1000 //размер полосы
int fdR[4];
void Ini_Raid0(void){
    fdR[0] = open("Disk0", O_RDWR);
    fdR[1] = open("Disk1", O_RDWR);
    fdR[2] = open("Disk2", O_RDWR);
    fdR[3] = open("Disk3", O_RDWR);
}
void Read_Raid0(int num, char *buf){
    int numBlock = (int)num/4;
    int offset = numBlock * BlockSize;
    int numDisk = num % 4;
    lseek(fdR[numDisk], offset, SEEK_SET);
    read(fdR[numDisk], buf, BlockSize);
}

