#pragma once
#include "BuddyAllocator.h"

#define PAGE_SIZE 4096
#define MAX_BUDDY_SIZE 1024*1024
#define B_LEVELS 9
#define MIN_BUDDY_SIZE (MAX_BUDDY_SIZE >> B_LEVELS)

void PseudoMallocInit(BuddyAllocator * BuddyAlloc);
void * PseudoMallocAlloc(BuddyAllocator * BuddyAlloc, int size);
void PseudoMallocFree(BuddyAllocator * BuddyAlloc, void* ptr, int size);