CC ?= gcc
PKGCONFIG ?= $(shell which pkg-config)
CFLAGS := $(shell $(PKGCONFIG) --cflags gtk+-3.0 libcurl libxml-2.0) -Wall -Wextra -rdynamic -MMD -MP -Wno-unknown-pragmas 
LIBS := $(shell $(PKGCONFIG) --libs gtk+-3.0 libcurl libxml-2.0) -lssl -lcrypto

SRC := gui/gui.c gui/tools.c gui/search_form.c
OBJS := $(SRC:.c=.o)
DEPS := $(OBJS:.o=.d)

<<<<<<< HEAD
# Library paths
ZLIB_LIB := -lz -lcurl
OPENSSL_LIB := -lcrypto
XML_LIB := -lxml2
XML_INC := `xml2-config --cflags`
SQL_LIB := -lsqlite3

# Includes
INCLUDES := -I/usr/include/libxml2 -I/usr/include/sqlite3

# Source files
SRC := crawler/crawler.c database/store_server.c structures/string/operations.c main.c database/crypto.c database/io.c \
structures/stack/operations.c structures/stack/properties.c structures/lists/linked-list/operations.c indexer/link_extractor.c indexer/word_extractor.c
# Object files
OBJ := $(SRC:.c=.o)
=======
-include $(DEPS)

all: exampleapp
>>>>>>> b909e130dcde3a589c2f8968207cb124de890867

%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<

<<<<<<< HEAD
$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(CURL_LIB) $(ZLIB_LIB) $(OPENSSL_LIB) $(XML_LIB) $(SQL_LIB) -o $(BIN)

%.o: %.c
	$(CC) $(CFLAGS) $(XML2_INC) $(INCLUDES) -c $< -o $@
=======
exampleapp: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS) 

run: exampleapp
	./exampleapp
>>>>>>> b909e130dcde3a589c2f8968207cb124de890867

clean:
	rm -f $(OBJS) $(DEPS) exampleapp

.PHONY: all run clean
