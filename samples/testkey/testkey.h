 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef	_H_TESTKEY_
#define _H_TESTKEY_

#include <smoothx.h>

class TestKey : public SApplication
{
	private:
		SWindow		*wnd;
		SLayer		*layer;
		STitlebar	*title;
		SText		*text;

		SVoid		 MessageProc(SInt, SInt, SInt);
		SInt		 GetBits(SInt, SUInt, SUInt);
	public:
				 TestKey();
				~TestKey();
};

#endif
