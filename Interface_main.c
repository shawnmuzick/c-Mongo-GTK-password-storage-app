#include <stdio.h>
#include <stdlib.h>
#include "internal.h"

void Interface_Main_Init()
{
        GTK_error = NULL;
        gtk_init(0,NULL);

        builder = gtk_builder_new ();
        if (gtk_builder_add_from_file (builder, "window_main.glade", &GTK_error) == 0) {
                g_printerr ("Error loading file: %s\n", GTK_error->message);
                g_clear_error (&GTK_error);
                return;
        }

        window = gtk_builder_get_object (builder, "window_main");

        g_signal_connect(window, "destroy", G_CALLBACK(closeApp), NULL);

        GObject *label_welcome = gtk_builder_get_object(builder, "label_welcome");
        char string[20] = "hello ";
        strcat(string, user->username);
        gtk_label_set_text(GTK_LABEL(label_welcome), string);

        GtkGrid *grid_main = (GtkGrid *)gtk_builder_get_object(builder, "grid_main");

        //create additional fields for additional entries, subtracting the one used above
        for(int i = 0; i < user->dataLength; i++) {
                gtk_grid_insert_row(grid_main, i);
                GtkWidget *key = gtk_label_new(user->data[i].passwordKey);
                GtkWidget *value = gtk_label_new(user->data[i].passwordValue);
                gtk_widget_set_hexpand(key, TRUE);
                gtk_widget_set_vexpand(key, TRUE);

                gtk_widget_set_hexpand(value, TRUE);
                gtk_widget_set_vexpand(value, TRUE);

                gtk_grid_attach(grid_main,key, 0,i,1,1);
                gtk_grid_attach(grid_main,value, 1,i,1,1);
        }

        gtk_widget_show_all(GTK_WIDGET(window));
        gtk_main ();

}
