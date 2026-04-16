#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 9001
#define BUFFER_SIZE 256

void remove_newline(char *str) {
    if (str == NULL) return;
    str[strcspn(str, "\r\n")] = '\0';
}

int main() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("socket failed");
        return 1;
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("connect failed");
        close(clientSocket);
        return 1;
    }

    char filename[BUFFER_SIZE];
    printf("Enter file name: ");
    fgets(filename, sizeof(filename), stdin);
    remove_newline(filename);

    if (strlen(filename) == 0) {
        printf("Error: No file name entered.\n");
        close(clientSocket);
        return 1;
    }

    send(clientSocket, filename, strlen(filename) + 1, 0);

    char response[BUFFER_SIZE];
    memset(response, 0, sizeof(response));

    int bytesReceived = recv(clientSocket, response, sizeof(response) - 1, 0);
    if (bytesReceived < 0) {
        perror("recv failed");
        close(clientSocket);
        return 1;
    }

    response[bytesReceived] = '\0';

    printf("\nServer response:\n%s\n", response);

    close(clientSocket);
    return 0;
}