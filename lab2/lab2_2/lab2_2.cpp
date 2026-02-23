#include <iostream>
#include <windows.h>
using namespace std;

double x = 5, y = 6, z = 8;

HANDLE sem1, sem2, sem3, sem4;

DWORD WINAPI T1(LPVOID lpParam)
{
    x *= 5;
    ReleaseSemaphore(sem1, 1, NULL);
    return 0;
}

DWORD WINAPI T2(LPVOID lpParam)
{
    y += 2;
    ReleaseSemaphore(sem2, 1, NULL);
    return 0;
}

DWORD WINAPI T3(LPVOID lpParam)
{
    WaitForSingleObject(sem1, INFINITE);
    WaitForSingleObject(sem2, INFINITE);

    y -= 3;
    ReleaseSemaphore(sem3, 1, NULL);

    return 0;
}

DWORD WINAPI T4(LPVOID lpParam)
{
    WaitForSingleObject(sem3, INFINITE);

    x += 2;
    ReleaseSemaphore(sem4, 1, NULL);

    return 0;
}

DWORD WINAPI T5(LPVOID lpParam)
{
    WaitForSingleObject(sem4, INFINITE);

    y *= x;

    return 0;
}

int main()
{
    cout << "x = " << x << "; y = " << y << "; z = " << z << endl;

    DWORD tid1, tid2, tid3, tid4, tid5;

    sem1 = CreateSemaphore(NULL, 0, 1, NULL);
    sem2 = CreateSemaphore(NULL, 0, 1, NULL);
    sem3 = CreateSemaphore(NULL, 0, 1, NULL);
    sem4 = CreateSemaphore(NULL, 0, 1, NULL);

    HANDLE h1 = CreateThread(NULL, 0, T1, NULL, 0, &tid1);
    HANDLE h2 = CreateThread(NULL, 0, T2, NULL, 0, &tid2);
    HANDLE h3 = CreateThread(NULL, 0, T3, NULL, 0, &tid3);
    HANDLE h4 = CreateThread(NULL, 0, T4, NULL, 0, &tid4);
    HANDLE h5 = CreateThread(NULL, 0, T5, NULL, 0, &tid5);

    WaitForSingleObject(h1, INFINITE);
    WaitForSingleObject(h2, INFINITE);
    WaitForSingleObject(h3, INFINITE);
    WaitForSingleObject(h4, INFINITE);
    WaitForSingleObject(h5, INFINITE);

    cout << "x = " << x << "; y = " << y << "; z = " << z << endl;

    CloseHandle(sem1);
    CloseHandle(sem2);
    CloseHandle(sem3);
    CloseHandle(sem4);

    CloseHandle(h1);
    CloseHandle(h2);
    CloseHandle(h3);
    CloseHandle(h4);
    CloseHandle(h5);

    system("pause");
    return 0;
}