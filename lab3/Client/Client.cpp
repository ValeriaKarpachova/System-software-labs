#include "stdafx.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server;
    InetPton(AF_INET, L"127.0.0.1", &server.sin_addr);
    server.sin_family = AF_INET;
    server.sin_port = htons(6000);

    connect(s, (sockaddr*)&server, sizeof(server));

    char input[256];
    char response[256];

    while (true)
    {
        printf("Enter age: ");
        std::cin >> input;

        send(s, input, strlen(input), 0);

        if (strcmp(input, "exit") == 0)
            break;

        int count = recv(s, response, 255, 0);
        response[count] = '\0';

        printf("Server: %s\n", response);
    }

    closesocket(s);
    WSACleanup();
    return 0;
}