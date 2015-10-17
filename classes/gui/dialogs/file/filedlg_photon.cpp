 /* The smooth Class Library
  * Copyright (C) 1998-2015 Robert Kausch <robert.kausch@gmx.net>
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

const Error &S::GUI::Dialogs::FileSelection::ShowDialog()
{
	/* Setup file selection dialog.
	 */
	PtFileSelectionInfo_t	 info;

	memset(&info, 0, sizeof(info));

	/* Generate default path.
	 */
	String	 path = defPath != NIL ? defPath : "~";

	if (!path.EndsWith("/") && defFile != NIL) path.Append("/");

	path.Append(defFile);

	/* Setup flags.
	 */
	int	 ptfl = 0;

	if	(mode == SFM_OPEN) ptfl =  flags & SFD_ALLOWMULTISELECT ? Pt_FSR_MULTIPLE	  : 0;
	else if (mode == SFM_SAVE) ptfl = (flags & SFD_CONFIRMOVERWRITE ? Pt_FSR_CONFIRM_EXISTING : 0) | Pt_FSR_NO_FCHECK;

	/* Show dialog.
	 */
	int	 result = PtFileSelection(NULL, NULL, caption, path, NULL, NULL, NULL, NULL, &info, ptlf);

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
