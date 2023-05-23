CC ?= gcc
PKGCONFIG ?= $(shell which pkg-config)
CFLAGS := $(shell $(PKGCONFIG) --cflags gtk+-3.0 libcurl libxml-2.0 sqlite3) -Wall -Wextra -rdynamic -MMD -MP -Wno-unknown-pragmas 
LIBS := $(shell $(PKGCONFIG) --libs gtk+-3.0 libcurl libxml-2.0 sqlite3) -lssl -lcrypto -lstemmer

SRC := gui.c tools.c search_form.c ../crawler/crawler.c ../crawler/tools.c ../indexer/lexicon.c ../indexer/parse.c ../indexer/table.c ../indexer/word_extractor.c ../structures/lists/linked-list/operations.c ../indexer/query.c ../searcher/search.c ../searcher/rank.c ../searcher/find.c 
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
