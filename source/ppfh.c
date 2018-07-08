#include "../headers/ppfh.h"

/* Variable value initializers */
int MODULES_FOUND = 0;
int mreloadFlag = 0;
char mod_base_dir[10] = "modules";
char redstr[10] = "\e[31m";
char yellowstr[10] = "\e[33m";
char cyanstr[10] = "\e[36m";
char pinkstr[10] = "\e[35m";
char endcolor[10] = "\e[0m";

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

/* Parse line of ppf config files */
void parse_conf_line(char *conf_line, char *conf)
{
	char line_buf[strlen(conf_line)+1];
	sscanf(conf_line, "%s %[^\n]s", line_buf, conf);
}

/* Parse config files */
void parse_module_config(int mod)
{
	char fbuf[1000];
	FILE *fd;

	if ((fd=fopen(minfo[mod].mod_conf,"r")) == NULL)
	{
		printf("%s[!] Couldn't open configuration file %s%s\n",yellowstr,minfo[mod].mod_conf,endcolor);
		return;
	}

	while (!feof(fd))
	{
		fgets(fbuf, 100, fd);

		if (fbuf[0] == '#' || strlen(fbuf) < 4)
		{
			continue;
		}

		if (strstr(fbuf, "MOD_NICKNAME "))
		{
			parse_conf_line(fbuf,minfo[mod].mod_nick);
		}

		if (strstr(fbuf, "MOD_DESC "))
		{
			parse_conf_line(fbuf,minfo[mod].mod_desc);
		}

		if (strstr(fbuf, "MOD_SO_PATH "))
		{
			parse_conf_line(fbuf,minfo[mod].mod_so_path);
		}

		if (strstr(fbuf, "MOD_AUTHOR "))
		{
			parse_conf_line(fbuf,minfo[mod].mod_author);
		}

	}
	fclose(fd);
}

/* Enumerate module type for modules */
void type_enum(struct dirent *type_dir)
{
	DIR *dir_point;
	struct dirent *mod_ent;
	char temp_path[100] = "modules/";
	strcat(temp_path,type_dir->d_name);
	
	if ((dir_point = opendir(temp_path)) == NULL)
	{
		ppferr("Couldn't open module type directory");
		exit(1);
	}

	for (int i = 0; i < MOD_STRUCT_COUNT; i++)
	{
		if (i != 0)
		{
			strncpy(temp_path,"modules/",100);
			strcat(temp_path,type_dir->d_name);
		}

		if ((mod_ent = readdir(dir_point)) == NULL)
		{
			closedir(dir_point);
			return;
		}

		if (strcmp(mod_ent->d_name,".") == 0 || strcmp(mod_ent->d_name, "..") == 0)
		{
			continue;
		}
		
		strcat(temp_path,"/");
		strcat(temp_path,mod_ent->d_name);
		strncpy(minfo[MODULES_FOUND].mod_dir,temp_path,MOD_DIR_SIZE);

		strncpy(minfo[MODULES_FOUND].mod_name,mod_ent->d_name,MOD_NAME_SIZE);

		strncpy(minfo[MODULES_FOUND].mod_type,type_dir->d_name,MOD_TYPE_SIZE);

		char modh_buf[MOD_DIR_SIZE];
		strncpy(modh_buf,temp_path,MOD_DIR_SIZE);
		strcat(modh_buf,"/");
		strcat(modh_buf,mod_ent->d_name);
		strcat(modh_buf,".ppfhelp");
		strncpy(minfo[MODULES_FOUND].mod_h_file,modh_buf,MOD_HFIL_SIZE);

		char modso_buf[MOD_SO_PATH];
		strncpy(modso_buf,temp_path,MOD_SO_PATH);
		strcat(modso_buf,"/");
		strcat(modso_buf,mod_ent->d_name);
		strcat(modso_buf,".so");
		strncpy(minfo[MODULES_FOUND].mod_so_path,modso_buf,MOD_SO_PATH);
/*
		char modd_buf[MOD_DESC_PATH];
		strncpy(modd_buf,temp_path,MOD_DESC_PATH);
		strcat(modd_buf,"/");
		strcat(modd_buf,mod_ent->d_name);
		strcat(modd_buf,".ppfdesc");
		strncpy(minfo[MODULES_FOUND].mod_desc_path,modd_buf,MOD_DESC_PATH);
*/
		char modc_buf[MOD_CONF_SIZE];
		strncpy(modc_buf,temp_path,MOD_CONF_SIZE);
		strcat(modc_buf,"/");
		strcat(modc_buf,mod_ent->d_name);
		strcat(modc_buf,".ppfconf");
		strncpy(minfo[MODULES_FOUND].mod_conf,modc_buf,MOD_CONF_SIZE);

		/* Put file reading stuff after this? */
		parse_module_config(MODULES_FOUND);
/*
		int oFD;
		ssize_t br;
		if ((oFD = open(minfo[MODULES_FOUND].mod_desc_path,O_RDONLY)) == -1)
		{
			printf("%s[!] Couldn't open description file %s%s\n",yellowstr,minfo[MODULES_FOUND].mod_desc_path,endcolor);
			MODULES_FOUND++;
			continue;
		}
		if ((br = read(oFD,minfo[MODULES_FOUND].mod_desc,MOD_DESC_SIZE)) == -1)
		{
			printf("%s[!] Couldn't read description file %s%s\n",yellowstr,minfo[MODULES_FOUND].mod_desc_path,endcolor);
			MODULES_FOUND++;
			continue;
		}
		if (close(oFD) == -1)
		{
			printf("%s[!] Couldn't close file %s from module %s%s\n",yellowstr,minfo[MODULES_FOUND].mod_desc_path,minfo[MODULES_FOUND].mod_name,endcolor);
		}
*/

		MODULES_FOUND++;

	}
	
}

