 /* The smooth Class Library
  * Copyright (C) 1998-2009 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <gtk/gtk.h>

#include <smooth/gui/dialogs/file/filedlg_gtk.h>
#include <smooth/files/file.h>

const Error &S::GUI::Dialogs::FileSelection::ShowDialog()
{
	/* Init the GTK
	 */
	gtk_init(NULL, NULL);

	/* Create file chooser dialog
	 */
	GtkWidget	*dialog = NULL;

	if (mode == SFM_OPEN)
	{
		dialog = gtk_file_chooser_dialog_new(caption, NULL, GTK_FILE_CHOOSER_ACTION_OPEN,
						     GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
						     GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
						     NULL);

		if (flags & SFD_ALLOWMULTISELECT) gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(dialog), True);
	}
	else if (mode == SFM_SAVE)
	{
		dialog = gtk_file_chooser_dialog_new(caption, NULL, GTK_FILE_CHOOSER_ACTION_SAVE,
						     GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
						     GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
						     NULL);

		if (flags & SFD_CONFIRMOVERWRITE) gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), True);

		gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), File(defFile).GetFilePath());
		gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), File(defFile).GetFileName());
	}

	/* Add file filters
	 */
	for (int i = 0; i < filters.Length(); i++)
	{
		GtkFileFilter	*filter = gtk_file_filter_new();

		gtk_file_filter_set_name(filter, filterNames.GetNth(i));
		gtk_file_filter_add_pattern(filter, filters.GetNth(i));

		gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
	}

	/* Run dialog and check result
	 */
	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
	{
		GSList	*list = gtk_file_chooser_get_uris(GTK_FILE_CHOOSER(dialog));
		GSList	*current = list;

		while (current != NULL)
		{
			files.Add(String((char *) current->data));

			g_free(current->data);

			current = current->next;
		}

		g_slist_free(list);
	}
	else
	{
		error = Error();
	}

	gtk_widget_destroy(dialog);

	/* Wait for GTK to finish pending actions
	 */
	while (gtk_events_pending()) gtk_main_iteration();

	return error;
}
