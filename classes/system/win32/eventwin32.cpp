 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/event.h>
#include <smooth/system/win32/eventwin32.h>

S::System::EventBackend *CreateEventWin32()
{
	return new S::System::EventWin32();
}

S::Int	 eventWin32Tmp = S::System::EventBackend::AddBackend(&CreateEventWin32);

S::System::EventWin32::EventWin32()
{
	type = EVENT_WIN32;
}

S::System::EventWin32::~EventWin32()
{
}

S::Int S::System::EventWin32::ProcessNextEvent(Bool block)
{
	MSG	 msg;
	Bool	 result = False;

	if (block)
	{
		if (Setup::enableUnicode)	result = GetMessageW(&msg, NIL, 0, 0);
		else				result = GetMessageA(&msg, NIL, 0, 0);

		TranslateMessage(&msg);

		if (Setup::enableUnicode)	DispatchMessageW(&msg);
		else				DispatchMessageA(&msg);

		if (!result) return Break;
	}
	else
	{
		if (Setup::enableUnicode)	result = PeekMessageW(&msg, 0, 0, 0, PM_REMOVE);
		else				result = PeekMessageA(&msg, 0, 0, 0, PM_REMOVE);

		if (Setup::enableUnicode)	PostMessageW(NIL, SM_EXECUTEPEEK, 0, 0);
		else				PostMessageA(NIL, SM_EXECUTEPEEK, 0, 0);

		if (result)
		{
			TranslateMessage(&msg);

			if (Setup::enableUnicode)	DispatchMessageW(&msg);
			else				DispatchMessageA(&msg);

			if (msg.message == WM_QUIT) return Break;
		}
	}

	return Success;
}
