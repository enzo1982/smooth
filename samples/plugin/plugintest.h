 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_PLUGINTEST_
#define _H_PLUGINTEST_

#include <smooth.h>
#include "plugin.h"

using namespace smooth;
using namespace smooth::GUI;

class PlugInTest : public Application
{
	private:
		Window			*wnd;
		Layer			*layer;
		Titlebar		*title;
		ActiveAreaPlugin	*activearea;
	public:
					 PlugInTest();
					~PlugInTest();
		Void			 activeAreaProc();
};

#endif
