#ifndef _MSG_PRINT_H_
#define _MSG_PRINT_H_

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

static List *list;
static pthread_t tid;
static pthread_mutex_t *s_mutex;
static pthread_cond_t *s_cond;
static pthread_cond_t *k_cond;

static char name[50];

void init_msg_print(List *msg_list, pthread_mutex_t *serverMutex, pthread_cond_t *serverCond, const char *hostname, pthread_cond_t *killCond);
void* run_msg_print();
void shutdown_msg_print();

#endif