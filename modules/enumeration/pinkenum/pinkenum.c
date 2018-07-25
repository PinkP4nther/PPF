#include "pinkenum.h"

void pemain()
{
    char nmapCmd[100] = "nmap -sC -A -p";
    strcat(nmapCmd,ports);
    strcat(nmapCmd," ");
    strcat(nmapCmd,rhost);
    strcat(nmapCmd," >> ");
    strcat(nmapCmd,ofile);
    printf("[+] Beginning enumeration of host: %s\n",rhost);
    system(nmapCmd);
    printf("[+] Enumeration finished of host: %s\n[+] Output file: %s\n",rhost,ofile);
    return;
}
