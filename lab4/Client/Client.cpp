#include <iostream>
#include <Windows.h>
using namespace std;

int main()
{
    cout << "Client\n";
    const wchar_t* pipeName = L"\\\\.\\pipe\\MyPipe";
    
    HANDLE hPipe = CreateFile(
        pipeName,
        GENERIC_READ | GENERIC_WRITE,
        0, NULL, OPEN_EXISTING, 0, NULL
    );

    if (hPipe == INVALID_HANDLE_VALUE) {
        cout << "Failed to connect to pipe\n";
        return 1;
    }

    int a, b;
    cout << "Enter two numbers: ";
    cin >> a >> b;

    int numbers[2] = { a, b };
    DWORD bytesWritten;

    WriteFile(hPipe, numbers, sizeof(numbers), &bytesWritten, NULL);

    int sum;
    DWORD bytesRead;

    ReadFile(hPipe, &sum, sizeof(sum), &bytesRead, NULL);
    cout << "Sum from server: " << sum << endl;

    CloseHandle(hPipe);
    return 0;
}