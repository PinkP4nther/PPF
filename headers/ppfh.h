#ifndef PPFH_H_
#define PPFH_H_

/* Defines */
#define PPF_VERSION "0.0.1"
#define PPF_AUTHOR "@Pink_P4nther <pinkp4nther@protonmail.com>"
#define MOD_STRUCT_COUNT 100
#define MOD_NAME_SIZE 100
#define MOD_DIR_SIZE 100
#define MOD_QCOM_SIZE 10
#define MOD_DESC_SIZE 1000
#define MOD_TYPE_SIZE 100
#define MOD_CONF_SIZE 100
#define MOD_NICK_SIZE 100
#define MOD_HELP_SIZE 10000
#define MOD_HFIL_SIZE 100
#define MOD_SO_PATH 100
#define MOD_AUTHOR_SIZE 100

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <dirent.h>

/* Calls */
void ppferr(char *errmsg);
void ppfwarn(char *warnmsg);
void ppfinfo(char *infomsg);
void ppfBanner();
void lowerString(char *str, int strSize);
/* parses config for module */
void parse_module_config(int mod);
/* Parses config files lines */
void parse_conf_line(char *conf_line, char *conf);
/* Enum mods will enumerate mod dirs and put the full path of the module in the mod struct */
void enum_mods();
/* Load mods will iterate through the mods found and load them */
void loadMods();
/* Handles commands */
void cmdHandle(char *cmdStr);
/* Lists information about modules and other stuff eventually */
void ppfList(char *listCmd, char *base, char *type, char *name);
/* Shows help for module or command */
void ppfHelp(char *helpCmd, char *base, char *type, char *name);
/* Compile modules with make */
void ppfModMake(char *makeCmd, char *base, char *type, char *name);
/* Reload modules */
void ppfReload();
/* Shows basic command help */
void displayMainHelp();
/* Processes args */
void processArgs();
/* Processes module args */
void processModArgs(char *cmd, char *modName, char *args);
/* Run module */
void processModRun(char *cmd, char *modName, char *args);
/* syncs module args */
void syncArg(char *modName, char *base, char *type, char *name);
/* PPF console prompt */
void ppfPrompt(char *mode);
/* Module Menu */
void ppfModMenu(char *modCmd, char *base, char *type, char *name);

/* Variables */
int MODULES_FOUND;
char mod_base_dir[10];
char redstr[10];
char yellowstr[10];
char cyanstr[10];
char pinkstr[10];
char endcolor[10];
int mrf;

/* Structs */
struct mod_info
{
    void *modSOhandle;
    char mod_name[MOD_NAME_SIZE];
    char mod_author[MOD_AUTHOR_SIZE];
    char mod_dir[MOD_DIR_SIZE];
    char mod_type[MOD_TYPE_SIZE];
    char mod_conf[MOD_CONF_SIZE];
    char mod_nick[MOD_NICK_SIZE];
    char mod_h_file[MOD_HFIL_SIZE];
    char mod_so_path[MOD_SO_PATH];
    char mod_desc[MOD_DESC_SIZE];
    char mod_help[MOD_HELP_SIZE];
    char mod_zero;
};

struct mod_info *minfo;
//struct mod_info minfo[MOD_STRUCT_COUNT];

#endif // PPF_H_
