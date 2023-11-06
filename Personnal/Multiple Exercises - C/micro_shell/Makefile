CC=gcc
FLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -g


exec: all
all: library exec2


library: run_command.o
	ar csr libruncom.a run_command.o


base : run_command.c
	$(CC) $(FLAGS) -c run_command.c


exec2 : micro_shell.c
	$(CC) $(FLAGS)  micro_shell.c -L. -lruncom -o micro_shell


clean : 
	rm -rf run_command.o libruncom.a micro_shell
