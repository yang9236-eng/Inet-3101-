#ifndef RESERVATION_H
#define RESERVATION_H

// Maximum number of seats per flight
#define MAX_SEATS 48

// Maximum length for first names, last names, and flight names
#define MAX_NAME 50

// File used to save and load reservation data
#define DATA_FILE "reservations.dat"

// This struct represents one seat on a flight
typedef struct {
    int seatNumber;              // Seat number from 1 to 48
    int assigned;                // 0 means empty, 1 means reserved
    char firstName[MAX_NAME];    // Passenger first name
    char lastName[MAX_NAME];     // Passenger last name
} Seat;

// This struct represents one flight with a list of seats
typedef struct {
    char flightName[MAX_NAME];   // Example: Outbound Flight or Inbound Flight
    Seat seats[MAX_SEATS];       // Array of all seats on this flight
} Flight;

// Setup and file I/O functions
void initializeFlights(Flight flights[], int flightCount);
void loadFlights(Flight flights[], int flightCount);
void saveFlights(Flight flights[], int flightCount);

// Menu functions
void mainMenu(Flight flights[], int flightCount);
void flightMenu(Flight *flight);

// Reservation system functions
void showEmptySeatCount(const Flight *flight);
void showEmptySeats(const Flight *flight);
void showAlphabeticalSeats(const Flight *flight);
void assignSeat(Flight *flight);
void updateReservation(Flight *flight);
void deleteReservation(Flight *flight);
void searchReservation(const Flight *flight);
void showReport(const Flight flights[], int flightCount);

// Networking helper used by the server
char *getAvailabilityReport(const Flight flights[], int flightCount, char *buffer, int bufferSize);

#endif
