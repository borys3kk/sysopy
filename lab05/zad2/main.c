#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void print_sorted_email(char *sort_by)
{
    FILE *file;
    char *cmd;
    if (!(strcmp(sort_by, "date")))
        cmd = "echo | mail -f | head -n -1 | tac";
    else if (!(strcmp(sort_by, "sender")))
        cmd = "echo | mail -f | head -n -1 | sort -k 3";
    else
    {
        printf("WRNG MODE!\n Choose date or sender\n");
        exit(-1);
    }
    file = popen(cmd, "r");
    if (file==NULL)
    {
        printf("popen erro\n");
        exit(-1);
    }
    printf("Sorted mails: \n");
    char *line;
    while(fgets(line, 256, file) != NULL)
        printf("%s\n", line);
}

void send_mail(char *address, char *subject, char *content)
{
    FILE *file;
    char command[256];
    snprintf(command, sizeof(command), "echo %s | mail -s %s %s", content, subject, address);
    printf("%s\n", command);
    file = popen(command, "r");

    if (file == NULL)
    {
        printf("popen erro\n");
        exit(-1);
    }

    printf("\n ----- email theoretically sent -----\n");
    printf("TO: %s\nSUBJECT: %s\nCONTENT: %s\n", address, subject, content);

}

int main(int argc, char *argv[])
{
    if (argc != 2 && argc != 4)
    {
        printf("Wrong number of parameters :/\n");
        exit(-1);
    }
    if (argc == 2)
    {
        printf("Print emails sorted by [%s] \n", argv[1]);
        print_sorted_email(argv[1]);
    }
    else
    {
        printf("Sending email\n");
        send_mail(argv[1], argv[2], argv[3]);
    }
}