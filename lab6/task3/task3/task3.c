#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

int main() {
    int i;

    for (i = 1; i <= 5; i++) {

        if (freopen("data_streams.txt", "a", stdout) == NULL) {
            perror("Помилка freopen -> файл");
            return 1;
        }

        puts("Рядок виведений у файл через puts()");
        fflush(stdout); 

        if (freopen("CON", "w", stdout) == NULL) {

            return 1;
        }

        printf("Рядок %d виведений на екран\n", i);
        fflush(stdout);
        }

    return 0;
    }