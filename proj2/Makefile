PROJECT 		= 'IOS_project_num2'
PR 				= proj2
M 				= main
SOURCES 		= Makefile \
		  			$(M).c \
		  			$(M).h
SHELL 			= /bin/bash
COMPILER 		= gcc
FLAGS 			= -std=gnu99 -Wall -Wextra -Werror -pedantic -pthread

.PHONY: 	all run clean valgrind zip

all: 		main.o
			$(COMPILER) $(FLAGS) $(M).o  -o $(PR) -lm

main.o: 	$(M).c $(M).h
			$(COMPILER) $(FLAGS) -c main.c -o $@

run:		all $(PR)
			./proj2 8 4 10 4 5

clean:
			-rm -f $(M) $(M).o


valgrind:	$(PR)
			valgrind --leak-check=full ./proj2 8 4 10 4 5

zip:
			$(MAKE) clean
			$(MAKE) all
			$(MAKE) clean
			zip -q $(PR).zip $(SOURCES)