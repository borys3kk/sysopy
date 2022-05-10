#include <unistd.h>
#include <wait.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

void create_empty_line(char *line)
{
    for (int i = 0; i < 255; i++)
        line[i] = ' ';
    line[255] = '\n';
}

void create_empty_file(char *filename, int rows)
{
    FILE *file = fopen(filename, "w");
    char *line = calloc(256, sizeof(char));
    create_empty_line(line);
    for (int i = 0; i < rows - 1; i++)
    {
        fprintf(file, "%s", line);
    }
    line[255] = '\0';
    fprintf(file, "%s", line);

    fclose(file);
    free(line);
}




void many_to_one_test()
{
    char result[] = "results/many_to_one.txt";
    char cons_N[] = "10";
    char prod_N[] = "5";
    char *input_files[] = 
    {
        "producer_files/W.txt",
        "producer_files/X.txt",
        "producer_files/Y.txt",
        "producer_files/Z.txt",
        "producer_files/2137.txt"
    };
    int producers = 5;
    pid_t pid = fork();
    if (pid == 0)
    {
        execl("./consumer", "./consumer", "pipe", result, cons_N, NULL);
        printf("Somethin went left if this is printed :/\n");
    }

    create_empty_file(result, producers);

    for (int i = 0; i < producers; i++)
    {
        pid = fork();
        char row[2];
        sprintf(row, "%d", i);
        if (pid == 0)
            printf("%d\n", execl("./producer", "./producer", "pipe", row, input_files[i], prod_N, NULL));
    }

    for (int i = 0; i < producers + 1 ; i++){
        wait(NULL);
    }
    printf("\nANAKIN SKYWALKER MODE ON\n(ALL CHILDREN HAS BEEN TERMINATED\n");
}

void one_to_many_test()
{
    char result[] = "results/one_to_many.txt";
    char cons_N[] = "10";
    char prod_N[] = "5";
    char input_file[] = "producer_files/2137.txt";
    int consumers = 5;
    pid_t pid;
    for (int i = 0; i < consumers; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            execl("./consumer", "./consumer", "pipe", result, cons_N, NULL);
            printf("Somethin went left if this is printed :/\n");
        }
    }

    create_empty_file(result, 1);

    char row[2];
    sprintf(row, "%d", 0);
    pid = fork();
    if (pid == 0)
        printf("%d\n", execl("./producer", "./producer", "pipe", row, input_file, prod_N, NULL));
    
    for (int i = 0; i < consumers + 1 ; i++){
        wait(NULL);
    }
    printf("\nANAKIN SKYWALKER MODE ON\n(ALL CHILDREN HAS BEEN TERMINATED\n");
}

void many_to_many_test()
{
    char result[] = "results/many_to_many.txt";
    char cons_N[] = "10";
    char prod_N[] = "5";
    char *input_files[] = 
    {
        "producer_files/W.txt",
        "producer_files/X.txt",
        "producer_files/Y.txt",
        "producer_files/Z.txt",
        "producer_files/2137.txt"
    };
    int producers = 5;
    int consumers = 5;

    pid_t pid;
    for(int i = 0; i < consumers; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            execl("./consumer", "./consumer", "pipe", result, cons_N, NULL);
            printf("Somethin went left if this is printed :/\n");
        }
    }

    create_empty_file(result, producers);

    for (int i = 0; i < producers; i++)
    {
        pid = fork();
        char row[2];
        sprintf(row, "%d", i);
        if (pid == 0)
            printf("%d\n", execl("./producer", "./producer", "pipe", row, input_files[i], prod_N, NULL));

    }
    for (int i = 0; i < producers + consumers; i++){
        wait(NULL);
    }
    printf("\nANAKIN SKYWALKER MODE ON\n(ALL CHILDREN HAS BEEN TERMINATED\n");
}

int main()
{
    // printf("Making fifo\n");
    
    // mkfifo("pipe", 0666);
    // printf("made pipe\n");

    printf("|---------- MANY TO ONE ----------|\n");
    many_to_one_test();
    printf("|---------- ONE TO MANY ----------|\n");
    one_to_many_test();
    printf("|---------- MANY TO MANY ----------|\n");
    many_to_many_test();
    return 0;
}