#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void check_signal_pending()
{
    sigset_t pending_set;
    sigpending(&pending_set);
    printf("PPID: %d [PID] %d PENDING SIGNAL IN CHILD, ", getppid(), getpid());
    if(sigismember(&pending_set, SIGUSR1))
        printf("PART OF PROC.\n");
    else
        printf("NOT PART OF PROC.\n");
}

int main(int argc, char *argv[]) {
    printf("executing child process \n");
    raise(SIGUSR1);
    check_signal_pending();
    printf("ending of child process \n");
    return 0;
}