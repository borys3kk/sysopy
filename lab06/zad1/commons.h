#ifndef COMMON_INCLUDES
#define COMMON_INCLUDES

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <mqueue.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_CLIENTS 5
#define MAX_MSG_LEN 256
#define HOME getenv("HOME")
#define PROJECT 'P'

#define TRUE 1
#define FALSE 0

#define STOP 1
#define LIST 2
#define TOALL 3
#define TOONE 4
#define INIT 5

typedef struct 
{
    long type;
    char content[MAX_MSG_LEN];
    int queue_id;
    int client_id; //sender id
    int server_id; //where do we want to connect
} message;

typedef struct
{
    int id;
    int queue_id;
    int CONNECTED;
} client;

const int SIZE = (sizeof(message) - sizeof(long));

void del_queue(int q_id)
{
    msgctl(q_id, IPC_RMID, NULL);
}

void send_msg(message *msg, int receiver_id)
{
    if (msgsnd(receiver_id, msg, SIZE,0) == -1)
    {
        printf("Error while sending msg :/ \n");
        exit(1);
    }
}

void receive_msg(int queue_id, message *msg, long type)
{
    if (msgrcv(queue_id, msg, SIZE, type, 0) == -1)
    {
        printf("Error while receiving msg :/ \n");
        exit(1);
    }
}

#endif