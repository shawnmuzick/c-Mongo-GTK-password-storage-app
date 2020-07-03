#include <stdio.h>
#include <stdlib.h>
#include "internal.h"
bool User_Validate(User *a, User *b)
{
        bool validated = false;

        if(strcmp(a->password, b->password) == 0) validated = true;
        else validated = false;
        return validated;
}
bool user_register_password_match(Credential *c)
{
        if(strcmp(gtk_entry_get_text(GTK_ENTRY(c->password_entry)), gtk_entry_get_text(GTK_ENTRY(c->password_retype))) == 0) {
                return true;
        } else {
                return false;
        }


}
