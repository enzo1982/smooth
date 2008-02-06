 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/dialogs/dialog.h>
#include <smooth/errors/success.h>

S::GUI::Dialogs::Dialog::Dialog()
{
	parentWindow = NIL;

	error = Success();
}

S::GUI::Dialogs::Dialog::~Dialog()
{
}

S::Int S::GUI::Dialogs::Dialog::SetCaption(const String &newCaption)
{
	caption = newCaption;

	return Success();
}

S::Int S::GUI::Dialogs::Dialog::SetParentWindow(Window *newParent)
{
	parentWindow = newParent;

	return Success();
}

const Error &S::GUI::Dialogs::Dialog::GetErrorStatus() const
{
	return error;
}
