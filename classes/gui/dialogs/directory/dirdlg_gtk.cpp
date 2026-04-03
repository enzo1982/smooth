 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <gtk/gtk.h>

#ifdef GDK_WINDOWING_X11
#	include <gdk/gdkx.h>

#	undef True
#	undef False
#	undef Bool
#	undef Success
#endif

#include <smooth/gui/dialogs/dirdlg.h>
#include <smooth/files/directory.h>

const Error &S::GUI::Dialogs::DirSelection::ShowDialog()
{
	/* Create file chooser dialog.
	 */
	GtkWidget	*dialog = gtk_file_chooser_dialog_new(caption, NULL, GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
							      GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
							      GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
							      NULL);

	if (directory != NIL) gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), directory);

	/* Make sure dialog gets the focus.
	 */
#ifdef GDK_WINDOWING_X11
	gtk_widget_show_all(GTK_WIDGET(dialog));

	if (GDK_IS_X11_DISPLAY(gtk_widget_get_display(GTK_WIDGET(dialog))))
	{
		GdkWindow	*window = gtk_widget_get_window(GTK_WIDGET(dialog));

		gdk_window_set_events(window, GdkEventMask(gdk_window_get_events(window) | GDK_PROPERTY_CHANGE_MASK));
		gtk_window_present_with_time(GTK_WINDOW(dialog), gdk_x11_get_server_time(window));
	}
#endif

	/* Run dialog and check result.
	 */
	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
	{
		char	*name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

		directory.ImportFrom("UTF-8", name);

		g_free(name);
	}
	else
	{
		directory = NIL;
	}

	gtk_widget_destroy(dialog);

	/* Wait for GTK to finish pending actions.
	 */
	while (gtk_events_pending()) gtk_main_iteration();

	/* Check if we actually have a directory.
	 */
	if (directory != NIL)
	{
		if (!directory.EndsWith(Directory::GetDirectoryDelimiter())) directory.Append(Directory::GetDirectoryDelimiter());
	}
	else
	{
		error = Error();
	}

	return error;
}
