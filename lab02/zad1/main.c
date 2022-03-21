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




void lib_copying(char *fileFrom, char *fileTo)
{
    FILE *sourceFile = fopen(fileFrom, "r+");
    FILE *targetFile = fopen(fileTo, "w");

    if (sourceFile == NULL || targetFile == NULL)
    {
        perror("Couldnt open source file\n");
        exit(1);
    }
    char _char;
    int flag = 0;
    int whitespace_flag = 0;
    while (feof(sourceFile) == 0)
    {
        fread(&_char, sizeof(char), 1, sourceFile);
        if (_char != '\n')
        {
            flag = 0;
            fwrite(&_char, sizeof(char), 1, targetFile);
        }
        else
        {
            flag++;
            if (flag == 1)
            {
                fwrite(&_char, sizeof(char), 1, targetFile);
            }
        }
    }
    fclose(sourceFile);
    fclose(targetFile);

}

void sys_copying(char *fileFrom, char *fileTo)
{
    int sourceFile = open(fileFrom, O_RDONLY);
    int targetFile = open(fileTo, O_WRONLY | O_CREAT, 0666);

    if (sourceFile == -1)
    {
        perror("Couldnt open source file\n");
        exit(1);
    }
    char _char;
    int flag = 0;
    while(read(sourceFile, &_char, sizeof(char)) != 0)
    {
        if (_char != '\n')
        {
            flag = 0;
            write(targetFile, &_char, sizeof(char));
        }
        else
        {
            flag++;
            if (flag == 1)
            {
                write(targetFile, &_char, sizeof(char));
            }
        }
    }
    close(sourceFile);
    close(targetFile);

}

int main(int argc, char** argv)
{
    char sourceFile[100], targetFile[100];
    sourceFile[0] = '\0';
    targetFile[0] = '\0';
    if (argc < 3)
    {
        printf("Name of file to copy: ");
        scanf("%s", sourceFile);
        printf("Name of file to copy to: ");
        scanf("%s", targetFile);
        system("clear");
    }
    else
    {
        strcat(sourceFile, argv[1]);
        strcat(targetFile, argv[2]);
    }
    FILE* result = fopen("pomiar_zad_1.txt", "w");
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
    lib_copying(sourceFile, targetFile);
    clock_end = times(&end_tms);

    printf("\n SYS COPYING\n");
    clock_start_1 = times(&start_tms_1);
    sys_copying(sourceFile, targetFile);
    clock_end_1 = times(&end_tms_1);

    fprintf(result, "\n|---------------LIB---------------|\n");
    print_res(clock_start, clock_end, start_tms, end_tms, result);

    fprintf(result, "\n\n|---------------SYS---------------|\n");
    print_res(clock_start_1, clock_end_1, start_tms_1, end_tms_1, result);
}