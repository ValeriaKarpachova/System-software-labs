#include <windows.h>
#include <stdio.h>

int main()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    WCHAR buffer[200];
    DWORD written;
    char choice;

    for (;;)
    {
        SetConsoleTextAttribute(hOut, FOREGROUND_RED + FOREGROUND_GREEN + FOREGROUND_BLUE);
        wsprintf(buffer, L"\nChoose program:\n");
        WriteConsole(hOut, buffer, lstrlen(buffer), &written, NULL);

        SetConsoleTextAttribute(hOut, FOREGROUND_RED + FOREGROUND_INTENSITY);
        wsprintf(buffer, L"p - Paint\n");
        WriteConsole(hOut, buffer, lstrlen(buffer), &written, NULL);

        SetConsoleTextAttribute(hOut, FOREGROUND_GREEN + FOREGROUND_INTENSITY);
        wsprintf(buffer, L"c - Calculator\n");
        WriteConsole(hOut, buffer, lstrlen(buffer), &written, NULL);

        SetConsoleTextAttribute(hOut, FOREGROUND_BLUE + FOREGROUND_INTENSITY);
        wsprintf(buffer, L"n - Notepad\n");
        WriteConsole(hOut, buffer, lstrlen(buffer), &written, NULL);

        SetConsoleTextAttribute(hOut, FOREGROUND_RED + FOREGROUND_GREEN + FOREGROUND_BLUE);
        wsprintf(buffer, L"e - Exit\n");
        WriteConsole(hOut, buffer, lstrlen(buffer), &written, NULL);

        choice = getchar();
        getchar(); 

        if (choice == 'e') break;

        switch (choice)
        {
        case 'p':
            ShellExecute(NULL, L"open", L"mspaint.exe", NULL, NULL, SW_SHOWNORMAL);
            
            SetConsoleTextAttribute(hOut, FOREGROUND_RED + FOREGROUND_INTENSITY);
            wsprintf(buffer, L"\nopen Paint:\n");
            WriteConsole(hOut, buffer, lstrlen(buffer), &written, NULL);

            break;

        case 'c':
            ShellExecute(NULL, L"open", L"calc.exe", NULL, NULL, SW_SHOWNORMAL);

            SetConsoleTextAttribute(hOut, FOREGROUND_GREEN + FOREGROUND_INTENSITY);
            wsprintf(buffer, L"\nopen Calculator:\n");
            WriteConsole(hOut, buffer, lstrlen(buffer), &written, NULL);

            break;

        case 'n':
            ShellExecute(NULL, L"open", L"notepad.exe", NULL, NULL, SW_SHOWNORMAL);

            SetConsoleTextAttribute(hOut, FOREGROUND_BLUE + FOREGROUND_INTENSITY);
            wsprintf(buffer, L"\nopen Notepad:\n");
            WriteConsole(hOut, buffer, lstrlen(buffer), &written, NULL);

            break;
        }
    }
    return 0;
}