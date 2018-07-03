#include <dirent.h>

#include <vector>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include<switch.h>

#include "config.h"

//Very rough implementation of a rom selector... Yeah I know its ugly
char* menu_FileBrowser() 
{
	std::vector<std::string> entries;
	DIR *directory;
	dirent *entry;
	directory = opendir("sdmc:/switch/desmume/roms");

	while(entry = readdir(directory))
	{
		if(entry->d_type == DT_DIR) continue;

		char *extension = strrchr(entry->d_name,'.');

		if(extension != NULL)
		{
			if(strcmp(extension, ".nds") == 0)
				entries.push_back(entry->d_name);
			//else if(strcmp(extension, ".srl") == 0)
			//	entries.push_back(entry->d_name);
		}
	}

	if(entries.empty())
		return NULL;

	int count, cursor = 0;
	uint32_t keysDown;

	while(true) 
	{
		hidScanInput();
		keysDown = hidKeysDown(CONTROLLER_P1_AUTO);

		if(keysDown & KEY_A)
			break;
		
		if(keysDown & KEY_B)
			UserConfiguration.portraitEnabled = !UserConfiguration.portraitEnabled;

		if(keysDown & KEY_X)
			UserConfiguration.soundEnabled = !UserConfiguration.soundEnabled;

		if(keysDown & KEY_DOWN)
			cursor++;

		if(keysDown & KEY_UP)
			cursor--;

		if(keysDown & KEY_LEFT)
		{
			if(UserConfiguration.frameSkip > 0)
			UserConfiguration.frameSkip--;
		}

		if(keysDown & KEY_RIGHT)
		{
			UserConfiguration.frameSkip++;
		}

		if(cursor < 0)
			cursor = 0;

		if(cursor > entries.size() - 1)
			cursor = entries.size() - 1;

		count = 0;

		printf(CONSOLE_ESC(0;0H));

		for( auto it = entries.begin(); it != entries.end(); it++)
		{
			if(cursor == count)
				printf(CONSOLE_ESC(4m));
			else
				printf(CONSOLE_ESC(0m));

			printf("%s\n", it->c_str());

			count++;
		}

		printf(CONSOLE_ESC(0m));
		printf(CONSOLE_ESC(40;0H)"Press ");
		printf(CONSOLE_BLUE"(X) ");
		printf(CONSOLE_WHITE"to %s Sound\n", UserConfiguration.soundEnabled ? "disable" : " enable");
		printf("Press ");
		printf(CONSOLE_RED"(A) ");
		printf(CONSOLE_WHITE"to Launch a Title\n");
		printf("Press ");
		printf(CONSOLE_YELLOW"(B) ");
		printf(CONSOLE_WHITE"to %s Portrait Mode\n", UserConfiguration.portraitEnabled ? "disable" : "enable");
		printf("Press Left or Right DPAD to change frameSkip\n");
		printf("Current frameSkip value: %u\n", UserConfiguration.frameSkip);

	}
	printf(CONSOLE_ESC(0;0H));

	char *filename = (char*)malloc(4096);
	sprintf(filename, "sdmc:/switch/desmume/roms/%s", entries[cursor].c_str());

	return filename;
}

//Uninplemented
int menu_Init()
{
	return 0;
}

//Uninplemented
//This is where all the tabs(rom selection, save states, settings, etc) will be handled
int menu_Display()
{
	return 0;
}