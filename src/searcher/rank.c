#include "rank.h"

int countCallback(void* data, int argc, char** argv, char** columnNames) 
{
    printf("I am in countCallback\n");
    
    int* count = (int*)data;
    *count = atoi(argv[0]);
    return 0;
}


int check_chunk(sqlite3* db, struct chunk* words)
{
    printf("I am in check_chunk\n");
    
    int res = 1;
    
    // this is just a word
    if(words->next == NULL)
    {
        char* error_message = NULL;
        
        char* target = words->stem;
        if(words->formatted)
        {
            target = words->word;
        }

        char query[100];
        snprintf(query, sizeof(query), "SELECT COUNT(*) FROM positions WHERE word = '%s'", target);

        int count = 0;
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
        char* error_message = NULL;
        
        // calculate the number of words
        char query[512];
        strcpy(query, "SELECT COUNT(*) FROM positions WHERE word IN (");
        struct chunk* current = words;
        while(current->next != NULL) 
        {
            strcat(query, "'");
            strcat(query, current->stem);
            strcat(query, "'");

            current = current->next;
        }

        strcat(query, "'");
        strcat(query, current->stem);
        strcat(query, "'");
        strcat(query, ", ");
        strcat(query, ");");

        int count = 0;
        int rc = sqlite3_exec(db, query, countCallback, &count, &error_message);
        if (rc != SQLITE_OK) 
        {
            fprintf(stderr, "SQLite error: %s\n", error_message);
            sqlite3_free(error_message);
            sqlite3_close(db);
            return 0;
        }

        // calculate their spread
        char spreadQuery[512];
        sprintf(spreadQuery, "SELECT MIN((MAX(position) - MIN(position)) / (COUNT(*) - 1)) "
                         "FROM positions WHERE word IN (");

        current = words;
        while (current->next != NULL) 
        {
            strcat(query, "'");
            strcat(query, current->stem);
            strcat(query, "'");

            current = current->next;
        }

        strcat(query, "'");
        strcat(query, current->stem);
        strcat(query, "'");
        strcat(query, ", ");
        strcat(query, ");");

        double averageSpread = 0;
        rc = sqlite3_exec(db, spreadQuery, countCallback, &averageSpread, &error_message);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQLite error: %s\n", error_message);
            sqlite3_free(error_message);
            sqlite3_close(db);
            return 0;
        }

        res = (int)(averageSpread * (double)count);
    }

    return res;
}

// This function calculates the final score of a page for a full query
int evaluate(struct result** page, struct chunk** words, int num_words)
{
    printf("I am in evaluate\n");

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

    sqlite3_close(db);
    (*page)->score = score;
    return score;
}

struct result** output_results(struct result** results, struct chunk** words, int num_words)
{
    printf("I am in output_results\n");
    
    // Initialize result struct 
    evaluate(results, words, num_words);
    struct result** sorted_results = malloc(sizeof(struct result*));
    *sorted_results = init_result((*results)->url, (*results)->score);
    
    struct result* current = (*results)->next;
    while(current != NULL)
    {
        evaluate(&current, words, num_words);
        *sorted_results = insert_result(*sorted_results, init_result(current->url, current->score));
        current = current->next; 
    }

    return sorted_results;
}