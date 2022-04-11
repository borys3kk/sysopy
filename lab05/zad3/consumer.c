#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>

void save_to_file(FILE *file, char *msg, int row)
{
    rewind(file); // to go to beginning
    int fd = fileno(file);
    char buffer[257];
    int file_line = 0;

    flock(fd, LOCK_EX); //locking the file so only we have access to it

    printf("Writing to file in line %d\n", row);



    int i;
    while (fgets(buffer, 257, file) != NULL)
    {
        if (file_line == row)
        {
            for (i = 0; i < 256; i++)
            {
                if (buffer[i] == ' ') break;
            }

            fseek(file, row * 256 + i, SEEK_SET);
            printf("Write mess: %s\n\n", msg);
            fprintf(file, msg, strlen(msg) + 1);
            fflush(file);
            break;
        }
        file_line++;
    }
    flock(fd, LOCK_UN);
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Invalind num of args\n");
        exit(-1);
    }
    char *pp = argv[1];
    char *fp = argv[2];
    int N = atoi(argv[3]);

    FILE *pipe = fopen(pp, "r");
    FILE *file = fopen(fp, "r+");
    if (pipe < 0 || file < 0)
    {
        printf("An error with either file or pipe occured!\n");
        exit(-1);
    }

    char buffer[N];

    while (fread(buffer, sizeof(char), N, pipe) == N)
    {
        printf("Consumer read from pp: %s\n", buffer);

        char *seq = strtok(buffer, ".");
        int num = atoi(seq);

        seq = strtok(NULL, "\n");

        char *line;
        sprintf(line, "%s", seq);

        save_to_file(file, line, num);
    }

    fclose(file);
    fclose(pipe);
    return 0;
}