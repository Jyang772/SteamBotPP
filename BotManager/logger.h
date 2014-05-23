//
//  Logger.h
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

#ifndef _Logger_included
#define _Logger_included
#include <stdarg.h>
#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;

namespace BotManager
{
	enum LogLevel
	{
		LogLevel_None=0,
		LogLevel_Debug=1,
		LogLevel_Info=2,
		LogLevel_Success=3,
		LogLevel_Warning=4,
		LogLevel_Error=5,
	};
	class Logger
	{
		public:
			typedef bool (*consoleLogger)(char* output, int outputSize, LogLevel type);
			Logger(char* loggerName, char* loggerShortName, char* logFileName, int *status, char* errorBuffer, int bufferSize);
			virtual ~Logger();
			virtual void RedirectConsoleOutput(consoleLogger outputFunc);
			virtual void ResetConsoleOutput();
			virtual void LogNoneWithConsole(char *format, ...);
			virtual void LogNone(char *format, ...);
			virtual void LogDebug(char *format, ...);
			virtual void LogInfo(char *format, ...);
			virtual void LogSuccess(char *format, ...);
			virtual void LogWarning(char *format, ...);
			virtual void LogError(char *format, ...);
		private:
			char loggerName[30];
			char loggerShortName[10];
			LogLevel cLogLevel;
			path logsFolderPath;
			path logFilePath;
			consoleLogger conLogger;
			virtual void LogLevelStr(LogLevel type, char* buffer, int bufferSize);
			virtual bool LogFile(char* output);
			virtual void Log(char* logMSG, LogLevel type, bool forceConsole);
			virtual void Log(char* logMSG, LogLevel type);
	};
}
#endif //_Logger_included