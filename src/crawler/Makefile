# Simple mini Makefile

CC=gcc
CPPFLAGS=  
CFLAGS= -Wall -Wextra -std=c99  -lpthread -D_REENTRANT
LDFLAGS=
LDLIBS= 

all: release

release: CFLAGS += -O2 -DNDBUG
release: main.o crawler.o
release: 
		$(CC) $^ -o prog


check: CFLAGS += -g 
check: main.o crawler.o
check:
		$(CC) $^ -o test-suite 
		###########################################
		valgrind --leak-check=full ./test-suite example.com
		###########################################
		make clean

clean:
		${RM} prog
		${RM} test-suite 
		${RM} *.o
		${RM} *.gch
