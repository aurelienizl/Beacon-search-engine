CC ?= gcc
PKGCONFIG ?= $(shell which pkg-config)
CFLAGS := $(shell $(PKGCONFIG) --cflags gtk+-3.0) -Wall -Wextra -rdynamic -MMD -MP -Wno-unknown-pragmas
LIBS := $(shell $(PKGCONFIG) --libs gtk+-3.0)

SRC := gui.c tools.c
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
