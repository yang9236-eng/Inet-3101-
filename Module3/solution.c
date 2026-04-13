#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SEATS 48
#define NAME_LEN 30

typedef struct {
    int seatNumber;
    int assigned;
    char firstName[NAME_LEN];
    char lastName[NAME_LEN];
} Seat;

/* Function prototypes */
void initializeSeats(Seat seats[]);
void firstLevelMenu(Seat outbound[], Seat inbound[]);
void secondLevelMenu(Seat seats[], const char flightName[]);

int countEmptySeats(Seat seats[]);
void showNumberOfEmptySeats(Seat seats[]);
void showListOfEmptySeats(Seat seats[]);
void showAlphabeticalListOfSeats(Seat seats[]);
void assignSeat(Seat seats[]);
void deleteSeatAssignment(Seat seats[]);
void clearInputBuffer(void);

int main(void) {
    Seat outbound[SEATS];
    Seat inbound[SEATS];

    initializeSeats(outbound);
    initializeSeats(inbound);

    firstLevelMenu(outbound, inbound);

    printf("Program ended.\n");
    return 0;
}

/* Initialize all seats */
void initializeSeats(Seat seats[]) {
    int i;
    for (i = 0; i < SEATS; i++) {
        seats[i].seatNumber = i + 1;
        seats[i].assigned = 0;
        strcpy(seats[i].firstName, "");
        strcpy(seats[i].lastName, "");
    }
}

/* First level menu */
void firstLevelMenu(Seat outbound[], Seat inbound[]) {
    char choice;

    do {
        printf("\n========== FIRST LEVEL MENU ==========\n");
        printf("a. Outbound Flight\n");
        printf("b. Inbound Flight\n");
        printf("c. Quit\n");
        printf("Enter your choice: ");
        scanf(" %c", &choice);
        choice = tolower(choice);

        switch (choice) {
            case 'a':
                secondLevelMenu(outbound, "Outbound Flight");
                break;
            case 'b':
                secondLevelMenu(inbound, "Inbound Flight");
                break;
            case 'c':
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 'c');
}

/* Second level menu */
void secondLevelMenu(Seat seats[], const char flightName[]) {
    char choice;

    do {
        printf("\n====== %s MENU ======\n", flightName);
        printf("a. Show number of empty seats\n");
        printf("b. Show list of empty seats\n");
        printf("c. Show alphabetical list of seats\n");
        printf("d. Assign a customer to a seat assignment\n");
        printf("e. Delete a seat assignment\n");
        printf("f. Return to Main menu\n");
        printf("Enter your choice: ");
        scanf(" %c", &choice);
        choice = tolower(choice);

        switch (choice) {
            case 'a':
                showNumberOfEmptySeats(seats);
                break;
            case 'b':
                showListOfEmptySeats(seats);
                break;
            case 'c':
                showAlphabeticalListOfSeats(seats);
                break;
            case 'd':
                assignSeat(seats);
                break;
            case 'e':
                deleteSeatAssignment(seats);
                break;
            case 'f':
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 'f');
}

/* Count empty seats */
int countEmptySeats(Seat seats[]) {
    int i, count = 0;
    for (i = 0; i < SEATS; i++) {
        if (!seats[i].assigned) {
            count++;
        }
    }
    return count;
}

/* Show number of empty seats */
void showNumberOfEmptySeats(Seat seats[]) {
    printf("Number of empty seats: %d\n", countEmptySeats(seats));
}

/* Show list of empty seats */
void showListOfEmptySeats(Seat seats[]) {
    int i;

    printf("Empty seats: ");
    for (i = 0; i < SEATS; i++) {
        if (!seats[i].assigned) {
            printf("%d ", seats[i].seatNumber);
        }
    }
    printf("\n");
}

/* Show seats alphabetically by last name, then first name */
void showAlphabeticalListOfSeats(Seat seats[]) {
    Seat assignedSeats[SEATS];
    Seat temp;
    int i, j, count = 0;

    for (i = 0; i < SEATS; i++) {
        if (seats[i].assigned) {
            assignedSeats[count++] = seats[i];
        }
    }

    if (count == 0) {
        printf("No seats are currently assigned.\n");
        return;
    }

    for (i = 0; i < count - 1; i++) {
        for (j = i + 1; j < count; j++) {
            int lastCmp = strcmp(assignedSeats[i].lastName, assignedSeats[j].lastName);
            int firstCmp = strcmp(assignedSeats[i].firstName, assignedSeats[j].firstName);

            if (lastCmp > 0 || (lastCmp == 0 && firstCmp > 0)) {
                temp = assignedSeats[i];
                assignedSeats[i] = assignedSeats[j];
                assignedSeats[j] = temp;
            }
        }
    }

    printf("\nAlphabetical list of assigned seats:\n");
    printf("%-10s %-20s %-20s\n", "Seat", "Last Name", "First Name");
    for (i = 0; i < count; i++) {
        printf("%-10d %-20s %-20s\n",
               assignedSeats[i].seatNumber,
               assignedSeats[i].lastName,
               assignedSeats[i].firstName);
    }
}

/* Assign a customer to a seat */
void assignSeat(Seat seats[]) {
    int seatNum;
    char first[NAME_LEN];
    char last[NAME_LEN];

    if (countEmptySeats(seats) == 0) {
        printf("No empty seats available.\n");
        return;
    }

    printf("Enter seat number to assign (1-48, 0 to abort): ");
    if (scanf("%d", &seatNum) != 1) {
        clearInputBuffer();
        printf("Invalid input.\n");
        return;
    }

    if (seatNum == 0) {
        printf("Assignment aborted.\n");
        return;
    }

    if (seatNum < 1 || seatNum > SEATS) {
        printf("Invalid seat number.\n");
        return;
    }

    if (seats[seatNum - 1].assigned) {
        printf("Seat %d is already assigned.\n", seatNum);
        return;
    }

    printf("Enter first name (or type abort to cancel): ");
    scanf("%29s", first);
    if (strcmp(first, "abort") == 0 || strcmp(first, "ABORT") == 0) {
        printf("Assignment aborted.\n");
        return;
    }

    printf("Enter last name (or type abort to cancel): ");
    scanf("%29s", last);
    if (strcmp(last, "abort") == 0 || strcmp(last, "ABORT") == 0) {
        printf("Assignment aborted.\n");
        return;
    }

    seats[seatNum - 1].assigned = 1;
    strcpy(seats[seatNum - 1].firstName, first);
    strcpy(seats[seatNum - 1].lastName, last);

    printf("Seat %d assigned to %s %s.\n", seatNum, first, last);
}

/* Delete a seat assignment */
void deleteSeatAssignment(Seat seats[]) {
    int seatNum;

    printf("Enter seat number to delete assignment (1-48, 0 to abort): ");
    if (scanf("%d", &seatNum) != 1) {
        clearInputBuffer();
        printf("Invalid input.\n");
        return;
    }

    if (seatNum == 0) {
        printf("Deletion aborted.\n");
        return;
    }

    if (seatNum < 1 || seatNum > SEATS) {
        printf("Invalid seat number.\n");
        return;
    }

    if (!seats[seatNum - 1].assigned) {
        printf("Seat %d is already empty.\n", seatNum);
        return;
    }

    seats[seatNum - 1].assigned = 0;
    strcpy(seats[seatNum - 1].firstName, "");
    strcpy(seats[seatNum - 1].lastName, "");

    printf("Assignment for seat %d deleted.\n", seatNum);
}

/* Clear bad input from buffer */
void clearInputBuffer(void) {
    while (getchar() != '\n')
        ;
}