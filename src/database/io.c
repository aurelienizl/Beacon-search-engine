#include "store_server.h"

/**
** @brief         Returns the first file in a directory.
** @param path    Path to the directory.
** @return        The first file in the directory (full_path).
** @note          The returned string must be freed, memory leaks safe.
*/

void rek_mkdir(char *path)
{
    char *sep = strrchr(path, '/');
    if (sep != NULL)
    {
        *sep = 0;
        rek_mkdir(path);
        *sep = '/';
    }
    if (mkdir(path, 0777) && errno != EEXIST)
        printf("error while trying to create '%s'\n%m\n", path);
}

void create_directory(char *url)
{
    char *db_directory = string_to_heap("db/");
    char *folder_name = sha1_hash((const unsigned char *)url, strlen(url));
    char *path = string_concat(db_directory, folder_name);

    printf("Folder name : %s\n", path);
    rek_mkdir(path);

    free(folder_name);
    free(path);
    
}

int write_to_file(char* data, char* url)
{
    char *db_directory = string_to_heap("db/");
    char *folder_name = sha1_hash((const unsigned char *)url, strlen(url));
    char *path = string_concat(db_directory, folder_name);
    char *filename = string_concat(path, "/index.html");

    create_directory(url);
    
    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("Failed to open file %s\n", filename);
        return 0;
    }

    fprintf(fp, "%s", data);
    fclose(fp);

    return 1;
}

char *io_get_file(char *path)
{
    DIR *dir = opendir(path);
    struct dirent *entry;

    if (dir == NULL)
    {
        printf("Failed to open directory at path %s\n", path);
        return NULL;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            break;
        }
            
    }
    
    closedir(dir);

    char* full_path = string_concat(path, "/");
    char* new_path = string_concat(full_path, entry->d_name);

    free(full_path);


    return new_path;
}