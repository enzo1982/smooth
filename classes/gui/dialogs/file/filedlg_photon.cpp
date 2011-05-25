 /* The smooth Class Library
  * Copyright (C) 1998-2011 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <Pt.h>

#include <smooth/gui/dialogs/file/filedlg_photon.h>
#include <smooth/files/file.h>
#include <smooth/misc/number.h>
#include <smooth/foreach.h>

const Error &S::GUI::Dialogs::FileSelection::ShowDialog()
{
	/* Init the Photon API.
	 */
	PtInit(NULL);

	/* Show file selection dialog.
	 */
	PtFileSelectionInfo_t	 info;
	int			 result = -1;

	memset(&info, 0, sizeof(info));

	if (mode == SFM_OPEN)
	{
		result = PtFileSelection(NULL, NULL, caption, "~", NULL, NULL, NULL, NULL, &info, flags & SFD_ALLOWMULTISELECT ? Pt_FSR_MULTIPLE : 0);
	}
	else if (mode == SFM_SAVE)
	{
		result = PtFileSelection(NULL, NULL, caption, defFile, NULL, NULL, NULL, NULL, &info, Pt_FSR_NO_FCHECK |
								      (flags & SFD_CONFIRMOVERWRITE ? Pt_FSR_CONFIRM_EXISTING : 0));
	}

	/* Check result and get file names.
	 */
	if (result == 0 && info.ret == Pt_FSDIALOG_BTN1)
	{
		if (!(flags & SFD_ALLOWMULTISELECT))
		{
			files.Add(info.path);
		}
		else
		{
			for (Int i = 0; i < info.minfo->nitems; i++)
			{
				files.Add(info.minfo->multipath[i]);
			}
		}
	}
	else
	{
		error = Error();
	}

	return error;
}
