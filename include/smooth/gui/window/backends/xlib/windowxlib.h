 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_WINDOWXLIB
#define H_OBJSMOOTH_WINDOWXLIB

namespace smooth
{
	namespace GUI
	{
		class Cursor;

		class WindowXLib;
	};
};

#include "../windowbackend.h"
#include "../../../../graphics/forms/rect.h"
#include "../../../../input/keyboard.h"
#include "../../../../threads/mutex.h"

namespace X11
{
	extern "C"
	{
#		define True	1
#		define False	0
#		define Bool	int
#		define Success	0

#		include <X11/Xlib.h>
#		include <X11/Xproto.h>
#		include <X11/Xutil.h>
#		include <X11/Xatom.h>
#		include <X11/XKBlib.h>

#		undef True
#		undef False
#		undef Bool
#		undef Success

#		include <X11/keysym.h>
	}
};

namespace smooth
{
	namespace GUI
	{
		const Short	 WINDOW_XLIB	= 2;

		class WindowXLib : public WindowBackend
		{
			private:
				static Array<WindowXLib *, Void *>	 windowBackends;

				static Cursor				*activeCursor;
				static Point				 activeCursorPos;

				X11::Display				*display;
				X11::XIM				 im;

				WindowXLib				*FindLeaderWindow();

				Bool					 IsModalWindowActive();
			protected:
				Int					 id;

				Threads::Mutex				 mutex;

				X11::Window				 wnd;
				X11::Window				 oldwnd;

				X11::XIC				 ic;
				X11::Window				 iwnd;

				UnsignedLong				*sysIcon;
				Int					 sysIconSize;

				X11::Time				 xdndTimeStamp;
				Bool					 acceptDrop;
				Bool					 enableDropFiles;

				Float					 fontSize;

				Point					 pos;
				Size					 size;

				Size					 minSize;
				Size					 maxSize;

				Int					 flags;

				String					 selection;
				String					 clipboard;

				Void					 UpdateWMNormalHints();

				Input::Keyboard::Key			 ConvertKey(Int);
			public:
				static Int				 Initialize();
				static Int				 Free();

									 WindowXLib(Void * = NIL);
									~WindowXLib();

				Void					*GetSystemWindow() const;

				static WindowXLib			*GetWindowBackend(X11::Window);

				Int					 ProcessSystemMessages(X11::XEvent *);

				Int					 Open(const String &, const Point &, const Size &, Int);
				Int					 Close();

				Int					 RequestClose();

				Int					 SetTitle(const String &);

				Int					 SetIcon(const Bitmap &);

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

				Void					 OnXIMPreeditStart();
				Void					 OnXIMPreeditDone();
				Void					 OnXIMPreeditDraw(X11::XIMPreeditDrawCallbackStruct *);
				Void					 OnXIMPreeditCaret(X11::XIMPreeditCaretCallbackStruct *);
		};
	};
};

#endif
