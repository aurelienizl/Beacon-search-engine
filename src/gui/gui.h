#ifndef _GUI_H_
#define _GUI_H_

#include <gtk/gtk.h>
#include <sys/stat.h>  
#include <dirent.h>
#include <stdio.h>
#include <string.h>

#ifndef DT_DIR
#define DT_DIR 4
#endif

gchar *open_folder();

gint64 get_folder_size(gchar *folder_name);

gint64 folder_count(gchar* folderPath);

#endif  