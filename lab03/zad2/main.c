#include <stdlib.h>
#include "malloc.h"
#include "unistd.h"
#include "string.h"
#include <sys/times.h>
#include <stdint.h>
#include <sys/wait.h>

long double time_sec(clock_t time){
    return (long double)(time) / sysconf(_SC_CLK_TCK);
}

void print_res(clock_t clock_start, clock_t clock_end, struct tms start_tms, struct tms end_tms, FILE* file){
    // printf("\nEXECUTION TIME\n");
    // printf("real %Lf\n", time_sec(clock_end - clock_start));
    // printf("user %Lf\n", time_sec(end_tms.tms_utime - start_tms.tms_utime));
    // printf("sys  %Lf\n\n", time_sec(end_tms.tms_stime - start_tms.tms_stime));
    fprintf(file, "\nEXECUTION TIME\n");
    fprintf(file, "real %Lf\n", time_sec(clock_end - clock_start));
    fprintf(file, "user %Lf\n", time_sec(end_tms.tms_utime - start_tms.tms_utime));
    fprintf(file, "sys  %Lf\n\n", time_sec(end_tms.tms_stime - start_tms.tms_stime));
}

double height(double x) // height for our rectangle
{
    return 4/(x*x + 1);
}

double rect_area(double width, double right)
{
    return height(right) * width;
}


void calculate_integral(int i, double _width, int rect_per_process, double right_edge) // in Riemann integral definiton we use right edge to calculate the height of rectangle
{
    char* res = calloc(sizeof(char), 256);
    sprintf(res, "w%d.txt", i);
    FILE* res_file = fopen(res, "w+");
    double result;
    for (int rect = 1; rect <= rect_per_process; rect++)
    {
        result = rect_area(_width, right_edge);
        printf("PPID: %d PID: %d rect_area: %f \n", getppid(), getpid(), result);
        fprintf(res_file, "%f \n", result);
        right_edge -= _width;
    }
    free(res);
    fclose(res_file);
}

int main(int argc, char const *argv[])
{
    struct tms start_tms, end_tms;
    clock_t clock_start, clock_end;
    
    if (argc < 3)
    {
        printf("Too little arguments");
        exit(1);
    }

    clock_start = times(&start_tms);

    double rect_width = atof(argv[1]);
    int num_processes = atoi(argv[2]);

    int rect_cnt = 1 / rect_width; // 1 is our range in integral (1-0)
    int rect_per_process = (int)(rect_cnt/num_processes);

    int process_id;
    double right_edge;
    for(int id_proc = 1; id_proc <= num_processes; id_proc++)
    {
        process_id = fork();
        if(process_id == 0) // to ensure we calculate it in the child process
        {
            right_edge = id_proc * rect_per_process * rect_width;
            calculate_integral(id_proc, rect_width, rect_per_process, right_edge);
            return 0; //to make sure the child process has ended
        }
    }

    pid_t wpid;
    int status = 0;
    while ((wpid = wait(&status)) > 0); //to not leave all time orphans

    double final_result = 0;
    FILE *res_file = fopen("result.txt", "w+");
    for (int i = 1; i <= num_processes; i++)
    {
        char* res = calloc(sizeof(char), 256);
        sprintf(res, "w%d.txt", i);
        FILE *read_result = fopen(res, "r");
        char chunk[256];
        while (!feof(read_result))
        {
            fgets(chunk, 256, read_result);
            double add_res = atof(chunk);
            final_result += add_res;
        }
        

        free(res);
    }
    fprintf(res_file, "Number of processes: %d Rectangle width: %f Result: %f", num_processes, rect_width, final_result);
    clock_end = times(&end_tms);
    print_res(clock_start, clock_end, start_tms, end_tms, res_file);
    fclose(res_file);
}