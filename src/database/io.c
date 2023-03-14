#include "store_server.h"

/**
** @brief         Returns the first file in a directory.
** @param path    Path to the directory.
** @return        The first file in the directory (full_path).
** @note          The returned string must be freed, memory leaks safe.
*/

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