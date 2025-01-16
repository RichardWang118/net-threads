#ifndef _UDP_INBOUND_H_
#define _UDP_INBOUND_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <pthread.h>
#include "list.h"

static pthread_t tid;
static const char *host;
static char *c_port;
static char *r_port;
static List *list;
static pthread_mutex_t *s_mutex;
static pthread_cond_t *s_cond;
static int socket_desc;

void init_udp_server(const char *hostname, char *client_port, char *remote_port, List *msg_list, pthread_mutex_t *serverMutex, pthread_cond_t *serverCond);
void* run_udp_server();
void shutdown_udp_server();

#endif