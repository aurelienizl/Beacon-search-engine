CC = gcc
CFLAGS = -Wall -g
LIBS = -lcurl
SRCS = main.c crawler.c
OBJS = $(SRCS:.c=.o)
TARGET = crawler

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
