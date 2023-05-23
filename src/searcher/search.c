#include "search.h"

struct result* init_result(const char* url, const char* title, const char* description, int score)
{
    struct result* new_result = malloc(sizeof(struct result));
    char* curl = malloc(sizeof(char) * (strlen(url) + 1));
    strcpy(curl, (char*)url);
    
    new_result->url = curl;
    
    if(title != NULL)
    {
        new_result->title = strdup(title);
    }
    else
    {
        new_result->title = NULL;
    }

    if(description != NULL)
    {
        new_result->description = strdup(description);
    }
    else
    {
        new_result->description = NULL;
    }
    new_result->next = NULL;
    new_result->score = score;

    return new_result;
}

struct result* stack_result(struct result* result, struct result* new_result)
{
    new_result->next = result;

    return new_result;
}

struct result* unstack_result(struct result* result, struct result** element)
{
    (*element)->url = result->url;
    (*element)->title = result->title;
    (*element)->description = result->description;
    (*element)->score = result->score;

    struct result* new_head = result->next;
    free(result);

    return new_head;
}

struct result* insert_result(struct result* result, struct result* new_result)
{
    if(result->score < new_result->score)
    {
        new_result->next = result;
        return new_result;
    }

    struct result* current = result;
    while(current->next != NULL)
    {
        if(current->next->score < new_result->score)
        {
            new_result->next = current->next;
            current->next = new_result;
            return result;
        }

        current = current->next;
    }

    current->next = new_result;
    return result;
}

void add_url_to_results(struct result** results, const char* url, int inc) 
{
    struct result* current = *results;
    while (current != NULL) {
        if (strcmp(current->url, url) == 0) {
            current->score = current->score + inc;
            return;
        }
        current = current->next;
    }

    //if the URL is not already in the results, add it as a new node
    struct result* new_result = init_result(url, NULL, NULL, inc);
    (*results) = stack_result(*results, new_result);
}

int process_database_callback(void* data, int argc, char** argv, char**column_names) 
{
    (void)argc;
    (void)column_names;
    
    struct result** results = (struct result**)data;
    const char* url = argv[2];

    add_url_to_results(results, url, 1);

    return 0;
}

struct result** get_pages(struct chunk** words, int num_words) 
{
    struct result** results = (struct result**)malloc(sizeof(struct result*));
    *results = NULL;
    
    sqlite3* db;
    char* error_message = NULL;
    int rc;

    rc = sqlite3_open("reverse_vector.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }

    // Construct the SQL query dynamically using the input words
    
    // For the SAME EXACT word
    char query[512];
    strcpy(query, "SELECT word, count, url FROM reverse_vector WHERE word IN (");
    for (int i = 0; i < num_words; i++) {
        strcat(query, "'");
        strcat(query, words[i]->stem);
        strcat(query, "'");
        if (i < num_words - 1) {
            strcat(query, ", ");
        }
    }
    strcat(query, ");");

    rc = sqlite3_exec(db, query, process_database_callback, results, &error_message);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", error_message);
        sqlite3_free(error_message);
    }

    sqlite3_close(db);

    return results;
}

/*
int main() 
{
    struct result** results;

    // Example list of words to search for
    int num_words;
    struct chunk** words = get_query(&num_words);

    results = get_pages(words, num_words);

    // Print the results
    struct result* current = *results;
    while (current != NULL) {
        printf("URL: %s, Score: %d\n", current->url, current->score);
        current = current->next;
    }

    return 0;
}
*/
