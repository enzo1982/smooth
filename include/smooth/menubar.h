 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_MENUBAR_
#define _H_OBJSMOOTH_MENUBAR_

#define SMenubar SMOOTHMenubar

class SMOOTHMenubar;

#include "object.h"
#include "menu.h"

const SMOOTHInt	 MB_GRAYSCALE	= 0;
const SMOOTHInt	 MB_COLOR	= 1;

class SMOOTHAPI SMOOTHMenubar : public SMOOTHObject, public SMOOTHMenu
{
	private:
		SMOOTHInt		 popupHandle;
		SMOOTHInt		 style;

		SMOOTHString		 backupStatusText;
	public:
					 SMOOTHMenubar();
					~SMOOTHMenubar();

		virtual SMOOTHInt	 Paint(SMOOTHInt);
		SMOOTHInt		 Process(SMOOTHInt, SMOOTHInt, SMOOTHInt);

		SMOOTHInt		 SetStyle(SMOOTHInt);
};

SMOOTHVAR SMOOTHInt OBJ_MENUBAR;

#endif
