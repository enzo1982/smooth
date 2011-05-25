 /* The smooth Class Library
  * Copyright (C) 1998-2011 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <gtk/gtk.h>

#include <smooth/gui/dialogs/directory/dirdlg_gtk.h>
#include <smooth/files/directory.h>

const Error &S::GUI::Dialogs::DirSelection::ShowDialog()
{
	/* Init the GTK.
	 */
	gtk_init(NULL, NULL);

	/* Create file chooser dialog.
	 */
	GtkWidget	*dialog = NULL;

	dialog = gtk_file_chooser_dialog_new(caption, NULL, GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER,
					     GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					     GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
					     NULL);

	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), directory);

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
