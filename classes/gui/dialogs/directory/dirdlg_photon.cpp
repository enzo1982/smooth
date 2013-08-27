 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <Pt.h>

#include <smooth/gui/dialogs/directory/dirdlg_photon.h>
#include <smooth/files/directory.h>

const Error &S::GUI::Dialogs::DirSelection::ShowDialog()
{
	/* Show directory selection dialog.
	 */
	PtFileSelectionInfo_t	 info;

	memset(&info, 0, sizeof(info));

	int	 result = PtFileSelection(NULL, NULL, caption, directory, NULL, NULL, NULL, NULL, &info, Pt_FSR_SELECT_DIRS     |
													 Pt_FSR_DONT_SHOW_FILES |
													 Pt_FSR_NO_FSPEC );

	/* Check result and get path.
	 */
	if (result == 0 && info.ret == Pt_FSDIALOG_BTN1)
	{
		directory = info.path;
	}

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
