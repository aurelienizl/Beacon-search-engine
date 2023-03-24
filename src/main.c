/*
 * Author: Izoulet Aurélien
 * Purpose: Store server.
 * Language: C.
 */

#include "crawler/crawler.h"
#include "database/store_server.h"
#include "structures/string/string.h"
#include "structures/stack/stack.h"
// #include "indexer/indexer.h"
#include "indexer/link_extractor.h"

char *get_domain_ext(char *str)
{
    const char *prefix = "://";
    const char *start = strstr(str, prefix);
    if (start == NULL)
        return NULL;
    start += strlen(prefix);
    const char *end = strchr(start, '/');
    if (end == NULL)
        end = str + strlen(str);
    size_t length = end - start;
    char *domain = malloc(length + 1);
    if (domain == NULL)
        return NULL;
    strncpy(domain, start, length);
    domain[length] = '\0';
    return domain;
}

void new_test()
{
    // Get the url
    char *url = string_to_heap("http://monumentslitteraires.com/about-litep-literature-in-the-public-space");

    // TODO: get the domain name
    char *domain = get_domain_ext(url);

    // Create the stack
    struct stack *stack = new_stack();

    // Get the content of the url
    char *content = get_content(domain, url);

    printf("%s\n", content);

    bparser(&stack, content, url, strlen(content));

    // Free the url
    free(content);

    // While the stack is not empty
    while (!is_empty_stack(stack))
    {

        char *uurl = unstack(stack);
        char *ccontent = get_content(domain, uurl);
        printf("%s\n", uurl);

        if (ccontent == NULL)
        {
            continue;
        }

        // print first line of content
        char *line = strtok(ccontent, "\n");
        printf("%s\n", line);

        // Free the url
        free(ccontent);

        // Free the url
        free(uurl);

        // Free the url
        free(line);
    }

    // Free the stack
    freestack(stack);

    // Free the url
    free(url);

    // Free the domain
    free(domain);
}

int main()
{
    // test crawler
    new_test();

    return 0;
}
