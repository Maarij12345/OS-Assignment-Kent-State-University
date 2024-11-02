# OS Assignment - Kent State University

## Project: Producer-Consumer Problem

This project demonstrates a solution to the Producer-Consumer problem using semaphores, threads, and shared memory in C++. The producer generates items and places them on a table, while the consumer picks them up. The table can hold up to two items, and both producer and consumer wait when necessary.

### Setup Instructions

1. **Compile the Programs**:
   ```bash
   gcc producer.cpp -pthread -lrt -o producer
   gcc consumer.cpp -pthread -lrt -o consumer
