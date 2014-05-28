//
//  consoleWindow.cpp
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
#include "consoleWindow.h"
#define gtk_builder_get_widget(builder, id) GTK_WIDGET(gtk_builder_get_object(builder, id))
#define CMD_LOG_LINE "botmgr> "

using namespace BotManager;

Logger *conLogger;
GtkBuilder *builder;
GtkWidget *window;
GObject *conBuffer;
GtkWidget *conView;
GtkWidget *cmdSubmit;
GtkWidget *cmdLine;
GtkWidget *cmdStatus;
GtkTextIter textIter;
GtkTextTag *successTag;
GtkTextTag *warningTag;
GtkTextTag *errorTag;
GtkTextTag *defaultTag;
consoleCmdHandler *localCmdHandler;

CmdStatus QuitCommand(char* msg, char *args[], int numArgs)
{
	gtk_widget_destroy(window);
	return CmdStatus_Passed;
}

bool GtkConsoleOutput(char* output, int outputSize, LogLevel type)
{
	GtkTextTag *colorTag;
	switch (type)
	{
		case LogLevel_Success:
		{
			colorTag = successTag;
			break;
		}
		case LogLevel_Warning:
		{
			colorTag = warningTag;
			break;
		}
		case LogLevel_Error:
		{
			colorTag = errorTag;
			break;
		}
		default:
		{
			colorTag = defaultTag;
			break;
		}
	}
	gtk_text_buffer_insert_with_tags(GTK_TEXT_BUFFER(conBuffer), &textIter, output, outputSize, colorTag, NULL);
	return true;
}

bool GtkConsoleOutput(char* output, int outputSize)
{
	return GtkConsoleOutput(output, outputSize, LogLevel_None);
}

static void enterCommand(GtkWidget *widget, gpointer data)
{
	const gchar* inputLine = gtk_entry_get_text(GTK_ENTRY(cmdLine));
	if(inputLine[0] != '\0')
	{
		char input[strlen(inputLine)+5];
		sprintf(input, "%s\n", inputLine);
		char output[sizeof(input)+strlen(CMD_LOG_LINE)];
		sprintf(output, "%s%s", CMD_LOG_LINE, input);
		GtkConsoleOutput(output, strlen(output));
		conLogger->LogDebug((char*)"Recieved command input: %s", inputLine);
		localCmdHandler->FireCommand((char*)inputLine);
	}
	gtk_entry_set_text(GTK_ENTRY(cmdLine), "");
}

ConsoleWindow::ConsoleWindow(char* consoleTitle, char* consoleUiFilePath, Logger *logger, consoleCmdHandler *cmdHandler, int *status, char* errorBuffer)
{
	this->consoleUiFile += consoleUiFilePath;
	conLogger = logger;
	conLogger->LogDebug((char*)"%s will now attempt to be built.", consoleTitle);
	localCmdHandler = cmdHandler;
	if (!exists(this->consoleUiFile))
	{
		sprintf(errorBuffer, "Can't find file %s\n", consoleUiFilePath);
		*status=1;
	}
	else
	{
		builder = gtk_builder_new();
		gtk_builder_add_from_file(builder, this->consoleUiFile.c_str(), NULL);
		window = gtk_builder_get_widget(builder, "consoleWindow");
		g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
		gtk_window_set_title(GTK_WINDOW(window), consoleTitle);
		conBuffer = gtk_builder_get_object(builder, "consoleBuffer");
		conView = gtk_builder_get_widget(builder, "consoleView");
		GdkRGBA tColor;
		gdk_rgba_parse(&tColor, "#000000");
		gtk_widget_override_background_color(conView, GTK_STATE_FLAG_NORMAL, &tColor);
		gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(conBuffer), &textIter, 0);
		successTag = gtk_text_buffer_create_tag(GTK_TEXT_BUFFER(conBuffer), "Success Color Tag", "foreground", "#00FF00", NULL);
		warningTag = gtk_text_buffer_create_tag(GTK_TEXT_BUFFER(conBuffer), "Warning Color Tag", "foreground", "#FF9900", NULL);
		errorTag = gtk_text_buffer_create_tag(GTK_TEXT_BUFFER(conBuffer), "Error Color Tag", "foreground", "#FF0000", NULL);
		defaultTag = gtk_text_buffer_create_tag(GTK_TEXT_BUFFER(conBuffer), "Default Color Tag", "foreground", "#FFFFFF", NULL);
		cmdSubmit = gtk_builder_get_widget(builder, "cmdSubmit");
		g_signal_connect(cmdSubmit, "clicked", G_CALLBACK(enterCommand), NULL);
		cmdLine = gtk_builder_get_widget(builder, "cmdLine");
		g_signal_connect(cmdLine, "activate", G_CALLBACK(enterCommand), NULL);
		conLogger->LogDebug((char*)"%s has been successfully built!", consoleTitle);
		conLogger->RedirectConsoleOutput(GtkConsoleOutput);
		gtk_widget_show_all(window);
		localCmdHandler->AddConsoleCommand((char*)"quit", (char*)"Quits program(without clicking close button)", QuitCommand);
		strcpy(errorBuffer, "");
		*status=0;
	}
}

ConsoleWindow::~ConsoleWindow()
{
}
#endif