#include "udp_inbound.h"

void init_udp_server(const char *hostname, char *client_port, char *remote_port, List *msg_list, pthread_mutex_t *serverMutex, pthread_cond_t *serverCond)
{   

    s_mutex = serverMutex;
    s_cond = serverCond;
    host = hostname;
    c_port = client_port;
    r_port = remote_port;
    list = msg_list;

    pthread_create(&tid, NULL, run_udp_server, NULL); 
}
void* run_udp_server()
{
    struct sockaddr_in client_addr;
    int client_length = sizeof(struct sockaddr_in);

    // Get server address info using getaddrinfo
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, c_port, &hints, &res) != 0)
    {
        fprintf(stderr, "Error while getting server address info\n");
        exit(EXIT_FAILURE);
    }

    // Create UDP socket:
    socket_desc = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if (socket_desc < 0)
    {
        printf("Error while creating socket\n");
        exit(EXIT_FAILURE);
    }
    printf("Server Socket created successfully\n");

    // Bind to the set port and IP:
    if (bind(socket_desc, res->ai_addr, res->ai_addrlen) < 0)
    {
        printf("Couldn't bind to the port\n");
        exit(EXIT_FAILURE);
    }
    freeaddrinfo(res);

    while (1)
    {
        // buffer size was chosen from a vision (randomly chosen)
        char buffer[500];
        // clean buffer memory
        memset(buffer, 0, sizeof(buffer));

        if (recvfrom(socket_desc, buffer, sizeof(buffer), 0,
                     (struct sockaddr *)&client_addr, &client_length) < 0)
        {
            printf("Couldn't receive\n");
            exit(EXIT_FAILURE);
        }

        // This single line is the result of 7-8 hours of night coding:
        // the buffer is preset at an array of 500 characters
        //
        // realistically no one is going to type that much unless if its a
        // hyper long breakup text of how "its not you, its me"
        //
        // anyways
        //
        // if the buffer receives a character array smaller than 500 characters
        // its no longer a valid c-string because while it has the exact same
        // characters as the received char array, because programming is very fun,
        // the size of the buffer array remains 500
        //
        // and causes the buffer to be not printed due to the rest of the buffer array
        // being filled with the null terminator and RANDOM CHARACTERS
        //
        // so as a fix to this, string duplication is used which so conveniently
        // dynamically allocates space for the duplicated character array (server_message)
        // and at least from a lot of testing, seems to fix the random character filling issue
        char *server_message = strdup(buffer);

        pthread_mutex_lock(s_mutex);
        {
            List_prepend(list, server_message);
            pthread_cond_signal(s_cond);
        }
        pthread_mutex_unlock(s_mutex);
    }
    // Close the socket
    shutdown_udp_server();
    return NULL;
}

void shutdown_udp_server()
{
    close(socket_desc);
    // cancel thread
    pthread_cancel(tid);
    // Waits for thread to finish
    pthread_join(tid, NULL); // makes sure that the thread finishes before program finishes
}