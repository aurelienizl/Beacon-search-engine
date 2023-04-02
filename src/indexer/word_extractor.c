#include "word_extractor.h"

int create_database(word_info_t* word_list, int word_count, const char* db_path) {
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
    char* create_positions_sql = "CREATE TABLE IF NOT EXISTS positions (word TEXT, position INT, FOREIGN KEY(word) REFERENCES words(word));";
    rc = sqlite3_exec(db, create_positions_sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create positions table: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }

    // Insert the word info into the database
    sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, 0);
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

void extract_words(xmlNodePtr node, word_info_t** word_list, int* word_count, int* pos) {
    xmlNodePtr cur = NULL;

    for (cur = node; cur != NULL; cur = cur->next) {
        if (cur->type == XML_ELEMENT_NODE && strcmp((char*)cur->name, "p") == 0) {
            char* text_content = (char*)xmlNodeGetContent(cur);

            // Tokenize the text content into words
            char* word = strtok(text_content, " ");
            while (word != NULL) {
                // Check if the word already exists in the list
                normalize(word);
                if(check_word(word))
                {
                    word = strtok(NULL, " ");
                    (*pos)++;
                    continue;
                }
                word = get_stem(word);
                int i;
                for (i = 0; i < *word_count; i++) {
                    if (strcmp((*word_list)[i].word, word) == 0) {
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
                    strncpy((*word_list)[*word_count].word, word, MAX_WORD_LEN);
                    (*word_list)[*word_count].count = 1;
                    (*word_list)[*word_count].positions = new_list();
                    int* data = calloc(1, sizeof(int));
                    *data = *pos;
                    struct list* position = new_element(data);
                    add_top((*word_list)[*word_count].positions, position);
                    (*word_count)++;
                }

                // Get the next word
                word = strtok(NULL, " ");
                (*pos)++;
            }

            xmlFree(text_content);
        }

        extract_words(cur->children, word_list, word_count, pos);
    }
}

int get_words(char* url, char* html_content) {

    // Initialize libxml2 parser
    xmlInitParser();

    // Step 1: Clean up the HTML content using libtidy
    TidyBuffer output = {0};
    TidyDoc tdoc = tidyCreate();
    tidyParseString(tdoc, html_content);
    tidyCleanAndRepair(tdoc);
    tidyRunDiagnostics(tdoc);
    tidySaveBuffer(tdoc, &output);
    char* preprocessed_html_content = strdup((char*)output.bp);

    // Parse the HTML content
    xmlDocPtr doc = xmlReadMemory(preprocessed_html_content, strlen(preprocessed_html_content), url, NULL, 0);
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse input\n");
        return 1;
    }

    printf("%s\n", preprocessed_html_content);

    // Extract words from the <p> tags
    xmlNodePtr root = xmlDocGetRootElement(doc);
    word_info_t* word_list = NULL;
    int word_count = 0;
    int pos = 0;

    extract_words(root->children, &word_list, &word_count, &pos);

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
    create_database(word_list, word_count, (const char *)directory);

    // Clean up
    xmlFreeDoc(doc);
    xmlCleanupParser();
    free(word_list);

    return 0;
}
