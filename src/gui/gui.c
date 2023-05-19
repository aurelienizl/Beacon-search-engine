/*
 * Author: Izoulet Aurélien
 * Purpose: GUI Hermes implementation
 * Language: C.
 */

#include "gui.h"

GtkWidget *mainWindow;
GtkWidget *mainWindowFixed;
GtkTextBuffer *databaseInfo;

// --------------------MENU MAIN-----------------------

// Main Menu
GtkWidget *mainMenu;

// Cat 1
GtkWidget *buttonSave;
GtkWidget *buttonReload;
GtkWidget *buttonExit;
// Cat 2 
GtkWidget *buttonOpenDB;
GtkWidget *buttonUpdateDB;
GtkWidget *buttonRepairDB;
// Cat 3
GtkWidget *buttonInternetStatus;
GtkWidget *buttonTimeout;
GtkWidget *buttonThreads;

// ---------------------ABOUT MENU-----------------------

GtkWidget *aboutMenu;

// Cat 1
GtkWidget *buttonProjectTeam;
GtkWidget *buttonWebsite;

// --------------------INPUT SEARCH-----------------------

GtkWidget *inputSearch;

// Builder 
GtkBuilder *builder;

int main(int argc, char **argv)
{
    // Init gtk
    gtk_init(&argc, &argv);

    // Load ui configuration into the builder
    builder = gtk_builder_new_from_file("interface/UI.xml");

    // Load the main window
    mainWindow = GTK_WIDGET(gtk_builder_get_object(builder, "mainWindow"));

    // Exit ui when user closed the window
    g_signal_connect(mainWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Load signals from builder
    gtk_builder_connect_signals(builder, NULL);

    // Define all widgets from builder
    mainWindowFixed = GTK_WIDGET(gtk_builder_get_object(builder, "mainWindowFixed"));
    mainMenu = GTK_WIDGET(gtk_builder_get_object(builder, "mainMenu"));

    buttonSave = GTK_WIDGET(gtk_builder_get_object(builder, "buttonSave"));
    buttonReload = GTK_WIDGET(gtk_builder_get_object(builder, "buttonReload"));
    buttonExit = GTK_WIDGET(gtk_builder_get_object(builder, "buttonExit"));

    buttonOpenDB = GTK_WIDGET(gtk_builder_get_object(builder, "buttonOpenDB"));
    buttonUpdateDB = GTK_WIDGET(gtk_builder_get_object(builder, "buttonUpdateDB"));
    buttonRepairDB = GTK_WIDGET(gtk_builder_get_object(builder, "buttonRepairDB"));

    buttonInternetStatus = GTK_WIDGET(gtk_builder_get_object(builder, "buttonOpenDB"));
    buttonTimeout = GTK_WIDGET(gtk_builder_get_object(builder, "buttonUpdateDB"));
    buttonThreads = GTK_WIDGET(gtk_builder_get_object(builder, "buttonRepairDB"));

    inputSearch = GTK_WIDGET(gtk_builder_get_object(builder, "inputSearch"));
    databaseInfo = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "databaseInfo"));

    // Display the ui
    gtk_widget_show(mainWindow);

    // Wait the ui closed
    gtk_main();

    return EXIT_SUCCESS;
}

/*
 * Author: Izoulet Aurélien
 * Purpose: GUI Hermes implementation
 * Language: C.
 */

#include "gui.h"

// Main menu

void on_buttonSave_activate(GtkWidget *widget, gpointer *userdata)
{
    g_print("Button Save clicked !\n");
}

void on_buttonReload_activate(GtkWidget *widget, gpointer *userdata)
{
    g_print("Button Reload clicked !\n");
}

void on_buttonExit_activate(GtkWidget *widget, gpointer *userdata)
{
    g_print("Button Exit clicked !\n");
    gtk_main_quit();
}

void on_buttonOpenDB_activate(GtkWidget *widget, gpointer *userdata)
{
    g_print("button OpenDB clicked !\n");
    open_db();
}

void on_buttonUpdateDB_activate(GtkWidget *widget, gpointer *userdata)
{
    g_print("button ReloadDB clicked !\n");
}

void on_buttonRepairDB_activate(GtkWidget *widget, gpointer *userdata)
{
    g_print("button RepairDB clicked !\n");
}

void on_buttonThreads_activate(GtkWidget *widget, gpointer *userdata)
{
    g_print("button Threads clicked !\n");
}

void on_buttonTimeout_activate(GtkWidget *widget, gpointer *userdata)
{
    g_print("button Timeout clicked !\n");
}

void on_buttonInternetStatus_activate(GtkWidget *widget, gpointer *userdata)
{
    g_print("button Internet status clicked !\n");
}

// About menu

void on_buttonProjectTeam_activate(GtkWidget *widget, gpointer *userdata)
{
    g_print("button Project team clicked ! \n");
}

void on_buttonWebsite_activate(GtkWidget *widget, gpointer *userdata)
{
    g_print("button Website clisked !\n");
    system("open https://wiki-dev.org");
}

// Input search 

void on_inputSearch_activate(GtkWidget *widget, gpointer *userdata)
{
    g_print("You hit enter !\n");
    printf("You have typed : %s\n", gtk_entry_get_text((GtkEntry*)widget));
    display_new_form_with_results(gtk_entry_get_text((GtkEntry*)widget));
}


