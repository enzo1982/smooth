 /* The smooth Class Library
  * Copyright (C) 1998-2011 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/clipboard/backends/win32/clipboardwin32.h>
#include <smooth/gui/window/window.h>

S::GUI::ClipboardBackend *CreateClipboardWin32(S::GUI::Window *window)
{
	return new S::GUI::ClipboardWin32(window);
}

S::Int	 clipboardWin32Tmp = S::GUI::ClipboardBackend::SetBackend(&CreateClipboardWin32);

S::GUI::ClipboardWin32::ClipboardWin32(Window *iWindow)
{
	type	= CLIPBOARD_WIN32;

	window	= iWindow;
}

S::GUI::ClipboardWin32::~ClipboardWin32()
{
}

S::String S::GUI::ClipboardWin32::GetClipboardText() const
{
	if (window == NIL) return NIL;

	String	 clipboardText;

	OpenClipboard((HWND) window->GetSystemWindow());

	if (Setup::enableUnicode && IsClipboardFormatAvailable(CF_UNICODETEXT))
	{
		clipboardText = (wchar_t *) GetClipboardData(CF_UNICODETEXT);
	}
	else if (IsClipboardFormatAvailable(CF_TEXT))
	{
		clipboardText = (char *) GetClipboardData(CF_TEXT);
	}

	CloseClipboard();

	return clipboardText;
}

S::Bool S::GUI::ClipboardWin32::SetClipboardText(const String &text)
{
	if (window == NIL) return False;

	OpenClipboard((HWND) window->GetSystemWindow());

	HGLOBAL	 memory = NIL;

	if (Setup::enableUnicode)
	{
		memory = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, sizeof(wchar_t) * (text.Length() + 1));

		memcpy(GlobalLock(memory), (wchar_t *) text, sizeof(wchar_t) * (text.Length() + 1));

		SetClipboardData(CF_UNICODETEXT, memory);
	}
	else
	{
		memory = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, sizeof(char) * (text.Length() + 1));

		memcpy(GlobalLock(memory), (char *) text, sizeof(char) * (text.Length() + 1));

		SetClipboardData(CF_TEXT, memory);
	}

	CloseClipboard();

	return True;
}
