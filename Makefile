CC = gcc
EXE = simple
CFLAGS = -Wall #-DDEBUG

all: simple

simple: main.o config.o hash.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o tags $(EXE)
