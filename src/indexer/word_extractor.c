#include "word_extractor.h"

int create_database(word_info_t* word_list, char* url, int word_count, const char* db_path) {
    sqlite3* db;
    char* err_msg = 0;

    //check if database already exists
    if(access(db_path, F_OK) == 0)
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
    char* create_words_sql = "CREATE TABLE IF NOT EXISTS words (word TEXT PRIMARY KEY, count INT);";
    rc = sqlite3_exec(db, create_words_sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create words table: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }

    // Create the positions table
    char create_positions_sql[1024];
    sprintf(create_positions_sql, "CREATE TABLE IF NOT EXISTS positions (word TEXT, position INT, FOREIGN KEY(word) REFERENCES words(word));");
    rc = sqlite3_exec(db, create_positions_sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create positions table: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }

    // Create anchor table 
    char create_anchor_sql[1024];
    sprintf(create_anchor_sql, "CREATE TABLE IF NOT EXISTS anchor (url TEXT);");
    rc = sqlite3_exec(db, create_anchor_sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create anchor table: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }

    // Insertions
    sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, 0);
    
    // Insert the info into the anchor table
    char insert_info_sql[1024];
        sprintf(insert_info_sql, "INSERT INTO anchor (url) VALUES ('%s');", url);
        rc = sqlite3_exec(db, insert_info_sql, 0, 0, &err_msg);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Failed to insert word into words table: %s\n", err_msg);
            sqlite3_free(err_msg);
            sqlite3_close(db);
            return 1;
        }


    for (int i = 0; i < word_count; i++) {
        // Insert the word count into the words table
        char insert_words_sql[1024];
        sprintf(insert_words_sql, "INSERT INTO words (word, count) VALUES ('%s', %d);", word_list[i].word, word_list[i].count);
        rc = sqlite3_exec(db, insert_words_sql, 0, 0, &err_msg);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Failed to insert word into words table: %s\n", err_msg);
            sqlite3_free(err_msg);
            sqlite3_close(db);
            return 1;
        }

        // Insert the positions into the positions table
        struct list* current = word_list[i].positions->next;
        while (current != NULL) {
            int position = *((int*)current->data);
            char insert_positions_sql[1024];
            sprintf(insert_positions_sql, "INSERT INTO positions (word, position) VALUES ('%s', %d);", word_list[i].word, position);
            rc = sqlite3_exec(db, insert_positions_sql, 0, 0, &err_msg);
            if (rc != SQLITE_OK) {
                fprintf(stderr, "Failed to insert position into positions table: %s\n", err_msg);
                sqlite3_free(err_msg);
                sqlite3_close(db);
                return 1;
            }
            current = current->next;
        }
    }
    sqlite3_exec(db, "COMMIT TRANSACTION", 0, 0, 0);

    // Close the database connection
    sqlite3_close(db);

    return 0;
}

int is_word_char(char c) {
    return isalnum(c) || c == '_';
}

void extract_words(char* html_content, word_info_t** word_list, int* word_count, int* pos)
{
    char *p_start = NULL;
    char *p_end = NULL;
    char *word_start = NULL;
    char *word_end = NULL;

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
        char* word = calloc(MAX_WORD_LEN, sizeof(char));
        
        while (word_start < p_end) {
            // Find the end of the current word
            word_end = word_start;
            while (is_word_char(*word_end) && word_end < p_end) {
                word_end++;
            }
            
            // Copy the current word to the words array
            if (word_end > word_start) 
            {
                
                int word_len = word_end - word_start;
                if (word_len >= MAX_WORD_LEN) {
                    word_len = MAX_WORD_LEN - 1;
                }

                strncpy(word, word_start, word_len);
                word[word_len] = '\0';
                
                // Save the position of the current word
                *pos = (int)(word_start - html_content);

                normalize(word);
                
                if(check_word(word))
                {
                    while (!is_word_char(*word_end) && word_end < p_end) 
                    {
                        word_end++;
                    }
                    word_start = word_end;
                    continue;
                }
                
                word = get_stem(word);
                int i = 0;
                for (i = 0; i < *word_count; i++) {
                    if (strcmp((*word_list)[i].word, word) == 0) {
                        (*word_list)[i].count++;
                        //printf("updated %s, count %d, position, %d\n", word, (*word_list)[i].count, *pos);
                        int* data = calloc(1, sizeof(int));
                        *data = *pos;
                        struct list* position = new_element(data);
                        add_top((*word_list)[i].positions, position);
                        break;
                    }
                }

                // If the word doesn't exist in the list, add it
                if (i == *word_count) {
                    //printf("found %s, count %d, position %d\n", word, 1, *pos);
                    *word_list = realloc(*word_list, (*word_count + 1) * sizeof(word_info_t));
                    strncpy((*word_list)[*word_count].word, word, MAX_WORD_LEN);
                    (*word_list)[*word_count].count = 1;
                    (*word_list)[*word_count].positions = new_list();
                    int* data = calloc(1, sizeof(int));
                    *data = *pos;
                    struct list* position = new_element(data);
                    add_top((*word_list)[*word_count].positions, position);
                    (*word_count)++;
                }
            }
            
            // Find the start of the next word
            while (!is_word_char(*word_end) && word_end < p_end) {
                if((*word_end) == '<')
                {
                    word_end++;
                    while ((*word_end) != '>' && word_end < p_end) 
                    {
                        word_end++;
                    }
                    //continue;
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

    word_info_t* word_list = NULL;
    int word_count = 0;
    int pos = 0;

    extract_words(html_content, &word_list, &word_count, &pos);
    printf("done parsing : %d\n", word_count);

    // Print the list of words with their positions and counts
    printf("Word\tPosition\tCount\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s\t%d\n", word_list[i].word, word_list[i].count);
        struct list* current = word_list[i].positions->next;
        while(current != NULL)
        {
            printf("%d ", *((int*)current->data));
            current = current->next;
        }
        printf("\n");
    }

    char* directory = calloc(265, sizeof(char));
    char* checksum = sha1_hash((const unsigned char *) url, strlen(url)); 
    strcpy(directory, "../barrels/");
    strcat(directory, checksum);
    strcat(directory, ".db");
    create_database(word_list, url, word_count, (const char *)directory);

    // Clean up
    free(word_list);

    return 0;
}
