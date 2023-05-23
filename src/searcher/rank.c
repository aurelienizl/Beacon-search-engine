#include "rank.h"

char* sha1_hash(const unsigned char *data, size_t len)
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


int countCallback(void* data, int argc, char** argv, char** columnNames) 
{
    (void)argc;
    (void)columnNames;
    
    int* count = (int*)data;
    *count = atoi(argv[0]);
    return 0;
}


int check_chunk(sqlite3* db, struct chunk* words)
{
    int res = 1;
    
    // this is just a word
    if(words->next == NULL)
    {
        char* error_message = NULL;
        
        char query[256];
        
        if(words->formatted)
        {
            snprintf(query, sizeof(query), "SELECT COUNT(*) FROM positions WHERE word LIKE '%s'", words->word);
        }
        else
        {
            snprintf(query, sizeof(query), "SELECT COUNT(*) FROM positions WHERE word = '%s'", words->stem);
        }

        int count = 0;
        //printf("%s\n", query);
        int rc = sqlite3_exec(db, query, countCallback, &count, &error_message);
        if (rc != SQLITE_OK) 
        {
            printf("Query execution error: %s\n", error_message);
            sqlite3_free(error_message);
            return 0;
        }

        res = count*2;

        if(words->weighted)
        {
            res *= 2;
        }
    }
    // this is a whole chunk
    else
    {
        int numWords = 1;
        int threshold = 0;
        char** elements = malloc(sizeof(char*));
        elements[0] = words->stem;
        threshold += strlen(words->word);
        struct chunk* current = words->next;
        while(current != NULL)
        {
            numWords++;
            elements = realloc(elements, sizeof(char*) * numWords);
            elements[numWords - 1] = current->stem;
            threshold += strlen(current->word);
            current = current->next;
        }

        threshold += 50;
        
        int count = 0;
        sqlite3_stmt *stmt;
        
        for (int i = 0; i < numWords; i++) {
            for (int j = i + 1; j < numWords; j++) 
            {
                // Prepare the SQL statement
                const char *sql = "SELECT ABS((SELECT position FROM positions WHERE word = ?) - (SELECT position FROM positions WHERE word = ?))";
                int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
                if (rc != SQLITE_OK) 
                {
                    printf("Error preparing SQL statement: %s\n", sqlite3_errmsg(db));
                    continue;
                }
                
                // Bind the words as parameters
                rc = sqlite3_bind_text(stmt, 1, elements[i], -1, SQLITE_STATIC);
                if (rc != SQLITE_OK) 
                {
                    printf("Error binding word1 parameter: %s\n", sqlite3_errmsg(db));
                    continue;
                }
                
                rc = sqlite3_bind_text(stmt, 2, elements[j], -1, SQLITE_STATIC);
                if (rc != SQLITE_OK) 
                {
                    printf("Error binding word2 parameter: %s\n", sqlite3_errmsg(db));
                    continue;
                }
                
                // Execute the SQL statement
                rc = sqlite3_step(stmt);
                if (rc == SQLITE_ROW) 
                {
                    int distance = sqlite3_column_int(stmt, 0);
                    if (distance < threshold) 
                    {
                        count++;
                    }
                } 
                else if (rc != SQLITE_DONE) 
                {
                    printf("Error executing SQL statement: %s\n", sqlite3_errmsg(db));
                    continue;
                }

                // Reset the statement for the next iteration
                sqlite3_reset(stmt);
            }
        }

        // Finalize the statement and return the result
        sqlite3_finalize(stmt);
        res = count;
    }

    return res;
}

// This function calculates the final score of a page for a full query
int evaluate(struct result** page, struct chunk** words, int num_words)
{
    int score = 0;
    
    // Get the directory of the database of the page
    char* directory = calloc(265, sizeof(char));
    char* checksum = sha1_hash((const unsigned char *) (*page)->url, strlen((*page)->url)); 
    strcpy(directory, "../barrels/");
    strcat(directory, checksum);
    strcat(directory, ".db");

    // Open the database
    sqlite3* db;
    int rc;
    rc = sqlite3_open(directory, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    // Check for every word of the query
    for(int i = 0; i < num_words; i++)
    {
        score += check_chunk(db, words[i]);
    }
    
    sqlite3_stmt* fill;
    const char* data = "SELECT url, title, description FROM anchor";
    rc = sqlite3_prepare_v2(db, data, -1, &fill, NULL);
    if (rc != SQLITE_OK) 
    {
        fprintf(stderr, "Failed to prepare statement for anchor: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0;
    }

    // Fetch the rows from the result set
    while (sqlite3_step(fill) == SQLITE_ROW) 
    {
        const char* title = (const char*)sqlite3_column_text(fill, 1);
        const char* description = (const char*)sqlite3_column_text(fill, 2);

        // Fill the result struct with data
        (*page)->title = strdup(title);
        (*page)->description = strdup(description);
    }

    // Finalize the statement and close the database connection
    sqlite3_finalize(fill);

    (*page)->score = score;
    
    sqlite3_close(db);
    
    return score;
}

struct result** output_results(struct result** results, struct chunk** words, int num_words)
{
    if(*results == NULL)
    {
        return NULL;
    }
    
    // Initialize result struct 
    evaluate(results, words, num_words);
    struct result** sorted_results = malloc(sizeof(struct result*));
    *sorted_results = init_result((*results)->url, (*results)->title, (*results)->description, (*results)->score);
    
    struct result* current = (*results)->next;
    while(current != NULL)
    {
        evaluate(&current, words, num_words);
        *sorted_results = insert_result(*sorted_results, init_result(current->url, current->title, current->description, current->score));
        current = current->next; 
    }

    return sorted_results;
}