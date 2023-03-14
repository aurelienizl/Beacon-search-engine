/*
 * Author: Izoulet Aurélien
 * Purpose: Store server.
 * Language: C.
 */

#include "crawler/crawler.h"
#include "database/store_server.h"
#include "structures/string/string.h"

void tests()
{

    char* url1 = string_to_heap("http://www.citizendium.org/wiki/Welcome_to_Citizendium", 52);

    // Get the page from the url
    char* page = crawl(url1);

    // Create the webpage structure
    struct webpage webpage = create_webpage(url1, 52, page, strlen(page));

        
    // Write the webpage to the database
    write_webpage(&webpage);

    // Read the webpage from the database
    unsigned char* content = read_webpage(webpage.url, webpage.url_size);

    // Free the memory
    free(page);
    free(url1);
    free(webpage.url);
    free(webpage.content->data);
    free(webpage.content);
    free(webpage.checksum);
    free(content);
}

int main()
{
    // test crawler
    tests();

    return 0;
}