/* Enumerate modules */
void enum_mods()
{
	DIR *dirp;
	struct dirent *dp;
	if ((dirp = opendir(mod_base_dir)) == NULL)
	{
		ppferr("Couldn't open modules/ directory");
		exit(1);
	}

	for (;;)
	{
		if ((dp = readdir(dirp)) == NULL)
			break;
		
		if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
			continue;
		
		type_enum(dp);
	}
	closedir(dirp);
	
}

/* Load modules */
void loadMods()
{

	minfo = (struct mod_info*)malloc(MOD_STRUCT_COUNT * sizeof(*minfo));
	*minfo = (struct mod_info){0};

	enum_mods();

	/* Eventually when ppf config is made add option for loading module SO's on load
	* or when they are ran then unload them after execution (will save memory)
	*/
	/**
	for (int i = 0; i < MODULES_FOUND; i++)
	{
		// Get handles for each module put into handle minfo for parsing commands iterate using
		//	dlsym and checking for NULL or address returned then execute
		minfo[i].modSOhandle = dlopen(minfo[i].mod_so_path, RTLD_LAZY);
		if (minfo[i].modSOhandle == NULL)
		{
			printf("%s[!] Couldn't open shared object file %s for module %s%s\n",yellowstr,minfo[i].mod_so_path,minfo[i].mod_name,endcolor);
		}
		if (mreloadFlag == 1)
		{
			printf("%s[**] Reloaded module: %s%s\n",cyanstr,minfo[i].mod_name,endcolor);
		}
	}*/
}

/* Command Parsing */
void cmdHandle(char *cmdStr)
{
	void (*cmdPointer)(void);
	/* Iterate through modules and attempt to resolve command */
	for (int i = 0; i < MODULES_FOUND; i++)
	{
		/* Implement quick access here auto loads then resolves symbols and executes then
		* unloads */
		/* Find way to make module menu with args and a run command that executes (using dlsym to look for global variables and call names) */
		//minfo[i].modSOhandle = dlopen(minfo[i].mod_so_path, RTLD_LAZY);
		if ((*(void**)(&cmdPointer) = dlsym(minfo[i].modSOhandle,cmdStr)) == NULL)
		{
			//dlclose(minfo[i].modSOhandle);
			continue;
		}
		else
		{
			(*cmdPointer)();
			//dlclose(minfo[i].modSOhandle);
			break;
		}
	}
	ppfwarn("Not a command. Use 'help' for commands.");

}

/* Process command args */
void processArgs(char *cmd, char *args)
{
	char base[10];
	char type[10];
	char name[10];
	char origin_cmd[100];
	strncpy(origin_cmd,args,100);
	char *ncmd = args;
	char *token;
	char *rest = ncmd;
	int i = 0;
	while((token = strtok_r(rest," ",&rest)))
	{
		if (i==0)
			strcpy(base,token);
		if (i==1)
			strcpy(type,token);
		if (i==2)
			strcpy(name,token);
		i++;
	}
	if (strcmp(cmd,"help") == 0)
	{
		ppfHelp(origin_cmd,base,type,name);
		return;
	}
	else if (strcmp(cmd,"list") == 0)
	{
		ppfList(origin_cmd,base,type,name);
		return;
	}
	else if (strcmp(cmd,"wield") == 0)
	{
		ppfModMenu(origin_cmd,base,type,name);
		return;
	}
}

