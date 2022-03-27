#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        perror("Expected 1 argument!");
        exit(1);
    }
    int n = atoi(argv[1]);

    if (n < 1)
    {
        perror("None child processes to be made");
        exit(2);
    }
    pid_t pid;
    pid = fork();
    n-=1;
    while (pid != 0 && n>0)
    {
        pid = fork();
        n-=1;
    }

    if (pid==0)
    {
        printf("Child process No: %d\n", getpid());
        return 0; //to make sure the child process has ended
    }
    return 0;
}