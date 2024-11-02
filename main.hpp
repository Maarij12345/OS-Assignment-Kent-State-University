#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdlib>

#define TABLE_CAPACITY 2    // Capacity for the shared "table" buffer
#define TOTAL_PRODUCTIONS 10 // Total items to produce and consume

// Structure to represent shared memory with mutex and semaphores
struct SharedBuffer {
    pthread_mutex_t lock;
    sem_t slotsAvailable, itemsAvailable;
    char table[TABLE_CAPACITY];
};

