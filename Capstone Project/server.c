#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "reservation.h"

#define PORT 8080
#define BUFFER_SIZE 1024

// This server sends a real-time seat availability report to the client
int main(void) {
    int serverSocket;
    int clientSocket;
    struct sockaddr_in serverAddress;
    char response[BUFFER_SIZE];
    Flight flights[2];

    // Load current reservation data
    loadFlights(flights, 2);

    // Create TCP socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket < 0) {
        perror("Socket failed");
        return 1;
    }

    // Configure the server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    // Bind socket to port
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Bind failed");
        close(serverSocket);
        return 1;
    }

    // Listen for incoming client connections
    if (listen(serverSocket, 5) < 0) {
        perror("Listen failed");
        close(serverSocket);
        return 1;
    }

    printf("Reservation availability server running on port %d...\n", PORT);
    printf("Press Ctrl+C to stop the server.\n");

    while (1) {
        // Accept one client connection
        clientSocket = accept(serverSocket, NULL, NULL);

        if (clientSocket < 0) {
            perror("Accept failed");
            continue;
        }

        // Reload data each time so the report uses the newest saved reservations
        loadFlights(flights, 2);

        // Build the availability report
        getAvailabilityReport(flights, 2, response, BUFFER_SIZE);

        // Send report to client
        send(clientSocket, response, strlen(response), 0);

        // Close client connection
        close(clientSocket);
    }

    close(serverSocket);
    return 0;
}
