all: s21_grep

s21_grep: s21_grep.o
	gcc -Wall -Werror -Wextra -std=c11 -pedantic s21_grep.o -o s21_grep

s21_grep.o: s21_grep.c
	gcc -Wall -Werror -Wextra -std=c11 -pedantic -c s21_grep.c


clean:
	/bin/rm -rf s21_grep check*

test: s21_grep
	sh tests.sh
	
checks: style leaks 

style:
	clang-format -i s21_grep.c s21_grep.h
	clang-format -n s21_grep.c s21_grep.h
 
leaks: s21_grep
	leaks --atExit -- ./s21_grep -eivclnhso /test_files/*
