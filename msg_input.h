#ifndef _MSG_INPUT_H__
#define _MSG_INPUT_H__

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

static List *list;
static pthread_t tid; 
static pthread_mutex_t *c_mutex;
static pthread_cond_t *c_cond;

void init_msg_input(List *client_msg, pthread_mutex_t *clientMutex, pthread_cond_t *clientCond);
void* run_msg_input();
void shutdown_msg_input();

#endif