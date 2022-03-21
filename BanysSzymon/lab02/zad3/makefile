CC = gcc
CFLAGS = -Wall -g

all: run

compile_dir:
		$(CC) $(FLAGS) main.c -o main

compile_nftw:
		$(CC) $(FLAGS) main1.c -o main1

run_dir: compile_dir
		./main $(ARGS)

run_nftw: compile_nftw
		./main1 $(ARGS)


clean:
		rm -f main main1 *.o 