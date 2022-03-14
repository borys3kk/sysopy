//
// Szymon Banys
//
#define  _GNU_SOURCE
#include "library.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void create_tmp(char** filenames, int len)
 { 
     if (len==0)
    {
        return;
    }
    char command[10000];
    command[0] = '\0';
    char* com1 = "wc ";
    char* com2 = "> temp";
    strcat(command, com1);
    for (int i = 0; i < len; i++)
    {
        strcat(command, filenames[i]);
        strcat(command, " ");
    }
    strcat(command, com2);

    system(command);
 }

struct block_table * make_arr(int num_blocks)
{
    struct block_table * new_tab = calloc(sizeof(struct memory_block), 1);
    new_tab->size = num_blocks;
    new_tab->array = calloc(sizeof(struct memory_block), num_blocks);
    return new_tab;
}

long get_size(FILE* file)
{
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

int write_to_block(struct block_table * blocks)
{
    FILE * file = fopen("temp", "r");
    int _id = 0;
    for(; _id < blocks->size; _id++)
    {
        if (blocks->array[_id] == 0) break;
    }
    if (_id >= blocks->size) return -1;
    
    int file_size = get_size(file);

    struct memory_block * block = calloc(sizeof(struct memory_block), 1);
    blocks->array[_id] = block;
    block->text = calloc(sizeof(char), file_size + 1);
    int i = 0;
    while (i < file_size)
    {
        block->text[i] = getc(file);
        i++;
    }
    block->text[i++] = '\0';
    block->size = file_size;
    fclose(file);
    return _id;
}

void remove_block(struct block_table * blocks, int block_id)
{
    if (blocks->array[block_id] == 0) return;
    struct memory_block * block = blocks->array[block_id];
    free(block->text);
    free(block);
}