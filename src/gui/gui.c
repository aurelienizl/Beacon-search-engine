#include "gui.h" 

#pragma region Variables

// Width and height of the window

int WIDTH = 1280;
int HEIGHT = 720;

gchar *databasePath = "/db/";
gint64 databaseSize = 0;
gint64 entryCount = 0;
gchar *crawler_url = "";
gint crawler_thread_count = 0;
gchar *current_ip = "";
pthread_t crawler_thread;


// Widgets

GtkWidget *window;
GtkWidget *fixed;
GtkWidget *mainMenu;
GtkWidget *menuFiles;
GtkWidget *menuSubFiles;
GtkWidget *buttonSave;
GtkWidget *buttonReload;
GtkWidget *buttonQuit;
GtkWidget *menuDatabase;
GtkWidget *menuSubDatabase;
GtkWidget *buttonOpen;
GtkWidget *buttonCreate;
GtkWidget *menuSettings;
GtkWidget *menuSubSettings;
GtkWidget *buttonInternet;
GtkWidget *buttonThreads;
GtkWidget *buttonTimeout;
GtkWidget *inputSearch;
GtkWidget *engineNameImage;
GtkWidget *infoText;
GtkWidget *menuBarAbout;
GtkWidget *menuAbout;
GtkWidget *menuSubAbout;
GtkWidget *buttonWebsite;
GtkWidget *buttonGithub;

GtkTextBuffer *databaseInfo;

#pragma endregion Variables 

#pragma region Callbacks

void on_buttonSave_activate(GtkMenuItem *menuitem, gpointer data) {
    (void) menuitem;
    (void) data;
    g_print("Save button pressed\n");
}

void on_buttonReload_activate(GtkMenuItem *menuitem, gpointer data) {
    (void) menuitem;
    (void) data;
    g_print("Reload button pressed\n");
}

void on_buttonQuit_activate(GtkMenuItem *menuitem, gpointer data) {
    (void) menuitem;
    (void) data;
    g_print("Quit button pressed\n");
    gtk_main_quit();
}

void on_buttonOpen_activate(GtkMenuItem *menuitem, gpointer data) {
    (void) menuitem;
    (void) data;
    g_print("Open button pressed\n");
    gchar *folderPath = open_folder();
    if (folderPath == NULL || strcmp(folderPath, "") == 0)
    {
        g_print("No folder selected!\n");
        return;
    }
    databasePath = folderPath;
    databaseSize = get_folder_size(databasePath);
    entryCount = folder_count(databasePath);
    gchar *databaseInfoText = g_strdup_printf("DB :  %s\nDB Size : %ld KB\nEntry Count : %ld", databasePath, databaseSize, entryCount);
    gtk_text_buffer_set_text(databaseInfo, databaseInfoText, -1);
}

#pragma region CreateDatabase

void on_spin_button_value_changed(GtkSpinButton *spinButton, gpointer data) {
    (void) spinButton;
    (void) data;
    g_print("Spin button value changed\n");
    crawler_thread_count = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinButton));
    g_print("Thread count : %d\n", crawler_thread_count);
}

void on_urlEntry_activate(GtkEntry *urlEntry, gpointer data) {
    (void) urlEntry;
    (void) data;
    g_print("Create button pressed\n");
    crawler_url = g_strdup(gtk_entry_get_text(GTK_ENTRY(urlEntry)));
    g_print("URL : %s\n", crawler_url);
}

void* on_crawler_launched(void *data) 
{
    
    char* url = (char*)data;
    g_print("Crawler launched\n");
    g_print("URL : %s\n", url);
    crawl((void*) url);
    // Set database path to the crawler database path
    databasePath = "db/";
    databaseSize = get_folder_size(databasePath);
    entryCount = folder_count(databasePath);
    gchar *databaseInfoText = g_strdup_printf("DB :  %s\nDB Size : %ld KB\nEntry Count : %ld", databasePath, databaseSize, entryCount);
    gtk_text_buffer_set_text(databaseInfo, databaseInfoText, -1);
    parse("db");
    return NULL;
}

void on_buttonStart_activate(GtkButton *startButton, gpointer data) {
    (void) startButton;
    (void) data;
    g_print("Start button pressed\n");
    g_print("URL : %s\n", crawler_url);
    g_print("Thread count : %d\n", crawler_thread_count);
    pthread_create(&crawler_thread, NULL, on_crawler_launched, (void *)crawler_url);

}

