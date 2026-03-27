#include <iostream>
#include <Windows.h>
using namespace std;

int main()
{
    cout << "Server\n";
    const wchar_t* pipeName = L"\\\\.\\pipe\\MyPipe";
   
    HANDLE hPipe = CreateNamedPipe(
        pipeName,
        PIPE_ACCESS_DUPLEX,       
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        1,                       
        sizeof(int) * 2,          
        sizeof(int),              
        0,                        
        NULL                      
    );

    if (hPipe == INVALID_HANDLE_VALUE) {
        cout << "CreateNamedPipe failed\n";
        return 1;
    }

    cout << "Waiting for client...\n";
    ConnectNamedPipe(hPipe, NULL); 
    cout << "Client connected\n";

    int numbers[2]; 
    DWORD bytesRead;

    if (ReadFile(hPipe, numbers, sizeof(numbers), &bytesRead, NULL)) {
        int sum = numbers[0] + numbers[1];
        cout << "Received: " << numbers[0] << ", " << numbers[1]
            << " -> Sum = " << sum << endl;

        DWORD bytesWritten;
        WriteFile(hPipe, &sum, sizeof(sum), &bytesWritten, NULL);
    }

    CloseHandle(hPipe);
    system("pause");
    return 0;
}