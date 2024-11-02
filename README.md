# OS Assignment - Kent State University

## Project: Producer-Consumer Problem

### Overview

This project is an implementation of the classic Producer-Consumer problem in C++ using semaphores, threads, and shared memory in a Linux environment. The producer creates items and places them onto a shared table (represented by a fixed-size buffer with a capacity of two items). The consumer picks up items from this table. Both the producer and consumer must synchronize their actions to avoid race conditions and ensure that the table never overflows or becomes empty at the wrong time.

The problem is solved by:
- Limiting the table to hold only two items at any time.
- Using semaphores to synchronize access between the producer and consumer.
- Implementing mutual exclusion to prevent simultaneous access to the shared memory buffer.

### Structure

The project is organized into three main files:
- **main.hpp**: This header file includes necessary libraries, defines constants, and declares the shared memory structure (`SharedBuffer`). It contains the mutex lock and semaphores for synchronization, along with a character buffer array representing the table.
- **producer.cpp**: The producer program creates items and adds them to the table. When the table is full, the producer waits until a slot is available.
- **consumer.cpp**: The consumer program picks up items from the table. When the table is empty, the consumer waits until there is an item to consume.

The producer and consumer programs interact through shared memory and are compiled and executed as separate processes. The project utilizes the POSIX `pthread` and `rt` libraries to handle threading and semaphore operations.

### Dependencies

This code requires:
- A Linux environment (or a Linux emulator like VirtualBox if using a non-Linux system).
- `pthread` and `rt` libraries for threading and real-time semaphore management.
- `gcc` or `g++` compiler for compiling C++ files.

Ensure that the required libraries are available in your environment to compile and execute the code. 

### Setup Instructions

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/Maarij12345/OS-Assignment-Kent-State-University.git
   cd OS-Assignment-Kent-State-University
