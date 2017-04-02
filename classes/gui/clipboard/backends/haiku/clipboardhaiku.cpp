 /* The smooth Class Library
  * Copyright (C) 1998-2017 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/clipboard/backends/haiku/clipboardhaiku.h>
#include <smooth/gui/window/window.h>

#include <Clipboard.h>

S::GUI::ClipboardBackend *CreateClipboardHaiku(S::GUI::Window *window)
{
	return new S::GUI::ClipboardHaiku(window);
}

S::Int	 clipboardHaikuTmp = S::GUI::ClipboardBackend::SetBackend(&CreateClipboardHaiku);

S::GUI::ClipboardHaiku::ClipboardHaiku(Window *iWindow)
{
	type	= CLIPBOARD_HAIKU;

	window	= iWindow;
}

S::GUI::ClipboardHaiku::~ClipboardHaiku()
{
}

S::String S::GUI::ClipboardHaiku::GetClipboardText() const
{
	if (window == NIL) return NIL;

	String	 clipboardText;

	if (be_clipboard->Lock())
	{
		BMessage	*clip	= be_clipboard->Data();
		const void	*data	= NIL;
		ssize_t		 length = 0;

		if (clip->FindData("text/plain", B_MIME_TYPE, &data, &length) == B_OK)
		{
			Buffer<char>	 buffer(length + 1);

			buffer.Zero();
			memcpy(buffer, data, length);

			clipboardText.ImportFrom("UTF-8", buffer);
		}

		be_clipboard->Unlock();
	}

	return clipboardText;
}

S::Bool S::GUI::ClipboardHaiku::SetClipboardText(const String &text)
{
	if (window == NIL) return False;

	if (be_clipboard->Lock())
	{
		be_clipboard->Clear();

		BMessage	*clip	= be_clipboard->Data();
		const char	*string = text.ConvertTo("UTF-8");

		if (clip->AddData("text/plain", B_MIME_TYPE, string, strlen(string)) == B_OK)
		{
			be_clipboard->Commit();
		}

		be_clipboard->Unlock();
	}

	return True;
}
