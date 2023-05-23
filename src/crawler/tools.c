#include "crawler.h"

char *string_concat(char *str1, char *str2)
{
    size_t str1_len = strlen(str1);
    size_t str2_len = strlen(str2);
    char *str = malloc(str1_len + str2_len + 1); 
    if (str == NULL) {
        fprintf(stderr, "Failed to allocate memory for string_concat\n");
        exit(1);
    }
    strcpy(str, str1);
    strcpy(str + str1_len, str2);
    return str;
}


char *string_to_heap(char *string)
{
    size_t str_len = strlen(string) + 1;
    char *buffer = calloc(sizeof(char), str_len);
    if (buffer == NULL) {
        fprintf(stderr, "Failed to allocate memory for string_to_heap\n");
        exit(1);
    }
    memcpy(buffer, string, str_len);
    return buffer;
}


char* base64_encode(char* input)
{
    BIO *bio, *b64;
    BUF_MEM *buffer;
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);
    BIO_write(bio, input, strlen(input));
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &buffer);
    char *output = (char *) malloc(buffer->length + 1);
    memcpy(output, buffer->data, buffer->length);
    output[buffer->length] = '\0';
    BIO_free_all(bio);
    return output;
}