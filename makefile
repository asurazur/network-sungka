CC = gcc
CFLAGS = -Wall 
RM = rm -f

all: server client

client: client.c
	$(CC) $(CFLAGS) client.c -o client

server: server.c
	$(CC) $(CFLAGS) server.c -o server

sungka: sungka.c
	$(CC) $(CFLAGS) sungka.c -o sungka
	
clean:
	$(RM) server && $(RM) client
