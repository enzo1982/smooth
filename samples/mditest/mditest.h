 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_MDITEST_
#define _H_MDITEST_

#include <smoothx.h>

class MDITest : public SMOOTHApplication
{
	private:
		SMOOTHWindow		*mainWnd;
		SMOOTHTitlebar		*mainWnd_titlebar;
		SMOOTHStatusbar		*mainWnd_statusbar;
		SMOOTHMDIClient		*mainWnd_client;
		SMOOTHMenubar		*mainWnd_menubar;
		SMOOTHPopupMenu		*menu_file;
	public:
					 MDITest();
					~MDITest();
		SMOOTHVoid		 QuitProc();
		SMOOTHBool		 KillProc();
		SMOOTHVoid		 NewMDI();
};

#endif
