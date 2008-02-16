 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef	_H_TESTKEY_
#define _H_TESTKEY_

#include <smooth.h>

using namespace smooth;
using namespace smooth::GUI;

class TestKey : public Application
{
	private:
		Window		*wnd;
		Titlebar	*title;
		Text		*text;

		Int		 GetBits(Int, UnsignedInt, UnsignedInt);
	public:
				 TestKey();
				~TestKey();
	slots:
		Void		 EventProc(Int, Int, Int);
};

#endif
