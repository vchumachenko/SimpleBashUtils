CFLAGS = -Wall -Werror -Wextra -std=c11 -pedantic
CC = gcc

all: s21_cat

s21_cat: s21_cat.c s21_cat.h
	$(CC) s21_cat.c $(CFLAGS) -o s21_cat

clean:
	/bin/rm -rf s21_cat check*

checks: style leaks cppcheck

style:
	clang-format -i s21_cat.c  s21_cat.h
	clang-format -n s21_cat.c  s21_cat.h
 
leaks: s21_cat
	leaks --atExit -- ./s21_cat -bents /test_files/*

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem . *.c *.h
