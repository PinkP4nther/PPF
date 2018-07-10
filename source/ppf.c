/* PPF */

#include "../headers/ppfh.h"

int main()
{
	int runFlag = 1;
	char cmdStr[100];

	/* Load Modules */
	loadMods();
	ppfBanner();

	while (runFlag)
	{
		ppfPrompt("PPF");
		if (fgets(cmdStr,sizeof(cmdStr),stdin) != NULL)
		{
			cmdStr[strcspn(cmdStr,"\n")] = '\0';

			if (strncmp(cmdStr,"exit",4) == 0)
			{
				break;
			}
			else if (strncmp(cmdStr,"help",4) == 0)
			{
				processArgs("help",cmdStr);
			}
			else if (strncmp(cmdStr,"wield",5) == 0)
			{
				processArgs("wield",cmdStr);
			}
			else if (strncmp(cmdStr,"list",4) == 0)
			{
				processArgs("list",cmdStr);
			}
			else if (strncmp("reload",cmdStr,6) == 0)
			{
				ppfReload();
			}
			else if (strncmp("makemod",cmdStr,7) == 0)
			{
				processArgs("makemod",cmdStr);
			}
			else if (strncmp("clear",cmdStr,5) == 0)
			{
				printf("\033[H\033[J");
			}
			else if (strncmp("banner",cmdStr,6) == 0)
			{
				printf("\033[H\033[J");
				ppfBanner();
			}
			else
			{
				if (strcmp(cmdStr,"") == 0)
				{
					continue;
				}
				else
				{
					cmdHandle(cmdStr);
				}
			}
		}
	}

	for (int i = 0; i < MODULES_FOUND; i++)
	{
		if (minfo[i].modSOhandle != NULL)
			dlclose(minfo[i].modSOhandle);
	}

	free(minfo);
	minfo = NULL;
	
	ppfinfo("Exiting PPF!");
	return 0;
}
