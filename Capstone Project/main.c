#include "reservation.h"

// Main program entry point
int main(void) {
    // The system manages two flights: outbound and inbound
    Flight flights[2];

    // Load reservation data from file if it exists
    loadFlights(flights, 2);

    // Start the menu system
    mainMenu(flights, 2);

    return 0;
}
