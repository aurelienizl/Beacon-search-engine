#include "gui.h"

char* open_db()
{
    g_print("Asking user to choose database\n");
    GtkWidget *p_dialog = NULL;
    p_dialog = gtk_file_chooser_dialog_new ("Open a db file", NULL,
			GTK_FILE_CHOOSER_ACTION_OPEN,
			"Cancel", GTK_RESPONSE_CANCEL,
			"Open", GTK_RESPONSE_ACCEPT,
			NULL);

	gchar *file_name = NULL;
    
	if (gtk_dialog_run (GTK_DIALOG (p_dialog)) == GTK_RESPONSE_ACCEPT)
	{
		file_name = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (p_dialog));
	}

	gtk_widget_destroy (p_dialog);

    if(file_name != NULL)
    {
        g_print("New database : %s\n", file_name);
    }
    else
    {
        g_print("No database selected ! \n");
    }

    // return file_name in any case
    return file_name;
}
