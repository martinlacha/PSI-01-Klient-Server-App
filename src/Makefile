CC=gcc
CFLAGS=
LDFLAGS=

all: client server

client: client.o
	$(CC) $(LDFLAGS) client.o -o client

server: server.o
	$(CC) -pthread $(LDFLAGS) server.o -o server

clean:
	rm -f *.o server client

