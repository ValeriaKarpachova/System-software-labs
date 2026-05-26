#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>       
#include <fcntl.h>     
#include <sys/stat.h>  
#define R 0   
#define W 1   

#pragma pack(1)
struct _rec {
    char name[14];
    int  gr;
};

union _bufrec {
    char        bufrec[16];
    struct _rec rec;
} rbuf;
#pragma pack()

typedef struct afile {
    char* buf; /* буфер обміну 512 байт (один дисковий блок) */
    int bz; /* Номер блоку завантаженого в буфер, -1 = порожньо */
    int w; /* прапор: 1 = буфер змінено і треба скинути на диск */
    int rcrdsz; /* Розмір одного логічного запису в байтах */
    int numrec; /* кількість логічних записів у файлі */
    int handler;/* хендл відкритого файлу */
} AFILE;

int    acreate(char* name, int rcrdsz, int numrec);
AFILE* aopen(char* name);
int    areadwrite(AFILE* ap, int nr, char* rec, int mode);
int    aclose(AFILE* ap);


/* Структура файлу на диску:
 *   [2 байта: rcrdsz][2 байта: numrec][rcrdsz*numrec байт: дані]*/
int acreate(char* name, int rcrdsz, int numrec) {
    int fh;
    short sz = (short)rcrdsz;  /* short = 2 байта */
    short num = (short)numrec;

    fh = _open(name, O_RDONLY | O_BINARY);
    if (fh != -1) {
        _close(fh);
        return -1; 
    }

    fh = _open(name, O_WRONLY | O_CREAT | O_BINARY, S_IREAD | S_IWRITE);
    if (fh == -1) {
        perror("acreate: помилка створення файлу");
        return -1;
    }

    /*
    * Записуємо службовий заголовок: 2 байти розміру запису,
    * потім 2 байти кількості записів.
    * write (хендл, буфер, кількість_байт)
    */
    _write(fh, &sz, sizeof(short)); /* позиція 0-1: розмір запису  */
    _write(fh, &num, sizeof(short)); /* позиція 2-3: кількість записів */

    /*chsize заповнює додане місце нулями.*/
    _chsize(fh, 4L + (long)rcrdsz * numrec);

    _close(fh);
    return 0;
}

AFILE* aopen(char* name) {
    short sz, num;

    AFILE* ap = (AFILE*)malloc(sizeof(AFILE));
    if (!ap) return NULL;

    ap->buf = (char*)malloc(512);
    if (!ap->buf) { free(ap); return NULL; }

    ap->handler = _open(name, O_RDWR | O_BINARY);
    if (ap->handler == -1) {
        perror("aopen: помилка відкриття");
        free(ap->buf);
        free(ap);
        return NULL;
    }

    _read(ap->handler, &sz, sizeof(short));
    _read(ap->handler, &num, sizeof(short));

    ap->rcrdsz = sz;
    ap->numrec = num;
    ap->w = 0;   
    ap->bz = -1;  

    return ap;
}

int areadwrite(AFILE* ap, int nr, char* rec, int mode) {
    int  i;
    int  rcrdsz = ap->rcrdsz;
    int  bl = (nr * rcrdsz) / 512;
    int  n = (nr * rcrdsz) % 512;

    if (bl != ap->bz) {

        if (ap->w == 1) {
            _lseek(ap->handler, 4L + (long)ap->bz * 512, SEEK_SET);
            _write(ap->handler, ap->buf, 512);
            ap->w = 0;
        }

        _lseek(ap->handler, 4L + (long)bl * 512, SEEK_SET);
        memset(ap->buf, 0, 512);
        _read(ap->handler, ap->buf, 512);

        ap->bz = bl; 
        ap->w = 0;
    }

    if (mode == R) {
        for (i = 0; i < rcrdsz; i++)
            rec[i] = (ap->buf + n)[i];
    }
    else {
        for (i = 0; i < rcrdsz; i++)
            (ap->buf + n)[i] = rec[i];
        ap->w = 1;
    }

    return 0;
}

int aclose(AFILE* ap) {
    if (ap->w == 1) {
        _lseek(ap->handler, 4L + (long)ap->bz * 512, SEEK_SET);
        _write(ap->handler, ap->buf, 512);
    }

    free(ap->buf);          
    _close(ap->handler);    
    free(ap);               
    return 0;
}

int main() {
    int   i, nr;
    char  s[50], c, c1;
    AFILE* ap;

    printf("\nВведіть ім'я нового файлу прямого доступу"
        " (100 записів по 16 байт): ");
    fgets(s, sizeof(s), stdin);
    s[strcspn(s, "\n")] = '\0'; 

    if (-1 == acreate(s, 16, 100))
        printf("\nФайл вже існує, працюємо з існуючим %s\n", s);

    ap = aopen(s);
    if (!ap) {
        printf("Помилка відкриття!\n");
        return 1;
    }

    do {
        for (i = 0; i < 16; i++) rbuf.bufrec[i] = '\0';

        printf("\nНомер запису? ");
        scanf("%d", &nr);
        while (getchar() != '\n');

        areadwrite(ap, nr, rbuf.bufrec, R);

        printf("\nЗапис #%d. Вміст полів:\n  Ім'я=\"%s\". Рік народження=%d",
            nr, rbuf.rec.name, rbuf.rec.gr);

        printf("\nЧи будете змінювати запис (y/n)? ");
        c = getchar();
        getchar();

        if (c == 'y') {
            printf("\nПрізвище? ");
            fgets(rbuf.rec.name, 14, stdin);
            rbuf.rec.name[strcspn(rbuf.rec.name, "\n")] = '\0';

            printf("\nРік народження? ");
            scanf("%d", &rbuf.rec.gr);
            while (getchar() != '\n');  

            areadwrite(ap, nr, rbuf.bufrec, W);
        }

        printf("\nЧи будете працювати з файлом ще (y/n)? ");
       
        c1 = getchar();
        while (getchar() != '\n');  

    } while (c1 == 'y');

    aclose(ap);

    printf("\nГотово!\n");
    return 0;
}