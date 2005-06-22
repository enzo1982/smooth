 /* The smooth Class Library
  * Copyright (C) 1998-2005 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_TEST_
#define _H_TEST_

#include <smooth.h>

using namespace smooth;
using namespace smooth::GUI;

class Test : public Application
{
	private:
		Window		*mainWnd;
		Titlebar	*mainWnd_titlebar;
		Statusbar	*mainWnd_statusbar;

		Menubar		*mainWnd_menubar;

		Menu		*menu_file;
		Menu		*menu_dialogs;
	public:
				 Test();
				~Test();
};

#endif
