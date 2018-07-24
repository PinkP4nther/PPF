#include "shell.h"

void shell()
{
    if (!msg)
    {
	    printf("[+] Argument 'msg' not set\n");
	    return;
    }
    printf("[+++] %s!!!\n",msg);
    system("/bin/bash");
}
