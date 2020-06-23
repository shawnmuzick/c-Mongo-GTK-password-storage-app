#include <stdio.h>
#include <stdlib.h>
#include "internal.h"
#include <string.h>

User *Api_Read_User(){

    User *user = malloc(sizeof(User));

    cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);
    mongoc_cursor_next(cursor, &document);

    //iterate the keys of the document
    if (bson_iter_init(&iter, document))
    {
        //while the document has more keys, step forward
        while (bson_iter_next(&iter))
        {
            //saves a username and password to the struct
            if(strcmp(bson_iter_key(&iter), "user") == 0){
                strcpy(user->username, bson_iter_utf8(&iter, NULL));
            }else if(strcmp(bson_iter_key(&iter), "password") == 0){
                strcpy(user->password, bson_iter_utf8(&iter, NULL));
            }else if(strcmp(bson_iter_key(&iter), "dataLength") == 0){
                user->dataLength = bson_iter_int32(&iter);
            }
        }
    }
    //set password array based on size variable
    user->data = malloc(sizeof(SavedPassword) * user->dataLength);

    //initialize child for use with sub array
    bson_iter_t child;
    //use initialized child to recurse into data array
    if(bson_iter_init_find(&iter, document, "data") && BSON_ITER_HOLDS_ARRAY(&iter) && bson_iter_recurse(&iter, &child)){
        int i = 0;
        while(bson_iter_next(&child)){

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

    //free allocations
    bson_destroy(query);
    mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(collection);
    mongoc_cleanup();
    return user;
}

User *Api_Create_User(User *submission){
    User *user = malloc(sizeof(User));

    return user;
}
