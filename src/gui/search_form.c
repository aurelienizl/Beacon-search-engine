#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct results
{
    char *title;
    char *url;
    char *description;
    struct results *next;
};

struct results *generate_random_results()
{
    struct results *results = NULL;
    struct results *current = NULL;

    for (int i = 0; i < 10; i++)
    {
        struct results *new_result = malloc(sizeof(struct results));
        // Generate random title.
        new_result->title = strdup("Titled");
        new_result->url = strdup("http://www.google.fr");
        new_result->description = strdup("Description");
        new_result->next = NULL;
        if (results == NULL)
        {
            results = new_result;
            current = results;
        }
        else
        {
            current->next = new_result;
            current = current->next;
        }
    }
    return results;
}

/*
<div class="searchresult">
            <h2>{TITLE}</h2>
            <a href="{URL}">{URL}</a> <button>▼</button>
            <p>{DESCRIPTION}</p>
        </div>

*/

char *build_block_result(const char *title, const char *url, const char *description)
{
    const char *block = 
    "<div class=\"searchresult\">"
        "<h2>{TITLE}</h2>"
        "<a href=\"{URL}\">{URL}</a> <button>▼</button>"
        "<p>{DESCRIPTION}</p>"
    "</div>";

    size_t blockLength = strlen(block);
    size_t titleLength = strlen(title);
    size_t urlLength = strlen(url);
    size_t descriptionLength = strlen(description);

    char *res = malloc(blockLength + titleLength + urlLength + descriptionLength + 1);
    if (res == NULL)
    {
        perror("Memory allocation failed");
        return NULL;
    }

    char *ptr = res;
    size_t i = 0;

    while (*block)
    {
        if (strncmp(block, "{TITLE}", 7) == 0)
        {
            strcpy(ptr, title);
            ptr += titleLength;
            block += 7;
        }
        else if (strncmp(block, "{URL}", 5) == 0)
        {
            strcpy(ptr, url);
            ptr += urlLength;
            block += 5;
        }
        else if (strncmp(block, "{DESCRIPTION}", 13) == 0)
        {
            strcpy(ptr, description);
            ptr += descriptionLength;
            block += 13;
        }
        else
        {
            *ptr++ = *block++;
        }
    }

    *ptr = '\0';

    return res;
}

int main(void)
{
    struct results *results = generate_random_results();
    struct results *current = results;
    while (current != NULL)
    {
        char *block = build_block_result(current->title, current->url, current->description);
        printf("%s\n", block);
        free(block);
        current = current->next;
    }

    // Cleanup
    current = results;
    while (current != NULL)
    {
        struct results *next = current->next;
        free(current->title);
        free(current->url);
        free(current->description);
        free(current);
        current = next;
    }

    return 0;
}
