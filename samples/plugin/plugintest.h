 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_PLUGINTEST_
#define _H_PLUGINTEST_

#include <smoothx.h>
#include "plugin.h"

class PlugInTest : public SApplication
{
	private:
		SWindow			*wnd;
		SLayer			*layer;
		STitlebar		*title;
		SActiveAreaPlugin	*activearea;
	public:
					 PlugInTest();
					~PlugInTest();
		SVoid			 activeAreaProc();
};

#endif
