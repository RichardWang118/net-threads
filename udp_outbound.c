#include "udp_outbound.h"

void init_udp_client(const char *hostname, char *remote_port, List *msg_list, pthread_mutex_t *clientMutex, pthread_cond_t *clientCond, pthread_cond_t *killCond)
{
    c_mutex = clientMutex;
    c_cond = clientCond;
    k_cond = killCond;
    host = hostname;
    r_port = remote_port;
    list = msg_list;

    pthread_create(&o_tid, NULL, run_udp_client, NULL);
}

void* run_udp_client()
{

    // Get server address info using getaddrinfo
    int send_result;
    int socket_desc;
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    if (getaddrinfo(host, r_port, &hints, &res) != 0)
    {
        fprintf(stderr, "Error while getting server address info\n");
        exit(EXIT_FAILURE);
    }

    //  Create socket:
    socket_desc = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if (socket_desc < 0)
    {
        printf("Error while creating socket\n");
        exit(EXIT_FAILURE);
    }
    printf("Client Socket created successfully\n");

    while (1)
    {
        pthread_mutex_lock(c_mutex);
        while (List_count(list) == 0)
            pthread_cond_wait(c_cond, c_mutex);

        char *client_message = ((char *)List_trim(list));
        pthread_mutex_unlock(c_mutex);
        send_result = sendto(socket_desc, client_message, strlen(client_message), 0,
                            res->ai_addr, res->ai_addrlen);
         if (send_result < 0)
            printf("Unable to send message. Error: %d\n", send_result);

        if (strcmp(client_message, "!\n") == 0)
        {
            printf("------ SHUTTING DOWN STALK ------\n");
                pthread_cond_signal(k_cond);
        }
    }

    freeaddrinfo(res);

    // Close the socket:
    close(socket_desc);
    return NULL;
}



void shutdown_udp_client()
{
    pthread_cancel(o_tid);
    pthread_join(o_tid, NULL);
}