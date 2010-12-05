 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/clipboard/backends/xlib/clipboardxlib.h>
#include <smooth/gui/window/backends/xlib/windowxlib.h>
#include <smooth/backends/xlib/backendxlib.h>
#include <smooth/gui/window/window.h>

using namespace X11;

S::GUI::ClipboardBackend *CreateClipboardXLib(S::GUI::Window *window)
{
	return new S::GUI::ClipboardXLib(window);
}

S::Int	 clipboardXLibTmp = S::GUI::ClipboardBackend::SetBackend(&CreateClipboardXLib);

S::GUI::ClipboardXLib::ClipboardXLib(Window *iWindow)
{
	type	= CLIPBOARD_XLIB;

	window	= iWindow;
}

S::GUI::ClipboardXLib::~ClipboardXLib()
{
}

unsigned char *S::GUI::ClipboardXLib::QueryAtom(Display *display, ::Window self, Atom atom) const
{
	XConvertSelection(display, XA_PRIMARY, atom, atom, self, CurrentTime);
	XFlush(display);

	/* Wait for SelectionNotify event to be sent.
	 */
	XEvent	 e;

	do
	{
		XNextEvent(display, &e);

		WindowXLib	*backend = GUI::WindowXLib::GetWindowBackend(e.xany.window);

		if (backend != NIL) backend->ProcessSystemMessages(&e);
	}
	while (e.type != SelectionNotify);

	Atom		 type;
	int		 format;
	unsigned long	 items, bytes;
	unsigned char	*data = NIL;

	/* Do not get any data, see how much data is there.
	 */
	XGetWindowProperty(display, self, atom, 0, 0, 0, AnyPropertyType, &type, &format, &items, &bytes, &data);				

	/* Data is there!
	 */
	if (bytes > 0) XGetWindowProperty(display, self, atom, 0, bytes, 0, AnyPropertyType, &type, &format, &items, &bytes, &data);

	return data;
}

S::String S::GUI::ClipboardXLib::GetClipboardText() const
{
	if (window == NIL) return NIL;

	String		 clipboardText;

	Display		*display = Backends::BackendXLib::GetDisplay();
	::Window	 self	 = (::Window) window->GetSystemWindow();
	::Window	 owner	 = XGetSelectionOwner(display, XA_PRIMARY);

	if (owner != None)
	{
		unsigned char *data = NIL;

		data = QueryAtom(display, self, XA_UTF8_STRING(display));

		if (data != NIL)
		{
			clipboardText.ImportFrom("UTF-8", (char *) data);

			XFree(data);
		}
		else
		{
			data = QueryAtom(display, self, XA_STRING);

			if (data != NIL)
			{
				clipboardText = (char *) data;

				XFree(data);
			}
		}
	}

	return clipboardText;
}

S::Bool S::GUI::ClipboardXLib::SetClipboardText(const String &text)
{
	if (window == NIL) return False;

	WindowXLib	*backend = GUI::WindowXLib::GetWindowBackend((::Window) window->GetSystemWindow());

	if (backend != NIL) backend->SetSelection(text);

	Display		*display = Backends::BackendXLib::GetDisplay();
	::Window	 self	 = (::Window) window->GetSystemWindow();

	XSetSelectionOwner(display, XA_PRIMARY, self, CurrentTime);
	XSetSelectionOwner(display, XA_CLIPBOARD(display), self, CurrentTime);
	XFlush(display);

	return True;
}
