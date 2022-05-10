#include "commons.h"

int server_queue;
int client_queue;
int cur_id;

void send_INIT()
{
    printf("Client initiated \n");
    message msg = {.queue_id = client_queue, .type = INIT};
    send_msg(&msg, server_queue);

    message response;
    receive_msg(client_queue, &response, INIT);
    cur_id = response.client_id;
}

void send_LIST()
{
    printf("Client wants to see everyone\n");
    message msg = {.type = LIST};
    send_msg(&msg, server_queue);
}

void send_STOP()
{
    printf("Client want to STOP\n");
    message msg = {.client_id = cur_id, .type = STOP};
    send_msg(&msg, server_queue);
    del_queue(client_queue);
    exit(0);
}

void send_TOONE(char *content, int to_id)
{
    printf("2ONE\n");
    message msg = {.client_id = cur_id, .type = TOONE, .server_id = to_id};
    strcpy(msg.content, content);

    send_msg(&msg, server_queue);
}

void send_TOALL(char *content)
{
    printf("2ALL\n");
    message msg = {.client_id = cur_id, .type = TOALL};
    strcpy(msg.content, content);

    send_msg(&msg, server_queue);
}

void handle_message(message *msg)
{
    printf("Message from: %d\n", msg->client_id);
    printf("%s\n", msg->content);
}

void handle_STOP()
{
    printf("Server is shutting down\n");
    send_STOP();
}

int is_empty(int q_id)
{
    struct msqid_ds buf;
    msgctl(q_id, IPC_STAT, &buf);

    if (buf.msg_qnum != 0) return 0;
    return 1;
}

void handler()
{
    while (!is_empty(client_queue))
    {
        message msg;
        if (msgrcv(client_queue, &msg, SIZE, -6, IPC_NOWAIT) == -1)
        {
            printf("NOWAIT ERR\n");
            exit(1);
        }
        switch (msg.type)
        {
        case STOP:
            handle_STOP();
            break;
        case TOONE:
            handle_message(&msg);
            break;
        case TOALL:
            handle_message(&msg);
            break;
        default:
            break;
        }
    }   
}

int check_input(){
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
    return (FD_ISSET(0, &fds));
}

int main()
{
    signal(SIGINT, send_STOP);

    key_t client_key, queue_key;

    if ((client_key = ftok(HOME, getpid())) == -1)
    {
        perror("ftok\n");
        exit(1);
    }
    if ((client_queue = msgget(client_key, IPC_CREAT | IPC_EXCL | 0666)) == -1)
    {
        perror("Client queue\n");
        exit(2);
    }
    if ((queue_key = ftok(HOME, PROJECT)) == -1)
    {
        perror("ftok1\n");
        exit(3);
    }
    if ((server_queue = msgget(queue_key, IPC_CREAT | 0666)) == -1)
    {
        perror("msgget\n");
        exit(4);
    }
    printf("client_id: %d\n", client_queue);

    send_INIT();

    char cmd[MAX_MSG_LEN];
    while (TRUE)
    {
        if (check_input())
        {
            fgets(cmd, MAX_MSG_LEN, stdin);
            if (strcmp("LIST\n", cmd) == 0)
                send_LIST();
            else if (strcmp("STOP\n", cmd) == 0)
                send_STOP();
            else if (strcmp("2ONE\n", cmd) == 0)
            {
                printf("Receiver ID: ");
                fgets(cmd, MAX_MSG_LEN, stdin);
                int receiver_id = atoi(cmd);
                printf("Enter content of the message: ");
                fgets(cmd, MAX_MSG_LEN, stdin);
                send_TOONE(cmd, receiver_id);
            }
            else if (strcmp("2ALL\n", cmd) == 0)
            {
                printf("Enter content of the message: ");
                fgets(cmd, MAX_MSG_LEN, stdin);
                send_TOALL(cmd);
            }
            else
                printf("Unknwn cmd :/\n");
        }
        sleep(1);
        handler();
    }
    
}