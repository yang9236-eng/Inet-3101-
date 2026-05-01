# Flight Reservation System (Capstone Project)

## Overview
This project is a flight reservation system written in C that allows users to manage seat bookings for outbound and inbound flights.

## Problem Statement 
The goal of this project is to build a complete reservation system that allows users to:
- View all available seats
- Reserve seats for customers
- Update or delete seat reservations
- Generate a report on seat availability
the system must also store data persistently and demonstrate networking capabilities.

## Design and Architecture Details
### Data Structures
- **Seat**
- Seat number
- Assigned status
- First and last name
- **Flight**
- Flight name (outbound/inbound)
- Array of seats (48 total)

### Core Features
- Menu driven CLI interface
- Full CRUD operations:
  - **Create:** Assign a seat
  - **Read:** View/search reservations and reports
  - **Update:** Modify reservation details
  - **Delete:** Remove reservations
- Alphabetical sorting of reservations
- Summary reporting of seat availability

### File Storage 
- Data is saved to a binary file: 'reservations.dat'
- Automatically loads when program starts
- Saves when program exits

### Networking (Client-Server)
- **Server**
  - Runs on port 8080
  - Sends real-time seat availability
- **Client**
  - Connects to server
  - Displays availability report

## Pros and Cons of Your Solution

### Pros
- Clean and modular code structure  
- Implements full CRUD functionality  
- Persistent data using file I/O  
- Includes networking with client-server communication  
- Easy-to-use menu system  

### Cons
- Command-line interface only (no GUI)  
- Fixed number of seats (48 per flight)  
- Limited error handling  
- Server handles only one client at a time  
- Binary file is not human-readable  

## Demo Video Link
- https://mediaspace.umn.edu/media/t/1_pvlftq2h
