#include "word_extractor.h"

void create_database(word_info_t* word_list, int word_count) {
    sqlite3* db;
    char* err_msg = NULL;

    // Check if the database file exists
    int file_exists = access("words.db", F_OK) == 0;

    // Open a connection to the database
    int rc = sqlite3_open("words.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // Create the "words" table if it doesn't exist
    char* sql = "CREATE TABLE IF NOT EXISTS words ("
                "word TEXT PRIMARY KEY,"
                "count INTEGER,"
                "position INTEGER"
                ");";
    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create table: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return;
    }

    // Insert the words into the "words" table
    sql = "INSERT INTO words (word, count, position) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    for (int i = 0; i < word_count; i++) {
        sqlite3_bind_text(stmt, 1, word_list[i].word, strlen(word_list[i].word), SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, word_list[i].count);
        sqlite3_bind_int(stmt, 3, word_list[i].position);
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            fprintf(stderr, "Failed to insert row: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return;
        }
        sqlite3_reset(stmt);
    }

    // Finalize the statement and close the database connection
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    if (!file_exists) {
        printf("Created new database file 'words.db'\n");
    } else {
        printf("Added %d new words to existing database file 'words.db'\n", word_count);
    }
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
                int i;
                for (i = 0; i < *word_count; i++) {
                    if (strcmp((*word_list)[i].word, word) == 0) {
                        (*word_list)[i].count++;
                        break;
                    }
                }

                // If the word doesn't exist in the list, add it
                if (i == *word_count) {
                    *word_list = realloc(*word_list, (*word_count + 1) * sizeof(word_info_t));
                    strncpy((*word_list)[*word_count].word, word, MAX_WORD_LEN);
                    (*word_list)[*word_count].count = 1;
                    (*word_count)++;
                }

                // Store the position of the word
                (*word_list)[i].position = *pos;

                // Get the next word
                word = strtok(NULL, " ");
                (*pos)++;
            }

            xmlFree(text_content);
        }

        extract_words(cur->children, word_list, word_count, pos);
    }
}

int main(int argc, char* argv[]) {
    char* html_content = "<html><body><p>This is the first paragraph.</p><p>This is the second paragraph.</p></body></html>";

    // Initialize libxml2 parser
    xmlInitParser();

    // Parse the HTML content
    xmlDocPtr doc = xmlReadMemory(html_content, strlen(html_content), "noname.xml", NULL, 0);
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse input\n");
        return 1;
    }

    // Extract words from the <p> tags
    xmlNodePtr root = xmlDocGetRootElement(doc);
    word_info_t* word_list = NULL;
    int word_count = 0;
    int pos = 0;

    extract_words(root->children, &word_list, &word_count, &pos);

    // Print the list of words with their positions and counts
    printf("Word\tPosition\tCount\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s\t%d\t%d\n", word_list[i].word, word_list[i].position, word_list[i].count);
    }

    create_database(word_list, word_count);

    // Clean up
    xmlFreeDoc(doc);
    xmlCleanupParser();
    free(word_list);

    return 0;
}
