CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -g -std=c99 -D_DEFAULT_SOURCE  
LDFLAGS = -lz -lcrypto

SRCDIR = .
SRCDIR2 = ../structures/string
SRCS = $(wildcard $(SRCDIR)/*.c $(SRCDIR2)/*.c)
OBJS = $(SRCS:.c=.o)

store_server: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) store_server
	rm -r database