void on_buttonCreate_activate(GtkMenuItem *menuitem, gpointer data) {
    (void) menuitem;
    (void) data;
    g_print("Create button pressed\n");

    // Create and display a new window.
    GtkWidget *dialog = gtk_dialog_new();
    gtk_window_set_title(GTK_WINDOW(dialog), "Create Database");

    // Get the dialog's content area.
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Set the dialog's size and position.
    gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 100);
    
    // Create a label for the spin button.
    GtkWidget *spinLabel = gtk_label_new("Number of threads");
    gtk_box_pack_start(GTK_BOX(content_area), spinLabel, FALSE, FALSE, 5);

    // Create a spin button for choosing the number of threads.
    GtkWidget *spinButton = gtk_spin_button_new_with_range(1, 100, 1);
    gtk_box_pack_start(GTK_BOX(content_area), spinButton, FALSE, FALSE, 5);
    g_signal_connect(spinButton, "value-changed", G_CALLBACK(on_spin_button_value_changed), NULL);

    // Create a label for showing the current IP address.
    if (current_ip == NULL)
    {
        current_ip = "xxx.xxx.xxx.xxx";
    }
    else 
    {
        // Add to the current IP address the string "IP:"
        gchar *ipLabelString = g_strdup_printf("Current IP: %s", current_ip);
        current_ip = ipLabelString;
    }
    GtkWidget *ipLabel = gtk_label_new(current_ip); 
    gtk_box_pack_start(GTK_BOX(content_area), ipLabel, FALSE, FALSE, 5);

    // Create an entry for user to enter an URL.
    GtkWidget *urlEntry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(urlEntry), "Enter URL");
    gtk_box_pack_start(GTK_BOX(content_area), urlEntry, FALSE, FALSE, 5);
    // Create a signal handler for the entry when the value changes.
    g_signal_connect(urlEntry, "activate", G_CALLBACK(on_urlEntry_activate), NULL);

    // Add a button for starting the crawler.
    GtkWidget *startButton = gtk_button_new_with_label("Start");
    gtk_box_pack_start(GTK_BOX(content_area), startButton, FALSE, FALSE, 5);
    // Create a signal handler for the button when it is clicked.
    g_signal_connect(startButton, "clicked", G_CALLBACK(on_buttonStart_activate), NULL);

    gtk_widget_show_all(dialog);
}

#pragma endregion CreateDatabase

void on_buttonInternet_activate(GtkMenuItem *menuitem, gpointer data)
{
    (void)menuitem;
    (void)data;

    g_print("Internet button pressed\n");

    // Check internet connectivity
    GNetworkConnectivity connectivity = g_network_monitor_get_connectivity(g_network_monitor_get_default());

    GtkWidget *dialog = gtk_dialog_new();
    gtk_window_set_title(GTK_WINDOW(dialog), "Internet Connectivity");

    // Set dialog size and position
    gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 40);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);

    GtkWidget *contentArea = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_container_add(GTK_CONTAINER(contentArea), box);

    GtkWidget *label = gtk_label_new(NULL);
    gtk_label_set_text(GTK_LABEL(label), (connectivity == G_NETWORK_CONNECTIVITY_FULL) ? "You are connected!" : "You are not connected!");
    gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 10);
    gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(label, GTK_ALIGN_CENTER);

    if (connectivity == G_NETWORK_CONNECTIVITY_FULL)
    {
        gtk_widget_set_name(dialog, "connected");  // Apply CSS class for styling (green color)
    }
    else
    {
        gtk_widget_set_name(dialog, "not-connected");  // Apply CSS class for styling (red color)
    }

    gtk_widget_show_all(dialog);

    // Run the dialog
    gtk_dialog_run(GTK_DIALOG(dialog));

    // Clean up
    gtk_widget_destroy(dialog);
}

void on_buttonThreads_activate(GtkMenuItem *menuitem, gpointer data) {
    (void) menuitem;
    (void) data;
    g_print("Threads button pressed\n");
}

void on_buttonTimeout_activate(GtkMenuItem *menuitem, gpointer data) {
    (void) menuitem;
    (void) data;
    g_print("Timeout button pressed\n");
}

void on_buttonSearch_activate(GtkMenuItem *menuitem, gpointer data) {
    (void) menuitem;
    (void) data;
    g_print("Search button pressed\n");
    // Get the text from the search entry
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(inputSearch));
    // Launch the searcher
    launch_searcher(text);
}

void on_buttonWebsite_activate(GtkMenuItem *menuitem, gpointer data) {
    (void) menuitem;
    (void) data;
    g_print("Website button pressed\n");
}

void on_buttonGithub_activate(GtkMenuItem *menuitem, gpointer data) {
    (void) menuitem;
    (void) data;
    g_print("Github button pressed\n");
}

#pragma endregion Callbacks

