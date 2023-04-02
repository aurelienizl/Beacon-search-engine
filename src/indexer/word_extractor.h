#ifndef WORD_EXTRACTOR_H
#define WORD_EXTRACTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../structures/lists/linked-list/list.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/HTMLparser.h>
#include <sqlite3.h>
#include <unistd.h>
#include <tidy.h>
#include <buffio.h>
#include "../database/store_server.h"
#include "lexicon.h"

#define MAX_WORD_LEN 100
#define MAX_URL_LEN 512

typedef struct word_info {
    char word[MAX_WORD_LEN];
    int count;
    struct list* positions;
} word_info_t;

int create_database(word_info_t* word_list, char* url, int word_count, const char* db_path);
int is_word_char(char c);
void extract_words(char* html_content, word_info_t** word_list, int* word_count, int* pos);
int get_words(char* url, char* html_content);

#endif
