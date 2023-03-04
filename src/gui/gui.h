/*
 * Author: Izoulet Aurélien
 * Purpose: GUI Hermes implementation
 * Language: C.
 */

#ifndef _GUI_H_
#define _GUI_H_

#include <gtk/gtk.h>
#include <string.h>

// Contains all signals from the main gui window. 

/**
** @brief             Signals handlers methods. 
** @param widget      Widget that triggered the signal. 
** @param userdata    Current userdata.
** @return            Void.
*/

void on_buttonSave_activate(GtkWidget *widget, gpointer *userdata);

void on_buttonReload_activate(GtkWidget *widget, gpointer *userdata);

void on_buttonExit_activate(GtkWidget *widget, gpointer *userdata);

void on_buttonOpenDB_activate(GtkWidget *widget, gpointer *userdata);

void on_buttonUpdateDB_activate(GtkWidget *widget, gpointer *userdata);

void on_buttonRepairDB_activate(GtkWidget *widget, gpointer *userdata);

void on_buttonThreads_activate(GtkWidget *widget, gpointer *userdata);

void on_buttonTimeout_activate(GtkWidget *widget, gpointer *userdata);

void on_buttonInternetStatus_activate(GtkWidget *widget, gpointer *userdata);

void on_buttonProjectTeam_activate(GtkWidget *widget, gpointer *userdata);

void on_buttonWebsite_activate(GtkWidget *widget, gpointer *userdata);
 
void on_inputSearch_activate(GtkWidget *widget, gpointer *userdata);

// Contains all tools used by the main gui window. 

/**
** @brief             Open a dialog and override current database path. 
** @return            Void.
*/

void open_db();

#endif  