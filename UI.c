#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "internal.h"
#include <string.h>
#include <stdbool.h>

void closeWindow(GObject *window);
void closeApp(GObject *window, bool validated);
void register_new_user_button_clicked(GObject *button, Credential *c);
void login_button_clicked(GObject *button, Credential *c);
void new_user_btn_clicked(GObject *btn);

User *get_Submission();
User *get_Submission(Credential *c);

User *get_Submission(Credential *c)
{
        User *submission = malloc(sizeof(User));
        strcpy(submission->username, gtk_entry_get_text(GTK_ENTRY(c->username_entry)));
        strcpy(submission->password, gtk_entry_get_text(GTK_ENTRY(c->password_entry)));
        return submission;
}

void register_new_user_button_clicked(GObject *button, Credential *c)
{
        Database *d = Database_create_connection();
        bool password_match = user_register_password_match(c);
        if(password_match) {
                User *submission = get_Submission(c);
                submission->dataLength = 1;
                bool created = Api_Create_User(submission, d);
                if(created) {
                        closeWindow(window);
                        Database_destroy_connection(d);
                        Interface_Login_Init(0, NULL);
                } else {
                        printf("%d", created);
                        Interface_Login_Init(0, NULL);
                }
        } else {
                printf("passwords do not match!\n");
        }

}

void closeApp(GObject *window, bool validated)
{
        gtk_main_quit();
}

void closeWindow(GObject *window)
{
        gtk_window_close(GTK_WINDOW(window));
}
//login click callback function
void login_button_clicked(GObject *button, Credential *c)
{
        Database *d = Database_create_connection();
        GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;

        //build a user model from the input fields
        User *submission = get_Submission(c);

        //build database query from the submission
        bson_t *query = Database_Query_Build(submission);

        //set up a new user
        user  = Api_Read_User(query, d);

        bool validated = User_Validate(user, submission);
        char message[20];
        if(!validated) {
                strcpy(message, submission->password);
                GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window), flags, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "%s",message);
                gtk_dialog_run(GTK_DIALOG(dialog));
                gtk_widget_destroy(dialog);
                Database_destroy_connection(d);
        } else {
                closeWindow(window);
                Database_destroy_connection(d);
                Interface_Main_Init(0,NULL);
        }

        free(submission);
}

void new_user_btn_clicked(GObject *btn)
{
        closeWindow(window);
        Interface_Register_Init();
}

//initialize a login UI
void Interface_Login_Init()
{
        GTK_error = NULL;
        gtk_init(0,NULL);

        /* Construct a GtkBuilder instance and load our UI description */
        builder = gtk_builder_new ();
        if (gtk_builder_add_from_file (builder, "window_login.glade", &GTK_error) == 0) {
                g_printerr ("Error loading file: %s\n", GTK_error->message);
                g_clear_error (&GTK_error);
                return;
        }

        window = gtk_builder_get_object (builder, "window_login");

        g_signal_connect(window, "destroy", G_CALLBACK(closeApp), NULL);

        GObject *username_entry = gtk_builder_get_object(builder, "username_input");
        GObject *password_entry = gtk_builder_get_object(builder, "password_input");

        GObject *button = gtk_builder_get_object(builder, "btn_login");
        GObject *btn_new_user = gtk_builder_get_object(builder, "btn_new_user");

        Credential *credential = malloc(sizeof(Credential));
        credential->password_entry = password_entry;
        credential->username_entry = username_entry;

        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(login_button_clicked), credential);
        g_signal_connect(G_OBJECT(btn_new_user), "clicked", G_CALLBACK(new_user_btn_clicked), NULL);

        gtk_widget_show_all(GTK_WIDGET(window));
        //render the interface
        gtk_main ();
}

void Interface_Register_Init()
{
        GTK_error = NULL;

        gtk_init(0, NULL);

        // Construct a GtkBuilder instance and load our UI description
        builder = gtk_builder_new ();
        if (gtk_builder_add_from_file (builder, "window_register.glade", &GTK_error) == 0) {
                g_printerr ("Error loading file: %s\n", GTK_error->message);
                g_clear_error (&GTK_error);
                return;
        }

        window = gtk_builder_get_object (builder, "window_register");

        g_signal_connect(window, "destroy", G_CALLBACK(closeApp), NULL);
        GObject *username_entry = gtk_builder_get_object(builder, "username_input");
        GObject *password_entry = gtk_builder_get_object(builder, "password_input");
        GObject *password_retype = gtk_builder_get_object(builder, "password_check_input");
        GObject *button = gtk_builder_get_object(builder, "btn_register");

        Credential *credential = malloc(sizeof(Credential));
        credential->password_entry = password_entry;
        credential->username_entry = username_entry;
        credential->password_retype = password_retype;

        g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(register_new_user_button_clicked), credential);

        gtk_widget_show_all(GTK_WIDGET(window));
        //render the interface
        gtk_main ();
}
