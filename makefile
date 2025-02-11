CC=gcc
CFLAGS=-g -Wall -pedantic

all: chatter simpleserver simpleclient test hashmaptest

arraylist.o: arraylist.c arraylist.h
	gcc -c arraylist.c

linkedlist.o: linkedlist.c linkedlist.h
	gcc -c linkedlist.c

hashmap.o: hashmap.c hashmap.h
	gcc -c hashmap.c

gui.o: gui.c chatter.h
	gcc -c gui.c

chatter: chatter.c chatter.h gui.o arraylist.o linkedlist.o hashmap.o
	gcc $(CFLAGS) -o chatter chatter.c gui.o arraylist.o -lncurses -lpthread

simpleclient: simpleclient.c
	$(CC) $(CFLAGS) -o simpleclient simpleclient.c

simpleserver: simpleserver.c
	$(CC) $(CFLAGS) -o simpleserver simpleserver.c -lpthread

test: test.c
	$(CC) $(CFLAGS) -o test test.c

hashmaptest: hashmaptest.c hashmap.o
	gcc -g -o hashmaptest hashmaptest.c hashmap.o

clean:
	rm *.o chatter simpleserver simpleclient test hashmaptest 