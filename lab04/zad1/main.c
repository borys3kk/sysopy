#include "stdio.h"
#include "stdlib.h"
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <errno.h>

enum sig_mode
{
    IGNORE,
    HANDLER,
    MASK,
    PENDING
};

void handle_signal(int _id)
{
    printf("Signal: %d was captured and handled!\n", _id);
}

void check_signal_pending()
{
    sigset_t pending_set;
    sigpending(&pending_set);
    printf("PPID: %d [PID] %d PENDING SIGNAL IN MAIN, ", getppid(), getpid());
    if(sigismember(&pending_set, SIGUSR1))
        printf("PART OF PROC.\n");
    else
        printf("NOT PART OF PROC.\n");
}

enum sig_mode return_mode(char *h_mode)
{
if (!strcmp(h_mode, "ignore"))
    {
        return IGNORE;
    }
    else if (!strcmp(h_mode, "handler"))
    {
        return HANDLER;
    }
    else if (!strcmp(h_mode, "mask"))
    {
        return MASK;
    }
    else if (!strcmp(h_mode, "pending"))
    {
        return PENDING;
    }
    else
    {
        printf("Your argument is invalid :/\n");
        return -1;
    }
}

int main(int argc, char *argv[])
{
    enum sig_mode mode;
    if (argc < 2)
    {
        printf("Supply an option: ignore|handler|mask|pending");
        return 1;
    }
    mode = return_mode(argv[1]);
    if (mode == -1)
    {
        return 1;
    }
    

    printf("Starting main proc.\n");
    switch (mode)
        {
            case IGNORE:
                signal(SIGUSR1, SIG_IGN);
                break;
            case HANDLER:
                signal(SIGUSR1, handle_signal);
                break;
            default:
            {
                sigset_t block_set;
                sigemptyset(&block_set);
                sigaddset(&block_set, SIGUSR1);
                sigprocmask(SIG_BLOCK, &block_set, NULL);
            }
            break;
        }


    raise(SIGUSR1); //signal send
    check_signal_pending();

    pid_t child_pid;

    if ((child_pid = fork())==0)
    {
        printf("Child starts runnin\n");
        if (mode == PENDING)
            check_signal_pending();
            
        else
                raise(SIGUSR1);

        if (mode != HANDLER)
        {
            execl("./child", "child", NULL);
            return 0;
        }
    printf("Stop runnin child\n");
    exit(1);
    }

    wait(NULL);
    printf("Main proc end\n");
    return 0;

}