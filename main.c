#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "BuddyAllocator.h"


#define BUDDY_LEVELS 5
#define MEMORY_SIZE (1 << 20)
#define MIN_BUCKET_SIZE (MEMORY_SIZE >> (BUDDY_LEVELS-1))
#define PAGE_SIZE 4096
#define BITMAP_BUFFER_SIZE (1 << (BUDDY_LEVELS+1))
#define BUDDY_THRESHOLD (PAGE_SIZE / 4)
#define BITMAP_SIZE_IN_BYTES 32


char memory[MEMORY_SIZE];
uint8_t bitmapBuffer[BITMAP_BUFFER_SIZE];

void BitMapTest()
{
	int numBits = BITMAP_SIZE_IN_BYTES * 8;
    	uint8_t* buffer = (uint8_t*) malloc(BITMAP_SIZE_IN_BYTES);
    	if (buffer == NULL) {
    		printf("Allocation memory failed (malloc)\n");
    		return;
    	}
        BitMap* bitMap = malloc(sizeof(BitMap));
        BitMapInit(bitMap, numBits, buffer);
        BitMapPrint(bitMap);
    	BitMapSetBit(bitMap, 1, 1);
        int bitValue = BitMapGetBit(bitMap, 1);
        printf("Bit value at index 1: %d\n", bitValue);
        bitValue = BitMapGetBit(bitMap, 2);
        printf("Bit value at index 2: %d\n", bitValue);
        printf("BitMapTest finish!\n");
        BitMapPrint(bitMap);
        BitMapDestroy(bitMap);
    	return;
}

void BuddyAllocatorTest()
{
	BuddyAllocator *BuddyAlloc = malloc(sizeof(BuddyAllocator));
    BuddyAllocatorInit(BuddyAlloc, MIN_BUCKET_SIZE,  BUDDY_LEVELS, memory, bitmapBuffer);
	int *ptr = (int *)BuddyAllocatorAlloc(BuddyAlloc, sizeof(int));
	if (ptr == NULL)
		printf("Failed to allocate memory for an integer\n");
	else
	{
		*ptr = 123456798;
		printf("Allocated integer: %d\n", *ptr);
	}

	printf("\n\n");

	int *ptr2 = (int *)BuddyAllocatorAlloc(BuddyAlloc, sizeof(int));
	if (ptr2 == NULL)
		printf("Failed to allocate memory for an integer\n");
	else
	{
		*ptr2 = 123456798;
		printf("Allocated integer: %d\n", *ptr2);
	}

	printf("\n\n");
	int x = *ptr + *ptr2;
	printf("%d sum", x);

	printf("\n\n");


	int *ptr3 = (int *)BuddyAllocatorAlloc(BuddyAlloc, sizeof(int));
	if (ptr3 == NULL)
		printf("Failed to allocate memory for an integer\n");
	else
	{
		*ptr3 = 123456798;
		printf("Allocated integer: %d\n", *ptr3);
	}

	printf("\n\n");
	x = x + *ptr3;
	printf("%d sum", x);

	printf("\n\n");

	printf("\n\n");
	printf("%d\t", *ptr);
	printf("\n\n");
	printf("%d\t", *ptr2);
	printf("\n\n");
	printf("%d\t", *ptr3);
	printf("\n\n");

	int bitValue1 = BitMapGetBit(&BuddyAlloc->bitmap, 0);
	printf("Bit value at index 0: %d\n", bitValue1);
	int bitValue2 = BitMapGetBit(&BuddyAlloc->bitmap, 9);
	printf("Bit value at index 9: %d\n", bitValue2);
	int bitValue3 = BitMapGetBit(&BuddyAlloc->bitmap, 18);
	printf("Bit value at index 18: %d\n", bitValue3);

	BuddyAllocatorFree(BuddyAlloc, ptr);
	BuddyAllocatorFree(BuddyAlloc, ptr2);
	BuddyAllocatorFree(BuddyAlloc, ptr3);
	free(BuddyAlloc);
	return;
}

int main()
{
	BitMapTest();
	//BuddyAllocatorTest();
	return 0;

}