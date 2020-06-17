#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "internal.h"
#include <string.h>
#include <stdbool.h>
User *user;

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
    free(user);
}


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
    button = gtk_builder_get_object(builder, "submit_btn");
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);

    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(login_button_clicked), password_entry);

    gtk_widget_show_all(GTK_WIDGET(window));
    //render the interface
    gtk_main ();
}

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

    GObject *welcome = gtk_builder_get_object(builder, "welcome_label");
    gtk_label_set_text(GTK_LABEL(welcome), user->data[0].passwordValue);
    gtk_widget_show_all(GTK_WIDGET(window));
    //render the interface
    gtk_main ();

}
