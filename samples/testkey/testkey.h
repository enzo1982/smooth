 /* The smooth Class Library
  * Copyright (C) 1998-2014 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef	H_TESTKEY
#define H_TESTKEY

#include <smooth.h>

using namespace smooth;
using namespace smooth::GUI;

class TestKey : public Application
{
	private:
		GUI::Window	*wnd;
		Titlebar	*title;
		Text		*text;
	public:
				 TestKey();
				~TestKey();
	slots:
		Void		 EventProc(Int, Int, Int);
};

#endif
