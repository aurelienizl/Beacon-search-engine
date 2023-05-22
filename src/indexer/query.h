#ifndef QUERY_H
#define QUERY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../indexer/lexicon.h"

#define MAX_QUERY_LENGTH 100

struct chunk 
{
    char* word;
    char* stem;
    struct chunk* next;
    int weighted;
    int formatted;
};

struct chunk* init_chunk(char* word, int weighted, int formatted);
struct chunk* stack_word(struct chunk* chunk, struct chunk* new_chunk);
struct chunk* unstack_word(struct chunk* chunk, struct chunk** element);
struct chunk** extractWords(const char* query, int* chunkCount);
struct chunk** get_query(int* num);

#endif