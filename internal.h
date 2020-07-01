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

typedef struct{
        GObject *username_entry;
        GObject *password_entry;
        GObject *password_retype;
}Credential;

//-------------------
bool validated;

//Mongo----------------------------------------------------
mongoc_database_t *database;
mongoc_client_t *client;
mongoc_collection_t *collection;
mongoc_cursor_t *cursor;

//BSON-----------------------------------------------------
bson_t *b;
bson_t *query;
bson_iter_t iter;
const bson_t *document;
bson_error_t error;

//API-------------------------------------------------------
//CRUD
bool Api_Create_User(User *a);
User *Api_Read_User();
User *Api_Update_User();
User *Api_Delete_User(User *a);

//Database
void Database_Init();
void Database_Query_Build(User *s);

//User
bool User_Validate(User *a, User *b);
bool user_register_password_match(Credential *c);

//Interface
void Interface_Login_Init(int argc, char *argv[]);
void Interface_Main_Init(int argc, char *argv[]);

#endif // INTERNAL_H_INCLUDED

