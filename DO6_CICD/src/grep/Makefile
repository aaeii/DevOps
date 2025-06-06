CC=gcc
CFLAGS = -Wall -Wextra -Werror

all: clean s21_grep

s21_grep: s21_grep.c
	$(CC) $(CFLAGS) s21_grep.c -o s21_grep

clean:
	rm -f s21_grep *.log