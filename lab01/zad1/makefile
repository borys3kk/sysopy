CC = gcc -Wall -std=c11 -ggdb

all: clean static shared

static:
	$(CC) -c library.c
	ar rcs liblibrary.a library.o

shared:
	$(CC) -c -fPIC library.c
	$(CC) -shared -fPIC -o library.so library.o

clean:
	rm -f *.o *.a *.so temp main