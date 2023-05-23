#include "info.h"

char* get_description(char* content)
{
    if(content == NULL)
    {
        return "no description\n";
    }
    
    const char* desc_start = "<meta name=\"description\" content=\"";
    const char* desc_end = "\"";

    // Find the start and end positions of the description
    const char* start_pos = strstr(content, desc_start);
    if (start_pos == NULL) {
        return strdup("no description"); // Return "no description" if no description found
    }

    start_pos += strlen(desc_start); // Move past the description start tag

    const char* end_pos = strstr(start_pos, desc_end);
    if (end_pos == NULL) {
        return strdup("no description"); // Return "no description" if closing tag not found
    }

    // Calculate the length of the description
    size_t desc_len = end_pos - start_pos;

    // Allocate memory for the result and copy the description
    char* result = malloc(desc_len + 1);
    if (result == NULL) {
        return strdup("no description"); // Memory allocation failed, return "no description"
    }

    strncpy(result, start_pos, desc_len);
    result[desc_len] = '\0'; // Null-terminate the string

    return result;
}



char* get_title(char* content) {
    
    if(content == NULL)
    {
        return "no title\n";
    }
    
    const char* title_start = "<title>";
    const char* title_end = "</title>";

    // Find the start and end positions of the title
    const char* start_pos = strstr(content, title_start);
    if (start_pos == NULL) {
        return "no title\n"; // No title found
    }

    start_pos += strlen(title_start); // Move past the title start tag

    const char* end_pos = strstr(start_pos, title_end);
    if (end_pos == NULL) {
        return "no title\n"; // No closing tag found
    }

    // Calculate the length of the title
    size_t title_len = end_pos - start_pos;

    // Allocate memory for the result and copy the title
    char* result = malloc(title_len + 1);
    if (result == NULL) {
        return "no title\n"; // Memory allocation failed
    }

    strncpy(result, start_pos, title_len);
    result[title_len] = '\0'; // Null-terminate the string

    return result;
}