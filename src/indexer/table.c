#include "table.h"

void get_db_files(const char *dir_path, char ***db_file_paths, int *num_files) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(dir_path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    // loop through each entry in the directory
    while ((entry = readdir(dir)) != NULL) {
        // check if the entry is a file with a .db extension
        char *ext = strrchr(entry->d_name, '.');
        if (ext != NULL && strcmp(ext, ".db") == 0) {
            // construct the path to the file and add it to the array
            char *path = malloc(strlen(dir_path) + strlen(entry->d_name) + 2);
            sprintf(path, "%s/%s", dir_path, entry->d_name);
            (*db_file_paths)[*num_files] = path;
            (*num_files)++;
        }
    }

    closedir(dir);
}

void create_word_db(const char *dir_path) {
    sqlite3 *db;
    char *err_msg = NULL;
    int rc = sqlite3_open("reverse_vector.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // create the wordID
    char* create_wid_sql = "CREATE TABLE IF NOT EXISTS word_id (word TEXT PRIMARY KEY);";
    rc = sqlite3_exec(db, create_wid_sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create words table: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return;
    }
    
    // create the reverse vector
    char* create_rv_sql = "CREATE TABLE IF NOT EXISTS reverse_vector (word TEXT, count INT, url TEXT, FOREIGN KEY(word) REFERENCES words(word));";
    rc = sqlite3_exec(db, create_rv_sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create words table: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return;
    }

    // get all db files in the directory
    int num_files = 0;
    char **db_file_paths = malloc(sizeof(char *) * MAX_NUM_FILES);
    get_db_files(dir_path, &db_file_paths, &num_files);


    sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, 0);

    // for each db file, get the words and positions tables
    for (int i = 0; i < num_files; i++) {
        // open the SQLite database file
        sqlite3 *sdb;
        int rc = sqlite3_open(db_file_paths[i], &sdb);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Failed to open database: %s\n", sqlite3_errmsg(sdb));
            sqlite3_close(sdb);
            continue;
        }

        // prepare a statement to select the words table
        sqlite3_stmt *words_stmt;
        rc = sqlite3_prepare_v2(sdb, "SELECT * FROM words;", -1, &words_stmt, NULL);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(sdb));
            sqlite3_finalize(words_stmt);
            sqlite3_close(sdb);
            continue;
        }

        // prepare a statement to select the positions table
        sqlite3_stmt *anchor_stmt;
        rc = sqlite3_prepare_v2(sdb, "SELECT * FROM anchor;", -1, &anchor_stmt, NULL);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(sdb));
            sqlite3_finalize(anchor_stmt);
            sqlite3_close(sdb);
            continue;
        }

        sqlite3_step(anchor_stmt);
        const char *url = (const char *) sqlite3_column_text(anchor_stmt, 0);

        // execute the statements and extract the data
        while (sqlite3_step(words_stmt) == SQLITE_ROW) {
            // extract data from the words table
            const char *word = (const char *) sqlite3_column_text(words_stmt, 0);
            int occurrences = sqlite3_column_int(words_stmt, 1);

            // insert the data into the reverse_vector table
            char insert_words_sql[1024];
            sprintf(insert_words_sql, "INSERT INTO reverse_vector (word, count, url) VALUES ('%s', %d, '%s');", word, occurrences, url);
            rc = sqlite3_exec(db, insert_words_sql, 0, 0, &err_msg);
            if (rc != SQLITE_OK) {
                fprintf(stderr, "Failed to insert word into words table: %s\n", err_msg);
                sqlite3_free(err_msg);
                sqlite3_close(db);
                return;
            }

            // insert the word into the wordID table 
            char insert_wordid_sql[1024];
            sprintf(insert_wordid_sql, "INSERT OR IGNORE INTO word_id (word) VALUES ('%s');", word);
            rc = sqlite3_exec(db, insert_wordid_sql, 0, 0, &err_msg);
            if (rc != SQLITE_OK) {
                fprintf(stderr, "Failed to insert word into words table: %s\n", err_msg);
                sqlite3_free(err_msg);
                sqlite3_close(db);
                return;
            }
        }

        // finalize the statements and close the database
        sqlite3_finalize(words_stmt);
        sqlite3_close(sdb);
    }
    
    sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, &err_msg);

    sqlite3_close(db);
}