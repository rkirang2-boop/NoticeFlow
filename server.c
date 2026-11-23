#include <string.h>
#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

int main() {

    WSADATA wsa;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddress;

    char response[] =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Content-Length: 34\r\n"
        "Connection: close\r\n"
        "\r\n"
        "NoticeFlow C Server is Running!";

    printf("Starting NoticeFlow Server...\n");

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Winsock initialization failed!\n");
        return 1;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == INVALID_SOCKET) {
        printf("Socket creation failed!\n");
        WSACleanup();
        return 1;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8080);

    if (bind(serverSocket,
        (struct sockaddr *)&serverAddress,
        sizeof(serverAddress)) == SOCKET_ERROR) {

        printf("Port binding failed!\n");
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    listen(serverSocket, 5);

    printf("\nNoticeFlow server is running!\n");
    printf("Open http://localhost:8080 in your browser.\n");

    while (1) {

        clientSocket = accept(serverSocket, NULL, NULL);

        if (clientSocket == INVALID_SOCKET) {
            continue;
        }

        char request[2048] = {0};

        recv(clientSocket, request, sizeof(request) - 1, 0);

        printf("Browser request received!\n");

        send(clientSocket, response,
             (int)strlen(response), 0);

        closesocket(clientSocket);
    }

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
