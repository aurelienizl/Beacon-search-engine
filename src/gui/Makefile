CC ?= gcc
PKGCONFIG ?= $(shell which pkg-config)
CFLAGS := $(shell $(PKGCONFIG) --cflags gtk+-3.0 libcurl libxml-2.0) -Wall -Wextra -rdynamic -MMD -MP -Wno-unknown-pragmas 
LIBS := $(shell $(PKGCONFIG) --libs gtk+-3.0 libcurl libxml-2.0) -lssl -lcrypto

SRC := gui.c tools.c search_form.c ../crawler/crawler.c ../crawler/tools.c
OBJS := $(SRC:.c=.o)
DEPS := $(OBJS:.o=.d)

-include $(DEPS)

all: exampleapp

%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<

exampleapp: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS) 

run: exampleapp
	./exampleapp

clean:
	rm -f $(OBJS) $(DEPS) exampleapp

.PHONY: all run clean
