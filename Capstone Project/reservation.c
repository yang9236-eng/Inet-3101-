#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reservation.h"

// Clears leftover characters from input
static void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

// Reads a full line of text and removes the newline
static void readLine(char *buffer, int size) {
    if (fgets(buffer, size, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

// Sets up empty flights when no saved file exists
void initializeFlights(Flight flights[], int flightCount) {
    const char *defaultNames[] = {"Outbound Flight", "Inbound Flight"};

    for (int i = 0; i < flightCount; i++) {
        strncpy(flights[i].flightName, defaultNames[i], MAX_NAME - 1);
        flights[i].flightName[MAX_NAME - 1] = '\0';

        for (int j = 0; j < MAX_SEATS; j++) {
            flights[i].seats[j].seatNumber = j + 1;
            flights[i].seats[j].assigned = 0;
            flights[i].seats[j].firstName[0] = '\0';
            flights[i].seats[j].lastName[0] = '\0';
        }
    }
}

// Loads saved reservation data from the binary data file
void loadFlights(Flight flights[], int flightCount) {
    FILE *file = fopen(DATA_FILE, "rb");

    // If the file does not exist, start with empty flights
    if (file == NULL) {
        initializeFlights(flights, flightCount);
        return;
    }

    size_t readCount = fread(flights, sizeof(Flight), flightCount, file);
    fclose(file);

    // If the saved file is corrupted or incomplete, reset the data
    if (readCount != (size_t)flightCount) {
        printf("Data file was incomplete. Starting with empty reservations.\n");
        initializeFlights(flights, flightCount);
    }
}

// Saves reservation data to a binary file
void saveFlights(Flight flights[], int flightCount) {
    FILE *file = fopen(DATA_FILE, "wb");

    if (file == NULL) {
        printf("Error: Could not save reservation data.\n");
        return;
    }

    fwrite(flights, sizeof(Flight), flightCount, file);
    fclose(file);
}

// Main menu for choosing flight, reports, or quit
void mainMenu(Flight flights[], int flightCount) {
    int choice;

    do {
        printf("\n===== Colossus Travel Reservation System =====\n");
        printf("1. Manage Outbound Flight\n");
        printf("2. Manage Inbound Flight\n");
        printf("3. View Reservation Report\n");
        printf("4. Quit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input. Try again.\n");
            continue;
        }

        clearInputBuffer();

        switch (choice) {
            case 1:
                flightMenu(&flights[0]);
                break;
            case 2:
                flightMenu(&flights[1]);
                break;
            case 3:
                showReport(flights, flightCount);
                break;
            case 4:
                saveFlights(flights, flightCount);
                printf("Reservations saved. Goodbye.\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 4);
}

// Menu for one specific flight
void flightMenu(Flight *flight) {
    int choice;

    do {
        printf("\n===== %s Menu =====\n", flight->flightName);
        printf("1. Show number of empty seats\n");
        printf("2. Show list of empty seats\n");
        printf("3. Show alphabetical list of reserved seats\n");
        printf("4. Assign a customer to a seat\n");
        printf("5. Update a reservation\n");
        printf("6. Delete a reservation\n");
        printf("7. Search reservation by last name\n");
        printf("8. Return to main menu\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input. Try again.\n");
            continue;
        }

        clearInputBuffer();

        switch (choice) {
            case 1:
                showEmptySeatCount(flight);
                break;
            case 2:
                showEmptySeats(flight);
                break;
            case 3:
                showAlphabeticalSeats(flight);
                break;
            case 4:
                assignSeat(flight);
                break;
            case 5:
                updateReservation(flight);
                break;
            case 6:
                deleteReservation(flight);
                break;
            case 7:
                searchReservation(flight);
                break;
            case 8:
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 8);
}

// Counts empty seats for one flight
void showEmptySeatCount(const Flight *flight) {
    int count = 0;

    for (int i = 0; i < MAX_SEATS; i++) {
        if (!flight->seats[i].assigned) {
            count++;
        }
    }

    printf("%s has %d empty seats.\n", flight->flightName, count);
}

// Prints every empty seat for one flight
void showEmptySeats(const Flight *flight) {
    printf("\nEmpty seats for %s:\n", flight->flightName);

    for (int i = 0; i < MAX_SEATS; i++) {
        if (!flight->seats[i].assigned) {
            printf("Seat %d\n", flight->seats[i].seatNumber);
        }
    }
}

// Shows reserved seats sorted by passenger last name
void showAlphabeticalSeats(const Flight *flight) {
    Seat reserved[MAX_SEATS];
    int count = 0;

    // Copy reserved seats into a temporary array
    for (int i = 0; i < MAX_SEATS; i++) {
        if (flight->seats[i].assigned) {
            reserved[count++] = flight->seats[i];
        }
    }

    if (count == 0) {
        printf("No reserved seats found.\n");
        return;
    }

    // Simple bubble sort by last name
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(reserved[i].lastName, reserved[j].lastName) > 0) {
                Seat temp = reserved[i];
                reserved[i] = reserved[j];
                reserved[j] = temp;
            }
        }
    }

    printf("\nReserved seats for %s alphabetically by last name:\n", flight->flightName);

    for (int i = 0; i < count; i++) {
        printf("Seat %d: %s %s\n",
               reserved[i].seatNumber,
               reserved[i].firstName,
               reserved[i].lastName);
    }
}

// CREATE operation: assigns a passenger to a seat
void assignSeat(Flight *flight) {
    int seatNumber;

    printf("Enter seat number to assign (1-%d): ", MAX_SEATS);

    if (scanf("%d", &seatNumber) != 1) {
        clearInputBuffer();
        printf("Invalid input.\n");
        return;
    }

    clearInputBuffer();

    if (seatNumber < 1 || seatNumber > MAX_SEATS) {
        printf("Invalid seat number.\n");
        return;
    }

    Seat *seat = &flight->seats[seatNumber - 1];

    if (seat->assigned) {
        printf("Seat %d is already assigned.\n", seatNumber);
        return;
    }

    printf("Enter first name: ");
    readLine(seat->firstName, MAX_NAME);

    printf("Enter last name: ");
    readLine(seat->lastName, MAX_NAME);

    seat->assigned = 1;

    printf("Seat %d assigned to %s %s.\n",
           seatNumber,
           seat->firstName,
           seat->lastName);
}

// UPDATE operation: changes passenger name on an existing reservation
void updateReservation(Flight *flight) {
    int seatNumber;

    printf("Enter seat number to update (1-%d): ", MAX_SEATS);

    if (scanf("%d", &seatNumber) != 1) {
        clearInputBuffer();
        printf("Invalid input.\n");
        return;
    }

    clearInputBuffer();

    if (seatNumber < 1 || seatNumber > MAX_SEATS) {
        printf("Invalid seat number.\n");
        return;
    }

    Seat *seat = &flight->seats[seatNumber - 1];

    if (!seat->assigned) {
        printf("Seat %d is not currently assigned.\n", seatNumber);
        return;
    }

    printf("Current reservation: %s %s\n", seat->firstName, seat->lastName);

    printf("Enter new first name: ");
    readLine(seat->firstName, MAX_NAME);

    printf("Enter new last name: ");
    readLine(seat->lastName, MAX_NAME);

    printf("Reservation updated for seat %d.\n", seatNumber);
}

// DELETE operation: removes a reservation from a seat
void deleteReservation(Flight *flight) {
    int seatNumber;

    printf("Enter seat number to delete (1-%d): ", MAX_SEATS);

    if (scanf("%d", &seatNumber) != 1) {
        clearInputBuffer();
        printf("Invalid input.\n");
        return;
    }

    clearInputBuffer();

    if (seatNumber < 1 || seatNumber > MAX_SEATS) {
        printf("Invalid seat number.\n");
        return;
    }

    Seat *seat = &flight->seats[seatNumber - 1];

    if (!seat->assigned) {
        printf("Seat %d is already empty.\n", seatNumber);
        return;
    }

    seat->assigned = 0;
    seat->firstName[0] = '\0';
    seat->lastName[0] = '\0';

    printf("Reservation deleted for seat %d.\n", seatNumber);
}

// READ operation: searches reservations by last name
void searchReservation(const Flight *flight) {
    char lastName[MAX_NAME];
    int found = 0;

    printf("Enter last name to search: ");
    readLine(lastName, MAX_NAME);

    for (int i = 0; i < MAX_SEATS; i++) {
        if (flight->seats[i].assigned &&
            strcmp(flight->seats[i].lastName, lastName) == 0) {
            printf("Found: Seat %d - %s %s\n",
                   flight->seats[i].seatNumber,
                   flight->seats[i].firstName,
                   flight->seats[i].lastName);
            found = 1;
        }
    }

    if (!found) {
        printf("No reservation found for last name: %s\n", lastName);
    }
}

// Shows booked and empty seat counts for all flights
void showReport(const Flight flights[], int flightCount) {
    printf("\n===== Reservation Report =====\n");

    for (int i = 0; i < flightCount; i++) {
        int booked = 0;

        for (int j = 0; j < MAX_SEATS; j++) {
            if (flights[i].seats[j].assigned) {
                booked++;
            }
        }

        printf("%s\n", flights[i].flightName);
        printf("Booked seats: %d\n", booked);
        printf("Empty seats: %d\n\n", MAX_SEATS - booked);
    }
}

// Builds a text report that the server sends to the client
char *getAvailabilityReport(const Flight flights[], int flightCount, char *buffer, int bufferSize) {
    int used = 0;

    used += snprintf(buffer + used, bufferSize - used,
                     "===== Seat Availability Report =====\n");

    for (int i = 0; i < flightCount; i++) {
        int empty = 0;

        for (int j = 0; j < MAX_SEATS; j++) {
            if (!flights[i].seats[j].assigned) {
                empty++;
            }
        }

        used += snprintf(buffer + used, bufferSize - used,
                         "%s: %d empty seats, %d booked seats\n",
                         flights[i].flightName,
                         empty,
                         MAX_SEATS - empty);

        if (used >= bufferSize) {
            break;
        }
    }

    return buffer;
}
