// Szymon Banys
//Zad 3 using opendir() readdir() DONE
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>


struct num_of_types
{
    int reg; //regular files
    int dir; //directories
    int chr; //special character files(?)
    int blk; //block files
    int fifo;//fifos
    int lnk; //links
    int sck; //sockets
};

char* update_num_types(struct num_of_types *types, struct dirent *entity) 
{
    switch (entity->d_type)
    {
    case 8:
        types->reg++;
        return "-reg";
    case 4:
        types->dir++;
        return "-dir";
    case 2:
        types->chr++;
        return "-char dev";
    case 6:
        types->blk++;
        return "-block dev";
    case 1:
        types->fifo++;
        return "-fifo";
    case 10:
        types->lnk++;
        return "-slink";
    case 12:
        types->sck++;
        return "-sock";
    default:
        return "";
    }
}
void get_file_info(char path[], struct stat file_stat, struct dirent *entity, struct num_of_types *ptr)
{
    char* file_type = update_num_types(ptr, entity);

    printf("Absolute path to file: %s , type: %s, size: %ld, number of links: %ld last access: %s, last modification: %s", path, file_type, file_stat.st_size, file_stat.st_nlink, ctime(&file_stat.st_atime), ctime(&file_stat.st_mtime));
}

void list_files(const char* dirname, struct num_of_types *ptr) //simplest recursive implementation to check the whole directory and subdirectories
{
    DIR* dir = opendir(dirname);
    if (dir == NULL)
    {
        perror("Can't open directory");
        exit(1);
    }
    
    struct dirent* entity;
    struct stat file_stat;
    entity = readdir(dir);
    while (entity != NULL)
    {
        
        if (strcmp(entity->d_name, ".")!= 0 && strcmp(entity->d_name, "..") != 0) // to ensure we won't fall into infinite loop
            {
            printf("%s\n", entity->d_name);
            char path[100] = {0};
            strcat(path, dirname);
            strcat(path, "/");
            strcat(path, entity->d_name);
            stat(path, &file_stat);
            get_file_info(path, file_stat, entity, ptr);
            if (entity->d_type == DT_DIR)
            {
                list_files(path, ptr);
            }
            }
            entity = readdir(dir);
    }
    

    closedir(dir);
}
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Pass argument");
        exit(1);
    }
    char* absolute_path = realpath(argv[1], NULL);
    struct num_of_types types = {0, 1, 0, 0, 0, 0, 0}; // we have at least one dir so we initialise the structure with it
    struct num_of_types *ptr = &types;
    list_files(absolute_path, ptr);
    printf("\n");
    printf("Directories: %d, regular files: %d, character devices: %d, block devices: %d, FIFO's: %d, links: %d, sockets: %d\n", types.dir, types.reg, types.chr, types.blk, types.fifo, types.lnk, types.sck);
    return 0;
}