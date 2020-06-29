#include <gtk/gtk.h>
#include <mongoc.h>
#include "internal.h"

int main(int argc, char *argv[])
{
        //initialize database
        Database_Init();

        //initialize login interface
        Interface_Login_Init(argc, argv);

        //initialize main app interface
        if(validated)Interface_Main_Init(argc, argv);
        else printf("not validated");

        return 0;
}
