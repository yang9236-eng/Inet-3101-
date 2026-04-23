#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TOTAL_SEATS 48
#define NAME_LEN 30
#define DATA_FILE "reservations.dat"

typedef struct {
    int seatNumber;
    int assigned;
    char firstName[NAME_LEN];
    char lastName[NAME_LEN];
} Seat;

Seat seats[TOTAL_SEATS];

/* Function prototypes */
void initializeSeats();
void loadReservations();
void saveReservations();

void mainMenu();
void flightMenu(int start, int end, const char *flightName);

void showEmptyCount(int start, int end);
void showEmptySeats(int start, int end);
void showAlphabeticalSeats(int start, int end);
void assignSeat(int start, int end);
void deleteSeat(int start, int end);

void printFlightSeats(int start, int end);
void clearInputBuffer();

int main() {
    initializeSeats();
    loadReservations();
    mainMenu();
    saveReservations();
    printf("\nReservation data saved. Exiting program.\n");
    return 0;
}

/* Initialize all seats */
void initializeSeats() {
    for (int i = 0; i < TOTAL_SEATS; i++) {
        seats[i].seatNumber = i + 1;
        seats[i].assigned = 0;
        strcpy(seats[i].firstName, "");
        strcpy(seats[i].lastName, "");
    }
}

/* Load reservations from file if it exists */
void loadReservations() {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (fp == NULL) {
        printf("No existing reservation file found. Starting fresh.\n");
        return;
    }

    size_t count = fread(seats, sizeof(Seat), TOTAL_SEATS, fp);
    fclose(fp);

    if (count == TOTAL_SEATS) {
        printf("Reservation data loaded successfully from file.\n");
    } else {
        printf("Warning: reservation file incomplete or corrupted. Starting with current data.\n");
    }
}

/* Save reservations to file */
void saveReservations() {
    FILE *fp = fopen(DATA_FILE, "wb");
    if (fp == NULL) {
        printf("Error: could not save reservation data.\n");
        return;
    }

    fwrite(seats, sizeof(Seat), TOTAL_SEATS, fp);
    fclose(fp);
}

/* Main menu */
void mainMenu() {
    int choice;

    do {
        printf("\n========== COLOSSUS AIRLINES ==========\n");
        printf("1. Outbound Flight\n");
        printf("2. Inbound Flight\n");
        printf("3. Quit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        switch (choice) {
            case 1:
                flightMenu(0, 23, "Outbound Flight");
                break;
            case 2:
                flightMenu(24, 47, "Inbound Flight");
                break;
            case 3:
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }

    } while (choice != 3);
}

/* Flight-specific menu */
void flightMenu(int start, int end, const char *flightName) {
    int choice;

    do {
        printf("\n========== %s ==========\n", flightName);
        printf("1. Show number of empty seats\n");
        printf("2. Show list of empty seats\n");
        printf("3. Show alphabetical list of seats\n");
        printf("4. Assign a customer to a seat\n");
        printf("5. Delete a seat assignment\n");
        printf("6. Return to Main Menu\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        switch (choice) {
            case 1:
                showEmptyCount(start, end);
                break;
            case 2:
                showEmptySeats(start, end);
                break;
            case 3:
                showAlphabeticalSeats(start, end);
                break;
            case 4:
                assignSeat(start, end);
                saveReservations();
                break;
            case 5:
                deleteSeat(start, end);
                saveReservations();
                break;
            case 6:
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }

    } while (choice != 6);
}

/* Show number of empty seats */
void showEmptyCount(int start, int end) {
    int count = 0;
    for (int i = start; i <= end; i++) {
        if (!seats[i].assigned) {
            count++;
        }
    }
    printf("Number of empty seats: %d\n", count);
}

/* Show list of empty seats */
void showEmptySeats(int start, int end) {
    printf("Empty seats: ");
    int found = 0;
    for (int i = start; i <= end; i++) {
        if (!seats[i].assigned) {
            printf("%d ", seats[i].seatNumber);
            found = 1;
        }
    }
    if (!found) {
        printf("None");
    }
    printf("\n");
}

/* Show seats sorted alphabetically by last name */
void showAlphabeticalSeats(int start, int end) {
    Seat temp[24];
    int count = 0;

    for (int i = start; i <= end; i++) {
        if (seats[i].assigned) {
            temp[count++] = seats[i];
        }
    }

    if (count == 0) {
        printf("No assigned seats.\n");
        return;
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            int cmp = strcmp(temp[i].lastName, temp[j].lastName);
            if (cmp > 0 || (cmp == 0 && strcmp(temp[i].firstName, temp[j].firstName) > 0)) {
                Seat swap = temp[i];
                temp[i] = temp[j];
                temp[j] = swap;
            }
        }
    }

    printf("\nAssigned seats in alphabetical order:\n");
    printf("%-8s %-15s %-15s\n", "Seat", "First Name", "Last Name");
    for (int i = 0; i < count; i++) {
        printf("%-8d %-15s %-15s\n", temp[i].seatNumber, temp[i].firstName, temp[i].lastName);
    }
}

/* Assign a customer to a seat */
void assignSeat(int start, int end) {
    int seatNum;

    showEmptySeats(start, end);
    printf("Enter seat number to assign (-1 to abort): ");
    if (scanf("%d", &seatNum) != 1) {
        clearInputBuffer();
        printf("Invalid input.\n");
        return;
    }

    if (seatNum == -1) {
        printf("Assignment aborted.\n");
        return;
    }

    if (seatNum < start + 1 || seatNum > end + 1) {
        printf("Seat number out of range for this flight.\n");
        return;
    }

    int index = seatNum - 1;
    if (seats[index].assigned) {
        printf("Seat %d is already assigned.\n", seatNum);
        return;
    }

    printf("Enter first name: ");
    scanf("%29s", seats[index].firstName);

    printf("Enter last name: ");
    scanf("%29s", seats[index].lastName);

    seats[index].assigned = 1;
    printf("Seat %d assigned to %s %s.\n",
           seats[index].seatNumber,
           seats[index].firstName,
           seats[index].lastName);
}

/* Delete a seat assignment */
void deleteSeat(int start, int end) {
    int seatNum;

    printFlightSeats(start, end);
    printf("Enter seat number to delete (-1 to abort): ");
    if (scanf("%d", &seatNum) != 1) {
        clearInputBuffer();
        printf("Invalid input.\n");
        return;
    }

    if (seatNum == -1) {
        printf("Deletion aborted.\n");
        return;
    }

    if (seatNum < start + 1 || seatNum > end + 1) {
        printf("Seat number out of range for this flight.\n");
        return;
    }

    int index = seatNum - 1;
    if (!seats[index].assigned) {
        printf("Seat %d is already empty.\n", seatNum);
        return;
    }

    seats[index].assigned = 0;
    strcpy(seats[index].firstName, "");
    strcpy(seats[index].lastName, "");
    printf("Seat %d assignment deleted.\n", seatNum);
}

/* Print all seats in a flight */
void printFlightSeats(int start, int end) {
    printf("\n%-8s %-10s %-15s %-15s\n", "Seat", "Assigned", "First Name", "Last Name");
    for (int i = start; i <= end; i++) {
        printf("%-8d %-10s %-15s %-15s\n",
               seats[i].seatNumber,
               seats[i].assigned ? "Yes" : "No",
               seats[i].firstName,
               seats[i].lastName);
    }
}

/* Clear leftover input */
void clearInputBuffer() {
    while (getchar() != '\n');
}