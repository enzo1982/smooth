 /* The smooth Class Library
  * Copyright (C) 1998-2011 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/clipboard/backends/cocoa/clipboardcocoa.h>
#include <smooth/gui/window/window.h>

S::GUI::ClipboardBackend *CreateClipboardCocoa(S::GUI::Window *window)
{
	return new S::GUI::ClipboardCocoa(window);
}

S::Int	 clipboardCocoaTmp = S::GUI::ClipboardBackend::SetBackend(&CreateClipboardCocoa);

S::GUI::ClipboardCocoa::ClipboardCocoa(Window *iWindow)
{
	type	= CLIPBOARD_COCOA;

	window	= iWindow;
}

S::GUI::ClipboardCocoa::~ClipboardCocoa()
{
}

S::String S::GUI::ClipboardCocoa::GetClipboardText() const
{
	if (window == NIL) return NIL;

	NSAutoreleasePool	*pool = [[NSAutoreleasePool alloc] init];

	String		 clipboardText;
	NSString	*clipboardString = [[NSPasteboard generalPasteboard] stringForType: NSStringPboardType];

	if (clipboardString != NIL) clipboardText.ImportFrom("UTF-8", [clipboardString UTF8String]);

	[pool release];

	return clipboardText;
}

S::Bool S::GUI::ClipboardCocoa::SetClipboardText(const String &text)
{
	if (window == NIL) return False;

	NSAutoreleasePool	*pool = [[NSAutoreleasePool alloc] init];

	[[NSPasteboard generalPasteboard] declareTypes: [NSArray arrayWithObject: NSStringPboardType] owner: nil];
	[[NSPasteboard generalPasteboard] setString: [NSString stringWithUTF8String: text.ConvertTo("UTF-8")] forType: NSStringPboardType];

	[pool release];

	return True;
}
