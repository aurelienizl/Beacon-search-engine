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

char* sha1_hash(const unsigned char *data, size_t len)
{
    unsigned char digest[SHA_DIGEST_LENGTH];
    SHA1(data, len, digest);

    static const char hex_table[] = "0123456789abcdef";
    char *sha1string = (char *)malloc(SHA_DIGEST_LENGTH * 2 + 1);

    if (!sha1string) 
    {
        fprintf(stderr, "Memory allocation failed in sha1_hash\n");
        return NULL;
    }

    for (int i = 0; i < SHA_DIGEST_LENGTH; ++i)
    {
        sha1string[i * 2] = hex_table[digest[i] >> 4];
        sha1string[i * 2 + 1] = hex_table[digest[i] & 0x0F];
    }
    sha1string[SHA_DIGEST_LENGTH * 2] = '\0';
    return sha1string;
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

char* base64_decode(char* input)
{
    BIO *bio, *b64;
    int len = strlen(input);
    char *buffer = (char *) malloc(len);
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new_mem_buf(input, len);
    bio = BIO_push(b64, bio);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    int decoded_len = BIO_read(bio, buffer, len);
    buffer[decoded_len] = '\0';
    BIO_free_all(bio);
    return buffer;
}