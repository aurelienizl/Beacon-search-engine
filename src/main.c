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

void init_db(char *url)
{
    struct stack *stack = new_stack();
    addstack(stack, url);
    while (!is_empty_stack(stack))
    {
        char *url = unstack(stack);
        printf("Crawling %s\n", url);
        if (exist_webpage(url, strlen(url)))
        {
            printf("Website %s already crawled\n", url);
            free(url);
            continue;
        }
        char *data = crawl(url);
        struct webpage webpage = create_webpage(url, strlen(url), data, strlen(data));
        printf("Writing %s\n", webpage.url);
        write_webpage(&webpage);

        // Print the length of the webpage
        printf("Length of webpage: %ld\n", strlen(data));

        // Print the content of the webpage
        char *content = (char *)read_webpage(url, strlen(webpage.url));
        printf("Content of webpage: %s\n\n\n\n", content);

        //struct list *url_list = new_list();
	//bparser(&url_list, data, url, strlen(data));
    
	//printf("\n%s\n", (char *)url_list->next->data);
	

        //free(content);
        free(url);
        free(data);
        free(webpage.url);
        free(webpage.content->data);
        free(webpage.content);
        free(webpage.checksum);
    }
    free(stack);
}

void tests()
{

    char *url1 = string_to_heap("http://en.citizendium.org/wiki/Welcome_to_Citizendium");
    printf("url1: %s\n", url1);

    init_db(url1);
}

int main()
{
    // test crawler
    tests();

    return 0;
}
