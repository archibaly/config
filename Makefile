CC = gcc
EXE = mail simple
CFLAGS = -Wall

all: mail simple

mail: mail.o hash.o config.o
	$(CC) -o $@ $^

simple: simple.o hash.o config.o
	$(CC) -o $@ $^

mail.o: mail.c config.h
simple.o: simple.c config.h
hash.o: hash.c hash.h
config.o: config.c config.h hash.h

clean:
	rm -f $(EXE) mail.o simple.o hash.o config.o
