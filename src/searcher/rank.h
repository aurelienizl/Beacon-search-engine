#ifndef RANK_H
#define RANK_H

#include "search.h"
#include "../indexer/query.h"
#include "../crawler/crawler.h"

char* sha1_hash(const unsigned char *data, size_t len);
int countCallback(void* data, int argc, char** argv, char** columnNames);
int check_chunk(sqlite3* db, struct chunk* words);
int evaluate(struct result** page, struct chunk** words, int num_words);
struct result** output_results(struct result** results, struct chunk** words, int num_words);

#endif