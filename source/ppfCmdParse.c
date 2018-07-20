/*
 * Command parsing source
 */

#include "../headers/ppfh.h"

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
		minfo[i].modSOhandle = dlopen(minfo[i].mod_so_path, RTLD_LAZY);
		if ((*(void**)(&cmdPointer) = dlsym(minfo[i].modSOhandle,cmdStr)) == NULL)
		{
			dlclose(minfo[i].modSOhandle);
            minfo[i].modSOhandle = NULL;
			continue;
		}
		else
		{
			(*cmdPointer)();
			dlclose(minfo[i].modSOhandle);
            minfo[i].modSOhandle = NULL;
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
	else if (strcmp(cmd,"makemod") == 0)
	{
		ppfModMake(origin_cmd,base,type,name);
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
                        minfo[i].modSOhandle = NULL;
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

/* Compile modules with make */
void ppfModMake(char *makeCmd, char *base, char *type, char *name)
{
	if (strcmp(type,"all") == 0)
	{
		printf("%s[**] Compiling all modules%s\n",cyanstr,endcolor);
		for (int i = 0; i < MODULES_FOUND; i++)
		{
			printf("%s[**] Compiling module: %s%s\n",cyanstr,minfo[i].mod_name,endcolor);
			char mod_make_cmd[1000] = "make -C ";
			strcat(mod_make_cmd,minfo[i].mod_dir);
			system(mod_make_cmd);
		}
		printf("%s[**] All modules compiled%s\n",cyanstr,endcolor);
	}
	else if (strcmp(type,"module") == 0)
	{
		for (int i = 0; i < MODULES_FOUND; i++)
		{
			if (strcmp(name,minfo[i].mod_name) == 0)
			{
				printf("%s[**] Compiling module: %s%s\n",cyanstr,minfo[i].mod_name,endcolor);
				char mod_make_cmd[1000] = "make -C ";
				strcat(mod_make_cmd,minfo[i].mod_dir);
				system(mod_make_cmd);
				printf("%s[**] Compiled module: %s%s\n",cyanstr,minfo[i].mod_name,endcolor);
				return;
			}
		}
		printf("%s[!] Couldn't find module: %s%s\n",yellowstr,name,endcolor);
		return;
	}
	else
	{
		printf("%s[!] Invalid argument: %s%s\n",yellowstr,type,endcolor);
		printf("%s[!] Use 'help usage makemod' for help%s\n",yellowstr,endcolor);
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
				printf("%s[**] Module information for %s\n",cyanstr,minfo[i].mod_name);
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
			"[[Help]]\n"
			"\n"
			"[Modes]\n"
			"  ~ usage - Gives usage of a ppf command.\n"
			"  ~ module - Displays help for a loaded module.\n"
			"\n[Usage: help <mode> <arg>]\n"
			;
			printf("%s%s%s",cyanstr,buf,endcolor);
			return;
		}
		else if (strcmp(name,"list") == 0)
		{
			char buf[] =
			"[[List]]\n"
			"\n"
			"[Modes]\n"
			"  ~ info - Gives information about a module.\n"
			"  ~ modules - Lists all loaded modules.\n"
			"\n[Usage: list <mode> <arg>]\n"
			;
			printf("%s%s%s",cyanstr,buf,endcolor);
			return;
		}
		else if (strcmp(name,"reload") == 0)
		{
			char buf[] =
			"[[Reload]]\n"
			"\n"
			"[Modes]\n"
			"  ~ reload - Reloads all modules.\n"
			"\n[Usage: reload]\n"
			;
			printf("%s%s%s",cyanstr,buf,endcolor);
			return;
		}
		else if (strcmp(name,"makemod") == 0)
		{
			char buf[] =
			"[[MakeMod]]\n"
			"\n"
			"[Modes]\n"
			"  ~ all - Compiles all modules (don't need to specify name argument).\n"
			"  ~ module - Compiles a specific module.\n"
			"\n[Usage: makemod <arg> <name>]\n"
			;
			printf("%s%s%s",cyanstr,buf,endcolor);
			return;
		}
		else if (strcmp(name,"clear") == 0)
		{
			char buf[] =
			"[[Clear]]\n"
			"\n"
			"[Modes]\n"
			"  ~ clear - Clears the screen.\n"
			"\n[Usage: clear]\n"
			;
			printf("%s%s%s",cyanstr,buf,endcolor);
			return;
		}
		else if (strcmp(name,"banner") == 0)
		{
			char buf[] =
			"[[Banner]]\n"
			"\n"
			"[Modes]\n"
			"  ~ banner - Clears screen and displays banner.\n"
			"\n[Usage: banner]\n"
			;
			printf("%s%s%s",cyanstr,buf,endcolor);
			return;
		}
		else if (strcmp(name,"wield") == 0)
		{
			char buf[] =
			"[[Wield]]\n"
			"\n"
			"[Modes]\n"
			"  ~ wield - Loads a module.\n"
			"\nUsage: wield <module>\n"
			;
			printf("%s%s%s",cyanstr,buf,endcolor);
		}
		else if (strcmp(name,"sync") == 0)
		{
			char buf[] =
			"[[Sync]]\n"
			"\n"
			"[Modes]\n"
			"  ~ sync - Sets the value of a module parameter.\n"
			"\n[Usage: sync <arg> <value>]\n"
			;
			printf("%s%s%s",cyanstr,buf,endcolor);
		}
		else if (strcmp(name,"retreat") == 0)
		{
			char buf[] =
			"[[Retreat]]\n"
			"\n"
			"[Modes]\n"
			"  ~ retreat - Backs out of the module shell.\n"
			"\n[Usage: retreat]\n"
			;
			printf("%s%s%s",cyanstr,buf,endcolor);
		}
		else if (strcmp(name,"run") == 0)
		{
			char buf[] =
			"[[Run]]\n"
			"\n"
			"[Modes]\n"
			"  ~ run - Runs a module's call.\n"
			"\n[Usage: run <call>]\n"
			;
			printf("%s%s%s",cyanstr,buf,endcolor);
		}
		else if (strcmp(name,"exit") == 0)
		{
			char buf[] =
			"[[Exit]]\n"
			"\n"
			"[Modes]\n"
			"  ~ exit - Exits PPF framework.\n"
			"\n[Usage: exit]\n"
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
	mrf = 1;
    loadMods();
	printf("%s[**] Loaded %d modules%s\n",cyanstr,MODULES_FOUND,endcolor);
	mrf = 0;
}