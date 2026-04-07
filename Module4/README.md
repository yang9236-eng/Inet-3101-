# Module 4 - Colossus Airlines Reservation System

## Problem Statement
This program extends the Module 3 airline reservation system by adding file storage. In the previous version, reservation data was lost when the program ended. This version stores reservation data in a file so it can be loaded again when the program restarts.

## Describe the Solution
The program uses an array of structures to store seat info for two flights, outbound and inbound. Each seat keeps should keep track of the seat number, whether it is assigned or not, and the passenger’s first and last name.
To make the data stay, the program saves both flight arrays into a binary file called `reservations.dat` using file I/O functions such as `fopen()`, `fwrite()`, and `fread()`. When the program starts, it will check whether the file exists. If it does, the seat data is loaded from the file. If it isnt, all seats are empty.

The program also keeps the original menu options from Module 3:
- Show number of empty seats
- Show list of empty seats
- Show alphabetical list of seats
- Assign a customer to a seat
- Delete a seat assignment

### Pros
- Reservation data is saved after the program ends
- Uses a simple binary file I/O
- Keeps the original menu/ easy to use
- Supports both outbound and inbound flights

### Cons
- The data file isnt human readable because it is binary
- Input validation is basic
- If the file gets corrupted, the program may not load correctly

## Screenshots
1. Program starting with no saved file
2. Assigning a customer to a seat
3. Showing empty seats
4. Restarting the program and showing that saved reservations are still there







