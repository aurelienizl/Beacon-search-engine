#ifndef SEARCH_H
#define SEARCH_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <dirent.h>
#include "../structures/lists/linked-list/list.h"
#include "../indexer/query.h"

struct result
{
    const char* url;
    char* title;
    char* description;
    struct result* next;
    int score;
};

struct result* init_result(const char* url, const char* title, const char* description, int score);
struct result* stack_result(struct result* result, struct result* new_result);
struct result* unstack_result(struct result* chunk, struct result** element);
struct result* insert_result(struct result* result, struct result* new_result);
void add_url_to_results(struct result** results, const char* url, int inc);
int process_database_callback(void* data, int argc, char** argv, char** column_names);
struct result** get_pages(struct chunk** words, int num_words);
int main();


#endif 