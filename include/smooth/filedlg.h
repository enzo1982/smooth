 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_FILEDLG_
#define _H_OBJSMOOTH_FILEDLG_

#define SDialogFileSelection SMOOTHDialogFileSelection

class SMOOTHDialogFileSelection;

#include "definitions.h"
#include "dialog.h"
#include "array.h"

// File dialog mode constants - SFM_OPEN is default

const SMOOTHInt	 SFM_OPEN	= 0;
const SMOOTHInt	 SFM_SAVE	= 1;

// File dialog flags - can be set with ->SetFlags(...); SFD_FILEMUSTEXIST is always set for SFM_OPEN mode

const SMOOTHInt	 SFD_ALLOWMULTISELECT	= 512;
const SMOOTHInt	 SFD_FILEMUSTEXIST	= 4096;
const SMOOTHInt	 SFD_CONFIRMOVERWRITE	= 2;

class SMOOTHAPI SMOOTHDialogFileSelection : public SMOOTHDialog
{
	private:
		SMOOTHArray<SMOOTHString>	 filterNames;
		SMOOTHArray<SMOOTHString>	 filters;
		SMOOTHArray<SMOOTHString>	 files;
		SMOOTHInt			 flags;
		SMOOTHInt			 mode;
		SMOOTHString			 defExt;
	public:
						 SMOOTHDialogFileSelection();
						~SMOOTHDialogFileSelection();

		SMOOTHInt			 ShowDialog();

		SMOOTHInt			 SetMode(SMOOTHInt);
		SMOOTHInt			 SetFlags(SMOOTHInt);
		SMOOTHInt			 SetDefaultExtension(SMOOTHString);

		SMOOTHInt			 AddFilter(SMOOTHString, SMOOTHString);

		SMOOTHInt			 GetNumberOfFiles();
		SMOOTHString			 GetFileName();
		SMOOTHString			 GetNthFileName(SMOOTHInt);
};

#endif