int main(int argc, char *argv[]) {

    gtk_init(&argc, &argv);

    // Get the current ip address
    current_ip = get_public_ip();

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_window_set_default_size(GTK_WINDOW(window), WIDTH, HEIGHT);
    gtk_window_set_title(GTK_WINDOW(window), "Beacon Search Engine");

    fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), fixed);

    #pragma region MainMenu

    mainMenu = gtk_menu_bar_new();
    gtk_fixed_put(GTK_FIXED(fixed), mainMenu, 0, 0);

    menuFiles = gtk_menu_item_new_with_mnemonic("_Files");
    gtk_menu_shell_append(GTK_MENU_SHELL(mainMenu), menuFiles);

    menuSubFiles = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuFiles), menuSubFiles);

    buttonSave = gtk_menu_item_new_with_label("Save research");
    gtk_menu_shell_append(GTK_MENU_SHELL(menuSubFiles), buttonSave);
    g_signal_connect(buttonSave, "activate", G_CALLBACK(on_buttonSave_activate), NULL);

    buttonReload = gtk_menu_item_new_with_label("Reload research");
    gtk_menu_shell_append(GTK_MENU_SHELL(menuSubFiles), buttonReload);
    g_signal_connect(buttonReload, "activate", G_CALLBACK(on_buttonReload_activate), NULL);

    buttonQuit = gtk_menu_item_new_with_label("Quit");
    gtk_menu_shell_append(GTK_MENU_SHELL(menuSubFiles), buttonQuit);
    g_signal_connect(buttonQuit, "activate", G_CALLBACK(on_buttonQuit_activate), NULL);


    menuDatabase = gtk_menu_item_new_with_mnemonic("_Database");
    gtk_menu_shell_append(GTK_MENU_SHELL(mainMenu), menuDatabase);

    menuSubDatabase = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuDatabase), menuSubDatabase);

    buttonOpen = gtk_menu_item_new_with_label("Open database");
    gtk_menu_shell_append(GTK_MENU_SHELL(menuSubDatabase), buttonOpen);
    g_signal_connect(buttonOpen, "activate", G_CALLBACK(on_buttonOpen_activate), NULL);

    buttonCreate = gtk_menu_item_new_with_label("Create database");
    gtk_menu_shell_append(GTK_MENU_SHELL(menuSubDatabase), buttonCreate);
    g_signal_connect(buttonCreate, "activate", G_CALLBACK(on_buttonCreate_activate), NULL);

    menuSettings = gtk_menu_item_new_with_mnemonic("_Settings");
    gtk_menu_shell_append(GTK_MENU_SHELL(mainMenu), menuSettings);

    menuSubSettings = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuSettings), menuSubSettings);

    buttonInternet = gtk_menu_item_new_with_label("Internet Status");
    gtk_menu_shell_append(GTK_MENU_SHELL(menuSubSettings), buttonInternet);
    g_signal_connect(buttonInternet, "activate", G_CALLBACK(on_buttonInternet_activate), NULL);

    buttonThreads = gtk_menu_item_new_with_label("Threads Status");
    gtk_menu_shell_append(GTK_MENU_SHELL(menuSubSettings), buttonThreads);
    g_signal_connect(buttonThreads, "activate", G_CALLBACK(on_buttonThreads_activate), NULL);

    buttonTimeout = gtk_menu_item_new_with_label("Timeout Status");
    gtk_menu_shell_append(GTK_MENU_SHELL(menuSubSettings), buttonTimeout);
    g_signal_connect(buttonTimeout, "activate", G_CALLBACK(on_buttonTimeout_activate), NULL);

    #pragma endregion MainMenu

    #pragma region AboutMenu

    menuBarAbout = gtk_menu_bar_new();
    gtk_fixed_put(GTK_FIXED(fixed), menuBarAbout, WIDTH, 0);

    menuAbout = gtk_menu_item_new_with_mnemonic("_About");
    gtk_menu_shell_append(GTK_MENU_SHELL(menuBarAbout), menuAbout);

    menuSubAbout = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuAbout), menuSubAbout);

    buttonWebsite = gtk_menu_item_new_with_label("Website");
    gtk_menu_shell_append(GTK_MENU_SHELL(menuSubAbout), buttonWebsite);
    g_signal_connect(buttonWebsite, "activate", G_CALLBACK(on_buttonWebsite_activate), NULL);

    buttonGithub = gtk_menu_item_new_with_label("Github");
    gtk_menu_shell_append(GTK_MENU_SHELL(menuSubAbout), buttonGithub);
    g_signal_connect(buttonGithub, "activate", G_CALLBACK(on_buttonGithub_activate), NULL);

    #pragma endregion AboutMenu

    #pragma region MainWidgets

    inputSearch = gtk_search_entry_new();
    gtk_fixed_put(GTK_FIXED(fixed), inputSearch, (WIDTH / 2) - 50, HEIGHT / 2);
    gtk_widget_set_size_request(inputSearch, 200, 40);
    gtk_entry_set_placeholder_text(GTK_ENTRY(inputSearch), "Search");
    gtk_entry_set_max_length(GTK_ENTRY(inputSearch), 100);
    gtk_entry_set_text(GTK_ENTRY(inputSearch), "Make a search here...");
    g_signal_connect(inputSearch, "activate", G_CALLBACK(on_buttonSearch_activate), NULL);

    databaseInfo = gtk_text_buffer_new(NULL);
    gtk_text_buffer_set_text(databaseInfo, "DB :  /home/aurelien/Desktop/Epita/Project.db\nDB Size : 230 MB", -1);

    infoText = gtk_text_view_new_with_buffer(databaseInfo);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(infoText), FALSE);
    gtk_fixed_put(GTK_FIXED(fixed), infoText, 0, 700);

    engineNameImage = gtk_image_new_from_file("interface/img/image.png");
    gtk_fixed_put(GTK_FIXED(fixed), engineNameImage, (WIDTH / 2) + 10, (HEIGHT / 2) - 100);

    #pragma endregion MainWidgets

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
