#ifndef TABLE_H
#define TABLE_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <dirent.h>
#include "../structures/lists/linked-list/list.h"

#define MAX_NUM_FILES 2500

void get_db_files(const char *dir_path, char ***db_file_paths, int *num_files);
void create_word_db(const char *dir_path);

#endif