CC = gcc
EXE = simple
CFLAGS = -Wall -DDEBUG
LDFLAGS = -lm

all: simple

simple: main.o config.o hash.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o tags $(EXE)
