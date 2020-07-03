#include <mongoc.h>
#include <gtk/gtk.h>
#include <stdbool.h>
#ifndef INTERNAL_H_INCLUDED
#define INTERNAL_H_INCLUDED
//Structs--------------------------------------------------

typedef struct {
        char passwordKey[10];
        char passwordValue[20];
} SavedPassword;


typedef struct {
        char username[20];
        char password[20];
        int dataLength;
        SavedPassword *data;
} User;

typedef struct {
        GObject *username_entry;
        GObject *password_entry;
        GObject *password_retype;
} Credential;

typedef struct {
        mongoc_client_t *client;
        mongoc_collection_t *collection;
        mongoc_cursor_t *cursor;
        bson_t *document;
} Database;

User *user;

//GTK-----------------------------------------------------
//GObject *button;
GError *GTK_error;
GtkBuilder *builder;
GObject *window;

//BSON-----------------------------------------------------
bson_t *b;
bson_iter_t iter;
//const bson_t *document;
bson_error_t error;

//API-------------------------------------------------------
//CRUD
bool Api_Create_User(User *a, Database *d);
User *Api_Read_User(bson_t *query, Database *d);
User *Api_Update_User();
User *Api_Delete_User(User *a);

//Database
Database *Database_create_connection();
void Database_destroy_connection();
bson_t *Database_Query_Build(User *s);

//User
bool User_Validate(User *a, User *b);
bool user_register_password_match(Credential *c);

//Interface
void Interface_Login_Init();
void Interface_Main_Init();
void Interface_Register_Init();

void closeApp(GObject *window, bool validated);

#endif // INTERNAL_H_INCLUDED

