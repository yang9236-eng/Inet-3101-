#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 9001
#define BUFFER_SIZE 256
#define MAX_FILE_CONTENT 255

int serverSocketID = -1;

void handle_sigint(int sig) {
    (void)sig;
    printf("\nServer shutting down gracefully...\n");
    if (serverSocketID != -1) {
        close(serverSocketID);
    }
    exit(0);
}

int is_valid_filename(const char *filename) {
    if (filename == NULL || strlen(filename) == 0) {
        return 0;
    }

    // Prevent path traversal and weird names
    if (strstr(filename, "..") != NULL || strchr(filename, '/') != NULL || strchr(filename, '\\') != NULL) {
        return 0;
    }

    for (size_t i = 0; i < strlen(filename); i++) {
        if (!isalnum((unsigned char)filename[i]) &&
            filename[i] != '.' &&
            filename[i] != '_' &&
            filename[i] != '-') {
            return 0;
        }
    }

    return 1;
}

void remove_newline(char *str) {
    if (str == NULL) return;
    str[strcspn(str, "\r\n")] = '\0';
}

int main() {
    signal(SIGINT, handle_sigint);

    serverSocketID = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocketID < 0) {
        perror("socket failed");
        return 1;
    }

    int opt = 1;
    if (setsockopt(serverSocketID, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        close(serverSocketID);
        return 1;
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocketID, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind failed");
        close(serverSocketID);
        return 1;
    }

    if (listen(serverSocketID, 5) < 0) {
        perror("listen failed");
        close(serverSocketID);
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        printf("Waiting for client connection...\n");

        int clientSocket = accept(serverSocketID, NULL, NULL);
        if (clientSocket < 0) {
            perror("accept failed");
            continue;
        }

        char filename[BUFFER_SIZE];
        memset(filename, 0, sizeof(filename));

        int bytesReceived = recv(clientSocket, filename, sizeof(filename) - 1, 0);
        if (bytesReceived <= 0) {
            printf("No filename received from client.\n");
            close(clientSocket);
            continue;
        }

        filename[bytesReceived] = '\0';
        remove_newline(filename);

        printf("Client requested file: %s\n", filename);

        char response[BUFFER_SIZE];
        memset(response, 0, sizeof(response));

        if (!is_valid_filename(filename)) {
            snprintf(response, sizeof(response), "ERROR: Invalid file name.");
            send(clientSocket, response, strlen(response) + 1, 0);
            close(clientSocket);
            continue;
        }

        FILE *fp = fopen(filename, "r");
        if (fp == NULL) {
            snprintf(response, sizeof(response), "ERROR: File not found.");
            send(clientSocket, response, strlen(response) + 1, 0);
            close(clientSocket);
            continue;
        }

        char fileContent[BUFFER_SIZE];
        memset(fileContent, 0, sizeof(fileContent));

        size_t bytesRead = fread(fileContent, 1, MAX_FILE_CONTENT + 1, fp);
        fclose(fp);

        if (bytesRead > MAX_FILE_CONTENT) {
            snprintf(response, sizeof(response), "ERROR: File size exceeds 255 characters.");
            send(clientSocket, response, strlen(response) + 1, 0);
            close(clientSocket);
            continue;
        }

        fileContent[bytesRead] = '\0';
        send(clientSocket, fileContent, strlen(fileContent) + 1, 0);

        printf("Response sent to client.\n");
        close(clientSocket);
    }

    close(serverSocketID);
    return 0;
}