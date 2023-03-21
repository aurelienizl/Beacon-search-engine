/*
 * Author: Izoulet Aurélien
 * Purpose: Strings implementation
 * Language: C.
 */

#include "string.h"

char *string_concat(char *str1, char *str2)
{
    size_t str1_len = strlen(str1);
    size_t str2_len = strlen(str2);
    char *str = malloc(str1_len + str2_len + 1); 
    if (str == NULL) {
        fprintf(stderr, "Failed to allocate memory for string_concat\n");
        exit(1);
    }
    strncpy(str, str1, str1_len);
    strncpy(str + str1_len, str2, str2_len);
    str[str1_len + str2_len] = '\0'; 
    return str;
}


char *string_to_heap(char *string)
{
    char *buffer = calloc(sizeof(char), strlen(string));
    memcpy(buffer, string, strlen(string));
    return buffer;
}
