#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#define READ 0 // for our pipes
#define WRITE 1 // -||-
#define MAX_ARGS_IN_LINE 10
#define MAX_LINES 10


char **get_program_name_and_args(char *command, char *program)
{
    char **args = (char**)calloc(256, sizeof(char*));

    char *arg = strtok(command, " ");
    strcpy(program, arg);

    int ctr = 0;
    args[ctr++] = arg;
    while ((arg = strtok(NULL, " ")) != NULL)
    {
        args[ctr++] = arg;
    }
    args[ctr] = NULL;
    return args;
}

char **slice_commands(char *line)
{
    char **commands = (char **)(calloc(MAX_ARGS_IN_LINE, sizeof(char*)));

    char *cmd = strtok(line, "=");
    int ctr = 0;
    while ((cmd = strtok(NULL, "|")) != NULL)
    {
        commands[ctr++] = cmd;
    }
    return commands;
    
}

int get_index_to_exec(char *line, int i)
{
    //   first line is  "commandsX", therefore we take 8th index
    // next lines are : " commandsY", therefore we take 9th index (it's shifted from strtok func)
    if (i==0) return line[8] - '0';
    else return line[9] - '0';
}

int *get_lines_to_exec(char *line)
{
    static int lines_num[MAX_ARGS_IN_LINE];

    int ctr = 0;
    //line format is: commandsX | commandsY | etc
    //where X is the index of line in commands tab
    //so we have to extract it
    char **commands = (char **)(calloc(MAX_ARGS_IN_LINE, sizeof(char*)));
    char *cmd = strtok(line, "|");

    commands[ctr++] = cmd;
    
    while((cmd = strtok(NULL, "|")) != NULL)
    {
        commands[ctr++] = cmd;
    }
    int i = 0;
    while (commands[i] != NULL)
    {
        printf("\nLINE\n%s", commands[i]);
        lines_num[i] = get_index_to_exec(commands[i], i);
        printf("%d", lines_num[i]);
        i++;
    }
    //mark end of array
    lines_num[i] = -1;
    return lines_num;
}


void main_exec(FILE *fp)
{
    int commands_start_line_num;
    int *lines_to_exec_tab;
    int arg_id;


    char **commands = (char**)calloc(MAX_LINES, sizeof(char*)); //lines are the lines with commands in them
    char **commands_1;
    char **args; //the args for our exec
    char *curr_line;

    int line_id = 0;
    int lines_to_execute_counter = 0;
    int lines_to_execute_counter_1 = 0;

    
    char *line = (char*)calloc(256, sizeof(char));
    size_t len = 0;

    while (getline(&line, &len, fp) != -1)
    {
        printf("\nLINE: %s\n", line);

        if (strstr(line, "="))
        {
            //lines with definitions of commands
            char *line_cpy = (char *) calloc(256, sizeof(char));
            strcpy(line_cpy, line);
            commands[line_id++] = line_cpy;
        }
        else
        {
            //our joined commands
            lines_to_exec_tab = get_lines_to_exec(line); //lines in file (our commands array to exec)
            lines_to_execute_counter = 0; //how many lines we execute
            while(lines_to_exec_tab[lines_to_execute_counter] != -1)
            {
                printf("Line %d\n", lines_to_exec_tab[lines_to_execute_counter]);
                lines_to_execute_counter++;
            }

            int pipe_in[2];
            int pipe_out[2];
            if (pipe(pipe_out) != 0)
            {
                printf("Error creating a pipe!\n");
                exit(-1);
            }
            lines_to_execute_counter_1 = lines_to_execute_counter;
            printf("Lines to execute ctr: %d\n", lines_to_execute_counter_1);
            for (int i = 0; i < lines_to_execute_counter_1; i++)
            {
                curr_line = commands[lines_to_exec_tab[i]];
                printf("\nLine executed: %d %s\n", i, curr_line);

                commands_1 = slice_commands(commands[lines_to_exec_tab[i]]);

                int num_of_cmd_in_line = 0;

                while(commands_1[num_of_cmd_in_line] != NULL)
                {
                    printf("Command%d: %s\n", num_of_cmd_in_line + 1, commands_1[num_of_cmd_in_line]);
                    num_of_cmd_in_line++;
                }
                for (int j = 0; j < num_of_cmd_in_line; j++)
                {
                    pid_t pid = fork();

                    if (pid == 0)
                    {
                        char program[256];
                        args = get_program_name_and_args(commands_1[j], program);

                        printf("----- EXEC ----- \n program: %s\n", program);
                        arg_id = 0;
                        while (args[arg_id] != NULL)
                        {
                            printf("arg%d: %s\n", arg_id + 1, args[arg_id]);
                            arg_id++;
                        }

                        if (execvp(program, args) == -1)
                        {
                            printf("EXEC ERROR\n");
                            exit(-1);
                        }
                        //first line and first command
                        if (j == i == 0) 
                        {
                            close(pipe_out[READ]);
                            dup2(pipe_out[WRITE], STDOUT_FILENO);
                        }
                        //last line and last command
                        else if (j == num_of_cmd_in_line - 1 && i == lines_to_execute_counter - 1)
                        {
                            close(pipe_out[READ]);
                            close(pipe_out[WRITE]);
                            close(pipe_in[WRITE]);
                            dup2(pipe_in[READ], STDIN_FILENO);
                        }
                        else
                        {
                            close(pipe_in[WRITE]);
                            close(pipe_out[READ]);
                            dup2(pipe_in[READ], STDIN_FILENO);
                            dup2(pipe_out[WRITE], STDOUT_FILENO);

                        }
                        
                    }
                    else
                    {
                        close(pipe_in[WRITE]);
                        pipe_in[READ] = pipe_out[READ];
                        pipe_in[WRITE] = pipe_out[WRITE];

                        if (pipe(pipe_out) != 0)
                        {
                            printf("Error while reconnecting pipes! \n");
                            exit(-1);
                        }
                    }
                }
            }
        }
    int status = 0;
    pid_t wpid;
    while((wpid = wait(&status)) != -1);
    printf("\nANAKIN SKYWALKER MODE ON\n");
    }
    
}

int main(int argc, char * argv[])
{
    if (argc != 2)
    {
        printf("Incorrect number of arguments, expected 1!\n");
        exit(-1);
    }
    char *text = argv[1];
    FILE *fp = fopen(text, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    main_exec(fp);
    fclose(fp);
    return 0;
}