/* Sync argument */
void syncArg(char *modName, char *base, char *type, char *name)
{
	char *opt;
	const char *err;
	for (int i = 0; i <MODULES_FOUND; i++)
	{
		if (strcmp(modName,minfo[i].mod_name) == 0)
		{

			opt = (char *) dlsym(minfo[i].modSOhandle,type);
			err = dlerror();
			if (err != NULL)
			{
				ppfwarn("Argument does not exist");
				printf("[-] %s\n",err);
				return;
			}
			strcpy(opt,name);
			printf("%s[**] %s: [%s]%s\n",cyanstr,type,name,endcolor);
			return;
		}
	}
}

/* Process module args */
void processModArgs(char *cmd, char *modName, char *args)
{
	char base[10];
	char type[10];
	char name[10];
	char origin_cmd[100];
	strncpy(origin_cmd,args,100);
	char *ncmd = args;
	char *token;
	char *rest = ncmd;
	int i = 0;
	while((token = strtok_r(rest," ",&rest)))
	{
		if (i==0)
			strcpy(base,token);
		if (i==1)
			strcpy(type,token);
		if (i==2)
			strcpy(name,token);
		if (i>2)
		{
			strcat(name," ");
			strcat(name,token);
		}
		i++;
	}
	if (strcmp(cmd,"sync") == 0)
	{
		syncArg(modName,base,type,name);
		return;
	}

}

/* Runs module */
void processModRun(char *cmd, char *modName, char *args)
{
	char base[10];
	char name[10];
	char origin_cmd[100];
	strncpy(origin_cmd,args,100);
	char *ncmd = args;
	char *token;
	char *rest = ncmd;
	int i = 0;
	while((token = strtok_r(rest," ",&rest)))
	{
		if (i==0)
			strcpy(base,token);
		if (i==1)
			strcpy(name,token);
		i++;
	}
	for (int i = 0; i < MODULES_FOUND; i++)
	{
		if (strcmp(modName,minfo[i].mod_name) == 0)
		{
			void (*callptr)(void);
			const char *err;
			*(void **) (&callptr) = dlsym(minfo[i].modSOhandle,name);
			err = dlerror();
			if (err != NULL)
			{
				ppfwarn("Call does not exist");
				return;
			}
			(*callptr)();
			return;
		}
	}
}

/* Loads module up and shows module menu */
/* Should allow to call specific calls and a normal default call name */
void ppfModMenu(char *modCmd, char *base, char *type, char *name)
{
	for (int i = 0; i < MODULES_FOUND; i++)
	{
		if (strcmp(type,minfo[i].mod_name) == 0)
		{
			minfo[i].modSOhandle = dlopen(minfo[i].mod_so_path, RTLD_LAZY);
			if (minfo[i].modSOhandle == NULL)
			{
				printf("%s[!] Couldn't open shared object file %s for module %s%s\n",yellowstr,minfo[i].mod_so_path,minfo[i].mod_name,endcolor);
				return;
			}
			(void) dlerror();
			/* Module is loaded, continue */
			char mCmd[100];
			int running = 1;
			while (running)
			{
				ppfPrompt(minfo[i].mod_nick);
				if (fgets(mCmd,sizeof(mCmd),stdin) != NULL)
				{
					mCmd[strcspn(mCmd,"\n")] = '\0';

					if (strncmp(mCmd,"retreat",7) == 0)
					{
						dlclose(minfo[i].modSOhandle);
						return;
					}
					else if (strncmp(mCmd,"sync",4) == 0)
					{
						processModArgs("sync",minfo[i].mod_name,mCmd);
					}
					else if (strcmp("reload",mCmd) == 0)
					{
						dlclose(minfo[i].modSOhandle);
						ppfinfo("Leaving module");
						ppfReload();
						return;
					}
					else if (strncmp(mCmd,"exit",4) == 0)
					{
						for (int i = 0; i < MODULES_FOUND; i++)
						{
							if (minfo[i].modSOhandle != NULL)
								dlclose(minfo[i].modSOhandle);
						}
						free(minfo);
						minfo = NULL;
						ppfinfo("Exiting PPF!");
						exit(0);
					}
					else if (strncmp(mCmd,"wield",5) == 0)
					{
						ppfinfo("Must leave module to wield another");
					}
					else if (strncmp("clear",mCmd,5) == 0)
					{
						printf("\033[H\033[J");
					}
					else if (strncmp("banner",mCmd,6) == 0)
					{
						printf("\033[H\033[J");
						ppfBanner();
					}
					else if (strncmp(mCmd,"run",3) == 0)
					{
						processModRun("run",minfo[i].mod_name,mCmd);
					}
					else if (strncmp(mCmd,"help",4) == 0)
					{
						char modhelpbuf[100];
						strncpy(modhelpbuf,"help module ",13);
						strcat(modhelpbuf,minfo[i].mod_name);
						processArgs("help",mCmd);
						printf("\n%s[**] Module specific help%s\n",cyanstr,endcolor);
						processArgs("help",modhelpbuf);
					}
					else if (strncmp(mCmd,"list",4) == 0)
					{
						processArgs("list",mCmd);
					}
					else
					{
						ppfwarn("Not a command. Use 'help' for commands.");
					}
				}
			}
		}
	}
}

