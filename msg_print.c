#include "msg_print.h"

void init_msg_print(List *msg_list, pthread_mutex_t *serverMutex, pthread_cond_t *serverCond, const char *hostname, pthread_cond_t *killCond)
{
    k_cond = killCond;

    
    list = msg_list;    
    s_mutex = serverMutex;
    s_cond = serverCond;
    sprintf(name, "%s", hostname);
    pthread_create(&tid, NULL, run_msg_print, NULL);
}

void* run_msg_print()
{

    while (1)
    {
        // server message character variable
        char *server_message;

        // allow only a single thread to trim and print list element
        pthread_mutex_lock(s_mutex);
        {
            // wait for signal from udp_inbound.c
            // signal condition: after list_prepend
            pthread_cond_wait(s_cond, s_mutex);

            while (List_count(list) > 0)
            {
                // trim list element and store in server message character variable
                server_message = List_trim(list);

                if(strcmp(server_message, "!\n") == 0){
                    printf("------ SHUTTING DOWN STALK ------\n");
                    pthread_cond_signal(k_cond);
                }
                else
                {
                   // print message in the form of "HOSTNAME: SERVER_MESSAGE"
                    printf("%s: %s", name, server_message); 
                }
                
            }

            // free message after printing
            free(server_message);

        }
        // allow next thread to access
        pthread_mutex_unlock(s_mutex);
    }
    return NULL;
}

void shutdown_msg_print()
{
    pthread_cancel(tid);
    pthread_join(tid, NULL);
}
