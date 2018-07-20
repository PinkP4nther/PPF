/*
 * Uncategorized call source
 */

#include "../headers/ppfh.h"

/* Variable value initializers */
int MODULES_FOUND = 0;
char mod_base_dir[10] = "modules";
char redstr[10] = "\e[31m";
char yellowstr[10] = "\e[33m";
char cyanstr[10] = "\e[36m";
char pinkstr[10] = "\e[35m";
char endcolor[10] = "\e[0m";
int mrf = 0;

/* Send errors here */
void ppferr(char *errmsg)
{
    printf("\e[31m[-] %s%s\n",errmsg,endcolor);
}

void ppfwarn(char *warnmsg)
{
	printf("\e[33m[!] %s%s\n",warnmsg,endcolor);
}

void ppfinfo(char *infomsg)
{
	printf("\e[36m[**] %s%s\n",infomsg,endcolor);
}

void ppfPrompt(char *mode)
{
	printf("%s[*][%s%s%s%s]>%s ",pinkstr,cyanstr,mode,endcolor,pinkstr,endcolor);
}

/* Banner */
void ppfBanner()
{
    char *buffer = 
    "[#]##############################################################[#]\n"
    "[#]                  Pinky's Pentest Framework                   [#]\n"
    "[#]                A modular pentesting framework                [#]\n"
    "[#]##############################################################[#]\n"
    "| Author: %s\n"
    "| Modules: %d\n"
    "| Version: %s\n"
    ;
    printf(buffer,PPF_AUTHOR,MODULES_FOUND,PPF_VERSION);
}

/* Displays main help */
void displayMainHelp()
{
	char helpbuf[] = 
	"[**] [PPF Shell Commands]\n"
	"  {*} help\n"
	"  {*} banner\n"
	"  {*} clear\n"
	"  {*} list\n"
	"  {*} reload\n"
	"  {*} makemod\n"
	"  {*} wield\n"
	"  {*} use\n"
	"  {*} exit\n"
	"\n[**] [Module Shell Commands]\n"
	"  {*} sync\n"
	"  {*} retreat\n"
	"  {*} run\n"
	"\n[For command usage use: 'help usage <command>']\n"
	;
	printf("%s%s%s",cyanstr,helpbuf,endcolor);
}
