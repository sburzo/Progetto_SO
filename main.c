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
    		printf("Allocazione memoria fallita (malloc)\n");
    		return;
    	}
        BitMap* bitMap = malloc(sizeof(BitMap));
        BitMapInit(bitMap, numBits, buffer);
    	BitMapSetBit(bitMap, 1, 1);
    	BitMapPrint(bitMap);
        int bitValue = BitMapGetBit(bitMap, 1);
        printf("Valore del bit all'indirizzo 1: %d\n", bitValue);
        bitValue = BitMapGetBit(bitMap, 2);
        printf("Valore del bit all'indirizzo 2: %d\n", bitValue);
        printf("BitMapTest finish!\n");
        BitMapDestroy(bitMap);
    	return;
}

void BuddyAllocatorTest()
{
	BuddyAllocator *BuddyAlloc = malloc(sizeof(BuddyAllocator));
    BuddyAllocatorInit(BuddyAlloc, MIN_BUCKET_SIZE,  BUDDY_LEVELS, memory, bitmapBuffer);
	int *ptr = (int *)BuddyAllocatorAlloc(BuddyAlloc, sizeof(int));
	if (ptr == NULL)
		printf("Fallita allocazione memoria intero\n");
	else
	{
		*ptr = 123456798;
		printf("Intero allocato: %d\n", *ptr);
	}

	printf("\n\n");

	int *ptr2 = (int *)BuddyAllocatorAlloc(BuddyAlloc, sizeof(int));
	if (ptr2 == NULL)
		printf("Fallita allocazione memoria intero\n");
	else
	{
		*ptr2 = 123456798;
		printf("Intero allocato: %d\n", *ptr2);
	}

	printf("\n\n");
	int x = *ptr + *ptr2;
	printf("%d somma", x);

	printf("\n\n");


	int *ptr3 = (int *)BuddyAllocatorAlloc(BuddyAlloc, sizeof(int));
	if (ptr3 == NULL)
		printf("Fallita allocazione memoria intero\n");
	else
	{
		*ptr3 = 123456798;
		printf("Intero allocato: %d\n", *ptr3);
	}

	printf("\n\n");
	x = x + *ptr3;
	printf("%d somma", x);

	printf("\n\n");

	printf("\n\n");
	printf("%d\t", *ptr);
	printf("\n\n");
	printf("%d\t", *ptr2);
	printf("\n\n");
	printf("%d\t", *ptr3);
	printf("\n\n");

	int bitValue1 = BitMapGetBit(&BuddyAlloc->bitmap, 0);
	printf("Valore del bit all'indirizzo 0: %d\n", bitValue1);
	int bitValue2 = BitMapGetBit(&BuddyAlloc->bitmap, 9);
	printf("Valore del bit all'indirizzo 9: %d\n", bitValue2);
	int bitValue3 = BitMapGetBit(&BuddyAlloc->bitmap, 18);
	printf("Valore del bit all'indirizzo 18: %d\n", bitValue3);

	BuddyAllocatorFree(BuddyAlloc, ptr);
	BuddyAllocatorFree(BuddyAlloc, ptr2);
	BuddyAllocatorFree(BuddyAlloc, ptr3);
	free(BuddyAlloc);
	return;
}

void PseudoMallocTest() {

    BuddyAllocator BuddyAlloc;
    PseudoMallocInit(&BuddyAlloc);

    printf("TEST 1: allocazione 512+4 bytes\n");
    void* ptr1 = (char*) PseudoMallocAlloc(&BuddyAlloc, 512); //512+4 minore di 4096/4 bytes
    if(ptr1!=NULL){
        printf("Riuscita allocazione blocco da 512 all'indirizzo %p\n\n", ptr1);
    }
    printf("TEST 2: allocazione 1020+4 bytes\n");
    void* ptr2 = (char*) PseudoMallocAlloc(&BuddyAlloc, 1020); //1020+4 uguale a 4096/4 bytes
    if(ptr2!=NULL){
        printf("Riuscita allocazione blocco da 1020 all'indirizzo %p\n\n", ptr2);
    }
    printf("TEST 3: allocazione 1024+4 bytes\n");
    void* ptr3 = (char*) PseudoMallocAlloc(&BuddyAlloc, 1024); //1024+4 maggiore a 4096/4 bytes
    if(ptr3!=NULL){
        printf("Riuscita allocazione blocco da 1024 all'indirizzo %p\n\n", ptr3);
    }
    printf("TEST 4: allocazione 2048+4 bytes\n");
    char* ptr4 = (char*) PseudoMallocAlloc(&BuddyAlloc, 2048); //2048+4 maggiore di 4096/4 bytes
    if(ptr4!=NULL){
        printf("Riuscita allocazione blocco da 2048 all'indirizzo %p\n\n", ptr4);
    }
    printf("TEST 5: allocazione 4096+4 bytes\n");
    void* ptr5 = (char*) PseudoMallocAlloc(&BuddyAlloc, 4096);
    if(ptr5!=NULL){
        printf("Riuscita allocazione blocco da 4096 all'indirizzo %p\n\n", ptr5);
    }

    printf("TEST 6: allocazione 0+4 bytes\n");
    void* ptr6 = (char*) PseudoMallocAlloc(&BuddyAlloc, 0);
    if(ptr6!=NULL){
        printf("Riuscita allocazione blocco da 0 all'indirizzo %p\n", ptr6);
    }

    printf("TEST 7: allocazione -1+4 bytes\n");
    void* ptr7 = (char*) PseudoMallocAlloc(&BuddyAlloc, -1);
    if(ptr7!=NULL){
        printf("Riuscita allocazione blocco da -1 all'indirizzo %p\n", ptr7);
    }

    PseudoMallocFree(&BuddyAlloc, ptr1, 512);
    printf("deallocazione blocco da 512 ok \n\n");
    PseudoMallocFree(&BuddyAlloc, ptr2, 1020);
    printf("deallocazione blocco da 1020 ok \n\n");
    PseudoMallocFree(&BuddyAlloc, ptr3, 1024);
    printf("deallocazione blocco da 1024 ok \n\n");
    PseudoMallocFree(&BuddyAlloc, ptr4, 2048);
    printf("deallocazione blocco da 2048 ok \n\n");
    PseudoMallocFree(&BuddyAlloc, ptr5, 4096);
    printf("deallocazione blocco da 4096 ok \n\n");
    PseudoMallocFree(&BuddyAlloc, ptr6, 0);
    printf("deallocazione blocco da 0 ok \n\n");
    PseudoMallocFree(&BuddyAlloc, ptr7, -1);
    printf("deallocazione blocco da -1 ok \n\n");

    printf("Memoria liberata\n");


}

int main()
{
	//BitMapTest();
	//BuddyAllocatorTest();
	PseudoMallocTest();
	return 0;

}