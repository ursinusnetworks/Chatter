CC=gcc
CFLAGS=-g -Wall -pedantic

all: chatter simpleserver simpleclient test hashmaptest

hashmap.o: hashmap.c hashmap.h
	gcc -c hashmap.c

chatter: chatter.c
	gcc -g -Wall -pedantic -o chatter chatter.c -lncurses

simpleclient: simpleclient.c
	$(CC) $(CFLAGS) -o simpleclient simpleclient.c

simpleserver: simpleserver.c
	$(CC) $(CFLAGS) -o simpleserver simpleserver.c

test: test.c
	$(CC) $(CFLAGS) -o test test.c

hashmaptest: hashmaptest.c hashmap.o
	gcc -g -o hashmaptest hashmaptest.c hashmap.o

clean:
	rm chatter simpleserver simpleclient test hashmaptest hashmap.o