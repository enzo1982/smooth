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

#include <smooth/gui/dialogs/filedlg.h>
#include <smooth/files/file.h>
#include <smooth/misc/number.h>
#include <smooth/foreach.h>

const Error &S::GUI::Dialogs::FileSelection::ShowDialog()
{
	/* Create file chooser dialog.
	 */
	GtkFileChooserNative	*dialog = NULL;

	if (mode == SFM_OPEN)
	{
		dialog = gtk_file_chooser_native_new(caption, NULL, GTK_FILE_CHOOSER_ACTION_OPEN,
						     "_Open", "_Cancel");

		if (flags & SFD_ALLOWMULTISELECT) gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(dialog), True);
	}
	else if (mode == SFM_SAVE)
	{
		dialog = gtk_file_chooser_native_new(caption, NULL, GTK_FILE_CHOOSER_ACTION_SAVE,
						     "_Save", "_Cancel");

		if (flags & SFD_CONFIRMOVERWRITE) gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), True);
	}

	if (defPath != NIL) gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), defPath);
	if (defFile != NIL) gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), defFile);

	/* Add file filters.
	 */
	for (int i = 0; i < filters.Length(); i++)
	{
		GtkFileFilter		*filter = gtk_file_filter_new();
		const Array<String>	&patterns = filters.GetNth(i).Explode(";");

		foreach (const String &pattern, patterns)
		{
			/* Make patterns case insensitive.
			 */
			String	 converted = pattern.Head(		     pattern.FindLast(".") + 1);
			String	 extension = pattern.Tail(pattern.Length() - pattern.FindLast(".") - 1).ToLower();

			for (Int i = 0; i < extension.Length(); i++)
			{
				if (extension[i] >= 'a' && extension[i] <= 'z') converted.Append("[").Append(extension.SubString(i, 1)).Append(extension.SubString(i, 1).ToUpper()).Append("]");
				else						converted.Append(extension.SubString(i, 1));
			}

			gtk_file_filter_add_pattern(filter, converted.Trim());
		}

		gtk_file_filter_set_name(filter, filterNames.GetNth(i));

		gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
	}

	/* Run dialog and check result.
	 */
	if (gtk_native_dialog_run(GTK_NATIVE_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
	{
		GSList	*list = gtk_file_chooser_get_uris(GTK_FILE_CHOOSER(dialog));
		GSList	*current = list;

		while (current != NULL)
		{
			String	 file = String((char *) current->data).Replace("file://", NIL);

			for (Int i = 0; i < file.Length() - 2; i++)
			{
				if (file[i] == '%')
				{
					file[i] = (Int64) Number::FromHexString(file.SubString(i + 1, 2));
					file = file.Head(i + 1).Append(file.Tail(file.Length() - i - 3));
				}
			}

			file.ImportFrom("UTF-8", file.ConvertTo("ISO-8859-1"));
			files.Add(file);

			g_free(current->data);

			current = current->next;
		}

		g_slist_free(list);
	}
	else
	{
		error = Error();
	}

	g_object_unref(dialog);

	/* Wait for GTK to finish pending actions.
	 */
	while (gtk_events_pending()) gtk_main_iteration();

	return error;
}
