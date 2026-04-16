# Module 5

## Problem Statement
This code implements a client server application in C using sockets. The server accepts the file name from thje client and send the file contents back as a response. The file contents must be text only and cannot exceed 255 characters. The program can also handle errors such as invalid file name, missing files and files that are too large. Also can shut down gracefully!

## describe the solution 
The solution uses TCP sockets for communication between the client and server.
- The server creates a socket, binds to port 9001, listens for incoming clients and accepts filename request from the client.
- The server checks whether the filename is valid by rejecting unsafe names such as ones containing `..`, `/`, or `\`.
- If the file name is invalid, the server sends back an error message.
- If the file is larger than 255 characters, the server sends back an error saying the file is too large.
- If the file is valid and within the size limit, the server reads the contents and sends them to the client.
- The **client** connects to the server, asks the user for a filename, sends the filename to the server, receives the response, and prints it.
- The server also uses signal handling with `SIGINT` so it can shut down cleanly when the user presses `Ctrl + C`.

## Pros and cons of my solution

### pros
- sim-ple and easy to understand
- USES TCP which is reliable 
- Handles common errors nicely
- prevents unsafe file path request
- includes graceful shutdown

### cons
- only allows text files up to 255 characters
- can only handkle one request at a time
- doesnt support subdirectories or complext file paths
- mostly for local testing

## screenshots 
- shutdown screenshot has three. It gracefully shuts down, shows how it handles wrong/ invalid file name and if the file isnt found.
- scramble screenshot shows what happens when the file is too large/ has too many characters.
- The sample screenshot shows what happens when it runs correctly showing off what was in the text file. 
