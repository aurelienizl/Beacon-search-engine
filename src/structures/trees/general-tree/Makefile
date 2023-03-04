# Simple SDL mini Makefile

CC=gcc
CPPFLAGS=  
CFLAGS= -Wall -Werror -Wextra -pedantic -std=c99 
LDFLAGS=
LDLIBS= 

FILES=main.o operations.o properties.o

all: release

release: CFLAGS += -O2 -DNDBUG	
release: $(FILES)
release: 
	$(CC) $^ -o app
	make clean

check: CFLAGS += -g -fsanitize=address 
check: $(FILES)
check:
	$(CC) -fsanitize=address $^ -o app
	./app
	make clean

debug: CFLAGS += -g -fsanitize=address 
debug: $(FILES)
debug: 
	$(CC) -fsanitize=address $^ -o app
	make clean

clean:
	${RM} app
	${RM} *.o
	${RM} *.gch
	
