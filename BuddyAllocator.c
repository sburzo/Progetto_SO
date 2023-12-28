#include "BuddyAllocator.h"
#include <stdio.h>
#include <assert.h>
#include <math.h>

int getParent(int index) {
    return (int)floor((index - 1)/2);
}

int getSxChild(int index) {
    return (int)(index * 2 + 1);
}

int getDxChild(int index) {
    return (int)(index * 2 + 2);
}

int getBuddy(int index) {
    if (index == 0)
        return -1;
    else if (index % 2 == 0) 
        return index - 1;
    else {
        return index + 1;
    }
}

int getLevel(int index) {
    return floor(log2(index));
}

int getFirstAtLevel(int level) {
    return (int)(1 << level) - 1;
}

int getOffsetFirst(int index) {
    return (int)(index - ((1 << getLevel(index)) - 1));
}

void BuddyAllocatorInit(BuddyAllocator* BuddyAlloc, int minSize, int numLevels, char* mem, uint8_t *buf) {
    BuddyAlloc -> numLevels = numLevels;
    BuddyAlloc -> minSize = minSize;
    BuddyAlloc -> mem = mem;

    //Init bitmap
    int numBits = (1 << numLevels) - 1; //number of bits needed to represent a  binary tree with numLevels
    BitMapInit(&BuddyAlloc->bitmap, numBits, buf);

    for (int i = 0; i < numBits; i++)
        BitMapSetBit(&BuddyAlloc->bitmap, i, 1);

    printf(" buddy  inizialization ok \n");
}

int BuddyAllocatorFreeBlockAtLevel(BuddyAllocator * BuddyAlloc, int level) {
    assert(level < BuddyAlloc->numLevels);

    int startIndex = getFirstAtLevel(level);
    int endIndex = getFirstAtLevel(level + 1);

    for (int i = startIndex; i < endIndex; i++) {

        if (BitMapGetBit(&BuddyAlloc->bitmap, i) == 1) {
            return i;
        }
    }
    return -1; //no free block
}

int BuddyAllocatorFindMinLevel(BuddyAllocator * BuddyAlloc, int size) {
    int sizeCurr = BuddyAlloc->minSize;
    int level = BuddyAlloc->numLevels -1;
    while (sizeCurr < size) {
        level--;
        sizeCurr = sizeCurr * 2;
    }
    return level;
}

int BuddyAllocatorFindFreeBlock(BuddyAllocator * BuddyAlloc, int level) {
    if (level < 0) {
        printf("Buddy not found");
        return -1;
    }
    assert(level <= BuddyAlloc->numLevels);

    int buddyIndex = BuddyAllocatorFreeBlockAtLevel(BuddyAlloc, level);

    if (buddyIndex != -1) {
        printf("buddy find at level %d: \n", level);
        BitMapSetBit(&BuddyAlloc -> bitmap, buddyIndex, 0);
        return buddyIndex;
    }
    int parentIndex = BuddyAllocatorFindFreeBlock(BuddyAlloc, level-1);
    if (parentIndex == -1)
        return -1;

    BitMapSetBit(&BuddyAlloc->bitmap, getDxChild(parentIndex), 1);
    return getSxChild(parentIndex);
}

int * BuddyAllocatorGetAddress(BuddyAllocator * BuddyAlloc, int index, int level) {

    int offset = getOffsetFirst(index);
    int blockSize = (1 << (BuddyAlloc->numLevels - level - 1)) * BuddyAlloc->minSize;

    int pos = offset * blockSize;

    int * addr = (int*)(BuddyAlloc->mem + pos);

    return addr;
}

void * BuddyAllocatorAlloc(BuddyAllocator * BuddyAlloc, int size) {
    int level = BuddyAllocatorFindMinLevel(BuddyAlloc, size+sizeof(int));
    if(level == -1){
        perror("error buddyAllocatorAlloc: no memory avalaible");
    return NULL;
    }

    int blockIndex = BuddyAllocatorFreeBlockAtLevel(BuddyAlloc, level);


    if (blockIndex == -1) {
        perror("error buddyAllocatorAlloc: no memory avalaible");
        return NULL;
    }


    else {

        int * addr = BuddyAllocatorGetAddress(BuddyAlloc, blockIndex, level);

        *addr = blockIndex;

        return (void*)(addr + 1);
    }

}

void BuddyAllocatorFree(BuddyAllocator * BuddyAlloc, void *ptr) {
    if (ptr == NULL) {
        printf("Try to free ptr NULL \n");
        return;
    }

    int *addr = (int*) ptr;
    int blockIndex = *(addr-1);

    if (blockIndex < 0 || blockIndex >= (1 << BuddyAlloc->numLevels) - 1) {
        perror("Try to free invalid block index");
        return;
    }

    BitMapSetBit(&BuddyAlloc->bitmap, blockIndex, 1);


    int level = getLevel(blockIndex);

    while (level < BuddyAlloc->numLevels - 1) {

        int buddyIndex = getBuddy(blockIndex);
        int parentIndex = 0;

        if (buddyIndex == -1 || BitMapGetBit(&BuddyAlloc->bitmap, buddyIndex) == 0) {
            break;
        }
        if (blockIndex < buddyIndex) {
            parentIndex = getParent(blockIndex);
        }
        else {
            parentIndex = getParent(buddyIndex);
        }

        BitMapSetBit(&BuddyAlloc->bitmap, parentIndex, 1);
        BitMapSetBit(&BuddyAlloc->bitmap, blockIndex, 0);
        BitMapSetBit(&BuddyAlloc->bitmap, buddyIndex, 0);

        level++;
    }
    printf("buddy deallocation ok \n");
}