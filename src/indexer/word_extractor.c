#include "word_extractor.h"

char* sha1_hashh(const unsigned char *data, size_t len)
{
    unsigned char digest[SHA_DIGEST_LENGTH];
    SHA1(data, len, digest);
    char *sha1string = (char *)malloc(SHA_DIGEST_LENGTH * 2 + 1);
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
    {
        sprintf(&sha1string[i * 2], "%02x", (unsigned int)digest[i]);
    }
    sha1string[SHA_DIGEST_LENGTH * 2] = '\0';
    return sha1string;
}

int create_database(char* title, char* description, word_info_t* word_list, char* url, int word_count, const char* db_path) {
    sqlite3* db;
    char* err_msg = 0;

    // Check if database already exists
    if (access(db_path, F_OK) == 0)
    {
        printf("The word Database for this page already exists\n");
        return 0;
    }

    // Open the database connection
    int rc = sqlite3_open(db_path, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // Create the words table
    const char* create_words_sql = "CREATE TABLE IF NOT EXISTS words (word TEXT PRIMARY KEY, count INT);";
    rc = sqlite3_exec(db, create_words_sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create words table: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }

    // Create the positions table
    const char* create_positions_sql = "CREATE TABLE IF NOT EXISTS positions (word TEXT, position INT, FOREIGN KEY(word) REFERENCES words(word));";
    rc = sqlite3_exec(db, create_positions_sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create positions table: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }

    // Create anchor table 
    const char* create_anchor_sql = "CREATE TABLE IF NOT EXISTS anchor (url TEXT, title TEXT, description TEXT);";
    rc = sqlite3_exec(db, create_anchor_sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create anchor table: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }

    // Prepare the INSERT statements
    const char* insert_info_sql = "INSERT INTO anchor (url, title, description) VALUES (?, ?, ?);";
    sqlite3_stmt* insert_info_stmt;
    rc = sqlite3_prepare_v2(db, insert_info_sql, -1, &insert_info_stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare anchor insert statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(insert_info_stmt);
        sqlite3_close(db);
        return 1;
    }

    const char* insert_words_sql = "INSERT INTO words (word, count) VALUES (?, ?);";
    sqlite3_stmt* insert_words_stmt;
    rc = sqlite3_prepare_v2(db, insert_words_sql, -1, &insert_words_stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare words insert statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(insert_words_stmt);
        sqlite3_close(db);
        return 1;
    }

    const char* insert_positions_sql = "INSERT INTO positions (word, position) VALUES (?, ?);";
    sqlite3_stmt* insert_positions_stmt;
    rc = sqlite3_prepare_v2(db, insert_positions_sql, -1, &insert_positions_stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare positions insert statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(insert_positions_stmt);
        sqlite3_close(db);
        return 1;
    }

    // Begin the transaction
    sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, 0);

    // Bind and execute the INSERT statements
    rc = sqlite3_bind_text(insert_info_stmt, 1, url, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to bind URL parameter: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(insert_info_stmt);
        sqlite3_close(db);
        return 1;
    }
    rc = sqlite3_bind_text(insert_info_stmt, 2, title, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to bind TITLE parameter: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(insert_info_stmt);
        sqlite3_close(db);
        return 1;
    }
    rc = sqlite3_bind_text(insert_info_stmt, 3, description, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to bind DESCRIPTION parameter: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(insert_info_stmt);
        sqlite3_close(db);
        return 1;
    }
    rc = sqlite3_step(insert_info_stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Failed to insert URL into anchor table: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(insert_info_stmt);
        sqlite3_close(db);
        return 1;
    }
    sqlite3_reset(insert_info_stmt);

    for (int i = 0; i < word_count; i++) {
        rc = sqlite3_bind_text(insert_words_stmt, 1, word_list[i].word, -1, SQLITE_STATIC);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Failed to bind word parameter: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(insert_words_stmt);
            sqlite3_close(db);
            return 1;
        }
        rc = sqlite3_bind_int(insert_words_stmt, 2, word_list[i].count);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Failed to bind count parameter: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(insert_words_stmt);
            sqlite3_close(db);
            return 1;
        }
        rc = sqlite3_step(insert_words_stmt);
        if (rc != SQLITE_DONE) {
            fprintf(stderr, "Failed to insert word into words table: %s\n", sqlite3_errmsg(db));
            sqlite3_finalize(insert_words_stmt);
            sqlite3_close(db);
            return 1;
        }
        sqlite3_reset(insert_words_stmt);

        struct list* current = word_list[i].positions->next;
        while (current != NULL && current->data != NULL) {
            rc = sqlite3_bind_text(insert_positions_stmt, 1, word_list[i].word, -1, SQLITE_STATIC);
            if (rc != SQLITE_OK) {
                fprintf(stderr, "Failed to bind word parameter: %s\n", sqlite3_errmsg(db));
                sqlite3_finalize(insert_positions_stmt);
                sqlite3_close(db);
                return 1;
            }
            rc = sqlite3_bind_int(insert_positions_stmt, 2, *((int*)current->data));
            if (rc != SQLITE_OK) {
                fprintf(stderr, "Failed to bind position parameter: %s\n", sqlite3_errmsg(db));
                sqlite3_finalize(insert_positions_stmt);
                sqlite3_close(db);
                return 1;
            }
            rc = sqlite3_step(insert_positions_stmt);
            if (rc != SQLITE_DONE) {
                fprintf(stderr, "Failed to insert position into positions table: %s\n", sqlite3_errmsg(db));
                sqlite3_finalize(insert_positions_stmt);
                sqlite3_close(db);
                return 1;
            }
            sqlite3_reset(insert_positions_stmt);
            current = current->next;
        }
    }

    // Commit the transaction
    sqlite3_exec(db, "COMMIT TRANSACTION", 0, 0, 0);

    // Finalize the prepared statements
    sqlite3_finalize(insert_info_stmt);
    sqlite3_finalize(insert_words_stmt);
    sqlite3_finalize(insert_positions_stmt);

    // Close the database connection
    sqlite3_close(db);

    return 0;
}

