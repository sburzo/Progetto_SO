#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "BitMap.h"



// Initialize bitmap
void BitMapInit(BitMap* bitMap, int numBits, uint8_t* buffer){
  bitMap->buffer=buffer;
  bitMap->numBits=numBits;
  bitMap->bufferSize=getBytes(numBits);
  
}

// set the byte in the bitmap
void setBit(BitMap* bitmap, int index, int state) {
	if (index < 0 || index >= bitmap->numBits) {
		printf("Index out of range \n");
		return;
	}
	int byte_num = index / 8;
	int bit_in_byte = index % 8;
	if (state) {
		bitmap->buffer[byte_num] |= (1<<bit_in_byte);
	} else {
		bitmap->buffer[byte_num] &= ~(1<<bit_in_byte);
	}
}

//return bit in position i
int getBit(BitMap* BitMap, int i) {
	if (BitMap == NULL || BitMap->numBits <= i) 
	    return 0;
	int byteIndex = i / 8;
	int bitOffset = i % 8;
	return (BitMap -> buffer[byteIndex] >> bitOffset) & 1;
}


int getBytes(int numBits) {
	return numBits / 8 + (numBits % 8) != 0;
}

int main() {
	int numBits = BITMAP_SIZE_IN_BYTES * 8;
	uint8_t* buffer = (uint8_t*) malloc(BITMAP_SIZE_IN_BYTES);
	if (buffer == NULL) {
		printf("Allocation memory failed (malloc)\n");
		return 1;
	}
    BitMap* bitMap = malloc(sizeof(BitMap));
    BitMapInit(bitMap, numBits, buffer);
	setBit(bitMap, 1, 1);
    int bitValue = getBit(bitMap, 1);
    printf("Bit value at index 1: %d\n", bitValue);
    bitValue = getBit(bitMap, 2);
    printf("Bit value at index 2: %d\n", bitValue);
	return 0;
}