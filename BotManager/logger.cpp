//
//  Logger.cpp
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

#include "logger.h"
#include <iostream>
#include <fstream>

using namespace BotManager;
using namespace std;
using namespace boost::filesystem;

bool DefaultConsoleLogger(char* output, int outputSize, LogLevel type)
{
	cout << output;
	return true;
}

Logger::Logger(char* loggerName, char* loggerShortName, char* logFileName, int *status, char* errorBuffer)
{
	this->logsFolderPath += "logs/";
	this->logFilePath += this->logsFolderPath;
	this->logFilePath += logFileName;
	strcpy(this->loggerName, loggerName);
	strcpy(this->loggerShortName, loggerShortName);
	this->cLogLevel = LogLevel_Info;
	this->conLogger = &DefaultConsoleLogger;
	if (!exists(this->logsFolderPath))
	{
		if (!create_directory(this->logsFolderPath))
		{
			sprintf(errorBuffer, "Can't create folder called %s", this->logsFolderPath.c_str());
			*status = 1;
		}
	}
	this->LogInfo((char*)"Logger %s started.", loggerName);
	strcpy(errorBuffer, "");
	*status = 0;
}

Logger::~Logger()
{
}

void Logger::RedirectConsoleOutput(consoleLogger outputFunc)
{
	this->conLogger = outputFunc;
}

void Logger::ResetConsoleOutput()
{
	this->conLogger = &DefaultConsoleLogger;
}

void Logger::LogLevelStr(LogLevel type, char* buffer)
{
	switch(type)
	{
		case LogLevel_Debug:
		{
			strcpy(buffer, "Debug");
			break;
		}
		case LogLevel_Info:
		{
			strcpy(buffer, "Info");
			break;
		}
		case LogLevel_Success:
		{
			strcpy(buffer, "Success");
			break;
		}
		case LogLevel_Warning:
		{
			strcpy(buffer, "Warning");
			break;
		}
		case LogLevel_Error:
		{
			strcpy(buffer, "Error");
			break;
		}
		default:
		{
			strcpy(buffer, "");
			break;
		}
	}
}

bool Logger::LogFile(char* output)
{
	fstream logFile(this->logFilePath.c_str(), ios::out|ios::app);
	if (logFile.is_open())
	{
		logFile << output;
		logFile.close();
		return true;
	}
	else
		return false;
	return false;
}

void Logger::Log(char* logMSG, LogLevel type, bool forceConsole)
{
	char logSNameOut[sizeof(this->loggerShortName)+2];
	char typeOut[10];
	char typeStr[8];
	char starterOut[sizeof(typeOut)+sizeof(logSNameOut)+2];
	char output[5000];
	this->LogLevelStr(type, typeStr);
	if (type != LogLevel_None)
	{
		sprintf(typeOut, "[%s]", typeStr);
		sprintf(logSNameOut, "[%s]", this->loggerShortName);
		sprintf(starterOut, "%s%s: ", logSNameOut, typeOut);
	}
	else
	{
		strcpy(typeOut, typeStr);
		strcpy(logSNameOut, "");
		strcpy(starterOut, "");
	}
	sprintf(output, "%s%s\n", starterOut, logMSG);
	if ((type >= this->cLogLevel && type != LogLevel_None) || forceConsole)
	{
		if (!this->conLogger(output, strlen(output), type))
			cout << "Error: Unable to log to console we were given!" << endl;
	}
	if (!this->LogFile(output))
		cout << "Error: Unable to log to " << logFilePath.c_str() << endl;
}

void Logger::Log(char* logMSG, LogLevel type)
{
	this->Log(logMSG, type, false);
}

void Logger::LogNoneWithConsole(char* format, ...)
{
	char outputMSG[500];
	va_list args;
	va_start(args, format);
	vsprintf(outputMSG, format, args);
	va_end(args);
	this->Log(outputMSG, LogLevel_None, true);
}

void Logger::LogNone(char* format, ...)
{
	char outputMSG[500];
	va_list args;
	va_start(args, format);
	vsprintf(outputMSG, format, args);
	va_end(args);
	this->Log(outputMSG, LogLevel_None);
}

void Logger::LogDebug(char* format, ...)
{
	#ifdef Debug
	char outputMSG[500];
	va_list args;
	va_start(args, format);
	vsprintf(outputMSG, format, args);
	va_end(args);
	this->Log(outputMSG, LogLevel_Debug);
	#endif
}

void Logger::LogInfo(char* format, ...)
{
	char outputMSG[500];
	va_list args;
	va_start(args, format);
	vsprintf(outputMSG, format, args);
	va_end(args);
	this->Log(outputMSG, LogLevel_Info);
}

void Logger::LogSuccess(char* format, ...)
{
	char outputMSG[500];
	va_list args;
	va_start(args, format);
	vsprintf(outputMSG, format, args);
	va_end(args);
	this->Log(outputMSG, LogLevel_Success);
}

void Logger::LogWarning(char* format, ...)
{
	char outputMSG[500];
	va_list args;
	va_start(args, format);
	vsprintf(outputMSG, format, args);
	va_end(args);
	this->Log(outputMSG, LogLevel_Warning);
}

void Logger::LogError(char* format, ...)
{
	char outputMSG[500];
	va_list args;
	va_start(args, format);
	vsprintf(outputMSG, format, args);
	va_end(args);
	this->Log(outputMSG, LogLevel_Error);
}
