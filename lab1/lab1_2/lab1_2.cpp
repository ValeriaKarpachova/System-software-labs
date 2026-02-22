#include <windows.h>
#include <iostream>
#include <ctime>

using namespace std;

int numbers[18];

DWORD WINAPI PrintSquares(LPVOID lpParam)
{
    cout << "\nT0: Squares of elements:\n";

    for (int i = 0; i < 18; i++)
    {
        cout << numbers[i] << "^2 = " << numbers[i] * numbers[i] << endl;
    }

    return 0;
}

DWORD WINAPI PrintLessThan50(LPVOID lpParam)
{
    cout << "\nT1: Numbers less than 50:\n";

    for (int i = 0; i < 18; i++)
    {
        if (numbers[i] < 50)
            cout << numbers[i] << endl;
    }

    return 0;
}

int main()
{
    srand(time(NULL)); 

    for (int i = 0; i < 18; i++)
    {
        numbers[i] = rand() % 91;
    }

    cout << "Array:\n";
    for (int i = 0; i < 18; i++)
        cout << numbers[i] << " ";

    cout << endl;

    HANDLE T0, T1;
    DWORD id0, id1;

    T0 = CreateThread(NULL, 0, PrintSquares, NULL, 0, &id0);
    WaitForSingleObject(T0, INFINITE);
    CloseHandle(T0);

    T1 = CreateThread(NULL, 0, PrintLessThan50, NULL, 0, &id1);
    WaitForSingleObject(T1, INFINITE);
    CloseHandle(T1);

    system("pause");
    return 0;
}