//
// Szymon Banys
//
#ifndef LIBRARY_H
#define LIBRARY_H
#include <stdio.h>
struct memory_block
{
    int size;
    char * text;
};

struct block_table
{
    int size;
    struct memory_block** array;
    };

void create_tmp(char** filenames, int len);
int write_to_block(struct block_table * blocks);
struct block_table * make_arr(int num_blocks);
void remove_block(struct block_table * blocks, int block_id);
#endif //LIBRARY_H