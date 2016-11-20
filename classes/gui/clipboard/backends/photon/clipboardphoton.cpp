 /* The smooth Class Library
  * Copyright (C) 1998-2016 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <Ph.h>

#include <smooth/gui/clipboard/backends/photon/clipboardphoton.h>
#include <smooth/gui/window/window.h>

S::GUI::ClipboardBackend *CreateClipboardPhoton(S::GUI::Window *window)
{
	return new S::GUI::ClipboardPhoton(window);
}

S::Int	 clipboardPhotonTmp = S::GUI::ClipboardBackend::SetBackend(&CreateClipboardPhoton);

S::GUI::ClipboardPhoton::ClipboardPhoton(Window *iWindow)
{
	type	= CLIPBOARD_PHOTON;

	window	= iWindow;
}

S::GUI::ClipboardPhoton::~ClipboardPhoton()
{
}

S::String S::GUI::ClipboardPhoton::GetClipboardText() const
{
	if (window == NIL) return NIL;

	String		 clipboardText;

	/* Get clipboard data.
	 */
   	int		 ig   = PhInputGroup(NIL);
	PhClipboardHdr	*clip = PhClipboardRead(ig, (char *) Ph_CLIPBOARD_TYPE_TEXT);

	if (clip != NIL)
	{
		Buffer<char>	 buffer(clip->length + 1);

		buffer.Zero();
		memcpy(buffer, clip->data, clip->length);

		clipboardText.ImportFrom("UTF-8", buffer);

		free(clip->data);
		free(clip);
	}

	return clipboardText;
}

S::Bool S::GUI::ClipboardPhoton::SetClipboardText(const String &text)
{
	if (window == NIL) return False;

	/* Set clipboard data.
	 */
	int		 ig	= PhInputGroup(NIL);
	char		*string = text.ConvertTo("UTF-8");
	PhClipboardHdr	 clip[1];

	strncpy(clip[0].type, Ph_CLIPBOARD_TYPE_TEXT, sizeof(clip[0].type));

	clip[0].data   = string;
	clip[0].length = strlen(string);

	PhClipboardWrite(ig, 1, clip);

	return True;
}
