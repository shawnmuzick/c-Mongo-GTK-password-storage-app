#include <stdio.h>
#include <stdlib.h>
#include "internal.h"
#include <string.h>

User *Api_Read_User(){
    //allocate space for a user struct defined in header
    User *user = malloc(sizeof(User));
    user->data = malloc(sizeof(SavedPassword) * user->dataLength);
    //create a mongo cursor to get data
    cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);
    //get data and store it in document
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
    //initialize child for use with sub array
    bson_iter_t child;
    //use initialized child to recurse into data array
    if(bson_iter_init_find(&iter, document, "data") && BSON_ITER_HOLDS_ARRAY(&iter) && bson_iter_recurse(&iter, &child)){
        //while the array has more entries, step forward
        while(bson_iter_next(&child)){
            //initialize an element
            bson_iter_t element;
            //recurse into it
            bson_iter_recurse(&child, &element);
            //grab it's values
            while(bson_iter_next(&element)){
                strcpy(user->data[0].passwordKey, bson_iter_key(&element));
                strcpy(user->data[0].passwordValue, bson_iter_utf8(&element, NULL));
            }
        }

    }
    //debug -- check values to make sure they copied correctly to the struct
    /*
    printf("%s\n", user->username);
    printf("%s\n", user->password);
    printf("datalength %d\n", user->dataLength);
    printf("data[0]: %s\t%s\n", user->data[0].passwordKey, user->data[0].passwordValue);
    getchar();*/

    //free allocations
    bson_destroy(query);
    mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(collection);
    mongoc_cleanup();
    return user;
}
