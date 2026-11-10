 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_CLIPBOARDWAYLAND
#define H_OBJSMOOTH_CLIPBOARDWAYLAND

namespace smooth
{
	namespace GUI
	{
		class ClipboardWayland;
	};
};

#include "../clipboardbackend.h"

namespace smooth
{
	namespace GUI
	{
		const Short	 CLIPBOARD_WAYLAND = 5;

		class ClipboardWayland : public ClipboardBackend
		{
			public:
					 ClipboardWayland(Window *);
					~ClipboardWayland();

				String	 GetSelectionText() const;
				Bool	 SetSelectionText(const String &);

				String	 GetClipboardText() const;
				Bool	 SetClipboardText(const String &);
		};
	};
};

#endif
