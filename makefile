CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lm -lc

# Nome dell'eseguibile
EXECUTABLE = test

all: $(EXECUTABLE)

$(EXECUTABLE): BitMap.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)


BitMap.o: BitMap.h BitMap.c
	$(CC) $(CFLAGS) -c -o $@ BitMap.c



clean:
	rm -f BitMap.o $(EXECUTABLE)