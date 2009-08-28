 /* The smooth Class Library
  * Copyright (C) 1998-2009 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/menu/menuentry.h>
#include <smooth/gui/widgets/multi/menu/popupmenu.h>
#include <smooth/gui/widgets/special/shortcut.h>
#include <smooth/gui/window/window.h>

const S::Int	 S::GUI::MenuEntry::classID = S::Object::RequestClassID();

S::GUI::MenuEntry::MenuEntry(const String &iText, const Bitmap &iBitmap, PopupMenu *iPopup, Bool *ibVar, Int *iiVar, Int iiCode) : Widget(Point(), Size())
{
	type		= classID;

	popup		= iPopup;
	bVar		= ibVar;
	iVar		= iiVar;
	iCode		= iiCode;
	orientation	= OR_LEFT;

	SetText(iText);
	SetBitmap(iBitmap);

	shortcut	= NIL;

	if (popup != NIL)
	{
		popup->onEnqueueForDeletion.Connect(&MenuEntry::InternalOnDeletePopup, this);
	}
}

S::GUI::MenuEntry::~MenuEntry()
{
	if (shortcut != NIL) DeleteObject(shortcut);

	if (popup != NIL) popup->onEnqueueForDeletion.Disconnect(&MenuEntry::InternalOnDeletePopup, this);
}

S::Void S::GUI::MenuEntry::InternalOnDeletePopup()
{
	popup = NIL;
}

S::Int S::GUI::MenuEntry::SetShortcut(Int scFlags, Int scKey, Window *window)
{
	shortcut = new Shortcut(scFlags, scKey, this);
	shortcut->onKeyDown.Connect(&onAction);

	window->Add(shortcut);

	return Success();
}

S::Int S::GUI::MenuEntry::SetBitmap(const Bitmap &newBitmap)
{
	bitmap	= newBitmap;
	graymap	= newBitmap;

	if (bitmap == NIL) return Success();

	bitmap.ReplaceColor(Color(192, 192, 192), Setup::BackgroundColor);

	graymap.ReplaceColor(Color(192, 192, 192), Setup::BackgroundColor);
	graymap.GrayscaleBitmap();

	return Success();
}

const S::GUI::Bitmap &S::GUI::MenuEntry::GetBitmap() const
{
	return bitmap;
}
