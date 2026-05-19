#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>       
#include <conio.h>    

FILE* rndfopen(char* path);

int main() {
    char s[10], n[15];   
    long l, ll;          
    int  c, ii;          
    FILE* p;

    printf("\nІм'я файлу? ");

    if (fgets(n, sizeof(n), stdin) == NULL) return 1;

    n[strcspn(n, "\n")] = '\0';

    p = rndfopen(n);
    if (p == NULL) {
        perror("\nПомилка відкриття файлу");
        exit(1);
    }

    while (1) {
        printf("\nЯка позиція? ");
        fgets(s, sizeof(s), stdin);

        l = atol(s);

        if (fseek(p, l, SEEK_SET) != 0) {
            perror("\nНе можу перемістити вказівник");
            continue;
        }

        ll = ftell(p);
        if (ll == -1L) {
            perror("\nПомилка ftell");
            continue;
        }

        ii = fgetc(p);

        if (ii == EOF) {
            printf("\nВ позиції %ld записано (-1)", ll);
        }
        else {
            printf("\nВ позиції %ld записано %c (%d).", ll, ii, ii);
        }

        printf("\nРобимо запис? (y/n)? ");
        if (_getche() == 'y') {
            printf("\nЯкий символ? ");
            c = _getche(); 

            if (fseek(p, l, SEEK_SET) != 0) {
                perror("\nНе можу перемістити вказівник");
            }
            else {
                if (fputc(c, p) == EOF) {
                    perror("\nПомилка запису");
                }
                fflush(p);
            }
        }

        printf("\nЩе? (y/n)? ");
        if (_getche() == 'n') break;

        printf("\n");
    }

    if (fclose(p) == EOF) {
        perror("\nНе можу закрити файл");
    }

    printf("\n");
    return 0;
}

FILE* rndfopen(char* path) {
    FILE* fp;

    if (_access(path, 00) == 0) {
        fp = fopen(path, "r+b");
    }
    else {
        fp = fopen(path, "w+b");
    }

    return fp;
}