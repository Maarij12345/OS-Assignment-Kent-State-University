#include "main.hpp"

int main() {
    // Open the shared memory object
    int sharedMemory = shm_open("/shared_mem_buffer", O_RDWR, 0);
    if (sharedMemory == -1) {
        perror("Consumer: Error opening shared memory");
        exit(EXIT_FAILURE);
    }

    // Map shared memory to `SharedBuffer` structure
    struct SharedBuffer *buffer = (struct SharedBuffer *)mmap(NULL, sizeof(*buffer), PROT_READ | PROT_WRITE, MAP_SHARED, sharedMemory, 0);
    if (buffer == MAP_FAILED) {
        perror("Consumer: mmap failed");
        exit(EXIT_FAILURE);
    }

    int position = 0;
    int count = 0;

    // Consume items from the table in a circular manner
    while (count < TOTAL_PRODUCTIONS) {
        sem_wait(&buffer->itemsAvailable);  // Wait if no items are available

        pthread_mutex_lock(&buffer->lock);  // Lock the buffer for exclusive access
        char item = buffer->table[position];
        printf("Consumer picked up item from position %d: %c\n", position, item);
        position = (position + 1) % TABLE_CAPACITY;  // Move to the next position in a circular way
        pthread_mutex_unlock(&buffer->lock);

        sem_post(&buffer->slotsAvailable);  // Signal that a slot has been freed

        count++;
    }

    // Clean up
    munmap(buffer, sizeof(*buffer));
    close(sharedMemory);
    exit(EXIT_SUCCESS);
}
