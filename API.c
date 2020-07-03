#include <stdio.h>
#include <stdlib.h>
#include "internal.h"
#include <string.h>
#include <stdbool.h>

User *Api_Read_User(bson_t *query, Database *d)
{

        User *user = malloc(sizeof(User));
        d->cursor = mongoc_collection_find_with_opts(d->collection, query, NULL, NULL);
        mongoc_cursor_next(d->cursor, &d->document);
        //iterate the keys of the document
        if (bson_iter_init(&iter, d->document)) {
                //while the document has more keys, step forward
                while (bson_iter_next(&iter)) {
                        //saves a username and password to the struct
                        if(strcmp(bson_iter_key(&iter), "user") == 0) {
                                //printf("found user!\n");
                                strcpy(user->username, bson_iter_utf8(&iter, NULL));
                        } else if(strcmp(bson_iter_key(&iter), "password") == 0) {
                                strcpy(user->password, bson_iter_utf8(&iter, NULL));
                        } else if(strcmp(bson_iter_key(&iter), "dataLength") == 0) {
                                printf("extracted data length %d\n", bson_iter_int32(&iter));
                                user->dataLength = bson_iter_int32(&iter);
                        }
                }
        }

        //set password array based on size variable
        user->data = malloc(sizeof(SavedPassword) * user->dataLength);

        //initialize child for use with sub array
        bson_iter_t child;
        //use initialized child to recurse into data array
        if(bson_iter_init_find(&iter, d->document, "data") && BSON_ITER_HOLDS_ARRAY(&iter) && bson_iter_recurse(&iter, &child)) {
                int i = 0;
                while(bson_iter_next(&child)) {

                        //initialize an element
                        bson_iter_t element;
                        //recurse into it
                        bson_iter_recurse(&child, &element);

                        bson_iter_next(&element);
                        strcpy(user->data[i].passwordKey, bson_iter_key(&element));
                        strcpy(user->data[i].passwordValue, bson_iter_utf8(&element, NULL));

                        i++;
                }

        }

        return user;
}

bool Api_Create_User(User *submission, Database *d)
{

        bson_t *arr;
        bson_oid_t oid;
        bson_t *child;
        bson_t *doc;

        doc = bson_new();
        arr = bson_new();
        child = bson_new();

        bson_oid_init(&oid, NULL);
        BSON_APPEND_OID(doc, "_id", &oid);
        BSON_APPEND_UTF8(doc, "user", submission->username);
        BSON_APPEND_UTF8(doc, "password", submission->password);
        //initially set this to 1
        BSON_APPEND_INT32(doc, "dataLength", 1);

        //add initial data to child
        BSON_APPEND_UTF8(child, "Begin", "Here");
        //append child as first element of array
        BSON_APPEND_DOCUMENT(arr, "0", child);
        //append array to document as array
        bson_append_array(doc, "data", -1,arr);

        if (!mongoc_collection_insert_one (
                                d->collection, doc, NULL, NULL, &error)) {
                fprintf (stderr, "%s\n", error.message);
                return false;
        }

        return true;
}
