 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_POPUPMENU_
#define _H_OBJSMOOTH_POPUPMENU_

#define SPopupMenu SMOOTHPopupMenu

#include "definitions.h"

const SMOOTHInt	 POPUP_NORMAL	= 0;
const SMOOTHInt	 POPUP_FINISHED	= 1;
const SMOOTHInt	 POPUP_PENDING	= 2;

class SMOOTHPopupMenu;
class SMOOTHWindow;
class SMOOTHToolWindow;
class SMOOTHPopupView;

#include "object.h"
#include "menu.h"

class SMOOTHAPI SMOOTHPopupMenu : public SMOOTHObject, public SMOOTHMenu
{
	friend class SMOOTHWindow;
	friend class SMOOTHPopupView;
	private:
		SMOOTHToolWindow	*toolwnd;
		SMOOTHPopupView		*popupView;

		SMOOTHPopupMenu		*prevPopup;
		SMOOTHPopupMenu		*nextPopup;
	public:
		static SMOOTHInt	 status;

					 SMOOTHPopupMenu();
					~SMOOTHPopupMenu();

		SMOOTHVoid		 MenuToPopup(SMOOTHMenu *);

		SMOOTHInt		 Process(SMOOTHInt, SMOOTHInt, SMOOTHInt);

		SMOOTHInt		 Show();
		SMOOTHInt		 Hide();
};

SMOOTHVAR SMOOTHInt OBJ_POPUP;

#endif
