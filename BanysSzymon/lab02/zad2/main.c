#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/times.h>


long double time_sec(clock_t time){
    return (long double)(time) / sysconf(_SC_CLK_TCK);
}

void print_res(clock_t clock_start, clock_t clock_end, struct tms start_tms, struct tms end_tms, FILE* file){
    printf("\nEXECUTION TIME\n");
    printf("real %Lf\n", time_sec(clock_end - clock_start));
    printf("user %Lf\n", time_sec(end_tms.tms_utime - start_tms.tms_utime));
    printf("sys  %Lf\n\n", time_sec(end_tms.tms_stime - start_tms.tms_stime));
    fprintf(file, "\nEXECUTION TIME\n");
    fprintf(file, "real %Lf\n", time_sec(clock_end - clock_start));
    fprintf(file, "user %Lf\n", time_sec(end_tms.tms_utime - start_tms.tms_utime));
    fprintf(file, "sys  %Lf\n\n", time_sec(end_tms.tms_stime - start_tms.tms_stime));
}


void lib_search(char *fileFrom, char searched_char)
{
    FILE *sourceFile = fopen(fileFrom, "r+");


    if (sourceFile == NULL)
    {
        perror("Couldnt open source file\n");
        exit(1);
    }

    char _char;
    int num_of_lines = 0;
    int num_of_char = 0;
    int newline_flag = 0;

    while (feof(sourceFile) == 0)
    {
        fread(&_char, sizeof(char), 1, sourceFile);
        if (_char == searched_char)
        {
            num_of_char++;
            if (!newline_flag)
            {
                newline_flag = 1;
                num_of_lines++;
            }
        }
        if (_char == '\n')
        {
            newline_flag = 0;
        }

    }
    printf("Number of: '%s' character: %d \n", &searched_char, num_of_char);
    printf("It appeared in %d lines\n", num_of_lines);
    fclose(sourceFile);
}

void sys_search(char *fileFrom, char searched_char)
{
    int sourceFile = open(fileFrom, O_RDONLY);

    if (sourceFile == -1)
    {
        perror("Couldnt open source file\n");
        exit(1);
    }
    char _char;
    int num_of_lines = 0;
    int num_of_char = 0;
    int newline_flag = 0;

    while(read(sourceFile, &_char, sizeof(char)) != 0)
    {
        if (_char == searched_char)
        {
            num_of_char++;
            if (!newline_flag)
            {
                newline_flag = 1;
                num_of_lines++;
            }
        }
        if (_char == '\n')
        {
            newline_flag = 0;
        }

    }

    printf("Number of: '%s' character: %d \n", &searched_char, num_of_char);
    printf("It appeared in %d lines\n", num_of_lines);
    close(sourceFile);
}

int main(int argc, char** argv)
{
    char sourceFile[100], search_char;
    sourceFile[0] = '\0';
    if (argc < 3)
    {
        perror("Too little args");
        exit(1);
    }
    else
    {
        strcat(sourceFile, argv[2]);
        search_char = *argv[1];
    }
    FILE* result = fopen("pomiar_zad_2.txt", "w");
    struct tms start_tms;
    struct tms end_tms;
    clock_t clock_start;
    clock_t clock_end;
    struct tms start_tms_1;
    struct tms end_tms_1;
    clock_t clock_start_1;
    clock_t clock_end_1;
    
    
    printf("\nLIB COPYING\n");
    clock_start = times(&start_tms);
    lib_search(sourceFile, search_char);
    clock_end = times(&end_tms);

    printf("\n SYS COPYING\n");
    clock_start_1 = times(&start_tms_1);
    sys_search(sourceFile, search_char);
    clock_end_1 = times(&end_tms_1);

    fprintf(result, "\n|---------------LIB---------------|\n");
    print_res(clock_start, clock_end, start_tms, end_tms, result);

    fprintf(result, "\n\n|---------------SYS---------------|\n");
    print_res(clock_start_1, clock_end_1, start_tms_1, end_tms_1, result);

}