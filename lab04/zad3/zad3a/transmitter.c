#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
int signals_to_send;
int signals_received = 0;
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
    if (sig_no == SIGUSR2 || sig_no == SIGRTMIN + 1)
    {
        waiting = 1;
        printf("Finished waiting for signals, Received %d out of %d sent.\n", signals_received, signals_to_send);
        exit(0);
    }
    signals_received++;
    printf("Received signal with if: %d\n", info->si_value.sival_int);
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
        while(signals < signals_to_send)
        {
            kill(receiver_pid, SIGUSR1);
            signals++;
        }
        kill(receiver_pid, SIGUSR2);
    }
    else if (!strcmp(cmd, "SIGQUEUE"))
    {
        while (signals < signals_to_send)
        {
            sigqueue(receiver_pid, SIGUSR1, (const union sigval) getpid());
            signals++;
        }
        sigqueue(receiver_pid, SIGUSR2, (const union sigval) getpid());
    }
    else if (!strcmp(cmd, "SIGRT"))
    {
        while(signals < signals_to_send)
        {
            kill(receiver_pid, SIGRTMIN);
            signals++;
        }
        kill(receiver_pid, SIGRTMIN + 1);
    }

    while (1)
    {
        pause();
    }
    
}