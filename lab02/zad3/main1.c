// Szymon Banys
//Zad 3 using NFTW
#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <ftw.h>

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

struct num_of_types types = {0, 0, 0, 0, 0, 0, 0};
struct num_of_types *ptr = &types;

char* update_num_types(int _types) 
{
    printf("%d", _types);
    switch (_types)
    {
    case FTW_D || FTW_DNR:
        ptr->dir++;
        return "-dir";
    case FTW_F:
        ptr->reg++;
        return "-reg";
    case FTW_SL:
        ptr->lnk++;
        return "-slink";
    default:
        return "-unknown";
    }
}

int get_file_info(const char path[], const struct stat *file_stat, int _file, struct FTW *nftw_file)
{

    printf("Absolute path to file: %s , type: %s, size: %ld, number of links: %ld last access: %s, last modification: %s", path, update_num_types(_file), file_stat->st_size, file_stat->st_nlink, ctime(&file_stat->st_atime), ctime(&file_stat->st_mtime));
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Too little args");
        exit(1);
    }
    char* absolute_path = realpath(argv[1], NULL);
    int nftw_flags = FTW_PHYS | FTW_CHDIR | FTW_DEPTH | FTW_MOUNT;
    nftw(absolute_path, get_file_info, 20, nftw_flags);
    printf("Directories: %d, regular files: %d, character devices: %d, block devices: %d, FIFO's: %d, links: %d, sockets: %d\n", types.dir, types.reg, types.chr, types.blk, types.fifo, types.lnk, types.sck);
    printf("\n%d\n", S_IFDIR);
    return 0;
}