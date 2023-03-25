# Makefile

# Compiler settings
CC := gcc
CFLAGS := -Wall -Wextra -std=c99 -D_DEFAULT_SOURCE -g

# Library paths
ZLIB_LIB := -lz -lcurl
OPENSSL_LIB := -lcrypto

# Source files
SRC := crawler/crawler.c database/store_server.c structures/string/operations.c main.c database/crypto.c database/io.c \
structures/stack/operations.c structures/stack/properties.c structures/lists/linked-list/operations.c indexer/link_extractor.c
# Object files
OBJ := $(SRC:.c=.o)

# Output binary
BIN := program

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(CURL_LIB) $(ZLIB_LIB) $(OPENSSL_LIB) -o $(BIN)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(BIN)
	rm -r db
