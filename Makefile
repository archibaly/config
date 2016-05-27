CC = gcc
EXE = simple
CFLAGS = -Wall -DDEBUG

all: simple

simple: simple.o config.o
	$(CC) -o $@ $^

simple.o: simple.c config.h
config.o: config.c config.h uthash.h

clean:
	rm -f $(EXE) mail.o simple.o config.o
