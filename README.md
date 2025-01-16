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
 
## Usage Instructions
1. Run the program on two different terminals. Each terminal should act as a chat client.
  
   Use the following format to start the program:
    ```./talk <local_port> <hostname> <remote_port>```
    - local_port: Port for this client to listen for messages.
    - hostname: IP address or hostname of the remote client (use 127.0.0.1 for local testing).
    - remote_port: Port of the remote client to send messages to.

7. Example Usage:
    - Terminal 1:
     ```./talk 5000 127.0.0.1 6000```
    - Terminal 2:
    ```./talk 6000 127.0.0.1 5000```

8. Communicate:
Type messages into one terminal and press Enter to send them.
Received messages will appear in the opposite terminal.

> This project was developed as part of an Operating Systems class assignment.
