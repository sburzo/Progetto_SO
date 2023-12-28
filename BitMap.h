#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


typedef struct {
	uint8_t* buffer;
	int bufferSize;
	int numBits;
} BitMap;


void BitMapInit(BitMap* BitMap, int numBits, uint8_t* buffer);
void BitMapSetBit(BitMap* BitMap, int i, int stato);
int BitMapGetBit(BitMap* BitMap, int i);
int getBytes(int bits);
