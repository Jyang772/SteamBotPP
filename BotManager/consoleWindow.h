//
//  consoleWindow.h
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

#ifndef ServerMode
#ifndef _consoleWindow_included
#define _consoleWindow_included
#include <gtk/gtk.h>
#include "consoleCmdHandler.h"

namespace BotManager
{
	class ConsoleWindow
	{
		public:
			ConsoleWindow(char* consoleTitle, char* consoleUiFilePath, Logger *logger, consoleCmdHandler *cmdHandler, int *status, char* errorBuffer);
			virtual ~ConsoleWindow();
		private:
			boost::filesystem::path consoleUiFile;
	};
}
#endif
#endif
