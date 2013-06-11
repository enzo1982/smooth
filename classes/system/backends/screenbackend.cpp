 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/backends/screenbackend.h>

#if defined __WIN32__ && defined SMOOTH_STATIC
	#include <smooth/system/backends/win32/screenwin32.h>
#endif

S::System::ScreenBackend *CreateScreenBackend()
{
	return new S::System::ScreenBackend();
}

S::System::ScreenBackend *(*S::System::ScreenBackend::backend_creator)() = &CreateScreenBackend;

S::Int S::System::ScreenBackend::SetBackend(ScreenBackend *(*backend)())
{
	if (backend == NIL) return Error();

	backend_creator = backend;

	return Success();
}

S::System::ScreenBackend *S::System::ScreenBackend::CreateBackendInstance()
{
	return backend_creator();
}

S::System::ScreenBackend::ScreenBackend()
{
#if defined __WIN32__ && defined SMOOTH_STATIC
	volatile Bool	 null = 0;

	if (null) ScreenWin32();
#endif

	type = SCREEN_NONE;
}

S::System::ScreenBackend::~ScreenBackend()
{
}

S::Short S::System::ScreenBackend::GetScreenType() const
{
	return type;
}

S::GUI::Rect S::System::ScreenBackend::GetActiveScreenMetrics()
{
	return GUI::Rect();
}

S::GUI::Rect S::System::ScreenBackend::GetActiveScreenWorkArea()
{
	return GUI::Rect();
}

S::GUI::Rect S::System::ScreenBackend::GetVirtualScreenMetrics()
{
	return GUI::Rect();
}
