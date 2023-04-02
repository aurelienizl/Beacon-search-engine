# Makefile

# Compiler settings
CC := gcc
CFLAGS := -Wall -Wextra -std=c99 -D_DEFAULT_SOURCE -g `pkg-config --cflags libxml-2.0` -I/usr/include/libxml2

# Library paths
ZLIB_LIB := -lz -lcurl
OPENSSL_LIB := -lcrypto
LIBXML2_LIB := `pkg-config --libs libxml-2.0` -lxml2

# Source files
SRC := crawler/crawler.c crawler/url_parser.c database/crypto.c database/io.c database/store_server.c structures/string/operations.c main.c 
# Object files
OBJ := $(SRC:.c=.o)

# Output binary
BIN := program

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(CURL_LIB) $(ZLIB_LIB) $(OPENSSL_LIB) $(LIBXML2_LIB) -o $(BIN)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(BIN)
	rm -r db
