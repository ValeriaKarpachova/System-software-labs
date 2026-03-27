#include "stdafx.h"
#include <WinSock2.h>
#include <Ws2tcpip.h>
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

    bind(s, (sockaddr*)&server, sizeof(server));
    listen(s, 1);

    printf("Waiting for client...\n");
    SOCKET client = accept(s, nullptr, nullptr);
    printf("Client connected\n");

    int secret = 23; 
    char buf[256];
    printf("Secret age = 23\n");

    while (true)
    {
        int count = recv(client, buf, 255, 0);
        if (count <= 0) break;
        buf[count] = '\0';

        if (strcmp(buf, "exit") == 0)
        {
            printf("Client disconnected\n");
            break;
        }

        int guess;
        try
        {
            guess = atoi(buf);
        }
        catch (...)
        {
            send(client, "Error", 5, 0);
            continue;
        }

        const char* response;

        if (guess == secret)
            response = "Equal";
        else if (guess < secret)
            response = "More";
        else
            response = "Less";

        send(client, response, strlen(response), 0);
    }

    closesocket(client);
    closesocket(s);
    WSACleanup();
    return 0;
}