/* list info for modules and other things eventually */
void ppfList(char *listCmd, char *base, char* type, char *name)
{
	if (strcmp(type,"modules") == 0)
	{
		printf("%s[**] Loaded Modules\t\tDescription",cyanstr);
		for (int i = 0; i < MODULES_FOUND; i++)
		{
			printf("\n  {%d} %s\t\t\t%s",i+1,minfo[i].mod_name,minfo[i].mod_desc);
		}
		printf("%s\n",endcolor);
		return;
	}
	else if (strcmp(type,"info") == 0)
	{
		for (int i = 0; i < MODULES_FOUND; i++)
		{
			if (strcmp(minfo[i].mod_name,name) == 0)
			{
				printf("%s[+] Module information for %s\n",cyanstr,minfo[i].mod_name);
				printf("  Name: %s\n",minfo[i].mod_name);
				printf("  Author: %s\n",minfo[i].mod_author);
				printf("  Nickname: %s\n",minfo[i].mod_nick);
				printf("  Type: %s\n",minfo[i].mod_type);
				printf("  Directory: %s\n",minfo[i].mod_dir);
				printf("  Configuration File: %s\n",minfo[i].mod_conf);
				printf("  Help File: %s\n",minfo[i].mod_h_file);
				printf("  SO Path: %s\n",minfo[i].mod_so_path);
				printf("  Description:\n    %s",minfo[i].mod_desc);
				printf("%s\n",endcolor);
				return;
			}
		}
		printf("%s[!] Module %s doesn't exist%s\n",yellowstr,name,endcolor);
	}
	else
	{
		printf("%s[!] Invalid argument: %s%s\n",yellowstr,type,endcolor);
	}
}

