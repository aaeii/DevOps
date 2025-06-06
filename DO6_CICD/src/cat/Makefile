CC=gcc
CFLAGS= -c -Wall -Werror -Wextra

all: clean s21_cat

s21_cat: s21_cat.o
	$(CC) $(FLAGS) s21_cat.o -o s21_cat

clean:
	rm -f *.o s21_cat *.log