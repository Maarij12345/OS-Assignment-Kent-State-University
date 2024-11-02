#include "main.hpp"

int main() {
    // Create shared memory object
    int sharedMemory = shm_open("/shared_mem_buffer", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (sharedMemory == -1) {
        perror("Producer: Error creating shared memory");
        exit(EXIT_FAILURE);
    }

  
    ftruncate(sharedMemory, sizeof(struct SharedBuffer));
    struct SharedBuffer *buffer = (struct SharedBuffer *)mmap(NULL, sizeof(*buffer), PROT_READ | PROT_WRITE, MAP_SHARED, sharedMemory, 0);
    if (buffer == MAP_FAILED) {
        perror("Producer: mmap failed");
        exit(EXIT_FAILURE);
    }

    // Initialize the mutex and semaphores
    pthread_mutex_init(&buffer->lock, NULL);
    sem_init(&buffer->slotsAvailable, 1, TABLE_CAPACITY); // Initially, all slots are available
    sem_init(&buffer->itemsAvailable, 1, 0);              // Initially, no items are available

    // Produce items and place them on the table in a circular manner
    char item = 'A';
    int position = 0;
    int count = 0;

    while (count < TOTAL_PRODUCTIONS) {
        sem_wait(&buffer->slotsAvailable);  // Waits if no empty slot is available

        pthread_mutex_lock(&buffer->lock);  // Lock the bufer for exclusive access
        buffer->table[position] = item;
        printf("Producer placed item in position %d: %c\n", position, item);
        position = (position + 1) % TABLE_CAPACITY;  // Move  the next position in a circular way
        pthread_mutex_unlock(&buffer->lock);

        sem_post(&buffer->itemsAvailable);  // Signal that a new item is available

        item = (item == 'Z') ? 'A' : item + 1;  // Cycle through A to Z
        count++;
    }

    // Cleans up
    munmap(buffer, sizeof(*buffer));
    close(sharedMemory);
    shm_unlink("/shared_mem_buffer");
    exit(EXIT_SUCCESS);
}
