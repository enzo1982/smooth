 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_MDIWINDOW_
#define _H_OBJSMOOTH_MDIWINDOW_

#define SMDIWindow SMOOTHMDIWindow

class SMOOTHMDIWindow;

#include "window.h"

class SMOOTHAPI SMOOTHMDIWindow : public SMOOTHWindow
{
	public:
		 SMOOTHMDIWindow(SMOOTHString title = NIL);
		~SMOOTHMDIWindow();
};

SMOOTHVAR SMOOTHInt OBJ_MDIWINDOW;

#endif
