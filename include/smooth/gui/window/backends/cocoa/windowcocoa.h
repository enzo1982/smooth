 /* The smooth Class Library
  * Copyright (C) 1998-2014 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_WINDOWCOCOA
#define H_OBJSMOOTH_WINDOWCOCOA

#include <Cocoa/Cocoa.h>

namespace smooth
{
	namespace GUI
	{
		class Cursor;

		class WindowCocoa;
	};
};

#include "../windowbackend.h"
#include "../../../../graphics/forms/rect.h"
#include "../../../../input/keyboard.h"

namespace smooth
{
	namespace GUI
	{
		const Short	 WINDOW_COCOA	= 3;

		class WindowCocoa : public WindowBackend
		{
			private:
				static Array<WindowCocoa *, Void *>	 windowBackends;
			protected:
				Int					 wid;

				NSWindow				*wnd;

				NSPasteboard				*pasteBoard;
				Bool					 enableDropFiles;

				Float					 fontSize;

				Size					 minSize;
				Size					 maxSize;

				Int					 flags;

				String					 selection;
				String					 clipboard;

				Input::Keyboard::Key			 ConvertKey(Int);
			public:
				static Int				 Initialize();
				static Int				 Free();

									 WindowCocoa(Void * = NIL);
									~WindowCocoa();

				Void					*GetSystemWindow() const;

				static WindowCocoa			*GetWindowBackend(NSWindow *);

				Int					 ProcessSystemMessages(NSEvent *);

				Int					 Open(const String &, const Point &, const Size &, Int);
				Int					 Close();

				Int					 RequestClose();

				Int					 SetTitle(const String &);

				Int					 EnableDropFiles(Bool);
				const Array<String>			&GetDroppedFiles() const;

				Int					 SetMinimumSize(const Size &);
				Int					 SetMaximumSize(const Size &);

				Int					 Show();
				Int					 Hide();

				Int					 SetMetrics(const Point &, const Size &);

				Int					 Raise();
			accessors:
				Void					 SetSelection(const String &nSelection) { selection = nSelection; }
				Void					 SetClipboard(const String &nClipboard) { clipboard = nClipboard; }
			slots:
				static Void				 SetCursor(Cursor *, const Point &);
				static Void				 RemoveCursor(Cursor *);
		};
	};
};

#endif
