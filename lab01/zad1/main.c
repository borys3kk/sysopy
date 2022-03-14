//
// Szymon Banys
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "library.h"

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
    struct block_table * main_tab = NULL;

    char* _command;
    for (int i = 1; i < argc; i++)
    {
        _command = argv[i];

        if (strcmp(_command, "create_table") == 0)
        {
            if (i+1 >= argc)
            {
                printf("\nNo arguments given to create_table\n");
                exit(2);
            }
            i++;
            int size = atoi(argv[i]);
            main_tab = make_arr(size);
        }
        else if (strcmp(_command, "wc_files") == 0)
        {
            if (i+2 >= argc)
            {
                printf("\nToo little arguments given to wc_files\n");
                exit(1);
            }
        }
    }





}


    // system("clear");
    // int size;
    // int i;
    // struct block_table * main_tab = NULL;
    // char* _command = argv[1];
    // if (strcmp(_command, "create_table")==0)
    // {
    //     if (argc == 3)
    //     {
    //         size = atoi(argv[2]);
    //         main_tab = make_arr(size);
    //     }
    //     else
    //     {
    //         puts("Wrong number of args for this command");
    //         exit(1);
    //     }

    // }
    // else if (strcmp(_command, "wc_files")==0)
    // {
    //     if (argc > 2 && main_tab != NULL)
    //     {   
    //         char ** subset;
    //         subset = malloc((argc-2)*sizeof(char*));
    //         for (int j=2; j<argc;j++)
    //             subset[j-2] = argv[j];
    //         create_tmp(subset, argc-2);
    //     }
    //     else
    //     {
    //         printf("Something went wrong");
    //         exit(2);
    //     }
    // }
    // else if (strcmp(_command, "remove_block")==0)
    // {
    //     if (main_tab == NULL)
    //     {    
    //         printf("Create an array first");
    //         exit(3);       
    //     }
    // }
    // else
    //     printf("COMMAND NOT FOUND");
    // printf("\n");
    // printf("%d", argc);
    // printf("\n");