# Simple mini Makefile

CC=gcc
CPPFLAGS=  
CFLAGS= -Wall -Wextra -pedantic -std=c99 
LDFLAGS=
LDLIBS= 

FILES=main.o operations.o properties.o

all: release

release: CFLAGS += -O2 -DNDBUG	
release: $(FILES)
release: 
	$(CC) $^ -o app

check: CFLAGS += -g -fsanitize=address 
check: $(FILES)
check:
	$(CC) -fsanitize=address $^ -o app
	./app 
	make clean
	

debug: CFLAGS += -g
debug: $(FILES)
debug: 
	$(CC) $^ -o app
	valgrind --leak-check=full ./app 
	make clean

clean:
	${RM} app
	${RM} *.o
	${RM} *.gch
	