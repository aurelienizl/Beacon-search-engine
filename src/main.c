/*
 * Author: Izoulet Aurélien
 * Purpose: Store server.
 * Language: C.
 */

#include "crawler/crawler.h"
#include "database/store_server.h"
#include "structures/string/string.h"
#include "structures/stack/stack.h"

void init_db(char* url)
{
   struct stack *stack = new_stack();
   addstack(stack, url);
   while(!is_empty(stack))
   {
       char* url = unstack(stack);
       if(exist_webpage(url, strlen(url)))
       {
           free(url);
           continue;
       }
       char* data = crawl(url);
       struct webpage webpage = create_webpage(url, strlen(url), data, strlen(data));
       write_webpage(&webpage);

        // Analyse des liens de page 
        // J'ajoute dans la pile 
        // Je continue

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
    
    char* url1 = string_to_heap("http://www.citizendium.org/wiki/Welcome_to_Citizendium", 52);

    // Crawle the page
    char* data = crawl(url1);

    // Store to server
    struct webpage webpage = create_webpage(url1, strlen(url1), data, strlen(data));
    write_webpage(&webpage);

    // Check if the folder is properly created
    int res = exist_webpage(webpage.url, webpage.url_size);
    printf("The folder is created: %d\n", res);

    // Get webpage 
    unsigned char* content = read_webpage(webpage.url, webpage.url_size);

    // Print page 
    printf("%s\n", content);

    // Free memory

    free(content);
    free(url1);
    free(data);
    free(webpage.url);
    free(webpage.content->data);
    free(webpage.content);
    free(webpage.checksum);

    
}

int main()
{
    // test crawler
    tests();

    return 0;
}