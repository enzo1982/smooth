 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_MENU_
#define _H_OBJSMOOTH_MENU_

#define SMenu SMOOTHMenu

class SMOOTHMenu;

#include "object.h"

const SMOOTHInt SM_SEPARATOR		= 0;
const SMOOTHInt SM_TEXT			= 1;
const SMOOTHInt SM_BITMAP		= 2;

class SMOOTHAPI SMOOTHMenu
{
	public: class SMOOTHAPI Entry
	{
		public:
			SMOOTHInt	 type;
			SMOOTHInt	 id;

			SMOOTHString	 text;
			SMOOTHString	 tooltip;
			SMOOTHString	 description;

			HBITMAP		 bitmap;
			HBITMAP		 graymap;

			SMOOTHInt	 orientation;

			SMOOTHBool	 checked;
			SMOOTHBool	 clicked;

			SMOOTHProcMember;
			SMOOTHVoid	*procParam;

			SMOOTHBool	*bVar;
			SMOOTHInt	*iVar;
			SMOOTHInt	 iCode;

			SMOOTHMenu	*popup;

			SMOOTHInt	 size;
			SMOOTHBool	 sizeset;
			SMOOTHRect	 rect;

					 Entry(SMOOTHInt, SMOOTHInt);
					~Entry();

			SMOOTHInt	 SetText(SMOOTHString);
			SMOOTHInt	 SetTooltip(SMOOTHString);
			SMOOTHInt	 SetStatusText(SMOOTHString);

			SMOOTHInt	 SetBitmap(HBITMAP);

			SMOOTHInt	 SetOrientation(SMOOTHInt);
			SMOOTHInt	 SetProc(SMOOTHProcParam, SMOOTHVoid *);
	};

	protected:
		SMOOTHInt		 nOfEntries;
		SMOOTHInt		 GetSizeY();
		SMOOTHInt		 GetSizeX();
		SMOOTHVoid		 GetMenuEntriesSize();
	public:
		SMOOTHBool		 sizeset;
		SMOOTHBool		 entrysizesset;
		SMOOTHArray<Entry *>	 entries;
		SMOOTHSize		 popupsize;

					 SMOOTHMenu();
					~SMOOTHMenu();

		Entry			*AddEntry(SMOOTHString text = NIL, HBITMAP bitmap = NIL, SMOOTHProcParam = NIL, SMOOTHVoid *procParam = NIL, SMOOTHMenu *popupMenu = NIL, SMOOTHBool *bVar = NIL, SMOOTHInt *iVar = NIL, SMOOTHInt iCode = 0, SMOOTHInt orientation = OR_LEFT);
		SMOOTHInt		 RemoveEntry(Entry *);
		SMOOTHInt		 RemoveEntry(SMOOTHInt);

		SMOOTHVoid		 GetSize();
		SMOOTHInt		 GetNOfEntries();

		SMOOTHInt		 Clear();
};

#endif
