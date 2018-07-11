/*
 * Module loading call source
 */

#include "../headers/ppfh.h"

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

		if (fbuf[0] == '#' || strlen(fbuf) < 8)
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

		char modc_buf[MOD_CONF_SIZE];
		strncpy(modc_buf,temp_path,MOD_CONF_SIZE);
		strcat(modc_buf,"/");
		strcat(modc_buf,mod_ent->d_name);
		strcat(modc_buf,".ppfconf");
		strncpy(minfo[MODULES_FOUND].mod_conf,modc_buf,MOD_CONF_SIZE);

		/* Put file reading stuff after this? */
		parse_module_config(MODULES_FOUND);

		if (mrf == 1)
			printf("%s[**] Reloaded module: %s%s\n",cyanstr,minfo[MODULES_FOUND].mod_name,endcolor);

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