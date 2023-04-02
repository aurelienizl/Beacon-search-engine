#include "store_server.h"

char* sha1_hash(const unsigned char *data, size_t len)
{
    unsigned char digest[SHA_DIGEST_LENGTH];
    SHA1(data, len, digest);
    char *sha1string = (char *)malloc(SHA_DIGEST_LENGTH * 2 + 1);
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
    {
        sprintf(&sha1string[i * 2], "%02x", (unsigned int)digest[i]);
    }
    sha1string[SHA_DIGEST_LENGTH * 2] = '\0';
    return sha1string;
}

char *compress_array(char *data, size_t data_len)
{
    char *compressed_data = (char *)malloc(2 * data_len + 1); // Allocate memory for the compressed data
    size_t compressed_index = 0; // Index for the compressed data array

    for (size_t i = 0; i < data_len;)
    {
        char current_char = data[i];    // Current character being processed
        size_t char_count = 1;          // Initialize count to 1

        while (i + char_count < data_len && data[i + char_count] == current_char)
        {
            char_count++; // Increment count if the next character is the same
        }

        compressed_data[compressed_index++] = current_char; // Add the current character to the compressed data array
        compressed_index += sprintf(&compressed_data[compressed_index], "%zu", char_count); // Add the count to the compressed data array

        i += char_count; // Move to the next character
    }

    compressed_data[compressed_index] = '\0'; // Add null terminator to the compressed data array
    return compressed_data;
}

char* uncompress_array(char* data, size_t data_len)
{
    char *uncompressed_data = NULL;
    size_t uncompressed_data_len = 0;
    size_t uncompressed_index = 0;

    for (size_t i = 0; i < data_len;)
    {
        if (!isdigit(data[i])) // If the current character is not a digit
        {
            char current_char = data[i];
            size_t char_count = 0;
            i++;

            while (i < data_len && isdigit(data[i])) // Read the character count
            {
                char_count = char_count * 10 + (data[i] - '0');
                i++;
            }

            // Reallocate memory to accommodate the new uncompressed characters
            uncompressed_data = (char *)realloc(uncompressed_data, uncompressed_data_len + char_count + 1);
            memset(uncompressed_data + uncompressed_index, current_char, char_count); // Fill the array with char_count copies of current_char

            uncompressed_index += char_count;
            uncompressed_data_len += char_count;
        }
    }

    if (uncompressed_data)
    {
        uncompressed_data[uncompressed_index] = '\0'; // Add null terminator to the uncompressed data array
    }
    return uncompressed_data;
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