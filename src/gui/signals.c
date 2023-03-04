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
}


