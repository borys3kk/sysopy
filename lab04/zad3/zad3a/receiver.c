#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

int received_signals = 0;
size_t transmitter_pid;

void response(size_t pid, int sig_no)
{
    printf("Receiver received: %d signals, starting to respond\n", received_signals);
    for(int i = 0; i < received_signals; i++)
    {
        if(sig_no == SIGUSR1)
            kill(pid, SIGUSR1);
        else
            kill(pid, SIGRTMIN);        
    }
    if (sig_no == SIGUSR2)
        kill(pid, SIGUSR2);
    else
        kill(pid, SIGRTMIN + 1);

    exit(0);
}

void signal_handler(int sig_no, siginfo_t *info, void *ucontext)
{
    if (sig_no == SIGUSR2 || sig_no == SIGRTMIN + 1)
    {
        transmitter_pid = info->si_pid;
        response(transmitter_pid, sig_no);
        exit(0);
    }
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
}