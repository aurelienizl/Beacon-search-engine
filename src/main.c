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

void init_crawler(char* url)
{
	struct stack *stack = new_stack();
    addstack(stack, url);
    char* domain = get_domain_ext(url);


    while(!is_empty_stack(stack))
    {
        printf("0 : Unstacking url\n\n\n");
        char* current_url = unstack(stack);
        printf("1 : New url is : %s \n", current_url);
        if(exist_webpage(current_url))
        {
            printf("Webpage already exist !\n");
            continue;
        }
        printf("2 : Processing: %s\n", current_url);
        char* content = get_content(domain, current_url);
        printf("3 : Page crawled ! \n");
        if(content == NULL)
        {
            printf("Error while crawling page !\n");
            continue;
        }
        printf("3 : Adding page to database\n");
        write_to_file(content, current_url);
        printf("4 : Page added ! Parsing webpage...\n");
        bparser(&stack, content, current_url, strlen(content));
        printf("5 : Webpage parsed, stack updated !\n");
        free(current_url);
        printf("6 : Content freed !\n");

    }
    free(domain);
    freestack(stack);

}

int main()
{
    
    // test crawler
    char* url = string_to_heap("http://en.citizendium.org/wiki/index.php?title=Main_Page");
    init_crawler(url);

    return 0;
}
