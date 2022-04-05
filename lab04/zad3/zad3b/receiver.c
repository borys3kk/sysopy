#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

int received_signals = -1;


void signal_handler(int sig_no, siginfo_t *info, void *ucontext)
{
    int transmiter_pid = info->si_pid;

    if (sig_no == SIGUSR2 || sig_no == SIGRTMIN + 1)
    {
        printf("Receiver received %d signals\n", received_signals);
        exit(0);
    }
    kill(transmiter_pid, SIGUSR1);
    received_signals++;

};


int main(int argc, int *argv[])
{
    printf("CATCHER PID: %d\n", getpid());

    struct sigaction sa_struct;
    sa_struct.sa_flags = SA_SIGINFO;
    sigemptyset(&sa_struct.sa_mask);
    sa_struct.sa_sigaction = &signal_handler;
    sigaction(SIGUSR1, &sa_struct, NULL);
    sigaction(SIGUSR2, &sa_struct, NULL);
    sigaction(SIGRTMIN, &sa_struct, NULL);
    sigaction(SIGRTMIN + 1, &sa_struct, NULL);

    while (1)
        pause();
    exit(0);
}