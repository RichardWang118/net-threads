#include <stdio.h>
#include "udp_inbound.h"
#include "udp_outbound.h"
#include "msg_print.h"
#include "msg_input.h"
#include "list.h"

List *client_msg;
List *server_msg;

int main(int argc, char *argv[])
{
    pthread_cond_t clientCond, serverCond, killCond;
    pthread_mutex_t clientMutex, serverMutex, killMutex;
    if (argc != 4)
    {
        printf("Usage: %s <local_port> <hostname> <remote_port>\n", argv[0]);
        return 1;
    }
    char *local_port = argv[1];
    const char *hostname = argv[2];
    char *remote_port = argv[3];
    if (atoi(local_port) <= 1024 || atoi(remote_port) <= 1024)
    {
        printf("Invalid port numbers.\n");
        return 1;
    }

    client_msg = List_create();
    server_msg = List_create();

    pthread_mutex_init(&clientMutex, NULL);
    pthread_mutex_init(&serverMutex, NULL);
    pthread_mutex_init(&killMutex, NULL);

    pthread_cond_init(&clientCond, NULL);
    pthread_cond_init(&serverCond, NULL);
    pthread_cond_init(&killCond, NULL);

    init_msg_print(server_msg, &serverMutex, &serverCond, hostname, &killCond);
    init_msg_input(client_msg, &clientMutex, &clientCond);

    init_udp_server(hostname, local_port, remote_port, server_msg, &serverMutex, &serverCond);
    init_udp_client(hostname, remote_port, client_msg,  &clientMutex, &clientCond,&killCond);

    
    pthread_cond_wait(&killCond, &killMutex);
    shutdown_msg_input();
    shutdown_udp_client();
    shutdown_udp_server();
    shutdown_msg_print();
    pthread_mutex_destroy(&serverMutex);
    pthread_mutex_destroy(&clientMutex);
    pthread_mutex_destroy(&killMutex);

    pthread_cond_destroy(&clientCond);
    pthread_cond_destroy(&serverCond);
    pthread_cond_destroy(&killCond);
    return 0;
}