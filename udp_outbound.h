#ifndef _UDP_OUTBOUND_H_
#define _UDP_OUTBOUND_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <netdb.h>
#include <pthread.h>
#include "list.h"

static char *r_port;
static const char *host;
static List *list;
static pthread_mutex_t *c_mutex;
static pthread_cond_t *c_cond;
static pthread_cond_t *k_cond;
static pthread_t o_tid;

void init_udp_client(const char *hostname, char *remote_port, List *msg_list, pthread_mutex_t *clientMutex, pthread_cond_t *clientCond, pthread_cond_t *killCond);
void* run_udp_client();
void shutdown_udp_client();

#endif