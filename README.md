# NetThreads
NetThreads is a UDP-based CLI chat application implemented in C. It uses multithreading and shared memory to enable real-time communication between two users.

## How It Works
Thread 1: Captures keyboard input and adds messages to the shared message queue for transmission.
Thread 2: Sends queued messages to the remote client using UDP sockets.
Thread 3: Listens for incoming UDP messages and queues them for display.
Thread 4: Retrieves messages from the queue and prints them to the screen for the user.

## Technologies
- Language: C
- Concepts:
    - Multithreading (Pthreads)
    - Socket Programming (UDP Protocol)
    - Shared Memory and Synchronization

> This project was developed as part of an Operating Systems class assignment.
