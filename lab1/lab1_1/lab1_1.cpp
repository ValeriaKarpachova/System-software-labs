#include <Windows.h>

int main()
{
    WCHAR buf[2048];

    SYSTEMTIME localTime;
    GetLocalTime(&localTime); 

    int count = wsprintf(buf, L"LOCAL TIME:\n"
        L"Date: %02i.%02i.%i\n"
        L"Time: %02i:%02i:%02i\n\n",

        localTime.wDay,
        localTime.wMonth,
        localTime.wYear,
        localTime.wHour,
        localTime.wMinute,
        localTime.wSecond
    );

    SYSTEMTIME sysTime;
    GetSystemTime(&sysTime); 

    count += wsprintf(buf + count,
        L"SYSTEM TIME:\n"
        L"Date: %02i.%02i.%i\n"
        L"Time: %02i:%02i:%02i\n\n",

        sysTime.wDay,
        sysTime.wMonth,
        sysTime.wYear,
        sysTime.wHour,
        sysTime.wMinute,
        sysTime.wSecond
    );

    SYSTEM_INFO info;
    GetSystemInfo(&info);

    count += wsprintf(buf + count,
        L"SYSTEM INFO:\n"
        L"Page size: %u bytes\n"
        L"Min app address: %p\n"
        L"Max app address: %p\n"
        L"Allocation granularity: %u\n"
        L"Number of processors: %u",

        info.dwPageSize,
        info.lpMinimumApplicationAddress,
        info.lpMaximumApplicationAddress,
        info.dwAllocationGranularity,
        info.dwNumberOfProcessors
    );

    MessageBox(NULL, buf, L"System Information", MB_OK);

    return 0;
}