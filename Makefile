CC = gcc
EXE = mail simple
CFLAGS = -Wall -DDEBUG

all: mail simple

mail: mail.o config.o
	$(CC) -o $@ $^

simple: simple.o config.o
	$(CC) -o $@ $^

mail.o: mail.c config.h
simple.o: simple.c config.h
config.o: config.c config.h uthash.h

clean:
	rm -f $(EXE) mail.o simple.o config.o
