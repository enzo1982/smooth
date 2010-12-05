 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/event.h>
#include <smooth/system/system.h>
#include <smooth/system/backends/win32/eventwin32.h>

S::System::EventBackend *CreateEventWin32()
{
	return new S::System::EventWin32();
}

S::Int	 eventWin32Tmp = S::System::EventBackend::SetBackend(&CreateEventWin32);

S::System::EventWin32::EventWin32()
{
	type = EVENT_WIN32;
}

S::System::EventWin32::~EventWin32()
{
}

S::Int S::System::EventWin32::ProcessNextEvent()
{
	/* Emulate a timeout of ~100ms by trying to find a message
	 * 10 times while sleeping for 10ms between trying.
	 *
	 * We cannot use MsgWaitForMultipleObjects here as that
	 * function is not available on pre Windows 2000 systems.
	 */
	for (Int i = 0; i < 10; i++)
	{
		MSG	 msg;
		Bool	 result = False;

		if (Setup::enableUnicode) result = PeekMessageW(&msg, 0, 0, 0, PM_REMOVE);
		else			  result = PeekMessageA(&msg, 0, 0, 0, PM_REMOVE);

		if (result)
		{
			/* Process only the most recent WM_MOUSEMOVE message.
			 */
			if (msg.message == WM_MOUSEMOVE)
			{
				if (Setup::enableUnicode) while (PeekMessageW(&msg, msg.hwnd, WM_MOUSEMOVE, WM_MOUSEMOVE, PM_REMOVE)) { }
				else			  while (PeekMessageA(&msg, msg.hwnd, WM_MOUSEMOVE, WM_MOUSEMOVE, PM_REMOVE)) { }
			}

			/* Translate and dispatch message.
			 */
			TranslateMessage(&msg);

			if (Setup::enableUnicode) DispatchMessageW(&msg);
			else			  DispatchMessageA(&msg);

			if (msg.message == WM_QUIT) return Break;

			break;
		}

		S::System::System::Sleep(10);
	}

	return Success();
}