/* Show help for specified module or object */
void ppfHelp(char *helpCmd, char *base, char *type, char *name)
{
	if (strcmp(helpCmd,"help") == 0 || strcmp(helpCmd,"help ") == 0)
	{
		displayMainHelp();
		return;
	}
	
	if (strcmp(type,"usage") == 0)
	{
		if (strcmp(name,"help") == 0)
		{
			char buf[] =
			"[Help]\n"
			"\n"
			"Args:\n"
			"  usage - gives usage of a ppf command\n"
			"  module - displays help for a loaded module\n"
			"\nUsage: help <arg> <name>\n"
			;
			printf("%s%s%s",cyanstr,buf,endcolor);
			return;
		}
		else if (strcmp(name,"list") == 0)
		{
			char buf[] =
			"[List]\n"
			"\n"
			"Args:\n"
			"  info - gives information about a module\n"
			"  modules - lists all loaded modules\n"
			"\nUsage: list <arg> <name>\n"
			;
			printf("%s%s%s",cyanstr,buf,endcolor);
			return;
		}
		else if (strcmp(name,"reload") == 0)
		{
			char buf[] =
			"[Reload]\n"
			"\n"
			"  reload - reloads all modules\n"
			"\nUsage: reload\n"
			;
			printf("%s%s%s",cyanstr,buf,endcolor);
			return;
		}
		else if (strcmp(name,"clear") == 0)
		{
			char buf[] =
			"[Clear]\n"
			"\n"
			"  clear - clears the screen\n"
			"\nUsage: clear\n"
			;
			printf("%s%s%s",cyanstr,buf,endcolor);
			return;
		}
		else if (strcmp(name,"banner") == 0)
		{
			char buf[] =
			"[Banner]\n"
			"\n"
			"  banner - clears screen and displays banner\n"
			"\nUsage: banner\n"
			;
			printf("%s%s%s",cyanstr,buf,endcolor);
			return;
		}
		else if (strcmp(name,"wield") == 0)
		{
			char buf[] =
			"[Wield]\n"
			"\n"
			"  wield - loads a module\n"
			"\nUsage: wield <module>\n"
			;
			printf("%s%s%s",cyanstr,buf,endcolor);
		}
		else if (strcmp(name,"sync") == 0)
		{
			char buf[] =
			"[Sync]\n"
			"\n"
			"  sync - sets the value of a module parameter\n"
			"\nUsage: sync <arg> <value>\n"
			;
			printf("%s%s%s",cyanstr,buf,endcolor);
		}
		else if (strcmp(name,"retreat") == 0)
		{
			char buf[] =
			"[Retreat]\n"
			"\n"
			"  retreat - backs out of the module shell\n"
			"\nUsage: retreat\n"
			;
			printf("%s%s%s",cyanstr,buf,endcolor);
		}
		else if (strcmp(name,"run") == 0)
		{
			char buf[] =
			"[Run]\n"
			"\n"
			"  run - runs a module's call\n"
			"\nUsage: run <call>\n"
			;
			printf("%s%s%s",cyanstr,buf,endcolor);
		}
		else if (strcmp(name,"exit") == 0)
		{
			char buf[] =
			"[Exit]\n"
			"\n"
			"  exit - exits PPF framework\n"
			"\nUsage: exit\n"
			;
			printf("%s%s%s",cyanstr,buf,endcolor);
			return;
		}
		else
		{
			printf("%s[!] Command doesn't exist%s\n",yellowstr,endcolor);
		}
	}
	else if (strcmp(type,"module") == 0)
	{
		for (int i = 0; i < MODULES_FOUND; i++)
		{
			if (strcmp(minfo[i].mod_name,name) == 0)
			{
				int oFD;
				ssize_t b_read;
				if ((oFD = open(minfo[i].mod_h_file,O_RDONLY)) == -1)
				{
					printf("%s[!] Couldn't open help file for module: %s%s\n",yellowstr,minfo[i].mod_name,endcolor);
					printf("%s[!] Module help file path: %s%s\n",yellowstr,minfo[i].mod_h_file,endcolor);
					return;
				}
				if ((b_read = read(oFD,minfo[i].mod_help,MOD_HELP_SIZE)) == -1)
				{
					printf("%s[!] Couldn't read help file %s for module %s%s\n",yellowstr,minfo[i].mod_h_file,minfo[i].mod_name,endcolor);
					return;
				}

				printf("%s%s%s",cyanstr,minfo[i].mod_help,endcolor);

				if (close(oFD) == -1)
				{
					printf("%s[!] Couldn't close file %s from module %s%s\n",yellowstr,minfo[i].mod_h_file,minfo[i].mod_name,endcolor);
					return;
				}
				return;
			}
		}
		printf("%s[!] Module %s doesn't exist%s",yellowstr,name,endcolor);
	}
	else
	{
		printf("%s[!] Invalid argument: %s%s\n",yellowstr,type,endcolor);
	}
}

/* Reloads modules */
void ppfReload()
{
	for (int i = 0; i < MODULES_FOUND; i++)
	{
		if (minfo[i].modSOhandle != NULL)
    		dlclose(minfo[i].modSOhandle);
	}
	free(minfo);
	minfo = NULL;
	MODULES_FOUND = 0;
	mreloadFlag = 1;
    loadMods();
	printf("%s[**] Loaded %d modules%s\n",cyanstr,MODULES_FOUND,endcolor);
}

/* Displays main help */
void displayMainHelp()
{
	char helpbuf[] = 
	"[**] PPF Shell Commands\n"
	"  + help\n"
	"  + banner\n"
	"  + clear\n"
	"  + list\n"
	"  + reload\n"
	"  + wield\n"
	"  + exit\n"
	"\n[**] Module Shell Commands\n"
	"  + sync\n"
	"  + retreat\n"
	"  + run\n"
	"\nFor command usage use: 'help usage <command>'\n"
	;
	printf("%s%s%s",cyanstr,helpbuf,endcolor);
}
