//
//  consoleCmdHandler.h
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

#ifndef _consoleCmdHandler_included
#define _consoleCmdHandler_included
#include "logger.h"
#include <string>
#include <tuple>
#include <map>

namespace BotManager
{
	enum CmdStatus
	{
		CmdStatus_None,
		CmdStatus_Passed,
		CmdStatus_Errored,
	};
	typedef CmdStatus (*cmdFunc)(char* msg, char *args[], int numArgs);
	typedef std::tuple<std::string, bool> cmdArgInfo;
	typedef std::map<std::string, cmdArgInfo> cmdArgInfoMap;
	typedef std::pair<std::string, cmdArgInfo> cmdArgInfoPair;
	class consoleCmdHandler
	{
		public:
			consoleCmdHandler(Logger *logger);
			virtual ~consoleCmdHandler();
			virtual bool AddConsoleCommand(char* cmdName, char* cmdDesc, cmdFunc callBack, bool hasArgs, cmdArgInfoMap args);
			virtual bool AddConsoleCommand(char* cmdName, char* cmdDesc, cmdFunc callback);
			virtual void FireCommand(char* msg);
	};
}
#endif
