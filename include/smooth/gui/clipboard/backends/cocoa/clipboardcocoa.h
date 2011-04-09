 /* The smooth Class Library
  * Copyright (C) 1998-2011 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_CLIPBOARDCOCOA
#define H_OBJSMOOTH_CLIPBOARDCOCOA

#import <Cocoa/Cocoa.h>

namespace smooth
{
	namespace GUI
	{
		class Window;

		class ClipboardCocoa;
	};
};

#include "../clipboardbackend.h"

namespace smooth
{
	namespace GUI
	{
		const Short	 CLIPBOARD_COCOA	= 3;

		class ClipboardCocoa : public ClipboardBackend
		{
			private:
				Window	*window;
			public:
					 ClipboardCocoa(Window *);
					~ClipboardCocoa();

				String	 GetClipboardText() const;
				Bool	 SetClipboardText(const String &);
		};
	};
};

#endif
