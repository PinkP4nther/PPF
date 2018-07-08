#include "shell.h"

char test[100];

void shell()
{
    printf("[+++] %s!!!\n",test);
    system("bash");
}
