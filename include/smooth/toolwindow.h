 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_TOOLWINDOW_
#define _H_OBJSMOOTH_TOOLWINDOW_

#define SToolWindow SMOOTHToolWindow

class SMOOTHToolWindow;
class SMOOTHObject;

#include "definitions.h"
#include "window.h"

class SMOOTHAPI SMOOTHToolWindow : public SMOOTHWindow
{
	private:
		SMOOTHObject	*owner;
	public:
				 SMOOTHToolWindow();
				~SMOOTHToolWindow();

		SMOOTHInt	 SetOwner(SMOOTHObject *);
		SMOOTHInt	 FreeOwner();

		HWND		 Create();

		SMOOTHInt	 Paint(SMOOTHInt);
		SMOOTHInt	 Process(SMOOTHInt, SMOOTHInt, SMOOTHInt);

		SMOOTHInt	 RegisterObject(SMOOTHObject *);

		SMOOTHBool	 IsTypeCompatible(SMOOTHInt);
};

SMOOTHVAR SMOOTHInt OBJ_TOOLWINDOW;

#endif
