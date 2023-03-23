/*
 * Author: Izoulet Aurélien
 * Purpose: Store server.
 * Language: C.
 */

#include "crawler/crawler.h"
#include "database/store_server.h"
#include "structures/string/string.h"
#include "structures/stack/stack.h"
//#include "indexer/indexer.h"
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
    char *url = string_to_heap("http://en.citizendium.org/");

    // TODO: get the domain name
    char *domain = get_domain_ext(url);

    // Create the stack
    struct stack *stack = new_stack();
    // Add the url to the stack
    addstack(stack, url);
   
    // While the stack is not empty
    while(!is_empty_stack(stack))
    {
        char *url = unstack(stack);
        // Check if the url is already in the database
        if(read_webpage(url, strlen(url)) != NULL)
        {
            continue;
        }
        // Get the content of the url
        char* content = get_content(domain, url);
        if(content == NULL)
        {
            continue;
        }
        // Store the content in the database
        struct webpage page = create_webpage(url, strlen(url), content, strlen(content));
        write_webpage(&page);

        // TODO: get the links
    }
}

int main()
{
    // test crawler
    new_test();

    return 0;
}
