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
