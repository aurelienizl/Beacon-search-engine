# Compiler settings
CC := gcc
CFLAGS := -Wall -Wextra -std=c99 -D_DEFAULT_SOURCE -g -I/usr/include/libxml2 
DEPSFLAGS = -MMD -MP

# Libraries
CURL_LIB := `pkg-config --libs --cflags libcurl` 
ZLIB_LIB := -lz
OPENSSL_LIB := -lcrypto -lssl
LIBXML2_LIB := `pkg-config --libs --cflags libxml-2.0`

# Source files
SRC := crawler/crawler.c crawler/url_parser.c database/crypto.c database/io.c database/store_server.c structures/string/operations.c main.c 

# Object files
OBJ := $(SRC:.c=.o)
DEP := $(OBJ:.o=.d)

# Output binary
BIN := program

all: $(BIN)

$(BIN): $(OBJ)
	@pkg-config --exists libxml-2.0 libcurl || (echo "pkg-config or one of the libraries are not installed" && exit 1)
	$(CC) $(CFLAGS) $(OBJ) $(CURL_LIB) $(ZLIB_LIB) $(OPENSSL_LIB) $(LIBXML2_LIB) -o $(BIN)

%.o: %.c
	$(CC) $(CFLAGS) $(DEPSFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(BIN) $(DEP)
	rm -r db

-include $(DEP)
