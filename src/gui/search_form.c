#include "gui.h"

struct results
{
    char *title;
    char *url;
    char *description;
    struct results *next;
};

struct results *generate_random_results()
{
    struct results *results = NULL;
    struct results *current = NULL;

    for (int i = 0; i < 10; i++)
    {
        struct results *new_result = malloc(sizeof(struct results));
        // Generate random title.
        new_result->title = "Title";
        new_result->url = "http://www.google.fr";
        new_result->description = "Description";
        new_result->next = NULL;
        if (results == NULL)
        {
            results = new_result;
            current = results;
        }
        else
        {
            current->next = new_result;
            current = current->next;
        }
    }
    return results;
}

void on_row_activated(GtkListBox *list, GtkListBoxRow *row, gpointer user_data)
{
    g_print("Row %d activated.\n", gtk_list_box_row_get_index(row));

    GtkWidget *box = gtk_bin_get_child(GTK_BIN(row));
    GList *children = gtk_container_get_children(GTK_CONTAINER(box));

    GtkWidget *title_label = g_list_nth_data(children, 0);
    GtkWidget *url_label = g_list_nth_data(children, 1);
    GtkWidget *description_label = g_list_nth_data(children, 2);

    const char *title = gtk_label_get_text(GTK_LABEL(title_label));
    const char *url = gtk_label_get_text(GTK_LABEL(url_label));
    const char *description = gtk_label_get_text(GTK_LABEL(description_label));

    g_print("Title: %s\n", title);
    g_print("URL: %s\n", url);
    g_print("Description: %s\n", description);

    g_list_free(children);
}


void display_new_form_with_results(char *request)
{
    struct results *results = generate_random_results();

    // Create a new window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Results");

    // Set the window's default size
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    

    // Create a new vertical box to arrange widgets vertically
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create a new scrolled window to display results
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    // Add results to the scrolled window
    GtkWidget *list = gtk_list_box_new();
    gtk_container_add(GTK_CONTAINER(scrolled_window), list);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);

    g_signal_connect(list, "row-activated", G_CALLBACK(on_row_activated), NULL);

    struct results *current = results;

    while (current != NULL)
    {
        GtkWidget *row = gtk_list_box_row_new();
        gtk_list_box_row_set_activatable(GTK_LIST_BOX_ROW(row), TRUE);
        gtk_list_box_row_set_selectable(GTK_LIST_BOX_ROW(row), TRUE);
        
        GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
        gtk_container_add(GTK_CONTAINER(row), box);

        GtkWidget *title = gtk_label_new(current->title);
        gtk_box_pack_start(GTK_BOX(box), title, TRUE, TRUE, 0);

        GtkWidget *url = gtk_label_new(current->url);
        gtk_box_pack_start(GTK_BOX(box), url, TRUE, TRUE, 0);

        GtkWidget *description = gtk_label_new(current->description);
        gtk_box_pack_start(GTK_BOX(box), description, TRUE, TRUE, 0);

        gtk_container_add(GTK_CONTAINER(list), row);
        
        // Connect the "activate" signal to each row
        g_signal_connect(row, "activate", G_CALLBACK(open_url_in_browser), current->url);

        current = current->next;
    }

    // Display the window and all its content
    gtk_widget_show_all(window);

    // Cleanup results
    free_results(results);
}

void open_url_in_browser(GtkWidget *row, gpointer data)
{
    char *url = (char *)data;
    GError *error = NULL;
    gtk_show_uri_on_window(NULL, url, gtk_get_current_event_time(), &error);
    if (error != NULL)
    {
        g_print("Could not launch the specified URI.\n");
        g_error_free(error);
    }
    g_print("Opening url : %s\n", url);
}

void free_results(struct results *results)
{
    struct results *tmp;
    while (results != NULL)
    {
        tmp = results;
        results = results->next;
        free(tmp);
    }
}
