#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "internal.h"
#include <string.h>
#include <stdbool.h>
User *user;

//GTK-----------------------------------------------------
GObject *button;
GObject *username_entry, *password_entry;
GError *GTK_error;
GtkBuilder *builder;
GObject *window;

User *get_Submission()
{
    User *submission = malloc(sizeof(User));
    strcpy(submission->username, gtk_entry_get_text(GTK_ENTRY((GtkWidget *)username_entry)));
    strcpy(submission->password, gtk_entry_get_text(GTK_ENTRY((GtkWidget *)password_entry)));
    return submission;
}


void closeApp(GObject *window, bool validated)
{
    gtk_main_quit();
}

void closeWindow(GObject *window){
    gtk_window_close(GTK_WINDOW(window));
}
//login click callback function
void login_button_clicked(GObject *button, gpointer data)
{

    GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;

    //build a user model from the input fields
    User *submission = get_Submission();

    //build database query from the submission
    Database_Query_Build(submission);

    //set up a new user
    user  = Api_Read_User();

    validated = User_Validate(user, submission);
    char message[20];
    if(!validated){
        strcpy(message, submission->password);
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window), flags, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "%s",message);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }else{
        closeWindow(window);
    }

    free(submission);
}

//initialize a login UI
void Interface_Login_Init(int argc, char *argv[]){

    GTK_error = NULL;

    gtk_init(&argc, &argv);

      /* Construct a GtkBuilder instance and load our UI description */
    builder = gtk_builder_new ();
    if (gtk_builder_add_from_file (builder, "window_login.glade", &GTK_error) == 0)
    {
      g_printerr ("Error loading file: %s\n", GTK_error->message);
      g_clear_error (&GTK_error);
      return;
    }

    window = gtk_builder_get_object (builder, "window_login");

    g_signal_connect(window, "destroy", G_CALLBACK(closeApp), NULL);

    username_entry = gtk_builder_get_object(builder, "username_input");
    password_entry = gtk_builder_get_object(builder, "password_input");

    button = gtk_builder_get_object(builder, "btn_login");
    GObject *btn_new_user = gtk_builder_get_object(builder, "btn_new_user");


    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(login_button_clicked), password_entry);

    gtk_widget_show_all(GTK_WIDGET(window));
    //render the interface
    gtk_main ();
}
//initialize the main UI
void Interface_Main_Init(int argc, char *argv[]){
    GTK_error = NULL;

    gtk_init(&argc, &argv);

      /* Construct a GtkBuilder instance and load our UI description */
    builder = gtk_builder_new ();
    if (gtk_builder_add_from_file (builder, "window_main.glade", &GTK_error) == 0)
    {
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
    for(int i = 0; i < user->dataLength; i++){
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
    //render the interface
    gtk_main ();

}
