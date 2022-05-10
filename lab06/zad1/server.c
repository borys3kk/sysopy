#include "commons.h"

client clients[MAX_CLIENTS];
int queue_id;

void server_stop()
{
    printf("Shutting server down\n");

    message msg;
    int q_client;
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i].CONNECTED)
        {
            q_client = clients[i].queue_id;
            msg.type = STOP;
            send_msg(&msg, q_client);

            receive_msg(queue_id, &msg, STOP);
            printf("Client stopped");
        }
    }
    del_queue(queue_id);
    printf("\nSERVER HAS BEEN SHUT DOWN\n");
}

void handle_INIT(message *msg)
{
    int slot = -1;
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i].CONNECTED == 0)
        {
            slot = i;
            clients[i].CONNECTED = 1;
            clients[i].id = i;
            clients[i].queue_id = msg->queue_id;
            break;
        }
    }
    if (slot == -1)
    {
        printf("NO MORE SLOTS\n");
        exit(1);
    }

    printf("SERVER: Client with id: %d has been initiated\n", slot);
    message response = {.type = INIT, .client_id = slot};

    send_msg(&response, clients[slot].queue_id);
}

void handle_STOP(message *msg)
{
    clients[msg->client_id].CONNECTED = 0;
    clients[msg->client_id].queue_id = -1;
}

void handle_TOONE(message *msg)
{
    if (clients[msg->server_id].CONNECTED)
    {
        send_msg(msg, clients[msg->server_id].queue_id);
    }
    printf("Massage sent\n");
}

void handle_TOALL(message *msg)
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i].CONNECTED && i != msg->client_id)
        {
            send_msg(msg, clients[i].queue_id);
        }
    }
    printf("Massages sent\n");
}

void handle_LIST()
{
    printf("CLients: \n");
    for(int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i].CONNECTED)
        {
            printf("Client ID: %d\n", i);
        }
    }
}

void close_server_queue()
{
    msgctl(queue_id, IPC_RMID, NULL);
}

void choose_mode(message *msg)
{
    switch (msg->type)
    {
    case INIT:
        handle_INIT(msg);
        break;
    case LIST:
        handle_LIST();
        break;
    case TOALL:
        handle_TOALL(msg);
        break;
    case TOONE:
        handle_TOONE(msg);
        break;
    case STOP:
        handle_STOP(msg);
        break;
    default:
        printf("Wrong msg type :/\n");
        break;
    }
}

int main(int argc, char **argv)
{
    
    for (int i = 0; i < MAX_CLIENTS; i++) //when starting a new server no clients can be connected
    {
        clients[i].CONNECTED = 0;
    }

    key_t queue_key;

    if ((queue_key = ftok(HOME, PROJECT)) == -1)
    {
        perror("ftok\n");
        exit(1);
    }
    if ((queue_id = msgget(queue_key, IPC_CREAT | 0666)) == -1)
    {
        perror("msget xd");
        exit(1);
    }
    atexit(close_server_queue);
    signal(SIGINT, server_stop);
    printf("QUUEUEUSJBN %d\n",queue_id);
    message msg;
    printf("here\n");
    while (TRUE)
    {
        if (msgrcv(queue_id, &msg, sizeof(message),0,0) == -1)
        {
            perror("msgrcv error\n");
            exit(1);
        }
        printf("message received\n");
        fflush(stdout);
        choose_mode(&msg);
    }
}