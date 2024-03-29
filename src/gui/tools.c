#include "gui.h"

gchar* open_folder()
{
    g_print("Asking user to choose a folder\n");
    GtkWidget *p_dialog = NULL;
    p_dialog = gtk_file_chooser_dialog_new ("Open a folder", NULL,
			GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
			"Cancel", GTK_RESPONSE_CANCEL,
			"Open", GTK_RESPONSE_ACCEPT,
			NULL);

	gchar *folder_name = NULL;
    
	if (gtk_dialog_run (GTK_DIALOG (p_dialog)) == GTK_RESPONSE_ACCEPT)
	{
		folder_name = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (p_dialog));
	}

	gtk_widget_destroy (p_dialog);

    if(folder_name != NULL)
    {
        g_print("Selected folder: %s\n", folder_name);
    }
    else
    {
        g_print("No folder selected!\n");
    }

    // return folder_name in any case
    return folder_name;
}

gint64 get_folder_size(gchar* folderPath)
{
    gint64 size = 0;
    DIR* dir = opendir(folderPath);

    if (dir == NULL)
    {
        perror("Unable to open the directory");
        return -1;
    }

    struct dirent* entry;
    char filePath[2048];

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            snprintf(filePath, sizeof(filePath), "%s/%s", folderPath, entry->d_name);

            struct stat entryInfo;
            if (stat(filePath, &entryInfo) == 0)
            {
                if (S_ISDIR(entryInfo.st_mode))
                {
                    size += get_folder_size(filePath);
                }
                else if (S_ISREG(entryInfo.st_mode))
                {
                    size += entryInfo.st_size;
                }
            }
            else
            {
                perror("Unable to get file/directory information");
            }
        }
    }

    closedir(dir);

    // Convert to MB
    size = size / (1024);

    return size;
}

gint64 folder_count(gchar* folderPath)
{
    gint64 count = 0;
    DIR* dir = opendir(folderPath);

    if (dir == NULL)
    {
        perror("Unable to open the directory");
        return -1;
    }

    struct dirent* entry;
    char filePath[2048];

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            snprintf(filePath, sizeof(filePath), "%s/%s", folderPath, entry->d_name);

            struct stat entryInfo;
            if (stat(filePath, &entryInfo) == 0 && S_ISDIR(entryInfo.st_mode))
            {
                count++;
            }
        }
    }

    closedir(dir);
    return count;
}

#pragma region IP_Address

size_t callbackfunc(void* data, size_t size, size_t nmemb, char** response) {
    size_t total_size = size * nmemb;
    *response = malloc(total_size + 1);
    if (*response) {
        memcpy(*response, data, total_size);
        (*response)[total_size] = '\0';
    }
    return total_size;
}

char* get_public_ip() {
    CURL* curl;
    CURLcode res;
    char* response = NULL;
    
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.ipify.org");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callbackfunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        
        res = curl_easy_perform(curl);
        
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return strdup("xxx.xxx.xxx.xxx");
        }
        
        curl_easy_cleanup(curl);
    }
    
    curl_global_cleanup();
    
    return response;
}

#pragma endregion IP_Address