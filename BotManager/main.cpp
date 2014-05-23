//
//  main.cpp
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

#include "consoleWindow.h"
#include <iostream>

using namespace BotManager;

int main (int argc, char *argv[])
{
	char errorBuffer[256];
	int status;
	Logger *logger = new Logger((char*)"BotMgrLogger", (char*)"BOTMGR", (char*)"BotLog.log", &status, errorBuffer, sizeof(errorBuffer));
	if (status != 0)
	{
		cout << "ERROR: " << errorBuffer << endl;
		delete logger;
		return 1;
	}
	gtk_init(&argc, &argv);
	ConsoleWindow *console = new ConsoleWindow((char*)"BotManager Console", (char*)"uis/mainConsole.ui", logger, &status, errorBuffer, sizeof(errorBuffer));
	if (status != 0)
	{
		logger->LogError((char*)errorBuffer);
		delete console;
		delete logger;
		return 1;
	}
	gtk_main();
	logger->ResetConsoleOutput();
	logger->LogWarning((char*)"BotManager is now exitting.");
	delete console;
	delete logger;
	return 0;
}