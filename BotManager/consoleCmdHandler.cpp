//
//  consoleCmdHandler.cpp
//
//  Author:
//       WildCard65 <lollol222gg@gmail.com>
//
//  Copyright (c) 2014 WildCard65
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "consoleCmdHandler.h"

using namespace BotManager;

typedef map<string, cmdFunc> cmdMap;
typedef pair<string, cmdFunc> cmdPair;
typedef map<string, string> cmdDescMap;
typedef pair<string, string> cmdDescPair;
typedef map<string, cmdArgInfoMap> cmdArgMap;
typedef pair<string, cmdArgInfoMap> cmdArgPair;

cmdMap cmds;
cmdDescMap cmdDescs;
cmdArgMap cmdArgs;
Logger *myLogger;

CmdStatus HelpCommand(char* msg, char *args[], int numArgs)
{
	if (cmds.empty())
	{
		myLogger->LogNoneWithConsole((char*)"No commands available");
	}
	else
	{
		if (numArgs == 0)
		{
			char output[5000];
			sprintf(output, "Commands available:\n");
			for (cmdDescPair x: cmdDescs)
			{
				char temp[1000];
				sprintf(temp, "%s\t//%s\n", x.first.c_str(), x.second.c_str());
				strcat(output, temp);
			}
			myLogger->LogNoneWithConsole(output);
		}
		else
			myLogger->LogDebug(args[0]);
	}
	return CmdStatus_Passed;
}

consoleCmdHandler::consoleCmdHandler(Logger *logger)
{
	myLogger = logger;
	cmdArgInfoMap helpArgs;
	helpArgs.insert(cmdArgInfoPair("arg1", cmdArgInfo("Displays more information about the command.", true)));
	this->AddConsoleCommand((char*)"help", (char*)"Displays this help message.", HelpCommand, true, helpArgs);
}

consoleCmdHandler::~consoleCmdHandler()
{
}

bool consoleCmdHandler::AddConsoleCommand(char* cmdName, char* cmdDesc, cmdFunc callBack, bool hasArgs, cmdArgInfoMap args)
{
	if (cmds.find(cmdName) == cmds.end())
	{
		cmds.insert(cmdPair(cmdName, callBack));
		cmdDescs.insert(cmdDescPair(cmdName, cmdDesc));
		if (hasArgs)
		{
			cmdArgs.insert(cmdArgPair(cmdName, args));
		}
		return true;
	}
	return false;
}

bool consoleCmdHandler::AddConsoleCommand(char* cmdName, char* cmdDesc, cmdFunc callBack)
{
	cmdArgInfoMap tempMap;
	return this->AddConsoleCommand(cmdName, cmdDesc, callBack, false, tempMap);
}

void consoleCmdHandler::FireCommand(char* msg)
{
	if (cmds.empty())
	{
		myLogger->LogWarning((char*)"No commands available.");
		return;
	}
	char cmd[strcspn(msg, " ")+1];
	char splitLine[256][500];
	int k = 0;
	char * pch;
	pch = strtok(msg, " ");
	while (pch != NULL)
	{
		strcpy(splitLine[k], pch);
		++k;
		pch = strtok(NULL, " ");
	}
	strcpy(cmd, splitLine[0]);
	if (cmds.find(cmd) != cmds.end())
	{
		myLogger->LogDebug((char*)"Found command: %s", cmd);
		for (int i=0; i<k-1; i++)
		{
			strcpy(args[i], splitLine[i+1]);
		}
		if (cmds.at(cmd)(msg, args, k-1) == CmdStatus_Errored)
			myLogger->LogError((char*)"Command %s failed to be executed.", cmd);
	}
}
