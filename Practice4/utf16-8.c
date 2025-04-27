#include <stdio.h>
#include <string.h>

unsigned short u16c,x;
unsigned  c1,c2,c3;
int n; int k;
int main(int argc, char *argv[]){
    for (k=0; strcmp(argv[k], "-i") != 0; k++);
    FILE *i = fopen(argv[k+1], "r");
    for (k=0; strcmp(argv[k], "-o") != 0; k++);
    FILE *o = fopen(argv[k+1], "w");
    if (i == NULL) fprintf(stderr, "Ошибка, неправильный ввод файл\n");
    else{
        n = fread(&u16c, 1,2,i);
        if (u16c == 0xfffe) {
            while ((n = fread(&u16c, 1,2,i))){
                if (n==1){
                    fprintf(stderr,"Нечетное кол-во байтов в файле ucs-2\n Перевод файла некорректный\n");
                }
                c1 = u16c;
                u16c = u16c >>8;
                x = c1<<8;
                u16c+=x;
                if (u16c < 0x80){
                    c1 = u16c & 127;
                    fwrite(&c1, 1, 2, o);
                    continue;
                }
                if(u16c < 2048) {
                    c2 = 128 + (u16c & 0b00111111);
                    u16c = u16c>>6;
                    c1 = 0b11000000 + (u16c & 0b00011111);
                    fwrite(&c1, 1, 2, o);
                    fwrite(&c2, 1, 2, o);
                    continue;
                }
                c3 = 128 + (u16c & 0b00111111);
                u16c = u16c>>6;
                c2 = 128 + (u16c & 0b00111111);
                u16c = u16c>>6;
                c1 = 0b11100000 + (u16c & 1111);
                fwrite(&c1, 1, 2, o);
                fwrite(&c2, 1, 2, o);
                fwrite(&c3, 1, 2, o);
            }    
        }
        else if (u16c!=0xfeff){
                fprintf(stderr,"Отсутствует BOM в файле ucs-2\n По умолчанию выставлен le порядок\n Перевод может быть некорректный\n");
                if (u16c < 0x80){
                    c1 = u16c & 127;
                    fwrite(&c1, 1, 2, o);
                }
                else if(u16c < 2048) {
                    c2 = 128 + (u16c & 0b00111111);
                    u16c = u16c>>6;
                    c1 = 0b11000000 + (u16c & 0b00011111);
                    fwrite(&c1, 1, 2, o);
                    fwrite(&c2, 1, 2, o);
                }
                else {
                    c3 = 128 + (u16c & 0b00111111);
                    u16c = u16c>>6;
                    c2 = 128 + (u16c & 0b00111111);
                    u16c = u16c>>6;
                    c1 = 0b11100000 + (u16c & 1111);
                    fwrite(&c1, 1, 2, o);
                    fwrite(&c2, 1, 2, o);
                    fwrite(&c3, 1, 2, o);
                }
        }
            while ((n = fread(&u16c, 1,2,i))){
                if (n==1){
                    fprintf(stderr,"Нечетное кол-во байтов в файле ucs-2\n Перевод файла некорректный\n");
                }
                if (u16c < 0x80){
                    c1 = u16c & 127;
                    fwrite(&c1, 1, 2, o);
                    continue;
                }
                if(u16c < 2048) {
                    c2 = 128 + (u16c & 0b00111111);
                    u16c = u16c>>6;
                    c1 = 0b11000000 + (u16c & 0b00011111);
                    fwrite(&c1, 1, 2, o);
                    fwrite(&c2, 1, 2, o);
                    continue;
                }
                c3 = 128 + (u16c & 0b00111111);
                u16c = u16c>>6;
                c2 = 128 + (u16c & 0b00111111);
                u16c = u16c>>6;
                c1 = 0b11100000 + (u16c & 1111);
                fwrite(&c1, 1, 2, o);
                fwrite(&c2, 1, 2, o);
                fwrite(&c3, 1, 2, o);
                
            } 
            fclose(i);
    }
    fclose(o);
    return 0;
}