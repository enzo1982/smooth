 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/clipboard/backends/wayland/clipboardwayland.h>
#include <smooth/gui/window/backends/wayland/windowwayland.h>

S::GUI::ClipboardBackend *CreateClipboardWayland(S::GUI::Window *window)
{
	return new S::GUI::ClipboardWayland(window);
}

S::Int	 clipboardWaylandTmp = S::GUI::ClipboardBackend::SetBackend(&CreateClipboardWayland);

S::GUI::ClipboardWayland::ClipboardWayland(Window *iWindow)
{
	type = CLIPBOARD_WAYLAND;
}

S::GUI::ClipboardWayland::~ClipboardWayland()
{
}

S::String S::GUI::ClipboardWayland::GetSelectionText() const
{
	return WindowWayland::GetSelectionText();
}

S::Bool S::GUI::ClipboardWayland::SetSelectionText(const String &text)
{
	return WindowWayland::SetSelectionText(text);
}

S::String S::GUI::ClipboardWayland::GetClipboardText() const
{
	return WindowWayland::GetClipboardText();
}

S::Bool S::GUI::ClipboardWayland::SetClipboardText(const String &text)
{
	return WindowWayland::SetClipboardText(text);
}
