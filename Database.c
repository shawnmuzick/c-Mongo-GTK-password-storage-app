#include <stdio.h>
#include <stdlib.h>
#include "internal.h"

void Database_Init(){

    mongoc_init();
    client = mongoc_client_new("mongodb://localhost:27017");
    collection = mongoc_client_get_collection(client, "c_DB", "c_collection");
}

void Database_Query_Build(User *s){
   query = bson_new();
    BSON_APPEND_UTF8(query, "user", s->username);
}
