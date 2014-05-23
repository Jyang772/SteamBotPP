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

bool GtkConsoleOutput(char* output, int outputSize)
{
	gtk_text_buffer_insert_at_cursor(GTK_TEXT_BUFFER(conBuffer), output, outputSize);
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