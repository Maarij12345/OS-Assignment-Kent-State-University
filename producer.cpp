#include "main.hpp"

int main() {
    // Create shared memory object
    int sharedMemory = shm_open("/shared_mem_buffer", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (sharedMemory == -1) {
        std::cerr << "Producer: Error creating shared memory" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Configure the size of the shared memory object and map it to `SharedBuffer`
    ftruncate(sharedMemory, sizeof(SharedBuffer));
    auto *buffer = static_cast<SharedBuffer *>(mmap(nullptr, sizeof(*buffer), PROT_READ | PROT_WRITE, MAP_SHARED, sharedMemory, 0));
    if (buffer == MAP_FAILED) {
        std::cerr << "Producer: mmap failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Initialize the mutex and semaphores
    pthread_mutex_init(&buffer->lock, nullptr);
    sem_init(&buffer->slotsAvailable, 1, TABLE_CAPACITY); // Initially, all slots are available
    sem_init(&buffer->itemsAvailable, 1, 0);              // Initially, no items are available

    // Produce items and place them on the table in a circular manner
    char item = 'A';
    int position = 0;
    int count = 0;

    while (count < TOTAL_PRODUCTIONS) {
        sem_wait(&buffer->slotsAvailable);  // Waits if no empty slot is available

        pthread_mutex_lock(&buffer->lock);  // Lock the buffer for exclusive access
        buffer->table[position] = item;
        std::cout << "Producer placed item in position " << position << ": " << item << std::endl;
        position = (position + 1) % TABLE_CAPACITY;  // Move to the next position in a circular way
        pthread_mutex_unlock(&buffer->lock);

        sem_post(&buffer->itemsAvailable);  // Signal that a new item is available

        item = (item == 'Z') ? 'A' : item + 1;  // Cycle through A to Z
        count++;
    }

    // Clean up
    munmap(buffer, sizeof(*buffer));
    close(sharedMemory);
    shm_unlink("/shared_mem_buffer");
    return 0;
}

