/*
 * Author: Izoulet Aurélien
 * Purpose: Store server.
 * Language: C.
 */

#include "store_server.h"

struct webpage create_webpage(char *url, size_t url_size, char *content, size_t content_size)
{
    struct webpage webpage;

    normalize_html(content, &content_size);

    webpage.url = malloc(sizeof(char) * url_size);
    strcpy(webpage.url, url);
    webpage.url_size = url_size;

    webpage.content = malloc(sizeof(struct data));
    webpage.content->data = malloc(sizeof(char) * content_size);
    memcpy(webpage.content->data, content, content_size);
    webpage.content->data_size = content_size;
    webpage.content->data_capacity = content_size;

    // The checksum is calculated using the url

    webpage.checksum = sha1_hash((const unsigned char *)webpage.url, webpage.url_size);
    webpage.checksum_size = strlen(webpage.checksum);

    compress_data(webpage.content);

    return webpage;
}

void write_webpage(struct webpage *webpage)
{
    char *db_directory = string_to_heap("db/", 3);

    char *full_folder_name = string_concat(db_directory, webpage->checksum);

    char *file_name = string_to_heap("index.html", 10);

    if (access(db_directory, F_OK) == -1)
    {
        mkdir(db_directory, 0777);
    }

    if (access(full_folder_name, F_OK) == -1)
    {
        mkdir(full_folder_name, 0777);
    }

    char *path = string_concat(full_folder_name, "/");
    char *path_full = string_concat(path, file_name);

    FILE *file = fopen(path_full, "w");

    fwrite(webpage->content->data, sizeof(char), webpage->content->data_size, file);

    fclose(file);
    free(db_directory);
    free(full_folder_name);
    free(path_full);
    free(path);
    free(file_name);
}

void normalize_html(char *html, size_t *html_size)
{
    char *p = strstr(html, "<html");

    if (p != NULL)
    {
        size_t offset = p - html;
        size_t remaining = *html_size - offset;
        *html_size = remaining;
        memmove(html, html + offset, remaining + 1);
    }
}

unsigned char *read_webpage(char *url, size_t url_size)
{
    char *db_directory = string_to_heap("db/", 3);

    char *folder_name = sha1_hash((const unsigned char *)url, url_size);

    char *path = string_concat(db_directory, folder_name);

    char *file_path = io_get_file(path);

    FILE *file = fopen(file_path, "r");

    if (file == NULL)
    {
        printf("Failed to open file at path %s\n", file_path);
        free(db_directory);
        free(folder_name);
        free(path);
        free(file_path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char *buffer = (unsigned char *)calloc(sizeof(unsigned char), (file_size + 1));

    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';

    fclose(file);

    struct data *data = (struct data *)malloc(sizeof(struct data));
    data->data = buffer;
    data->data_size = file_size;
    data->data_capacity = 256000;

    decompress_data(data);

    unsigned char *res = data->data;

    free(db_directory);
    free(folder_name);
    free(path);
    free(data);
    free(file_path);

    return res;
}

int exist_webpage(char *url, size_t url_size)
{
    char *db_directory = string_to_heap("db/", 3);
    char *folder_name = sha1_hash((const unsigned char *)url, url_size);
    char* path = string_concat(db_directory, folder_name);

    DIR *dir = opendir(path);
    if (dir)
    {
        free(path);
        free(db_directory);
        free(folder_name);
        closedir(dir);
        return 1;
    }
    else
    {
        free(path);
        free(db_directory);
        free(folder_name);
        closedir(dir);
        return 0;
    }
}