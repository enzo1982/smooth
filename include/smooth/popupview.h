 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_POPUPVIEW_
#define _H_OBJSMOOTH_POPUPVIEW_

#define SPopupView SMOOTHPopupView

class SMOOTHPopupView;
class SMOOTHPopupMenu;

#include "object.h"

class SMOOTHAPI SMOOTHPopupView : public SMOOTHObject
{
	private:
		SMOOTHString		 backupStatusText;
	protected:
		SMOOTHPopupMenu		*myPopup;
	public:
					 SMOOTHPopupView(SMOOTHPopupMenu *);
					~SMOOTHPopupView();

		SMOOTHInt		 Process(SMOOTHInt, SMOOTHInt, SMOOTHInt);
		SMOOTHInt		 Paint(SMOOTHInt);
};

SMOOTHVAR SMOOTHInt OBJ_POPUPVIEW;

#endif
