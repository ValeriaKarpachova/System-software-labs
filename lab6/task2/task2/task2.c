#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <windows.h>

typedef LONG(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);

int main() {

    int file_handle;
    int saved_stdout;
    int i;

    RTL_OSVERSIONINFOW rovi = { 0 };
    rovi.dwOSVersionInfoSize = sizeof(rovi);

    HMODULE hMod = GetModuleHandleW(L"ntdll.dll");

    if (hMod) {
        RtlGetVersionPtr fxPtr =
            (RtlGetVersionPtr)GetProcAddress(hMod, "RtlGetVersion");

        if (fxPtr != NULL) {
            fxPtr(&rovi);
        }
    }

    saved_stdout = _dup(1);
    if (saved_stdout == -1) {
        perror("Помилка dup");
        return 1;
    }

    file_handle = _open("data.txt",
        O_WRONLY | O_CREAT | O_APPEND, S_IWRITE
    );

    if (file_handle == -1) {
        perror("Помилка відкриття файлу");
        return 1;
    }

    for (i = 1; i <= 5; i++) {

        if (_dup2(file_handle, 1) == -1) {
            perror("Помилка dup2 -> файл");
            return 1;
        }

        printf("Рядок %d: Windows %ld.%ld build %ld (запис у файл)\n", i,
            rovi.dwMajorVersion,
            rovi.dwMinorVersion,
            rovi.dwBuildNumber
        );

        fflush(stdout);

        if (_dup2(saved_stdout, 1) == -1) {
            perror("Помилка dup2 -> екран");
            return 1;
        }

        printf("Рядок %d: Windows %ld.%ld build %ld (виведення на екран)\n", i,
            rovi.dwMajorVersion,
            rovi.dwMinorVersion,
            rovi.dwBuildNumber
        );

        fflush(stdout);
    }

    _close(file_handle);
    _close(saved_stdout);

    return 0;
}