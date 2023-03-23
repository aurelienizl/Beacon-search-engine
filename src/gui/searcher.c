#include "gui.h"

void display_new_form(char* text) {
    GtkWidget *window, *label;

    // create a new window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "New Window");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    // create a label widget to display the input text
    label = gtk_label_new(text);
    gtk_container_add(GTK_CONTAINER(window), label);

    // show the window and all its contents
    gtk_widget_show_all(window);
}