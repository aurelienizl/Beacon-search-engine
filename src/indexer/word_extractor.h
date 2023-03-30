#ifndef WORD_EXTRACTOR_H
#define WORD_EXTRACTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ctype.h>
#include <libxml/parser.h>
#include <libxml/HTMLparser.h>
#include <sqlite3.h>

#define MAX_POSITIONS_STR_LEN 100

typedef struct word_info
{
    char *word;
    int count;
    int *positions;
    int num_positions;
    struct word_info *next;
} word_info_t;

word_info_t *extract_words(xmlDocPtr doc);
void free_word_info(word_info_t *word_infos);
int save_word_info(sqlite3* db, word_info_t* word_info, int num_words, char *document_id);
int build_barrels(char *content, char *document_id);

#endif
