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

#include "consoleWindow.h"
#define gtk_builder_get_widget(builder, id) GTK_WIDGET(gtk_builder_get_object(builder, id))

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

ConsoleWindow::ConsoleWindow(char* consoleTitle, char* consoleUiFilePath, Logger *logger, int *status, char* errorBuffer, int bufferSize)
{
	this->consoleUiFile += consoleUiFilePath;
	conLogger = logger;
	conLogger->LogDebug((char*)"%s will now attempt to be built.", consoleTitle);
	if (!exists(this->consoleUiFile))
	{
		snprintf(errorBuffer, bufferSize, "Can't find file %s\n", consoleUiFilePath);
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
		conLogger->LogDebug((char*)"%s has been successfully built!", consoleTitle);
		conLogger->RedirectConsoleOutput(GtkConsoleOutput);
		gtk_widget_show_all(window);
		strncpy(errorBuffer, "", bufferSize);
		*status=0;
	}
}

ConsoleWindow::~ConsoleWindow()
{
}