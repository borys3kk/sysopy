//
// Szymon Banys
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#ifdef DYNAMIC
    #include <dlfcn.h>
#else
    #include "library.h"
#endif


#ifdef TESTS
#include <sys/times.h>
#include <stdint.h>
#include <unistd.h>
clock_t clock_t_begin, clock_t_end;
struct tms times_start_buffer, times_end_buffer;

void start_timer(){
	clock_t_begin = times(&times_start_buffer);
}

void stop_timer(){
	clock_t_end = times(&times_end_buffer);
}

double calc_time(clock_t s, clock_t e) {
    return ((double) (e - s) / (double) sysconf(_SC_CLK_TCK));
}

void print_times(const char* operation){
	printf("%20s real %.3fs user %.3fs sys %.3fs\n",
        operation,
        calc_time(clock_t_begin, clock_t_end),
        calc_time(times_start_buffer.tms_cutime, times_end_buffer.tms_cutime),
        calc_time(times_start_buffer.tms_cstime, times_end_buffer.tms_cstime));
}
#endif
int main(int argc, char** argv)
{   
    #ifdef DYNAMIC
        void * handle = dlopen("../zad1/library.so", RTLD_LAZY);
        if (!handle)
        {
            printf("Cannot load library.so\n");
            exit(9);

        }
        void (*create_tmp) (char**, int) = (void (*) (char**, int)) dlsym(handle, "create_tmp");
        int (*write_to_block) (block_table) = (int (*) (void)) dlsym(handle, "write_to_block");
        struct block_table (* make_arr)(int) = (struct block_table (*)(int)) dlsym(handle, "make_arr");
        void (* remove_block)(int) = (void(*)(int)) dlsym(handle, "remove_block");

        if (dlerror())
        {
            printf("Cannot load funcs from lib :/");
            exit(1);
        }


    #else
    struct block_table * main_tab = NULL;

    char* _command;

    if (argc <= 1)
    {
        printf("No args given");
        exit(2);
    }
    int idx = 1;
    while(idx < argc)
    {
        _command = argv[idx];

        if (strcmp(_command, "create_table") == 0)
        {
            if (idx+1 >= argc)
            {
                printf("\nNo arguments given to create_table\n");
                exit(1);
            }
            idx++;
            int size = atoi(argv[idx]);
            main_tab = make_arr(size);
        }
        else if (strcmp(_command, "wc_files") == 0)
        {
            if (idx+1 >= argc)
            {
                printf("\nToo little arguments given to wc_files\n");
                exit(3);
            }
        
            int wc_args = 0;
            int new_id = idx + 1;
            for(;new_id < argc; new_id++)
            {
                        if(strcmp(argv[new_id], "create_table") == 0 ||
                        strcmp(argv[new_id], "wc_files") == 0 ||
                        strcmp(argv[new_id], "remove_block") == 0 ||
                        strcmp(argv[new_id], "stimer") == 0 ||
                        strcmp(argv[new_id], "etimer") == 0)
                        {
                            break;
                        }
                        else
                        {
                            wc_args++;
                        }
            }
            if (wc_args == 0)
            {
                printf("No args were given to wc/ command was right after\n");
                exit(7);
            }
            if (main_tab == NULL)
            {
                printf("Create a table first!\n");
                exit(4);
            }
            create_tmp(argv + idx + 1, wc_args);
            int _written = write_to_block(main_tab);
            idx += wc_args;
        }
        else if (strcmp(argv[idx], "remove_block") == 0)
        {   if (idx + 1 >= argc)
            {
                printf("No argument was given to remove_block\n");
                exit(2);
            }
            int id_to_rem = atoi(argv[idx + 1]);
            if (id_to_rem > main_tab->size-1)
            {
                printf("Block with given id does not exist\n");
                exit(1);
            }
            remove_block(main_tab, id_to_rem);
            idx += 2; 
        }
        #ifdef TESTS
        else if (strcmp(_command,"stimer") == 0)
            start_timer();
        else if (strcmp(_command,"etimer") == 0)
        {
            stop_timer();
            idx++;
            print_times(argv[idx]);
        }
        #endif
        else {
            printf("There's no command for given argument: %s\n", _command);
            break;
        }
        
    idx++;
    }

    free(main_tab);
#endif
}