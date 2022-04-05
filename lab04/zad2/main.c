#include <stdio.h>
#include <signal.h>
#include <string.h>

void siginfo_handler(int sig_no, siginfo_t *sig_info, void *ucontext)
{
    printf("Received [SA_SIGINFO] signal: %d\n", sig_no);
    printf("[PID]: %d\n", sig_info->si_pid);
}

void restart_handler(int sig_no)
{
    printf("Received [SA_RESTART] signal: %d \n", sig_no);
}

void onstack_handler(int sig_no)
{
    printf("Received [SA_ONSTACK] signal: %d \n", sig_no);
}

void resethand_handler(int sig_no)
{
    printf("SA_RESETHAND_HANDLER\n");
}

int main(int argc, char * argv[])
{
    printf("SA_SIGINFO FLAG TEST: \n");
    struct sigaction sig;
    sigemptyset(&sig.sa_mask);
    sig.sa_flags = SA_SIGINFO;
    sig.sa_sigaction = siginfo_handler;
    sigaction(SIGUSR1, &sig, NULL);
    raise(SIGUSR1);

    printf("SA_RESTART FLAG TEST: \n");
    struct sigaction sig1;
    sigemptyset(&sig1.sa_mask);
    sig1.sa_flags = SA_RESTART;
    sig1.sa_handler = restart_handler;
    sigaction(SIGUSR1, &sig1, NULL);
    raise(SIGUSR1);

    printf("SA_ONSTACK FLAG TEST\n");
    struct sigaction sig2;
    sigemptyset(&sig2.sa_mask);
    sig2.sa_flags = SA_ONSTACK;
    sig2.sa_handler = onstack_handler;
    sigaction(SIGUSR1, &sig2, NULL);
    raise(SIGUSR1);

    struct sigaction sig3;
    sigemptyset(&sig3.sa_mask);
    sig3.sa_flags = SA_RESETHAND;
    sig3.sa_handler = resethand_handler;
    sigaction(SIGUSR1, &sig3, NULL);   
    raise(SIGUSR1);

    raise(SIGUSR1);
    printf("testunio"); //click noice
    return 0;
}