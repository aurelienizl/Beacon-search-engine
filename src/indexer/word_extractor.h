#ifndef WORD_EXTRACTOR_H
#define WORD_EXTRACTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/HTMLparser.h>
#include <sqlite3.h>
#include <unistd.h>


#define MAX_WORD_LEN 100

typedef struct word_info {
    char word[MAX_WORD_LEN];
    int count;
    int position;
} word_info_t;

void create_database(word_info_t* word_list, int word_count);
void extract_words(xmlNodePtr node, word_info_t** word_list, int* word_count, int* pos);
int main(int argc, char* argv[]);

#endif
