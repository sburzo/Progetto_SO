CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lm -lc

EXECUTABLE = test

all: $(EXECUTABLE)

$(EXECUTABLE): BuddyAllocator.o BitMap.o PseudoMalloc.o main.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f BuddyAllocator.o BitMap.o PseudoMalloc.o main.o $(EXECUTABLE)