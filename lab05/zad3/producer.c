#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        printf("Invalid num of args :/\n");
        exit(-1);
    }
    char *pp = argv[1];
    int row = atoi(argv[2]);
    char *fp = argv[3];
    int N = atoi(argv[4]);

    printf("Row_num: %d\n", row);
    FILE *pipe = fopen(pp, "w");
    FILE *file = fopen(fp, "r+");
        printf("Im here\n,%d", getpid());
    

    char buffer[N];
    char msg[N+5];
    printf("\nRead\n");
    while (fread(buffer, sizeof(char), N, file) == N)
    {
        buffer[strlen(buffer) - 1] = '\n';
        snprintf(msg, sizeof(msg), "%d.%s", row, buffer);
        printf("PRODUCER pp: %s\n", buffer);

        fwrite(msg, sizeof(char), N + 5, pipe);
        sleep(1);
    }
    printf("\nafter read\n");
    fclose(pipe);
    fclose(file);
    return 0;
}