 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/shortcut.h>
#include <smooth/window.h>

const S::Int	 S::GUI::Shortcut::classID = S::Object::RequestClassID();

S::GUI::Shortcut::Shortcut(Int iKey, Int iFlags, Int iParam)
{
	type	= classID;
	key	= iKey;
	flags	= iFlags;
	param	= iParam;

	possibleContainers.AddEntry(Window::classID);
}

S::GUI::Shortcut::~Shortcut()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::Shortcut::Process(Int message, Int param1, Int param2)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	Window	*wnd = myContainer->GetContainerWindow();

	if (wnd == NIL) return Success;

	Int	 retVal = Success;

	switch (message)
	{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			if (param1 == key)
			{
				BYTE	 state[256];

				GetKeyboardState(state);

				if ((((flags & SC_ALT) 	 && (param2 & 536870912))      || (!(flags & SC_ALT)   && !(param2 & 536870912)))      &&
				    (((flags & SC_CTRL)  && (state[VK_CONTROL] & 128)) || (!(flags & SC_CTRL)  && !(state[VK_CONTROL] & 128))) &&
				    (((flags & SC_SHIFT) && (state[VK_SHIFT] & 128))   || (!(flags & SC_SHIFT) && !(state[VK_SHIFT] & 128))))
				{
					onKeyDown.Emit(param);

					retVal = Break;
				}
			}

			break;
	}

	return retVal;
}

S::Int S::GUI::Shortcut::SetShortcut(Int nKey, Int nFlags, Int nParam)
{
	key	= nKey;
	flags	= nFlags;
	param	= nParam;

	return Success;
}
