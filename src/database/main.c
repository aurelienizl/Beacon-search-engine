/*
 * Author: Izoulet Aurélien
 * Purpose: Store server.
 * Language: C.
 */

#include "store_server.h"

int main()
{
    char *str_test = "TIME : 12/89/09 <html><head><title>Page Title</title></head><body><h1>My First Heading</h1><p>My first paragraph.</p></body></html>";
    size_t str_test_size = strlen(str_test);
    char *new_str = string_to_heap(str_test, str_test_size);
    add_to_server(new_str, str_test_size, "http://www.google.com", 22);

    char *html_test2 = "<html><head><title>Page Title</title></head><body><h1>My First Heading</h1><p>My first paragraph.</p></body></html>";
    size_t html_test2_size = strlen(html_test2);
    char *new_str2 = string_to_heap(html_test2, html_test2_size);
    add_to_server(new_str2, html_test2_size, "http://www.googles.com", 23); 

    free(new_str);
    free(new_str2);

    char *url = "http://www.google.com";
    size_t url_size = 22;
    char *request = string_to_heap(url, url_size);

    // Display request size
    printf("Request size: %ld\n\n", strlen(url));
    unsigned char *page = get_page_from_disk_with_url(request, url_size);
    printf("Page: %s\n\n", page);

    free(request);
    free(page);

    return 0;
}