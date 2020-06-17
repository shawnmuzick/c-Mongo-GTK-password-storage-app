#include <stdio.h>
#include <stdlib.h>
#include "internal.h"
#include <string.h>

/*
    16-6-2020 22:59 - this works as is, go git commit it before making any changes
*/


User *Api_Read_User(){

    User *user = malloc(sizeof(User));
    const unsigned char *str;

    cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);

    mongoc_cursor_next(cursor, &document);

    //iterate the keys of the document from what we stored above
    if (bson_iter_init(&iter, document))
    {
        while (bson_iter_next(&iter))
        {
            if(strcmp(bson_iter_key(&iter), "user") == 0){
                strcpy(user->username, bson_iter_utf8(&iter, NULL));
            }else if(strcmp(bson_iter_key(&iter), "password") == 0){
                strcpy(user->password, bson_iter_utf8(&iter, NULL));
            }else if(strcmp(bson_iter_key(&iter), "dataLength") == 0){
                user->dataLength = (uint32_t *)bson_iter_int32(&iter);
            }
        }
    }
            bson_iter_t child;
        if(bson_iter_init_find(&iter, document, "data") && BSON_ITER_HOLDS_ARRAY(&iter) && bson_iter_recurse(&iter, &child)){
            while(bson_iter_next(&child)){
                printf("found sub-key of data named %s", bson_iter_key(&child));
            }
        }
    getchar();


    return user;
}
