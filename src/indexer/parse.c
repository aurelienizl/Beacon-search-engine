#include <stdio.h>
#include <stdlib.h>
#include "word_extractor.h"

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

void read_file(const char *filename, char **content) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Seek to the end of the file to determine file size
    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate enough memory for the file data (and null terminator)
    *content = malloc(fsize + 1);
    if (*content == NULL) {
        perror("Error allocating memory for file content");
        fclose(file);
        return;
    }

    // Read file into memory
    fread(*content, 1, fsize, file);
    (*content)[fsize] = '\0';

    fclose(file);
}

void parse(const char* path) 
{
    struct dirent *entry;
    DIR *dir = opendir(path);

    if (dir == NULL) {
        perror("Unable to read directory");
        return;
    }

    char sub_path[512];
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            snprintf(sub_path, sizeof(sub_path), "%s/%s", path, entry->d_name);
            printf("Directory: %s\n", sub_path);

            char index_path[512];
            snprintf(index_path, sizeof(index_path), "%s/index.html", sub_path);

            struct stat buffer;
            if (stat(index_path, &buffer) == 0)
            {
                // Get the url by decoding the directory name
                char* url = base64_decode(entry->d_name);
                char* file_content;
                // Read the file
                read_file(index_path, &file_content);
                // Extract the words from the file
                get_words(url, file_content);
            }
            else
            {
                printf("No index.html in %s\n", sub_path);
            }
        }
    }
    closedir(dir);
}
