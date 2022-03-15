CC = gcc
C_FLAGS = -Wall -Wno-unused-variable

all:
		static shared dynamic

static:
		make -C ../zad1 static 
		$(CC) $(C_FLAGS) ../zad2/main.c -I../zad1 -llibrary -L ../zad1 -D TESTS -o main

shared:
		make -C ../zad1 shared
		$(CC) $(C_FLAGS) ../zad2/main.c -I../zad1 -llibrary -L ../zad1 -D TESTS -o main

dynamic:
		make -C ../zad1 shared
		$(CC) $(C_FLAGS) ../zad2/main.c -o main.o -ldl -D DYNAMIC -D TESTS -o main

clean:
		make -C ../zad1 clean
		make -C ../zad2 clean
		rm -f *.o *.a *.so temp main 
		rm -rf files
		rm -f report_3a.txt
		make -C ../zad1 clean

test: clean
		cp -r ../zad2/files ./

		printf "static:\n" 2>/dev/null >> report_3a.txt
		make static
		make report

		printf "shared:\n" 2>/dev/null >> report_3a.txt
		make shared
		make report

		printf "dynamic:\n" 2>/dev/null >> report_3a.txt
		make dynamic
		make report

		rm -rf files temp main

report:
		cp -r ../zad2/files/ ./

		rm -f ./report_2.txt
		@printf "Static: \n" 2>/dev/null >>report_3a.txt

		@printf "One small file\n" 2>/dev/null >> report_3a.txt
		./main `cat ./files/arg1.txt| tr '\n' ' '` 2>/dev/null >> report_3a.txt
		
		@printf "Two small files\n" 2>/dev/null >> report_3a.txt
		./main `cat ./files/small_arg1.txt| tr '\n' ' '` 2>/dev/null >> report_3a.txt

		@printf "One mid file\n" 2>/dev/null >> report_3a.txt
		./main `cat ./files/arg2.txt | tr '\n' ' '` 2>/dev/null >> report_3a.txt

		@printf "Two mid files\n" 2>/dev/null >> report_3a.txt
		./main `cat ./files/mid_arg1.txt | tr '\n' ' '` 2>/dev/null >> report_3a.txt

		@printf "One chonky boi file\n" 2>/dev/null >> report_3a.txt
		./main `cat ./files/arg3.txt | tr '\n' ' '` 2>/dev/null >> report_3a.txt

		@printf "Many chonky boi test\n" 2>/dev/null >> report_3a.txt
		./main `cat ./files/big_arg1.txt | tr '\n' ' '` 2>/dev/null >> report_3a.txt