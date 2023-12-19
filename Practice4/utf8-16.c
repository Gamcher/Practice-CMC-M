#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned short ec;
unsigned char c1,c2,c3,h,l;
int k, has_bom=0;
char* bom;
int main(int argc, char* argv[]){
    for (k=0; strcmp(argv[k], "-i") != 0; k++);
    FILE *i = fopen(argv[k+1], "r");
    for (k=0; strcmp(argv[k], "-o") != 0; k++);
    FILE *o = fopen(argv[k+1], "w");
    for (k=0;k<argc;k++){
        if (strcmp(argv[k],"-le")==0){
            has_bom=1;
            bom = argv[k];
            ec = 0xfeff;
            fwrite(&ec,2,1,o);
        } 
        if (strcmp(argv[k],"-be")==0){
                if (has_bom){
                    fprintf(stderr,"Ошибка! Одновременно не может быть два флага\n По умолчанию будет выставлен флаг le\n");
                    continue;
                }
                has_bom=1;
                bom = argv[k];
                ec = 0xfffe;
                fwrite(&ec,2,1,o);
        }
    }
    if (!has_bom){
        fprintf(stderr,"Не указан BOM в начале выходного файла \nПо умолчанию выставлен le порядок\n");
        ec = 0xfeff;
        bom = "-le";
        fwrite(&ec,2,1,o);
    }
    k=0;
    //Проверка на ВОМ
    fread(&c1,1,1,i);
    if (c1 == 0xef){
        fread(&c2,1,1,i);
        if (c2 == 0xbb){
            fread(&c3,1,1,i);
            k+=3;
        }
        else{
            k+=1;
            if (c1 < 128){
                l = c1 & 0x7f;
                h = 0;
                if (strcmp(bom,"-be")==0){
                    fwrite(&h,1,1,o);
                    fwrite(&l,1,1,o);
                }
                else{
                    fwrite(&l,1,1,o);
                    fwrite(&h,1,1,o);
                }
            }
            if (c1 >> 5 == 0b110){
                if (c2==0){
                    fprintf(stderr,"Пустой второй байт %x № %d\n",c1,k);
                    return 0;
                }
                k+=1;
                if (c2 >> 6 != 0b10){
                    fprintf(stderr,"Некорректный второй байт %x № %d\n",c2,k);
                }
                l = (c2 & 0x3f) + ((c1 & 0b11)<<6);
                h = (c1>>2) & 0b111;
                if (strcmp(bom,"-be")==0){
                    fwrite(&h,1,1,o);
                    fwrite(&l,1,1,o);
                }
                else{
                    fwrite(&l,1,1,o);
                    fwrite(&h,1,1,o);
                }
            }
            if (c1>>4 == 0xe){
                if (c2==0){
                    fprintf(stderr,"Пустой второй байт %x № %d\n",c1,k);
                    return 0;
                }
                k+=1;
                if (c2 >> 6 != 0b10){
                fprintf(stderr,"Некорректный второй байт %x № %d\n",c2,k);
                } 
                if (fread(&c3,1,1,i)==0){
                    fprintf(stderr,"Пустой третий байт %x № %d\n",c2,k);
                    return 0;
                }
                k+=1;
                if (c3 >> 6 != 0b10){
                    fprintf(stderr,"Некорректный третий байт %x № %d\n",c3,k);
                }
                l = (c3 & 0x3f) + ((c2 & 0b11)<<6);
                h = (c1<<4)+((c2>>2) & 0xf);
                if (strcmp(bom,"-be")==0){
                    fwrite(&h,1,1,o);
                    fwrite(&l,1,1,o);
                }
                else{
                    fwrite(&l,1,1,o);
                    fwrite(&h,1,1,o);
                }
            }
            if((c1 & 0xc0) == 0x80){
                fprintf(stderr,"Некорректный начальный байт %x № %d\n",c1,k);
            }
        }
    }
    else{
        k+=1;
        if (c1 < 128){
            l = c1 & 0x7f;
            h = 0;
            if (strcmp(bom,"-be")==0){
                fwrite(&h,1,1,o);
                fwrite(&l,1,1,o);
            }
            else{
                fwrite(&l,1,1,o);
                fwrite(&h,1,1,o);
            }
        }
        if (c1 >> 5 == 0b110){
            if (fread(&c2,1,1,i)==0){
                fprintf(stderr,"Пустой второй байт %x № %d\n",c1,k);
                return 0;
            }
            k+=1;
            if (c2 >> 6 != 0b10){
                fprintf(stderr,"Некорректный второй байт %x № %d\n",c2,k);
            }
            l = (c2 & 0x3f) + ((c1 & 0b11)<<6);
            h = (c1>>2) & 0b111;
             if (strcmp(bom,"-be")==0){
                fwrite(&h,1,1,o);
                fwrite(&l,1,1,o);
            }
            else{
                fwrite(&l,1,1,o);
                fwrite(&h,1,1,o);
            }
        }
        if (c1>>4 == 0xe){
            if (fread(&c2,1,1,i)==0){
                fprintf(stderr,"Пустой второй байт %x № %d\n",c1,k);
                return 0;
            }
            k+=1;
            if (c2 >> 6 != 0b10){
               fprintf(stderr,"Некорректный второй байт %x № %d\n",c2,k);
               fread(&c2,1,1,i);
            } 
            if (fread(&c3,1,1,i)==0){
                fprintf(stderr,"Пустой третий байт %x № %d\n",c2,k);
                return 0;
            }
            k+=1;
            if (c3 >> 6 != 0b10){
                fprintf(stderr,"Некорректный третий байт %x № %d\n",c3,k);
            }
            l = (c3 & 0x3f) + ((c2 & 0b11)<<6);
            h = (c1<<4)+((c2>>2) & 0xf);
            if (strcmp(bom,"-be")==0){
                fwrite(&h,1,1,o);
                fwrite(&l,1,1,o);
            }
            else{
                fwrite(&l,1,1,o);
                fwrite(&h,1,1,o);
            }
        }
        if((c1 & 0xc0) == 0x80){
            fprintf(stderr,"Некорректный начальный байт %x № %d\n",c1,k);
        }
    }

    //Переркодировка
    
    while (fread(&c1,1,1,i)){
        k+=1;
        if (c1 < 128){
            l = c1 & 0x7f;
            h = 0;
            if (strcmp(bom,"-be")==0){
                fwrite(&h,1,1,o);
                fwrite(&l,1,1,o);
            }
            else{
                fwrite(&l,1,1,o);
                fwrite(&h,1,1,o);
            }
            continue;
        }
        if (c1 >> 5 == 0b110){
            if (fread(&c2,1,1,i)==0){
                fprintf(stderr,"Пустой второй байт %x № %d\n",c1,k);
                return 0;
            }
            k+=1;
            if (c2 >> 6 != 0b10){
                fprintf(stderr,"Некорректный второй байт %x № %d\n",c2,k);
                continue;
            }
            l = (c2 & 0x3f) + ((c1 & 0b11)<<6);
            h = (c1>>2) & 0b111;
             if (strcmp(bom,"-be")==0){
                fwrite(&h,1,1,o);
                fwrite(&l,1,1,o);
            }
            else{
                fwrite(&l,1,1,o);
                fwrite(&h,1,1,o);
            }
            continue;
        }
        if (c1>>4 == 0xe){
            if (fread(&c2,1,1,i)==0){
                fprintf(stderr,"Пустой второй байт %x № %d\n",c1,k);
                return 0;
            }
            k+=1;
            if (c2 >> 6 != 0b10){
               fprintf(stderr,"Некорректный второй байт %x № %d\n",c2,k);
               fread(&c2,1,1,i);
                continue;
            } 
            if (fread(&c3,1,1,i)==0){
                fprintf(stderr,"Пустой третий байт %x № %d\n",c2,k);
                return 0;
            }
            k+=1;
            if (c3 >> 6 != 0b10){
                fprintf(stderr,"Некорректный третий байт %x № %d\n",c3,k);
                continue;
            }
            l = (c3 & 0x3f) + ((c2 & 0b11)<<6);
            h = (c1<<4)+((c2>>2) & 0xf);
            if (strcmp(bom,"-be")==0){
                fwrite(&h,1,1,o);
                fwrite(&l,1,1,o);
            }
            else{
                fwrite(&l,1,1,o);
                fwrite(&h,1,1,o);
            }
            continue; 
        }
        fprintf(stderr,"Некорректный начальный байт %x № %d\n",c1,k);
    }
    fclose(i);
    fclose(o);
    return 0;
}