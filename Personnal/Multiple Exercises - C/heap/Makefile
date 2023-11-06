CC=gcc
CFLAGS = -std=c99  -D_DEFAULT_SOURCE -pedantic -Werror -Wall -Wextra -g
src_files=add.o create.o pop.o del.o print.o
src_files_c=add.c create.c pop.c del.c print.c

library: create
	ar rcs libheap.a $(src_files)

create:
	gcc -c $(src_files_c)

clean : 
	rm -rf libmylib.a $(src_files) 

