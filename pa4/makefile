CC=gcc
CFLAGS= -Wall -Werror -std=c99 -pedantic -g

all: client server 

client: 
	$(CC) $(CFLAGS) -D_POSIX_SOURCE -c client.c
	$(CC) $(CFLAGS) -lpthread -o client client.o
	
server:
	$(CC) $(CFLAGS) -D_POSIX_SOURCE -c server.c bank.c
	$(CC) $(CFLAGS) -lpthread -o server server.o bank.o
	
clean:
	rm -rf *.o client server