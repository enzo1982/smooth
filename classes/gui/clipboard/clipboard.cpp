 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/clipboard/clipboard.h>
#include <smooth/gui/clipboard/backends/clipboardbackend.h>

S::GUI::Clipboard::Clipboard(Window *window)
{
	backend = ClipboardBackend::CreateBackendInstance(window);
}

S::GUI::Clipboard::~Clipboard()
{
	delete backend;
}

S::String S::GUI::Clipboard::GetClipboardText() const
{
	return 	backend->GetClipboardText();
}

S::Bool S::GUI::Clipboard::SetClipboardText(const String &text)
{
	return backend->SetClipboardText(text);
}
