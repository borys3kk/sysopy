#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "unistd.h"
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/wait.h>

int search_file(char *path, char *word_to_find)
{   
    FILE *searched_file = fopen(path, "r+");

    if (searched_file == NULL)
    {
        return -1;
    }
    char * line = NULL;
    size_t _len = 0;
    ssize_t read;
    while (read = getline(&line, &_len, searched_file) != -1)
    {
        if (strstr(line, word_to_find) != NULL)
            printf("PID: %d, path: %s \n", getpid(), path);
    }
    fclose(searched_file);
    return 0;
}

void search_dir(char *dir_name, char *word_to_find, int depth, int cur_depth)
{
    DIR *cur_dir = opendir(dir_name);
    if (cur_dir == NULL)
    {
        exit(1);
    }
    char *path = calloc(sizeof(char), 100);
    int num_of_dir = 0;
    struct dirent *entity;
    int file_type;
    //first loop is to find number of child processes and to check first directory for files with given word
    while ((entity = readdir(cur_dir)) != NULL)
    {
        if (strcmp(entity->d_name, ".")!= 0 && strcmp(entity->d_name, "..") != 0)
        {
            file_type = entity->d_type;
            if (file_type == DT_DIR)
                num_of_dir++;
            if (file_type == DT_REG)
            {
                sprintf(path,"%s/%s", dir_name, entity->d_name);
                search_file(path, word_to_find);
            }
        }
    }
    free(path);
    closedir(cur_dir);
    if (cur_depth >= depth)
        exit(1);
    // pid_t *child_ids = calloc(sizeof(pid_t), num_of_dir);
    //in second loop we create child processes to check subdirectories as given 


    cur_dir = opendir(dir_name);
    if (cur_dir == NULL)
    {
        exit(1);
    }

    pid_t child = 0;

    while ((entity = readdir(cur_dir)) != NULL)
    {
        if (strcmp(entity->d_name, ".")!= 0 && strcmp(entity->d_name, "..") != 0 && entity->d_type == DT_DIR)
        {
            child = fork();
            if (child != 0)
            {
                char *path_to_dir = calloc(sizeof(char), strlen(entity->d_name + strlen(dir_name) + 2));
                sprintf(path_to_dir, "%s/%s", dir_name, entity->d_name);
                search_dir(path_to_dir, word_to_find, depth, cur_depth + 1);
                free(path_to_dir);
                break;
            }
            
        }
    }
    
    
    if (child != 0)
    {
        closedir(cur_dir);
        pid_t wpid;
        int status;
        while ((wpid = wait(&status)) > 0)
        {
            
        }
    }
        
}
int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        printf("Too little args\n");
        exit(1);
    }
    search_dir(argv[1], argv[2], atoi(argv[3]), 0);
}