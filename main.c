#include <gtk/gtk.h>
#include <mongoc.h>
#include "internal.h"

int main()
{
        //initialize database
        Database_Init();

        //initialize login interface
        Interface_Login_Init(0, NULL);

        //initialize main app interface
        if(validated)Interface_Main_Init(0, NULL);
        else printf("not validated");

        return 0;
}
