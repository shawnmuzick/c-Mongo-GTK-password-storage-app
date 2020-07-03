#include <stdio.h>
#include <stdlib.h>
#include "internal.h"

Database *Database_create_connection()
{
        Database *d = malloc(sizeof(Database));
        mongoc_init();
        d->client = mongoc_client_new("mongodb://localhost:27017");
        d->collection = mongoc_client_get_collection(d->client, "c_DB", "c_collection");
        return d;
}


void Database_destroy_connection(Database *d)
{
        bson_destroy (d->document);
        mongoc_collection_destroy (d->collection);
        mongoc_client_destroy (d->client);
        mongoc_cleanup ();
        return;
}

bson_t *Database_Query_Build(User *s)
{
        bson_t *query = bson_new();
        BSON_APPEND_UTF8(query, "user", s->username);
        return query;
}
