#include <iostream>
#include <windows.h>
using namespace std;

HANDLE semA; 
HANDLE semB; 

DWORD WINAPI Func1(LPVOID data)
{
    cout << "P1: Event A happened -> signal P2" << endl;

    ReleaseSemaphore(semA, 1, NULL);

    return 0;
}

DWORD WINAPI Func2(LPVOID data)
{
    WaitForSingleObject(semA, INFINITE);

    cout << "P2: Received A -> Event B happened -> signal P3" << endl;

    ReleaseSemaphore(semB, 1, NULL);

    return 0;
}

DWORD WINAPI Func3(LPVOID data)
{
    WaitForSingleObject(semB, INFINITE);

    cout << "P3: Received B" << endl;

    return 0;
}

int main()
{
    DWORD tid1, tid2, tid3;

    semA = CreateSemaphore(NULL, 0, 1, NULL);
    semB = CreateSemaphore(NULL, 0, 1, NULL);

    HANDLE t1 = CreateThread(NULL, 0, Func1, NULL, 0, &tid1);
    HANDLE t2 = CreateThread(NULL, 0, Func2, NULL, 0, &tid2);
    HANDLE t3 = CreateThread(NULL, 0, Func3, NULL, 0, &tid3);

    WaitForSingleObject(t1, INFINITE);
    WaitForSingleObject(t2, INFINITE);
    WaitForSingleObject(t3, INFINITE);

    CloseHandle(semA);
    CloseHandle(semB);
    CloseHandle(t1);
    CloseHandle(t2);
    CloseHandle(t3);

    return 0;
}
