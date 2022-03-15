CC = gcc
CFLAGS = -Wall -Wno-unused-variable

static:
		make -C ../zad1 static

clean:
		make -C ../zad1 clean
		rm -f *.o temp report_2.txt
test: 	static
		$(CC) $(CFLAGS) main.c -I ../zad1 -llibrary -L ../zad1 -D TESTS -o main
		make report
report:
		rm -f ./report_2.txt
		@printf "Static: \n" 2>/dev/null >>report_2.txt

		@printf "One small file\n" 2>/dev/null >> report_2.txt
		./main `cat ./files/arg1.txt| tr '\n' ' '` 2>/dev/null >> report_2.txt
		
		@printf "Two small files\n" 2>/dev/null >> report_2.txt
		./main `cat ./files/small_arg1.txt| tr '\n' ' '` 2>/dev/null >> report_2.txt

		@printf "One mid file\n" 2>/dev/null >> report_2.txt
		./main `cat ./files/arg2.txt | tr '\n' ' '` 2>/dev/null >> report_2.txt

		@printf "Two mid files\n" 2>/dev/null >> report_2.txt
		./main `cat ./files/mid_arg1.txt | tr '\n' ' '` 2>/dev/null >> report_2.txt

		@printf "One chonky boi file\n" 2>/dev/null >> report_2.txt
		./main `cat ./files/arg3.txt | tr '\n' ' '` 2>/dev/null >> report_2.txt

		@printf "Many chonky boi test\n" 2>/dev/null >> report_2.txt
		./main `cat ./files/big_arg1.txt | tr '\n' ' '` 2>/dev/null >> report_2.txt