 /* The smooth Class Library
  * Copyright (C) 1998-2020 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <gtk/gtk.h>

#include <smooth/gui/dialogs/dirdlg.h>
#include <smooth/files/directory.h>

const Error &S::GUI::Dialogs::DirSelection::ShowDialog()
{
	/* Create file chooser dialog.
	 */
	GtkFileChooserNative	*dialog = gtk_file_chooser_native_new(caption, NULL, GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
							      "_Open", "_Cancel");

	if (directory != NIL) gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), directory);

	/* Run dialog and check result.
	 */
	if (gtk_native_dialog_run(GTK_NATIVE_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
	{

		GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
		char	*name = gtk_file_chooser_get_filename(chooser);

		directory.ImportFrom("UTF-8", name);

		g_free(name);
	}
	else
	{
		directory = NIL;
	}

	g_object_unref(dialog);

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
