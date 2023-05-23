#include "find.h"

struct results** find(char** input)
{   
    struct result** results;

    // Example list of words to search for
    int num_words;
    struct chunk** words = get_query(input, &num_words);

    results = get_pages(words, num_words);

    // Print the results
    struct result* current = *results;
    while (current != NULL) {
        printf("URL: %s, Score: %d\n", current->url, current->score);
        current = current->next;
    }

    struct result** sorted = output_results(results, words, num_words);

    if(sorted == NULL)
    {
        printf("no results found for your request\n");
        return NULL;
    }

    current = *sorted;
    while(current != NULL)
    {
        printf("SORTED URL: %s, Score: %d\n", current->url, current->score);
        current = current->next;
    }

    return sorted;
}
