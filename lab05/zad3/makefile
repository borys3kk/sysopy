CC = gcc
CFLAGS = -Wall -g

all : test

compile:
	$(CC) $(FLAGS) test.c -o test
	$(CC) $(FLAGS) consumer.c -o consumer
	$(CC) $(FLAGS) producer.c -o producer

test: compile
		./test

clean:
		rm -f test producer consumer pipe