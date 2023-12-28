#pragma once
#include "BitMap.h"


typedef struct {
    int minSize;
    int numLevels;
    BitMap bitmap;
    char* mem;
} BuddyAllocator;

void BuddyAllocatorInit(BuddyAllocator* BuddyAlloc, int minSize, int numLevels, char* mem, uint8_t *buf);

int BuddyAllocatorFreeBlockAtLevel(BuddyAllocator * BuddyAlloc, int level);

int BuddyAllocatorFindMinLevel(BuddyAllocator * BuddyAlloc, int size);

int BuddyAllocatorFindFreeBlock(BuddyAllocator * BuddyAlloc, int level);

int * BuddyAllocatorGetAddress(BuddyAllocator * BuddyAlloc, int index, int level);

void * BuddyAllocatorAlloc(BuddyAllocator * BuddyAlloc, int size);

void BuddyAllocatorFree(BuddyAllocator * BuddyAlloc, void *ptr);