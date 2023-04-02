#include "store_server.h"

int exist_webpage(char *url)
{
    char *db_directory = string_to_heap("db/");
    char *folder_name = sha1_hash((const unsigned char *)url, strlen(url));
    char *path = string_concat(db_directory, folder_name);

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