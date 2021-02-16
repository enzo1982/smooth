 /* The smooth Class Library
  * Copyright (C) 1998-2021 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_CLIPBOARDXLIB
#define H_OBJSMOOTH_CLIPBOARDXLIB

namespace smooth
{
	namespace GUI
	{
		class Window;

		class ClipboardXLib;
	};
};

#include "../clipboardbackend.h"

#include <stdlib.h>

namespace X11
{
	extern "C"
	{
#		define True	1
#		define False	0
#		define Bool	int
#		define Success	0

#		include <X11/Xlib.h>
#		include <X11/Xatom.h>

#		undef True
#		undef False
#		undef Bool
#		undef Success
	}
};

namespace smooth
{
	namespace GUI
	{
		const Short	 CLIPBOARD_XLIB	= 2;

		class ClipboardXLib : public ClipboardBackend
		{
			private:
				X11::Display	*display;
				Window		*window;

				unsigned char	*QueryAtom(X11::Display *, X11::Window, X11::Atom, X11::Atom) const;

				String		 GetText(X11::Atom) const;
				Bool		 SetText(X11::Atom, const String &);
			public:
						 ClipboardXLib(Window *);
						~ClipboardXLib();

				String		 GetSelectionText() const;
				Bool		 SetSelectionText(const String &);

				String		 GetClipboardText() const;
				Bool		 SetClipboardText(const String &);
		};
	};
};

#endif
