# Makefile

# Compiler settings
CC := gcc
CFLAGS := -Wall -Wextra -std=c99 -D_DEFAULT_SOURCE -g

# Library paths
ZLIB_LIB := -lz -lcurl
OPENSSL_LIB := -lcrypto
XML_LIB := -lxml2
XML_INC := `xml2-config --cflags`
SQL_LIB := -lsqlite3
STEMMER_LIB := -L/path/to/libstemmer -lstemmer
STEMMER_INC := -I/path/to/libstemmer/include

# Includes
INCLUDES := -I/usr/include/libxml2 -I/usr/include/sqlite3

# Source files
SRC := ../structures/lists/linked-list/operations.c lexicon.c word_extractor.c
# Object files
OBJ := $(SRC:.c=.o)

# Output binary
BIN := word_extractor

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(CURL_LIB) $(ZLIB_LIB) $(OPENSSL_LIB) $(XML_LIB) $(STEMMER_LIB) $(SQL_LIB) -o $(BIN)

%.o: %.c
	$(CC) $(CFLAGS) $(XML2_INC) $(STEMMER_INC) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ) $(BIN)
	rm ../barrels/*
