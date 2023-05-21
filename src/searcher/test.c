#include "../indexer/query.h"
#include "search.h"
#include "rank.h"

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

    struct result** sorted = output_results(results, words, num_words);
    current = *sorted;
    while(current != NULL)
    {
        printf("SORTED URL: %s, Score: %d\n", current->url, current->score);
        current = current->next;
    }

    return 0;
}