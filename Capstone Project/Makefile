CC = gcc
CFLAGS = -Wall -Wextra -std=c11

all: app server client

app: main.c reservation.c reservation.h
	$(CC) $(CFLAGS) main.c reservation.c -o app

server: server.c reservation.c reservation.h
	$(CC) $(CFLAGS) server.c reservation.c -o server

client: client.c
	$(CC) $(CFLAGS) client.c -o client

clean:
	rm -f app server client reservations.dat
