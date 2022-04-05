#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

int signals_to_send;
int signals_received = -1;
int sended = 0;
int waiting = 0;
char *cmd;
pid_t receiver_pid;

typedef enum{
	KILL,
	SIGQUEUE,
	SIGRT
}sending_mode;


void signal_handler(pid_t sig_no, siginfo_t *info, void *ucontext)
{
    signals_received++;
    if (sended < signals_to_send)
    {
        if (!(strcmp(cmd, "KILL")))
            kill(receiver_pid, SIGUSR1);
        else if (!(strcmp(cmd, "SIGQUEUE")))
            sigqueue(receiver_pid, SIGUSR1, (const union sigval) getpid());
        else if (!strcmp(cmd, "SIGRT"))
            kill(receiver_pid, SIGRTMIN);
        sended++;
    }
    else if(sended == signals_to_send)
    {
        if (!(strcmp(cmd, "KILL")))
            kill(receiver_pid, SIGUSR2);
        else if (!(strcmp(cmd, "SIGQUEUE")))
            sigqueue(receiver_pid, SIGUSR2, (const union sigval) getpid());
        else if (!strcmp(cmd, "SIGRT"))
            kill(receiver_pid, SIGRTMIN+1);
    printf("Received %d out of %d sent.\n", signals_received, signals_to_send);
    exit(0);
    }
}



int main(int argc, char *argv[])
{
    printf("\n|======SENDER======|\n\n");

    if (argc != 4)
    {
        printf("Usage: <catcher_pid> <signal_count> <KILL | SIGQUEUE | SIGRT>\n");
        return 1;
    }

    struct sigaction sa_struct;
    sa_struct.sa_flags = SA_SIGINFO;
    sigemptyset(&sa_struct.sa_mask);
    sa_struct.sa_sigaction = &signal_handler;
    sigaction(SIGUSR1, &sa_struct, NULL);
    sigaction(SIGUSR2, &sa_struct, NULL);
    sigaction(SIGRTMIN, &sa_struct, NULL);
    sigaction(SIGRTMIN + 1, &sa_struct, NULL);


    receiver_pid = atoi(argv[1]);
    signals_to_send = atoi(argv[2]);

    int signals = 0;
    cmd = argv[3];
    if (!strcmp(cmd, "KILL"))
    {
        kill(receiver_pid, SIGUSR1);
        signals++;
    }
    else if (!strcmp(cmd, "SIGQUEUE"))
    {
        sigqueue(receiver_pid, SIGUSR1, (const union sigval) getpid());
        signals++;
    }
    else if (!strcmp(cmd, "SIGRT"))
    {
        kill(receiver_pid, SIGRTMIN);
        signals++;
    }

    while (1)
    {
        pause();
    }
    
}