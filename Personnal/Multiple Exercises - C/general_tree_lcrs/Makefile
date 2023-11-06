CC=gcc
FLAGS = -std=c99 -pedantic -Werror -Wall -Wextra

library: gtree.o print.o serialize.o
	ar csr libgtree.a gtree.o print.o serialize.o

base : gtree.c print.c serialize.c
	$(CC) $(FLAGS) -c gtree.c print.c serialize.c

debug:
	$(CC) $(FLAGS) -g gtree.c print.c serialize.c


clean : 
	rm -rf gtree.o print.o serialize.o libgtree.a


