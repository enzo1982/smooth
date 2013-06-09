 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/screen.h>
#include <smooth/system/backends/screenbackend.h>
#include <smooth/init.h>

S::Int	 addScreenInitTmp = S::AddInitFunction(&S::System::Screen::Initialize);
S::Int	 addScreenFreeTmp = S::AddFreeFunction(&S::System::Screen::Free);

S::System::ScreenBackend	*S::System::Screen::backend = NIL;

S::System::Screen::Screen()
{
}

S::System::Screen::Screen(const Screen &)
{
}

S::Int S::System::Screen::Initialize()
{
	backend = ScreenBackend::CreateBackendInstance();

	return Success();
}

S::Int S::System::Screen::Free()
{
	delete backend;

	return Success();
}

S::GUI::Rect S::System::Screen::GetActiveScreenMetrics()
{
	return backend->GetActiveScreenMetrics();
}

S::GUI::Rect S::System::Screen::GetActiveScreenWorkArea()
{
	return backend->GetActiveScreenWorkArea();
}

S::GUI::Rect S::System::Screen::GetVirtualScreenMetrics()
{
	return backend->GetVirtualScreenMetrics();
}
