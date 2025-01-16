#include "msg_input.h"

void init_msg_input(List *msg_list, pthread_mutex_t *clientMutex, pthread_cond_t *clientCond)
{   
    list = msg_list;
    c_mutex = clientMutex;
    c_cond = clientCond;

    pthread_create(&tid, NULL, run_msg_input, NULL);
}

void* run_msg_input()
{
    // Needed to wipe the character in case of nothing sent
    while (1)
    {
        char client_message[65506];
        fgets(client_message, sizeof(client_message), stdin);

        pthread_mutex_lock(c_mutex);
        List_prepend(list, client_message);
        pthread_cond_signal(c_cond);
        pthread_mutex_unlock(c_mutex);
    }
    return NULL;
}

void shutdown_msg_input(){
    pthread_cancel(tid);
    pthread_join(tid, NULL);
}
