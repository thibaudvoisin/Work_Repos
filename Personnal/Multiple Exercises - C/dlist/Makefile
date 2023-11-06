CC=gcc
FLAGS = -std=c99 -pedantic -Werror -Wall -Wextra

library: dlist_tresh_1.o dlist_tresh_2.o dlist_tresh_3.o
	ar csr libdlist.a dlist_tresh_1.o dlist_tresh_2.o dlist_tresh_3.o
 

debug: dlist_tresh_1.c dlist_tresh_2.c dlist_tresh_3.c
	$(CC) $(FLAGS) -g dlist_tresh_1.c dlist_tresh_2.c dlist_tresh_3.c -o debug

base : dlist_tresh_1.c dlist_tresh_2.c dlist_tresh_3.c
	$(CC) $(FLAGS) -c dlist_tresh_1.c dlist_tresh_2.c dlist_tresh_3.c

clean : 
	rm -rf dlist_tresh_1.o dlist_tresh_2.o dlist_tresh_3.o libdlist.a debug

