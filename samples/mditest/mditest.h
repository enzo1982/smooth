 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_MDITEST_
#define _H_MDITEST_

#include <smooth.h>

using namespace smooth;

class MDITest : public Application
{
	private:
		Window		*mainWnd;
		Titlebar	*mainWnd_titlebar;
		Statusbar	*mainWnd_statusbar;
		MDIClient	*mainWnd_client;
		Menubar		*mainWnd_menubar;
		PopupMenu	*menu_file;
	public:
				 MDITest();
				~MDITest();
		Bool		 ExitProc();
		Void		 Close();
		Void		 NewMDI();
};

#endif
