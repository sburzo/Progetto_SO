#include "PseudoMalloc.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/mman.h>



char mem[MAX_BUDDY_SIZE];
uint8_t buf[((1 << B_LEVELS)) - 1];

void PseudoMallocInit(BuddyAllocator * BuddyAlloc) {
     BuddyAllocatorInit(BuddyAlloc, MIN_BUDDY_SIZE, B_LEVELS, mem, buf);
}

void * PseudoMallocAlloc(BuddyAllocator * BuddyAlloc, int size) {

    //assert(size > 0);

    size += sizeof(int); //includere lo spazio per l'header dell'allocazione

    if (size <= PAGE_SIZE/4) {
        void * ptr_s = BuddyAllocatorAlloc(BuddyAlloc, size);
        if (ptr_s == NULL) {
            printf("Fallita BuddyAllocatorAlloc");
            return NULL;
        }
        return ptr_s;
    }
    else if (size > PAGE_SIZE/4){
        void * ptr_b = mmap(0, size, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (ptr_b == MAP_FAILED) {
            perror("Errore mmap allocazione \n");
            return NULL;
        }
        return ptr_b;
    }
}

void PseudoMallocFree(BuddyAllocator * BuddyAlloc, void* ptr, int size) {

    if (size < 1 || ptr == NULL) {
        printf("Errore: size non valida o puntatore nullo");
        return;
    }

    size += sizeof(int);
    int i;

    if (size <= PAGE_SIZE/4) {
        BuddyAllocatorFree(BuddyAlloc, ptr);
    } else if (size > PAGE_SIZE/4) {
        i = munmap(ptr, size);
        if (i == -1) {
            perror("Fallita munmap");
            return;
        }
    }
}