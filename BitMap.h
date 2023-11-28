#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>



#define BITMAP_SIZE_IN_BYTES 512 * 512


typedef struct {
	uint8_t* buffer;
	int bufferSize;
	int numBits;
} BitMap;


void BitMapInit(BitMap* BitMap, int numBits, uint8_t* buffer);
void setBit(BitMap* BitMap, int i, int stato);
int getBit(BitMap* BitMap, int i);
int getBytes(int bits);
