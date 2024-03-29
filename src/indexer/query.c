#include "query.h"

struct chunk* init_chunk(const char* word, int weighted, int formatted)
{
    struct chunk* new_chunk = malloc(sizeof(struct chunk));
    new_chunk->word = strdup(word);
    new_chunk->stem = get_stem(new_chunk->word);
    new_chunk->next = NULL;
    new_chunk->weighted = weighted;
    new_chunk->formatted = formatted;

    return new_chunk;
}

struct chunk* stack_word(struct chunk* chunk, struct chunk* new_chunk)
{
    new_chunk->next = chunk;

    return new_chunk;
}

struct chunk* unstack_word(struct chunk* chunk, struct chunk** element)
{
    (*element)->word = strdup(chunk->word);
    (*element)->stem = strdup(chunk->stem);
    (*element)->weighted = chunk->weighted;
    (*element)->formatted = chunk->formatted;

    struct chunk* new_head = chunk->next;
    free(chunk->word);
    free(chunk->stem);
    free(chunk);

    return new_head;
}

struct chunk** extractWords(const char* query, int* chunkCount)
{
    struct chunk** result = NULL;
    int count = 0;
    int list = 0;

    char* query_copy = strdup(query);
    char* token = strtok(query_copy, " ");
    while (token != NULL)
    {
        // We're in a chunk
        if (list)
        {
            if (strcmp(token, "\"") == 0)
            {
                list = 0;
            }
            else
            {
                // Create that word, add it to a chunk
                struct chunk* new_chunk = init_chunk(token, token[0] == '$', strchr(token, '%') != NULL);
                result[count - 1] = stack_word(result[count - 1], new_chunk);
            }
        }
        // We need a new chunk
        else
        {
            if (strcmp(token, "\"") == 0)
            {
                list = 1;

                token = strtok(NULL, " ");

                if (token == NULL)
                {
                    break;
                }
            }
            // Create that word, add it to the list
            result = realloc(result, (count + 1) * sizeof(struct chunk));
            normalize(token);
            char* token_stem = get_stem(token);
            result[count] = init_chunk(token_stem, token[0] == '$', strchr(token, '%') != NULL);
            count++;
        }

        token = strtok(NULL, " ");
    }

    free(query_copy);

    *chunkCount = count;
    return result;
}

struct chunk** get_query(char** query, int* num)
{
    int wordCount;
    struct chunk** words = extractWords(*query, &wordCount);

    printf("Extracted words:\n");
    for (int i = 0; i < wordCount; i++)
    {
        printf("%s, weight = %d, format = %d\n", words[i]->word, words[i]->weighted, words[i]->formatted);
        struct chunk* current = words[i]->next;
        while (current != NULL)
        {
            printf("=> %s, weight = %d, format = %d\n", current->word, current->weighted, current->formatted);
            current = current->next;
        }
    }

    *num = wordCount;

    return words;
}
