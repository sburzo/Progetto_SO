#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "BitMap.h"




void BitMapInit(BitMap* bitMap, int numBits, uint8_t* buffer){
  bitMap->buffer=buffer;
  bitMap->numBits=numBits;
  bitMap->bufferSize=getBytes(numBits);
  
}


void BitMapSetBit(BitMap* bitmap, int index, int state) {
	if (index >= bitmap->numBits || state < 0 || state > 1 || index < 0) {
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


int BitMapGetBit(BitMap* BitMap, int i) {
	if (BitMap == NULL || BitMap->numBits <= i) 
	    return 0;
	int byteIndex = i / 8;
	int bitOffset = i % 8;
	return (BitMap -> buffer[byteIndex] >> bitOffset) & 1;
}


int getBytes(int numBits) {
	return numBits / 8 + (numBits % 8) != 0;
}

void BitMapPrint(BitMap* BitMap) {
	if (!BitMap || !BitMap->buffer) {
		printf("BitMap not valid or not initialized\n");
		return;
	}
	printf("Bitmap : ");
	for (int i = 0; i < BitMap->numBits; i++) {
		if (i % 8 == 0 && i != 0) {
			printf(" ");
		}
		printf("%d", BitMapGetBit(BitMap, i));
	}
	printf("\n");
}

void BitMapDestroy(BitMap* BitMap) {
	if (BitMap == NULL) return;
	free(BitMap);
}

