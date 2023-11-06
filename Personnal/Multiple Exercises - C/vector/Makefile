CC=gcc
FLAGS = -std=c99 -pedantic -Werror -Wall -Wextra

all:library


library: vector.o
	ar csr libvector.a vector.o


base : vector.c
	$(CC) $(FLAGS) -c vector.c

clean : 
	rm -rf vector.o lebvector.a

