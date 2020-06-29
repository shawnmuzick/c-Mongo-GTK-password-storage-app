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
