CC=gcc
FLAGS = -std=c99 -pedantic -Werror -Wall -Wextra

library: fifo.o
	ar csr libfifo.a fifo.o


base : fifo.c
	$(CC) $(FLAGS) -c fifo.c

clean : 
	rm -rf fifo.o libfifo.a