int is_word_char(char c) {
    return isalnum(c) || c == '_';
}

void extract_words(char* html_content, word_info_t** word_list, int* word_count, int* pos)
{
    if (html_content == NULL || word_list == NULL || word_count == NULL || pos == NULL) {
        // Handle null pointers appropriately
        return;
    }

    char* p_start = NULL;
    char* p_end = NULL;
    char* word_start = NULL;
    char* word_end = NULL;

    // Find the first <p> tag
    p_start = strstr(html_content, "<p>");
    
    // Loop over all <p> tags
    while (p_start != NULL) {
        // Find the end of the <p> tag
        p_end = strstr(p_start, "</p>");
        if (p_end == NULL) {
            break; // No end tag found, stop processing
        }
        
        // Find the start of the first word inside the <p> tag
        word_start = strstr(p_start, ">") + 1;
        
        // Loop over all words inside the <p> tag
        while (word_start < p_end) {
            // Find the end of the current word
            word_end = word_start;
            while (is_word_char(*word_end) && word_end < p_end) {
                word_end++;
            }
            
            // Copy the current word to a separate variable
            if (word_end > word_start) {
                char word[MAX_WORD_LEN];
                int word_len = word_end - word_start;
                if (word_len >= MAX_WORD_LEN) {
                    word_len = MAX_WORD_LEN - 1;
                }
                strncpy(word, word_start, word_len);
                word[word_len] = '\0';
                
                // Save the position of the current word
                *pos = (int)(word_start - html_content);

                normalize(word);
                
                if (check_word(word)) {
                    while (!is_word_char(*word_end) && word_end < p_end) {
                        word_end++;
                    }
                    word_start = word_end;
                    continue;
                }
                
                char* stemmed_word = get_stem(word);
                int i = 0;
                for (i = 0; i < *word_count; i++) {
                    if (strcmp((*word_list)[i].word, stemmed_word) == 0) {
                        (*word_list)[i].count++;
                        int* data = calloc(1, sizeof(int));
                        *data = *pos;
                        struct list* position = new_element(data);
                        add_top((*word_list)[i].positions, position);
                        break;
                    }
                }

                // If the word doesn't exist in the list, add it
                if (i == *word_count) {
                    *word_list = realloc(*word_list, (*word_count + 1) * sizeof(word_info_t));
                    strncpy((*word_list)[*word_count].word, stemmed_word, MAX_WORD_LEN);
                    (*word_list)[*word_count].count = 1;
                    (*word_list)[*word_count].positions = new_list();
                    int* data = calloc(1, sizeof(int));
                    *data = *pos;
                    struct list* position = new_element(data);
                    add_top((*word_list)[*word_count].positions, position);
                    (*word_count)++;
                }

                free(stemmed_word);
            }
            
            // Find the start of the next word
            while (!is_word_char(*word_end) && word_end < p_end) {
                if ((*word_end) == '<') {
                    word_end++;
                    while ((*word_end) != '>' && word_end < p_end) {
                        word_end++;
                    }
                }
                word_end++;
            }
            word_start = word_end;
        }
        
        // Find the next <p> tag
        p_start = strstr(p_end, "<p>");
    }
}

int get_words(char* url, char* html_content) {
    printf("GETTING WORDS\n");

    word_info_t* word_list = NULL;
    int word_count = 0;
    int pos = 0;

    char* title = get_title(html_content);
    char* description = get_description(html_content);

    extract_words(html_content, &word_list, &word_count, &pos);
    printf("done parsing : %d\n", word_count);

    // Print the list of words with their positions and counts
    printf("Word\tPosition\tCount\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s\t%d\n", word_list[i].word, word_list[i].count);
        struct list* current = word_list[i].positions->next;
        while (current != NULL && current->data != NULL) {
            printf("%d ", *((int*)current->data));
            current = current->next;
        }
        printf("\n");
    }

    char* directory = calloc(265, sizeof(char));
    char* checksum = sha1_hashh((const unsigned char *)url, strlen(url)); 
    strcpy(directory, "../barrels/");
    strcat(directory, checksum);
    strcat(directory, ".db");
    create_database(title, description, word_list, url, word_count, (const char*)directory);

    // Clean up
    
    free(word_list);
    free(directory);

    return 0;
}
